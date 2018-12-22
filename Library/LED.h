#ifndef LED_H
#define LED_H

#include "LPC407x_8x_177x_8x.h"
#include "SystemStructures.h"
#include "GPIO.h"

#define IOCON_1_ADDR	0x4002C094
#define IOCON_1	*((volatile uint32_t*)(IOCON_1_ADDR))
	
#define IOCON_2_ADDR	0x4002C098
#define IOCON_2	*((volatile uint32_t*)(IOCON_2_ADDR))
	
#define IOCON_3_ADDR	0x4002C09C
#define IOCON_3	*((volatile uint32_t*)(IOCON_3_ADDR))
	
#define IOCON_4_ADDR	0x4002C0AC
#define IOCON_4 *((volatile uint32_t*)(IOCON_4_ADDR))

void LED_Init(void);
void LED_Change(uint32_t LED_NUMBER, uint32_t IS_ON);

#endif
