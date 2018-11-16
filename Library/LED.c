#include "LED.h"

void LED_Init() {
	LED1_Init();
	LED2_Init();
	LED3_Init();
	LED4_Init();
}

void LED1_Init() {
	GPIO_DIR_Write(LED1_PORT,LED1_MASK,OUTPUT);
}

void LED2_Init() {
	GPIO_DIR_Write(LED2_PORT,LED2_MASK,OUTPUT);
}

void LED3_Init() {
	GPIO_DIR_Write(LED3_PORT,LED3_MASK,OUTPUT);
}

void LED4_Init() {
	GPIO_DIR_Write(LED4_PORT,LED4_MASK,OUTPUT);
}

void LED_ON() {
	LED1_On();
	LED2_On();
	LED3_On();
	LED4_On();
}

void LED1_On() {
	GPIO_PIN_Write(LED1_PORT,LED1_MASK,LOW);
}

void LED2_On() {
	GPIO_PIN_Write(LED2_PORT,LED2_MASK,LOW);
}

void LED3_On() {
	GPIO_PIN_Write(LED3_PORT,LED3_MASK,HIGH);
}

void LED4_On() {
	GPIO_PIN_Write(LED4_PORT,LED4_MASK,HIGH);
}

void LED_OFF() {
	LED1_Off();
	LED2_Off();
	LED3_Off();
	LED4_Off();
}

void LED1_Off() {
	GPIO_PIN_Write(LED1_PORT,LED1_MASK,HIGH);
}

void LED2_Off() {
	GPIO_PIN_Write(LED2_PORT,LED2_MASK,HIGH);
}

void LED3_Off() {
	GPIO_PIN_Write(LED3_PORT,LED3_MASK,LOW);
}

void LED4_Off() {
	GPIO_PIN_Write(LED4_PORT,LED4_MASK,LOW);
}
