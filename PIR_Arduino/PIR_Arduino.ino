// local references
#include "MQTT_Writer.h"
#include "Pin_Definition.h"

// external libraries
#include <SPI.h>
#include <WiFi101.h>

// mqtt writer
MQTT_Writer writer;

// state change
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
	// check for the presence of the breakout
	if (WiFi.status() == WL_NO_SHIELD) {
		Serial.println(F("not present"));
		while (true); // don't continue:
	}
	Serial.println(F("ATWINC OK!"));

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
		Serial.println((state) ? F("Motion Detected!") : F("Motion Not Detected!"));
		
		bool published = writer.write(state);
		Serial.println((published) ? F("Published to feed!") : F("Failed to publish!"));
	}
}