#pragma once
#include "led_matrix_driver.h"
void led_matrix_print_alphabet(char to_display, int row, int col);
void led_matrix_draw_alphabets_print_string(char *, int, int, led_matrix__color_e);
void led_matrix_print_digit(int num, int row, int column);
void led_matrix_print_digits_string(int num, int row, int column, led_matrix__color_e color);
