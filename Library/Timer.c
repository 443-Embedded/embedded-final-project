#include "Timer.h"

uint16_t count = 0;

void Timer0_Init() {
	//Change the mode to Timer Mode.
	TIMER0->CTCR = 0x00;
	
	//Change PR Register value for 1 millisecond incrementing
	TIMER0->PR = PERIPHERAL_CLOCK_FREQUENCY / 1000 - 1;
	
	//Set MR0 to 10.
	TIMER0->MR0 = 10;
	
	//After match occurs, interrupt on MR0 and reset MR0
	TIMER0->MCR |= 3;
	
	//Clear pendings
	NVIC_ClearPendingIRQ(TIMER0_IRQn);
	
	//Set Priority to 5.
	NVIC_SetPriority(TIMER0_IRQn, 5);
	
	//Enable Interrupts.
	NVIC_EnableIRQ(TIMER0_IRQn);
}

void Timer0_Start() {
	//Reset Timer Counter and Prescale Counter
	TIMER0->TCR |= (1 << 1);
	
	//Enable Timer Counter and Prescale Counter
	TIMER0->TCR |= (1 << 0);
	
	//Clear reset of timer and prescale counter
	TIMER0->TCR &= ~(1 << 1);
}

void Timer0_Stop() {
	//Disable the counters
	TIMER0->TCR &= ~(1 << 0);
}

/*
* Every 10 ms, this handler will be called.
*/
void TIMER0_IRQHandler() {
	count++;
	if (COMM_TYPE == WIFI_COMM && count == 300) {
		count = 0;
		wifi_check();
	}
	ADC_Start();
	
	//Clear the interrupt flag for MAT channel 0 event
	TIMER0->IR = (1 << 0);
}

void Timer1_Init() {
	//Change the mode of Timer1 to Timer Mode.
	TIMER1->CTCR = 0x00;
	
	//Disable Timer Counter and Prescale Counter for Timer1.
	TIMER1->TCR &= ~(1 << 0);
	
	//Reset Timer Counter and Prescale Counter for Timer1.
	TIMER1->TCR |= (1 << 1);
	
	//Change PR Register value for 1 millisecond incrementing
	TIMER1->PR = PERIPHERAL_CLOCK_FREQUENCY / 1000 - 1;
	
	//Set MR0 to 250 in order to blink LED in every 250ms.
	TIMER1->MR0 = 250;
	//After match occurs, interrupt on MR0 and reset MR0
	TIMER1->MCR |= 3;
	
	//Clear pendings for Timer1.
	NVIC_ClearPendingIRQ(TIMER1_IRQn);
	
	//Set Priority Timer1 IRQ as 5.
	NVIC_SetPriority(TIMER1_IRQn, 5);
	
	//Enable TIMER1_IRQn (Interrupt Request).
	NVIC_EnableIRQ(TIMER1_IRQn);
}

// Starts timer1 for LEDs
void TIMER1_Start() {
	//Reset Timer Counter and Prescale Counter for Timer1.
	TIMER1->TCR |= (1 << 1);
	
	// Enable Counter and remove reset on Timer1.
	TIMER1->TCR |= (1 << 0);
	TIMER1->TCR &= ~(1 << 1);
}

// Stops timer1 for LEDs
void TIMER1_Stop() {
	//Disable counter for timer1
	TIMER1->TCR &= ~(1 << 0);
	
	//Flag for direction of rotation, if it is not 0 then rotation is in that way.
	TURN_LEFT_FLAG = 0;
	TURN_RIGHT_FLAG = 0;
}

/*
* Every 250 ms, this handler will be called. Either rotation is left or right, we increment flag by 1 at each call.
* Then, we call corresponding LEDs and blink occurs.
*/
void TIMER1_IRQHandler() {
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
	TIMER1->IR = (1 << 0);
}

void Timer2_Init() {
	//Enable Timer2
	PCONP |= (1 << 22);
	
	//Set the function of P0_5 to T2_CAP1 
	IOCON_TIMER2_CAP1 |= 3;

	//Set the function of P0_4 to T2_CAP0 
	IOCON_TIMER2_CAP0 |= 3;
	
	//Set the function of P0_7 to T2_MAT1
	IOCON_TRIGGER |= 3;
	
	//Enable capture rising edge for both CAP0 and CAP1
	TIMER2->CCR |= 1 << 0 | 1 << 3;

	//Enable interrupts for both CAP0 and CAP1
	TIMER2->CCR |= 1 << 2 | 1 << 5;
	
	//Change the mode of Timer2 to Timer Mode.
	TIMER2->CTCR = 0x00;
	
	//TRIGGER INIT
	//Change PR Register value for 1 microsecond incrementing
	TIMER2->PR = PERIPHERAL_CLOCK_FREQUENCY / 1000000 - 1;

	//Enable interrupt for MR1 register, stop if MR1 register matches the TC.
	TIMER2->MCR |= (5 << 3);
	
	//Write the Correct Configuration for EMR (Toggle Output)
	TIMER2->EMR |= (3 << 6);
	
	//Clear pendings for Timer2
	NVIC_ClearPendingIRQ(TIMER2_IRQn);
	
	//Set Priority Timer2 IRQ as 5.
	NVIC_SetPriority(TIMER2_IRQn, 5);
	
	//Enable TIMER2_IRQn (Interrupt Request).
	NVIC_EnableIRQ(TIMER2_IRQn);
}

// Keeps the number of holes detected on wheels by speed sensor.
uint32_t tacho[2];

// Starts timer2 for speed sensor
void TIMER2_Start() {
	//Reset tacho counts
	tacho[0] = tacho[1] = 0;
	
	//Reset Timer Counter and Prescale Counter for Timer2.
	TIMER2->TCR |= (1 << 1);
	
	//Enable Counter
	TIMER2->TCR |= (1 << 0);

	//Set the pin and MR1 to 10us
	TIMER2->MR1 = 10;
	TIMER2->EMR |= (1 << 1);

	//Remove reset on Timer2.
	TIMER2->TCR &= ~(1 << 1);
}

uint8_t ultrasonicSensorEdgeCount = 0;

/*
* Depending on the capture register, we increment tacho count of corresponding motor. If a motors completes its 
* total rotation then stops the wheel, if both completes turns off LEDs.
*/
void TIMER2_IRQHandler() { 
	if ((TIMER2->IR & (1 << 1))) {
		if(TIMER2->MR1 == 10) {
			//Change MR1 Register Value for Suggested Waiting
			TIMER2->MR1 = 60000;
			
			ultrasonicSensorEdgeCount = 0;
		}
		else {
			TIMER2->MR1 = 10;
		}
		
		//Reset Timer Counter and Prescale Counter for Timer2.
		TIMER2->TCR |= (1 << 1);
		//Start timer again
		TIMER2->TCR |= 1 << 0;
		//Remove reset on Timer2.
		TIMER2->TCR &= ~(1 << 1);
		
		//Clear IR Register Flag for Corresponding Interrupt
		TIMER2->IR = (1 << 1);
	}
	tacho[0] += (TIMER2->IR >> 4) & 1;
	tacho[1] += (TIMER2->IR >> 5) & 1;
	TIMER2->IR = 3 << 4;
}

void Timer3_Init() {
	PCONP |= (1 << 23);
	
	IOCON_ECHO |= 3;
	
	//Change the mode of Timer3 to Timer Mode.
	TIMER3->CTCR = 0x00;
	
	//Change PR Register value for 1 microsecond incrementing
	TIMER3->PR = PERIPHERAL_CLOCK_FREQUENCY / 1000000 - 1;
	
	//ECHO INIT
	TIMER3->CCR |= (7 << 3);

	//Clear pendings for Timer3.
	NVIC_ClearPendingIRQ(TIMER3_IRQn);

	//Set Priority Timer3 IRQ as 5.
	NVIC_SetPriority(TIMER3_IRQn, 5);
	
	//Enable TIMER3_IRQn (Interrupt Request).
	NVIC_EnableIRQ(TIMER3_IRQn);
}

void TIMER3_Start() {
	//Enable reset on counters
	TIMER3->TCR |= (1 << 1);

	//Enable Timer3 Counter and Prescale Counter for counting.
	TIMER3->TCR |= (1 << 0);

	//Remove the reset on counters of Timer3.
	TIMER3->TCR &= ~(1 << 1);
}


uint32_t ultrasonicSensorRisingTime = 0;
uint32_t ultrasonicSensorDistance = 0;

uint8_t goBack = 0;

void TIMER3_IRQHandler() {
	if(ultrasonicSensorEdgeCount == 0) {
		//Store the rising time into ultrasonicSensorRisingTime variable
		ultrasonicSensorRisingTime = TIMER3->CR1;
	}
	else if(ultrasonicSensorEdgeCount == 1){
		ultrasonicSensorDistance = (TIMER3->CR1 - ultrasonicSensorRisingTime) / 58;
		
		if (ultrasonicSensorDistance <= OBSTACLE_DISTANCE && FORWARD_FLAG) {
			LED_Adjuster(BACKWARD_LED);
			MOTOR_Direction(0, BACKWARD);
			MOTOR_Direction(1, BACKWARD);
			goBack = 1;
		} else if (ultrasonicSensorDistance > OBSTACLE_ESCAPE_DISTANCE && FORWARD_FLAG && goBack) {
			LED_Adjuster(FORWARD_LED);
			MOTOR_Direction(0, FORWARD);
			MOTOR_Direction(1, FORWARD);
			goBack = 0;
		}
	}
	ultrasonicSensorEdgeCount++;
	TIMER3->IR = 1 << 5;
}

