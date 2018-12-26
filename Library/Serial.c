#include "Serial.h"

char* serialTransmitData = 0;
uint8_t serialTransmitCompleted = 0;
char serialReceivedCharacter = 0;

void Serial_Init() {
	//Change the function of TX and RX pins for UART.
	
	//Turn on UART0.
	
	//Enable FIFO for UART0.
	
	//In order to change the DLM, DLL and FDR values, Write correct code for enabling the access to Divisor Latches.
	
	//Write correct DLM, DLL and FDR values for 9600 baudrate
	
	//Write correct code for disabling the access to Divisor Latches.
	
	//Change LCR register value for 8-bit character transfer, 1 stop bits and Even Parity.
							
	//Enable the Receive Data Available and THRE Interrupt.
	
	//Enable UART0_IRQn Interrupt.
	
	//Set UART0_IRQn Priority to 5.
}

void UART0_IRQHandler() {	
	uint32_t currentInterrupt = ((Serial_UART->IIR & (0x7 << 1)) >> 1);
	
	//First if statement is for Receive Data Available interrupt.
	if(currentInterrupt == 0x00) {
		serialReceivedCharacter = Serial_ReadData();
	}
	//Second if statement is for THRE interrupt
	else if(currentInterrupt == 0x00) {
		if(*serialTransmitData > 0) {
			Serial_WriteData(*serialTransmitData++);
		}
		else {
			serialTransmitCompleted = 1;
		}
	}
}

char Serial_ReadData() {
	return Serial_UART->RBR;
}

void Serial_WriteData(char data) {
	serialTransmitCompleted = 0;
	Serial_UART->THR = data;
}

