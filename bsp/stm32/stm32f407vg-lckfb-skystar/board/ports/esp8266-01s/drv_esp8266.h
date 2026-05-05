#ifndef DRV_ESP8266_H
#define DRV_ESP8266_H

#include <rtthread.h>
#include <rtdevice.h>
#include <drv_gpio.h>

#ifdef BSP_ESP8266_USING_PIN_CONTROL

#if defined(BSP_ESP8266_RST_PORT_A)
#define BSP_ESP8266_RST_PIN         GET_PIN(A, BSP_ESP8266_RST_PIN_NUM)
#elif defined(BSP_ESP8266_RST_PORT_B)
#define BSP_ESP8266_RST_PIN         GET_PIN(B, BSP_ESP8266_RST_PIN_NUM)
#elif defined(BSP_ESP8266_RST_PORT_C)
#define BSP_ESP8266_RST_PIN         GET_PIN(C, BSP_ESP8266_RST_PIN_NUM)
#elif defined(BSP_ESP8266_RST_PORT_D)
#define BSP_ESP8266_RST_PIN         GET_PIN(D, BSP_ESP8266_RST_PIN_NUM)
#elif defined(BSP_ESP8266_RST_PORT_E)
#define BSP_ESP8266_RST_PIN         GET_PIN(E, BSP_ESP8266_RST_PIN_NUM)
#elif defined(BSP_ESP8266_RST_PORT_F)
#define BSP_ESP8266_RST_PIN         GET_PIN(F, BSP_ESP8266_RST_PIN_NUM)
#elif defined(BSP_ESP8266_RST_PORT_G)
#define BSP_ESP8266_RST_PIN         GET_PIN(G, BSP_ESP8266_RST_PIN_NUM)
#elif defined(BSP_ESP8266_RST_PORT_H)
#define BSP_ESP8266_RST_PIN         GET_PIN(H, BSP_ESP8266_RST_PIN_NUM)
#else
#define BSP_ESP8266_RST_PIN         GET_PIN(A, 6)
#endif

#if defined(BSP_ESP8266_EN_PORT_A)
#define BSP_ESP8266_EN_PIN          GET_PIN(A, BSP_ESP8266_EN_PIN_NUM)
#elif defined(BSP_ESP8266_EN_PORT_B)
#define BSP_ESP8266_EN_PIN          GET_PIN(B, BSP_ESP8266_EN_PIN_NUM)
#elif defined(BSP_ESP8266_EN_PORT_C)
#define BSP_ESP8266_EN_PIN          GET_PIN(C, BSP_ESP8266_EN_PIN_NUM)
#elif defined(BSP_ESP8266_EN_PORT_D)
#define BSP_ESP8266_EN_PIN          GET_PIN(D, BSP_ESP8266_EN_PIN_NUM)
#elif defined(BSP_ESP8266_EN_PORT_E)
#define BSP_ESP8266_EN_PIN          GET_PIN(E, BSP_ESP8266_EN_PIN_NUM)
#elif defined(BSP_ESP8266_EN_PORT_F)
#define BSP_ESP8266_EN_PIN          GET_PIN(F, BSP_ESP8266_EN_PIN_NUM)
#elif defined(BSP_ESP8266_EN_PORT_G)
#define BSP_ESP8266_EN_PIN          GET_PIN(G, BSP_ESP8266_EN_PIN_NUM)
#elif defined(BSP_ESP8266_EN_PORT_H)
#define BSP_ESP8266_EN_PIN          GET_PIN(H, BSP_ESP8266_EN_PIN_NUM)
#else
#define BSP_ESP8266_EN_PIN          GET_PIN(A, 7)
#endif

void esp8266_reset(void);
void esp8266_enable(void);
void esp8266_disable(void);

#endif

#endif
