#include "Wait.h"

// wait for given millisecond
void wait(uint32_t miliseconds) {
	uint32_t i;
	uint32_t totalDuration = miliseconds*24000;
	for(i=0;i<totalDuration;i++);
}

// wait for given microsecond
void waitMicroseconds(uint32_t microseconds) {
	uint32_t i;
	uint32_t totalDuration = microseconds*24;
	for(i=0;i<totalDuration;i++);
}
