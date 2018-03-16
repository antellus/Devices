// local 
#include "LED_RGB_Arduino.h"

// led rgb handler
LED_RGB ledrgb;

// mqtt reader
MQTT_Reader reader;

// utility class
Utility util;

// ledrgb command str: #000000, F, U, D, P, etc.
char cmd[20] = { '\0' };

// arduino setup runs once per power up
void setup() {
	// required for winc breakout
	WiFi.setPins(P_Cs, P_Irq, P_Rst);

#ifdef UTIL_PRINTER
	// init serial port
	while (!Serial);
	Serial.begin(115200);
#endif

	// init wifi board
	UTIL_PRINT(F("Init WiFi module..."));
	if (WiFi.status() == WL_NO_SHIELD) {
		UTIL_PRINTLN(F("not present"));
		while (true); // don't continue:
	}
	UTIL_PRINTLN(F("ATWINC OK!"));

	// init led rgb controller
	ledrgb.init(P_R, P_G, P_B);

	// init the mqtt reader
	reader.init();

	// read eeprom last saved value for resets
	UTIL_PRINT("Reading EEPROM...");
	EEPROM.get(0, cmd);
	UTIL_PRINTLN(cmd);
	
	// handle startup command
	ledrgb.cmdExecutor(cmd);

	// enable watchdog timer, reset after 8 seconds if reset not called
	wdt_enable(WDTO_8S);
}

// arduino loop runs continuously
void loop() {
	// reset watchdog timer
	wdt_reset();

	char* value = reader.read(5000);
	if (!isNull(value)) {
		char* v = ledrgb.feedHandler(value, cmd);
		if (!isNull(v)) {
			UTIL_PRINT("Writing EEPROM..."); UTIL_PRINTLN(cmd);
			EEPROM.put(0, cmd);
		}
	}
		
	ledrgb.cmdExecutor(cmd);
}
