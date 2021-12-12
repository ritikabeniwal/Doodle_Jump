#include "game_logic.h"
#include "FreeRTOS.h"
#include "background_screen.h"
#include "enemy.h"
#include "joystick.h"
#include "jumper.h"
#include "led_matrix_draw_alphabets.h"
#include "led_matrix_draw_objects.h"
#include "led_matrix_driver.h"
#include "mp3.h"
#include "task.h"
//#define PRINT_DEBUG

#define GAME_PLAY_TASK "GAME_PLAY"
#define GUN_TASK "gun_task"
#define MAX_JUMP_STANDARD 11
#define MAX_JUMP_SPRING 20

#define LEVEL1_DELAY_TIME_STANDARD 65
#define LEVEL1_DELAY_TIME_SPRING 50
#define LEVEL2_DELAY_TIME_STANDARD 45
#define LEVEL2_DELAY_TIME_SPRING 40

#define LEVEL1_SCORE_INCREMENT 10
#define LEVEL2_SCORE_INCREMENT 15
#define LEVEL1_SCORE_MAX_SCORE 150

static int level2_song_played = 0;
static int level1_song_played = 0;
static int max_jump = 11;
static uint32_t score = 0;
static int going_up = 1;
static int level = 0;
static int jumper_row, jumper_col;
static bool collision_detected = 0;
static int jump_count = 0;
static joystick_value joystick_data;
static int delay_time_ms = LEVEL1_DELAY_TIME_STANDARD;
static bool enemy_task_started = 0;
static uint32_t score_increment = LEVEL1_SCORE_INCREMENT;
bool game_over = 0;

void end_game() {
  game_over = 1;
  vTaskDelay(100);
  led_matrix__clear_data_buffer();
  fprintf(stderr, "Game over\n");
  // mp3_play_game_over_song();
  led_matrix_draw_alphabets_print_string("GAME OVER", 15, 8, MAGENTA);
  led_matrix_draw_alphabets_print_string("SCORE ", 25, 8, MAGENTA);
  led_matrix_print_digits_string(score, 25, 40, MAGENTA);
  led_matrix_draw_alphabets_print_string("LEVEL ", 35, 8, MAGENTA);
  led_matrix_print_digits_string(level + 1, 35, 40, MAGENTA);
  while (1) {
    vTaskDelay(10000);
  }
}

void get_jumper_position_based_on_joystick_data(joystick_value *data, int *row, int *col) {

  if (data->s_x < 500) {
    *col += 1;
  } else if (data->s_x > 3500) {
    *col -= 1;
  } else {
    return;
  }
  if (*col >= 59) {
    *col = 59;
  } else if (*col <= 0) {
    *col = 0;
  }
}

void basic_level() {
  int prev_jumper_row = 0;
  update_background_screen(collision_detected);
  if (check_collision_with_enemy(jumper_row, jumper_col)) {
    mp3_play_collision_with_enemy_sound();
    vTaskDelay(1000);
    level2_song_played = 0;
    level1_song_played = 0;
    end_game();
  }
  if (going_up) {
    clear_jumper(jumper_row, jumper_col);
    joystick_data = get_joystick_data();
    get_jumper_position_based_on_joystick_data(&joystick_data, &jumper_row, &jumper_col);
    jumper_row -= 1;
    draw_jumper(jumper_row, jumper_col);
    jump_count++;
#ifdef PRINT_DEBUG
    fprintf(stderr, "jump_count going up %d MAX_JUMP = %d, going up = %d\n", jump_count, max_jump, going_up);
#endif
    if (jump_count == max_jump) {
      going_up = 0;
#ifdef PRINT_DEBUG
      fprintf(stderr, "going_up = %d\n", going_up);
#endif
    }

  } else {
    clear_jumper(jumper_row, jumper_col);
    joystick_data = get_joystick_data();
    get_jumper_position_based_on_joystick_data(&joystick_data, &jumper_row, &jumper_col);
    jumper_row += 1;
    draw_jumper(jumper_row, jumper_col);
    if (detect_collision_background_screen(jumper_row, jumper_col)) {
      shift_background_screen_down(jumper_row);
      clear_jumper(jumper_row, jumper_col);
      prev_jumper_row = jumper_row;
      jumper_row = get_last_background_screen_row(jumper_row);
      draw_jumper(jumper_row, jumper_col);
      going_up = 1;
      if (prev_jumper_row != jumper_row) {
        score += score_increment;
        mp3_play_jump_up_sound();
        level2_song_played = 0;
        level1_song_played = 0;
      }
      /*
      if (score == 50) {
        level = 1;
        score += 5;
      }
      */
      if (jumper_row % 2 == 0) {
        jump_count = 0;
        max_jump = MAX_JUMP_STANDARD;
        delay_time_ms = LEVEL1_DELAY_TIME_STANDARD;
      } else {
        jump_count = 1;
        max_jump = MAX_JUMP_SPRING;
        delay_time_ms = LEVEL1_DELAY_TIME_SPRING;
        mp3_play_spring_sound();
        level2_song_played = 0;
        level1_song_played = 0;
      }
      vTaskDelay(delay_time_ms);
      return;
    }
    jump_count--;
#ifdef PRINT_DEBUG
    fprintf(stderr, "jump_count going down  %d\n", jump_count);
#endif
    if (jump_count == 0) {
      end_game();
    }
  }
}

void play_level_2() {
  if (!level2_song_played) {
    mp3_play_level2_song();
    level2_song_played = 1;
  }
  if (!enemy_task_started) {
    led_matrix__clear_data_buffer();
    led_matrix_draw_alphabets_print_string("LEVEL", 15, 15, GREEN);
    led_matrix_print_digits_string(level + 1, 15, 50, YELLOW);
    vTaskDelay(2000);
    led_matrix__clear_data_buffer();
    create_enemy_task();
    enemy_task_started = 1;
    delay_time_ms = LEVEL2_DELAY_TIME_STANDARD;
  }
  basic_level();
  return;
}

void play_level_1() {
  if (!level1_song_played) {
    mp3_play_level1_song();
    level1_song_played = 1;
  }
  basic_level();
}
void update_score() {
  if ((level == 0) && (score >= LEVEL1_SCORE_MAX_SCORE)) {
    score_increment = LEVEL2_SCORE_INCREMENT;
    level++;
  }

  led_matrix_draw_alphabets_print_string("SCORE", 1, 2, GREEN);
  led_matrix_print_digits_string(score, 1, 35, MAGENTA);
}

void game_play(void *params) {
  delay_time_ms = LEVEL1_DELAY_TIME_STANDARD;
  // Initialize Background Screen
  led_matrix_draw_alphabets_print_string("LEVEL", 15, 15, GREEN);
  led_matrix_print_digits_string(level + 1, 15, 50, YELLOW);
  vTaskDelay(2000);
  led_matrix__clear_data_buffer();
  initialize_background_screen();
  (void)find_initial_jumper_position(&jumper_row, &jumper_col);
  draw_jumper(jumper_row, jumper_col);
  while (1) {
    update_score();
    vTaskDelay(delay_time_ms);
    switch (level) {
    case 0:
      play_level_1();
      continue;
    case 1:
      play_level_2();
      continue;
    default:
      break;
    }
  }
  return;
}

void create_game_play_task() {
  xTaskCreate(game_play, GAME_PLAY_TASK, (1024U * 8) / sizeof(void *), NULL, PRIORITY_LOW, NULL);
}

void shoot_gun(int row, int col) {
  int gun_row = row;
  int gun_col = col + JUMPER_WIDTH / 2;
  joystick_value data;
  data = get_joystick_data();
  //  fprintf(stderr, "y value = %d \n", data.s_y);
  if (data.s_y > 3900) {
    while (1) {
      draw_gun(gun_row, gun_col);
      vTaskDelay(200);
      if (check_gun_collision_with_enemy(gun_row, gun_col)) {
        clear_gun(gun_row, gun_col);
        break;
      }
      clear_gun(gun_row, gun_col);
      gun_row -= GUN_LENGTH;
      if (gun_row < 16) {
        break;
      }
    }
  }
}

void gun_task(void *params) {
  while (1) {
    shoot_gun(jumper_row, jumper_col);
    vTaskDelay(100);
  }
}
void create_gun_task() { xTaskCreate(gun_task, GUN_TASK, (256U * 8) / sizeof(void *), NULL, PRIORITY_LOW, NULL); }
