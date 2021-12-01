
#pragma once

#include <stdbool.h>
#include <stdint.h>

#define NEXT_SONG 0x01
#define PREV_SONG 0x02
#define INDEX_SONG 0x03
#define VOL_UP 0X04
#define VOL_DOWN 0x05
#define SET_VOL 0x06
#define SINGLE_CYCLE_PLAY 0x08
#define SELECT_DEVICE 0x09

#define STOP 0x16
#define CYCLE_PLAY_FOLDER 0x17
#define CYCLE_PLAY_FILE 0x19
#define SET_DAC 0x1a
#define PLAY_WITH_VOL 0x22

#define SLEEP 0x0a
#define WAKE_UP 0x0b
#define RESET 0x0c
#define PLAY 0x0d
#define PAUSE 0x0e
#define PLAY_FOLDER_OR_FILE 0x0f

#define DEV_TF 0x02

void mp3__init();
void mp3__send_command(uint8_t command, uint8_t data_1, uint8_t data_2);

void mp3_task();