// local 
#include "LED_RGB_Arduino.h"

// led rgb handler
LED_RGB ledrgb;

// mqtt reader
MQTT_Reader reader;

// Utility class
Utility util;

// arduino setup runs once per power up
void setup() {
	// required for winc breakout
	WiFi.setPins(P_Cs, P_Irq, P_Rst);

	// init serial port
	while (!Serial);
	Serial.begin(115200);

	// init wifi board
	UTIL_PRINT(F("Init WiFi module..."));
	if (WiFi.status() == WL_NO_SHIELD) {
		UTIL_PRINTLN(F("not present"));
		while (true); // don't continue:
	}
	UTIL_PRINT(F("ATWINC OK!"));

	// init led rgb controller
	ledrgb.init(P_R, P_G, P_B);

	// init the mqtt reader
	reader.init();
}

// arduino loop runs continuously
void loop() {
	UTIL_PRINT(F("Free RAM:")); UTIL_PRINTLN(util.freeRam());

	// reader waits on timeout for incoming packets
	char* value = reader.read(5000);

	if (!isNull(value)) {
		UTIL_PRINTLN((char*)value);
		// handle the feed value: F, U, D, #ffffff
		ledrgb.cmdHandler(value);
	}
	else{
		// resumes the current state: fade, flash, etc
		ledrgb.resume();
	}
}