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

#define MY_STACK_SIZE 500
#define MY_PRIORITY 5

//error states
#define GPIO_PIN_SET_ERROR -1
#define GPIO_PIN_SET_SUCCESS 1

// Define motor pulse partten duration
#define MOTOR_SHORT_PULSE_TIME 100
#define MOTOR_MED_PULSE_TIME 300
#define MOTOR_LONG_PULSE_TIME 1000

//gpio devices
typedef enum {
    MOTOR_0,
    MOTOR_1, 
    MOTOR_BOTH,
    BUZZER_0,
    RED_LED,
    BLUE_LED
} PINS;

//motor pulse types.
typedef enum {
    MOTOR_SHORT_PULSE,
    MOTOR_MED_PULSE,
    MOTOR_DOUBLE_PULSE,
    MOTOR_S_L_S_PULSE, //short long short pulse
    MOTOR_LONG_PULSE, //hold for extened period
    MOTOR_DOUBLE_LONG_PULSE //hold for extened period
} PULSE_TYPE;

/*
    * initalize gpio pins as outputs,
    * allows the motors, buzzer, and two leds
    * to be driven high or low
*/
void gpio_init(void);
/*
    * set value being ouptut on GPIO PINS
    * returns 1 on sucess, -1 on failure
*/
int gpio_set_pin(PINS pin_to_set, int high_low);
/*
    * drive one of the two motors for a 
    * set amount of time, using one of three patterns
*/
int pulse_motor(PINS motor_pin, PULSE_TYPE pattern);

#endif // GPIO_H
