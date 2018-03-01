// local references
#include "LED_RGB.h"
#include "MQTT_reader.h"
#include "Pin_Definition.h"

// external libraries
#include <SPI.h>
#include <WiFi101.h>

// led rgb handler
LED_RGB ledrgb;

// mqtt reader
MQTT_Reader reader;

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

	// init led rgb controller
	ledrgb.init(P_R, P_G, P_B);

	// init the mqtt reader
	reader.init();
}

// arduino loop runs continuously
void loop() {
	// reader waits on timeout for incoming packets
	char* value = reader.read(5000);
	
	// handle the feed value, i.e., setRgb, fade, etc.
	ledrgb.cmdHandler(value);
}