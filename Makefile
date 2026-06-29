# Makefile — Autonomous Intersection CZ Reservation (FreeRTOS POSIX)
# Team A9
#
# Usage:
#   git clone --depth 1 https://github.com/FreeRTOS/FreeRTOS-Kernel.git
#   make
#   ./intersection_sim

KERNEL_DIR ?= ./FreeRTOS-Kernel
PORT_DIR   := $(KERNEL_DIR)/portable/ThirdParty/GCC/Posix

CC      := gcc
CFLAGS  := -std=gnu11 -O0 -g -Wall -Wextra \
           -I . \
           -I $(KERNEL_DIR)/include \
           -I $(PORT_DIR) \
           -I $(PORT_DIR)/utils
LDLIBS  := -lpthread -lrt

SRCS := main.c \
        assert_impl.c \
        $(KERNEL_DIR)/tasks.c \
        $(KERNEL_DIR)/list.c \
        $(KERNEL_DIR)/queue.c \
        $(KERNEL_DIR)/portable/MemMang/heap_3.c \
        $(PORT_DIR)/port.c \
        $(PORT_DIR)/utils/wait_for_event.c

TARGET := intersection_sim

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) $(LDLIBS) -o $(TARGET)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)
