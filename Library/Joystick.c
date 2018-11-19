#include "Joystick.h"

uint8_t Joystick_Left_Pressed(void) {
	if(GPIO_PIN_Read(JOYSTICK_PORT,JOYSTICK_LEFT_MASK) == 0) {
		return 1;
	}
	return 0;
}

uint8_t Joystick_Down_Pressed(void) {
	if(GPIO_PIN_Read(JOYSTICK_PORT,JOYSTICK_DOWN_MASK) == 0) {
		return 1;
	}
	return 0;
}

uint8_t Joystick_Up_Pressed(void) {
	if(GPIO_PIN_Read(JOYSTICK_PORT,JOYSTICK_UP_MASK) == 0) {
		return 1;
	}
	return 0;
}

uint8_t Joystick_Center_Pressed(void) {
	if(GPIO_PIN_Read(JOYSTICK_PORT,JOYSTICK_CENTER_MASK) == 0) {
		return 1;
	}
	return 0;
}

uint8_t Joystick_Right_Pressed(void) {
	if(GPIO_PIN_Read(JOYSTICK_PORT,JOYSTICK_RIGHT_MASK) == 0) {
		return 1;
	}
	return 0;
}
