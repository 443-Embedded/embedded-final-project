#include "ADC.h"

uint32_t ADC_TRIMPOT;
uint32_t ADC_RIGHT_LDR;
uint32_t ADC_LEFT_LDR;

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
	else if ((val = ADC->DR[2]) >> 31) {
		ADC_LEFT_LDR = (val >> 4) & 0xFFF;
		ADC->CR &= ~(1 << 3);
		ADC->CR |= (1 << 0);
		ADC->CR |= ~(1 << 24);
	}
}
