#include "mp3.h"
#include "FreeRTOS.h"
#include "gpio.h"
#include "mp3.h"
#include "task.h"
#include "uart.h"

static uint8_t mp3_uart_buffer[8];
static int NEXT_SONG = 0x01;
static int PREV_SONG = 0x02;
static int INDEX_SONG = 0x03;
static int VOL_UP = 0X04;
static int VOL_DOWN = 0x05;
static int SET_VOL = 0x06;
static int SINGLE_CYCLE_PLAY = 0x08;
static int SELECT_DEVICE = 0x09;

static int STOP = 0x16;
static int CYCLE_PLAY_FOLDER = 0x17;
static int CYCLE_PLAY_FILE = 0x19;
static int SET_DAC = 0x1a;
static int PLAY_WITH_VOL = 0x22;

static int SLEEP = 0x0a;
static int WAKE_UP = 0x0b;
static int RESET = 0x0c;
static int RESUME_PLAYBACK = 0x0d;
static int PAUSE = 0x0e;
static int PLAY_FILE_WITH_FOLDER = 0x0f;

static int DEV_TF = 0x02;

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
  mp3__send_command(SELECT_DEVICE, 0x00, DEV_TF);
  mp3_play_start_song();
}

void mp3_play(int num) { mp3__send_command(CYCLE_PLAY_FOLDER, num, 0x02); }

void mp3_play_sound(int num) { mp3__send_command(PLAY_FILE_WITH_FOLDER, 0x02, num); }

void mp3_play_jump_up_sound() {
  mp3_play_sound(1);
  vTaskDelay(50);
}

void mp3_play_spring_sound() {
  mp3_play_sound(2);
  vTaskDelay(50);
}

void mp3_play_collision_with_enemy_sound() {
  mp3_play_sound(5);
  vTaskDelay(70);
}

void mp3_play_shoot_gun_sound() {
  mp3_play_sound(4);
  vTaskDelay(100);
}

void mp3_play_monster_chomp_sound() {
  mp3_play_sound(6);
  vTaskDelay(10);
}

void mp3_play_game_over_song() { mp3_play_sound(7); }

void mp3_play_level1_song() { mp3_play(1); }
void mp3_play_level2_song() {
  mp3__send_command(SET_VOL, 0x00, 0x1E);
  mp3_play(3);
}
void mp3_play_start_song() { mp3_play(1); }

void create_mp3_task() {
  mp3__init();
  mp3__send_command(SELECT_DEVICE, 0x00, DEV_TF);
  xTaskCreate(mp3_play, "MP3", 2048 / sizeof(void *), NULL, PRIORITY_HIGH, NULL);
}
