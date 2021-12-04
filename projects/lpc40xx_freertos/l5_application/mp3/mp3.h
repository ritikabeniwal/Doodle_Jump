#pragma once

#include <stdbool.h>
#include <stdint.h>

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
static int PLAY = 0x0d;
static int PAUSE = 0x0e;
static int PLAY_FOLDER_OR_FILE = 0x0f;

static int DEV_TF = 0x02;

void create_mp3_task();
void mp3__init();
void mp3__send_command(uint8_t command, uint8_t data_1, uint8_t data_2);
void mp3_play();
