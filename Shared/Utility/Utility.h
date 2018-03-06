#ifndef _UTILITY_h
#define _UTILITY_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

// null check macro
#define isNull(a) (a == NULL || a[0] == '\0') ? true : false

#endif
