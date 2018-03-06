#ifndef _SECRETS_h
#define _SECRETS_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

// Wifi credentials
#define W_SSID "ssid"
#define W_PASS "pass"  

// Mqtt server credentials
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "username"
#define AIO_KEY         "key"
#define AIO_CID         "ffaa00"
#define AIO_PATH        "/feeds/feed"

#endif