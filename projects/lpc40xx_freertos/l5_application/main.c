#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "board_io.h"
#include "common_macros.h"
#include "game_start_stop.h"
#include "joystick.h"
#include "led_matrix_driver.h"
#include "led_matrix_tasks.h"
#include "mp3.h"
#include "periodic_scheduler.h"
#include "sj2_cli.h"

int main(void) {
  create_led_matrix_tasks();
  create_start_stop_task();
  initialize_joystick();
  create_mp3_task();
  // srand(xTaskGetTickCount());
  // start_game();
  sj2_cli__init();
  puts("Starting RTOS");
  vTaskStartScheduler(); // This function never returns unless RTOS scheduler runs out of memory and fails
  return 0;
}
