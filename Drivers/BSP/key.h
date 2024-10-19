#ifndef __KEY_H
#define __KEY_H

#define KEY1 GPIO_PIN_0
#define KEY2 GPIO_PIN_1

#include "sys.h"

void key_init(void);
uint8_t key_check(uint16_t key);

#endif
