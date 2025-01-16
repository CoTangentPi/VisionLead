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

#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

LOG_MODULE_REGISTER(main);


/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

void main(void)
{
    LOG_INF("Starting main application");

    ble_init();

    int ret;
    bool led_state = true;

    while(true){

	if (!gpio_is_ready_dt(&led)) {
		return 0;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}

	ret = gpio_pin_toggle_dt(&led);

	led_state = !led_state;
	printf("LED state: %s\n", led_state ? "ON" : "OFF");
	k_msleep(500);

	ret = gpio_pin_toggle_dt(&led);
	k_msleep(1000);

	ret = gpio_pin_toggle_dt(&led);
    } 

    return 0;
}
