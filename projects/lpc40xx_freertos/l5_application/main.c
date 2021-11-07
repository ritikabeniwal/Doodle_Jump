#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "board_io.h"
#include "common_macros.h"
#include "i2c_slave.h"
#include "periodic_scheduler.h"
#include "sj2_cli.h"

#define MAX_SLAVE_MEMORY 100

static volatile uint8_t slave_memory[MAX_SLAVE_MEMORY];

bool i2c_slave_callback__read_memory(uint8_t memory_index, uint8_t *memory) {
  if (memory_index < MAX_SLAVE_MEMORY) {
    *memory = slave_memory[memory_index];
    return true;
  }
  return false;
}

bool i2c_slave_callback__write_memory(uint8_t memory_index, uint8_t memory_value) {
  if (memory_index < MAX_SLAVE_MEMORY) {
    slave_memory[memory_index] = memory_value;
    return true;
  }
  return false;
}

int main(void) {
  i2c2_slave_init();
  while (1) {
    if (slave_memory[0] == 0) {
      gpio__set(board_io__get_led0());
    } else {
      gpio__reset(board_io__get_led0());
    }
  }
  return 0;
}
