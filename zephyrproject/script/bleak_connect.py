import asyncio
from bleak import BleakClient
import logging

# Enable debug logging
logging.basicConfig(level=logging.DEBUG)

async def list_services(address):
    async with BleakClient(address) as client:
        # Ensure the client is connected
        if not client.is_connected:
            print("Failed to connect to the device.")
            return

        print("Connected to the device.")
        
        # Discover services
        services = await client.get_services()
        for service in services:
            print(f"Service: {service.uuid}")
            for characteristic in service.characteristics:
                print(f"  Characteristic: {characteristic.uuid}, Properties: {characteristic.properties}")

def main():
    DEVICE_ADDRESS = "D4:04:8D:9B:68:72"

    asyncio.run(list_services(DEVICE_ADDRESS))

if __name__ == "__main__":
    main()