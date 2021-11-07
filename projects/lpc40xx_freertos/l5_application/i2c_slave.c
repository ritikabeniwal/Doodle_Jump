#include "i2c_slave.h"
#include <i2c.h>
#include <lpc40xx.h>

const uint32_t i2c2_slave_addr = 0x80;

void i2c2_slave_init() {
  uint8_t slave_addr_shift = 1;
  uint8_t ack_bit = 2;
  // LPC_I2C2->ADR2 = i2c2_slave_addr << slave_addr_shift;
  LPC_I2C2->ADR2 = i2c2_slave_addr;
  LPC_I2C2->CONSET |= 1 << ack_bit;
}
