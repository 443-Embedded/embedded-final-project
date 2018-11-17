#include "Timer.h"

void Timer_Init() {
	//Enable Timer2
	PCONP |= (1 << 22);
	
	//Change the mode of Timer2 to Timer Mode.
	TIMER2->CTCR = 0x00;
	
	//Change the mode of Timer3 to Counter Mode.
	TIMER0->CTCR = 0x01;
	TIMER1->CTCR = 0x01;
	
	//Disable Timer Counter and Prescale Counter for Timer0-1 and Timer2.
	TIMER2->TCR &= ~(1 << 0);
	TIMER0->TCR &= ~(1 << 0);
	TIMER1->TCR &= ~(1 << 0);
	
	//Reset Timer Counter and Prescale Counter for Timer2 and Timer0-1.
	TIMER2->TCR |= (1 << 1);
	TIMER0->TCR |= (1 << 1);
	TIMER1->TCR |= (1 << 1);
	
	//Change PR Register value for 1 microsecond incrementing
	TIMER2->PR = PERIPHERAL_CLOCK_FREQUENCY / 1000 - 1;
	TIMER0->PR = 0;
	TIMER1->PR = 0;
	
	TIMER2->MR0 = 250;
	TIMER2->MCR |= 3;
	
	//Clear pendings for Timer2.
	NVIC_ClearPendingIRQ(TIMER2_IRQn);
	
	//Set Priority Timer2 IRQ as 5.
	NVIC_SetPriority(TIMER2_IRQn,5);
	
	//Enable TIMER2_IRQn (Interrupt Request).
	NVIC_EnableIRQ(TIMER2_IRQn);
}

void TIMER2_Start() {
	//Remove the reset on counters of Timer2.
	TIMER2->TCR |= (1 << 1);
	
	//Enable Timer2 Counter and Prescale Counter for counting.
	TIMER2->TCR |= (1 << 0);	
	TIMER2->TCR &= ~(1 << 1);
}

void TIMER2_Stop() {
	//Stop Timer2 Counter 
	TIMER2->TCR &= ~(1 << 0);
	
	TURN_LEFT_FLAG = 0;
	TURN_RIGHT_FLAG = 0;
}

void TIMER2_IRQHandler() {
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
	
	//Clear the interrupt flag for Mat channel 1 event
	TIMER2->IR = (1 << 0);
}
