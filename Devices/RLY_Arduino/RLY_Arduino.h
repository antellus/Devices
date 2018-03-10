#ifndef _RLY_ARDUINO_h
#define _RLY_ARDUINO_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

// local
#include "RLY.h"

// shared libraries
#include "MQTT_Reader.h"
#include "Utility.h"

// external libraries
#include <SPI.h>
#include <WiFi101.h>

// pinout
enum PinDef : uint8_t {
	// Relay switch
	P_Rly = 7,

	// atwinc pins
	P_Rst = 8, // reset
	P_Irq = 9, // interrupts
	P_Cs = 10  // chip select
};
#endif