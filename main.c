#include "Library/main.h"

void init() {
	GPIO_Init();
	Joystick_Init();
	PWM_Init();
	LED_Init();
	Timer_Init();
}

void LED_Adjuster(LED_State state) {
	LED_Change(0, (state == FORWARD_LED) | (state == LEFT_BLINKER));
	LED_Change(1, (state == FORWARD_LED) | (state == RIGHT_BLINKER));
	LED_Change(2, (state == BACKWARD_LED) | (state == LEFT_BLINKER));
	LED_Change(3, (state == BACKWARD_LED) | (state == RIGHT_BLINKER));
	
	if (state == LEFT_BLINKER | state == RIGHT_BLINKER) {
		TIMER3_Start();
		TIMER2_Start();
	} else {
		TIMER3_Stop();
		TIMER2_Stop();
	}
}

void MOTOR_Direction(uint32_t MOTOR_TYPE, Motor_State state) {
	if (state == FORWARD){
		PORT0->SET = (1 << MOTOR_PINS[MOTOR_TYPE][0]);
		PORT0->CLR = (1 << MOTOR_PINS[MOTOR_TYPE][1]);
	} else if (state == BACKWARD){
		PORT0->CLR = (1 << MOTOR_PINS[MOTOR_TYPE][0]);
		PORT0->SET = (1 << MOTOR_PINS[MOTOR_TYPE][1]);
	} else if (state == STOP) {
		PORT0->SET = (1 << MOTOR_PINS[MOTOR_TYPE][0]);
		PORT0->SET = (1 << MOTOR_PINS[MOTOR_TYPE][1]);
	} 
}

void update() {	
	uint32_t l = GPIO_PIN_Read(JOYSTICK_PORT,JOYSTICK_LEFT_MASK);
		uint32_t r = GPIO_PIN_Read(JOYSTICK_PORT,JOYSTICK_RIGHT_MASK);

		uint32_t u = GPIO_PIN_Read(JOYSTICK_PORT,JOYSTICK_UP_MASK);

		uint32_t d = GPIO_PIN_Read(JOYSTICK_PORT,JOYSTICK_DOWN_MASK);
	if (Joystick_Center_Pressed()) {
		TURN_LEFT_FLAG = TURN_RIGHT_FLAG = FORWARD_FLAG = BACKWARD_FLAG = 0;
		PWM_MOTOR_Write(0, 0);
		PWM_MOTOR_Write(0, 1);
		MOTOR_Direction(0, STOP);
		MOTOR_Direction(1, STOP);
		LED_Adjuster(STOP_LED);
	} else if (Joystick_Up_Pressed()) {
		TURN_LEFT_FLAG = TURN_RIGHT_FLAG = BACKWARD_FLAG = 0;
		FORWARD_FLAG = 1;
		PWM_MOTOR_Write(ROBOT_SPEED, 0);
		PWM_MOTOR_Write(ROBOT_SPEED, 1);
		MOTOR_Direction(0, FORWARD);
		MOTOR_Direction(1, FORWARD);
		LED_Adjuster(FORWARD_LED);
	} else if (Joystick_Down_Pressed()) {
		TURN_LEFT_FLAG = TURN_RIGHT_FLAG = FORWARD_FLAG = 0;
		BACKWARD_FLAG = 1;
		PWM_MOTOR_Write(ROBOT_SPEED, 0);
		PWM_MOTOR_Write(ROBOT_SPEED, 1);
		MOTOR_Direction(0, BACKWARD);
		MOTOR_Direction(1, BACKWARD);
		LED_Adjuster(BACKWARD_LED);
	} else if (Joystick_Right_Pressed()) {
		TURN_LEFT_FLAG = BACKWARD_FLAG = FORWARD_FLAG = 0;
		TURN_RIGHT_FLAG = 1;
		PWM_MOTOR_Write(ROBOT_SPEED, 0);
		PWM_MOTOR_Write(ROBOT_SPEED, 1);
		MOTOR_Direction(0, BACKWARD);
		MOTOR_Direction(1, FORWARD);
		LED_Adjuster(RIGHT_BLINKER);
	} else if (Joystick_Left_Pressed()) {
		TURN_RIGHT_FLAG = BACKWARD_FLAG = FORWARD_FLAG = 0;
		TURN_LEFT_FLAG = 1;
		PWM_MOTOR_Write(ROBOT_SPEED, 0);
		PWM_MOTOR_Write(ROBOT_SPEED, 1);
		MOTOR_Direction(0, FORWARD);
		MOTOR_Direction(1, BACKWARD);
		LED_Adjuster(LEFT_BLINKER);
	}
}

int main() {
	init();
	while(1) {
		update();
	}
}
