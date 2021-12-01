#include "joystick.h"
#include "adc.h"
#include "gpio.h"
#include "lpc40xx.h"
#include <stdio.h>

gpio_s p1 = {1, 31};
gpio_s adc_xvalue;
gpio_s adc_yvalue;
adc_channel_e x_port = ADC__CHANNEL_2;
adc_channel_e y_port = ADC__CHANNEL_4;
gpio_s button_press;
void enable_joystick(gpio__port_e adc1, uint8_t x_pin, gpio__port_e adc2, uint8_t y_pin, gpio__port_e port, int pin) {
  const uint32_t set_analog_mode = (1 << 7);

  LPC_IOCON->P1_30 &= ~(set_analog_mode);
  LPC_IOCON->P0_25 &= ~(set_analog_mode);

  adc_xvalue = gpio__construct_with_function(adc1, x_pin, 1);
  adc_yvalue = gpio__construct_with_function(adc2, y_pin, 3);
  adc__initialize();
}

joystick_value get_joystick_data() {
  joystick_value joystick;
  joystick.s_x = adc__get_adc_value(x_port);
  joystick.s_y = adc__get_adc_value(y_port);

  if (gpio__get(button_press)) {
    joystick.switch_pressed = true;
  } else {
    joystick.switch_pressed = false;
  }
  //  fprintf(stderr, "Joystick Data x= %d, Joystick Data y = %d", joystick.s_x, joystick.s_y);
  return joystick;
}

void initialize_joystick() {
  enable_joystick(GPIO__PORT_0, 25, GPIO__PORT_1, 30, GPIO__PORT_1, 31);
  button_press = p1;
}
