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
    MOTOR_0,
    MOTOR_1, 
    BUZZER_0,
    RED_LED,
    BLUE_LED
} PINS;

// Define the patterns
#define MOTOR_SHORT_PULSE 1
#define MOTOR_LONG_PULSE 2
#define MOTOR_DOUBLE_PULSE 3

void gpio_init(void);

int gpio_set_pin(PINS pin_to_set, int high_low);

int pulse_motor(PINS motor_pin, int pattern);

#endif // GPIO_H
