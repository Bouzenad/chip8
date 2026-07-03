#include "state.h"
#include <stdint.h>
#include <stdio.h>

int init_state(system_state_t *state, char *file_name) {
  *state = (system_state_t){0};
  state->registers.program_counter = MEMORY_PROGRAM_START;
  FILE *program = fopen(file_name, "rb");
  if (program == NULL) {
    return -1;
  }
  fread(state->memory + MEMORY_PROGRAM_START, sizeof(uint8_t),
        MEMORY_END - MEMORY_PROGRAM_START, program);
  return 0;
}
