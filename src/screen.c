#include "screen.h"
#include "state.h"
#include <ncurses.h>

void init_screen() {
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);
	curs_set(0);
}

void draw_screen(system_state_t state) {
	clear();
	for (int i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT; i++) {
		char c = state.display[i / DISPLAY_WIDTH][i % DISPLAY_WIDTH] == 0 ? '.' : '#';
		move(i / DISPLAY_WIDTH, i % DISPLAY_WIDTH);
		printw("%c", c);
	}
	refresh();
}
