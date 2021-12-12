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

static int8_t enemies_row[MAX_ENEMIES];
static int8_t enemies_col[MAX_ENEMIES];

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
bool check_gun_collision_with_enemy(int gun_row, int gun_col) {
  int row_diff, col_diff;
  for (int i = 0; i < MAX_ENEMIES; i++) {
    row_diff = gun_row - enemies_row[i];
    col_diff = gun_col - enemies_col[i];
    //    fprintf(stderr, "gun_row = %d, gun col %d, enemy row = %d enemy col = %d row diff = %d row col = %d\n",
    //    gun_row,
    //          gun_col, enemies_row[i], enemies_col[i], row_diff, col_diff);
    //    fprintf(stderr,
    //          "Jumper row %d, jumper col = %d, row diff = %d col diff = %d, eneemy row = %d enenmy col=%d, i = %d\n",
    //        jumper_row, jumper_col, row_diff, col_diff, enemies_row[i], enemies_col[i], i);
    if (((row_diff >= -(GUN_LENGTH)) && (row_diff <= (GUN_LENGTH))) &&
        ((gun_col >= enemies_col[i]) && ((gun_col <= enemies_col[i] + ENEMY_LENGTH)))) {
      clear_enemy(enemies_row[i], enemies_col[i]);
      enemies_row[i] = -1;
      enemies_col[i] = -1;
      fprintf(stderr, "killed %d\n", i);
      vTaskDelay(400);
      return true;
    }
  }
  return false;
}
void find_initial_enemy_positions() {
  for (int i = 0; i < MAX_ENEMIES; i++) {
    enemies_row[i] = rand() % 14 + (i / 2 + 1) * 10;
    if (i % 2 == 0) {
      // enemies_col[i] = rand() % 40;
      enemies_col[i] = 1;
    } else {
      //  enemies_col[i] = rand() % 40;
      enemies_col[i] = rand() % 58;
    }
  }
}

void find_new_enemy_position(int enemy_num) {
  enemies_row[enemy_num] = rand() % 16 + (enemy_num / 2 + 1) * 14;
  if (enemy_num % 2 == 0) {
    // enemies_col[enemy_num] = rand() % 40;
    enemies_col[enemy_num] = 1;
  } else {
    // enemies_col[enemy_num] = rand() % 40;
    enemies_col[enemy_num] = 58;
  }
}

void draw_enemies() {
  for (int i = 0; i < MAX_ENEMIES; i++) {
    if ((enemies_row[i] != -1) || enemies_col[i] != -1) {
      clear_enemy(enemies_row[i], enemies_col[i]);
    }

    fprintf(stderr, "Value %d enemy at %d %d\n", i, enemies_row[i], enemies_col[i]);
    if (enemies_col[i] == 59 || enemies_col[i] == 0 || enemies_col[i] == -1 || enemies_row[i] == -1) {
      find_new_enemy_position(i);
      fprintf(stderr, "Drawing new enemy at %d %d\n", enemies_row[i], enemies_col[i]);
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
  xTaskCreate(enemy_task, ENEMY_TASK, (1024U * 8) / sizeof(void *), NULL, PRIORITY_LOW, NULL);
}
