#include "Secrets.h"
#include "MQTT_Reader.h"
#include <Adafruit_MQTT_Client.h>


// wifi client
WiFiClient* wifi;

// mqtt client and feed
Adafruit_MQTT_Client* mqtt;

// mqtt feed
Adafruit_MQTT_Subscribe* feed;

// initializes the MQTT_reader class
void MQTT_Reader::init()
{
	// int wifi, mqtt and subscription
	wifi = new WiFiClient();
	mqtt = new Adafruit_MQTT_Client(wifi, AIO_SERVER, AIO_SERVERPORT, AIO_CID, AIO_USERNAME, AIO_KEY);
	feed = new Adafruit_MQTT_Subscribe(mqtt, AIO_USERNAME AIO_PATH);

	// set subscriptions
	mqtt->subscribe(feed);
	connect();
}

// Connect board to wifi and mqtt, waits indefinitely until connected
void MQTT_Reader::connect()
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

char* MQTT_Reader::read(uint16_t timeout)
{
	char result[20] = { '\0' };

	// handle incoming feed from mqtt
	Serial.println(F("Reading..."));

	// re-connect wifi and mqtt if needed
	connect();

	// waits for incoming packets for 5 seconds on this subscription
	while (Adafruit_MQTT_Subscribe *subscription = mqtt->readSubscription(timeout)) {
		if (subscription == feed) {
			Serial.println((char*)feed->lastread);
			strcpy(result, (char*)feed->lastread);
			break;
		}
	}

	return result;
}