/*
 * GPIO header file
 *
 * This file contains the declarations for the GPIO 
 * initialization and GPIO related functions used in the VisionLead project.
 * 
 * Copyright 2024 University of Calgary and Garmin International Inc.
 *
 * Author: 
 *       James Platt <james.platt@ucalgary.ca>	
 */

#ifndef GPIO_H
#define GPIO_H 

typedef enum {
    D4,
    D5, 
    D6,
    RED_LED,
    BLUE_LED
} PINS;

void gpio_init(void);

int gpio_set_pin(PINS pin_to_set, int high_low);

#endif // GPIO_H
