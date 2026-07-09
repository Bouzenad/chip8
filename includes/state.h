#ifndef STATE_H
#define STATE_H

#include <stdint.h>
// Size of the address space (4KB)
#define MEMORY_SIZE 4096
// Positions of the first memory address, memory address of the first
// instruction, and final memory address
#define MEMORY_START 0x000
#define MEMORY_PROGRAM_START 0x200
#define MEMORY_END 0xFFF
// Number of general purpose registers
#define GENERAL_REGS_NUMBER 16
#define TOTAL_KEY_AMOUNT 16

// Maximum possible number of addresses that can be pushed into the stack
#define MAX_STACK_DEPTH 16

// Dimensions of CHIP-8's monochrome display
#define DISPLAY_HEIGHT 32
#define DISPLAY_WIDTH 64

// Address where digit sprites are located
#define DIGIT_LOCATION 0x50
#define BYTE_PER_DIGIT 5
#define NUMBER_OF_DIGITS 16

// Struct for holding the different kinds of registers
// - Program counter holds the index of the next instruction to be executed
// - Address register is a 16-bit register for holding an arbitrary address
// (addresses in CHIP-8 are 12-bits)
// - 16 general purpose 8-bit registers
// - Stack pointer for holding the current return address on the stack
// - Delay register (active when non-zero, decrements by 1 at a 60Hz rate when
// active)
// - Sound register (active when non-zero, decrements by 1 at a 60 Hz rate and
// creates a beeping sound when active)
typedef struct registers_t {
  uint16_t program_counter;
  uint16_t address_register;
  uint8_t v_register[GENERAL_REGS_NUMBER];
  uint8_t stack_pointer;
  uint8_t delay_register;
  uint8_t sound_register;
} registers_t;

// Struct for storing the system's general state, includes registers, memory,
// stack, and display
typedef struct system_state_t {
  registers_t registers;
  uint8_t memory[MEMORY_SIZE];
  uint8_t display[DISPLAY_HEIGHT][DISPLAY_WIDTH];
  uint16_t stack[MAX_STACK_DEPTH];
  uint8_t input[TOTAL_KEY_AMOUNT];
} system_state_t;

// Takes in a pointer for a system_state and a ch8 program,
// and initialilzes it (all data zeroed out, program data loaded
// in memory, and font data loaded in (TO BE DONE)
int init_state(system_state_t *state, char *file_name);

#endif
