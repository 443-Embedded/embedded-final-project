#include "Timer.h"

void Timer_Init() {
	//Enable Timer2 and Timer3
	PCONP |= (1 << 22 | 1 << 23);
	
	// TIMER 2
	//Set the function of P0_5 to T2_CAP1 
	IOCON_TIMER2_CAP1 |= 3;
	//Set the function of P0_4 to T2_CAP0 
	IOCON_TIMER2_CAP0 |= 3;
	
	//Enable capture rising edge and interrupt for both CAP0 and CAP1
	TIMER2->CCR |= 1 << 0 | 1 << 2 | 1 << 3 | 1 << 5;
	
	//Disable Timer Counter and Prescale Counter for Timer2.
	TIMER2->TCR &= ~(1 << 0);
	
	//Reset Timer Counter and Prescale Counter for Timer2.
	TIMER2->TCR |= (1 << 1);
	
	//Change the mode of Timer2 to Timer Mode.
	TIMER2->CTCR = 0x00;
	
	// TIMER 3
	//Change the mode of Timer3 to Timer Mode.
	TIMER3->CTCR = 0x00;
	
	//Disable Timer Counter and Prescale Counter for Timer2 and Timer3.
	TIMER3->TCR &= ~(1 << 0);
	
	//Reset Timer Counter and Prescale Counter for Timer2 and Timer3.
	TIMER3->TCR |= (1 << 1);
	
	//Change PR Register value for 1 microsecond incrementing
	TIMER3->PR = PERIPHERAL_CLOCK_FREQUENCY / 1000 - 1;
	
	//Set MR0 to 250 in order to blink LED in every 250ms.
	TIMER3->MR0 = 250;
	//After match occurs, interrupt on MR0 and reset MR0
	TIMER3->MCR |= 3;
	
	//Clear pendings for Timer2 and Timer3.
	NVIC_ClearPendingIRQ(TIMER3_IRQn);
	NVIC_ClearPendingIRQ(TIMER2_IRQn);
	
	//Set Priority Timer2 and Timer3 IRQ as 5.
	NVIC_SetPriority(TIMER3_IRQn, 5);
	NVIC_SetPriority(TIMER2_IRQn, 5);
	
	//Enable TIMER2_IRQn  and TIMER3_IRQn (Interrupt Request).
	NVIC_EnableIRQ(TIMER3_IRQn);
	NVIC_EnableIRQ(TIMER2_IRQn);
}

// Starts timer3 for LEDs
void TIMER3_Start() {
	//Reset Timer Counter and Prescale Counter for Timer3.
	TIMER3->TCR |= (1 << 1);
	
	// Enable Counter and remove reset on Timer3.
	TIMER3->TCR |= (1 << 0);
	TIMER3->TCR &= ~(1 << 1);
}

// Stops timer3 for LEDs
void TIMER3_Stop() {
	//Disable counter for timer3
	TIMER3->TCR &= ~(1 << 0);
	
	//Flag for direction of rotation, if it is not 0 then rotation is in that way.
	TURN_LEFT_FLAG = 0;
	TURN_RIGHT_FLAG = 0;
}

/*
* Every 250 ms, this handler will be called. Either rotation is left or right, we increment flag by 1 at each call.
* Then, we call corresponding LEDs and blink occurs.
*/
void TIMER3_IRQHandler() {
	if(TURN_LEFT_FLAG != 0) {	
		TURN_LEFT_FLAG++;	
		LED_Change(0, TURN_LEFT_FLAG & 1);
		LED_Change(2, TURN_LEFT_FLAG & 1);
	}
	else if(TURN_RIGHT_FLAG != 0) {	
		TURN_RIGHT_FLAG++;
		LED_Change(1, TURN_RIGHT_FLAG & 1);
		LED_Change(3, TURN_RIGHT_FLAG & 1);
	}
	
	//Clear the interrupt flag for MAT channel 0 event
	TIMER3->IR = (1 << 0);
}

// Keeps the number of holes detected on wheels by speed sensor.
uint32_t tacho[2];

// Starts timer2 for speed sensor
void TIMER2_Start() {
	//Reset tacho counts
	tacho[0] = tacho[1] = 0;
	
	//Reset Timer Counter and Prescale Counter for Timer2.
	TIMER2->TCR |= (1 << 1);
	
	//Enable Counter and remove reset on Timer2.
	TIMER2->TCR |= (1 << 0);	
	TIMER2->TCR &= ~(1 << 1);
}

// Stops timer2 for speed sensor
void TIMER2_Stop() {
	//Stop Timer2 Counter 
	TIMER2->TCR &= ~(1 << 0);
}
/*
* Depending on the capture register, we increment tacho count of corresponding motor. If a motors completes its 
* total rotation then stops the wheel, if both completes turns off LEDs.
*/
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
