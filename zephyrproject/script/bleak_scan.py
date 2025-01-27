import asyncio
from bleak import BleakClient
from bleak import BleakScanner

async def scan_ble_devices(target_name):
    result = None
    devices = await BleakScanner.discover()
    for device in devices:
        print(f"Found target device: {device.name}, Address: {device.address}")

        if device.name == target_name:
            print(f"Found target device: {device.name}, Address: {device.address}")
            result = device.address

    return result

def main():
    # Scan for the device adress
    target_device_name = "Vision Lead"
    device_address = asyncio.run(scan_ble_devices(target_device_name))
    
    if device_address:
        print(f"Device address saved: {device_address}")
    else:
        print("Target device not found.")


if __name__ == "__main__":
    main()