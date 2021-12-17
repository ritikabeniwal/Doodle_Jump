#include <stdio.h>
#include <time.h>

#include "FreeRTOS.h"
#include "task.h"

#include "background_screen.h"
#include "board_io.h"
#include "common_macros.h"
#include "game_start_stop.h"
#include "jumper.h"
#include "led_matrix_driver.h"
#include "periodic_scheduler.h"
#include "sj2_cli.h"

#define SLAB_LENGTH 6
#define SPECIAL_SLAB_LENGTH 2
#define MIN_SLAB_GAP 6
#define MAX_SLAB_GAP 8
#define LAST_SLAB_INDEX 56
#define MAX_SLABS_PER_ROW 3
#define BACKGROUND_ROW_START 8
#define BACKGROUND_ROW_END 56
#define BACKGROUND_ROW_JUMP 8
#define BACKGROUND_SCREEN_TASK "bg_task"

// TODO: Check this in a different function

data_size background_buffer[TOTAL_LED_MATRIX_ROWS];

bool detect_collision_background_screen(int row, int col) {
  data_size background_row_data = background_buffer[row];
  data_size jumper_row_data;
  if (background_row_data == 0) {
    return 0;
  }

  jumper_row_data = get_jumper_row_data(col);

  if (jumper_row_data & background_row_data) {
    return 1;
  }
  return 0;
}

int get_last_background_screen_row(int row) {
  if (row % 2 == 0) {
    return BACKGROUND_ROW_END;
  } else {
    return BACKGROUND_ROW_END - 1;
  }
}

int get_last_background_row(data_size *return_data) {
  *return_data = background_buffer[BACKGROUND_ROW_END];
  return (int)BACKGROUND_ROW_END;
}

static void set_random_slabs_in_row(int row) {
  uint8_t previous_slab_end_index = 0;
  uint8_t current_slab_index = 0;
  uint8_t divisor;
  static int special_slab_count = 0;
  data_size slab_bits = 0;
  data_size special_slab_bits = 0;

  for (int i = 0; i < SLAB_LENGTH; i++) {
    slab_bits |= (1 << i);
  }
  for (int i = 0; i < SPECIAL_SLAB_LENGTH; i++) {
    special_slab_bits |= (1 << i);
  }
  background_buffer[row] = 0;

  for (int slab_count = 0; slab_count < MAX_SLABS_PER_ROW; slab_count++) {
    while (1) {
      divisor = LAST_SLAB_INDEX - previous_slab_end_index;
      current_slab_index = (rand() % (divisor)) + previous_slab_end_index;
      if (slab_count == 0 || (current_slab_index - previous_slab_end_index) <= MAX_SLAB_GAP) {
        break;
      }
    }
    // Udate the LED Matrix
    background_buffer[row] |= slab_bits << (64 - SLAB_LENGTH - current_slab_index);
    led_matrix__set_row_data(row, CYAN, background_buffer[row]);
    if (special_slab_count % 3 == 0) {
      background_buffer[row - 1] = (special_slab_bits) << (64 - SLAB_LENGTH - current_slab_index);
      led_matrix__set_row_data(row - 1, YELLOW, background_buffer[row - 1]);
    }
    special_slab_count++;
    previous_slab_end_index = current_slab_index + SLAB_LENGTH + MIN_SLAB_GAP;
    if (previous_slab_end_index > LAST_SLAB_INDEX) {
      break;
    }
  }
}

void initialize_background_screen() {
  for (int i = BACKGROUND_ROW_END; i >= BACKGROUND_ROW_START; i -= BACKGROUND_ROW_JUMP) {
    set_random_slabs_in_row(i);
  }
}

void shift_background_screen_down(int row) {
  int shift_by;
  int shift_till;
  int num_shifts;
  if (row >= BACKGROUND_ROW_END) {
    return;
  }

  srand(xTaskGetTickCount());

  num_shifts = (BACKGROUND_ROW_END - row) / BACKGROUND_ROW_JUMP;
  shift_by = num_shifts * BACKGROUND_ROW_JUMP;
  shift_till = num_shifts * BACKGROUND_ROW_START;

  fprintf(stderr, "row = %d, shift_by = %d, shift_till = %d, num shifts = %d\n", row, shift_by, shift_till, num_shifts);
  for (int i = BACKGROUND_ROW_END; i > shift_till; i -= BACKGROUND_ROW_JUMP) {
    background_buffer[i] = background_buffer[i - shift_by];
    led_matrix__set_row_data(i, CYAN, background_buffer[i]);
  }

  for (int i = BACKGROUND_ROW_END - 1; i > shift_till - 1; i -= BACKGROUND_ROW_JUMP) {
    background_buffer[i] = background_buffer[i - shift_by];
    led_matrix__set_row_data(i, YELLOW, background_buffer[i]);
  }
  for (int i = 0; i < num_shifts; i++) {
    fprintf(stderr, "Creating %d row\n", i);
    background_buffer[BACKGROUND_ROW_START + (i * BACKGROUND_ROW_JUMP) - 1] = 0;
    led_matrix__set_row_data(BACKGROUND_ROW_START + (i * BACKGROUND_ROW_JUMP) - 1, YELLOW,
                             background_buffer[BACKGROUND_ROW_START + i - 1]);
    set_random_slabs_in_row(BACKGROUND_ROW_START + i * BACKGROUND_ROW_JUMP);
  }
}

static void print_current_background_buffer() {
  for (int i = BACKGROUND_ROW_END; i >= BACKGROUND_ROW_START; i -= BACKGROUND_ROW_JUMP) {
    led_matrix__set_row_data(i, CYAN, background_buffer[i]);
    led_matrix__set_row_data(i - 1, YELLOW, background_buffer[i - 1]);
  }
}

void update_background_screen(bool collision_detected) {
  if (collision_detected) {
    shift_background_screen_down(48);
  } else {
    print_current_background_buffer();
  }
}

static void background_screen_task() {
  srand(xTaskGetTickCount());
  initialize_background_screen();
  while (1) {
    if (game_started) {
      update_background_screen(0);
      vTaskDelay(100);
    }
  }
}

void stop_background_tasks() {
  TaskHandle_t task_handle = xTaskGetHandle(BACKGROUND_SCREEN_TASK);
  vTaskSuspend(task_handle);
}

void resume_background_tasks() {
  TaskHandle_t task_handle = xTaskGetHandle(BACKGROUND_SCREEN_TASK);
  vTaskResume(task_handle);
}

void create_background_screen_tasks(void) {
  xTaskCreate(background_screen_task, BACKGROUND_SCREEN_TASK, (256U * 8) / sizeof(void *), NULL, PRIORITY_LOW, NULL);
}
