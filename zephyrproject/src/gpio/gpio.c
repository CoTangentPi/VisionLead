#include "gpio.h"

#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>
 
//get motor 0, motor 1, and buzzer 0 devices from device tree
const static struct gpio_dt_spec motor_0 = GPIO_DT_SPEC_GET(DT_NODELABEL(motor_0), gpios);
const static struct gpio_dt_spec motor_1 = GPIO_DT_SPEC_GET(DT_NODELABEL(motor_1), gpios);
const static struct gpio_dt_spec buzzer_0 = GPIO_DT_SPEC_GET(DT_NODELABEL(motor_1), gpios);
const static struct gpio_dt_spec red_led = GPIO_DT_SPEC_GET(DT_NODELABEL(red_led), gpios);
const static struct gpio_dt_spec blue_led = GPIO_DT_SPEC_GET(DT_NODELABEL(blue_led), gpios);

//error states
#define GPIO_PIN_SET_ERROR -1
#define GPIO_PIN_SET_SUCCESS 1

/*
    * initalize gpio pins as outputs,
    * allows the motors, buzzer, and two leds
    * to be driven high or low
*/
void gpio_init(){
    
    //configure motors, buzzer and leds as output
    gpio_pin_configure_dt(&motor_0, GPIO_OUTPUT);
    gpio_pin_configure_dt(&motor_1, GPIO_OUTPUT);
    gpio_pin_configure_dt(&buzzer_0, GPIO_OUTPUT);
    gpio_pin_configure_dt(&red_led, GPIO_OUTPUT);
    gpio_pin_configure_dt(&blue_led, GPIO_OUTPUT);

    //set leds to off
    gpio_pin_set_dt(&red_led, 0);
    gpio_pin_set_dt(&blue_led, 0);

    //set motor to off
    gpio_pin_set_dt(&motor_0, 1);
    gpio_pin_set_dt(&motor_1, 1);

    //done
}

/*
    *
    * set value being ouptut on GPIO pins D4, D5, D6
    * returns 1 on sucess, -1 on failure
    *
*/
int gpio_set_pin(PINS pin_to_set, int high_low){

    //check if inputs are valid values
    if(
	    high_low != 0 && high_low != 1
    ){
	    return GPIO_PIN_SET_ERROR;
    }

    if(
        pin_to_set != MOTOR_0 && 
        pin_to_set != MOTOR_1 && 
        pin_to_set != BUZZER_0 &&
        pin_to_set != RED_LED &&
        pin_to_set != BLUE_LED
    ){
        return GPIO_PIN_SET_ERROR;
    }

    //inputs are valid, set pin
    switch(pin_to_set){
	
	case MOTOR_0:
	    gpio_pin_set_dt(&motor_0, high_low);
	    break;
	case MOTOR_1:
	    gpio_pin_set_dt(&motor_1, high_low);
	    break;
	case BUZZER_0:
	    gpio_pin_set_dt(&buzzer_0, high_low);
	    break;   
	case RED_LED:
	    gpio_pin_set_dt(&red_led, high_low);
	    break;
	case BLUE_LED:
	    gpio_pin_set_dt(&blue_led, high_low);
	    break;
    }

    return GPIO_PIN_SET_SUCCESS;
}

int pulse_motor(PINS motor_pin, int pattern){
    if(
        motor_pin != MOTOR_0 && 
        motor_pin != MOTOR_1
    ){
        return GPIO_PIN_SET_ERROR;
    }

    // Check the pattern and perform the corresponding actions
    switch (pattern) {
    case MOTOR_SHORT_PULSE:
        gpio_set_pin(motor_pin, 0);
        k_msleep(100);
        gpio_set_pin(motor_pin, 1);
        break;
    case MOTOR_LONG_PULSE:
        gpio_set_pin(motor_pin, 0);
        k_msleep(500);
        gpio_set_pin(motor_pin, 1);
        break;
    case MOTOR_DOUBLE_PULSE:
        gpio_set_pin(motor_pin, 0);
        k_msleep(100);
        gpio_set_pin(motor_pin, 1);
        k_msleep(100);
        gpio_set_pin(motor_pin, 0);
        k_msleep(100);
        gpio_set_pin(motor_pin, 1);
        break;
    default:
        return GPIO_PIN_SET_ERROR;
    }

    return GPIO_PIN_SET_SUCCESS;
}

