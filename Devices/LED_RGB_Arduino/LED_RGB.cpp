#include "LED_RGB.h"

// Inits the LED RGB controller
void LED_RGB::init(uint8_t pinR, uint8_t pinG, uint8_t pinB) {
	// set pin mode
	pinMode(pinR, OUTPUT);
	pinMode(pinG, OUTPUT);
	pinMode(pinB, OUTPUT);
	
	// set pin definition
	r = pinR;
	g = pinG;
	b = pinB;
	
	// default last cmd
	lastCmd = CM_fade;  

	// default rgb
	lastRgb = { 0, 0, 0 };

	// set color sequence - memory is limited, so use ints instead of strings
	// circular linked list where starting node is blue
	addNode({ 0x00,0x00,0xff });  //blue   #0000ff      255 - start the circle
	addNode({ 0xff,0x00,0xff });  //violet #ff00ff 16711935
	addNode({ 0xff,0x00,0x00 });  //red    #ff0000 16711680
	addNode({ 0xff,0xff,0x00 });  //yellow #ffff00 16776960
	addNode({ 0x00,0xff,0x00 });  //green  #00ff00    65280
	addNode({ 0x00,0xff,0xff });  //teal   #00ffff    65535- end the circle
}

// Command handler for the specified value, maps feed data to a command,
CmdType LED_RGB::cmdHandler(char* val) {
	// handle new or existing state command
	UTIL_PRINTLN(F("Handling cmd"));

	// hex requires a string starting with #
	if (val[0] == CM_hex) {
		setRgb(val);
		lastCmd = CM_hex;
	} // requires F char
	else if (val[0] == CM_fade) {
		fade();
		lastCmd = CM_fade;
	} // up requires U char
	else if (val[0] == CM_up) {
		up();
		lastCmd = CM_up;
	} // down requires D char
	else if (val[0] == CM_dn) {
		down();
		lastCmd = CM_dn;
	}
	else if (val[0] == CM_pulse) {
		pulse();
		lastCmd = CM_pulse;
	}

	return lastCmd;
}

// Resumes continues a command. Designed to be called in the main loop for running state.
void LED_RGB::resume() {
	UTIL_PRINT(F("Resuming..."));UTIL_PRINTLN((char)lastCmd);

	switch (lastCmd) {
	case CM_fade:
		fade();
		break;
	case CM_pulse:
		pulse();
		break;
	default:
		break;

	}
}

// Pulses red twice per call. Designed to be called in the main loop for running state.
void LED_RGB::pulse() {
	// fade to black
	setRgb({ 0,0,0 });

	// pulses 2x
	for (uint8_t i = 0; i < 2; i++) {

		// pulse on red
		uint8_t n = 0;
		while(n < 255){
			analogWrite(r, n+=5); // fast fade in
		}

		while (n > 0) {
			analogWrite(r, n-=5);
			delay(FADESPEED); // slower fade out
		}
	}
}

// Sets rgb pins from provided hex str, e.g., #ffffff
void LED_RGB::setRgb(char* val) {
	if (!isNull(val)) {
		// set the pointer after # so we can convert to ulong
		setRgb(ultoRgb(strtoul((const char*)&val[1], NULL, 16)));
	}
}

// Sets rgb pings from provided long value
void LED_RGB::setRgb(unsigned long val) {
	setRgb(ultoRgb(val));
}

// Sets rgb pins to the specified value
void LED_RGB::setRgb(Rgb val) {
	fade(r, lastRgb.r, val.r);
	fade(g, lastRgb.g, val.g);
	fade(b, lastRgb.b, val.b);
}

// Fades pins in a circular list
void LED_RGB::fade() {
	// next color in sequence
	lastNode = lastNode->next;
	setRgb(lastNode->rgb);
}

// Fades pin from value to value, up or down
void LED_RGB::fade(uint8_t pin, uint8_t from, uint8_t to) {
	while (from < to) {
		analogWrite(pin, ++from);
		delay(FADESPEED);
	}

	while (from > to) {
		analogWrite(pin, --from);
		delay(FADESPEED);
	}

	// store for later
	setlast(pin, from);
}

// Increases non zero, non max value by MULTI to max 
uint8_t LED_RGB::up(uint8_t val) {
	return (val > 0 && val < 0xff) ? min(val * MULTI, 0xff) : val;
}

// Decreases non zero value by MULTI to 0
uint8_t LED_RGB::down(uint8_t val) {
	return (val > 0 && val <= 0xff) ? max(val / MULTI, 0) : val;
}

// Increases brightness to max
void LED_RGB::up() {
	fade(r, lastRgb.r, up(lastRgb.r));
	fade(g, lastRgb.g, up(lastRgb.g));
	fade(b, lastRgb.b, up(lastRgb.b));
}

// Decreases brightness to off
void LED_RGB::down() {
	fade(r, lastRgb.r, down(lastRgb.r));
	fade(g, lastRgb.g, down(lastRgb.g));
	fade(b, lastRgb.b, down(lastRgb.b));
}

// Set the last (current state) for the specified pin
void LED_RGB::setlast(uint8_t pin, uint8_t val) {
	if (pin == r) {
		lastRgb.r = val;
	}
	else if (pin == g) {
		lastRgb.g = val;
	}
	else if (pin == b) {
		lastRgb.b = val;
	}
}

// Gets an RGB value {rr,gg,bb} from the provided long value
Rgb LED_RGB::ultoRgb(unsigned long val) {
	Rgb result = { 0, 0, 0 };

	// bit shift and mask
	result.r = val >> 16 & 0xff;
	result.g = val >> 8 & 0xff;
	result.b = val & 0xff;

	return result;
}

// creates a new node, adds it to the list
void LED_RGB::addNode(Rgb val) {
	RgbNode *node = new RgbNode { val, NULL };

	if (lastNode == NULL) {
		// first node
		lastNode = node;
		lastNode->next = lastNode;
	}
	else {
		// more than one node, rearrange pointers for circular ref
		node->next = lastNode->next;
		lastNode->next = node;
		lastNode = node;
	}
}