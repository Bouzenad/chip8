#include "error.h"
#include "instruction.h"
#include "state.h"
#include "fetch.h"
#include "decode.h"
#include "execute.h"
#include "screen.h"
#include "input.h"
#include "parse.h"
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include <ncurses.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  init_screen();
  system_state_t state;
  int p;
  char* file_path = get_file_path(argc, argv);
  if (file_path == NULL) {
    printf("Missing or ambiguous file path");
    return -1;
  }
  if ((p = init_state(&state, file_path)) < 0) {
    printf("Error happened: %d\n", p);
    return -1;
  }
  // Setting up sound, boilerplate incoming...
  ma_engine engine;
  ma_engine_init(NULL, &engine);

  ma_waveform waveform;
  ma_waveform_config wc = ma_waveform_config_init(
        ma_format_f32,
        ma_engine_get_channels(&engine),
        ma_engine_get_sample_rate(&engine),
        ma_waveform_type_sine, 0.2, 440.0
    );
  ma_waveform_init(&wc, &waveform);

  ma_sound sound;
  ma_sound_init_from_data_source(&engine, &waveform, 0, NULL, &sound);
  while (state.registers.program_counter < MEMORY_SIZE) {
    for (int i = 0; i < 8; i++) {
      opcode_t opcode = fetch_next(&state);
      instruction_t inst = decode(opcode);
      int e;
      if ((e = execute(&state, inst)) < 0) {
        printf("%s\n", get_status_message(e));
        printf("Program counter: %x\n", state.registers.program_counter);
        printf("Last opcode executed: %x\n", opcode);
        return e;
      }
    }
    if (state.registers.delay_register > 0) {
      state.registers.delay_register--;
    }
    if (state.registers.sound_register > 0) {
      ma_sound_start(&sound);
      state.registers.sound_register--;
    } else {
      ma_sound_stop(&sound);
    }
    draw_screen(state);
    monitor_input(&state);
    usleep(16667);
  }
  printf("Reached end of memory");
	return 0;
}
