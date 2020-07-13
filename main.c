#include "main.h"

// I2C_SCLK P00 INT0
// I2C_SDA  P01 INT1
// i2c接收缓冲区，0字节为主机寻址广播，1~32数据区
idata uint8_t i2c_buffer[33];
data uint8_t i2c_buffer_cursor = 0;
data uint8_t i2c_byte = 0x00;
data uint8_t i2c_tick = 0;

xdata uint16_t spindle_duty_min = DUTY_MIN_DEFAULT;
xdata uint16_t spindle_duty_max = DUTY_MAX_DEFAULT;
xdata uint16_t vss_time = VSS_TIME_DEFAULT;

xdata uint8_t tone_tick = TONE_TICK_DEFAULT;
// 主轴继电器高电平有效，默认关闭
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
    
    // 关看门狗
    WDTCCR = 0x00;
    // 等待内部高频晶振起振
	while((CLKCON & 0x20) != 0x20);		
    // 选择内部高频RC为系统时钟，内部高频RC2分频，Fosc=16MHz	
	CLKSWR = 0x51;			
    // 等待内部高频切换完成    
    while((CLKSWR & 0xC0) != 0x40);		
    // Fosc 1分频得到Fcpu，Fcpu=16MHz 	
	CLKDIV = 0x01;
    // I2C停止状态
    I2C_STATE = I2C_STOP;
    BUFFER_CHANGED = FALSE;
    
    EA_Enable();
    
    // 设置蜂鸣器P20 推挽输出
    P2M0 = P2M0 & 0xF0 | 0x08;	
    P2 &= 0xFE;
    // 主轴状态指示灯P05,低电平有效
    GPIO_Init(GPIOT0, GPIO_PIN_5, GPIO_MODE_OUT_PP);
    GPIO_WriteHigh(GPIOT0, GPIO_PIN_5);
    
    // 主轴方向控制P02 P03,低电平有效,默认关闭
    GPIO_Init(GPIOT0, GPIO_PIN_2, GPIO_MODE_OUT_PP);
    GPIO_Init(GPIOT0, GPIO_PIN_3, GPIO_MODE_OUT_PP);
    GPIO_WriteHigh(GPIOT0, GPIO_PIN_2);
    GPIO_WriteHigh(GPIOT0, GPIO_PIN_3);
    
    // T0定时器做延时时钟, 优先级最低(0),时钟源Fosc=16M，不分频
    TIM0_Init(TIM0_CLK_1, TIM0_SOFT_CONTROL, TIM0_INTIMING, TIM0_16BIT_CNT_TIMING);
    TIM0_ITPriorityConfig(TIM3_ITPriority_0);
    TIM0_ClearITFlag();
    TIM0_ITCmd(ENABLE);
    TIM0_OutPutCmd(DISABLE);
    TIM0_Cmd(DISABLE);
    
    // TIM3做主轴VSS定时器，优先级1, 时钟源Fosc=16M，256分频
    TIM3_Init(TIM3_SYSCLK, TIM3_CLK_256, DISABLE);
    TIM3_ITPriorityConfig(TIM0_ITPriority_1);
    TIM3_PDModeCmd(DISABLE);
    TIM3_ClearITFlag();
    TIM3_ITCmd(ENABLE);
    TIM3_Cmd(DISABLE);
    
    // 配置模拟I2C引脚和中断
    softi2c_pinmux_setup();
    softi2c_int_config();
    
    // 配置P04 PWM推挽输出模式
    GPIO_Init(GPIOT0, GPIO_PIN_4, GPIO_MODE_OUT_PP);
    GPIO_WriteLow(GPIOT0, GPIO_PIN_4);
        
    PWM_Init(PWM0_Group, PWMCK_8, PWMX_PWX1_HIGH_VALID, INDEPENDENT_MODE);
    // 关闭PWM出错检测
    PWM_FLTConfig(PWM0_Group, FLFPIN_LOW_PWMOFF, PWMX_PWX1_ALL_LOW, DISABLE);
    // 清除并关闭PWM中断
    PWM_ClearITFlag(PWM0_Group);
    PWM_ITCmd(PWM0_Group, DISABLE);
     // 映射P04为PWM输出
    PWM_PinRemapConfig(PWM_0, PWM_OUTPin_P04);
    
    // 设置初始频率和占空比
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
                    // 参数都以字节小端为准
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
                    // 未设置方向不启动主轴
                    if((spindle_dir & 0x1) && ((spindle_dir >> 1)  & 0x1)) {
                        break;
                    }
                    GPIO_WriteLow(GPIOT0, GPIO_PIN_5);
                    if(i2c_buffer[2]) {
                        playTone(tone_tick);
                    }
                    // 更新状态位
                    systemState |= 0x1;
                    // 设置轴方向
                    P0_2 = (spindle_dir & 0x1);
                    P0_3 = ((spindle_dir >> 1)  & 0x1);
                    // 打开PMW输出功能
                    pwm_duty = 0;
                    PWM_OutPutCmd(PWM_0, ENABLE);
                    PWM_Cmd(PWM0_Group, ENABLE);
                    // 直线加速到最低转速过程
                    while(pwm_duty < spindle_duty_min) {
                        pwm_duty += speedup_duty_seg;
                        pwm_duty = (pwm_duty > spindle_duty_min) ? spindle_duty_min : pwm_duty;
                        PWM_DutyCycleConfig(PWM0_Group, pwm_duty);
                        sysDelay(speedup_time_seg);
                    }
                    // 提示音
                    playTone(1);
                    
                    if(vss_time > VSS_LOW_LIMIT && vss_time < VSS_HIGH_LIMIT) {
                        // 动态转速定时器
                        TIM3_TimerCnt(calcTimer3Count(vss_time));
                        TIM3_Cmd(ENABLE);
                    }else {
                        // 不启用VSS,直线加速到最高转速
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
                    // 关闭VSS定时器
                    TIM3_Cmd(DISABLE);
                    // 减速到0过程    
                    while(pwm_duty > 0) {
                        pwm_duty -= slowdown_duty_seg;
                        pwm_duty = (pwm_duty < 0) ? 0 : pwm_duty;
                        PWM_DutyCycleConfig(PWM0_Group, pwm_duty);
                        sysDelay(slowdown_time_seg);
                    }
                    PWM_OutPutCmd(PWM_0, DISABLE);
                    PWM_Cmd(PWM0_Group, DISABLE);
                    // 关闭输出继电器
                    GPIO_WriteHigh(GPIOT0, GPIO_PIN_2);
                    GPIO_WriteHigh(GPIOT0, GPIO_PIN_3);
                    // 关闭主轴状态指示灯
                    GPIO_WriteHigh(GPIOT0, GPIO_PIN_5);
                    // 更新状态位
                    systemState &= (~0x1);
                    if(i2c_buffer[2]) {
                        playTone(tone_tick);
                    }
                    break;
                case MAKE_TONE_CMD:
                     // 开启蜂鸣器
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
蜂鸣器音频标准A音
@param tick 发声次数
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
计算PWM周期，PWMxP为12位(0~0xFFF),时钟16M，8分频
@param microseconds 周期时间(微秒) 范围0~2047us
*/
uint16_t calcPWMPeriod(uint16_t microseconds) {
    return (microseconds << 1) & 0xFFF;
}

/*
计算timer3(4/3Mhz)初值
定时时间范围0~1048ms
*/
uint16_t calcTimer3Count(uint16_t miliseconds) {
    uint16_t count = 62 * miliseconds;
    miliseconds = (miliseconds & 0x1) ? ((miliseconds - 1)) : miliseconds;
    count += (miliseconds >> 1);
    return (0xFFFF - count);
}

// 优先级0
void TIMER0_ISR(void) interrupt TIMER0_VECTOR {
    TIM0_Cmd(DISABLE);
    TIM0_ClearITFlag();
    DelayFlag = FALSE;
}

// 优先级1
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


// 优先级3 SDA P01
void SDA_ISR() interrupt INT1_VECTOR {
    // 清除中断
    PINTF0 &= 0xFD;
    // I2C握手信号
    if(!(I2C_SCL)) {
        return;
    }
    // 起始和终止ACK都将字节缓存清空
    i2c_tick = 0;
    i2c_byte = 0x00;
    // START = SDA下降沿 & SCL高电平
    // STOP = SDA 上升沿 & SCL高电平  
    if(!I2C_SDA && I2C_STATE == I2C_STOP) {
        I2C_STATE = I2C_START;
        i2c_buffer_cursor = 0;
    }else {
        I2C_STATE = I2C_STOP;
        BUFFER_CHANGED = TRUE;
    }
}

// 优先级2 SCL P00
void SCL_ISR() interrupt INT0_VECTOR {
    // 清除中断
    PINTF0 &= 0xFE;
    // I2C数据(SDA)只允许在SCL低电平发生变化
    // I2C_STATE == I2C_STOP
    if(I2C_STATE) {
        return;
    }
    // i2c_tick改变之前，此处为第9个时钟
    if(i2c_tick == 8) {
        // 在第9个时钟返回ACK信号，距上升沿~4.4us
        // 设置(SDA)P01 PD输出
        P0M0 = P0M0 & 0x0F | 0x80;
        // 拉低SDA 返回ACK信号
        I2C_SDA = 0;
    }

    // 采样数据,距上升沿~6.6us
    i2c_byte |= ((uint8_t)I2C_SDA << ((~i2c_tick) & 0x7));
    i2c_tick++;
    
    if(i2c_tick == 8) {
        // 在第8个时钟复制数据
        i2c_buffer[i2c_buffer_cursor++] = i2c_byte;
        // 关SCL_ISR
        EX1 = 0;
    }else if(i2c_tick == 9) {
        // 重置收缓存字节
        i2c_byte = 0x00;
        i2c_tick = 0;
        // 清除SCL_ISR重新打开
        PINTF0 &= 0xFD;
        EX1 = 1;
        // 恢复P00 P01 SMT上拉输入
        P0M0 = 0x66;
    }
}

/*
定时器0(16MHz)延时，函数调用开销9.75~10us
@param microseconds 微秒 实际延时时间[0-4095]us + 10us
*/
void timerDelay(uint16_t microseconds) {
    DelayFlag =  TRUE;
    TIM0_TimerCnt((0xFFFF - (microseconds << 4)));
    TIM0_Cmd(ENABLE);
    while(DelayFlag);
}

/*
软件延时，误差0.1ms
@param miliseconds 毫秒(0~0xFFFF)
*/
void sysDelay(uint16_t miliseconds) {
    uint16_t i;
    for(; miliseconds > 0; miliseconds--) {
         for(i = 1596; i > 0; i--);
    }
}
