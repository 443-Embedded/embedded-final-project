#include "Timer.h"

void Timer_Init() {
	//Enable Timer2 and Timer3
	PCONP |= (1 << 22 | 1 << 23);
	
	//Set the function of P0_5 to T2_CAP1 
	IOCON_TIMER2_CAP1 |= 3;
	//Set the function of P0_4 to T2_CAP0 
	IOCON_TIMER2_CAP0 |= 3;
	
	//Change the mode of Timer3 to Timer Mode.
	TIMER3->CTCR = 0x00;
	
	//Change the mode of Timer2 to Counter Mode.
	TIMER2->CTCR = (1 << 2 | 1 << 3);
	
	TIMER2->CCR |= 1 << 0 | 1 << 2 | 1 << 3 | 1 << 5;
	
	//Disable Timer Counter and Prescale Counter for Timer0-1 and Timer2.
	TIMER3->TCR &= ~(1 << 0);
	TIMER2->TCR &= ~(1 << 0);
	
	//Reset Timer Counter and Prescale Counter for Timer2 and Timer0-1.
	TIMER3->TCR |= (1 << 1);
	TIMER2->TCR |= (1 << 1);
	
	//Change PR Register value for 1 microsecond incrementing
	TIMER3->PR = PERIPHERAL_CLOCK_FREQUENCY / 1000 - 1;
	
	TIMER3->MR0 = 250;
	TIMER3->MCR |= 3;
	
	TIMER2->MR0 = ROTATION_NUMBER_FOR_90_DEGREE * 6;
	TIMER2->MCR |= 0;
	
	//Clear pendings for Timer3 and Timer2.
	NVIC_ClearPendingIRQ(TIMER3_IRQn);
	NVIC_ClearPendingIRQ(TIMER2_IRQn);
	
	//Set Priority Timer3 and Timer2 IRQ as 5.
	NVIC_SetPriority(TIMER3_IRQn, 5);
	NVIC_SetPriority(TIMER2_IRQn, 5);
	
	//Enable TIMER3_IRQn  and TIMER2_IRQn (Interrupt Request).
	NVIC_EnableIRQ(TIMER3_IRQn);
	NVIC_EnableIRQ(TIMER2_IRQn);
}

void TIMER3_Start() {
	//Remove the reset on counters of Timer3.
	TIMER3->TCR |= (1 << 1);
	
	//Enable Timer3 Counter and Prescale Counter for counting.
	TIMER3->TCR &= ~(1 << 1);
	TIMER3->TCR |= (1 << 0);	
}

void TIMER3_Stop() {
	//Stop Timer3 Counter 
	TIMER3->TCR &= ~(1 << 0);
	
	TURN_LEFT_FLAG = 0;
	TURN_RIGHT_FLAG = 0;
}

void TIMER3_IRQHandler() {
	if(TURN_LEFT_FLAG != 0) {
		TURN_LEFT_FLAG++;
		LED_Change(0, TURN_LEFT_FLAG & 1);
		LED_Change(2, TURN_LEFT_FLAG & 1);
	}
	if(TURN_RIGHT_FLAG != 0) {
		TURN_RIGHT_FLAG++;
		LED_Change(1, TURN_RIGHT_FLAG & 1);
		LED_Change(3, TURN_RIGHT_FLAG & 1);
	}
	
	//Clear the interrupt flag for MAT channel 0 event
	TIMER3->IR = (1 << 0);
}

uint32_t tacho[2];

void TIMER2_Start() {
	//Reset tacho counts
	tacho[0] = tacho[1] = 0;
	
	//Remove the reset on counters of Timer2.
	TIMER2->TCR |= (1 << 1);
	
	//Enable Timer2 Counter and Prescale Counter for counting.
	TIMER2->TCR |= (1 << 0);	
	TIMER2->TCR &= ~(1 << 1);
}

void TIMER2_Stop() {
	//Stop Timer2 Counter 
	TIMER2->TCR &= ~(1 << 0);
}

void TIMER2_IRQHandler() {
	int IR_idx = 4;
	if(TIMER2->IR & (1 << 5))
		IR_idx = 5;
	tacho[IR_idx - 4]++;
	if(tacho[IR_idx - 4] >= ROTATION_NUMBER_FOR_90_DEGREE * 6) {
		MOTOR_Direction(IR_idx - 4, STOP);
		if(tacho[5 - IR_idx] >= ROTATION_NUMBER_FOR_90_DEGREE * 6)
			LED_Adjuster(STOP_LED);
	}
	//Clear the interrupt flag for CAP channel event
	TIMER2->IR = (1 << IR_idx);
}
