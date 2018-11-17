#include "GPIO.h"

void GPIO_Init() {
	PCONP |= 1 << 15;
	
	// Motor Directions and LED pins
 	PORT0->DIR |= (1 << 9 | 1 << 8 | 1 << 7 | 1 << 6 | 1 << 23 | 1 << 24 | 1 << 25 | 1 << 26);
	PORT0->MASK = ~(1 << 9 | 1 << 8 | 1 << 7 | 1 << 6 | 1 << 23 | 1 << 24 | 1 << 25 | 1 << 26);
	
	// Joystick to INPUT GPIO TYPE
	PORT5->DIR &= ~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3 | 1 << 4);
	PORT5->MASK = ~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3 | 1 << 4);
}

uint32_t GPIO_PIN_Read(GPIO_TypeDef* PORT,uint32_t MASK) {
	return PORT->PIN &= MASK;
}
