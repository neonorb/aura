#include "clock.h"

#include "../../kernel/ports.h"
#include "../../kernel/log.h"

#include "pit.c"
#include "rtc.c"

static ClockHandler clockHandler;

static void clockPitHandler() {
	if (clockHandler != 0) {
		clockHandler();
	}
}

static void clockRtcHandler() {
}

void clock_initialize() {
	pit_initialize();
	pit_handler(&clockPitHandler);

	rtc_initialize();
	rtc_handler(&clockRtcHandler);
}

void clock_handler(ClockHandler handler) {
	clockHandler = handler;
}
