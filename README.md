# Well Killer

## Project Overview

Well Killer is a C++ application that simulates a LoRa-based irrigation well monitoring and emergency shutdown system.

The purpose of the project is to allow a farmer to monitor irrigation wells and remotely shut them down when necessary. The system is based on a real-world concept using LoRa wireless communication and a Murphy shutdown circuit.

This repository currently contains the software simulation of the Well Killer system.

## Core Features

- View the status of all irrigation wells
- Remotely shut down an individual well
- Perform an emergency shutdown of all wells
- Record shutdown events in a log file
- View previous shutdown events
- Test simulated LoRa communication

## Object-Oriented Design

The application uses object-oriented programming principles including:

- Encapsulation
- Inheritance
- Polymorphism
- Function overloading
- Exception handling

The main classes currently include:

- `Device`
- `LoRaTransmitter`
- `LoRaReceiver`
- `Well`
- `WellController`

## System Architecture

The project is being developed using a layered modular architecture:

1. **Presentation Layer** - Handles menus, user input, and program output.
2. **Application Layer** - Handles well control and shutdown operations.
3. **Domain/Device Layer** - Represents wells and LoRa communication devices.
4. **Persistence/Hardware Layer** - Handles logging and simulated hardware communication.

## Technologies

- C++
- Microsoft Visual Studio
- Git
- GitHub
- LoRa communication hardware
- Arduino hardware
- Murphy irrigation engine controls

## Testing

Testing will include:

- Unit testing
- Integration testing
- System testing

Automated tests will be added as development continues.

## UML Documentation

The final project will include:

- Use Case Diagram
- Class Diagram
- Sequence Diagram
- Deployment Diagram

## Project Status

**In Development**

The current version provides the core Well Killer simulation. The project will continue to be updated with improved architecture, testing, UML documentation, and hardware integration.
