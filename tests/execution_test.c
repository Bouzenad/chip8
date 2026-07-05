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

TEST(jp_works_correctly) {
	system_state_t state; // 0x1300
	init_state(&state, "tests/test_resources/Animal Race [Brian Astle].ch8");
	instruction_t inst = {1, 0x300, 0x00, 0x3, 0x0, 0x0};
	execute(&state, inst);
	return state.registers.program_counter == 0x300;
}

TEST(call_works_correctly) {
	system_state_t state;
	init_state(&state, "tests/test_resources/Animal Race [Brian Astle].ch8");
	instruction_t inst = {2, 0x300, 0x00, 0x3, 0x0, 0x0};
	execute(&state, inst);
	return state.registers.program_counter == 0x300 && state.registers.stack_pointer == 1 && state.stack[0] == 0x200;
}

TEST(call_overflows_correctly) {
	system_state_t state;
	init_state(&state, "tests/test_resources/Animal Race [Brian Astle].ch8");
	state.registers.stack_pointer = 16;
	instruction_t inst = {2, 0x300, 0x00, 0x3, 0x0, 0x0};	
	return execute(&state, inst) == STACK_OVERFLOW;
}

TEST(add_rr_works_correctly) {
	system_state_t state;
	init_state(&state, "tests/test_resources/Animal Race [Brian Astle].ch8");
	state.registers.v_register[0] = 10;
	state.registers.v_register[1] = 10;
	instruction_t inst = {8, 0x014, 0x14, 0x0, 0x1, 0x4}; //0x8014
	execute(&state, inst);
	return state.registers.v_register[0] == 20 && state.registers.v_register[1] == 10 && state.registers.v_register[0xf] == 0; 
}

TEST(add_rr_carries_correctly) {
	system_state_t state;
	init_state(&state, "tests/test_resources/Animal Race [Brian Astle].ch8");
	state.registers.v_register[0] = 250;
	state.registers.v_register[1] = 10;
	instruction_t inst = {8, 0x014, 0x14, 0x0, 0x1, 0x4}; //0x8014
	execute(&state, inst);
	return state.registers.v_register[0] == 4 && state.registers.v_register[1] == 10 && state.registers.v_register[0xf] == 1; 
}

TEST(add_rr_flag_used_as_operand) {
	system_state_t state;
	init_state(&state, "tests/test_resources/Animal Race [Brian Astle].ch8");
	state.registers.v_register[0xf] = 250;
	state.registers.v_register[1] = 10;
	instruction_t inst = {8, 0xf14, 0x14, 0xf, 0x1, 0x4}; //0x8f14
	execute(&state, inst);
	return state.registers.v_register[1] == 10 && state.registers.v_register[0xf] == 1; 

}

int main() {
	RUN_TEST(ret_works_correctly);
	RUN_TEST(ret_underflows_correctly);
	RUN_TEST(cls_works_correctly);
	RUN_TEST(jp_works_correctly);
	RUN_TEST(call_works_correctly);
	RUN_TEST(call_overflows_correctly);
	RUN_TEST(add_rr_works_correctly);
	RUN_TEST(add_rr_carries_correctly);
	RUN_TEST(add_rr_flag_used_as_operand);
	return 0;
}
