#include "LPC407x_8x_177x_8x.h"

#include "Library/GPIO.h"
#include "Library/Timer.h"
#include "Library/PWM.h"
#include "Library/Joystick.h"
#include "Library/LED.h"
#include "Library/Wait.h"
#include "Library/Parameters.h"

uint32_t TURN_LEFT_FLAG = 0;
uint32_t TURN_RIGHT_FLAG = 0;
uint32_t FORWARD_FLAG = 0;
uint32_t BACKWARD_FLAG = 0;

typedef enum {
	STOP,	FORWARD, BACKWARD
} Motor_State;

typedef enum {
	RIGHT_BLINKER, LEFT_BLINKER, FORWARD_LED, BACKWARD_LED, STOP_LED
} LED_State;

void init() {
	GPIO_Init();
	PWM_Init();
	Joystick_Init();
	LED_Init();
	Timer_Init();
}

void LED_Adjuster(LED_State state) {
	LED_Change(0, (state == FORWARD_LED) | (state == LEFT_BLINKER));
	LED_Change(1, (state == FORWARD_LED) | (state == RIGHT_BLINKER));
	LED_Change(2, (state == BACKWARD_LED) | (state == LEFT_BLINKER));
	LED_Change(3, (state == BACKWARD_LED) | (state == RIGHT_BLINKER));
	
	if (state == LEFT_BLINKER | state == RIGHT_BLINKER) {
		TIMER2_Start();
	} else {
		TIMER2_Stop();
	}
}

void MOTOR_DIR(uint32_t MOTOR_TYPE, Motor_State state) {
	if (state == FORWARD){
		PORT0->SET = (1 << (9 - MOTOR_TYPE * 2));
		PORT0->CLR = (1 << (8 - MOTOR_TYPE * 2));
	} else if (state == BACKWARD){
		PORT0->CLR = (1 << (9 - MOTOR_TYPE * 2));
		PORT0->SET = (1 << (8 - MOTOR_TYPE * 2));
	} else if (state == STOP) {
		PORT0->SET = (1 << (9 - MOTOR_TYPE * 2));
		PORT0->SET = (1 << (8 - MOTOR_TYPE * 2));
	} 
}

void update() {	
	if (Joystick_Center_Pressed()) {
		TURN_LEFT_FLAG = TURN_RIGHT_FLAG = FORWARD_FLAG = BACKWARD_FLAG = 0;
		PWM_MOTOR_Write(0, 0);
		PWM_MOTOR_Write(0, 1);
		MOTOR_DIR(0, STOP);
		MOTOR_DIR(1, STOP);
		LED_Adjuster(STOP_LED);
	} else if (Joystick_Up_Pressed()) {
		TURN_LEFT_FLAG = TURN_RIGHT_FLAG = BACKWARD_FLAG = 0;
		FORWARD_FLAG = 1;
		PWM_MOTOR_Write(ROBOT_SPEED, 0);
		PWM_MOTOR_Write(ROBOT_SPEED, 1);
		MOTOR_DIR(0, FORWARD);
		MOTOR_DIR(1, FORWARD);
		LED_Adjuster(FORWARD_LED);
	} else if (Joystick_Down_Pressed()) {
		TURN_LEFT_FLAG = TURN_RIGHT_FLAG = FORWARD_FLAG = 0;
		BACKWARD_FLAG = 1;
		PWM_MOTOR_Write(ROBOT_SPEED, 0);
		PWM_MOTOR_Write(ROBOT_SPEED, 1);
		MOTOR_DIR(0, BACKWARD);
		MOTOR_DIR(1, BACKWARD);
		LED_Adjuster(BACKWARD_LED);
	} else if (Joystick_Right_Pressed()) {
		TURN_LEFT_FLAG = BACKWARD_FLAG = FORWARD_FLAG = 0;
		TURN_RIGHT_FLAG = 1;
		PWM_MOTOR_Write(ROBOT_SPEED, 0);
		PWM_MOTOR_Write(ROBOT_SPEED, 1);
		MOTOR_DIR(0, BACKWARD);
		MOTOR_DIR(1, FORWARD);
		LED_Adjuster(RIGHT_BLINKER);
	} else if (Joystick_Left_Pressed()) {
		TURN_RIGHT_FLAG = BACKWARD_FLAG = FORWARD_FLAG = 0;
		TURN_LEFT_FLAG = 1;
		PWM_MOTOR_Write(ROBOT_SPEED, 0);
		PWM_MOTOR_Write(ROBOT_SPEED, 1);
		MOTOR_DIR(0, FORWARD);
		MOTOR_DIR(1, BACKWARD);
		LED_Adjuster(LEFT_BLINKER);
	}
}

int main() {
	init();
	while(1) {
		update();
	}
}
