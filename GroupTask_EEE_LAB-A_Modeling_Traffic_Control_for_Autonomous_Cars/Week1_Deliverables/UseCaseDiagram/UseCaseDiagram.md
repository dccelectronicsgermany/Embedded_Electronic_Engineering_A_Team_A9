@startuml AIM_UseCaseDiagram

title Autonomous Intersection Management — Use Case Diagram

left to right direction

actor "Autonomous Vehicle" as AV
actor "Controller" as C
actor "Emergency Veicle" as EV

EV --|> AV

rectangle "Cross-Traffic Management System" {

  package "Vehicle Registration & Communication" {
    usecase "Register Vehicle\nat Intersection" as UC1
    usecase "Detect Approaching\nVehicle" as UC1a
    usecase "Share Position\nand Speed" as UC2a
    usecase "Broadcast Intent\n(direction)" as UC2b
    usecase "V2V Communication" as UC2
    usecase "V2I Communication" as UC3
  }

  package "Traffic Flow & Queue Management" {
    usecase "Optimize Traffic Flow" as UC4
    usecase "Monitor Traffic\nCongestion" as UC4a
    usecase "Calculate Waiting Time" as UC4b
    usecase "Manage Queue\n(M/M/1 or M/M/2)" as UC5
    usecase "Assign Crossing\nPriority" as UC6
    usecase "Assign Schedule Slot" as UC6a
  }

  package "Access Control" {
    usecase "Grant Access" as UC7
    usecase "Deny Access &\nRe-schedule" as UC8
    usecase "Monitor Intersection\nStatus" as UC9
  }

  package "Emergency & Safety" {
    usecase "Emergency Override" as UC10
    usecase "Ensure Collision\nAvoidance" as UC11
    usecase "Broadcast Emergency\nSignal" as UC12
    usecase "Halt / Yield Vehicle" as UC13
  }

}

AV --> UC1
AV --> UC2
AV --> UC3

C --> UC4
C --> UC5
C --> UC6
C --> UC7
C --> UC8
C --> UC10

EV --> UC10

UC1 ..> UC1a : <<include>>
UC2 ..> UC2a : <<include>>
UC2 ..> UC2b : <<include>>
UC4 ..> UC4a : <<include>>
UC4 ..> UC4b : <<include>>
UC6 ..> UC6a : <<include>>
UC10 ..> UC11 : <<extend>>
UC10 ..> UC12 : <<include>>
UC12 ..> UC13 : <<include>>
UC7 ..> UC9  : <<include>>
UC8 ..> UC9  : <<include>>

@enduml
