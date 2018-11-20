#include "PWM.h"

void PWM_Init() {
	
	// Enables PWM0
	PCONP |= 1 << 5;
	
	//Change the IOCON function of the pin to PWM0[1] and PWM0[2]
	IOCON_MOTOR1_SPEED |= 3;
	IOCON_MOTOR2_SPEED |= 3;
	
	// Enable PWM0[1] and PWM0[2] channels
	PWM0->PCR |= (1 << 9 | 1 << 10);
	
	// Reset counter
	PWM0->TCR = (1 << 1);
	
	// Give power to the motor every 20 millisecond
	PWM0->MR0 = (PERIPHERAL_CLOCK_FREQUENCY / 1000) * 20;
	
	//Reset TC, when MR0 matches TC.
	PWM0->MCR = 1 << 1;
	
	//Enable PWM Match 0 Latch.
	PWM0->LER |= 1 << 0;
	
	//Enable Counter and PWM and Clear Reset on the PWM
	PWM0->TCR |= (1 << 0 | 1 << 3);
	PWM0->TCR &= ~(1 << 1);
	
	// Stops both motors.
	PWM_MOTOR_Write(0, 0);
	PWM_MOTOR_Write(0, 1);
}

/*
* Sets power for given value to the given motor type. Power cannot be more than 100.
* Power 100 means give 100% power to the motor.
*/
void PWM_MOTOR_Write(uint32_t T_ON, uint32_t MOTOR_TYPE) {	
	if(T_ON > 100) {
		T_ON = 100;
	}
	
	T_ON = (uint32_t)(((PWM0->MR0) * T_ON) / 100);
	
	if (T_ON == PWM0->MR0) {
		T_ON++;
	}
	
	if (MOTOR_TYPE == 0) {
		PWM0->MR1 = T_ON;
	} else if (MOTOR_TYPE == 1) {
		PWM0->MR2 = T_ON;
	}
	
	PWM0->LER |= (1 << (MOTOR_TYPE + 1));
}
