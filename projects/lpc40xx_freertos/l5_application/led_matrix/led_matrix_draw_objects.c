#include "led_matrix_draw_objects.h"
#include "led_matrix_driver.h"
#define JUMPER_WIDTH 4
#define JUMPER_LENGTH 4
uint8_t jumper[JUMPER_LENGTH] = {0xF, 0xF, 0xF, 0xF};
uint8_t jumper_color[JUMPER_LENGTH][JUMPER_WIDTH] = {
    YELLOW, GREEN, BLUE, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN,
};

void draw_jumper(int row, int col) {
  data_size jumper_data;
  for (int i = 0; i < JUMPER_LENGTH; i++) {
    for (int j = JUMPER_WIDTH - 1; j >= 0; j--) {
      if (jumper[i] & (1 << j)) {
        led_matrix__set_pixel(row - i, j + col, jumper_color[i][j]);
      }
    }
  }
}

void clear_jumper(int row, int col) {
  for (int i = 0; i < JUMPER_LENGTH; i++) {
    for (int j = JUMPER_WIDTH - 1; j >= 0; j--) {
      if (jumper[i] & (1 << j)) {
        led_matrix__clear_pixel(row - i, j + col);
      }
    }
  }
}
