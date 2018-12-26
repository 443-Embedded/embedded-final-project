#ifndef ADC_H
#define ADC_H

#include "LPC407x_8x_177x_8x.h"

#include "SystemStructures.h"
#include "main.h"

typedef struct {
  volatile	uint32_t CR;
  volatile	uint32_t GDR;
			uint32_t RESERVED0;
  volatile	uint32_t INTEN;
  volatile	uint32_t DR[8];
  volatile	uint32_t STAT;
  volatile	uint32_t TRM;
} ADC_TypeDef;

#define ADC_CLOCK_FREQUENCY 1000000

//Write the IOCON address of Analog Pin
#define	ANALOG_PIN_IOCON_ADDRESS	0x00000000
#define ANALOG_PIN_IOCON	*((volatile uint32_t*)(ANALOG_PIN_IOCON_ADDRESS))

#define	TRIM_POT_PIN_IOCON_ADDRESS	0x4002C05C
#define TRIM_POT_PIN_IOCON	*((volatile uint32_t*)(TRIM_POT_PIN_IOCON_ADDRESS))
	
#define	RIGHT_LDR_PIN_IOCON_ADDRESS	0x4002C064
#define RIGHT_LDR_PIN_IOCON	*((volatile uint32_t*)(RIGHT_LDR_PIN_IOCON_ADDRESS))
	
#define	LEFT_LDR_PIN_IOCON_ADDRESS	0x4002C068
#define LEFT_LDR_PIN_IOCON	*((volatile uint32_t*)(LEFT_LDR_PIN_IOCON_ADDRESS))

//Write the CLKDIV of ADC for given ADC_CLOCK_FREQUENCY.
#define ADC_CLKDIV 0x3B

//Write the max value of ADC.
#define ADC_MAX 0xFFF

#define ADC_TRIMPOT_MAX 0xFE8
#define ADC_TRIMPOT_MIN 0x000

#define LDR_WEIGHT 10

#define ADC_BASE	0x40034000
#define ADC	((ADC_TypeDef*) ADC_BASE)

void ADC_Init(void);
void ADC_Start(void);

#endif
