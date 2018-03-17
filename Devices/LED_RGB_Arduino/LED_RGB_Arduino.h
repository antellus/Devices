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

// Note: LED pins need same frequency otherwise,
// the frequencies will be out of sync and will cause 
// flickering at duty cycles less than 100%. 
// See: http://playground.arduino.cc/Main/TimerPWMCheatsheet
// Pins  5 &  6 976.525 (Timer 0) Fast Mode 256 cycle length
// Pins  9 & 10  490.2  (Timer 1) Phase Correct Mode 510 cycle length  
// Pins 11 & 3   490.2  (Timer 2) Phase Correct Mode 510 cycle length
enum PinDef : uint8_t {
	// led pins
	P_R = 3, 
	P_G = 9,
	P_B = 10,

	// atwinc pins
	P_Rst = 4, // 4 reset
	P_Irq = 7, // 7 interrupts
	P_Cs = 8  // 8 chip select
};
#endif