#include "Parameters.h"

volatile uint32_t TURN_LEFT_FLAG = 0, TURN_RIGHT_FLAG = 0, FORWARD_FLAG = 0, BACKWARD_FLAG = 0;

const uint32_t ROBOT_SPEED = 60;
const uint32_t ROTATION_NUMBER_FOR_90_DEGREE = 100;

const uint32_t MOTOR_PINS[2][2] = {{9, 8}, {1, 0}};
