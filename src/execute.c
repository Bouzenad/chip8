#include "execute.h"
#include "state.h"
#include "instruction.h"
#include "error.h"

#include <stdio.h>
#include <string.h>

static int placeholder(system_state_t *state, instruction_t inst) {
	printf("%x\n", inst.kind);
	return UNIMPLEMENTED_INSTRUCTION;
}

static int sys_cls_ret(system_state_t *state, instruction_t inst) {
	if (inst.nnn != 0x0e0 && inst.nnn != 0x0ee) {
		return placeholder(state, inst);
	}
	switch (inst.n) {
		case 0x0:
			return placeholder(state, inst);
			break;
		case 0xe:
			return placeholder(state, inst);
	}
	return UNKNOWN_INSTRUCTION;
}

static int sys(system_state_t *state, instruction_t inst) {
	state->registers.address_register = inst.nnn;
	return STATUS_OK;
}

static int cls(system_state_t *state, instruction_t inst) {
	memset(state->display, 0, DISPLAY_HEIGHT * DISPLAY_WIDTH);
	return STATUS_OK;
}

static int ret(system_state_t *state, instruction_t inst) {
	if (state->registers.stack_pointer <= 0) {
		return STACK_UNDERFLOW;
	}
	state->registers.stack_pointer--;
	state->registers.program_counter = state->stack[state->registers.stack_pointer];
	return STATUS_OK;
}

int execute(system_state_t *state, instruction_t inst) {
	switch (inst.kind) {
		case SYS_CLS_RET:
			return placeholder(state, inst);
			break;
		case JP:
			return placeholder(state, inst);
			break;
		case CALL:
			return placeholder(state, inst);
			break;
		case SE_RI:
			return placeholder(state, inst);
			break;
		case SNE_RI:
			return placeholder(state, inst);
			break;
		case SE_RR:
			return placeholder(state, inst);
			break;
		case LD_RI:
			return placeholder(state, inst);
			break;
		case ADD_RI:
			return placeholder(state, inst);
			break;
		case ALU_OP:
			return placeholder(state, inst);
			break;
		case SNE_RR:
			return placeholder(state, inst);
			break;
		case LD_ADDR:
			return placeholder(state, inst);
			break;
		case JP_V0:
			return placeholder(state, inst);
			break;
		case RND:
			return placeholder(state, inst);
			break;
		case DRW:
			return placeholder(state, inst);
			break;
		case SKP_SKNP:
			return placeholder(state, inst);
			break;
		case MISC:
			return placeholder(state, inst);
			break;
		default:
			return UNKNOWN_INSTRUCTION;
	}
}
