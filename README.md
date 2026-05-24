Embedded Electronic Engineering Lab A
Cross-Traffic Management for Autonomous Vehicles
6th Semester Electronic Engineering
Hochschule Hamm-Lippstadt (HSHL)

Team Members:

1. Daniel Chidi Chimezie [1230515] - daniel-chidi.chimezie@stud.hshl.de
2. Jaleel Ur Rehman [1230351] - jaleel.ur-rehman@stud.hshl.de
3. Christian Percival [1230330] - Christian.Percival@stud.hshl.de
4. Rionela Kovaci [1231421] - rionela.kovaci@stud.hshl.de
   
Supervisors:
- Prof. Dr. Stefan Henkler
- Prof. Dr. Achim Rettberg
   
Project Overview: 

This project focuses on the modelling, simulation, and implementation of an intelligent cross-traffic management system for autonomous vehicles using queuing theory, embedded systems concepts, and FreeRTOS.

Research Questions:

1. How can traffic at an intersection for autonomous vehicles be modelled using queuing systems?
2. What traffic management scenario minimizes congestion at an intersection?
3. How can a traffic intersection be simulated using C-code and FreeRTOS?
   
Objectives:

- Develop a realistic intersection traffic model
- Simulate autonomous vehicle communication
- Implement traffic control logic using FreeRTOS
- Evaluate queue performance under different traffic conditions
- Explore HW/SW co-design methodologies

Technologies and Tools:

Software:
- C Programming Language
- FreeRTOS
- UPPAAL Timed Automata
- Git & GitHub
- LaTeX & BibTeX

Concepts:
- Embedded Systems
- Real-Time Systems
- Queuing Theory (M/M/1 and M/M/2)
- Vehicular Communication
- Traffic Flow Simulation
- 
Queuing Theory Model:

The project uses M/M/1 and M/M/2 queue models to analyze traffic flow, waiting time, queue length, and congestion in autonomous vehicle intersections.
Vehicular Communication.

The communication architecture considers:

- Vehicle-to-Vehicle (V2V)
- Vehicle-to-Infrastructure (V2I)
  
Project Structure:

docs/        Documentation and reports
models/      UPPAAL and system models
diagrams/    Use-case, sequence, and activity diagrams
src/         C source code
freertos/    FreeRTOS related files
simulation/  Traffic simulation logic
hardware/    HW/SW co-design resources
tests/       Testing and validation

Development Methodology:

1. Requirement Analysis
2. Use-Case Development
3. System Modelling
4. Simulation Design
5. Embedded Implementation
6. HW/SW Co-design
7. Testing and Validation
8. Documentation and Reporting
   
Milestones:

Week 1: Requirements and Use-Cases
Week 2: Models and Diagrams
Week 3: Initial Implementation
Week 4: HW/SW Co-design Proposal
Week 5: Final Co-design Implementation

Final Week: Final Presentation and Report

Git Workflow:

- Use GitHub for version control
- Commit changes regularly
- Create feature branches
- Track issues and responsibilities
  
Future Improvements:
- AI-based traffic optimization
- Machine learning for congestion prediction
- Multi-intersection synchronization
- FPGA acceleration
- Real-world sensor integration
  
License:

This project is developed for academic purposes under the Embedded Electronic Engineering Lab A course at Hochschule Hamm-Lippstadt.
