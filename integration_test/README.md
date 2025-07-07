# VisionLead Integration Testing

This directory contains comprehensive integration tests for the VisionLead embedded system using the pytest framework. The test suite validates BLE connectivity, data exchange, and GPIO control functionality.

## Overview

The integration tests are designed to verify end-to-end functionality of the VisionLead system, including:

- **BLE Communication**: Connection establishment and data exchange
- **GPIO Control**: Motor control and LED status verification
- **Protocol Validation**: Custom BLE protocol command testing
- **System Integration**: Complete workflow testing

## Test Framework

- **Framework**: pytest
- **BLE Library**: bleak (Bluetooth Low Energy library)

*Note: BLE drivers may not work well on certain ASUS laptops running Linux.


## Prerequisites

- **Python**: 3.7 or higher
- **Operating System**: Linux*, macOS, or Windows with Bluetooth support
- **Device**: VisionLead embedded device powered on and advertising

*Note: It is noticed that certain ASUS laptops running Linux may have BLE driver support issues

## Installation

1. **Install Dependencies**:
   ```bash
   pip install -r requirements.txt
   ```

2. **Verify Installation**:
   ```bash
   pytest --version
   ```

## Running Tests

### Run All Tests

Execute the complete test suite:

```bash
pytest -s
```

**Alternative syntax**:
```bash
python -m pytest -s
```

### Run Specific Test Files

Execute specific test modules:

```bash
# Run automated tests only
pytest tests/auto_test.py

# Run manual tests only
pytest tests/manual_test.py
```

### Command Line Options

- `-s`: Disable output capturing (required for manual tests with user interaction)
- `-v`: Verbose output showing individual test results
- `--tb=short`: Shorter traceback format for cleaner output

**Example with verbose output**:
```bash
pytest -s -v tests/auto_test.py
```

## Test Structure

```
integration_test/
├── README.md              # This file
├── requirements.txt       # Python dependencies
├── conftest.py            # pytest configuration and fixtures
└── tests/
    ├── auto_test.py       # Automated BLE and GPIO tests
    └── manual_test.py     # Manual testing procedures
```

## Test Categories

### Automated Tests (`auto_test.py`)

- **BLE Discovery**: Device scanning and identification
- **Connection Testing**: BLE connection establishment
- **Data Exchange**: Command transmission and response validation
- **GPIO Verification**: LED status and motor control testing

### Manual Tests (`manual_test.py`)

- **User Interaction**: Tests requiring manual verification
- **Physical Validation**: Motor vibration and LED visual confirmation
- **User Experience**: End-to-end workflow testing

## Important Notes

- **Output Flag**: The `-s` flag is **required** for manual tests to enable user interaction
- **Device State**: Ensure the VisionLead device is powered on and advertising before running tests
- **Bluetooth**: Verify your system's Bluetooth is enabled and functioning

## Troubleshooting

### Common Issues

1. **Device Not Found**: Ensure VisionLead device and tests are up to data, and it powered on and advertising
2. **Connection Timeout OR Permission Errors**: Check Bluetooth adapter functionality and device proximity, and run with appropriate permissions for Bluetooth access

*Linux system may have Bluetooth driver support/permission issues

### Test Debugging

Enable verbose output for detailed test information:

```bash
pytest -s -v --tb=long
```
