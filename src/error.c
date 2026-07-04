#include "error.h"

char* get_status_message(error_code code) {
	switch (code) {
			case STATUS_OK: return "Status OK";
			case GENERAL_ERROR: return "Error: Error";
			case STACK_UNDERFLOW: return "Error: Stack underflow";
			case STACK_OVERFLOW: return "Error: Stack overflow";
			case MEMORY_OUT_OF_BOUNDS: return "Error: Memory out of bounds";
			case UNKNOWN_INSTRUCTION: return "Error: Unknown instruction";
			case UNIMPLEMENTED_INSTRUCTION: return "Error Unimplemented instruction";
			default: return "Unknown status code";
			}
}
