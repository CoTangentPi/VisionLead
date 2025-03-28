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

#include <ble.h>
#include <gpio.h>

#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

void main(void)
{

    LOG_INF("Begain GPIO and BLE initialization");

    gpio_init();
    ble_init();
    
    while(true){
	k_msleep(1000);
    } 

    return 0;
}
