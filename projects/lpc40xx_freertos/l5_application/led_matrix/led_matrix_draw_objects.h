#pragma once
#include "led_matrix_driver.h"

#define JUMPER_WIDTH 5
#define JUMPER_LENGTH 5
#define ENEMY_WIDTH 5
#define ENEMY_LENGTH 5

void draw_jumper(int row, int col);
void clear_jumper(int row, int col);
void draw_enemy(int row, int col);
void clear_enemy(int row, int col);
void led_matrtix_draw_objects_print_start_screen();
