#ifndef INPUT_H
#define INPUT_H

#include "state.h"

#define POS_EDGE 6 // Value that an input takes on a positive edge

// Takes in a state, checks buttons pressed, and registers them as on.
// Only one button can be pressed at a time.
//
// Because of limitations of how the kernel hands in input, the controls
// are a bit clunky. The problem is that new inputs are sent in at a frequency
// lower than 16ms, so controls often aren't registered even when buttons are
// pressed. I had to use a decay mechanism to make it work a bit. There were
// alternatives, like directly connecting to an input file, which I didn't do
// because it would reduce portability. I could've also used a library like
// SDL2, but most of those didn't satisfy me, usually because they would've made
// usage more clunky (eg. for SDL2, I would've opened an invisible window in
// addition to the terminal, which checks for input alongside the terminal
// window. This obviously makes the code more complex and harder to reason
// about).
void monitor_input(system_state_t *state);

#endif
