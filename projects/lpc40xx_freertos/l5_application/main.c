#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "board_io.h"
#include "common_macros.h"
#include "game_start_stop.h"
#include "led_matrix_driver.h"
#include "led_matrix_tasks.h"
#include "periodic_scheduler.h"
#include "sj2_cli.h"

int main(void) {
  create_led_matrix_tasks();
  create_start_stop_task();
  // start_game();
  sj2_cli__init();
  puts("Starting RTOS");
  vTaskStartScheduler(); // This function never returns unless RTOS scheduler runs out of memory and fails
  return 0;
}
