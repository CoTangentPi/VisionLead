import pytest
import asyncio
from bleak import BleakScanner, BleakClient

# SERVICE_UUID = "CB94A5B0-1046-B287-D82E-19CE7CA6001B" 
SERVICE_UUID = "cb94a5b0-1046-b287-d82e-19ce7ca6001b"
# CHARACTERISTIC_UUID = "FDE3A192-8B1E-9CE6-C409-3626A6E17310"
CHARACTERISTIC_UUID = "fde3a192-8b1e-9ce6-c409-3626a6e17310"

@pytest.mark.asyncio
async def test_scan_ble_name(ble_device_address):
    assert ble_device_address is not None, "BLE device not found!"
    print(f"BLE device found at address: {ble_device_address}")

@pytest.mark.asyncio
async def test_connect_ble(ble_client):
    assert ble_client.is_connected, "Failed to connect to BLE device!"
    print("Connected successfully")

@pytest.mark.asyncio
async def test_read_gatt_characteristics(ble_client):
    services = ble_client.services

    services_list = list(services) 
    assert len(services_list) > 0, "No GATT services found!"
    
    for service in services_list:
        print(f"Service: {service.uuid}")
        for char in service.characteristics:
            print(f"  Characteristic: {char.uuid} | Properties: {char.properties}")

@pytest.mark.asyncio
async def test_match_gatt_service(ble_client):
    expected_services = {
        SERVICE_UUID: {  
            CHARACTERISTIC_UUID, 
        },
    }

    services_list = list(ble_client.services)

    device_services = {
        service.uuid: {char.uuid for char in service.characteristics}
        for service in services_list
    }

    for service_uuid, expected_chars in expected_services.items():
        assert service_uuid in device_services, f"Missing expected service: {service_uuid}"
        actual_chars = device_services[service_uuid]
        assert expected_chars.issubset(actual_chars), f"Service {service_uuid} missing expected characteristics!"

    print("All expected GATT services and characteristics found:")
    for service_uuid, chars in expected_services.items():
        print(f"  - Service: {service_uuid}")
        for char_uuid in chars:
            print(f"    - Characteristic: {char_uuid}")
            
@pytest.mark.asyncio
async def test_characteristic_properties(ble_client):
    characteristic = next(
        (char for service in ble_client.services for char in service.characteristics if char.uuid == CHARACTERISTIC_UUID),
        None,
    )

    assert characteristic is not None, f"Characteristic {CHARACTERISTIC_UUID} not found!"

    properties = characteristic.properties
    print(f"Properties of {CHARACTERISTIC_UUID}: {properties}")

    assert "write-without-response" in properties, f"Characteristic {CHARACTERISTIC_UUID} is not writable!"