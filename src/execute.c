#include "execute.h"
#include "error.h"
#include "input.h"
#include "instruction.h"
#include "state.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
  case 0xe:
    return ret(state, inst);
  default:
    return UNKNOWN_INSTRUCTION;
  }
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

static int sub(system_state_t *state, instruction_t inst) {
  // using temporary variables to handle the quirk of one of the operands being
  // the flag register
  uint8_t vx = state->registers.v_register[inst.x];
  uint8_t vy = state->registers.v_register[inst.y];
  state->registers.v_register[inst.x] = vx - vy;
  state->registers.v_register[0xf] = vx > vy ? 1 : 0;
  return STATUS_OK;
}

static int shr(system_state_t *state, instruction_t inst) {
  uint8_t vx = state->registers.v_register[inst.x];
  state->registers.v_register[inst.x] >>= 1;
  state->registers.v_register[0xf] = vx & 1 ? 1 : 0;
  return STATUS_OK;
}

static int subn(system_state_t *state, instruction_t inst) {
  // using temporary variables to handle the quirk of one of the operands being
  // the flag register
  uint8_t vx = state->registers.v_register[inst.x];
  uint8_t vy = state->registers.v_register[inst.y];
  state->registers.v_register[inst.x] = vy - vx;
  state->registers.v_register[0xf] = vy > vx ? 1 : 0;
  return STATUS_OK;
}

static int shl(system_state_t *state, instruction_t inst) {
  uint8_t vx = state->registers.v_register[inst.x];
  state->registers.v_register[inst.x] <<= 1;
  state->registers.v_register[0xf] = vx & 0b1000000 ? 1 : 0;
  return STATUS_OK;
}

static int alu_op(system_state_t *state, instruction_t inst) {
  switch (inst.n) {
  case 0x0:
    return ld_rr(state, inst);
  case 0x1:
    return or(state, inst);
  case 0x2:
    return and(state, inst);
  case 0x3:
    return xor(state, inst);
  case 0x4:
    return add_rr(state, inst);
  case 0x5:
    return sub(state, inst);
  case 0x6:
    return shr(state, inst);
  case 0x7:
    return subn(state, inst);
  case 0xe:
    return shl(state, inst);
  default:
    return UNKNOWN_INSTRUCTION;
  }
}

static int sne_rr(system_state_t *state, instruction_t inst) {
  if (state->registers.v_register[inst.x] !=
      state->registers.v_register[inst.y]) {
    state->registers.program_counter += 2;
  }
  return STATUS_OK;
}

static int ld_addr(system_state_t *state, instruction_t inst) {
  state->registers.address_register = inst.nnn;
  return STATUS_OK;
}

static int jp_v0(system_state_t *state, instruction_t inst) {
  state->registers.program_counter = state->registers.v_register[0] + inst.nnn;
  if (state->registers.program_counter >= MEMORY_SIZE) {
    return MEMORY_OUT_OF_BOUNDS;
  }
  return STATUS_OK;
}

static int rnd(system_state_t *state, instruction_t inst) {
  state->registers.v_register[inst.x] = rand() & inst.kk;
  return STATUS_OK;
}

static int drw(system_state_t *state, instruction_t inst) {
  uint16_t addr = state->registers.address_register;
  uint8_t x = state->registers.v_register[inst.x] % DISPLAY_WIDTH;
  uint8_t y = state->registers.v_register[inst.y] % DISPLAY_HEIGHT;
  if (addr + inst.n - 1 >= MEMORY_SIZE) {
    return MEMORY_OUT_OF_BOUNDS;
  }
  state->registers.v_register[0xf] = 0;
  for (int i = 0; i < inst.n; i++) {
    if (y + i >= DISPLAY_HEIGHT) {
      continue;
    }
    for (int j = 0; j < 8; j++) {
      if (x + j >= DISPLAY_WIDTH) {
        continue;
      }
      uint8_t bit = state->display[y+i][x+j];
      uint8_t mask = (state->memory[addr + i] >> (8 - j - 1)) & 1;
      if (bit && mask) {
        state->registers.v_register[0xf] = 1;
      }
      state->display[y+i][x+j] = bit ^ mask; 
    }
  }
  return STATUS_OK;
}

// Might have to error check for memory out of bounds
static int skp(system_state_t *state, instruction_t inst) {
  uint8_t vx = state->registers.v_register[inst.x];
  if (vx <= 0xf && state->input[vx]) {
    state->registers.program_counter += 2;
  }
  return STATUS_OK;
}

static int sknp(system_state_t *state, instruction_t inst) {
  uint8_t vx = state->registers.v_register[inst.x];
  if (vx <= 0xf && state->input[vx]) {
    return STATUS_OK; 
  }
  state->registers.program_counter += 2;
  return STATUS_OK;
}

static int skp_sknp(system_state_t *state, instruction_t inst) {
  switch (inst.n) {
    case 0xe: 
      return skp(state, inst);
    case 0x1:
      return sknp(state, inst);
    default:
      return UNKNOWN_INSTRUCTION;
  }
}

static int ld_vx_dt(system_state_t *state, instruction_t inst) {
  state->registers.v_register[inst.x] = state->registers.delay_register;
  return STATUS_OK;
}

static int ld_vx_k(system_state_t *state, instruction_t inst) {
  for (int i = 0; i < TOTAL_KEY_AMOUNT; i++) {
    if (state->input[i] == POS_EDGE) {
      state->registers.v_register[inst.x] = i;
      return STATUS_OK;
    }
  }
  state->registers.program_counter -= 2;
  return STATUS_OK;
}

static int ld_dt_vx(system_state_t *state, instruction_t inst) {
  state->registers.delay_register = state->registers.v_register[inst.x];
  return STATUS_OK;
}

static int ld_st_vx(system_state_t *state, instruction_t inst) {
  state->registers.sound_register = state->registers.v_register[inst.x];
  return STATUS_OK;
}

static int add_i(system_state_t *state, instruction_t inst) {
  state->registers.address_register += state->registers.v_register[inst.x];
  return STATUS_OK;
}

static int store_in_memory(system_state_t *state, instruction_t inst) {
  if (state->registers.address_register + inst.x >= MEMORY_SIZE) {
    return MEMORY_OUT_OF_BOUNDS;
  }
  memcpy(state->memory + state->registers.address_register, state->registers.v_register, inst.x + 1);
  return STATUS_OK;
}

static int read_from_memory(system_state_t *state, instruction_t inst) {
  if (state->registers.address_register + inst.x >= MEMORY_SIZE) {
    return MEMORY_OUT_OF_BOUNDS;
  }
  memcpy(state->registers.v_register, state->memory + state->registers.address_register, inst.x + 1);
  return STATUS_OK;
}

static int misc(system_state_t *state, instruction_t inst) {
  switch (inst.kk) {
    default:
      return UNKNOWN_INSTRUCTION;
  }
}

int execute(system_state_t *state, instruction_t inst) {
  switch (inst.kind) {
  case SYS_CLS_RET:
    return sys_cls_ret(state, inst);
  case JP:
    return jp(state, inst);
  case CALL:
    return call(state, inst);
  case SE_RI:
    return se_ri(state, inst);
  case SNE_RI:
    return sne_ri(state, inst);
  case SE_RR:
    return se_rr(state, inst);
  case LD_RI:
    return ld_ri(state, inst);
  case ADD_RI:
    return add_ri(state, inst);
  case ALU_OP:
    return alu_op(state, inst);
  case SNE_RR:
    return sne_rr(state, inst);
  case LD_ADDR:
    return ld_addr(state, inst);
  case JP_V0:
    return jp_v0(state, inst);
  case RND:
    return rnd(state, inst);
  case DRW:
    return drw(state, inst);
  case SKP_SKNP:
    return skp_sknp(state, inst);
  case MISC:
    return placeholder(state, inst);
  default:
    return UNKNOWN_INSTRUCTION;
  }
}
