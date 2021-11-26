#include "led_matrix_draw_objects.h"
#include "led_matrix_driver.h"
uint8_t jumper[JUMPER_LENGTH] = {0xE, 0x1F, 0x1F, 0x1F, 0xE};
uint8_t jumper_color[JUMPER_LENGTH][JUMPER_WIDTH] = {
    {OFF, YELLOW, YELLOW, YELLOW, OFF},  {YELLOW, YELLOW, RED, RED, YELLOW}, {YELLOW, YELLOW, YELLOW, YELLOW, YELLOW},
    {GREEN, GREEN, GREEN, GREEN, GREEN}, {OFF, BLUE, BLUE, BLUE, OFF},
};

void draw_jumper(int row, int col) {
  data_size jumper_data;
  for (int i = 0; i < JUMPER_LENGTH; i++) {
    for (int j = JUMPER_WIDTH - 1; j >= 0; j--) {
      if (jumper[i] & (1 << j)) {
        led_matrix__set_pixel(row - i, j + col, jumper_color[i][j]);
        // led_matrix__set_pixel(row + i, j + col, jumper_color[i][j]);
      }
    }
  }
}

void clear_jumper(int row, int col) {
  for (int i = 0; i < JUMPER_LENGTH; i++) {
    for (int j = JUMPER_WIDTH - 1; j >= 0; j--) {
      if (jumper[i] & (1 << j)) {
        led_matrix__clear_pixel(row - i, j + col);
        // led_matrix__clear_pixel(row + i, j + col);
      }
    }
  }
}
void led_matrtix_draw_objects_print_start_screen() {
  led_matrix_draw_alphabets_print_string("JUVENILE", 15, 8, GREEN);
  led_matrix_draw_alphabets_print_string("JUMPERS", 25, 11, CYAN);
}
