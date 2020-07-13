#include "hc89s_softi2c.h"

void softi2c_pinmux_setup() {
    // P00-SCL������SMT���� P01-SDA ������SMT����
    P0M0 = 0x66;
}

void softi2c_int_config() {
    // INT0(SCL)�����ش��� INT1(SDA) �����½��ش���
     PITS0 |= 0xE;
    // INT�˿�ѡ�� INT0->P00 INT1->P01
    INT01_PINS = 0x00;
    
    EXTI_ITPriorityConfig(INT0, EXTI_ITPriority_2);
    EXTI_ITPriorityConfig(INT1, EXTI_ITPriority_3);
    
    EXTI_ClearFlag(INT0);
    EXTI_ClearFlag(INT1);
    
    EXTI_Cmd(INT0, ENABLE);
    EXTI_Cmd(INT1, ENABLE);
}
