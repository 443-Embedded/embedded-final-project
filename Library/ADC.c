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
	
	//Turn on ADC.
	PCONP |= (1 << 12);
	
	//Set the CLKDIV and make the A/D converter operational without Burst mode.
	ADC->CR |= (ADC_CLKDIV << 8);
	ADC->CR &= ~(1 << 16);
	
	//ADC->INTEN |= (1 << 0);
	
	//NVIC_ClearPendingIRQ(ADC_IRQn);
	//NVIC_EnableIRQ(ADC_IRQn);
	
	ADC->CR |= (1 << 21);
}

void ADC_Start() {
	//Write a code for starting A/D conversion
	uint32_t value = ADC->CR;
	value |= (1 << 24);
	value &= ~(1 << 25 | 1 << 26);
	ADC->CR = value;
}

void ADC_Stop() {
	//Write a code for stopping A/D conversion
	ADC->CR &= ~(1<< 24 | 1 << 25 | 1 << 26);
}

uint32_t ADC_Read() {
	uint32_t data;
	
	//Configure CR SEL bits for sampled and converting corresponding pin
	ADC->CR |= (1 << 0);
	
	ADC_Start();
	
	//Wait until A/D conversion completed
	while(!(ADC->DR[0] >> 31));
	
	//Convert the data RESULT to 0 - 1000 range and return the ADC data
	data = (ADC->DR[0] >> 8) & 0xFFF;
	data *= 100;
	data /= ADC_TRIM_POT;
	
	ADC_Stop();
	
	return data;
}
