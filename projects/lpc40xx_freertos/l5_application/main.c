#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "board_io.h"
#include "common_macros.h"
#include "led_matrix_driver.h"
#include "led_matrix_tasks.h"
#include "periodic_scheduler.h"
#include "sj2_cli.h"

static uint64_t border_data[8] = {0xFE, 0x10, 0x10, 0x10, 0x10, 0x90, 0x90, 0xF0};
static int num_leds_to_print = 8;
static int delay_time = 40;
void left_to_right(int row) {
  int i;
  for (int col = 0; col <= 64; col += num_leds_to_print) {
    i = 0;
    for (int row_num = row; row_num < num_leds_to_print; row_num++) {
      led_matrix__set_row_data(row_num, BLUE_COLOR_BIT, border_data[i] << (64 - col));
      led_matrix__set_row_data(row_num, GREEN_COLOR_BIT, border_data[i] << (64 - col));
      i++;
    }
    delay__ms(delay_time);
    // led_matrix__update_display();
  }
}
/*
void top_to_down(int col) {
  uint64_t to_shift = 64 - col - 1;

  for (int row = 0; row <= 64; row += num_leds_to_print) {
    for (int row_num = row; row_num < row + num_leds_to_print; row_num++) {
      led_matrix__set_row_data(row_num, BLUE_COLOR_BIT, border_data << to_shift);
      led_matrix__set_row_data(row_num, GREEN_COLOR_BIT, border_data << to_shift);
    }
    delay__ms(delay_time);
    for (int row_num = row; row_num < row + num_leds_to_print; row_num++) {
      led_matrix__set_row_data(row_num, BLUE_COLOR_BIT, 0 << to_shift);
      led_matrix__set_row_data(row_num, GREEN_COLOR_BIT, 0 << to_shift);
    }
  }
}

void right_to_left(int row) {
  for (int col = 63; col >= 0; col -= num_leds_to_print) {
    for (int row_num = row; row_num >= (64 - num_leds_to_print); row_num--) {
      led_matrix__set_row_data(row_num, BLUE_COLOR_BIT, border_data << (63 - col));
      led_matrix__set_row_data(row_num, GREEN_COLOR_BIT, border_data << (63 - col));
    }
    delay__ms(delay_time);
    // led_matrix__update_display();
  }
}

void down_to_top(int col) {
  uint64_t to_shift = 63 - num_leds_to_print + 1;

  for (int row = 63; row >= 0; row -= num_leds_to_print) {
    for (int row_num = row; row_num > row - num_leds_to_print; row_num--) {
      led_matrix__set_row_data(row_num, BLUE_COLOR_BIT, border_data << to_shift);
      led_matrix__set_row_data(row_num, GREEN_COLOR_BIT, border_data << to_shift);
    }
    delay__ms(delay_time);
    for (int row_num = row; row_num > row - num_leds_to_print; row_num--) {
      led_matrix__set_row_data(row_num, BLUE_COLOR_BIT, 0 << to_shift);
      led_matrix__set_row_data(row_num, GREEN_COLOR_BIT, 0 << to_shift);
    }
  }
}
*/
void print_border_pattern() {
  while (1) {
    left_to_right(0);
    //  top_to_down(63);
    //  right_to_left(63);
    //  down_to_top(0);
  }
}
// print
void fill_data_to_print_char(uint8_t *char_to_print, int starting_row, int starting_col) {
  for (int i = 0; i < 8; i++) {
    led_matrix__set_row_data(starting_row + i, BLUE_COLOR_BIT, (data_size)char_to_print[i] << (56 - starting_col));
  }
}
void printJ(int starting_row, int starting_col) {
  uint8_t char_to_print[8] = {0xFE, 0x10, 0x10, 0x10, 0x10, 0x90, 0x90, 0xF0};
  fill_data_to_print_char(char_to_print, starting_row, starting_col);
}

void printU(int starting_row, int starting_col) {
  uint8_t char_to_print[8] = {0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x7E};
  fill_data_to_print_char(char_to_print, starting_row, starting_col);
}

void printV(int starting_row, int starting_col) {
  uint8_t char_to_print[8] = {0x81, 0xC3, 0x42, 0x66, 0x6C, 0x28, 0x10, 0x00};
  fill_data_to_print_char(char_to_print, starting_row, starting_col);
}
/*
void printE(int starting_row, int starting_col) {
  uint8_t char_to_print[8] = {0x, 0x, 0x, 0x, 0x, 0x, 0x, 0x};
  fill_data_to_print_char(char_to_print, starting_row, starting_col);
}
void printN(int starting_row, int starting_col) {
  uint8_t char_to_print[8] = {0x, 0x, 0x, 0x, 0x, 0x, 0x, 0x};
  fill_data_to_print_char(char_to_print, starting_row, starting_col);
}
void printI(int starting_row, int starting_col) {
  uint8_t char_to_print[8] = {0x, 0x, 0x, 0x, 0x, 0x, 0x, 0x};
  fill_data_to_print_char(char_to_print, starting_row, starting_col);
}
void printL(int starting_row, int starting_col) {
  uint8_t char_to_print[8] = {0x, 0x, 0x, 0x, 0x, 0x, 0x, 0x};
  fill_data_to_print_char(char_to_print, starting_row, starting_col);
}
void printP(int starting_row, int starting_col) {
  uint8_t char_to_print[8] = {0x, 0x, 0x, 0x, 0x, 0x, 0x, 0x};
  fill_data_to_print_char(char_to_print, starting_row, starting_col);
}

void printR(int starting_row, int starting_col) {
  uint8_t char_to_print[8] = {0x, 0x, 0x, 0x, 0x, 0x, 0x, 0x};
  fill_data_to_print_char(char_to_print, starting_row, starting_col);
}
*/
static void led_matrix_task(void *params) {
  led_matrix_init();
  while (1) {
    // led_matrix__set_row_data(8, BLUE_COLOR_BIT, 0x1);
    // led_matrix__fill_data_buffer(0xF0F0F0F0F0F0F0F0, RED_COLOR_BIT);
    // Juvenile
    printJ(20, 24);
    led_matrix__update_display();
    printU(20, 32);
    led_matrix__update_display();
    printV(20, 40);
    led_matrix__update_display();
    /*  printE(20, 24);
      printN(20, 32);
      printI(20, 40);
      printL(20, 48);
      printE(20, 56);
      //Jumpers
      printJ(30, 0);
      printU(30, 8);
      printM(30, 16);
      printP(30, 24);
      printE(30, 32);
      printR(30, 40);
      printS(30, 48);
  */
    // led_matrix__update_display();
    vTaskDelay(1);
  }
}

void print_border_pattern_task(void *param) {
  while (1) {
    print_border_pattern();
    vTaskDelay(1);
  }
}

int main(void) {
  // xTaskCreate(led_matrix_task, "led_matrix_task", (512U * 8) / sizeof(void *), NULL, PRIORITY_LOW, NULL);
  // xTaskCreate(print_border_pattern_task, "print_border_pattern_task", (512U * 8) / sizeof(void *), NULL,
  // PRIORITY_LOW, NULL);
  create_led_matrix_tasks();
  puts("Starting RTOS");
  vTaskStartScheduler(); // This function never returns unless RTOS scheduler runs out of memory and fails
  return 0;
}
