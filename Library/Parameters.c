#include "Parameters.h"

#include "main.h"

volatile uint32_t TURN_LEFT_FLAG = 0, TURN_RIGHT_FLAG = 0, FORWARD_FLAG = 0, BACKWARD_FLAG = 0;

uint32_t ROBOT_SPEED = 0;
const uint32_t HOLE_NUMBER_FOR_90_DEGREE = 6;

const uint32_t OBSTACLE_DISTANCE = 15;	// Maximum distance that car can get close to an object in the front line.
const uint32_t OBSTACLE_ESCAPE_DISTANCE = 30;  // Minimum distance until car gets safe from object.

const uint32_t MOTOR_PINS[2][2] = {{9, 8}, {1, 0}};

const uint32_t LED_PINS[4] = {5, 6, 7, 11};

uint8_t START_MODE = MANUAL;	// Initial mode is manual

const uint8_t COMM_TYPE = BASIC;

// Change Start method via Push Button, UART signal or Wifi comminication
void changeStartMode (START_State state) {
	START_MODE = state;
	MOTOR_Direction(1, STOP);
	MOTOR_Direction(0, STOP);
	LED_Adjuster(STOP_LED);
	TURN_LEFT_FLAG = TURN_RIGHT_FLAG = FORWARD_FLAG = BACKWARD_FLAG = 0;
	if (COMM_TYPE == WIFI_COMM) {
		ESP8266_sendCommand("AT+CIPSTART=\"TCP\",\"192.168.0.105\",8080\r\n");
		wait(10);
		ESP8266_waitResponseEnd();
		
		if (START_MODE == AUTO) {
			ESP8266_sendCommand("AT+CIPSEND=57\r\n");
			wait(100);
			ESP8266_waitResponseEnd();
			
			ESP8266_sendCommand("GET /HWLAB_IoT/SetInformation?ID=1&DATA=AUTO HTTP/1.0\r\n\r\n");
			wait(100);
		}
		else {
			ESP8266_sendCommand("AT+CIPSEND=59\r\n");
			wait(100);
			ESP8266_waitResponseEnd();
			
			ESP8266_sendCommand("GET /HWLAB_IoT/SetInformation?ID=1&DATA=MANUEL HTTP/1.0\r\n\r\n");
			wait(100);
		}
	} 
	
	
}
