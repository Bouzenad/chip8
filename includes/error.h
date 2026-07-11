#ifndef ERROR_H
#define ERROR_H

// Differnt kinds of error/status codes that can happen during execution
typedef enum error_code {
  STATUS_OK = 0,
  GENERAL_ERROR = -1,
  STACK_UNDERFLOW = -2,
  STACK_OVERFLOW = -3,
  MEMORY_OUT_OF_BOUNDS = -4,
  UNKNOWN_INSTRUCTION = -5,
  UNIMPLEMENTED_INSTRUCTION = -6,
} error_code;

// Gives a human-readable message based on status
char *get_status_message(error_code code);

#endif
