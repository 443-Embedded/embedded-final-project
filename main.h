#include "LPC407x_8x_177x_8x.h"

#include "Library/GPIO.h"
#include "Library/Timer.h"
#include "Library/PWM.h"
#include "Library/Joystick.h"
#include "Library/LED.h"
#include "Library/Wait.h"
#include "Library/Parameters.h"

typedef enum {
	STOP,	FORWARD, BACKWARD
} Motor_State;

typedef enum {
	RIGHT_BLINKER, LEFT_BLINKER, FORWARD_LED, BACKWARD_LED, STOP_LED
} LED_State;