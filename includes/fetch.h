#ifndef FETCH_H
#define FETCH_H

#include "instruction.h"
#include "state.h"

// Takes in the current state, returns the opcode that the PC is currently pointing at and then increments it by 2
opcode_t fetch_next(system_state_t *state);

#endif
