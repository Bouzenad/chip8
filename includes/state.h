#include <stdint.h>

#ifndef STATE_H
#define STATE_H

#define MEMORY_SIZE 4096

#define GENERAL_REGS_NUMBER 16

#define DISPLAY_HEIGHT 32
#define DISPLAY_WIDTH 64

typedef struct registers{
	uint16_t program_counter;
	uint16_t address_register;
	uint8_t v_register[GENERAL_REGS_NUMBER];
	uint8_t stack_pointer;
	uint8_t delay_register;
	uint8_t sound_register;
} registers;

typedef struct memory {
	uint8_t bytes[MEMORY_SIZE];
} memory;

typedef struct display {
	uint8_t frame[DISPLAY_HEIGHT][DISPLAY_WIDTH];
} display;

typedef struct system_state {
	registers registers;
	memory memory;
	display display;
} system_state;

void init_state(system_state *state);

#endif
