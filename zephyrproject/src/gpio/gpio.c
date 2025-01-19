#include "gpio.h"

#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>

//Device tree node and name for gpio1
#define GPIO_1_NODE DT_NODELABEL(gpio1)
#define GPIO_1_NAME DEVICE_DT_NAME(GPIO_1_NODE)

//Device tree node and name for gpio0
#define GPIO_0_NODE DT_NODELABEL(gpio0)
#define GPIO_0_NAME DEVICE_DT_NAME(GPIO_0_NODE)

//pin numbers for D4, D5, D6
#define GPIO_1_PIN_D4	15
#define GPIO_1_PIN_D5	13
#define GPIO_1_PIN_D6	14
#define GPIO_0_PIN_LED_RED 24
#define GPIO_0_PIN_LED_BLUE 6

//error states
#define GPIO_PIN_SET_ERROR -1
#define GPIO_PIN_SET_SUCCESS 1

/*
    *
    * initalize gpio pins as outputs, currently only enables 
    * D4, D5, D6, as those are used in the latest schematic from
    * hardware team
    *
*/
void gpio_init(){
    
    //get device struct from device tree for gpio0
    const struct device * gpio_0_device = device_get_binding(GPIO_0_NAME); 

    //get device struct from device tree for gpio1
    const struct device * gpio_1_device = device_get_binding(GPIO_1_NAME);

    //configure pins D4, D5, and D6 as output
    gpio_pin_configure(gpio_1_device, GPIO_1_PIN_D4, GPIO_OUTPUT);
    gpio_pin_configure(gpio_1_device, GPIO_1_PIN_D5, GPIO_OUTPUT);
    gpio_pin_configure(gpio_1_device, GPIO_1_PIN_D6, GPIO_OUTPUT);

    //configure RED_LED (led0) and BLUE_LED (led1) as output
    gpio_pin_configure(gpio_0_device, GPIO_0_PIN_LED_RED, GPIO_OUTPUT);
    gpio_pin_configure(gpio_0_device, GPIO_0_PIN_LED_BLUE, GPIO_OUTPUT);

    //set leds to high (off)
    gpio_pin_set(gpio_0_device, GPIO_0_PIN_LED_BLUE, 1);
    gpio_pin_set(gpio_0_device, GPIO_0_PIN_LED_RED, 1);

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
	pin_to_set != D4 && 
	pin_to_set != D5 && 
	pin_to_set != D6 &&
	pin_to_set != RED_LED &&
	pin_to_set != BLUE_LED
    ){
	return GPIO_PIN_SET_ERROR;
    }

    //get device struct from device tree
    const struct device * gpio_1_device = device_get_binding(GPIO_1_NAME);

    //get device struct from device tree for gpio0
    const struct device * gpio_0_device = device_get_binding(GPIO_0_NAME); 

    //inputs are valid, set pin
    switch(pin_to_set){
	
	case D4:
	    gpio_pin_set(gpio_1_device, GPIO_1_PIN_D4, high_low);
	    break;
	case D5:
	    gpio_pin_set(gpio_1_device, GPIO_1_PIN_D5, high_low);
	    break;
	case D6:
	    gpio_pin_set(gpio_1_device, GPIO_1_PIN_D5, high_low);
	    break;   
	case RED_LED:
	    gpio_pin_set(gpio_0_device, GPIO_0_PIN_LED_RED, high_low);
	    break;
	case BLUE_LED:
	    gpio_pin_set(gpio_0_device, GPIO_0_PIN_LED_BLUE, high_low);
	    break;
    }


    return GPIO_PIN_SET_SUCCESS;

}

