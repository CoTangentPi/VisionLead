/*
 * BLE source file
 *
 * This file contains the definitions for the Bluetooth Low Energy (BLE)
 * initialization and BLE related functions used in the VisionLead project.
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
static const struct bt_data ad[] = {
    // General discoverable and no BR/EDR support
    BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)), 
    // Complete device name
    BT_DATA(BT_DATA_NAME_COMPLETE, "Zephyr BLE Test", 15), 
};

// Function to initialize BLE and start advertising
void ble_init(void)
{
    int err; // Variable to store error codes

    LOG_INF("Starting Zephyr BLE Test"); // Log the start of BLE initialization

    // Enable the Bluetooth stack
    err = bt_enable(NULL);
    if (err) {
        LOG_ERR("Bluetooth init failed (err %d)", err); // Log an error if Bluetooth initialization fails
        return;
    }

    LOG_INF("Bluetooth initialized"); // Log successful Bluetooth initialization

    // Start BLE advertising with the specified advertising data
    err = bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad), NULL, 0);
    if (err) {
        LOG_ERR("Advertising failed to start (err %d)", err); // Log an error if advertising fails to start
        return;
    }

    LOG_INF("Advertising successfully started"); // Log successful start of advertising
}