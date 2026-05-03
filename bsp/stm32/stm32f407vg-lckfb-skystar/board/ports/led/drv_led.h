#ifndef DRV_LED_H
#define DRV_LED_H

#include <rtthread.h>
#include <rtdevice.h>
#include <drv_gpio.h>

#define LED_PIN                 GET_PIN(B, 2)
#define LED_ACTIVE_HIGH         1

#if LED_ACTIVE_HIGH
#define LED_ON()                rt_pin_write(LED_PIN, PIN_HIGH)
#define LED_OFF()               rt_pin_write(LED_PIN, PIN_LOW)
#else
#define LED_ON()                rt_pin_write(LED_PIN, PIN_LOW)
#define LED_OFF()               rt_pin_write(LED_PIN, PIN_HIGH)
#endif

enum led_mode
{
    LED_MODE_GPIO = 0,
    LED_MODE_PWM  = 1,
};

void led_set_mode(enum led_mode mode);
enum led_mode led_get_mode(void);
void led_on(void);
void led_off(void);
void led_toggle(void);
void led_set_brightness(rt_uint8_t brightness);
void led_set_blink_interval(rt_uint32_t interval_ms);
void led_set_breathe_period(rt_uint32_t period_ms);

#endif
