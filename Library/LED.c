#include "LED.h"
#include "PWM.h"

void LED_Init() {
	PORT0->CLR = (1 << 23 | 1 << 24 | 1 << 25 | 1 << 26);
}

void LED_Change(uint32_t LED_NUMBER, uint32_t IS_ON) {
	if(IS_ON) {
		PORT0->SET = (1 << (23 + LED_NUMBER));
	} else {
		PORT0->CLR = (1 << (23 + LED_NUMBER));
	}
}
