#ifndef _MAIN_H_
#define _MAIN_H_

#define	ALLOCATE_EXTERN

#include "HC89S003F4.h"
#include "hc89s_timer3.h"
#include "hc89s_timer0.h"
#include "hc89s_pwmgroup.h"
#include "hc89s_gpio.h"
#include "hc89s_softi2c.h"

#include "stdint.h"
#include "string.h"

#define GPIO_PIN_HIGH  1
#define GPIO_PIN_LOW   0

#define I2C_SCL        P0_0
#define I2C_SDA        P0_1
#define BEEP_GPIO      P2_0

// PSW寄存器的F0自定义位做I2C状态机
#define I2C_STATE      F0
#define I2C_START      0
#define I2C_STOP       1
// PSW寄存器的F1自定义位作为缓冲区变化标识
#define BUFFER_CHANGED    F1

// i2c从机地址0x7F，仅接收主机的写操作,因此首字节为0xFE
#define SLAVER_WRITE         0xFE

#define SET_PARAM_CMD        0x00
#define SET_DUTY_CMD         0x01
#define SET_VSS_TIME_CMD     0x02
#define SET_SPINDLE_DIR_CMD  0x03
#define SET_TONE_TICK_CMD    0x04
#define SET_SPEEDUP_CMD      0x05
#define SET_SLOWDOWN_CMD     0x06
#define OPEN_SPINDLE_CMD     0x07
#define CLOSE_SPINDLE_CMD    0x08
#define MAKE_TONE_CMD        0x09
#define SWITCH_LED_CMD       0x0A

// PWM周期 T=500us，F=2KHz
#define PWM_CYCLE            500
#define DUTY_DELTA_DEFAULT   5
#define DUTY_MIN_DEFAULT     700
#define DUTY_MAX_DEFAULT     900
#define VSS_TIME_DEFAULT     500
#define VSS_LOW_LIMIT        0
#define VSS_HIGH_LIMIT       (1048+1)

#define SPINDLE_DIR_DEFAULT       (0x3)
#define TONE_TICK_DEFAULT          (1)

#define DUTY_SEG_DEFAULT     15
#define TIME_SEG_DEFAULT     100

#endif

