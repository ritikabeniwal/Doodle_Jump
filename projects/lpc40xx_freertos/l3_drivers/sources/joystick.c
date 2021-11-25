#include "joystick.h"
#include "adc.h"
#include "gpio.h"
#include "lpc40xx.h"
#include <stdio.h>

gpio_s p1 = {1, 31};
gpio_s adc_xvalue;
gpio_s adc_yvalue;

gpio_s enable_joystick(gpio__port_e adc1, uint8_t x_pin, gpio__port_e adc2, uint8_t y_pin, gpio__port_e port, int pin) {
  const uint32_t set_analog_mode = (1 << 7);

  LPC_IOCON->P1_30 &= ~(set_analog_mode);
  LPC_IOCON->P0_25 &= ~(set_analog_mode);

  adc_xvalue = gpio__construct_with_function(adc1, x_pin, 1);
  adc_yvalue = gpio__construct_with_function(adc2, y_pin, 3);
  adc__initialize();
  return p1;
}

j_value get_data(adc_channel_e x_port, adc_channel_e y_port, gpio_s button_press) {
  j_value joystick;
  joystick.s_x = adc__get_adc_value(x_port);
  joystick.s_y = adc__get_adc_value(y_port);

  if (gpio__get(button_press)) {
    joystick.switch_pressed = true;
  } else {
    joystick.switch_pressed = false;
  }

  return joystick;
}

void get_joystick_data() {
  enable_joystick(GPIO__PORT_0, 25, GPIO__PORT_1, 30, GPIO__PORT_1, 31);
  j_value joystick_val = get_data(ADC__CHANNEL_2, ADC__CHANNEL_4, p1);
  printf("X axis : %d,\tY axis : %d, \tswitch : %d\n", joystick_val.s_x, joystick_val.s_y, joystick_val.switch_pressed);
}