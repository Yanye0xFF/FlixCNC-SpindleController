#ifndef _HC89S_SOFTI2C_H_
#define _HC89S_SOFTI2C_H_

#include "HC89S003F4.h"
#include "hc89s_exit.h"
// I2C_SCLK P00 INT0
// I2C_SDA  P01 INT1

void softi2c_pinmux_setup();

void softi2c_int_config();

#endif
