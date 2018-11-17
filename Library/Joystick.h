#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "LPC407x_8x_177x_8x.h"

#include "GPIO.h"

#define JOYSTICK_PORT	PORT5

#define JOYSTICK_LEFT_MASK		((uint32_t) 1 << 0)
#define JOYSTICK_DOWN_MASK		((uint32_t) 1 << 1)
#define JOYSTICK_UP_MASK			((uint32_t) 1 << 2)
#define JOYSTICK_CENTER_MASK	((uint32_t) 1 << 3)
#define JOYSTICK_RIGHT_MASK		((uint32_t) 1 << 4)

#define IOCON_LEFT_ADDR	0x4002C280
#define IOCON_LEFT	*((volatile uint32_t*)(IOCON_LEFT_ADDR))

#define IOCON_DOWN_ADDR	0x4002C284
#define IOCON_DOWN	*((volatile uint32_t*)(IOCON_DOWN_ADDR))

#define IOCON_UP_ADDR	0x4002C288
#define IOCON_UP	*((volatile uint32_t*)(IOCON_UP_ADDR))
	
#define IOCON_CENTER_ADDR	0x4002C28C
#define IOCON_CENTER	*((volatile uint32_t*)(IOCON_CENTER_ADDR))
	
#define IOCON_RIGHT_ADDR 0x4002C290
#define IOCON_RIGHT	*((volatile uint32_t*)(IOCON_RIGHT_ADDR))

void Joystick_Init(void);

uint8_t Joystick_Left_Pressed(void);
uint8_t Joystick_Down_Pressed(void);
uint8_t Joystick_Up_Pressed(void);
uint8_t Joystick_Center_Pressed(void);
uint8_t Joystick_Right_Pressed(void);

#endif
