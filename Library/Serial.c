#include "Serial.h"

char* serialTransmitData = 0;
uint8_t serialTransmitCompleted = 0;
char serialReceivedCharacter = 0;

char prevChar = 0;
char currentChar = 0;

void Serial_Init() {
	//Change the function of TX and RX pins for UART.
	Serial_UART_TX_PIN |= 1;
	Serial_UART_RX_PIN |= 1;
	
	//Turn on UART0.
	PCONP |= 1 <<  3;
	
	//Enable FIFO for UART0.
	Serial_UART->FCR =	1 << 0
						 |	0 << 1
						 |	0 << 2
						 |	0 << 6;
	
	//In order to change the DLM, DLL and FDR values, Write correct code for enabling the access to Divisor Latches.
	Serial_UART->LCR |= (1 << 7);
	
	//Write correct DLM, DLL and FDR values for 9600 baudrate
	Serial_UART->DLM = 0x01;
	Serial_UART->DLL = 0x25;
	Serial_UART->FDR = 0x01 << 0 | 0x03 << 4;
	
	//Write correct code for disabling the access to Divisor Latches.
	Serial_UART->LCR &= ~(1 << 7);
	
	//Change LCR register value for 8-bit character transfer, 1 stop bits and Even Parity.
	Serial_UART->LCR |= 3 << 0 | 3 << 3;
	Serial_UART->LCR &= ~(1 << 2);
							
	//Enable the Receive Data Available and THRE Interrupt.
	Serial_UART->IER |= (1 << 0);
	
	//Enable UART0_IRQn Interrupt.
	NVIC_ClearPendingIRQ(UART0_IRQn);
	NVIC_EnableIRQ(UART0_IRQn);
	
	//Set UART0_IRQn Priority to 5.
	NVIC_SetPriority(UART0_IRQn, 5);
}

void UART0_IRQHandler() {	
	prevChar = currentChar;
	currentChar = Serial_ReadData();
	Serial_WriteData(currentChar);
	if (currentChar == '*') {
		changeStartMode(MANUAL);
		Serial_Write("Mode is MANUAL!\r\n");
	}
	
	if (currentChar == '#') {
		changeStartMode(AUTO);
		Serial_Write("Mode is AUTO!\r\n");
	}
	
	if (START_MODE == AUTO && !FORWARD_FLAG && prevChar == '6' && currentChar == '6') {
		TURN_LEFT_FLAG = TURN_RIGHT_FLAG = BACKWARD_FLAG = 0;
		FORWARD_FLAG = 1;
		MOTOR_Direction(0, FORWARD);
		MOTOR_Direction(1, FORWARD);
		LED_Adjuster(FORWARD_LED);
		Serial_Write("Start!\r\n");
	}
}

char Serial_ReadData() {
	return Serial_UART->RBR;
}

void Serial_WriteData(char data) {
	while (!(Serial_UART->LSR & 0x20));
	Serial_UART->THR = data;
}

void Serial_Write(char* data) {
	while(*data)  {
		Serial_WriteData(*data++);
	}
}
