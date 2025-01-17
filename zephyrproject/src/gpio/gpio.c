#include "gpio.h"

#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>

//Device tree node and name for gpio1
#define GPIO_1_NODE DT_NODELABEL(gpio1)
#define GPIO_1_NAME DEVICE_DT_NAME(GPIO_NODE)

//pin numbers for D4, D5, D6
#define GPIO_1_PIN_D4	15
#define GPIO_1_PIN_D5	13
#define GPIO_1_PIN_D6	14

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
   
    //get device struct from device tree
    const struct device * gpio_1_device = device_get_binding(GPIO_1_NAME);

    //configure pins D4, D5, and D6 as output
    gpio_pin_configure(gpio_1_device, GPIO_1_PIN_D4, GPIO_OUTPUT);
    gpio_pin_configure(gpio_1_device, GPIO_1_PIN_D5, GPIO_OUTPUT);
    gpio_pin_configure(gpio_1_device, GPIO_1_PIN_D6, GPIO_OUTPUT);

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
	return GPIO_SET_PIN_ERROR;
    }

    if(
	pin_to_set != D4 && pin_to_set != D5 && pin_to_set != D6
    ){
	return GPIO_SET_PIN_ERROR;
    }

    //get device struct from device tree
    const struct device * gpio_1_device = device_get_binding(GPIO_1_NAME);

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
    }


    return GPIO_SET_PIN_SUCCESS;

}

