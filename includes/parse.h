#ifndef PARSE_H
#define PARSE_H

// Takes in arguments, and returns the filepath to a CHIP-8 ROM.
// Anything that doesn't start with - counts as a candidate
// Multiple paths added as arguments return NULL, due to ambiguity
char* get_file_path(int argc, char *argv[]);

#endif
