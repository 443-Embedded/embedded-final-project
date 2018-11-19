#include "LED.h"
#include "PWM.h"

void LED_Init() {
	PORT0->CLR = (1 << LED_PINS[0] |1 << LED_PINS[1] | 1 << LED_PINS[2] | 1 << LED_PINS[3]);
}

void LED_Change(uint32_t LED_NUMBER, uint32_t IS_ON) {
	if(IS_ON) {
		PORT0->SET = (1 << LED_PINS[LED_NUMBER]);
	} else {
		PORT0->CLR = (1 << LED_PINS[LED_NUMBER]);
	}
}
