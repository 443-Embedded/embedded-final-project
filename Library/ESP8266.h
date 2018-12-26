#ifndef ESP8266_H
#define ESP8266_H

#include "LPC407x_8x_177x_8x.h"

#include <stdio.h>
#include <string.h>

#pragma anon_unions

typedef struct
{
	union
	{
		volatile  uint8_t  RBR;
		volatile  uint8_t  THR;
		volatile	uint8_t  DLL;
							uint32_t RESERVED0;
	};
	union
	{
		volatile	uint8_t  DLM;
		volatile	uint32_t IER;
	};
	union
	{
		volatile  uint32_t IIR;
		volatile  uint8_t  FCR;
	};
	volatile	uint8_t  LCR;
						uint8_t  RESERVED1[7];
	volatile  uint8_t  LSR;
						uint8_t  RESERVED2[7];
	volatile	uint8_t  SSCR;
						uint8_t  RESERVED3[3];
	volatile	uint32_t ACR;
	volatile	uint8_t  ICR;
						uint8_t  RESERVED4[3];
	volatile	uint8_t  FDR;
						uint8_t  RESERVED5[7];
	volatile	uint8_t  TER;
						uint8_t  RESERVED8[27];
	volatile	uint8_t  RS485CTRL;
						uint8_t  RESERVED9[3];
	volatile	uint8_t  ADRMATCH;
						uint8_t  RESERVED10[3];
	volatile	uint8_t  RS485DLY;
						uint8_t  RESERVED11[3];
	volatile  uint8_t  FIFOLVL;
}UART_TypeDef;

#define ESP8266BufferSize 256

#define ESP8266_UART_BASE	 0x4009C000
#define ESP8266_UART	((UART_TypeDef*) ESP8266_UART_BASE)

#define ESP8266_UART_TX_PIN_ADDRESS	0x4002C008
#define ESP8266_UART_TX_PIN	*((volatile uint32_t*)(ESP8266_UART_TX_PIN_ADDRESS))

#define ESP8266_UART_RX_PIN_ADDRESS	0x4002C00C
#define ESP8266_UART_RX_PIN	*((volatile uint32_t*)(ESP8266_UART_RX_PIN_ADDRESS))
	
#define P1_ADDRESS	0x4002C0D0
#define P1	*((volatile uint32_t*)(P1_ADDRESS))

#define P0_ADDRESS	0x4002C054
#define P0	*((volatile uint32_t*)(P0_ADDRESS))

extern uint8_t esp8266CurrentBufferIndex;
extern uint8_t esp8266ResponseStartIndex;
extern uint8_t esp8266ResponseCurrentIndex;

extern char esp8266Buffer[ESP8266BufferSize];
extern char esp8266Response[ESP8266BufferSize];

void ESP8266_Init(void);
void ESP8266_sendCommand(char* command);
uint8_t ESP8266_waitResponseEnd(void);
char* ESP8266_readResponse(void);
void ESP8266_clearBuffer(void);

char ESP8266_ReadData(void);
void ESP8266_WriteData(char data);
void ESP8266_Write(char* data);


#endif
