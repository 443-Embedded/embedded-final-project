#include "GPIO.h"

void GPIO_Init() {
	// Enable GPIO
	PCONP |= 1 << 15;
	
	// Sets IOCON functions as GPIO for Joystick.
	IOCON_LEFT &= ~(7);
	IOCON_DOWN &= ~(7);
	IOCON_RIGHT &= ~(7);
	IOCON_CENTER &= ~(7);
	IOCON_UP &= ~(7);
	
	// Sets Motor Directions and LED pins
	PORT0->DIR |= (1 << MOTOR_PINS[0][0] | 1 << MOTOR_PINS[0][1] | 1 << MOTOR_PINS[1][0] | 1 << MOTOR_PINS[1][1]);
	PORT0->DIR |= (1 << LED_PINS[0] | 1 << LED_PINS[1] | 1 << LED_PINS[2] | 1 << LED_PINS[3]);

	// Joystick to INPUT GPIO TYPE
	PORT5->DIR &= ~(1 << 0 | 1 << 1 | 1 << 2 | 1 << 3 | 1 << 4);
}

void GPIO_DIR_Write(GPIO_TypeDef* PORT,uint32_t MASK,uint8_t value) {
	if(value == 0) {
		PORT->DIR &= ~MASK;
	}
	else {
		PORT->DIR |= MASK;
	}
}

void GPIO_PIN_Write(GPIO_TypeDef* PORT,uint32_t MASK,uint8_t value) {
	if(value == 0) {
		PORT->PIN &= ~MASK;
	}
	else {
		PORT->PIN |= MASK;
	}
}

// Returns given pin value for GPIOs
uint32_t GPIO_PIN_Read(GPIO_TypeDef* PORT,uint32_t MASK) {
	return PORT->PIN & MASK;
}
