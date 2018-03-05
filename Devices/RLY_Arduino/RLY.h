#ifndef _RLY_h
#define _RLY_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

// feed commands
enum CmdType :byte {
	CM_open  = 0x0,
	CM_close = 0x1
};

// class def for relay controller
class RLY
{
private:
	CmdType lastCmd;
	uint8_t pin;
	void open();
	void close();
public:
	void init(uint8_t pinRly);
	void cmdHandler(char* val);
};
#endif