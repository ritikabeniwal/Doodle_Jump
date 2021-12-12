#pragma once
#include <stdbool.h>

void create_enemy_task();
bool check_collision_with_enemy(int jumper_row, int jumper_col);
bool check_gun_collision_with_enemy(int gun_row, int gun_col);
