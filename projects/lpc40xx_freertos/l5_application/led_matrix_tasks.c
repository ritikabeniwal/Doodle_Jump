#include <stdio.h>
#include <time.h>

#include "FreeRTOS.h"
#include "task.h"

#include "board_io.h"
#include "common_macros.h"
#include "led_matrix_driver.h"
#include "led_matrix_tasks.h"
#include "periodic_scheduler.h"
#include "sj2_cli.h"
#define SLAB_LENGTH 8
#define MIN_SLAB_GAP 4
#define MAX_SLAB_GAP 8
#define LAST_SLAB_INDEX 56
#define MAX_SLABS_PER_ROW 4
#define BACKGROUND_ROW_START 8
#define BACKGROUND_ROW_END 56
#define BACKGROUND_ROW_JUMP 8

static data_size background_buffer[TOTAL_LED_MATRIX_ROWS];

static void set_random_slabs_in_row(int row) {
  uint8_t previous_slab_end_index = 0;
  uint8_t current_slab_index = 0;
  uint8_t divisor;
  data_size slab_bits = 0;

  for (int i = 0; i < SLAB_LENGTH; i++) {
    slab_bits |= (1 << i);
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
    led_matrix__set_row_data(row, RED_COLOR_BIT, background_buffer[row]);
    previous_slab_end_index = current_slab_index + SLAB_LENGTH + MIN_SLAB_GAP;
    if (previous_slab_end_index > LAST_SLAB_INDEX) {
      break;
    }
  }
}

static void initialize_background_screen() {
  for (int i = BACKGROUND_ROW_END; i >= BACKGROUND_ROW_START; i -= BACKGROUND_ROW_JUMP) {
    set_random_slabs_in_row(i);
  }
}

static void shift_background_screen_down() {
  for (int i = BACKGROUND_ROW_END; i > BACKGROUND_ROW_START; i -= BACKGROUND_ROW_JUMP) {
    background_buffer[i] = background_buffer[i - BACKGROUND_ROW_JUMP];
    led_matrix__set_row_data(i, RED_COLOR_BIT, background_buffer[i]);
  }
  set_random_slabs_in_row(BACKGROUND_ROW_START);
}

static void update_background_screen() { shift_background_screen_down(); }

static void background_screen_task() {
  srand(xTaskGetTickCount());
  initialize_background_screen();
  while (1) {
    update_background_screen();
    vTaskDelay(2000);
  }
}

static void display_task() {
  while (1) {
    led_matrix__update_display();
    vTaskDelay(10);
  }
}
void create_led_matrix_tasks(void) {
  led_matrix_init();
  xTaskCreate(background_screen_task, "background_screen", (512U * 8) / sizeof(void *), NULL, PRIORITY_LOW, NULL);
  xTaskCreate(display_task, "display_task", (512U * 8) / sizeof(void *), NULL, PRIORITY_LOW, NULL);
}
