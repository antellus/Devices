#ifndef _LED_RGB_ARDUINO_h
#define _LED_RGB_ARDUINO_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

// local
#include "LED_RGB.h"

// shared libraries
#include "MQTT_Reader.h"
#include "Utility.h"

// external libraries
#include <SPI.h>
#include <WiFi101.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>

enum PinDef : uint8_t {
	// led pins
	P_G = 3,
	P_R = 5, 
	P_B = 6,

	// atwinc pins
	P_Rst = 8, // reset
	P_Irq = 9, // interrupts
	P_Cs = 10  // chip select
};
#endif