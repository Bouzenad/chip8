#include "fetch.h"
#include "test_utils.h"
#include "state.h"

TEST(pc_always_even) {
	system_state_t state;
	init_state(&state, "tests/test_resources/Animal Race [Brian Astle].ch8");
	while (state.registers.program_counter < MEMORY_SIZE - 2) {
		fetch_next(&state);
		if (state.registers.program_counter % 2 != 0) {
			return false;
		}
	}
	return true;
}

TEST(fetch_correct_opcode) {
	system_state_t state;
	init_state(&state, "tests/test_resources/Animal Race [Brian Astle].ch8");
	return fetch_next(&state) == 0x6d0a && fetch_next(&state) == 0x6e02;
}

int main() {
	RUN_TEST(pc_always_even);
	RUN_TEST(fetch_correct_opcode);
	return 0;
}
