#pragma once
#include <led_matrix_driver.h>
#include <stdbool.h>

void create_jumper_tasks();
void jumper_display_on_start_screen(int row, int col);
bool find_initial_jumper_position(int *row, int *col);
data_size get_jumper_row_data(int col);
