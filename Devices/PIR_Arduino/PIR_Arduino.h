#ifndef _PIR_ARDUINO_h
#define _PIR_ARDUINO_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

// shared libraries
#include "MQTT_Writer.h"
#include "Utility.h"

// external libraries
#include <SPI.h>
#include <WiFi101.h>

enum PinDef : uint8_t {
	P_Pir = 2,
	P_Led = 7,

	// atwinc pins
	P_Rst = 8, // reset
	P_Irq = 9, // interrupts
	P_Cs = 10  // chip select
};
#endif