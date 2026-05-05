#include <rtthread.h>
#include <rtdevice.h>
#include <drv_gpio.h>
#include "drv_esp8266.h"

#ifdef BSP_USING_ESP8266

#ifdef BSP_ESP8266_USING_PIN_CONTROL

void esp8266_reset(void)
{
    rt_pin_mode(BSP_ESP8266_RST_PIN, PIN_MODE_OUTPUT);
    rt_pin_write(BSP_ESP8266_RST_PIN, PIN_LOW);
    rt_thread_mdelay(100);
    rt_pin_write(BSP_ESP8266_RST_PIN, PIN_HIGH);
    rt_thread_mdelay(500);
}

void esp8266_enable(void)
{
    rt_pin_mode(BSP_ESP8266_EN_PIN, PIN_MODE_OUTPUT);
    rt_pin_write(BSP_ESP8266_EN_PIN, PIN_HIGH);
}

void esp8266_disable(void)
{
    rt_pin_mode(BSP_ESP8266_EN_PIN, PIN_MODE_OUTPUT);
    rt_pin_write(BSP_ESP8266_EN_PIN, PIN_LOW);
}

static int rt_hw_esp8266_pin_init(void)
{
    esp8266_enable();
    esp8266_reset();
    return RT_EOK;
}
INIT_COMPONENT_EXPORT(rt_hw_esp8266_pin_init);

static void esp8266_reset_cmd(int argc, char *argv[])
{
    esp8266_reset();
    rt_kprintf("ESP8266 reset done\n");
}
MSH_CMD_EXPORT(esp8266_reset, reset esp8266 module);

#endif

#endif
