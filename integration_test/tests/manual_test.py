import pytest
import asyncio
from bleak import BleakScanner, BleakClient

# UUIDs for services and characteristics
SERVICE_UUID = "cb94a5b0-1046-b287-d82e-19ce7ca6001b"
CHARACTERISTIC_UUID = "fde3a192-8b1e-9ce6-c409-3626a6e17310"

# Motor control values
LEFT_MOTOR = 0x01
RIGHT_MOTOR = 0x02
BOTH_MOTOR = 0x03

# Pattern values
ONE_SHORT = 0x00
ONE_LONG = 0x01
DOUBLE_SHORT = 0x02
SHORT_LONG_SHORT = 0x03

# List of motors and patterns for parameterization
MOTORS = ['left', 'right', 'both']
PATTERNS = [ONE_SHORT, ONE_LONG, DOUBLE_SHORT, SHORT_LONG_SHORT]

# Parameterize the test with all combinations of motors and patterns
@pytest.mark.parametrize("motor, pattern", [(motor, pattern) for motor in MOTORS for pattern in PATTERNS])
@pytest.mark.asyncio
async def test_drive_motor_manual(ble_client, motor, pattern):
    """Test motor movement manually for different motors and patterns."""
    
    # Map motor to specific byte values
    if motor == "left":
        motor_value = LEFT_MOTOR
    elif motor == "right":
        motor_value = RIGHT_MOTOR
    else:
        motor_value = BOTH_MOTOR 

    # Prepare the test command (motor value + pattern)
    test_command = bytes([motor_value, pattern])

    # Retry loop
    while True:
        try:
            await ble_client.write_gatt_char(CHARACTERISTIC_UUID, test_command, response=True)
        except Exception as e:
            pytest.fail(f"Failed to write to characteristic {CHARACTERISTIC_UUID}: {e}")

        # Ask for manual validation
        user_input = input(f"\nCan you clearly feel {motor.upper()} motor vibrated with pattern {pattern}? (yes(y)/no(n)/retry(r)): ").strip().lower()

        # Validate user input
        if user_input == "yes" or user_input == "y":
            print(f"{motor} motor {pattern} pattern test PASS")
            break  # Exit the loop and end the test
        elif user_input == "no" or user_input == "n":
            assert False, f"{motor} motor {pattern} pattern test FAIL"
        elif user_input == "retry" or user_input == "r":
            print("Retrying...")
            continue  # Retry sending the command
        else:
            print("Invalid input. Please enter 'yes', 'no', or 'retry'.")
