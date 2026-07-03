#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <stdbool.h>
#include <stdio.h>

#define TEST(name) bool test_##name(void)
#define RUN_TEST(name)                                                         \
  do {                                                                         \
    if (test_##name()) {                                                       \
      printf("Test \"" #name "\" has passed\n");                               \
    } else {                                                                   \
      printf("Test \"" #name "\" has failed\n");                               \
    }                                                                          \
  } while (0)


#endif
