#pragma once

enum PinDef : uint8_t {
	// led pins
	P_G = 0x3,
	P_R = 0x5, 
	P_B = 0x6,

	// atwinc pins
	P_Rst = 0x8, // reset
	P_Irq = 0x9, // interrupts
	P_Cs  = 0xa   // chip select
};