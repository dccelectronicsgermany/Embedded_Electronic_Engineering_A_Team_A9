
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#define NUM_LANES     4
#define NUM_CZ        4      /* CZ1=SW  CZ2=SE  CZ3=NW  CZ4=NE */
#define NUM_SLOTS     64
#define NO_VEH       -1
#define T_EARLY       3      /* earliest arrival -> FAST   */
#define T_NORMAL      6      /* normal arrival   -> NORMAL */
#define T_LATE        12     /* latest arrival   -> SLOW   */
#define SIM_MS        300    /* ms per tick */
#define NUM_CROSSERS  8
#define NO_CZ         255    /* zone slot unused */
#define STATS_PRINT_EVERY 10   /* print statistics every 10 simulation ticks */
#define SIM_END_TICK 150

#define POLICY_FIFO              0
#define POLICY_SHORTEST_ROUTE    1
#define POLICY_LONGEST_ROUTE     2

#define SCHED_POLICY POLICY_LONGEST_ROUTE
#define ALLOW_SPEED_UP 0

typedef enum { NORTH, SOUTH, EAST, WEST } Lane;
typedef enum { STRAIGHT, TURN_RIGHT, TURN_LEFT } Intent;
typedef enum { SPEED_FAST, SPEED_NORMAL, SPEED_SLOW, SPEED_STOP } SpeedMode;
typedef struct {
    uint8_t id; Lane lane; Intent intent; bool emergency;
    uint8_t czA, czB, czC, approach;   /* up to 3 zones; NO_CZ = unused */
    uint32_t entry, slack; SpeedMode speed;
} Vehicle;


static const uint8_t CZ_MAP[NUM_LANES][3][3] = {
    /* NORTH */ { {2,0,NO_CZ}, {2,NO_CZ,NO_CZ}, {2,0,1}    },  /* S=CZ3->CZ1, R=CZ3, L=CZ3->CZ1->CZ2 */
    /* SOUTH */ { {1,3,NO_CZ}, {1,NO_CZ,NO_CZ}, {1,3,2}    },  /* S=CZ2->CZ4, R=CZ2, L=CZ2->CZ4->CZ3 */
    /* EAST  */ { {3,2,NO_CZ}, {3,2,0},          {3,NO_CZ,NO_CZ} }, /* S=CZ4->CZ3, R=CZ4->CZ3->CZ1, L=CZ4 */
    /* WEST  */ { {0,1,NO_CZ}, {0,1,3},          {0,NO_CZ,NO_CZ} }, /* S=CZ1->CZ2, R=CZ1->CZ2->CZ4, L=CZ1 */
};

/* destination compass direction (for printing) [lane][intent] */
static const uint8_t DEST_MAP[NUM_LANES][3] = {
    { SOUTH,WEST,EAST }, { NORTH,EAST,WEST }, { WEST,SOUTH,NORTH }, { EAST,NORTH,SOUTH },
};
static const char *LANE_STR[]  = { "NORTH","SOUTH","EAST","WEST" };
static const char *SPEED_STR[] = { "FAST","NORMAL","SLOW","STOP" };

/* shared state, protected by xMutex */
static int8_t   czTable[NUM_CZ][NUM_SLOTS];
static uint32_t gTick  = 0;
static uint8_t  nextID = 0;

static uint32_t totalDetected  = 0;
static uint32_t totalScheduled = 0;
static uint32_t totalFailed    = 0;
static uint32_t totalCrossed   = 0;

static FILE *statsFile = NULL;
static unsigned long runId = 0;

static QueueHandle_t laneQ[NUM_LANES], crossQ;
static SemaphoreHandle_t xMutex;

static void czLabel(const Vehicle *v, char *buf, size_t n) {
    if      (v->czB == NO_CZ) snprintf(buf, n, "CZ%d only", v->czA+1);
    else if (v->czC == NO_CZ) snprintf(buf, n, "CZ%d->CZ%d", v->czA+1, v->czB+1);
    else                      snprintf(buf, n, "CZ%d->CZ%d->CZ%d", v->czA+1, v->czB+1, v->czC+1);
}

static uint8_t zoneCount(const Vehicle *v) {
    if (v->czB == NO_CZ) return 1;
    if (v->czC == NO_CZ) return 2;
    return 3;
}

static bool hasHigherPriority(const Vehicle *candidate, const Vehicle *current) {
    uint8_t candidateZones = zoneCount(candidate);
    uint8_t currentZones   = zoneCount(current);

#if SCHED_POLICY == POLICY_SHORTEST_ROUTE
    if (candidateZones != currentZones) {
        return candidateZones < currentZones;
    }
#elif SCHED_POLICY == POLICY_LONGEST_ROUTE
    if (candidateZones != currentZones) {
        return candidateZones > currentZones;
    }
#endif

    /* FIFO tie-breaker */
    return candidate->id < current->id;
}

static void initStatsFile(void) {
    statsFile = fopen("simulation_results.csv", "a");

    if (statsFile == NULL) {
        printf("[WARN]   Could not open simulation_results.csv for writing.\n");
        fflush(stdout);
        return;
    }

    fseek(statsFile, 0, SEEK_END);

    if (ftell(statsFile) == 0) {
        fprintf(statsFile,
                "run_id,tick,T_NORMAL,T_LATE,NUM_CROSSERS,NUM_SLOTS,SIM_MS,"
                "detected,scheduled,crossed,pending,failed,sched_success_percent,failure_percent\n");
        fflush(statsFile);
    }
}

static void printStats(void) {
    uint32_t resolved = totalScheduled + totalFailed;
    uint32_t pending = totalScheduled - totalCrossed;

    uint32_t successRate = 0;
    uint32_t failureRate = 0;

    if (resolved > 0) {
        successRate = (totalScheduled * 100U) / resolved;
        failureRate = (totalFailed * 100U) / resolved;
    }

    printf("[STATS]  t=%-3lu  detected=%-3lu  scheduled=%-3lu  crossed=%-3lu  pending=%-3lu  failed=%-3lu  sched_success=%lu%%  failure=%lu%%\n",
        (unsigned long)gTick,
        (unsigned long)totalDetected,
        (unsigned long)totalScheduled,
        (unsigned long)totalCrossed,
        (unsigned long)pending,
        (unsigned long)totalFailed,
        (unsigned long)successRate,
        (unsigned long)failureRate);
    fflush(stdout); 

    if (statsFile != NULL) {
        fprintf(statsFile,
                "%lu,%lu,%d,%d,%d,%d,%d,%lu,%lu,%lu,%lu,%lu,%lu,%lu\n",
                runId,
                (unsigned long)gTick,
                T_NORMAL,
                T_LATE,
                NUM_CROSSERS,
                NUM_SLOTS,
                SIM_MS,
                (unsigned long)totalDetected,
                (unsigned long)totalScheduled,
                (unsigned long)totalCrossed,
                (unsigned long)pending,
                (unsigned long)totalFailed,
                (unsigned long)successRate,
                (unsigned long)failureRate);
        fflush(statsFile);
    }
}

static bool trySchedule(Vehicle *v, uint32_t *outEntry) {
    uint8_t zones[3] = { v->czA, v->czB, v->czC };
    for (uint32_t t = gTick + T_NORMAL; t <= gTick + T_LATE; t++) {
        bool ok = true;
        uint32_t cur = t;
        for (int z = 0; z < 3 && ok; z++) {
            if (zones[z] == NO_CZ) break;
            for (int d = 0; d < 2; d++)
                if (czTable[zones[z]][(cur+d) % NUM_SLOTS] != NO_VEH) { ok = false; break; }
            cur += 2;
        }
        if (!ok) continue;
        cur = t;
        for (int z = 0; z < 3; z++) {
            if (zones[z] == NO_CZ) break;
            for (int d = 0; d < 2; d++)
                czTable[zones[z]][(cur+d) % NUM_SLOTS] = (int8_t)v->id;
            cur += 2;
        }
        *outEntry = t;
        return true;
    }
    return false;
}

/* free every slot owned by this vehicle */
static void releaseSlots(uint8_t id) {
    for (int c = 0; c < NUM_CZ; c++)
        for (int s = 0; s < NUM_SLOTS; s++)
            if (czTable[c][s] == (int8_t)id) czTable[c][s] = NO_VEH;
}

/* arrival task — one per lane, generates cars (REQ-2,3,4) */
static void vArrivalTask(void *arg) {
    Lane lane = (Lane)(intptr_t)arg;
    vTaskDelay(pdMS_TO_TICKS(200 + lane * 150));
    for (;;) {
        Vehicle v;
        memset(&v, 0, sizeof v);
        xSemaphoreTake(xMutex, portMAX_DELAY);
        v.id = nextID++;
        totalDetected++;
        xSemaphoreGive(xMutex);
        v.lane      = lane;
        v.intent    = (Intent)(rand() % 3);
        v.emergency = false;
        v.czA       = CZ_MAP[lane][v.intent][0];
        v.czB       = CZ_MAP[lane][v.intent][1];
        v.czC       = CZ_MAP[lane][v.intent][2];
        char cz[24]; czLabel(&v, cz, sizeof cz);
        printf("[DETECT] Car#%-2d  lane=%-5s  route=%-5s  CZ:[%s]%s\n",
               v.id, LANE_STR[lane], LANE_STR[DEST_MAP[lane][v.intent]],
               cz, v.emergency ? "  *** EMERGENCY ***" : "");
        fflush(stdout);
        xQueueSend(laneQ[lane], &v, portMAX_DELAY);
        vTaskDelay(pdMS_TO_TICKS(700 + rand() % 900));
    }
}

/* controller — non-blocking scheduler with priority (REQ-5..12) */
static void vControllerTask(void *arg) {
    (void)arg;
    printf("[CTRL]  Controller running.\n"); fflush(stdout);
    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(SIM_MS));
        gTick++;
        Vehicle toCross[NUM_LANES]; int nCross = 0;
        xSemaphoreTake(xMutex, portMAX_DELAY);

        Vehicle cands[NUM_LANES]; bool hasCand[NUM_LANES];
        for (int i = 0; i < NUM_LANES; i++)
            hasCand[i] = (xQueuePeek(laneQ[i], &cands[i], 0) == pdTRUE);

        int order[NUM_LANES] = { 0, 1, 2, 3 };

        for (int i = 0; i < NUM_LANES - 1; i++) {
            for (int j = i + 1; j < NUM_LANES; j++) {
                int a = order[i];
                int b = order[j];
                bool sw = false;

                if (!hasCand[a] && hasCand[b]) {
                    sw = true;
                } else if (hasCand[a] && hasCand[b]) {
                    if (hasHigherPriority(&cands[b], &cands[a])) {
                        sw = true;
                    }
                }

                if (sw) {
                    int t = order[i];
                    order[i] = order[j];
                    order[j] = t;
                }
            }
        }

        for (int k = 0; k < NUM_LANES; k++) {
            int lane = order[k];
            if (!hasCand[lane]) continue;
            Vehicle v = cands[lane]; uint32_t entry = 0;
            if (trySchedule(&v, &entry)) {
                Vehicle popped; xQueueReceive(laneQ[lane], &popped, 0);
                v.entry = entry; v.approach = (uint8_t)(entry - gTick);
                v.slack = (gTick + T_LATE) - entry;
                if (entry <= gTick + T_NORMAL) {
                    v.speed = SPEED_NORMAL;
                } else {
                    v.speed = SPEED_SLOW;
                }
                char cz[24]; czLabel(&v, cz, sizeof cz);
                totalScheduled++;

                printf("[SCHED]  Car#%-2d  %s  speed=%s\n", v.id, cz, SPEED_STR[v.speed]);
                fflush(stdout);
                toCross[nCross++] = v;
            }  else {
                Vehicle failed;

                if (xQueueReceive(laneQ[lane], &failed, 0) == pdTRUE) {
                    char cz[24];
                    czLabel(&failed, cz, sizeof cz);

                    totalFailed++;

                    printf("[FAIL]   Car#%-2d  %s  no valid reservation inside feasible arrival window.\n",
                        failed.id, cz);
                    fflush(stdout);
                }
            }
        }
        if ((gTick % STATS_PRINT_EVERY) == 0) {
            printStats();
        }

        if (gTick >= SIM_END_TICK) {
            if ((gTick % STATS_PRINT_EVERY) != 0) {
                printStats();
            }

            if (statsFile != NULL) {
                fclose(statsFile);
                statsFile = NULL;
            }

            xSemaphoreGive(xMutex);

            printf("[DONE]   Simulation finished at t=%lu.\n", (unsigned long)gTick);
            fflush(stdout);

            exit(0);
        }

        xSemaphoreGive(xMutex);

        for (int i = 0; i < nCross; i++){
            xQueueSend(crossQ, &toCross[i], portMAX_DELAY);
        }
    }
    
}

/* crosser — independent concurrent vehicle journey (REQ-9,10) */
static void vCrosserTask(void *arg) {
    (void)arg;
    for (;;) {
        Vehicle v;
        if (xQueueReceive(crossQ, &v, portMAX_DELAY) != pdTRUE) continue;
        vTaskDelay(pdMS_TO_TICKS(v.approach * SIM_MS));
        char cz[24]; czLabel(&v, cz, sizeof cz);
        xSemaphoreTake(xMutex, portMAX_DELAY);
        printf("[CROSS]  Car#%-2d  entering %s  (speed=%s)\n", v.id, cz, SPEED_STR[v.speed]);
        fflush(stdout); xSemaphoreGive(xMutex);
        int crossTicks = (v.czC != NO_CZ) ? 6 : (v.czB != NO_CZ) ? 4 : 2;
        vTaskDelay(pdMS_TO_TICKS(crossTicks * SIM_MS));
        xSemaphoreTake(xMutex, portMAX_DELAY);
        releaseSlots(v.id);
        totalCrossed++;

        printf("[EXIT]   Car#%-2d  crossed; zones released.\n", v.id);
        fflush(stdout);
        xSemaphoreGive(xMutex);
    }
}

int main(void) {
    runId = (unsigned long)time(NULL);
    srand((unsigned)runId);

    initStatsFile();

    memset(czTable, NO_VEH, sizeof czTable);
    xMutex = xSemaphoreCreateMutex();
    crossQ = xQueueCreate(32, sizeof(Vehicle));
    for (int i = 0; i < NUM_LANES; i++) laneQ[i] = xQueueCreate(8, sizeof(Vehicle));
    for (int i = 0; i < NUM_LANES; i++)
        xTaskCreate(vArrivalTask, "Arr", configMINIMAL_STACK_SIZE*4, (void*)(intptr_t)i, 2, NULL);
    xTaskCreate(vControllerTask, "Ctrl", configMINIMAL_STACK_SIZE*8, NULL, 4, NULL);
    for (int i = 0; i < NUM_CROSSERS; i++)
        xTaskCreate(vCrosserTask, "Cross", configMINIMAL_STACK_SIZE*4, NULL, 3, NULL);
    vTaskStartScheduler();
    for (;;) { }
    return 0;
}
