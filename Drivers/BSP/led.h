#ifndef __LED_H
#define __LED_H

#define LED1 GPIO_PIN_8
#define LED2 GPIO_PIN_9

#include "sys.h"

void led_init(void);
void led_on(uint16_t led);
void led_off(uint16_t led);
void led_toggle(uint16_t led);

#endif
