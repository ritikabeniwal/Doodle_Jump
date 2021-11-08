#include <stdio.h>
#include <time.h>

#include "FreeRTOS.h"
#include "task.h"

#include "board_io.h"
#include "common_macros.h"
#include "led_matrix_driver.h"
#include "led_matrix_tasks.h"
#include "periodic_scheduler.h"
#include "sj2_cli.h"

static void display_task() {
  while (1) {
    led_matrix__update_display();
    vTaskDelay(10);
  }
}

void create_led_matrix_tasks(void) {
  led_matrix_init();
  xTaskCreate(display_task, "display_task", (512U * 8) / sizeof(void *), NULL, PRIORITY_LOW, NULL);
}
