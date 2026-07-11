#include "parse.h"
#include "state.h"
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

char* get_file_path(int argc, char* argv[]) {
    char* result = NULL;
    for (int i = 1; i < argc; i++) {
        size_t len = strlen(argv[i]);
        if (argv[i][0] != '-') {
            if (result != NULL) {
                return NULL; // not unique
            }
            result = argv[i];
        }
    }
    return result;
}

static int set_sound_freq(system_state_t *state, char* argv) {
    char* equals_pos = strchr(argv, '=');
    char* endptr;
    if (equals_pos == NULL) { // No '=' found
        return -1;
    }
    // There should always be '\0' after '='
    uint16_t freq = strtol(equals_pos + 1, &endptr, 10);
    if (*endptr != '\0' || endptr == equals_pos + 1) {
        return -1;
    }
    state->configs.sound_freq = freq;
    return freq;
}

int set_configs(system_state_t *state, int argc, char* argv[]) {
    for (int i = 0; i < argc; i++) {
        if (strlen(argv[i]) < 1 ||  argv[i][0] != '-') {
            continue;
        }
        char substring[100];
        size_t src_len = (strchr(argv[i], '=') - argv[i]) - 1;
        size_t len = src_len >= sizeof(substring) ? sizeof(substring) - 1 : src_len;
        memcpy(substring, argv[i] + 1, len);
        substring[len] = '\0';
        // Have to use if else if chain instead of switch
        if (strcmp(substring, "sound-freq") == 0) {
            if (set_sound_freq(state, argv[i]) < 0) {
                return -1;
            }
        } else {
            return -1;
        } 
    }

    return 0;
}
