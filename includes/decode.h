#ifndef DECODE_H
#define DECODE_H

#include "instruction.h"

// Takes in an opcode, and returns a decoded instruction, ready to be consumed
// by the execution module
instruction_t decode(opcode_t opcode);

#endif
