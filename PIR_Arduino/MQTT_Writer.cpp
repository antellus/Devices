#include "Secrets.h"
#include "MQTT_Writer.h"
#include <Adafruit_MQTT_Client.h>

// wifi client
WiFiClient* wifi;

// mqtt client and feed
Adafruit_MQTT_Client* mqtt;

// mqtt feed
Adafruit_MQTT_Publish* feed;

// initializes the MQTT_writer class
void MQTT_Writer::init()
{
	// int wifi, mqtt and subscription
	wifi = new WiFiClient();
	mqtt = new Adafruit_MQTT_Client(wifi, AIO_SERVER, AIO_SERVERPORT, AIO_CID, AIO_USERNAME, AIO_KEY);
	feed = new Adafruit_MQTT_Publish(mqtt, AIO_USERNAME AIO_PATH);

	connect();
}

// Connect board to wifi and mqtt, waits indefinitely until connected
void MQTT_Writer::connect()
{
	int8_t result;

	// connect to wifi
	while (WiFi.status() != WL_CONNECTED) {
		Serial.print(F("Connect SSID: "));
		Serial.println(W_SSID);
		WiFi.begin(W_SSID, W_PASS);

		// check connection every second for 10 seconds
		uint8_t timeout = 10;
		while (timeout && (WiFi.status() != WL_CONNECTED)) {
			timeout--;
			delay(1000);
		}
	}

	// Return if connected
	if (mqtt->connected()) {
		return;
	}

	Serial.print(F("Connect MQTT... "));
	while ((result = mqtt->connect()) != 0) { // connect will return 0 for connected
		Serial.println(mqtt->connectErrorString(result));
		Serial.println(F("Retrying"));
		mqtt->disconnect();
		delay(5000);  // wait 5 seconds
	}
	Serial.println(F("Connected!"));
}

bool MQTT_Writer::write(bool val)
{
	// handle incoming feed from mqtt
	Serial.println(F("Writing..."));

	// re-connect wifi and mqtt if needed
	connect();

	// publishes value to mqtt
	return feed->publish((uint32_t)val);
}