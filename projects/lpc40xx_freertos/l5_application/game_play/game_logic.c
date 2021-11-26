#include "game_logic.h"
#include "FreeRTOS.h"
#include "background_screen.h"
#include "jumper.h"
#include "led_matrix_driver.h"
#include "task.h"
#define GAME_PLAY_TASK "GAME_PLAY"
#define MAX_JUMP 7

static int score = 0;

void game_play(void *params) {
  int going_up = 1;
  int level = 0;
  int jumper_row, jumper_col;
  bool collision_detected = 0;
  int jump_count = 0;
  // Initialize Background Screen
  initialize_background_screen();
  (void)find_initial_jumper_position(&jumper_row, &jumper_col);
  draw_jumper(jumper_row, jumper_col);
  while (1) {
    vTaskDelay(100);
    switch (level) {
    case 0:
      update_background_screen(collision_detected);
      if (going_up) {
        clear_jumper(jumper_row, jumper_col);
        jumper_row -= 2;
        draw_jumper(jumper_row, jumper_col);
        jump_count++;
        fprintf(stderr, "jump_count going up %d MAX_JUMP = %d, going up = %d\n", jump_count, MAX_JUMP, going_up);
        if (jump_count == MAX_JUMP) {
          going_up = 0;
          fprintf(stderr, "going_up = %d\n", going_up);
        }

      } else {
        clear_jumper(jumper_row, jumper_col);
        jumper_row += 2;
        draw_jumper(jumper_row, jumper_col);
        if (detect_collision_background_screen(jumper_row, jumper_col)) {
          shift_background_screen_down(jumper_row);
          clear_jumper(jumper_row, jumper_col);
          jumper_row = get_last_background_screen_row();
          draw_jumper(jumper_row, jumper_col);
          going_up = 1;
          score++;
          jump_count = 0;
          vTaskDelay(100);
          continue;
        }
        jump_count--;
        fprintf(stderr, "jump_count going down  %d\n", jump_count);
        if (jump_count == 0) {
          going_up = 1;
        }
      }
      continue;
    case 1:
      continue;
    default:
      break;
    }
  }
  return;
}

void create_game_play_task() {
  xTaskCreate(game_play, GAME_PLAY_TASK, (1024U * 8) / sizeof(void *), NULL, PRIORITY_LOW, NULL);
}
