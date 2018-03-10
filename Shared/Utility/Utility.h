#ifndef _UTILITY_h
#define _UTILITY_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

// Uncomment/comment to turn on/off debug output messages.
// #define UTIL_DEBUG

// Set where debug messages will be printed.
#define UTIL_PRINTER Serial

// Define actual debug output functions when necessary.
#ifdef UTIL_DEBUG
#define UTIL_PRINT(...) { UTIL_PRINTER.print(__VA_ARGS__); }
#define UTIL_PRINTLN(...) { UTIL_PRINTER.println(__VA_ARGS__); }
#else
#define UTIL_PRINT(...) {}
#define UTIL_PRINTLN(...) {}
#endif


// null check macro
#define isNull(a) (a == NULL || a[0] == '\0') ? true : false

// mqtt reader class for retrieving feed values from the server
class Utility
{
	public:
		int freeRam();
};
#endif
