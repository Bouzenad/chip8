#include "decode.h"
#include "instruction.h"

instruction_t decode(opcode_t opcode) {
  instruction_t result = (instruction_t){0};
  // kind is technically signed, but it only goes from 0 to F, so it shouldn't
  // cause any issues
  result.kind = (opcode & 0xF000) >> 12;
  result.x = (opcode & 0x0F00) >> 8;
  result.y = (opcode & 0x00F0) >> 4;
  result.nnn = (opcode & 0x0FFF);
  result.kk = (opcode & 0x00FF);
  result.n = (opcode & 0x000F);

  return result;
}
