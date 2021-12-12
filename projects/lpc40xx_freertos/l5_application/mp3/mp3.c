#include "mp3.h"
#include "FreeRTOS.h"
#include "gpio.h"
#include "task.h"
#include "uart.h"

static uint8_t mp3_uart_buffer[8];

void mp3__send_command(uint8_t command, uint8_t data_1, uint8_t data_2) {
  mp3_uart_buffer[0] = 0x7e;
  mp3_uart_buffer[1] = 0xff;
  mp3_uart_buffer[2] = 0x06;
  mp3_uart_buffer[3] = command;
  mp3_uart_buffer[4] = 0x00;
  mp3_uart_buffer[5] = data_1;
  mp3_uart_buffer[6] = data_2;
  mp3_uart_buffer[7] = 0xef;

  for (uint8_t i = 0; i < 8; i++) {
    uart__polled_put(UART__3, mp3_uart_buffer[i]);
  }
}

void mp3__init() {
  int mp3_baud_rate = 9600;
  uart__init(UART__3, clock__get_peripheral_clock_hz(), mp3_baud_rate);
  gpio__construct_with_function(GPIO__PORT_4, 28, GPIO__FUNCTION_2);
  gpio__construct_with_function(GPIO__PORT_4, 29, GPIO__FUNCTION_2);
}

void mp3_play() {
  mp3__send_command(0x09, 0x00, 0x02);
  mp3__send_command(0x03, 0x00, 0x02);
}

void create_mp3_task() {
  mp3__init();
  xTaskCreate(mp3_play, "MP3", 2048 / sizeof(void *), NULL, PRIORITY_LOW, NULL);
}
