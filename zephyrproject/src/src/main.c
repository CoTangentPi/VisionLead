 /*
 * VisionLead Embedded Application
 *
 * This file contains the main entry point for the VisionLead.
 *
 * Copyright 2024 University of Calgary and Garmin International Inc.
 *
 * Author: 
 *       Chachi Han <chachi.han@ucalgary.ca>	
 */

/*
 * SPDX-License-Identifier: Apache-2.0
*/

#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include "ble/ble.h"

LOG_MODULE_REGISTER(main);

void main(void)
{
    LOG_INF("Starting main application");

    ble_init();
}