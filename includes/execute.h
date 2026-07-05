#ifndef EXECUTE_H
#define EXECUTE_H

#include "instruction.h"
#include "state.h"

// Takes in a pointer to a state and an instruction, and executes it. Returns a
// status code based on success or failure.
int execute(system_state_t *state, instruction_t instruction);

#endif
