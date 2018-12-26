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
	
	ADC->CR = 0x00200401;
	
	ADC->INTEN |= (1 << 0 | 1 << 2 | 1 << 3);
	
	NVIC_ClearPendingIRQ(ADC_IRQn);
	NVIC_EnableIRQ(ADC_IRQn);
}

void ADC_Start() {
	ADC->CR |= (1 << 24);
}

uint32_t ADC_TRIMPOT;
uint32_t ADC_RIGHT_LDR;
uint32_t ADC_LEFT_LDR;

void set_speed() {
	ROBOT_SPEED = 100 * (ADC_TRIMPOT - ADC_TRIMPOT_MIN) / (ADC_TRIMPOT_MAX - ADC_TRIMPOT_MIN);
	if(FORWARD_FLAG && goBack == 0) {
		uint32_t leftSpeed = ROBOT_SPEED - ROBOT_SPEED * LDR_WEIGHT * ADC_LEFT_LDR / ADC_MAX / 100;
		uint32_t rightSpeed = ROBOT_SPEED - ROBOT_SPEED * LDR_WEIGHT * ADC_RIGHT_LDR / ADC_MAX / 100;
		uint32_t inc = ROBOT_SPEED - (leftSpeed + rightSpeed) / 2;
		if(leftSpeed + inc > 100) {
			rightSpeed += 100 - leftSpeed;
			leftSpeed = 100;
		}
		else if(rightSpeed + inc > 100) {
			leftSpeed += 100 - rightSpeed;
			rightSpeed = 100;
		}
		else {
			leftSpeed += inc;
			rightSpeed += inc;
		}
		PWM_MOTOR_Write(rightSpeed, 0);	
 		PWM_MOTOR_Write(leftSpeed, 1);
	}
	else {
		PWM_MOTOR_Write(ROBOT_SPEED, 0);
 		PWM_MOTOR_Write(ROBOT_SPEED, 1);
	}
}

void ADC_IRQHandler() {
	uint32_t val;
	if ((val = ADC->DR[0]) >> 31) {
		ADC_TRIMPOT = (val >> 4) & 0xFFF;
		ADC->CR &= ~(1 << 0);
		ADC->CR |= (1 << 2);
		ADC->CR |= (1 << 24);
	}
	else if ((val = ADC->DR[2]) >> 31) {
		ADC_RIGHT_LDR = (val >> 4) & 0xFFF;
		ADC->CR &= ~(1 << 2);
		ADC->CR |= (1 << 3);
		ADC->CR |= (1 << 24);
	}
	else if ((val = ADC->DR[3]) >> 31) {
		ADC_LEFT_LDR = (val >> 4) & 0xFFF;
		ADC->CR &= ~(1 << 3);
		ADC->CR |= (1 << 0);
		ADC->CR &= ~(1 << 24);

		set_speed();
	}
}
