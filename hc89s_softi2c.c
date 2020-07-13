#include "hc89s_softi2c.h"

void softi2c_pinmux_setup() {
    // P00-SCL带上拉SMT输入 P01-SDA 带上拉SMT输入
    P0M0 = 0x66;
}

void softi2c_int_config() {
    // INT0(SCL)上升沿触发 INT1(SDA) 上升下降沿触发
     PITS0 |= 0xE;
    // INT端口选择 INT0->P00 INT1->P01
    INT01_PINS = 0x00;
    
    EXTI_ITPriorityConfig(INT0, EXTI_ITPriority_2);
    EXTI_ITPriorityConfig(INT1, EXTI_ITPriority_3);
    
    EXTI_ClearFlag(INT0);
    EXTI_ClearFlag(INT1);
    
    EXTI_Cmd(INT0, ENABLE);
    EXTI_Cmd(INT1, ENABLE);
}
