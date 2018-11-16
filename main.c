#include "LPC407x_8x_177x_8x.h"

#include "Library/GPIO.h"
#include "Library/Timer.h"
#include "Library/PWM.h"
#include "Library/Joystick.h"
#include "Library/LED.h"
#include "Library/Ultrasonic.h"
#include "Library/Wait.h"

int milisecond = 1000;

void update() {
	int i;
	for(i = 0; i < milisecond * 12000; i++);
}

int main() {
	while(1) {
		update();
	}
}