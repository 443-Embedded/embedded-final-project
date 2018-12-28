#ifndef MAIN_H
#define MAIN_H

#include "LPC407x_8x_177x_8x.h"

#include "GPIO.h"
#include "Timer.h"
#include "PWM.h"
#include "Joystick.h"
#include "LED.h"
#include "ADC.h"
#include "Wait.h"
#include "Parameters.h"
#include "External.h"
#include "ESP8266.h"
#include "Serial.h"

typedef enum {
	STOP,	FORWARD, BACKWARD
} Motor_State;

typedef enum {
	RIGHT_BLINKER, LEFT_BLINKER, FORWARD_LED, BACKWARD_LED, STOP_LED
} LED_State;

void MOTOR_Direction(uint32_t MOTOR_TYPE, Motor_State state);
void LED_Adjuster(LED_State state);
#endif
