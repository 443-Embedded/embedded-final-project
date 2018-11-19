#include "GPIO.h"

void GPIO_Init() {
	PCONP |= 1 << 15;
	
	// Motor Directions and LED pins
	PORT0->DIR |= (1 << MOTOR_PINS[0][0] | 1 << MOTOR_PINS[0][1] | 1 << MOTOR_PINS[1][0] | 1 << MOTOR_PINS[1][1]);
	PORT0->DIR |= (1 << 23 | 1 << 24 | 1 << 25 | 1 << 26);

	// Joystick to INPUT GPIO TYPE
	PORT5->DIR &= ~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3 | 1 << 4);
}

uint32_t GPIO_PIN_Read(GPIO_TypeDef* PORT,uint32_t MASK) {
	return PORT->PIN & MASK;
}
