# Hardware Software Co-Design

## Project Overview

This repository contains the complete Hardware/Software (HW/SW) Co-Design development of an **Autonomous Intersection Management System (AIMS)**. The project combines **formal verification (UPPAAL)**, **real-time software (FreeRTOS)**, and **hardware acceleration (FPGA)** to design, analyze, and validate a reservation-based intelligent traffic intersection controller.

The objective is to provide a collision-free, deadlock-free, and efficient autonomous intersection capable of coordinating multiple vehicles through Critical Zone (CZ) reservation while satisfying real-time constraints.

---

# Project Information

**Project Title**

Hardware/Software Co-Design of an Autonomous Intersection Management System

**Author**

Daniel Chidi Chimezie,Rionela Kovaci,Christian Percival and Jaleel Ur Rehman

**University**

Hochschule Hamm-Lippstadt (HSHL)

**Programme**

Electronic Engineering (B.Eng.)

**Course**

Hardware/Software Co-Design

**Supervisor**

Prof. Dr. Achim Rettberg

---

# Project Objectives

The project aims to:

- Develop a reservation-based autonomous intersection controller.
- Partition the system into hardware and software components.
- Implement real-time scheduling using FreeRTOS.
- Design hardware accelerators suitable for FPGA implementation.
- Formally verify system correctness using UPPAAL.
- Demonstrate collision-free and deadlock-free operation.
- Produce publication-quality engineering documentation.

---

# System Features

- Four-lane autonomous intersection
- Four Critical Zones (CZ1–CZ4)
- Reservation-based scheduling
- Continuous vehicle flow
- Collision avoidance
- Deadlock prevention
- Real-time scheduling
- FPGA hardware acceleration
- FreeRTOS software architecture
- Formal verification with UPPAAL

---

# Technologies

## Hardware

- FPGA Architecture
- Hardware Accelerators
- Critical Zone Reservation Engine
- Route Conflict Engine
- Timer Unit
- Memory Architecture

## Software

- C Programming
- FreeRTOS
- Task Scheduling
- Queues
- Semaphores
- Real-Time Control

## Formal Verification

- UPPAAL
- Timed Automata
- Model Checking

---

# Repository Structure

```text
Hardware Software Co-Design/
│
├── README.md
│
├── Phase 1 - System Engineering/
│   ├── System Context Diagram
│   ├── Stakeholder Diagram
│   ├── Requirements Diagram
│   ├── Use Case Diagram
│   └── Functional Decomposition
│
├── Phase 2 - HW_SW Partitioning/
│   ├── Hardware Software Partition
│   ├── Processing Elements
│   ├── Communication Architecture
│   ├── Data Flow
│   └── Memory Architecture
│
├── Phase 3 - FPGA Architecture/
│   ├── Top-Level FPGA
│   ├── Reservation Engine
│   ├── Route Conflict Engine
│   ├── Critical Zone Controller
│   ├── Timer
│   └── AXI Interface
│
├── Phase 4 - FreeRTOS/
│   ├── Task Architecture
│   ├── Queue Architecture
│   ├── Scheduler
│   ├── Semaphores
│   └── Timing
│
├── Phase 5 - UPPAAL/
│   ├── UPPAAL Models
│   ├── Verification Queries
│   ├── Timed Automata
│   └── Results
│
├── Phase 6 - UML/
│   ├── Activity Diagram
│   ├── Sequence Diagram
│   ├── State Machine
│   └── Class Diagram
│
├── Phase 7 - Evaluation/
│   ├── Timing Analysis
│   ├── Throughput
│   ├── CPU Utilization
│   └── Performance
│
└── Documents/
    ├── IEEE Paper
    ├── Presentation
    ├── References
    └── Figures
```

---

# Design Methodology

The project follows a standard Hardware/Software Co-Design methodology:

1. System Specification
2. Architecture Exploration
3. Hardware/Software Partitioning
4. Communication Synthesis
5. Hardware Design
6. Software Design
7. Formal Verification
8. Performance Evaluation

---

# Formal Verification

The system is formally verified using **UPPAAL** to ensure:

- Collision Freedom
- Deadlock Freedom
- Reservation Correctness
- Route Safety
- Timing Constraints
- Vehicle Liveness
- Continuous Traffic Flow

---

# Software Architecture

The software subsystem is implemented using **FreeRTOS** and includes:

- Vehicle Detection Tasks
- Reservation Scheduler
- Critical Zone Manager
- Controller Task
- Queue Communication
- Synchronization using Semaphores
- Statistics and Logging

---

# Hardware Architecture

The hardware subsystem targets FPGA implementation and consists of:

- Sensor Interface
- Route Mapping Unit
- Route Conflict Engine
- Critical Zone Reservation Engine
- Delay Assignment Unit
- Timer Module
- Memory Controller
- Hardware Scheduler

---

# Current Status

- System Specification ✔
- HW/SW Partitioning ✔
- FreeRTOS Prototype ✔
- UPPAAL Verification ✔
- FPGA Architecture (In Progress)
- IEEE Paper (In Progress)
- Final Presentation (In Progress)

---

# Future Work

- FPGA implementation using VHDL/SystemVerilog
- Hardware synthesis and timing analysis
- Hardware-in-the-loop validation
- Integration with physical sensors
- Multi-intersection coordination
- Machine learning-based traffic prediction

---

# License

This repository is intended for academic and research purposes.

---

# Acknowledgements

Special thanks to:

- Prof. Dr. Achim Rettberg
- Hochschule Hamm-Lippstadt (HSHL)
- FreeRTOS
- UPPAAL Development Team

---

**Author:** Daniel Chidi Chimezie,Rionela Kovaci,Christian Percival and Jaleel Ur Rehman.   
**Programme:** Electronic Engineering (B.Eng.)  
**University:** Hochschule Hamm-Lippstadt (HSHL)  
**Course:** Hardware/Software Co-Design
