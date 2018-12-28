#ifndef PARAMETERS_H
#define PARAMETERS_H

#include "LPC407x_8x_177x_8x.h"

typedef enum {
	MANUAL, AUTO
} START_State;

typedef enum {
	BASIC, UART_COMM, WIFI_COMM
} COMM_State;

extern uint32_t ROBOT_SPEED;
extern const uint32_t HOLE_NUMBER_FOR_90_DEGREE;

extern const uint32_t OBSTACLE_DISTANCE;
extern const uint32_t OBSTACLE_ESCAPE_DISTANCE;

extern uint8_t START_MODE;

extern volatile uint32_t TURN_LEFT_FLAG, TURN_RIGHT_FLAG, FORWARD_FLAG, BACKWARD_FLAG;
extern const uint32_t MOTOR_PINS[2][2];
extern const uint32_t LED_PINS[4];

extern const uint8_t COMM_TYPE;

void changeStartMode (START_State state);
#endif
