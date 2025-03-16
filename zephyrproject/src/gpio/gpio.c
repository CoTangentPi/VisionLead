#include "gpio.h"

#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include "zephyr/logging/log_core.h"
#include <zephyr/kernel.h>
 
//get motor 0, motor 1, and buzzer 0 devices from device tree
const static struct gpio_dt_spec motor_0 = GPIO_DT_SPEC_GET(DT_NODELABEL(motor_0), gpios);
const static struct gpio_dt_spec motor_1 = GPIO_DT_SPEC_GET(DT_NODELABEL(motor_1), gpios);
const static struct gpio_dt_spec buzzer_0 = GPIO_DT_SPEC_GET(DT_NODELABEL(motor_1), gpios);
const static struct gpio_dt_spec red_led = GPIO_DT_SPEC_GET(DT_NODELABEL(red_led), gpios);
const static struct gpio_dt_spec blue_led = GPIO_DT_SPEC_GET(DT_NODELABEL(blue_led), gpios);

K_THREAD_STACK_DEFINE(motor_0_stack, MY_STACK_SIZE);
K_THREAD_STACK_DEFINE(motor_1_stack, MY_STACK_SIZE);
struct k_work_q motor_0_work_queue;
struct k_work_q motor_1_work_queue;

struct MOTOR_DEVICE {
    struct k_work work;
    PINS pin;
    PULSE_TYPE type;
};

struct MOTOR_DEVICE motor_0_work;
struct MOTOR_DEVICE motor_1_work;

/*
    * initalize gpio pins as outputs,
    * allows the motors, buzzer, and two leds
    * to be driven high or low
*/
void gpio_init(){
    printk("starting gpio init\n");   
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
	motor_0_stack,
	K_THREAD_STACK_SIZEOF(motor_0_stack),
	MY_PRIORITY,
	NULL
    );

    k_work_queue_start(
	&motor_1_work_queue,
	motor_1_stack,
	K_THREAD_STACK_SIZEOF(motor_1_stack),
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

void buzz_motor_async(struct k_work *item)
{
    //get information from device pointer, and buzz motor 
    //using the given pattern
    struct MOTOR_DEVICE *device =
        CONTAINER_OF(item, struct MOTOR_DEVICE, work);

    PULSE_TYPE pattern = device->type;
    PINS pin = device->pin;


    switch (pattern) {
	case MOTOR_SHORT_PULSE:
	    gpio_set_pin(pin, 0);
	    k_msleep(100);
	    gpio_set_pin(pin, 1);
	    break;
	case MOTOR_LONG_PULSE:
	    gpio_set_pin(pin, 0);
	    k_msleep(500);
	    gpio_set_pin(pin, 1);
	    break;
	case MOTOR_DOUBLE_PULSE:
	    gpio_set_pin(pin, 0);
	    k_msleep(100);
	    gpio_set_pin(pin, 1);
	    k_msleep(100);
	    gpio_set_pin(pin, 0);
	    k_msleep(100);
	    gpio_set_pin(pin, 1);
	    break;
    }
}


int pulse_motor(PINS motor_pin, PULSE_TYPE pattern){
    if(
        motor_pin != MOTOR_0 && 
        motor_pin != MOTOR_1 &&
	motor_pin != MOTOR_BOTH
    ){
        return GPIO_PIN_SET_ERROR;
    }
    
    // Check the pattern and perform the corresponding actions
    // sets up work for each motor device if it is available,
    // and then submits the work
    switch(motor_pin) {
	case MOTOR_0:
	    if(k_work_busy_get(&motor_0_work.work)){
		//motor 0 is currently busy
		printk("MOTOR 0 BUSY!\n");
		return GPIO_PIN_SET_ERROR;
	    }
	    switch (pattern) {
		case MOTOR_SHORT_PULSE:
		    motor_0_work.type = MOTOR_SHORT_PULSE;
		    break; 
		case MOTOR_LONG_PULSE:
		    motor_0_work.type = MOTOR_LONG_PULSE;
		    break;
		case MOTOR_DOUBLE_PULSE:
		    motor_0_work.type = MOTOR_DOUBLE_PULSE;
		    break;
		default:
		    return GPIO_PIN_SET_ERROR;
	    }
	    k_work_init(&motor_0_work.work, buzz_motor_async);
	    k_work_submit_to_queue(&motor_0_work_queue, &motor_0_work.work);
	    break;
	case MOTOR_1:
	    if(k_work_busy_get(&motor_1_work.work)){
		//motor 1 is currently busy
		printk("MOTOR 1 BUSY!\n");
		return GPIO_PIN_SET_ERROR;
	    }
	    switch (pattern) {
		case MOTOR_SHORT_PULSE:
		    motor_1_work.type = MOTOR_SHORT_PULSE;
		    break; 
		case MOTOR_LONG_PULSE:
		    motor_1_work.type = MOTOR_LONG_PULSE;
		    break;
		case MOTOR_DOUBLE_PULSE:
		    motor_1_work.type = MOTOR_DOUBLE_PULSE;
		    break;
		default:
		    return GPIO_PIN_SET_ERROR;
	    }
	    k_work_init(&motor_1_work.work, buzz_motor_async);
	    k_work_submit_to_queue(&motor_1_work_queue, &motor_1_work.work);
	    break;
	case MOTOR_BOTH:
	    if(
		k_work_busy_get(&motor_1_work.work) 
		||
		k_work_busy_get(&motor_0_work.work)
	    ){
		//motor 1 is currently busy
		printk("BOTH MOTORS BUSY!\n");
		return GPIO_PIN_SET_ERROR;
	    }
	    switch (pattern) {
		case MOTOR_SHORT_PULSE:
		    motor_1_work.type = MOTOR_SHORT_PULSE;
		    motor_0_work.type = MOTOR_SHORT_PULSE;
		    break; 
		case MOTOR_LONG_PULSE:
		    motor_1_work.type = MOTOR_LONG_PULSE;
		    motor_0_work.type = MOTOR_LONG_PULSE;
		    break;
		case MOTOR_DOUBLE_PULSE:
		    motor_1_work.type = MOTOR_DOUBLE_PULSE;
		    motor_0_work.type = MOTOR_DOUBLE_PULSE;
		    break;
		default:
		    return GPIO_PIN_SET_ERROR;
	    }
	    k_work_init(&motor_1_work.work, buzz_motor_async);
	    k_work_init(&motor_0_work.work, buzz_motor_async);
	    k_work_submit_to_queue(&motor_1_work_queue, &motor_1_work.work);
	    k_work_submit_to_queue(&motor_0_work_queue, &motor_0_work.work);
	    break;
    }
    
    return GPIO_PIN_SET_SUCCESS;
}

