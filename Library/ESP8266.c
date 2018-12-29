#include "GPIO.h"
#include "ESP8266.h"

uint8_t esp8266CurrentBufferIndex;
uint8_t esp8266ResponseStartIndex;
uint8_t esp8266ResponseCurrentIndex;

char esp8266Buffer[ESP8266BufferSize];
char esp8266Response[ESP8266BufferSize];

/*
* Initializes UART3 for Wifi. Most of the part of this function is given.
*/
void ESP8266_Init() {
	ESP8266_UART_TX_PIN |= 0x02;
	ESP8266_UART_RX_PIN |= 0x02;
	
	PCONP |= 1 <<  25;
	
	ESP8266_UART->FCR =	1 << 0
						 |	0 << 1
						 |	0 << 2
						 |	0 << 6;
	
	ESP8266_UART->LCR |= (1 << 7);
	
	ESP8266_UART->DLM = 0x00;
	ESP8266_UART->DLL = 0x13;
	ESP8266_UART->FDR = 0x05 << 0 | 0x07 << 4;
	
	ESP8266_UART->LCR &= ~(1 << 7);
	
	ESP8266_UART->LCR =	3 << 0
							| 0 << 2
							| 0 << 3
							| 0 << 4;
	
	//Enable the Receive Data Available Interrupt.
	ESP8266_UART->IER |= (1 << 0);
	
	//Enable UART3_IRQn Interrupt.
	NVIC_ClearPendingIRQ(UART3_IRQn);	
	NVIC_EnableIRQ(UART3_IRQn);
}

/*
* Sends command to Wifi
*/
void ESP8266_sendCommand(char* command) {
	uint16_t index;
	for(index = 0; index < ESP8266BufferSize; index++) {
		esp8266Response[index] = 0;
	}
	esp8266ResponseStartIndex = esp8266CurrentBufferIndex;
	esp8266ResponseCurrentIndex = 0;
	ESP8266_Write(command);
}

/*
* Waits until response come, then returns according to response.
*/
uint8_t ESP8266_waitResponseEnd() {
	uint8_t responseEndIndex;
	uint8_t bufferIndex;
	for(;;) {
		responseEndIndex = (esp8266CurrentBufferIndex - esp8266ResponseStartIndex);
		for(; esp8266ResponseCurrentIndex < responseEndIndex; esp8266ResponseCurrentIndex++) {
			bufferIndex = esp8266ResponseStartIndex + esp8266ResponseCurrentIndex;
			esp8266Response[esp8266ResponseCurrentIndex] = esp8266Buffer[bufferIndex];
		}
		if(strstr(esp8266Response, "CONNECTED")) {
			return 6;
		}
		else if(strstr(esp8266Response, "#")) {
			return 10;
		}
		else if(strstr(esp8266Response, "66")) {
			return 13;
		}
		else if(strstr(esp8266Response, "*")) {
			return 12;
		}
		else if(strstr(esp8266Response, "OK")) {
			return 1;
		}
		else if(strstr(esp8266Response, "FAIL")) {
			return 2;
		}
		else if(strstr(esp8266Response, "SEND FAIL")) {
			return 4;
		}
		else if(strstr(esp8266Response, "CLOSED")) {
			return 5;
		}
		else if(strstr(esp8266Response, "ERROR")) {
			return 5;
		}
	}
	return 0;
}

/*
* Reads response from Wifi and returns the message as char array.
*/
char* ESP8266_readResponse() {
	char data;
	uint16_t index;
	
	esp8266CurrentBufferIndex = 0;
	for(index = 0; index < ESP8266BufferSize; index++) {
		esp8266Buffer[index] = 0;
	}	
	
	while(ESP8266_UART->LSR & 0x01) {
		data = ESP8266_ReadData();
		if(data != '\n') {
			esp8266Buffer[esp8266CurrentBufferIndex] = data;
			esp8266CurrentBufferIndex++;
		}
	}
	
	return esp8266Buffer;
}

void ESP8266_clearBuffer() {
}

char ESP8266_ReadData() {
	while (!(ESP8266_UART->LSR & 0x01));
	return ESP8266_UART->RBR;
}

void ESP8266_WriteData(char data) {
	while (!(ESP8266_UART->LSR & 0x20));
	ESP8266_UART->THR = data;
}

void ESP8266_Write(char* data) {
	while(*data)  {
		ESP8266_WriteData(*data++);
	}
}

void UART3_IRQHandler() {
	esp8266Buffer[esp8266CurrentBufferIndex++] = ESP8266_ReadData();
}

