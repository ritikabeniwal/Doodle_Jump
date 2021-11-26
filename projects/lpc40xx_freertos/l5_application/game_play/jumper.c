#include "jumper.h"
#include "FreeRTOS.h"
#include "led_matrix_draw_objects.h"
#include "led_matrix_driver.h"
#include "task.h"
#include <stdbool.h>
#define JUMPER_TASK "jumper"

data_size get_jumper_row_data(int col) {
  data_size row_data = 0;
  for (int i = col; i < (col + JUMPER_WIDTH); i++) {
    row_data |= (1 << i);
  }
  return row_data;
}

bool find_initial_jumper_position(int *row, int *col) {
  data_size last_bckground_row_data;
  fprintf(stderr, "Calling\n");
  int last_backround_row = get_last_background_row(&last_bckground_row_data);
  fprintf(stderr, "BG row = %d\n", last_backround_row);
  for (int i = 0; i < TOTAL_LED_MATRIX_COLS; i++) {
    if ((last_bckground_row_data) & (1 << i)) {
      *col = i;
      break;
    }
  }
  fprintf(stderr, "Col = %d", *col);
  *row = last_backround_row;
  *col = *col + 2;
  // If found a valid position return true
  if (*row != 0 || *col != 0) {
    return 1;
  }
  return 0;
}

void jumper_task() {
  int row = 0, col = 0;
  while (1) {
    if (find_initial_jumper_position(&row, &col)) {
      draw_jumper(row, col);
      break;
    }

    vTaskDelay(100);
  }
  clear_jumper((row), col);
  while (1) {
    for (int i = 0; i < 4; i++) {
      vTaskDelay(200);
      clear_jumper((row), col);
      draw_jumper(row - (i * 2), col);
      row = row - (i * 2);
    }
    for (int i = 0; i < 4; i++) {
      vTaskDelay(200);
      clear_jumper(row, col);
      draw_jumper(row + (i * 2), col);
      row = row + (i * 2);
    }
  }
}

void jumper_display_on_start_screen(int row, int col) {
  for (int i = 0; i < 4; i++) {
    vTaskDelay(100);
    clear_jumper((row), col);
    draw_jumper(row - (i * 2), col);
    row = row - (i * 2);
  }
  for (int i = 0; i < 4; i++) {
    vTaskDelay(100);
    clear_jumper(row, col);
    draw_jumper(row + (i * 2), col);
    row = row + (i * 2);
  }
}

void create_jumper_tasks() {
  xTaskCreate(jumper_task, JUMPER_TASK, (256U * 8) / sizeof(void *), NULL, PRIORITY_LOW, NULL);
}
