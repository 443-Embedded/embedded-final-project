#include "Timer.h"

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
	//Disable Timer Counter and Prescale Counter
	TIMER0->TCR &= ~(1 << 0);
	
	//Reset Timer Counter and Prescale Counter
	TIMER0->TCR |= (1 << 1);
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

void Timer2_Init() {
	//Enable Timer2
	PCONP |= (1 << 22);
	
	// TIMER 2
	//Set the function of P0_5 to T2_CAP1 
	IOCON_TIMER2_CAP1 |= 3;
	//Set the function of P0_4 to T2_CAP0 
	IOCON_TIMER2_CAP0 |= 3;
	
	//Set the function of P0_7 to T2_MAT1
	IOCON_TRIGGER |= 3;
	
	//Enable capture rising edge for both CAP0 and CAP1
	TIMER2->CCR |= 1 << 0 | 1 << 3;
	
	//Disable Timer Counter and Prescale Counter for Timer2.
	TIMER2->TCR &= ~(1 << 0);
	
	//Reset Timer Counter and Prescale Counter for Timer2.
	TIMER2->TCR |= (1 << 1);
	
	//Change the mode of Timer2 to Timer Mode.
	TIMER2->CTCR = 0x00;
	
	//TRIGGER INIT
	//Change PR Register value for 10 microsecond incrementing
	TIMER2->PR = PERIPHERAL_CLOCK_FREQUENCY / 100000 - 1;
	//Write the Correct Configuration for EMR (Toggle Output Value and Initial value is HIGH)
	TIMER2->EMR |= (1 << 1 | 3 << 6);
	
	//Clear pendings for Timer2
	NVIC_ClearPendingIRQ(TIMER2_IRQn);
	
	//Set Priority Timer2 IRQ as 5.
	NVIC_SetPriority(TIMER2_IRQn, 5);
	
	//Enable TIMER2_IRQn (Interrupt Request).
	NVIC_EnableIRQ(TIMER2_IRQn);
}

void Timer3_Init() {
	PCONP |= (1 << 23);
	
	IOCON_ECHO |= 3;
	
	// TIMER 3
	//Change the mode of Timer3 to Timer Mode.
	TIMER3->CTCR = 0x00;
	
	//Disable Timer Counter and Prescale Counter for Timer3.
	TIMER3->TCR &= ~(1 << 0);
	
	//Reset Timer Counter and Prescale Counter for Timer3.
	TIMER3->TCR |= (1 << 1);
	
	//Change PR Register value for 1 microsecond incrementing
	TIMER3->PR = PERIPHERAL_CLOCK_FREQUENCY / 1000000 - 1;
	
	//ECHO INIT
	TIMER3->CCR |= (5 << 3);
	
	//Remove the reset on counters of Timer3.
	TIMER3->TCR &= ~(1 << 1);
	//Enable Timer3 Counter and Prescale Counter for counting.
	TIMER3->TCR |= (1 << 0);
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

// Keeps the number of holes detected on wheels by speed sensor.
uint32_t tacho[2];

// Starts timer2 for speed sensor
void TIMER2_Start() {
	//Reset tacho counts
	tacho[0] = tacho[1] = 0;
	
	//Enable interrupts for both CAP0 and CAP1
	// TIMER2->CCR |= 1 << 2 | 1 << 5;
	
	//Give correct value to MR1 Register for 10 microsecond
	TIMER2->MR1 = 10;
	//Enable interrupt for MR1 register, if MR1 register matches the TC.
	TIMER2->MCR |= (1 << 3 | 1 << 4);
	
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
	
	//Disable interrupts for both CAP0 and CAP1
	TIMER2->CCR &= ~(1 << 2 | 1 << 5);
}

uint8_t isUltrasonicSensorTriggerEnded = 0;
uint8_t ultrasonicSensorEdgeCount = 0;

/*
* Depending on the capture register, we increment tacho count of corresponding motor. If a motors completes its 
* total rotation then stops the wheel, if both completes turns off LEDs.
*/
void TIMER2_IRQHandler() {
	if ((TIMER2->IR & (1 << 1))) {
		if(isUltrasonicSensorTriggerEnded == 0) {
			//Change MR1 Register Value for Suggested Waiting
			TIMER2->MR1 = 60000;
			
			isUltrasonicSensorTriggerEnded = 1;
			
			ultrasonicSensorEdgeCount = 0;
			
			//Clear pendings for Timer3.
			NVIC_ClearPendingIRQ(TIMER3_IRQn);
			
			//Enable TIMER3_IRQn (Interrupt Request).
			NVIC_EnableIRQ(TIMER3_IRQn);
		}
		else {
			TIMER2->MR1 = 10;
			isUltrasonicSensorTriggerEnded = 0;
		}
		
		//Clear IR Register Flag for Corresponding Interrupt
		TIMER2->IR = (1 << 1);
		
		TIMER2->TC = 0;
	} 
	if ((TIMER2->IR & (1 << 4)) || (TIMER2->IR & (1 << 5))) {
		int IR_idx = 4;
		if(TIMER2->IR & (1 << 5))
			IR_idx = 5;
		tacho[IR_idx - 4]++;
		if(tacho[IR_idx - 4] >= HOLE_NUMBER_FOR_90_DEGREE) {
			MOTOR_Direction(IR_idx - 4, STOP);
			if(tacho[5 - IR_idx] >= HOLE_NUMBER_FOR_90_DEGREE)
				LED_Adjuster(STOP_LED);
		}
		//Clear the interrupt flag for CAP channel event
		TIMER2->IR = (1 << IR_idx);
	}
}

uint32_t ultrasonicSensorRisingTime = 0;
uint32_t ultrasonicSensorFallingTime = 0;
uint32_t ultrasonicSensorDuration = 0;
uint32_t ultrasonicSensorDistance = 0;

uint8_t goBack = 0;

void TIMER3_IRQHandler() {
	if(ultrasonicSensorEdgeCount == 0) {
		
		//Store the rising time into ultrasonicSensorRisingTime variable
		ultrasonicSensorRisingTime = TIMER3->CR1;
		
		uint32_t value = TIMER3->CCR;
		value |= (1 << 4);
		value &= ~(1 << 3);
		TIMER3->CCR = value;
		
		ultrasonicSensorEdgeCount = 1;
	}
	else if(ultrasonicSensorEdgeCount == 1){
		
		//Store the falling time into ultrasonicSensorFallingTime variable
		ultrasonicSensorFallingTime = TIMER3->CR1;
		
		uint32_t value = TIMER3->CCR;
		value |= (1 << 3);
		value &= ~(1 << 4);
		TIMER3->CCR = value;
		
		ultrasonicSensorEdgeCount = 2;
		
		ultrasonicSensorDistance = (ultrasonicSensorFallingTime - ultrasonicSensorRisingTime) / 58;
		
		if (ultrasonicSensorDistance <= 15 && FORWARD_FLAG) {
			LED_Adjuster(BACKWARD_LED);
			MOTOR_Direction(0, BACKWARD);
			MOTOR_Direction(1, BACKWARD);
			goBack = 1;
		} else if (ultrasonicSensorDistance > 30 && goBack) {
			LED_Adjuster(FORWARD_LED);
			MOTOR_Direction(0, FORWARD);
			MOTOR_Direction(1, FORWARD);
			goBack = 0;
		}

		//Clear pendings for Timer3.
		NVIC_ClearPendingIRQ(TIMER3_IRQn);
		//Disable TIMER3_IRQn (Interrupt Request).
		NVIC_DisableIRQ(TIMER3_IRQn);
	}
	
	TIMER3->IR = 1 << 5;
}

