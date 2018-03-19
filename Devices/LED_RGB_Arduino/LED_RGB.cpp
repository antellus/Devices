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
		case CM_hex: {
			// set the pointer after # so we can convert to ulong
			setRgb(ultoRgb(strtoul((const char*)&cmd[1], NULL, HEX)));
			UTIL_PRINTLN(cmd);
			break;
		}
		case CM_fade: {
			// next color in sequence
			lastNode = lastNode->next;
			setRgb(lastNode->rgb);
			UTIL_PRINTLN(cmd);
			break;
		}
		case CM_pulse: {
			// pulses red
			pulse(r);
			UTIL_PRINTLN(cmd);
			break;
		}
		default: {
			UTIL_PRINTLN(F("not a valid command!"));
			break;
		}
	}
}

// Executes a command by type
void LED_RGB::cmdExecutor(CmdType cmd) {
	switch (cmd) {
		// cmds that can be executed by type
	case CM_fade:
	case CM_pulse:
	case CM_up:
	case CM_dn:
	{
		char c[2] = { cmd, '\0' };
		cmdExecutor(c);
		delete[] c;
		break;
	}
	default:
	{
		UTIL_PRINTLN(F("not a valid command!"));
		break;
	}
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
		val = calc(val, (CmdType)feed[0]);
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

// Sets rgb pins from the provided val
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
	cmdExecutor(CM_fade);
}

// Pulses a color
void LED_RGB::pulse(uint8_t pin) {
	// fade to black
	setRgb({ 0,0,0 });

	// pulses 2x
	for (uint8_t i = 0; i < 2; i++) {

		// pulse
		uint8_t n = 0;
		while (n < 255) {
			analogWrite(pin, n += 5); // fast fade in
		}

		while (n > 0) {
			analogWrite(pin, n -= 5);
			delay(FADESPEED); // slower fade out
		}
	}
}

// Pulses last rgb
void LED_RGB::pulse() {
	cmdExecutor(CM_pulse);
}

// Increases brightness
void LED_RGB::up() {
	cmdExecutor(CM_up);
}

// Increases or decreases each byte within the 24bit int by MULTI
uint32_t LED_RGB::calc(uint32_t val, CmdType cmd) {
	Serial.println(val);
	if (!(cmd == CM_up || cmd == CM_dn)) {
		return val;
	}

	uint32_t result = 0;

	// calculates each byte separately
	for (uint8_t i = 0; i <= 16; i+=8) {
		uint32_t r = val >> i & 0xff;

		if (cmd == CM_up) {
			r = round(r * MULTI);
			r = min(r, 0xff); // 256 max
		}
		else { // CM_dn
			r = round(r / MULTI); // 1 min
		}

		// shift back and combine
		result |= r << i;
	}

	return result; 
}

// Decreases brightness
void LED_RGB::down() {
	cmdExecutor(CM_dn);
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
	return ((uint32_t)(val.r) << 16) | ((uint16_t)(val.g) << 8) | val.b;
}

// Creates a new node, adds it to the list
void LED_RGB::addNode(Rgb val) {
	RgbNode *node = new RgbNode{ val, NULL };

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

