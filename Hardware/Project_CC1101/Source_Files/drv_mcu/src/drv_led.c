#include "drv_led.h"

/**
  * @brief :LED初始化
  * @param :无
  * @note  :无
  * @retval:无
  */
void drv_led_init(void)
{
    GPIO_Init(LED_RED_GPIO_PORT, LED_RED_GPIO_PIN, GPIO_Mode_Out_PP_Low_Fast);
    GPIO_Init(LED_GREEN_GPIO_PORT, LED_GREEN_GPIO_PIN, GPIO_Mode_Out_PP_Low_Fast);
}

/**
  * @brief :LED亮
  * @param :
  *			@LedPort:LED选择，红色或绿色
  * @note  :无
  * @retval:无
  */
void drv_led_on(LedPortType LedPort)
{
    if (LED_RED == LedPort)
    {
        GPIO_SetBits(LED_RED_GPIO_PORT, LED_RED_GPIO_PIN);
    }
    else
    {
        GPIO_SetBits(LED_GREEN_GPIO_PORT, LED_GREEN_GPIO_PIN);
    }
}

/**
  * @brief :LED灭
  * @param :
  *			@LedPort:LED选择，红色或绿色
  * @note  :无
  * @retval:无
  */
void drv_led_off(LedPortType LedPort)
{
    if (LED_RED == LedPort) //LED_RED
    {
        GPIO_ResetBits(LED_RED_GPIO_PORT, LED_RED_GPIO_PIN);
    }
    else
    {
        GPIO_ResetBits(LED_GREEN_GPIO_PORT, LED_GREEN_GPIO_PIN);
    }
}

/**
  * @brief :LED闪烁
  * @param :
  *			@LedPort:LED选择，红色或绿色
  * @note  :无
  * @retval:无
  */
void drv_led_flashing(LedPortType LedPort)
{
    if (LED_RED == LedPort)
    {
        GPIO_ToggleBits(LED_RED_GPIO_PORT, LED_RED_GPIO_PIN);
    }
    else
    {
        GPIO_ToggleBits(LED_GREEN_GPIO_PORT, LED_GREEN_GPIO_PIN);
    }
}

/**
  * @brief: led switch,
  *                     num | green | red
  *                      00 |   off | off
  *                      01 |   off |  on
  *                      10 |    on | off
  *                      11 |    on |  on
  */
void led_switch(int mySwitch)
{
    int state = mySwitch & 0x00000003;
    if (state & 1)
        led_red_on();
    else
        led_red_off();
    if (state & 2)
        led_green_on();
    else
        led_green_off();
}
