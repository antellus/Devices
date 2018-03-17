#include "LED_RGB.h"

// Inits the LED RGB controller
void LED_RGB::init(uint8_t pinR, uint8_t pinG, uint8_t pinB) {
	// set pin mode
	pinMode(pinR, OUTPUT);
	pinMode(pinG, OUTPUT);
	pinMode(pinB, OUTPUT);
	
	// reset to 0 for startup
	analogWrite(pinR, 0);
	analogWrite(pinG, 0);
	analogWrite(pinB, 0);

	// set pin definition
	r = pinR;
	g = pinG;
	b = pinB;
	
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

// Executes a command for the specified value
void LED_RGB::cmdExecutor(char* cmd) {
	if (cmd == NULL || cmd[0] < CM_min || cmd[0] > CM_max) {
		UTIL_PRINT(F("Unknown cmd...")); UTIL_PRINTLN(cmd);
		return;
	}

	UTIL_PRINT(F("Executing cmd..."));

	// map cmd to function to execute
	switch (cmd[0]) {
	case CM_hex:
		setRgb(cmd);
		UTIL_PRINTLN(cmd);
		break;
	case CM_fade:
		fade();
		UTIL_PRINTLN(cmd);
		break;
	case CM_pulse:
		pulse();
		UTIL_PRINTLN(cmd);
		break;
	default:
		UTIL_PRINTLN(F("not a valid command!"));
		break;
	}
}

// Feed handler maps feed to command
char* LED_RGB::feedHandler(char* feed, char* cmd) {
	if (feed == NULL || feed[0] < CM_min || feed[0] > CM_max) {
		UTIL_PRINT(F("Unknown feed value...")); UTIL_PRINTLN(feed);
		cmd = NULL;
		return cmd;
	}

	UTIL_PRINT(F("Handling feed..."));

	// map feed to command
	switch (feed[0]) {
	// hex, pulse, fade are already commands that can be executed as-is
	case CM_hex:
	case CM_pulse:
	case CM_fade:
		strcpy(cmd, feed);
		UTIL_PRINTLN(cmd);
		break;
	// up / down commands map to a new rgb / hex, needed for power resets
	// so the str can be persisted by the caller, i.e., store result in eeprom
	case CM_up:
	case CM_dn:
	{
		// compute new 24bit long from rgb, calc up or down, returns #nnnnnn
		uint32_t val = rgbtoul(lastRgb);
		val = (feed[0] == CM_up) ? up(val) : down(val);
		ultoa(val, &cmd[1], HEX);
		cmd[0] = '#';
		UTIL_PRINTLN(cmd);
		break;
	}
	default:
		UTIL_PRINTLN(F("not a valid feed!"));
		break;
	}
	return cmd;
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
	if (!isNull(val) && val[0] == '#') {
		// set the pointer after # so we can convert to ulong
		setRgb(ultoRgb(strtoul((const char*)&val[1], NULL, HEX)));
	}
}

// Sets rgb pings from provided long value
void LED_RGB::setRgb(uint32_t val) {
	setRgb(ultoRgb(val));
}

void LED_RGB::setRgb(Rgb val) {
	Rgb& l = lastRgb;

	// cross fade
	while (val.r != l.r ||
		   val.g != l.g ||
		   val.b != l.b) {

		l.r = tick(l.r, val.r);
		l.g = tick(l.g, val.g);
		l.b = tick(l.b, val.b);

		analogWrite(r, l.r);
		analogWrite(g, l.g);
		analogWrite(b, l.b);
		
		delay(FADESPEED);
	}
}

// Fades pins in a circular list
void LED_RGB::fade() {
	// next color in sequence
	lastNode = lastNode->next;
	setRgb(lastNode->rgb);
}

// Increases non zero value by MULTI to max
uint32_t LED_RGB::up(uint32_t val) {
	uint32_t result = 0;

	// 24bit
	if (val > 0x00ff00) {
		result = up(val, 16);
	}

	// 16bit
	if (val > 0xff) {
		result |= up(val, 8);
	}

	// 8bit
	result |= up(val, 0);

	return result;
}

// Decreases value by MULTI to 0
uint32_t LED_RGB::down(uint32_t val) {
	uint32_t result = 0;

	// 24bit
	if (val > 0x00ff00) {
		result = down(val, 16);
	}

	// 16bit
	if (val > 0xff) {
		result |= down(val, 8);
	}

	// 8bit
	result |= down(val, 0);

	return result;
}

// Increases a specific byte within the 24bit int by MULTI up to max value
uint32_t LED_RGB::up(uint32_t val, uint8_t bitshift) {
	// bit shift supplied val to 8bit, mask
	uint32_t r = 0;
	
	r = (val >> bitshift & 0xff) * MULTI;
	
	// constrain to max of 256, shift back
	r = min(r, 0xff) << bitshift;
	
	return r;
}

// Decreases a specific byte within the 24bit int by MULTI down to 0
uint32_t LED_RGB::down(uint32_t val, uint8_t bitshift) {
	// bit shift supplied val to 8bit, mask
	uint32_t r = 0;
	
	r = (val >> bitshift & 0xff) / MULTI;

	// constrain to min of 0, shift back
	r = max(r, 0x0) << bitshift;

	return r;
}

// Increases brightness
void LED_RGB::up() {
	cmdExecutor("U");
}

// Decreases brightness
void LED_RGB::down() {
	cmdExecutor("D");
}

// Gets an RGB value {rr,gg,bb} from the provided long value
Rgb LED_RGB::ultoRgb(uint32_t val) {
	Rgb result = { 0, 0, 0 };

	// bit shift and mask
	result.r = val >> 16 & 0xff;
	result.g = val >> 8 & 0xff;
	result.b = val & 0xff;

	return result;
}

// Converts an rgb to equivalent unsigned long
uint32_t LED_RGB::rgbtoul(Rgb val) {
	return (((uint32_t)(val.r)) << 16) | (((uint32_t)(val.g)) << 8) | (uint32_t)(val.b);
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