# This code will connection to the given BLE device adress and send DATA_TO_SEND to CHARACTERISTIC_UUID
# To run the code:
# Use bleak_scan first to sean BLE signle with device name "Vision Lead"
# Copy the device adress and past in DEVICE_ADDRESS
# This will sned the DATA_TO_SEND to CHARACTERISTIC_UUID

# This assumes there is only one devices named vision lead and assumes you did not modify the BLE GATT characteristic UUID or the BLE protocal.
# If evetyhing goes to the plan, the DATA_TO_SEND will turn on the onboard RED_LED
# Note, the BLUE_LED will turn on when BLE is connected and turn off when disconnection
# Based on my limited test, the code will disconnect after send the message, leaving only the RED_LED on.
# You can change the DATA_TO_SEND to turn off the LED by sned 0x04 0x02
import asyncio
from bleak import BleakClient
import logging


DEVICE_ADDRESS = "D4:04:8D:9B:68:72"
CHARACTERISTIC_UUID = "FDE3A192-8B1E-9CE6-C409-3626A6E17310" 
DATA_TO_SEND = bytes([0x04, 0x01])  

async def send_data(address):
    try:
        async with BleakClient(address, timeout=30.0) as client:
            if not client.is_connected:
                print("Failed to connect to the device.")
                return

            print("Connected to the device.")
            await asyncio.sleep(1)
            
            # Discover services
            services = await client.get_services()
            for service in services:
                print(f"Service: {service.uuid}")
                for characteristic in service.characteristics:
                    print(f"  Characteristic: {characteristic.uuid}, Properties: {characteristic.properties}")

            # Write data to the characteristic
            if CHARACTERISTIC_UUID:
                await client.write_gatt_char(CHARACTERISTIC_UUID, DATA_TO_SEND)
                print(f"Data sent to {CHARACTERISTIC_UUID}: {DATA_TO_SEND}")
    
    except Exception as e:
        print(f"An error occurred: {e}")


def main():
    asyncio.run(send_data(DEVICE_ADDRESS))


if __name__ == "__main__":
    main()
