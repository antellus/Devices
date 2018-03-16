#include "Utility.h"

// Returns available ram
int Utility::freeRam()
{
#ifdef UTIL_DEBUG
	extern int __heap_start, *__brkval;
	int v;
	return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
#else
	return 0;
#endif
}