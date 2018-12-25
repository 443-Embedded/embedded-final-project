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
	
	EXT->EXTINT |= 1;
}
