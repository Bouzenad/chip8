#include "input.h"
#include "state.h"
#include <string.h>
#include <sys/select.h>
#include <unistd.h>
#include <ncurses.h>

static int map_to_key(int input) {
  switch (input) {
  case '1':
    return 0x1;
    break;
  case '2':
    return 0x2;
    break;
  case '3':
    return 0x3;
    break;
  case '4':
    return 0xc;
    break;
  case 'q':
    return 0x4;
    break;
  case 'w':
    return 0x5;
    break;
  case 'e':
    return 0x6;
    break;
  case 'r':
    return 0xd;
    break;
  case 'a':
    return 0x7;
    break;
  case 's':
    return 0x8;
    break;
  case 'd':
    return 0x9;
    break;
  case 'f':
    return 0xe;
    break;
  case 'z':
    return 0xa;
    break;
  case 'x':
    return 0x0;
    break;
  case 'c':
    return 0xb;
    break;
  case 'v':
    return 0xf;
    break;
  default:
    return -1;
  }
}

void monitor_input(system_state_t *state) {
  int c;
  char input_char;
  int clicked = 0;
  // Don't you hate it when buffering is implemented, with no easy way of flushing?
  while ((c = getch()) != ERR) {
    if (map_to_key(c) >= 0) {
      clicked = 1;
      input_char = c;
    }
  }
  memset(state->input, 0, TOTAL_KEY_AMOUNT);
  if (clicked) {
    state->input[map_to_key(input_char)] = 1;
  }
}
