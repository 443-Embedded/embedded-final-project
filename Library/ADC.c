#include "ADC.h"

uint32_t ADC_Last = 0;
uint8_t ADC_New_Data_Available = 0;

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
	
	ADC->CR = 0x00201D0D;
	
	ADC->INTEN |= (1 << 0 | 1 << 2 | 1 << 3);
	
	NVIC_ClearPendingIRQ(ADC_IRQn);
}

void ADC_Start() {	
	ADC->CR &= ~(1<< 24 | 1 << 25 | 1 << 26);
	ADC->CR |= (1 << 16);
	NVIC_EnableIRQ(ADC_IRQn);
}

void ADC_Stop() {
	NVIC_DisableIRQ(ADC_IRQn);
	ADC->CR &= ~(1 << 16);
}

void ADC_IRQHandler(void) {
	uint32_t stat = ADC->STAT;
	uint32_t LEFT_LDR, RIGHT_LDR = 0;
	if (stat & (1 << 0)) {
		//Convert the data RESULT to 0 - 100 range and return the ADC data
		ROBOT_SPEED = (ADC->DR[0] >> 4) & 0xFFF;
		ROBOT_SPEED *= 100;
		ROBOT_SPEED /= ADC_TRIM_POT;
	}
	if (stat & (1 << 2)) {
		RIGHT_LDR = (ADC->DR[2] >> 4) & 0xFFF;
		RIGHT_LDR *= 100;
		RIGHT_LDR /= ADC_MAX_VALUE;
	}
	if (stat & (1 << 3)) {
		LEFT_LDR = (ADC->DR[3] >> 4) & 0xFFF;
		LEFT_LDR *= 100;
		LEFT_LDR /= ADC_MAX_VALUE;
		
		uint16_t rightSpeed = ROBOT_SPEED * (60 + 40 * (100 - LEFT_LDR) / 100) / 100;
		uint16_t leftSpeed = ROBOT_SPEED * (60 + 40 * (100 - RIGHT_LDR) / 100) / 100;
		
		ADC_Stop();
		PWM_MOTOR_Write(rightSpeed, 0);
		PWM_MOTOR_Write(leftSpeed, 1);
	}
}
