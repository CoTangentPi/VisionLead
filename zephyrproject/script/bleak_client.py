import asyncio
from bleak import BleakClient
import logging

# Enable debug logging
logging.basicConfig(level=logging.DEBUG)

async def send_data(address, characteristic_uuid, data):
    async with BleakClient(address) as client:
        # Ensure the client is connected
        if not client.is_connected:
            print("Failed to connect to the device.")
            return

        print("Connected to the device.")
        
        # Add a small delay before sending data
        await asyncio.sleep(1)
        
        # Write data to the characteristic
        await client.write_gatt_char(characteristic_uuid, data)
        print("Data sent.")

def main():
    DEVICE_ADDRESS = "D4:04:8D:9B:68:72"
    CHARACTERISTIC_UUID = "1811"
    data_to_send = bytearray([0x01, 0x02, 0x03])

    asyncio.run(send_data(DEVICE_ADDRESS, CHARACTERISTIC_UUID, data_to_send))

if __name__ == "__main__":
    main()