#include "enemy.h"
#include "FreeRTOS.h"
#include "background_screen.h"
#include "game_logic.h"
#include "joystick.h"
#include "jumper.h"
#include "led_matrix_draw_alphabets.h"
#include "led_matrix_draw_objects.h"
#include "led_matrix_driver.h"
#include "task.h"
#include <stdbool.h>

#define ENEMY_TASK "ENEMY_TASK"
#define MAX_ENEMIES 3

static uint8_t enemies_row[MAX_ENEMIES];
static uint8_t enemies_col[MAX_ENEMIES];

extern bool game_over;

bool check_collision_with_enemy(int jumper_row, int jumper_col) {
  // Check collision with each enemy
  int row_diff, col_diff;
  for (int i = 0; i < MAX_ENEMIES; i++) {
    row_diff = jumper_row - enemies_row[i];
    col_diff = jumper_col - enemies_col[i];
    //    fprintf(stderr,
    //          "Jumper row %d, jumper col = %d, row diff = %d col diff = %d, eneemy row = %d enenmy col=%d, i = %d\n",
    //        jumper_row, jumper_col, row_diff, col_diff, enemies_row[i], enemies_col[i], i);
    if (((row_diff > -(JUMPER_LENGTH - 1)) && (row_diff < (JUMPER_LENGTH - 1))) &&
        ((col_diff > -(JUMPER_WIDTH - 1)) && ((col_diff < JUMPER_WIDTH - 1)))) {
      return true;
    }
  }
  return false;
}

void find_initial_enemy_positions() {
  for (int i = 0; i < MAX_ENEMIES; i++) {
    enemies_row[i] = rand() % 14 + (i / 2 + 1) * 8;
    if (i % 2 == 0) {
      enemies_col[i] = 1;
    } else {
      enemies_col[i] = 58;
    }
  }
}

void find_new_enemy_position(int enemy_num) {
  enemies_row[enemy_num] = rand() % 16 + (enemy_num / 2 + 1) * 16;
  if (enemy_num % 2 == 0) {
    enemies_col[enemy_num] = 1;
  } else {
    enemies_col[enemy_num] = 58;
  }
}

void draw_enemies() {
  for (int i = 0; i < MAX_ENEMIES; i++) {
    if ((enemies_row[i] != -1) || enemies_col[i] != -1) {
      clear_enemy(enemies_row[i], enemies_col[i]);
    }
    if (enemies_col[i] == 59 || enemies_col[i] == 0) {
      find_new_enemy_position(i);
    } else {
      if (i % 2 == 0) {
        enemies_col[i] += 1;
      } else {
        enemies_col[i] -= 1;
      }
    }
    if ((enemies_row[i] != -1) || enemies_col[i] != -1) {
      draw_enemy(enemies_row[i], enemies_col[i]);
    }
  }
}

void enemy_task(void *params) {
  find_initial_enemy_positions();
  while (1) {
    if (!game_over) {
      draw_enemies();
      vTaskDelay(70);
    } else {
      vTaskSuspend(NULL);
    }
  }
}

void create_enemy_task() {
  xTaskCreate(enemy_task, ENEMY_TASK, (1024U * 8) / sizeof(void *), NULL, PRIORITY_HIGH, NULL);
}
