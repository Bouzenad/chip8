#include "execute.h"
#include "state.h"
#include "instruction.h"

#include <stdio.h>

static void placeholder(system_state_t *state, instruction_t instruction) {
	printf("%x\n", instruction.kind);
}

void execute(system_state_t *state, instruction_t instruction) {
	switch (instruction.kind) {
		case SYS_CLS_RET:
			placeholder(state, instruction);
			break;
		case JP:
			placeholder(state, instruction);
			break;
		case CALL:
			placeholder(state, instruction);
			break;
		case SE_RI:
			placeholder(state, instruction);
			break;
		case SNE_RI:
			placeholder(state, instruction);
			break;
		case SE_RR:
			placeholder(state, instruction);
			break;
		case LD_RI:
			placeholder(state, instruction);
			break;
		case ADD_RI:
			placeholder(state, instruction);
			break;
		case ALU_OP:
			placeholder(state, instruction);
			break;
		case SNE_RR:
			placeholder(state, instruction);
			break;
		case LD_ADDR:
			placeholder(state, instruction);
			break;
		case JP_V0:
			placeholder(state, instruction);
			break;
		case RND:placeholder(state, instruction);
			break;
		case DRW:
			placeholder(state, instruction);
			break;
		case SKP_SKNP:
			placeholder(state, instruction);
			break;
		case MISC:
			placeholder(state, instruction);
			break;
	}
}
