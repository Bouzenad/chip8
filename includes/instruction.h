#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stdint.h>

// Type for storing a 16-bit opcode, using this instead of uint16_t for clarity's sake
typedef uint16_t opcode_t;

// Enum that lists the different kinds of instructions in CHIP-8. Check Cowgod's chip8 technical reference for more details
typedef enum instruction_kind_t {
  SYS_CLS_RET,
  JP,
  CALL,
  SE_RI,
  SNE_RI,
  SE_RR,
  LD_RI,
  ADD_RI,
  ALU_OP,
  SNE_RR,
  LD_ADDR,
  JP_V0,
  RND,
  DRW,
  SKP_SKNP,
  MISC
} instruction_kind_t;

// Type for storing an instruction. It includes what kind of instruction it is, along with the parameters. Formats include:
// - 0nnn
// - 0xkk
// - 0x00
// - 0xy0
// - 0xyn
// The 0s here are just placeholders, they can be any valid number.
typedef struct instruction_t {
  instruction_kind_t kind;
  uint16_t nnn;
  uint8_t kk;
  uint8_t x;
  uint8_t y;
  uint8_t n;
} instruction_t;
#endif
