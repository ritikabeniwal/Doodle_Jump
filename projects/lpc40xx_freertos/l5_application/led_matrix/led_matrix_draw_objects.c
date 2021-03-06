#include "led_matrix_draw_objects.h"
#include "led_matrix_draw_alphabets.h"
#include "led_matrix_driver.h"

uint8_t jumper[JUMPER_LENGTH] = {0xE, 0x1F, 0x1F, 0x1F, 0xE};

uint8_t jumper_color[JUMPER_LENGTH][JUMPER_WIDTH] = {
    {OFF, BLUE, BLUE, BLUE, OFF},       {GREEN, GREEN, GREEN, GREEN, GREEN}, {YELLOW, YELLOW, YELLOW, YELLOW, YELLOW},
    {YELLOW, YELLOW, RED, RED, YELLOW}, {OFF, YELLOW, YELLOW, YELLOW, OFF},
};

uint8_t inverted_jumper[JUMPER_LENGTH] = {0xE, 0x1F, 0x1F, 0x1F, 0xE};

uint8_t inverted_jumper_color[JUMPER_LENGTH][JUMPER_WIDTH] = {
    {OFF, YELLOW, YELLOW, YELLOW, OFF},  {YELLOW, YELLOW, RED, RED, YELLOW}, {YELLOW, YELLOW, YELLOW, YELLOW, YELLOW},
    {GREEN, GREEN, GREEN, GREEN, GREEN}, {OFF, BLUE, BLUE, BLUE, OFF},
};
/*uint8_t enemy[ENEMY_LENGTH] = {0xE, 0x1F, 0x1F, 0x1F, 0xE};
uint8_t enemy_color[ENEMY_LENGTH][ENEMY_WIDTH] = {
    {OFF, RED, RED, RED, OFF},       {BLUE, BLUE, GREEN, GREEN, GREEN}, {OFF, YELLOW, YELLOW, YELLOW, OFF},
    {OFF, GREEN, GREEN, GREEN, RED}, {OFF, RED, RED, RED, OFF},
};
*/
uint8_t enemy[ENEMY_LENGTH] = {0x11, 0x0E, 0x0E, 0x04, 0x0A};
uint8_t enemy_color[ENEMY_LENGTH][ENEMY_WIDTH] = {
    {GREEN, OFF, OFF, OFF, GREEN}, {OFF, YELLOW, YELLOW, YELLOW, OFF}, {OFF, YELLOW, YELLOW, YELLOW, OFF},
    {OFF, OFF, RED, OFF, OFF},     {OFF, GREEN, OFF, GREEN, OFF},
};
uint8_t gun_shot[GUN_LENGTH] = {0x1, 0x1, 0x1, 0x1};
uint8_t gun_shot_color[GUN_LENGTH][GUN_WIDTH] = {
    {RED},
    {RED},
    {RED},
    {RED},
};

void draw_jumper(int row, int col) {
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

void draw_inverted_jumper(int row, int col) {
  for (int i = 0; i < JUMPER_LENGTH; i++) {
    for (int j = JUMPER_WIDTH - 1; j >= 0; j--) {
      if (inverted_jumper[i] & (1 << j)) {
        led_matrix__set_pixel(row - i, j + col, inverted_jumper_color[i][j]);
        // led_matrix__set_pixel(row + i, j + col, jumper_color[i][j]);
      }
    }
  }
}

void clear_inverted_jumper(int row, int col) {
  for (int i = 0; i < JUMPER_LENGTH; i++) {
    for (int j = JUMPER_WIDTH - 1; j >= 0; j--) {
      if (inverted_jumper[i] & (1 << j)) {
        led_matrix__clear_pixel(row - i, j + col);
        // led_matrix__clear_pixel(row + i, j + col);
      }
    }
  }
}

void draw_enemy(int row, int col) {
  for (int i = 0; i < ENEMY_LENGTH; i++) {
    for (int j = ENEMY_WIDTH - 1; j >= 0; j--) {
      if (enemy[i] & (1 << j)) {
        led_matrix__set_pixel(row - i, j + col, enemy_color[i][j]);
        // led_matrix__set_pixel(row + i, j + col, jumper_color[i][j]);
      }
    }
  }
}

void clear_enemy(int row, int col) {
  for (int i = 0; i < ENEMY_LENGTH; i++) {
    for (int j = ENEMY_WIDTH - 1; j >= 0; j--) {
      if (enemy[i] & (1 << j)) {
        led_matrix__clear_pixel(row - i, j + col);
        // led_matrix__clear_pixel(row + i, j + col);
      }
    }
  }
}

void draw_gun(int row, int col) {
  for (int i = 0; i < GUN_LENGTH; i++) {
    for (int j = GUN_WIDTH - 1; j >= 0; j--) {
      if (gun_shot[i] & (1 << j)) {
        led_matrix__set_pixel(row - i, j + col, gun_shot_color[i][j]);
        // led_matrix__set_pixel(row + i, j + col, jumper_color[i][j]);
      }
    }
  }
}

void clear_gun(int row, int col) {
  for (int i = 0; i < GUN_LENGTH; i++) {
    for (int j = GUN_WIDTH - 1; j >= 0; j--) {
      if (gun_shot[i] & (1 << j)) {
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
