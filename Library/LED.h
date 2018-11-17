#ifndef LED_H
#define LED_H

#include "LPC407x_8x_177x_8x.h"
#include "SystemStructures.h"
#include "GPIO.h"

void LED_Init(void);
void LED_Change(uint32_t LED_NUMBER, uint32_t IS_ON);

#endif
