#pragma once

#ifndef _LED_RGB_h
#define _LED_RGB_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

// operating constants
#define FADESPEED 5    // millis fade delay
#define MULTI 1.50     // increase/decrease brightness multiplier

// null check macro
#define isNull(a) (a == NULL || a[0] == '\0') ? true : false

// struct for storing red, green blue 
struct Rgb { uint8_t r; uint8_t g; uint8_t b; };

// struct for storing color nodes
struct RgbNode { Rgb rgb; RgbNode* next; };

// feed cmd tokens
enum CmdType:unsigned char {
	CM_hex  = '#',
	CM_fade = 'F', 
	CM_up   = 'U', 
	CM_dn   = 'D'
};

// class def for led rgb controller
class LED_RGB
{
	private:
		uint8_t r,g,b;
		Rgb lastRgb;
		RgbNode *lastNode;
		CmdType lastCmd; 
		void setlast(uint8_t pin, uint8_t val);
		void addNode(Rgb val);
		Rgb ultoRgb(unsigned long val);
		uint8_t up(uint8_t val);
		uint8_t down(uint8_t val);
	public:
		void init(uint8_t pinR, uint8_t pinG, uint8_t pinB);
		void cmdHandler(char* val);
		void setRgb(unsigned long val);
		void setRgb(char* val);
		void setRgb(Rgb val);
		void fade();
		void fade(uint8_t pin, uint8_t from, uint8_t to);
		void up();
		void down();
};
#endif