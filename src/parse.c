#include "parse.h"
#include <string.h>
#include <stddef.h>

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
