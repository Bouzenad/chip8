#include "execute.h"
#include "error.h"
#include "instruction.h"
#include "state.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

static int placeholder(system_state_t *state, instruction_t inst) {
  printf("%x\n", inst.kind);
  return UNIMPLEMENTED_INSTRUCTION;
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
  state->registers.program_counter =
      state->stack[state->registers.stack_pointer];
  return STATUS_OK;
}

static int sys_cls_ret(system_state_t *state, instruction_t inst) {
  if (inst.nnn != 0x0e0 && inst.nnn != 0x0ee) {
    return sys(state, inst);
  }
  switch (inst.n) {
  case 0x0:
    return cls(state, inst);
    break;
  case 0xe:
    return ret(state, inst);
  }
  return UNKNOWN_INSTRUCTION;
}

static int jp(system_state_t *state, instruction_t inst) {
  state->registers.program_counter = inst.nnn;
  return STATUS_OK;
}

static int call(system_state_t *state, instruction_t inst) {
  if (state->registers.stack_pointer >= 15) {
    return STACK_OVERFLOW;
  }
  state->stack[state->registers.stack_pointer] =
      state->registers.program_counter;
  state->registers.program_counter = inst.nnn;
  state->registers.stack_pointer++;
  return STATUS_OK;
}

static int se_ri(system_state_t *state, instruction_t inst) {
  if (state->registers.v_register[inst.x] == inst.kk) {
    state->registers.program_counter += 2;
  }
  return STATUS_OK;
}

static int sne_ri(system_state_t *state, instruction_t inst) {
  if (state->registers.v_register[inst.x] != inst.kk) {
    state->registers.program_counter += 2;
  }
  return STATUS_OK;
}

static int se_rr(system_state_t *state, instruction_t inst) {
  if (state->registers.v_register[inst.x] ==
      state->registers.v_register[inst.y]) {
    state->registers.program_counter += 2;
  }
  return STATUS_OK;
}

static int ld_ri(system_state_t *state, instruction_t inst) {
  state->registers.v_register[inst.x] = inst.kk;
  return STATUS_OK;
}

static int add_ri(system_state_t *state, instruction_t inst) {
  state->registers.v_register[inst.x] =
      state->registers.v_register[inst.x] + inst.kk;
  return STATUS_OK;
}

static int ld_rr(system_state_t *state, instruction_t inst) {
  state->registers.v_register[inst.x] = state->registers.v_register[inst.y];
  return STATUS_OK;
}

static int or(system_state_t *state, instruction_t inst) {
  state->registers.v_register[inst.x] |= state->registers.v_register[inst.y];
  return STATUS_OK;
}

static int and(system_state_t *state, instruction_t inst) {
  state->registers.v_register[inst.x] &= state->registers.v_register[inst.y];
  return STATUS_OK;
}

static int xor(system_state_t *state, instruction_t inst) {
  state->registers.v_register[inst.x] ^= state->registers.v_register[inst.y];
  return STATUS_OK;
}

static int add_rr(system_state_t *state, instruction_t inst) {
  // using temporary variables to handle the quirk of one of the operands being
  // the flag register
  uint8_t vx = state->registers.v_register[inst.x];
  uint8_t vy = state->registers.v_register[inst.y];
  state->registers.v_register[inst.x] = vx + vy;
  // casting to 32-bits should be implicit, but I'll make it more clear
  state->registers.v_register[0xf] = (uint32_t)vx + (uint32_t)vy > 255 ? 1 : 0;
  return STATUS_OK;
}

static int alu_op(system_state_t *state, instruction_t inst) {
  switch (inst.n) {
  case 0x0:
    return ld_rr(state, inst);
    break;
  case 0x1:
    return or(state, inst);
    break;
  case 0x2:
    return and(state, inst);
    break;
  case 0x3:
    return xor(state, inst);
    break;
  case 0x4:
    return add_rr(state, inst);
    break;
  case 0x5:
    return placeholder(state, inst);
    break;
  case 0x6:
    return placeholder(state, inst);
    break;
  case 0x7:
    return placeholder(state, inst);
    break;
  case 0xe:
    return placeholder(state, inst);
    break;
  default:
    return UNKNOWN_INSTRUCTION;
  }
}

int execute(system_state_t *state, instruction_t inst) {
  switch (inst.kind) {
  case SYS_CLS_RET:
    return sys_cls_ret(state, inst);
    break;
  case JP:
    return jp(state, inst);
    break;
  case CALL:
    return call(state, inst);
    break;
  case SE_RI:
    return se_ri(state, inst);
    break;
  case SNE_RI:
    return sne_ri(state, inst);
    break;
  case SE_RR:
    return se_rr(state, inst);
    break;
  case LD_RI:
    return ld_ri(state, inst);
    break;
  case ADD_RI:
    return add_ri(state, inst);
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
