#include "Parameters.h"

#include "main.h"

volatile uint32_t TURN_LEFT_FLAG = 0, TURN_RIGHT_FLAG = 0, FORWARD_FLAG = 0, BACKWARD_FLAG = 0;

uint32_t ROBOT_SPEED = 0;
const uint32_t HOLE_NUMBER_FOR_90_DEGREE = 6;

const uint32_t OBSTACLE_DISTANCE = 15;
const uint32_t OBSTACLE_ESCAPE_DISTANCE = 30;

const uint32_t MOTOR_PINS[2][2] = {{9, 8}, {1, 0}};

const uint32_t LED_PINS[4] = {5, 6, 7, 11};

uint8_t START_MODE = MANUAL;

const uint8_t COMM_TYPE = UART_COMM;

void changeStartMode (START_State state) {
	START_MODE = state;
	MOTOR_Direction(1, STOP);
	MOTOR_Direction(0, STOP);
	LED_Adjuster(STOP_LED);
	TURN_LEFT_FLAG = TURN_RIGHT_FLAG = FORWARD_FLAG = BACKWARD_FLAG = 0;
}
