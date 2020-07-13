#include "main.h"

// I2C_SCLK P00 INT0
// I2C_SDA  P01 INT1
// i2c���ջ�������0�ֽ�Ϊ����Ѱַ�㲥��1~32������
idata uint8_t i2c_buffer[33];
data uint8_t i2c_buffer_cursor = 0;
data uint8_t i2c_byte = 0x00;
data uint8_t i2c_tick = 0;

xdata uint16_t spindle_duty_min = DUTY_MIN_DEFAULT;
xdata uint16_t spindle_duty_max = DUTY_MAX_DEFAULT;
xdata uint16_t vss_time = VSS_TIME_DEFAULT;

xdata uint8_t tone_tick = TONE_TICK_DEFAULT;
// ����̵����ߵ�ƽ��Ч��Ĭ�Ϲر�
xdata uint8_t spindle_dir = SPINDLE_DIR_DEFAULT;

xdata uint16_t speedup_duty_seg = DUTY_SEG_DEFAULT, speedup_time_seg = TIME_SEG_DEFAULT;
xdata uint16_t slowdown_duty_seg = DUTY_SEG_DEFAULT, slowdown_time_seg = TIME_SEG_DEFAULT;

data int8_t delta = 0;
data int16_t pwm_duty = 0;
data uint8_t systemState = 0;
data uint8_t DelayFlag = FALSE;

uint16_t calcPWMPeriod(uint16_t microseconds);
uint16_t calcTimer3Count(uint16_t miliseconds);

void timerDelay(uint16_t microseconds);
void sysDelay(uint16_t miliseconds);

void playTone(uint8_t tick);

void main() {
    uint16_t pwm_period;
    
    // �ؿ��Ź�
    WDTCCR = 0x00;
    // �ȴ��ڲ���Ƶ��������
	while((CLKCON & 0x20) != 0x20);		
    // ѡ���ڲ���ƵRCΪϵͳʱ�ӣ��ڲ���ƵRC2��Ƶ��Fosc=16MHz	
	CLKSWR = 0x51;			
    // �ȴ��ڲ���Ƶ�л����    
    while((CLKSWR & 0xC0) != 0x40);		
    // Fosc 1��Ƶ�õ�Fcpu��Fcpu=16MHz 	
	CLKDIV = 0x01;
    // I2Cֹͣ״̬
    I2C_STATE = I2C_STOP;
    BUFFER_CHANGED = FALSE;
    
    EA_Enable();
    
    // ���÷�����P20 �������
    P2M0 = P2M0 & 0xF0 | 0x08;	
    P2 &= 0xFE;
    // ����״ָ̬ʾ��P05,�͵�ƽ��Ч
    GPIO_Init(GPIOT0, GPIO_PIN_5, GPIO_MODE_OUT_PP);
    GPIO_WriteHigh(GPIOT0, GPIO_PIN_5);
    
    // ���᷽�����P02 P03,�͵�ƽ��Ч,Ĭ�Ϲر�
    GPIO_Init(GPIOT0, GPIO_PIN_2, GPIO_MODE_OUT_PP);
    GPIO_Init(GPIOT0, GPIO_PIN_3, GPIO_MODE_OUT_PP);
    GPIO_WriteHigh(GPIOT0, GPIO_PIN_2);
    GPIO_WriteHigh(GPIOT0, GPIO_PIN_3);
    
    // T0��ʱ������ʱʱ��, ���ȼ����(0),ʱ��ԴFosc=16M������Ƶ
    TIM0_Init(TIM0_CLK_1, TIM0_SOFT_CONTROL, TIM0_INTIMING, TIM0_16BIT_CNT_TIMING);
    TIM0_ITPriorityConfig(TIM3_ITPriority_0);
    TIM0_ClearITFlag();
    TIM0_ITCmd(ENABLE);
    TIM0_OutPutCmd(DISABLE);
    TIM0_Cmd(DISABLE);
    
    // TIM3������VSS��ʱ�������ȼ�1, ʱ��ԴFosc=16M��256��Ƶ
    TIM3_Init(TIM3_SYSCLK, TIM3_CLK_256, DISABLE);
    TIM3_ITPriorityConfig(TIM0_ITPriority_1);
    TIM3_PDModeCmd(DISABLE);
    TIM3_ClearITFlag();
    TIM3_ITCmd(ENABLE);
    TIM3_Cmd(DISABLE);
    
    // ����ģ��I2C���ź��ж�
    softi2c_pinmux_setup();
    softi2c_int_config();
    
    // ����P04 PWM�������ģʽ
    GPIO_Init(GPIOT0, GPIO_PIN_4, GPIO_MODE_OUT_PP);
    GPIO_WriteLow(GPIOT0, GPIO_PIN_4);
        
    PWM_Init(PWM0_Group, PWMCK_8, PWMX_PWX1_HIGH_VALID, INDEPENDENT_MODE);
    // �ر�PWM������
    PWM_FLTConfig(PWM0_Group, FLFPIN_LOW_PWMOFF, PWMX_PWX1_ALL_LOW, DISABLE);
    // ������ر�PWM�ж�
    PWM_ClearITFlag(PWM0_Group);
    PWM_ITCmd(PWM0_Group, DISABLE);
     // ӳ��P04ΪPWM���
    PWM_PinRemapConfig(PWM_0, PWM_OUTPin_P04);
    
    // ���ó�ʼƵ�ʺ�ռ�ձ�
    pwm_period = calcPWMPeriod(PWM_CYCLE);
    PWM_PeriodConfig(PWM0_Group, pwm_period);
    PWM_DutyCycleConfig(PWM0_Group, 0);
    
    while(1) {
        if(BUFFER_CHANGED) {
            // check tag
            if(i2c_buffer[0] != SLAVER_WRITE || i2c_buffer_cursor == 0) {
                break;
            }
            
            switch(i2c_buffer[1]) {
                case SET_PARAM_CMD:
                    // ���������ֽ�С��Ϊ׼
                    spindle_duty_min = (i2c_buffer[3] << 8) | i2c_buffer[2];
                    spindle_duty_max = (i2c_buffer[5] << 8) | i2c_buffer[4];
                    vss_time = (i2c_buffer[7] << 8) | i2c_buffer[6];
                    tone_tick = i2c_buffer[9];
                    spindle_dir = i2c_buffer[8];
                    break;
                case SET_DUTY_CMD:
                    spindle_duty_min = (i2c_buffer[3] << 8) | i2c_buffer[2];
                    spindle_duty_max = (i2c_buffer[5] << 8) | i2c_buffer[4];
                    break;
                case SET_VSS_TIME_CMD:
                    vss_time = (i2c_buffer[3] << 8) | i2c_buffer[2];
                    if((systemState & 0x1) && (i2c_buffer[4] & 0x1)) {
                        TIM3_Cmd(DISABLE);
                        TIM3_TimerCnt(calcTimer3Count(vss_time));
                        TIM3_Cmd(ENABLE);
                    }
                    break;
                case SET_SPINDLE_DIR_CMD:
                    if(!(systemState & 0x1)) {
                        spindle_dir = i2c_buffer[2];
                        P0_2 = (spindle_dir & 0x1);
                        P0_3 = ((spindle_dir >> 1)  & 0x1);
                    }
                    break;
                case SET_TONE_TICK_CMD:
                    tone_tick = i2c_buffer[2];
                    break;
                case SET_SPEEDUP_CMD:
                    speedup_duty_seg = (i2c_buffer[3] << 8) | i2c_buffer[2];
                    speedup_time_seg = (i2c_buffer[5] << 8) | i2c_buffer[4];
                    break;
                case SET_SLOWDOWN_CMD:
                    slowdown_duty_seg = (i2c_buffer[3] << 8) | i2c_buffer[2];
                    slowdown_time_seg = (i2c_buffer[5] << 8) | i2c_buffer[4];
                    break;
                case OPEN_SPINDLE_CMD:
                    // δ���÷�����������
                    if((spindle_dir & 0x1) && ((spindle_dir >> 1)  & 0x1)) {
                        break;
                    }
                    GPIO_WriteLow(GPIOT0, GPIO_PIN_5);
                    if(i2c_buffer[2]) {
                        playTone(tone_tick);
                    }
                    // ����״̬λ
                    systemState |= 0x1;
                    // �����᷽��
                    P0_2 = (spindle_dir & 0x1);
                    P0_3 = ((spindle_dir >> 1)  & 0x1);
                    // ��PMW�������
                    pwm_duty = 0;
                    PWM_OutPutCmd(PWM_0, ENABLE);
                    PWM_Cmd(PWM0_Group, ENABLE);
                    // ֱ�߼��ٵ����ת�ٹ���
                    while(pwm_duty < spindle_duty_min) {
                        pwm_duty += speedup_duty_seg;
                        pwm_duty = (pwm_duty > spindle_duty_min) ? spindle_duty_min : pwm_duty;
                        PWM_DutyCycleConfig(PWM0_Group, pwm_duty);
                        sysDelay(speedup_time_seg);
                    }
                    // ��ʾ��
                    playTone(1);
                    
                    if(vss_time > VSS_LOW_LIMIT && vss_time < VSS_HIGH_LIMIT) {
                        // ��̬ת�ٶ�ʱ��
                        TIM3_TimerCnt(calcTimer3Count(vss_time));
                        TIM3_Cmd(ENABLE);
                    }else {
                        // ������VSS,ֱ�߼��ٵ����ת��
                        while(pwm_duty < spindle_duty_max) {
                            pwm_duty += speedup_duty_seg;
                            pwm_duty = (pwm_duty > spindle_duty_max) ? spindle_duty_max : pwm_duty;
                            PWM_DutyCycleConfig(PWM0_Group, pwm_duty);
                            sysDelay(speedup_time_seg);
                        }
                        playTone(1);
                    }
                    break;
                case CLOSE_SPINDLE_CMD:
                    // �ر�VSS��ʱ��
                    TIM3_Cmd(DISABLE);
                    // ���ٵ�0����    
                    while(pwm_duty > 0) {
                        pwm_duty -= slowdown_duty_seg;
                        pwm_duty = (pwm_duty < 0) ? 0 : pwm_duty;
                        PWM_DutyCycleConfig(PWM0_Group, pwm_duty);
                        sysDelay(slowdown_time_seg);
                    }
                    PWM_OutPutCmd(PWM_0, DISABLE);
                    PWM_Cmd(PWM0_Group, DISABLE);
                    // �ر�����̵���
                    GPIO_WriteHigh(GPIOT0, GPIO_PIN_2);
                    GPIO_WriteHigh(GPIOT0, GPIO_PIN_3);
                    // �ر�����״ָ̬ʾ��
                    GPIO_WriteHigh(GPIOT0, GPIO_PIN_5);
                    // ����״̬λ
                    systemState &= (~0x1);
                    if(i2c_buffer[2]) {
                        playTone(tone_tick);
                    }
                    break;
                case MAKE_TONE_CMD:
                     // ����������
                    playTone(i2c_buffer[2]);
                    break;
                case SWITCH_LED_CMD:
                    P0_5 = (i2c_buffer[2] & 0x1);
                    break;
                default:
                    break;
            }
            i2c_buffer_cursor = 0;
            BUFFER_CHANGED = FALSE;
        }
    }
}

/*
��������Ƶ��׼A��
@param tick ��������
*/
void playTone(uint8_t tick) {
    uint8_t i, j;
    for(j = 0; j < tick; j++) {
        for(i = 0; i < 0xFE; i++) {
            BEEP_GPIO = GPIO_PIN_HIGH;
            timerDelay(681);
            BEEP_GPIO = GPIO_PIN_LOW;
            timerDelay(1590);
        }
        sysDelay(500);
    }
}

/*
����PWM���ڣ�PWMxPΪ12λ(0~0xFFF),ʱ��16M��8��Ƶ
@param microseconds ����ʱ��(΢��) ��Χ0~2047us
*/
uint16_t calcPWMPeriod(uint16_t microseconds) {
    return (microseconds << 1) & 0xFFF;
}

/*
����timer3(4/3Mhz)��ֵ
��ʱʱ�䷶Χ0~1048ms
*/
uint16_t calcTimer3Count(uint16_t miliseconds) {
    uint16_t count = 62 * miliseconds;
    miliseconds = (miliseconds & 0x1) ? ((miliseconds - 1)) : miliseconds;
    count += (miliseconds >> 1);
    return (0xFFFF - count);
}

// ���ȼ�0
void TIMER0_ISR(void) interrupt TIMER0_VECTOR {
    TIM0_Cmd(DISABLE);
    TIM0_ClearITFlag();
    DelayFlag = FALSE;
}

// ���ȼ�1
void TIMER3_ISR(void) interrupt T3_VECTOR{
    T3CON &=~ TIM3_TF3;
    if(pwm_duty >= spindle_duty_max) {
        delta = -DUTY_DELTA_DEFAULT;
    }else if(pwm_duty <= spindle_duty_min){
        delta = +DUTY_DELTA_DEFAULT;
    }
    pwm_duty += delta;
    PWM_DutyCycleConfig(PWM0_Group, pwm_duty);
}


// ���ȼ�3 SDA P01
void SDA_ISR() interrupt INT1_VECTOR {
    // ����ж�
    PINTF0 &= 0xFD;
    // I2C�����ź�
    if(!(I2C_SCL)) {
        return;
    }
    // ��ʼ����ֹACK�����ֽڻ������
    i2c_tick = 0;
    i2c_byte = 0x00;
    // START = SDA�½��� & SCL�ߵ�ƽ
    // STOP = SDA ������ & SCL�ߵ�ƽ  
    if(!I2C_SDA && I2C_STATE == I2C_STOP) {
        I2C_STATE = I2C_START;
        i2c_buffer_cursor = 0;
    }else {
        I2C_STATE = I2C_STOP;
        BUFFER_CHANGED = TRUE;
    }
}

// ���ȼ�2 SCL P00
void SCL_ISR() interrupt INT0_VECTOR {
    // ����ж�
    PINTF0 &= 0xFE;
    // I2C����(SDA)ֻ������SCL�͵�ƽ�����仯
    // I2C_STATE == I2C_STOP
    if(I2C_STATE) {
        return;
    }
    // i2c_tick�ı�֮ǰ���˴�Ϊ��9��ʱ��
    if(i2c_tick == 8) {
        // �ڵ�9��ʱ�ӷ���ACK�źţ���������~4.4us
        // ����(SDA)P01 PD���
        P0M0 = P0M0 & 0x0F | 0x80;
        // ����SDA ����ACK�ź�
        I2C_SDA = 0;
    }

    // ��������,��������~6.6us
    i2c_byte |= ((uint8_t)I2C_SDA << ((~i2c_tick) & 0x7));
    i2c_tick++;
    
    if(i2c_tick == 8) {
        // �ڵ�8��ʱ�Ӹ�������
        i2c_buffer[i2c_buffer_cursor++] = i2c_byte;
        // ��SCL_ISR
        EX1 = 0;
    }else if(i2c_tick == 9) {
        // �����ջ����ֽ�
        i2c_byte = 0x00;
        i2c_tick = 0;
        // ���SCL_ISR���´�
        PINTF0 &= 0xFD;
        EX1 = 1;
        // �ָ�P00 P01 SMT��������
        P0M0 = 0x66;
    }
}

/*
��ʱ��0(16MHz)��ʱ���������ÿ���9.75~10us
@param microseconds ΢�� ʵ����ʱʱ��[0-4095]us + 10us
*/
void timerDelay(uint16_t microseconds) {
    DelayFlag =  TRUE;
    TIM0_TimerCnt((0xFFFF - (microseconds << 4)));
    TIM0_Cmd(ENABLE);
    while(DelayFlag);
}

/*
�����ʱ�����0.1ms
@param miliseconds ����(0~0xFFFF)
*/
void sysDelay(uint16_t miliseconds) {
    uint16_t i;
    for(; miliseconds > 0; miliseconds--) {
         for(i = 1596; i > 0; i--);
    }
}
