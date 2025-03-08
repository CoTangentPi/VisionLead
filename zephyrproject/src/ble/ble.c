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

#include <gpio.h>
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

//=======================================================================
// Define Advertising data to be sent during BLE advertising
//=======================================================================
static const struct bt_data adv_data[] = {

    // Set the flags to indicate device is in general discoverable mode 
    // and only supports BLE connections
    BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),

    // Set device name and length
    BT_DATA(BT_DATA_NAME_COMPLETE, "Vision Lead", 11),

    // // Set the appearance of the device to Human Interface Device (HID)
    // // Note the device appearance is a little endian  16-bit value, so the two bytes are backwards
    // BT_DATA_BYTES(BT_DATA_GAP_APPEARANCE, 0xC0, 0x03),

    // Set the UUID for alert notification service
    BT_DATA_BYTES(BT_DATA_UUID16_ALL, 0x11, 0x18),
};

//=======================================================================
// Decode function
//=======================================================================
void decode_and_execute_command(const uint8_t *message, uint16_t len) {
    switch (message[0]) {
        case 0x01:
            switch (message[1]) {
                case MOTOR_SHORT_PULSE:
                    pulse_motor(MOTOR_0, MOTOR_SHORT_PULSE);
                    break;
                case MOTOR_LONG_PULSE:
                    pulse_motor(MOTOR_0, MOTOR_LONG_PULSE);
                    break;
                case MOTOR_DOUBLE_PULSE:
                    int test = pulse_motor(MOTOR_0, MOTOR_DOUBLE_PULSE); 
		    printk("value of pulse %d", test);
                    break;
                default:
                    LOG_WRN("Unknown motor0 command: %x", message[1]);
                    break;
            }
            break;
        case 0x02:
            switch (message[1]) {
                case MOTOR_SHORT_PULSE:
                    pulse_motor(MOTOR_1, MOTOR_SHORT_PULSE);
                    break;
                case MOTOR_LONG_PULSE:
                    pulse_motor(MOTOR_1, MOTOR_LONG_PULSE);
                    break;
                case MOTOR_DOUBLE_PULSE:
                    pulse_motor(MOTOR_1, MOTOR_DOUBLE_PULSE);
                    break;
                default:
                    LOG_WRN("Unknown motor1 command: %x", message[1]);
                    break;
            }
            break;
        case 0x03:
            switch (message[1]) {
                case 0x01:
                    gpio_set_pin(RED_LED, 1);
                    break;
                case 0x02:
                    gpio_set_pin(RED_LED, 0);
                    break;
                default:
                    LOG_WRN("Unknown other command: %x", message[1]);
                    break;
            }
        default:
            LOG_WRN("Unknown command: %s\n", message);
            break;
    }
}

//=======================================================================
// Define the Alert Notification Service UUID
//=======================================================================
/* UUIDs for the service and characteristics */
static struct bt_uuid_128 my_service_uuid = BT_UUID_INIT_128(
    BT_UUID_128_ENCODE(0xCB94A5B0, 0x1046, 0xB287, 0xD82E, 0x19CE7CA6001B));
    // BT_UUID_128_ENCODE(0x12345678, 0x1234, 0x1234, 0x1234, 0x123456789abc));

static struct bt_uuid_128 char_uuid = BT_UUID_INIT_128(
    BT_UUID_128_ENCODE(0xFDE3A192, 0x8B1E, 0x9CE6, 0xC409, 0x3626A6E17310));
    // BT_UUID_128_ENCODE(0xabcdef01, 0x1234, 0x1234, 0x1234, 0x123456789abc));

/* Buffers for characteristic data */
#define MAX_DATA_LEN 20
static uint8_t rx_buffer[MAX_DATA_LEN];

/* Callback for writable characteristic (char) */
static ssize_t write_callback(struct bt_conn *conn,
                              const struct bt_gatt_attr *attr,
                              const void *buf,
                              uint16_t len,
                              uint16_t offset,
                              uint8_t flags) {
    if (len >= MAX_DATA_LEN) {
        printk("Received data is too large, truncating to %d bytes\n", MAX_DATA_LEN);
        len = MAX_DATA_LEN;
    }

    memset(rx_buffer, 0, sizeof(rx_buffer));
    memcpy(rx_buffer, buf, len);

    LOG_INF("Data length: %d", len);
    LOG_INF("Data received:");
    for (int i = 0; i < len; i++) {
        LOG_INF("%x", rx_buffer[i]);
    }

    // Call the decoder function
    decode_and_execute_command(rx_buffer, len);

    return len;
}

/* GATT service declaration with two characteristics */
BT_GATT_SERVICE_DEFINE(my_service,
    BT_GATT_PRIMARY_SERVICE(&my_service_uuid),
    BT_GATT_CHARACTERISTIC(&char_uuid.uuid,
                           BT_GATT_CHRC_WRITE_WITHOUT_RESP,
                           BT_GATT_PERM_WRITE,
                           NULL, write_callback, rx_buffer),
);

//=======================================================================
// Define the connection callback functions
//=======================================================================
static void connected(struct bt_conn *conn, uint8_t err)
{
    // Write ble connection status to the log
    if (err) {
        LOG_ERR("Connection failed (err %u)", err);
    } else {
        LOG_INF("Connected");
    }
    // Turn on the blue LED to indicate connection
    gpio_set_pin(BLUE_LED, 1);
}

static void disconnected(struct bt_conn *conn, uint8_t reason)
{
    // Write ble disconnection status to the log
    LOG_INF("Disconnected (reason %u)", reason);

    // Turn off the blue LED to indicate disconnection
    gpio_set_pin(BLUE_LED, 0);
}

// Define the connection callback structure
static struct bt_conn_cb conn_callbacks = {
    .connected = connected,
    .disconnected = disconnected,
};

//=======================================================================
// BLE Initialization funciton
//=======================================================================
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

    // Register connection callbacks
    bt_conn_cb_register(&conn_callbacks);

    LOG_INF("Bluetooth initialized"); 

    // Start BLE advertising with the specified advertising data
    err = bt_le_adv_start(BT_LE_ADV_CONN, adv_data, ARRAY_SIZE(adv_data), NULL, 0);
    if (err) {
        LOG_ERR("Advertising failed to start (err %d)", err); 
        return;
    }
    LOG_INF("Advertising successfully started"); 
}
