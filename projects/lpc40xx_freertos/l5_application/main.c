#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FreeRTOS.h"
#include "acceleration.h"
#include "board_io.h"
#include "common_macros.h"
#include "event_groups.h"
#include "ff.h"
#include "gpio.h"
#include "lpc40xx.h"
#include "lpc_peripherals.h"
#include "periodic_scheduler.h"
#include "semphr.h"
#include "sj2_cli.h"
#include "task.h"
#define MAX_SIZE 20

#define PRODUCER_WAIT_BITS (1 << 0)
#define CONSUMER_WAIT_BITS (1 << 1)
#define WATCHDOG_FIRED_BOTH_TASKS 0
#define WATCHDOG_FIRED_PRODUCER 1
#define WATCHDOG_FIRED_CONSUMER 2

static SemaphoreHandle_t file_mutex;
static EventGroupHandle_t event_group;
static QueueHandle_t accelerator_queue;
static acceleration__axis_data_s consumer_sensor_value[100];
static const char *filename = "sensor.txt";

void write_watchdog_error_in_file(int event) {
  FIL file; // File handle
  UINT bytes_written = 0;
  FRESULT result = f_open(&file, filename, (FA_WRITE | FA_OPEN_APPEND));

  if (FR_OK == result) {
    char string[100];
    if (event == WATCHDOG_FIRED_BOTH_TASKS) {
      sprintf(string, "Time %li: WatchDog Fired for both Producer and Consumer\n", xTaskGetTickCount());
    } else if (event == WATCHDOG_FIRED_PRODUCER) {
      sprintf(string, "Time %li: WatchDog Fired for both Producer\n", xTaskGetTickCount());

    } else if (event == WATCHDOG_FIRED_CONSUMER) {
      sprintf(string, "Time %li: WatchDog Fired for both Consumer\n", xTaskGetTickCount());
    }
    (void)xSemaphoreTake(file_mutex, portMAX_DELAY);
    if (FR_OK == f_write(&file, string, strlen(string), &bytes_written)) {
    } else {
      printf("ERROR: Failed to write data to file\n");
    }
    xSemaphoreGive(file_mutex);

    f_close(&file);
  } else {
    printf("Failed to open file\n");
  }
}

void write_file_using_fatfs_pi(acceleration__axis_data_s *val) {

  FIL file; // File handle
  UINT bytes_written = 0;
  FRESULT result = f_open(&file, filename, (FA_WRITE | FA_OPEN_APPEND));

  if (FR_OK == result) {
    char string[256];
    int offset = 0;
    for (int i = 0; i < MAX_SIZE; i++) {
      sprintf(string + offset, "Time %li, x:%i x:%i x:%i\n", xTaskGetTickCount(), val[i].x, val[i].y, val[i].z);
      offset = strlen(string);
    }

    (void)xSemaphoreTake(file_mutex, portMAX_DELAY);
    if (FR_OK == f_write(&file, string, strlen(string), &bytes_written)) {
    } else {
      printf("ERROR: Failed to write data to file\n");
    }
    xSemaphoreGive(file_mutex);

    f_close(&file);
  } else {
    printf("ERROR: Failed to open: %s\n", filename);
  }
}

void producer_of_accelerator_sensor(void *p) {

  while (1) {
    int i = 0;
    acceleration__axis_data_s sensor_value;
    acceleration__axis_data_s result = {0};

    sensor_value = acceleration__get_data();

    for (i = 0; i < 100; i++) {

      result.x = result.x + sensor_value.x;
      result.y = result.x + sensor_value.y;
      result.z = result.x + sensor_value.z;
    }

    sensor_value.x = result.x / 100;
    sensor_value.y = result.y / 100;
    sensor_value.z = result.z / 100;

    xQueueSend(accelerator_queue, &sensor_value, 0);
    //  printf("Sent: %i, %i, %i\n", sensor_value.x, sensor_value.y, sensor_value.z);

    i = 0;
    result.x = 0;
    result.y = 0;
    result.z = 0;
    vTaskDelay(100);
    xEventGroupSetBits(event_group, PRODUCER_WAIT_BITS);
  }
}

void consumer_of_accelerator_sensor(void *p) {

  while (1) {

    acceleration__axis_data_s accelerator_sensor_value;
    static int i = 0;
    if (!xQueueReceive(accelerator_queue, &accelerator_sensor_value, portMAX_DELAY))
      ;
    //    printf("Received: %i %i %i\n", accelerator_sensor_value.x, accelerator_sensor_value.y,
    //    accelerator_sensor_value.z);
    consumer_sensor_value[i] = accelerator_sensor_value;

    if (i == MAX_SIZE) {
      i = 0;
      //      write_file_using_fatfs_pi(consumer_sensor_value);
    }
    i++;
    xEventGroupSetBits(event_group, CONSUMER_WAIT_BITS);
  }
}

void watchdog_task(void *params) {
  EventBits_t watch_dog_event_bits;
  while (1) {
    watch_dog_event_bits =
        xEventGroupWaitBits(event_group, PRODUCER_WAIT_BITS | CONSUMER_WAIT_BITS, pdTRUE, pdTRUE, 200);
    if ((watch_dog_event_bits & (PRODUCER_WAIT_BITS | CONSUMER_WAIT_BITS)) == 0) {
      // log both of them failed
      printf("WatchDog fired for both Consumer and Producer Tasks\n");
      write_watchdog_error_in_file(WATCHDOG_FIRED_BOTH_TASKS);
    } else if ((watch_dog_event_bits & PRODUCER_WAIT_BITS) == 0) {
      // log producer hit watchdog
      printf("WatchDog fired for Producer Task\n");
      write_watchdog_error_in_file(WATCHDOG_FIRED_PRODUCER);
    } else if ((watch_dog_event_bits & CONSUMER_WAIT_BITS) == 0) {
      // log consumer hit watchdog
      printf("WatchDog fired for Consumer Task\n");
      write_watchdog_error_in_file(WATCHDOG_FIRED_CONSUMER);
    } else {
    }
    vTaskDelay(1000);
  }
}

int main(void) {
  acceleration__init();
  event_group = xEventGroupCreate();

  file_mutex = xSemaphoreCreateMutex();
  accelerator_queue = xQueueCreate(MAX_SIZE, sizeof(acceleration__axis_data_s));

  xTaskCreate(producer_of_accelerator_sensor, "Producer", 2048 / sizeof(void *), NULL, PRIORITY_MEDIUM, NULL);
  xTaskCreate(consumer_of_accelerator_sensor, "Consumer", 2048 / sizeof(void *), NULL, PRIORITY_MEDIUM, NULL);
  xTaskCreate(watchdog_task, "WatchDog", 2048 / sizeof(void *), NULL, PRIORITY_HIGH, NULL);

  puts("Starting RTOS");
  sj2_cli__init();
  vTaskStartScheduler(); // This function never returns unless RTOS scheduler runs out of memory and fails

  return 0;
}

// This sends periodic messages over printf() which uses system_calls.c to send them to UART0
