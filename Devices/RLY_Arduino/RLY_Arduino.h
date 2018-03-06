#define RLY_Arudino

// pinout
enum PinDef : uint8_t {
	// Relay switch
	P_Rly = 0x7,

	// atwinc pins
	P_Rst = 0x8, // reset
	P_Irq = 0x9, // interrupts
	P_Cs = 0xa   // chip select
};