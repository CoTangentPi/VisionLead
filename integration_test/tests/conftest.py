import pytest
import pytest_asyncio
from bleak import BleakScanner, BleakClient

DEVICE_NAME = "Vision-Lead"

@pytest_asyncio.fixture(scope="session")
async def ble_device_address():
    print("Scanning for BLE devices...")
    devices = await BleakScanner.discover()
    target_device = next((dev for dev in devices if dev.name == DEVICE_NAME), None)

    assert target_device, f"Device {DEVICE_NAME} not found!"
    print(f"Found device: {target_device.name} - Address: {target_device.address}")

    return target_device.address  # Return the address string

@pytest_asyncio.fixture(scope="session")
async def ble_client(ble_device_address):
    async with BleakClient(ble_device_address) as client:
        assert client.is_connected, "Failed to connect to BLE device!"
        print(f"Connected to {DEVICE_NAME} at {ble_device_address}")
        yield client  # This allows the client to be used in tests
        # Cleanup after tests are done
        print("Disconnecting BLE client...")
