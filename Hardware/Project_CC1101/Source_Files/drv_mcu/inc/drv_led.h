#ifndef __DRV_LED_H__
#define __DRV_LED_H__

#include "stm8l10x_gpio.h"

#define LED_RED_GPIO_PORT GPIOA
#define LED_RED_GPIO_PIN GPIO_Pin_2

#define LED_GREEN_GPIO_PORT GPIOA
#define LED_GREEN_GPIO_PIN GPIO_Pin_3

typedef enum LedPort
{
    LED_RED = 0,
    LED_GREEN
} LedPortType;

void drv_led_init(void);
void drv_led_on(LedPortType LedPort);
void drv_led_off(LedPortType LedPort);
void drv_led_flashing(LedPortType LedPort);
void led_switch(int mySwitch);

#define led_red_on() drv_led_on(LED_RED)
#define led_red_off() drv_led_off(LED_RED)
#define led_red_flashing() drv_led_flashing(LED_RED)

#define led_green_on() drv_led_on(LED_GREEN)
#define led_green_off() drv_led_off(LED_GREEN)
#define led_green_flashing() drv_led_flashing(LED_GREEN)

#endif
