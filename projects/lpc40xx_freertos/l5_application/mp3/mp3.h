#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

void create_mp3_task();

void mp3__init();
void mp3__send_command(uint8_t command, uint8_t data_1, uint8_t data_2);
void mp3_play(int num);
void mp3_play_level1_song();
void mp3_play_level2_song();
void mp3_play_start_song();
void mp3_play_sound(int num);
void mp3_play_jump_up_sound();
void mp3_play_spring_sound();
void mp3_play_collision_with_enemy_sound();
void mp3_play_game_over_song();
void mp3_play_monster_chomp_sound();
void mp3_play_shoot_gun_sound();