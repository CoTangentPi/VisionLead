/*
 * BLE source file
 *
 * This file contains the definitions for the Bluetooth Low Energy (BLE)
 * initialization and advertising functions used in the VisionLead project.
 * It sets up the BLE stack, starts advertising, and logs the status.
 *
 * Copyright 2024 University of Calgary and Garmin International Inc.
 *
 * Author: 
 *       Chachi Han <chachi.han@ucalgary.ca>	
 */

#include <zephyr/device.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/services/bas.h>
#include <zephyr/logging/log.h>

// Register a logging module named "ble"
LOG_MODULE_REGISTER(ble);

// Advertising data to be sent during BLE advertising
static const struct bt_data adv_data[] = {

    // Set the flags to indicate device is in general discoverable mode 
    // and only supports BLE connections
    BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),

    // Set device name and length
    BT_DATA(BT_DATA_NAME_COMPLETE, "Vision Lead", 11),

    // Set the appearance of the device to Human Interface Device (HID)
    // Note the device appearance is a little endian  16-bit value, so the two bytes are backwards
    BT_DATA_BYTES(BT_DATA_GAP_APPEARANCE, 0xC0, 0x03),

    // Include a custom 128-bit service UUID
    BT_DATA_BYTES(BT_DATA_UUID128_ALL, 
                  0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0, 
                  0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0),
};

/*
 * Function to initialize BLE and start advertising
 *
 * The `ble_init` function sets up the BLE stack and starts advertising. 
 * It logs the start of the process, enables the Bluetooth stack, and logs any errors. 
 * If successful, it starts advertising with the specified data and logs the status.
 */
void ble_init(void)
{
    int err; 

    LOG_INF("Starting Zephyr BLE Test"); 

    // Enable the Bluetooth stack
    err = bt_enable(NULL);
    if (err) {
        LOG_ERR("Bluetooth init failed (err %d)", err); 
        return;
    }

    LOG_INF("Bluetooth initialized"); 

    // Start BLE advertising with the specified advertising data
    err = bt_le_adv_start(BT_LE_ADV_CONN, adv_data, ARRAY_SIZE(adv_data), NULL, 0);
    if (err) {
        LOG_ERR("Advertising failed to start (err %d)", err); 
        return;
    }
    LOG_INF("Advertising successfully started"); 
}