#include "Parameters.h"

volatile uint32_t TURN_LEFT_FLAG = 0, TURN_RIGHT_FLAG = 0, FORWARD_FLAG = 0, BACKWARD_FLAG = 0;

uint32_t ROBOT_SPEED = 0;
const uint32_t HOLE_NUMBER_FOR_90_DEGREE = 6;

const uint32_t MOTOR_PINS[2][2] = {{9, 8}, {1, 0}};

const uint32_t LED_PINS[4] = {5, 6, 7, 11};

uint8_t START_MODE = MANUAL;

void changeStartMode (START_State state) {
	START_MODE = state;
}
