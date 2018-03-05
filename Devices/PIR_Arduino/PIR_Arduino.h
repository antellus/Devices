#pragma once

enum PinDef : uint8_t {
	P_Pir = 0x2,
	P_Led = 0x7,

	// atwinc pins
	P_Rst = 0x8, // reset
	P_Irq = 0x9, // interrupts
	P_Cs  = 0xa   // chip select
};