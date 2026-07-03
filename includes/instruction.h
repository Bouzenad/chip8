#include <stdint.h>

#ifndef INSTRUCTION_H

#define INSTRUCTION_H
typedef uint16_t opcode_t;

typedef enum instruction_kind_t {
  SYS_CLS_RET,
  JP,
  CALL,
  SE_RI,
  SNE_RI,
  SE_RR,
  LD_RI,
  ADD_RI,
  ALU_OP,
  SNE_RR,
  LD_ADDR,
  JP_V0,
  RND,
  DRW,
  SKP_SKNP,
  MISC
} instruction_kind_t;

typedef struct instruction_t {
  instruction_kind_t kind;
  uint16_t nnn;
  uint8_t kk;
  uint8_t x;
  uint8_t y;
} instruction_t;
#endif
