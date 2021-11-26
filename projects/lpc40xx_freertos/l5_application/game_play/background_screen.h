#pragma once
#include "led_matrix_driver.h"
#include <stdbool.h>
void create_background_screen_tasks(void);
void stop_background_tasks(void);
void resume_background_tasks(void);
int get_last_background_row(data_size *);
void initialize_background_screen();
void update_background_screen(bool collision_detected);
void shift_background_screen_down(int row);
bool detect_collision_background_screen(int row, int col);
int get_last_background_screen_row();
