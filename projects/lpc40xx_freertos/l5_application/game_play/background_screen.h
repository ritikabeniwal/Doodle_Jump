#pragma once
#include "led_matrix_driver.h"
void create_background_screen_tasks(void);
void stop_background_tasks(void);
void resume_background_tasks(void);
int get_last_background_row(data_size *);
