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

K_THREAD_STACK_DEFINE(my_stack_area, MY_STACK_SIZE);
struct k_work_q motor_0_work_queue;
struct k_work_q motor_1_work_queue;

struct MOTOR_DEVICE {
    struct k_work work;
    PINS pin;
};

struct MOTOR_DEVICE motor_0_work;
struct MOTOR_DEVICE motor_1_work;

/*
    * initalize gpio pins as outputs,
    * allows the motors, buzzer, and two leds
    * to be driven high or low
*/
void gpio_init(){
       
    k_work_queue_init(&motor_0_work_queue);
    k_work_queue_init(&motor_1_work_queue);
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
    
    k_work_queue_start(
	&motor_0_work_queue,
	my_stack_area,
	K_THREAD_STACK_SIZEOF(my_stack_area),
	MY_PRIORITY,
	NULL
    );

    k_work_queue_start(
	&motor_1_work_queue,
	my_stack_area,
	K_THREAD_STACK_SIZEOF(my_stack_area),
	MY_PRIORITY,
	NULL
    );

    motor_0_work.pin = MOTOR_0;
    motor_1_work.pin = MOTOR_1;
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

extern void buzz_motor(void * motor, void * pulse_type, void * p3) {
    
    const PINS motor_pin = *((PINS *) motor);
    const PULSE_TYPE pulse_length = *((PULSE_TYPE *) pulse_type);
}

void print_error(struct k_work *item)
{
    struct MOTOR_DEVICE *the_device =
        CONTAINER_OF(item, struct MOTOR_DEVICE, work);
    printk("Got error on device %s\n", the_device->pin);
}


int pulse_motor(PINS motor_pin, PULSE_TYPE pattern){
    if(
        motor_pin != MOTOR_0 && 
        motor_pin != MOTOR_1
    ){
        return GPIO_PIN_SET_ERROR;
    }
    
    // Check the pattern and perform the corresponding actions

    switch(motor_pin) {
	case MOTOR_0:
	    if(k_work_busy_get(&motor_0_work)){
		//motor 0 is currently busy
		return GPIO_PIN_SET_ERROR;
	    }
	    switch (pattern) {
		case MOTOR_SHORT_PULSE:
		    k_work_init(&motor_0_work.work, print_error);
		    break; 
		case MOTOR_LONG_PULSE:
		    k_work_init(&motor_0_work.work, print_error);
		    break;
		case MOTOR_DOUBLE_PULSE:
		    k_work_init(&motor_0_work.work, print_error);
		    break;
		default:
		    return GPIO_PIN_SET_ERROR;
	    }
	    break;
	case MOTOR_1:
	    if(k_work_busy_get(&motor_1_work)){
		//motor 1 is currently busy
		return GPIO_PIN_SET_ERROR;
	    }
	    switch (pattern) {
		case MOTOR_SHORT_PULSE:
		    k_work_init(&motor_1_work.work, print_error);
		    break; 
		case MOTOR_LONG_PULSE:
		    k_work_init(&motor_1_work.work, print_error);
		    break;
		case MOTOR_DOUBLE_PULSE:
		    k_work_init(&motor_1_work.work, print_error);
		    break;
		default:
		    return GPIO_PIN_SET_ERROR;
	    }
	    break;
    }

    return GPIO_PIN_SET_SUCCESS;
}

