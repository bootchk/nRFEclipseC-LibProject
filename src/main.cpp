
#include "nrf_power.h"
#include "nrf_delay.h"


void startHFClock() {
	/* Start 16 MHz crystal oscillator */
	NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
	NRF_CLOCK->TASKS_HFCLKSTART = 1;

	/* Wait for the external oscillator to start up */
	while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0) {}
}


void delay() {
	__ASM volatile (
#if ( defined(__GNUC__) && (__CORTEX_M == (0x00U) ) )
			".syntax unified\n"
#endif
			" NOP\n"
	);
}

void delay2() {
	__ASM volatile (
			" NOP\n"
	);
}

void delay3() {

}

/*
 * Test whether POFCON requires a delay, or whether as Product Spec says:
 * " This event will also be generated if the supply voltage is already below VPOF at the time the POF is enabled,
 *  or if VPOF is re-configured to a level above the supply voltage."
 *
 * To test: power unit with 1.9V, break into running function with debugger and check value of counters.
 * Since Vcc is 1.9V, vccGreater should never be non-zero and vccLess should be non-zero.
 */
int main() {

	uint64_t vccLess = 0;
	uint64_t vccGreater = 0;

	// POFCON requires 1v2 power supply.  This ensures it.
	// According to PS rev3:
	// "To save power, the power-fail comparator is not active in System OFF or in System ON when HFCLK is not running."
	startHFClock();

	while (true) {

		// assert event is clear and POFCON is disabled
		// assert 1v2 power rail is active

		// set threshold and enable
		nrf_power_pofcon_set(true, NRF_POWER_POFTHR_V23);
		//NRF_POWER->POFCON = 0X15;	// Equivalent to above

		(void) NRF_POWER->POFCON;	// flush arm write cache

		// POFCON sets event if Vcc less than threshold, event remains clear otherwise

		// >>>>>  Crux:  Some delay seems necessary, but undocumented?
		// Without this delay, event is not set before we check it below.
		// nrf_delay_us(1);
		//delay3();

		bool isPOF = nrf_power_event_check(NRF_POWER_EVENT_POFWARN);

		// Log for debugger to read
		if (isPOF)
			vccLess += 1;
		else
			vccGreater +=1;

		// disable
		nrf_power_pofcon_set(false, NRF_POWER_POFTHR_V23);

		nrf_power_event_clear(NRF_POWER_EVENT_POFWARN);

	}

	return 0;
}
