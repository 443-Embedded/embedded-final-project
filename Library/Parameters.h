#ifndef PARAMETERS_H
#define PARAMETERS_H

#include "LPC407x_8x_177x_8x.h"

extern const uint32_t ROBOT_SPEED;
extern const uint32_t ROTATION_NUMBER_FOR_90_DEGREE;

extern volatile uint32_t TURN_LEFT_FLAG, TURN_RIGHT_FLAG, FORWARD_FLAG, BACKWARD_FLAG;
extern const uint32_t MOTOR_PINS[2][2];
#endif
