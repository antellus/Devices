#pragma once
#include <Adafruit_MQTT_Client.h>
#include <WiFi101.h>
#include "Secrets.h"
#include "Utility.h"

// MQTT_writer.h
#ifndef _MQTT_WRITER_h
#define _MQTT_WRITER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

// mqtt writer class for publishing feed values to the server
class MQTT_Writer
{
	private:
		WiFiClient* wifi;
		Adafruit_MQTT_Client* mqtt;
		Adafruit_MQTT_Publish* feed;
		void connect();
	public:
		void init();
		bool write(bool val);
};
#endif
