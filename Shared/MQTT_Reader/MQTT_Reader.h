#pragma once
#include <Adafruit_MQTT_Client.h>
#include <WiFi101.h>
#include "Secrets.h"

// MQTT_reader.h
#ifndef _MQTT_READER_h
#define _MQTT_READER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

// mqtt reader class for retrieving feed values from the server
class MQTT_Reader
{
	private:
		WiFiClient* wifi;
		Adafruit_MQTT_Client* mqtt;
		Adafruit_MQTT_Subscribe* feed;
		void connect();
	public:
		void init();
		char* read(uint16_t timeout);
};
#endif

