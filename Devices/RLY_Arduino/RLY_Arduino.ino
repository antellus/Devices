// local references
#include "RLY.h"
#include "RLY_Arduino.h"
#include "MQTT_Reader.h"

// external libraries
#include <SPI.h>
#include <WiFi101.h>

// mqtt reader
MQTT_Reader reader;

// relay handler
RLY relay;

// arduino setup runs once per power up
void setup() {
	// required for winc breakout
	WiFi.setPins(P_Cs, P_Irq, P_Rst);

	// init serial port
	while (!Serial);
	Serial.begin(115200);

	// init wifi board
	Serial.print(F("Init WiFi module..."));
	// check for the presence of the breakout
	if (WiFi.status() == WL_NO_SHIELD) {
		Serial.println(F("not present"));
		while (true); // don't continue:
	}
	Serial.println(F("ATWINC OK!"));

	// init the relay handler
	relay.init(P_Rly);

	// init the mqtt reader
	reader.init();
}

// arduino loop runs continuously
void loop() {
	// reader waits on timeout for incoming packets
	char* value = reader.read(5000);

	if (value != NULL && value[0] != '\0') {
		// handle the feed value 0 or 1 expected
		relay.cmdHandler(value);
	}
}