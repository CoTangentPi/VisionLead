# VisionLead Embedded System

This project is for Bluetooth Low Energy (BLE) enabled embedded application built on the Zephyr RTOS, designed for the Arduino Nano 33 BLE board and adaptable to any nRF52 series microcontroller. This project offers BLE wireless connectivity with custom protocol, alongside GPIO control for audio components including buzzers and speakers.

## Features

- **Bluetooth Low Energy (BLE)**: Full BLE stack implementation with custom advertising and GATT services
- **GPIO Control**: LED control and GPIO management
- **Zephyr RTOS**: Built on the robust Zephyr Real-Time Operating System
- **Arduino Nano 33 BLE**: Optimized for the popular Arduino Nano 33 BLE development board, can be adopted for nRF52 series chips or similar
- **Integration Testing**: Comprehensive integration test suite for BLE functionality
- **Python Client Tools**: Additional BLE client scripts for set up for device interaction

## Demo

### Demo Overview

This demonstration showcases the core BLE communication and GPIO control functionality of the VisionLead system:

**Important Note**: The components are shown outside the enclosure for demonstration purposes. In the actual implementation, all components are contained within the enclosure.

### Demo Features

- **Blue LED Indicator**: Indicates successful BLE connection establishment
- **BLE Communication**: Manual BLE command transmission for testing (simulates real-world Google Maps API integration)
- **Vibration Patterns**: Three distinct vibration modes controlled via BLE commands

### BLE Command Demonstrations

| Command | Function | Description |
|---------|----------|-------------|
| `0103` | Motor 1 | Short double pulse vibration on motor 1 |
| `0203` | Motor 2 | Short double pulse vibration on motor 2 |
| `0303` | Both Motors | Simultaneous double pulse vibration on both motors |

**Note**: The demo uses manual BLE command transmission for easier demonstration. In production, these commands would be automatically generated based on GPS navigation data from Google Maps API integration.

### Video Demo

https://github.com/user-attachments/assets/560f7868-09c8-4e30-9f33-99de8105bef5

## Project Structure

```
visionlead/
├── README.md                          # This file
├── zephyrproject/                     # Main embedded application
│   ├── src/                           # Source code
│   │   ├── src/main.c                 # Main application entry point
│   │   ├── ble/                       # BLE implementation
│   │   │   ├── ble.c                  # BLE core functionality
│   │   │   └── ble.h                  # BLE header file
│   │   └── gpio/                      # GPIO drivers
│   │       ├── gpio.c                 # GPIO implementation
│   │       └── gpio.h                 # GPIO header file
│   ├── script/                        # Python BLE client tools
│   │   ├── bleak_client.py            # BLE client implementation
│   │   ├── bleak_connect.py           # BLE connection utilities
│   │   └── bleak_scan.py              # BLE device scanning
│   ├── INSTALL_INSTRUCTIONS.md        # Detailed setup guide
│   └── RECOMENDED_VIDEOS.md           # Learning resources
└── integration_test/                  # Test suite
    ├── tests/                         # Test implementations
    │   ├── auto_test.py               # Automated BLE tests
    │   └── manual_test.py             # Manual testing procedures
    └── requirements.txt               # Python test dependencies
```

## Quick Start

### Prerequisites

- **Hardware**: Arduino Nano 33 BLE board
- **OS**: Primary: Linux, macOS. Secondary: Windows
- **Python**: 3.7 or higher
- **Git**: For cloning the repository

### Setup Instructions

For detailed setup, build, and flash instructions, please see the README.md files in the respective folders:

- **Embedded Development**: [`zephyrproject/INSTALL_INSTRUCTIONS.md`](./zephyrproject/INSTALL_INSTRUCTIONS.md)
- **Integration Testing**: [`integration_test/README.md`](./integration_test/README.md)


## BLE Services

The application implements custom BLE services for:

- Device status monitoring
- GPIO control
- Data exchange with client applications

## Hardware Support

- **Primary Target**: Arduino Nano 33 BLE **nRF52840-based**
- **GPIO**: LED control and general I/O
- **Bluetooth**: BLE 5.0 with Nordic nRF52840 SoC

## Testing

The project includes comprehensive testing:

- **Automated Tests**: BLE connectivity and data exchange
- **Manual Tests**: User-interactive testing procedures
- **Integration Tests**: End-to-end system validation


## Contributing

This project is a University of Calgary Software and Electrical Engineering capstone project sponsored by Garmin.

### Development Team
- **Chachi Han** - Embedded Software and Integration
- **James Platt** - Embedded Software
- **Shamis Ali** - Mobile Application
- **Laura Zhao** - Electrical and Hardware
- **Bruce Zhao** - Electrical and Hardware


---

For questions or support regarding this project, please contact:

Chachi Han - contact@chachi.cc

