#pragma once

#include "adc.h"
#include "gpio.h"
#include <stdbool.h>
#include <stdint.h>

/*
Joystick values
*/

typedef struct {
  uint16_t s_x;
  uint16_t s_y;
  bool switch_pressed;
} j_value;

gpio_s enable_joystick(gpio__port_e adc1, uint8_t x_value, gpio__port_e adc2, uint8_t y_value, gpio__port_e port,
                       int pin);

j_value get_data(adc_channel_e x_port, adc_channel_e y_port, gpio_s button_press);

void get_joystick_data();