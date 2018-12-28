#include "External.h"

void External_Init(){
	IOCON_PUSH_BUTTON |= 1;
	
	EXT->EXMODE |= 1;
	
	NVIC_EnableIRQ(EINT0_IRQn);
}

void EINT0_IRQHandler(void){
	if (START_MODE == AUTO) {
		changeStartMode(MANUAL);
	} else {
		changeStartMode(AUTO);
	}
	
	MOTOR_Direction(1, STOP);
	MOTOR_Direction(0, STOP);
	LED_Adjuster(STOP_LED);
	
	EXT->EXTINT |= 1;
}
