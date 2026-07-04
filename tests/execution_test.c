#include "execute.h"
#include "instruction.h"
#include "state.h"
#include "test_utils.h"
#include "error.h"
#include "string.h"
#include <stdbool.h>

TEST(ret_works_correctly) {
	system_state_t state;
	init_state(&state, "tests/test_resources/Animal Race [Brian Astle].ch8");
	state.registers.stack_pointer = 1;
	state.stack[0] = 0x300;
	instruction_t inst = {0, 0x0ee, 0xee, 0x0, 0xe, 0xe};
	execute(&state, inst);
	return state.registers.program_counter == 0x300 && state.registers.stack_pointer == 0;
}

TEST(ret_underflows_correctly) {system_state_t state;
	init_state(&state, "tests/test_resources/Animal Race [Brian Astle].ch8");
	state.registers.stack_pointer = 0;
	instruction_t inst = {0, 0x0ee, 0xee, 0x0, 0xe, 0xe};
	return execute(&state, inst) == STACK_UNDERFLOW; 
}

TEST(cls_works_correctly) {
	system_state_t state;
	init_state(&state, "tests/test_resources/Animal Race [Brian Astle].ch8");
	memset(state.display, 1, DISPLAY_HEIGHT * DISPLAY_WIDTH);
	instruction_t inst = {0, 0x0e0, 0xe0, 0x0, 0xe, 0x0};
	execute(&state, inst);
	for (int i = 0; i < DISPLAY_HEIGHT * DISPLAY_WIDTH; i++) {
		if (state.display[i/DISPLAY_WIDTH][i%DISPLAY_WIDTH] != 0) {
			return false;
		}
	}
	return true;
}

int main() {
	RUN_TEST(ret_works_correctly);
	RUN_TEST(ret_underflows_correctly);
	RUN_TEST(cls_works_correctly);
	return 0;
}
