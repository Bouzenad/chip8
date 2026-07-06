#include "input.h"
#include "state.h"
#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>

static int map_to_key(char input) {
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

void* monitor_input(void *arg) {
  system_state_t *state = (system_state_t *) arg;
  while (1) {
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    struct timeval time = {0, 8333};
    int r = select(STDIN_FILENO + 1, &fds, NULL, NULL, &time);
    if (r == 0) { // timeout
      memset(state->input, 0, TOTAL_KEY_AMOUNT);
    } else {
      char c;
      read(STDIN_FILENO, &c, 1);
      int key = map_to_key(c);
      if (key >= 0) {
        memset(state->input, 0, TOTAL_KEY_AMOUNT);
        state->input[key] = 1;
      }
    }
  }
}
