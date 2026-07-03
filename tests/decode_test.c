#include "decode.h"
#include "test_utils.h"
#include "state.h"
#include "instruction.h"
#include "fetch.h"

TEST(decodes_correctly) {
	system_state_t state;
	init_state(&state, "tests/test_resources/Animal Race [Brian Astle].ch8");
	opcode_t opcode = fetch_next(&state);
	instruction_t instruction = decode(opcode);
	return instruction.kind == LD_RI && instruction.nnn == 0xd0a
		&& instruction.kk == 0x0a && instruction.x == 0xd
		&& instruction.y == 0x0 && instruction.n == 0xa;
}

int main() {
	RUN_TEST(decodes_correctly);
	return 0;
}
