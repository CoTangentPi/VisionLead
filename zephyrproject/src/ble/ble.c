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

#include "zephyr/logging/log_core.h"
#include <zephyr/device.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/services/bas.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>

// Register a logging module named "ble"
LOG_MODULE_REGISTER(ble, LOG_LEVEL_DBG);

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

    // Set the UUID for alert notification service
    BT_DATA_BYTES(BT_DATA_UUID16_ALL, 0x11, 0x18),
};

// Define the Alert Notification Service UUID
static struct bt_uuid_16 alert_notification_service_uuid = BT_UUID_INIT_16(0x1811);

// Define the Alert Notification Control Point Characteristic UUID
static struct bt_uuid_16 alert_notification_control_point_uuid = BT_UUID_INIT_16(0x2A44);

// Define a buffer to hold the characteristic value
static uint8_t alert_notification_control_point[20];



// Callback function to handle notifications
static void notify_cb(struct bt_conn *conn, const struct bt_gatt_attr *attr,
                      const void *buf, uint16_t len, void *user_data)
{
    LOG_INF("Received notification from phone");
    // Process the notification data

    gpio_set_pin(RED_LED, 0);
    memcpy(alert_notification_control_point, buf, len);
}

// Define the characteristic with the notification callback
static struct bt_gatt_attr alert_notification_attrs[] = {
    BT_GATT_PRIMARY_SERVICE(&alert_notification_service_uuid),
    BT_GATT_CHARACTERISTIC(&alert_notification_control_point_uuid.uuid, BT_GATT_CHRC_NOTIFY,
                           BT_GATT_PERM_READ, NULL, notify_cb, alert_notification_control_point),
};

// Define the GATT service
static struct bt_gatt_service alert_notification_service = BT_GATT_SERVICE(alert_notification_attrs);

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

    // Register the Alert Notification GATT service
    bt_gatt_service_register(&alert_notification_service);

    LOG_INF("Bluetooth initialized"); 

    // Start BLE advertising with the specified advertising data
    err = bt_le_adv_start(BT_LE_ADV_CONN, adv_data, ARRAY_SIZE(adv_data), NULL, 0);
    if (err) {
        LOG_ERR("Advertising failed to start (err %d)", err); 
        return;
    }
    LOG_INF("Advertising successfully started"); 


}
