#include "ADC.h"

void ADC_Init() {
	//Change the function value of pin to ADC.
	TRIM_POT_PIN_IOCON |= 1;
	
	//Change the mode value of pin to mode which should be selected if Analog mode is used.
	TRIM_POT_PIN_IOCON &= ~(3 << 3);
	
	//Change Analog/Digital mode of pin to Analog.
	TRIM_POT_PIN_IOCON &= ~(1 << 7);
	
	//Change the function value of pin to ADC.
	RIGHT_LDR_PIN_IOCON |= 1;
	
	//Change the mode value of pin to mode which should be selected if Analog mode is used.
	RIGHT_LDR_PIN_IOCON &= ~(3 << 3);
	
	//Change Analog/Digital mode of pin to Analog.
	RIGHT_LDR_PIN_IOCON &= ~(1 << 7);
	
	//Change the function value of pin to ADC.
	LEFT_LDR_PIN_IOCON |= 1;
	
	//Change the mode value of pin to mode which should be selected if Analog mode is used.
	LEFT_LDR_PIN_IOCON &= ~(3 << 3);
	
	//Change Analog/Digital mode of pin to Analog.
	LEFT_LDR_PIN_IOCON &= ~(1 << 7);
	
	//Turn on ADC.
	PCONP |= (1 << 12);
	
	//Sets Control Register Configuration.
	ADC->CR = 0x0020FF01;
	
	//Enables interrups for ADC channels 0,1, and 2.
	ADC->INTEN |= (1 << 0 | 1 << 2 | 1 << 3);
	
	// Enables ADC interrupts.
	NVIC_ClearPendingIRQ(ADC_IRQn);
	NVIC_EnableIRQ(ADC_IRQn);
}

// Starts ADC conversion
void ADC_Start() {
	ADC->CR |= (1 << 24);
}

int32_t ADC_TRIMPOT;
int32_t ADC_RIGHT_LDR;
int32_t ADC_LEFT_LDR;

// Those are variables for PID control mechanism.
int32_t Kp = 800;   // Estimated value 200-1000
int32_t Ki = 5;   // max 100 gibi
int32_t Kd = 15;   // max 50 gibi
int32_t prev_error = 0;
int32_t total_error = 0;

// Left_LDR - Right_LDR --> error 
// Calculates PID value and return value will be added to one motor and will be subtracted from another motor.
int32_t pid(int32_t error) {
	int32_t combinedPIDValues = 0;
	// P part
	combinedPIDValues += Kp * error;
	
	// I part
	total_error = total_error * 3 / 4 + error * 10; // 10ms beklediğimiz için
	combinedPIDValues += Ki * total_error;
	
	// D part
	combinedPIDValues += (Kd * (error - prev_error)) / 10; // 10ms beklediğimiz için
	prev_error = error;
	
	return combinedPIDValues / 0xFFF;
}

int32_t inc;  // Result of PID
int32_t leftSpeed;
int32_t rightSpeed;

// Sets specified motor speed according to parameters.
void set_motor(uint32_t MOTOR_TYPE, int32_t speed) {
	if(speed < 0) {
		MOTOR_Direction(MOTOR_TYPE, BACKWARD);
		speed = -speed;
	}
	else
		MOTOR_Direction(MOTOR_TYPE, FORWARD);
	if(speed > 100)
		speed = 100;
	PWM_MOTOR_Write(speed, MOTOR_TYPE);
}

/*
* Sets speed of motors by using Trimpot and LDR values. It normalizes speeds so that decreased speed from trimpot
* will not exceed.
*/
void set_speed() {
	ROBOT_SPEED = 100 * (ADC_TRIMPOT - ADC_TRIMPOT_MIN) / (ADC_TRIMPOT_MAX - ADC_TRIMPOT_MIN);
	if(FORWARD_FLAG && goBack == 0) {
		//leftSpeed = ROBOT_SPEED - ROBOT_SPEED * LDR_WEIGHT * ADC_LEFT_LDR / ADC_MAX / 100;
		//rightSpeed = ROBOT_SPEED - ROBOT_SPEED * LDR_WEIGHT * ADC_RIGHT_LDR / ADC_MAX / 100;
		//inc = pid((int)(pow(ADC_LEFT_LDR, 0.25) * 512 - (int)pow(ADC_RIGHT_LDR, 0.25) * 512));
		inc = pid((int)(ADC_LEFT_LDR) - (int)(ADC_RIGHT_LDR) - 200);  // Reference value is -200
		// Sets motor speeds with updated values by pid.
		rightSpeed = ROBOT_SPEED + inc;	
		leftSpeed = ROBOT_SPEED - inc;
		
		// Normalizes if speed exceeds 100 on left motor.
		if(leftSpeed > 100) {
			rightSpeed = rightSpeed * 100 / leftSpeed;
			leftSpeed = 100;
			if(rightSpeed < -100) {
				leftSpeed = leftSpeed * 100 / -rightSpeed;
				rightSpeed = -100;
			}
		}
		
		// Normalizes if speed exceeds 100 on right motor.
		if(rightSpeed > 100) {
			leftSpeed = leftSpeed * 100 / rightSpeed;
			rightSpeed = 100;
			if(leftSpeed < -100) {
				rightSpeed = rightSpeed * 100 / -leftSpeed;
				leftSpeed = -100;
			}
		}
		
		// If robot turns faster than threshold that is speed difference 30, blinks LEDS
		// Until speed difference declines to maximum 15, blinking continues then lights up front LEDS
		if (rightSpeed - leftSpeed > 30 && !TURN_LEFT_FLAG) {
			TURN_RIGHT_FLAG = BACKWARD_FLAG = 0;
			TURN_LEFT_FLAG = 1;
			LED_Adjuster(LEFT_BLINKER);	
		} else if (leftSpeed - rightSpeed > 30 && !TURN_RIGHT_FLAG) {
			TURN_LEFT_FLAG = BACKWARD_FLAG = 0;
			TURN_RIGHT_FLAG = 1;
			LED_Adjuster(RIGHT_BLINKER);
		} else if (leftSpeed - rightSpeed <= 15 && rightSpeed - leftSpeed <= 15) {
			TURN_RIGHT_FLAG = BACKWARD_FLAG = TURN_LEFT_FLAG = 0;
			FORWARD_FLAG = 1;
			LED_Adjuster(FORWARD_LED);
		}
		
		set_motor(1, rightSpeed);
 		set_motor(0, leftSpeed);
	}
	else {
		PWM_MOTOR_Write(ROBOT_SPEED, 0);
 		PWM_MOTOR_Write(ROBOT_SPEED, 1);
	}
}

/*
* According to which ADC completed on DR register, converts that signal.
* We will do these conversions sequentially at each 10 ms.
*/
void ADC_IRQHandler() {
	uint32_t val;
	if ((val = ADC->DR[0]) >> 31) {		// Trimpot conversion
		ADC_TRIMPOT = (val >> 4) & 0xFFF;	// Reads converted values up to 4095
		ADC->CR &= ~(1 << 0);	// disables conversion for channel 0
		ADC->CR |= (1 << 2);	// enables conversion for channel 2
		ADC->CR |= (1 << 24);	// starts conversion
	}
	else if ((val = ADC->DR[2]) >> 31) {	// LDR Right conversion
		ADC_RIGHT_LDR = (val >> 4) & 0xFFF;	// Reads converted values up to 4095
		ADC->CR &= ~(1 << 2);	// disables conversion for channel 2
		ADC->CR |= (1 << 3);	// enables conversion for channel 3
		ADC->CR |= (1 << 24);	// starts conversion
	}
	else if ((val = ADC->DR[3]) >> 31) {	// LDR Left conversion
		ADC_LEFT_LDR = (val >> 4) & 0xFFF;	// Reads converted values up to 4095
		ADC->CR &= ~(1 << 3);	// disables conversion for channel 3
		ADC->CR |= (1 << 0);	// enables conversion for channel 0
		ADC->CR &= ~(1 << 24);	// starts conversion

		set_speed();			// We change speed of motors at this phase.
	}
}
