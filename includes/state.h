#include <stdint.h>

#ifndef STATE_H
#define STATE_H

// Size of the address space (4KB)
#define MEMORY_SIZE 4096

// Number of general purpose registers
#define GENERAL_REGS_NUMBER 16

// Maximum possible number of addresses that can be pushed into the stack
#define MAX_STACK_DEPTH 16

// Dimensions of CHIP-8's monochrome display
#define DISPLAY_HEIGHT 32
#define DISPLAY_WIDTH 64

// Struct for holding the different kinds of registers
// - Program counter holds the index of the next instruction to be executed
// - Address register is a 16-bit register for holding an arbitrary address (addresses in CHIP-8 are 12-bits)
// - 16 general purpose 8-bit registers
// - Stack pointer for holding the current return address on the stack
// - Delay register (active when non-zero, decrements by 1 at a 60Hz rate when active)
// - Sound register (active when non-zero, decrements by 1 at a 60 Hz rate and creates a beeping sound when active)
typedef struct registers{
	uint16_t program_counter;
	uint16_t address_register;
	uint8_t v_register[GENERAL_REGS_NUMBER];
	uint8_t stack_pointer;
	uint8_t delay_register;
	uint8_t sound_register;
} registers;

// Struct for storing the system's general state, includes registers, memory, stack, and display
typedef struct system_state {
	registers registers;
	uint8_t memory[MEMORY_SIZE];
	uint8_t display[DISPLAY_HEIGHT][DISPLAY_WIDTH];
	uint16_t stack[MAX_STACK_DEPTH];
} system_state;

// Takes in a pointer for a system_state and returns an initialized state (all data zeroed out, except memory, which will be filled with font data (TO BE DONE))
void init_state(system_state *state);

#endif
