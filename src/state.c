#include "state.h"
#include <stdint.h>

void init_state(system_state *state) {
	state->registers = (registers) {0, 0, {0}, 0, 0, 0};
	state->memory = (memory) {{0}};
	state->display = (display) {{{0}}};
}
