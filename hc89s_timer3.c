/**
*									 模块性能介绍
*	1、定时器3只有一个工作方式：16位自动重载计数器/定时器，可以设置预分频比，并可以工作
*	   在掉电模式。
*	2、TH3和TL3读写操作遵循以下顺序：先高位后低位。
*	************************************************************************************
*									 应用注意事项
*	1、T3外部时钟必须输入一个方波。
*	2、T3外部时钟输入口必须设置为数字输入。
*	3、不可以在中断中判断中断标志位。
*	4、T3时钟使用内部时钟时无法工作在掉电模式下。
*	5、必须在使能前给TH3，TL3赋值。
*	6、当T3计数时钟源选择端口输入外部时钟，TR3和T3CLKS的配置需要同时进行，即使用一条指
*	   令完成配置。
*	************************************************************************************
*								      函数总览
*	TIM3_DeInit()						//恢复TIM3寄存器到默认值
*	TIM3_Init()							//根据指定的参数初始化TIM3
*	TIM3_TimerCnt()						//TIM3定时时间设置
*	TIM3_PinRemapConfig()				//配置TIM3输出的管脚映射
*	TIM3_PDModeCmd()					//使能或失能TIM3掉电模式下运行
*	TIM3_ITCmd()						//使能或失能TIM3中断
*	TIM3_Cmd()							//使能或失能TIM3
*	TIM3_GetITFlag()					//读取TIM3中断标志状态
*	TIM3_ClearITFlag()					//清除TIM3中断标志状态
*	TIM3_ITPriorityConfig();			//TIM3中断优先级配置
*	************************************************************************************
*									   计算公式（注：该公式仅能直接用于秒级延时，坑）
*	1、Tim3计算时间。
*	   定时时间 = (65536 - T3) * (1 / (Fosc /Timer分频系数))
*	2、Tim3反推初值。
*	   初值 = 65536 - ((定时时间) / (1/(Fosc / Timer分频系数)))
*	3、Tim3外部时钟计算时间。
*	   定时时间  = (65536 - T3) * (1 / 外部输入时钟)
*	2、Tim3外部时钟反推初值。
*	   初值  = 65536 - ((定时时间) / (1 / 外部输入时钟))
*	************************************************************************************
**/

#include "hc89s_timer3.h"

/**
  * @说明  	恢复TIM3寄存器到默认值
  * @参数  	无
  * @返回值 无
  * @注 	无
  */
void TIM3_DeInit(void)
{
	T3CON = TIM3_T3CON_RESET_VALUE;
	T3 = TIM3_T3_RESET_VALUE;
	T3_MAP = TIMER_T3MAP_RESET_VALUE;
}

/**
  * @说明  	根据指定的参数初始化TIM3
  * @参数  	ClockSource ：TIM3时钟源
  *			TIM3_SYSCLK		//OSC时钟源
  *			TIM3_OUTCLK		//T3端口时钟源
  * @参数  	ClockPrescaler ：TIM3时钟源分频系数
  *			TIM3_CLK_1		//时钟1分频
  *			TIM3_CLK_8		//时钟8分频
  *			TIM3_CLK_64		//时钟64分频
  *			TIM3_CLK_256	//时钟256分频	
  * @返回值 无
  * @注 	无
  */
void TIM3_Init(TIM3_ClkSource_Typedef ClkSource,TIM3_ClkPrescaler_Typedef ClkPrescaler,FunctionalState NewState)
{
	T3CON = (T3CON&(~(TIM3_T3PS|TIM3_T3CLKS|TIM3_TR3)))|(ClkPrescaler&TIM3_T3PS)|(ClkSource&TIM3_T3CLKS)|((unsigned char)NewState<<2);	
}

/**
  * @说明  	TIM3定时时间设置
  * @参数  	Value ：TIM3定时时间
  *         取值范围：0x0000 - 0xFFFF
  * @返回值 无
  * @注 	无
  */
void TIM3_TimerCnt(u16 Value)
{
	T3 = Value;	
}


/**
  * @说明  	配置TIM3输出的管脚映射
  * @参数  	OutPin ：管脚输出源
  *			TIM3_OUTPin_P00  	//TIM3输出端口为P00
  *			TIM3_OUTPin_P01  	//TIM3输出端口为P01
  *			TIM3_OUTPin_P02  	//TIM3输出端口为P02
  *			TIM3_OUTPin_P03  	//TIM3输出端口为P03
  *			TIM3_OUTPin_P04  	//TIM3输出端口为P04
  *			TIM3_OUTPin_P05  	//TIM3输出端口为P05
  *			TIM3_OUTPin_P06  	//TIM3输出端口为P06
  *			TIM3_OUTPin_P07		//TIM3输出端口为P07
  *			TIM3_OUTPin_P10		//TIM3输出端口为P10
  * 		TIM3_OUTPin_P11		//TIM3输出端口为P11
  *			TIM3_OUTPin_P20		//TIM3输出端口为P20
  *			TIM3_OUTPin_P21		//TIM3输出端口为P21
  *			TIM3_OUTPin_P22		//TIM3输出端口为P22
  *			TIM3_OUTPin_P23		//TIM3输出端口为P23
  *			TIM3_OUTPin_P24		//TIM3输出端口为P24
  *			TIM3_OUTPin_P25		//TIM3输出端口为P25
  *			TIM3_OUTPin_P26		//TIM3输出端口为P26
  *			TIM3_OUTPin_P27		//TIM3输出端口为P27
  * @返回值 无
  * @注 	无
  */
void TIM3_PinRemapConfig(TIM3_OutPin_TypeDef OutPin)
{
	T3_MAP = OutPin;
}

/**
  * @说明  	使能或失能TIM3掉电模式下运行
  * @参数  	NewState ：TIM3掉电模式下运行新状态
  *			DISABLE		//失能
  *			ENABLE		//使能
  * @返回值 无
  * @注 	无
  */
void TIM3_PDModeCmd(FunctionalState NewState)
{
	if(NewState)T3CON |= TIM3_T3PD_EN;
	else T3CON &=~ TIM3_T3PD_EN;
}

/**
  * @说明  	使能或失能TIM3中断
  * @参数  	NewState ：TIM3中断新状态
  *			DISABLE		//失能
  *			ENABLE		//使能
  * @返回值 无
  * @注 	无
  */
void TIM3_ITCmd(FunctionalState NewState)
{
	if(NewState)IE1 |= INT_ET3;
	else IE1 &=~ INT_ET3;
}

/**
  * @说明  	使能或失能TIM3
  * @参数  	NewState ：TIM3新状态
  *			DISABLE		//失能
  *			ENABLE		//使能
  * @返回值 无
  * @注 	无
  */
void TIM3_Cmd(FunctionalState NewState)
{
	if(NewState)T3CON |= TIM3_TR3;
	else T3CON &=~ TIM3_TR3;
}

/**
  * @说明  	读取TIM3中断标志状态
  * @参数  	无
  * @返回值 FlagStatus ：TIM3中断标志状态
  *			参考 FlagStatus 枚举类
  * @注 	无
  */
FlagStatus TIM3_GetITFlag(void)
{
	if(T3CON&TIM3_TF3) return SET;
	return RESET;
}

/**
  * @说明  	清除TIM3中断标志状态
  * @参数  	无
  * @返回值 无
  * @注 	无
  */
void TIM3_ClearITFlag(void)
{
	T3CON &=~ TIM3_TF3;	
}

/**
  * @说明  	设置TIM3中断优先级
  * @参数  	ITPriority ：TIM3中断优先级
  *			TIM3_ITPriority_0	//TIM3中断优先级0(最低)
  *			TIM3_ITPriority_1	//TIM3中断优先级1
  *			TIM3_ITPriority_2	//TIM3中断优先级2
  *			TIM3_ITPriority_3	//TIM3中断优先级3(最高)
  * @返回值 无
  * @注 	无
  */
void TIM3_ITPriorityConfig(TIM3_ITPriority_TypeDef ITPriority)
{
	IP2 = (IP2&(~INT_PT3))|ITPriority;		
}
