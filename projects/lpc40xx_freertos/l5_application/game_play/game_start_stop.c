#include "game_start_stop.h"
#include "FreeRTOS.h"
#include "background_screen.h"
#include "game_logic.h"
#include "gpio.h"
#include "jumper.h"
#include "led_matrix_draw_alphabets.h"
#include "led_matrix_draw_objects.h"
#include "task.h"

#include <stdbool.h>
#include <stdio.h>
//#define DEBUG 1
#define GAME_START_STOP_TASK "game_task"
#define START_SCREEN_TASK "start_task"
#define BUTTON_STATUS_TASK "button_status"
static void start_game();
static void stop_game();
bool game_started = 0;
static int play_button_count = 0;
static bool first_time = 1;
static gpio_s play_button_gpio;
static int start_game_button = 0;

static void check_play_button_status_task() {
  play_button_gpio.port_number = GPIO__PORT_1;
  play_button_gpio.pin_number = 31;
  gpio__construct_with_function(play_button_gpio.port_number, play_button_gpio.pin_number, 0);
  gpio__construct_as_input(play_button_gpio.port_number, play_button_gpio.pin_number);
  while (1) {
    if (!gpio__get(play_button_gpio)) {
      fprintf(stderr, "Gpio set\n");
      start_game_button = 1;
    }
    vTaskDelay(500);
  }
}

void start_screen_task() {
  led_matrtix_draw_objects_print_start_screen();
  while (1) {
    if (!game_started) {
      jumper_display_on_start_screen(56, 30);
    }
  }
}

void game_start_stop_task() {
  while (1) {
    if (!game_started) {
      led_matrtix_draw_objects_print_start_screen();
      while (1) {
        if (!start_game_button) {
          jumper_display_on_start_screen(56, 30);
        } else {
          break;
        }
      }
    }

    if (start_game_button) {
      start_game_button = 0;
      if ((play_button_count % 2) && game_started) {
#ifdef DEBUG
        fprintf(stderr, "Stopping game");
#endif
        stop_game();
        game_started = 0;
      } else if (!game_started) {
        led_matrix__clear_data_buffer();
#ifdef DEBUG
        fprintf(stderr, "Starting game");
#endif
        start_game();
        game_started = 1;
      }
      play_button_count++;
    }
    vTaskDelay(100);
  }
}
/*
static void stop_start_stop_task() {
  TaskHandle_t task_handle = xTaskGetHandle(GAME_START_STOP_TASK);
  game_started = 0;
  play_button_count = 0;
  first_time = 1;
  vTaskSuspend(task_handle);
}

static void restart_start_stop_task() {
  TaskHandle_t task_handle = xTaskGetHandle(GAME_START_STOP_TASK);
  vTaskResume(task_handle);
}

static void restart_game() {
  resume_background_tasks();
  restart_start_stop_task();
}
*/
void start_game() {
  if (first_time) {
    // create_start_stop_task();
    // create_background_screen_tasks();
    // create_jumper_tasks();
    create_game_play_tasks();
    first_time = 0;
  } else {
    // restart_game();
  }
}

void stop_game() {
  // Suspend all game tasks
  // stop_start_stop_task();
  // stop_background_tasks();
  uint32_t *bad_pointer = (uint32_t *)0x00000001;
  *bad_pointer = 0xDEADBEEF;
}

void create_start_stop_task() {
  xTaskCreate(check_play_button_status_task, BUTTON_STATUS_TASK, (256U * 8) / sizeof(void *), NULL, PRIORITY_MEDIUM,
              NULL);
  //  xTaskCreate(, START_SCREEN_TASK, (256U * 8) / sizeof(void *), NULL, PRIORITY_LOW, NULL);
  xTaskCreate(game_start_stop_task, GAME_START_STOP_TASK, (512U * 8) / sizeof(void *), NULL, PRIORITY_LOW, NULL);
}
