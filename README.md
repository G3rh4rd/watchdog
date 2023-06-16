# watchdog

A thread based software watchdog that fires callback function, when it's not resetted after a specified time intervall.

Example usage:
'''
#include "watchdog_timer.h"

#define WATCHDOG_RESET_TIME_MS 2000

static WatchdogTimer watchdogTimer;

// example callback function
void on_watchdog_fired_callback() {
    exit(-1);
}

int main() {
	// initialize watchdog with the time interval it has to be resetted and a callback
	watchdogTimer = WatchdogTimer(WATCHDOG_RESET_TIME_MS, on_watchdog_fired_callback);
	
	// start watchdog. it can also be stopped, if needed.
	watchdogTimer.start();
	
	while(true) {
		/*
			do some stuff
		*/
		
		// reset watchdog. otherwise it will fire, it's not called within specified time intervall.
		watchdogTimer.reset();
	}
	
	return 0;
}
'''
