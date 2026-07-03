#include "fetch.h"
#include "instruction.h"
#include <stdint.h>

opcode_t fetch_next(system_state_t *state) {
  uint16_t PC = state->registers.program_counter;
  opcode_t result = (state->memory[PC] << 8) | (state->memory[PC + 1]);
  state->registers.program_counter += 2;

  return result;
}
