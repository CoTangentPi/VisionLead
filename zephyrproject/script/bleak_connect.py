# ===============================================================
# Please use nRF connect instead, this code does not work well
# ===============================================================

import asyncio
from bleak import BleakClient
import logging

# Enable debug logging
logging.basicConfig(level=logging.DEBUG)

async def list_services(address):
    try:
        async with BleakClient(address, timeout=30.0) as client:  # Increase timeout
            # Ensure the client is connected
            if not client.is_connected:
                print("Failed to connect to the device.")
                return

            print("Connected to the device.")
            
            # Add a small delay before discovering services
            await asyncio.sleep(1)
            
            # Discover services
            services = await client.get_services()
            for service in services:
                print(f"Service: {service.uuid}")
                for characteristic in service.characteristics:
                    print(f"  Characteristic: {characteristic.uuid}, Properties: {characteristic.properties}")
    except Exception as e:
        print(f"An error occurred: {e}")

def main():
    DEVICE_ADDRESS = "D4:04:8D:9B:68:72"

    asyncio.run(list_services(DEVICE_ADDRESS))

if __name__ == "__main__":
    main()