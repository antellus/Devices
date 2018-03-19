#ifndef _LED_RGB_h
#define _LED_RGB_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

// shared libraries
#include "Utility.h"

// operating constants
#define FADESPEED 5    // millis fade delay
#define MULTI 1.65     // increase/decrease brightness multiplier

// increase or decrease a by 1 relative to b
#define tick(a, b) (a == b) ? a : ((a < b) ? a + 1 : a - 1)

// struct for storing red, green blue 
struct Rgb { uint8_t r; uint8_t g; uint8_t b; };

// struct for storing color nodes
struct RgbNode { Rgb rgb; RgbNode* next; };

// feed cmd tokens
enum CmdType:unsigned char {
	CM_hex   = '#',  //35
	CM_dn    = 'D',  //68
	CM_fade  = 'F',  //70
	CM_pulse = 'P',  //80
	CM_up    = 'U',  //85

	// range
	CM_min = CM_hex,
	CM_max = CM_up
};

// class def for led rgb controller
class LED_RGB
{
	private:
		// properties
		uint8_t r,g,b;
		Rgb lastRgb;
		RgbNode *lastNode;
		CmdType lastCmd; 
		void addNode(Rgb val);
		Rgb ultoRgb(uint32_t val);
		uint32_t rgbtoul(Rgb val);
		uint32_t calc(uint32_t val, CmdType cmd);
		void pulse(uint8_t val);
	public:
		// methods
		void init(uint8_t pinR, uint8_t pinG, uint8_t pinB);
		void cmdExecutor(char* cmd);
		void cmdExecutor(CmdType cmd);
		char* feedHandler(char* val, char* cmd);
		void setRgb(Rgb val);
		void pulse();
		void fade();
		void up();
		void down();
};
#endif