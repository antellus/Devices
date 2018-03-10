#include "RLY.h"

// inits the relay
void RLY::init(uint8_t pinRly){
	pin = pinRly;
	pinMode(pin, OUTPUT);
	lastCmd = CM_open; // off by default
}

// handles commands from mqtt feed
void RLY::cmdHandler(char* val){
	// handle the cmd  from feed, only values of 0 or 1 valid
	if (NULL != val && val[0] != '\0') {
		uint8_t cmd = atoi(val);

		// only respond to O (open circuit) or C (close circuit);
		switch (cmd)
		{
		case CM_open:
			open();
			lastCmd = CM_open;
			break;
		case CM_close:
			close();
			lastCmd = CM_close;
			break;
		}
	}
}

// opens the relay circuit (off mode)
void RLY::open(){
	UTIL_PRINTLN(F("Opening the circuit!"));
	digitalWrite(pin, LOW);
}

// closes the relay circuit (on mode)
void RLY::close(){
	Serial.println(F("Closing the circuit!"));
	UTIL_PRINTLN(pin, HIGH);
}
