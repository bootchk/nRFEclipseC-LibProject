
/*
 * Example main.
 * Tests using a c++ static library implementing LongClockTimer
 * With debug logging using Segger RTT.
 */





#include "nrf.h"	// SEV


void sleep() {
	__SEV();
	__WFE();	// Since internal event flag is set, this clears it without sleeping
	__WFE();	// This actually sleeps until event
}





/*
 * Test whether disabling radio affects current consumption.
 */
int main3() {
	// assert embedded system startup is done and calls main.

	//NRF_RADIO->POWER = 0;


	while (true) {
		sleep();
	}

	return 0;
}
