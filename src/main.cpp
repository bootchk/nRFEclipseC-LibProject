
/*
 * Example main.
 * Tests using a c++ static library implementing LongClockTimer
 * With debug logging using Segger RTT.
 */

// API of static library
#include "nRF5x.h"



#ifdef OLD
Used when testing nRFCounter project, which is now folded into nRF5x library

To revert, need:
nRFCounter.a
SEGGER_printf.c etc
a few other mods below in the changed API to nRFCounter

#include "nRFCounter.h"

#include "SEGGER_RTT.h"
void logLongLong(uint64_t value ){
	// Print on one line
	(void) SEGGER_RTT_printf(0, "x%04x", *(((uint32_t*) &value) + 1)  );	// MS word
	(void) SEGGER_RTT_printf(0, "%04x\n", value);	// LS word and newline
}
void log(char * string) {

}


#include "nrf.h"	// SEV
void sleep() {
	__SEV();
	__WFE();	// Since internal event flag is set, this clears it without sleeping
	__WFE();	// This actually sleeps until event
}

#define First 0
#define Second 1
#define Third 2

#endif


MCU mcu;
Nvic nvic;
LongClockTimer longClockTimer;


// Nothing, just wakes us.
void timeoutCallback0() {log("Timeout 0\n");}
void timeoutCallback1() {log("Timeout 1\n");}
void timeoutCallback2() {log("Timeout 2\n");}





void timerPattern0() {
	// Keep all timers running
	// Expires 1,1,0,1,1,0,2,.... more or less
	if (! longClockTimer.isTimerStarted(First)) {
		longClockTimer.startTimer(First, 20000, timeoutCallback0 );
	}

	if (! longClockTimer.isTimerStarted(Second)) {
		longClockTimer.startTimer(Second, 10000, timeoutCallback1 );
	}

	if (! longClockTimer.isTimerStarted(Third)) {
		longClockTimer.startTimer(Third, 40000, timeoutCallback2 );
	}
}


void timerPattern1() {
	// Should expire 0,1,2,0,1,2,...
	if (! longClockTimer.isTimerStarted(First)) {
		longClockTimer.startTimer(First, 20000, timeoutCallback0 );
	}
	else longClockTimer.cancelTimer(First);

	if (! longClockTimer.isTimerStarted(Second)) {
		longClockTimer.startTimer(Second, 10000, timeoutCallback1 );
	}
	else longClockTimer.cancelTimer(Second);

	if (! longClockTimer.isTimerStarted(Third)) {
		longClockTimer.startTimer(Third, 40000, timeoutCallback2 );
	}
}

int main() {
	// assert embedded system startup is done and calls main.

	initLogging();

	longClockTimer.init(&nvic);

	while (true) {
		// delay
		for(int i=0; i++; i<10000) {
			int j = 0;
			j++;
		}

		logLongLong( longClockTimer.nowTime() );

		timerPattern1();

		mcu.sleep();
		// woken by timeout or other event (rollover of LongClock)
	}

	return 0;
}
