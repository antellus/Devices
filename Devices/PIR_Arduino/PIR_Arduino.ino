// local references
#include "PIR_Arduino.h"

// mqtt writer
MQTT_Writer writer;

// default state change
byte state = LOW;

// arduino setup runs once per power up
void setup() {
	// required for winc breakout
	WiFi.setPins(P_Cs, P_Irq, P_Rst);

	// init serial port
	while (!Serial);
	Serial.begin(115200);

	// init wifi board
	Serial.print(F("Init WiFi module..."));
	if (WiFi.status() == WL_NO_SHIELD) {
		UTIL_PRINTLN(F("not present"));
		while (true); // don't continue:
	}
	UTIL_PRINTLN(F("ATWINC OK!"));

	// init pins
	pinMode(P_Led, OUTPUT);
	pinMode(P_Pir, INPUT);

	// init the mqtt writer
	writer.init();
}

// arduino loop runs continuously
void loop()
{
	// publish state changes
	if (digitalRead(P_Pir) != state) {
		state = !state;
		digitalWrite(P_Led, state);
		UTIL_PRINTLN((state) ? F("Motion Detected!") : F("Motion Ended!"));
		
		bool published = writer.write(state);
		UTIL_PRINTLN((published) ? F("Published!") : F("Failed to publish!"));
	}
}