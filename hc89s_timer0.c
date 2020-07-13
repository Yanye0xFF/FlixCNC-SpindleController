/**
*   ************************************************************************************
*								上海芯圣电子股份有限公司
*								    www.holychip.cn
*	************************************************************************************
*	@File				hc89s_timer0.c
*	@Library Version	V1.0.0.0
*	@Date				2019.07.19
*	************************************************************************************
*									 模块性能介绍
*	1、HC89S003的定时器0，1共有四种工作方式。
*	   方式0：16位自动重装定时器/计数器。
*	   方式1：16位定时器/计数器。
*	   方式2：8位自动重装定时器/计数器。
*	   方式3：T0分成两个（TL0/TH0）独立的8位定时器/计数器（T1无此模式）。
*	************************************************************************************
*									 应用注意事项
*	1、方式3时T0占用T1的TR1、TF1及中断源，由于TR1被T0占用，此时需要关闭T1可将T1设为工作
*	   方式3。
*	2、在方式0（Mode0）时THx(x = 0,1)和TLx(x = 0,1)读写操作遵循先高位后低位。并且在修改
*	   重载数据时，低位无论有无改变，只要高位被修改，低位都必须再写入一次，方式 1、2、3
*	   时无此要求。
*	************************************************************************************
*								       函数总览
*	TIM0_DeInit();						//恢复TIM0寄存器到默认值
*	TIM0_Init();						//根据指定的参数初始化TIM0
*	TIM0_TimerCnt();					//TIM0定时时间设置
*	TIM0_OutPutCmd();					//TIM0端口输出使能/失能
*	TIM0_Cmd();							//使能或失能TIM0
*	TIM0_PinRemapConfig()				//配置TIM0输出的管脚映射
*	TIM0_ITCmd();						//使能或失能TIM0中断
*	TIM0_GetITFlag();					//读取TIM0中断标志状态
*	TIM0_ClearITFlag();					//清除TIM0中断标志状态
*	TIM0_ITPriorityConfig();			//TIM0中断优先级配置
*	************************************************************************************
*									   计算公式
*	1、Tim0计算时间
*	   定时时间 = (65536 - T0) * (1 / (Fosc /Timer分频系数))
*	2、Tim0反推初值
*	   初值 = 65536 - ((定时时间) / (1/(Fosc / Timer分频系数)))
*	************************************************************************************
*  								       客户服务
*	 感谢您使用我们的单片机，若发现错误或对函数的使用存在疑问。请添加上海芯圣电子官方QQ
*	****************************技术支持群：201030494**********************************
*   ************************************************************************************
**/

#include "hc89s_timer0.h"

/**
  * @说明  	恢复TIM0寄存器到默认值
  * @参数  	无
  * @返回值 无
  * @注 	无
  */
void TIM0_DeInit(void)
{
	TMOD  &=~ (TIMER_GATE0|TIMER_CT0|TIMER_M0);
	TCON1 &=~ (TIMER_T0OUT|TIMER_T0X12);
	TCON  &=~ (TIMER_TF0|TIMER_TR0);
	T0_MAP = TIMER_T0MAP_RESET_VALUE;
	TH0 = TIMER_T0_RESET_VALUE>>8;
	TL0 = TIMER_T0_RESET_VALUE&0x00FF;
}

/**
  * @说明  	根据指定的参数初始化TIM0
  * @参数  	ClkPrescaler ：TIM0时钟分频系数
  *			TIM0_CLK_1		//时钟1分频
  *			TIM0_CLK_12		//时钟12分频
  * @参数  	Gate ：TIM0软件控制/软件硬件同时控制
  *			TIM0_SOFT_CONTROL			//只需软件置 TRx 即可启动 Tx
  *			TIM0_SOFT_AND_HARD_CONTROL	//只有在 INTx 端口电平为高电平时 TRx 置 1， Tx 才工作
  * @参数  	CntOrTim ：TIM0计数/定时
  *			TIM0_INTIMING		//Tx 用于内部定时
  *			TIM0_OUTCLKCNT		//Tx 用于外部计数
  * @参数  	Mode ：TIM0工作模式
  *			TIM0_16BIT_AUTO_CNT_TIMING		//16位重装载定时器/计数器
  *			TIM0_16BIT_CNT_TIMING			//16位定时器/计数器
  *			TIM0_8BIT_AUTO_CNT_TIMING		//8位自动重装初值定时器/计数器
  * @返回值 无
  * @注 	无
  */
void TIM0_Init(TIM0_ClkPrescaler_Typedef ClkPrescaler,TIM0_Gate_Typedef Gate,TIM0_CntOrTim_Typedef CntOrTim,TIM0_Mode_Typedef Mode)
{
	TCON1 = (TCON1&(~TIMER_T0X12))|(ClkPrescaler&TIMER_T0X12);
	TMOD  = (TMOD&(~(TIMER_GATE0|TIMER_CT0|TIMER_M0)))|(Gate&TIMER_GATE0)|(CntOrTim&TIMER_CT0)|(Mode&TIMER_M0);	
}

/**
  * @说明  	TIM0定时时间设置
  * @参数  	Value ：TIM0定时时间
  *         取值范围：0x0000 - 0xFFFF
  * @返回值 无
  * @注 	无
  */
void TIM0_TimerCnt(u16 Value)
{
	TH0 = Value>>8;
	TL0 = Value&0x00FF;	
}

/**
  * @说明  	TIM0端口输出使能/失能
  * @参数  	NewState ：TIM0通道输出新状态
  *			DISABLE		//失能
  *			ENABLE 		//使能
  * @返回值 无
  * @注 	无
  */
void TIM0_OutPutCmd(FunctionalState NewState)
{
	if(NewState)TCON1 |= TIMER_T0OUT;
	else TCON1 &=~ TIMER_T0OUT;
}

/**
  * @说明  	使能或失能TIM0
  * @参数  	NewState ：TIM0新状态
  *			DISABLE		//失能
  *			ENABLE 		//使能
  * @返回值 无
  * @注 	无
  */
void TIM0_Cmd(FunctionalState NewState)
{
	if(NewState)TCON |= TIMER_TR0;
	else TCON &=~ TIMER_TR0;
}

/**
  * @说明  	配置TIM0输出的管脚映射
  * @参数  	OutPin ：管脚输出源
  *			TIM0_OUTPin_P00  	//TIM0输出端口为P00
  *			TIM0_OUTPin_P01  	//TIM0输出端口为P01
  *			TIM0_OUTPin_P02  	//TIM0输出端口为P02
  *			TIM0_OUTPin_P03  	//TIM0输出端口为P03
  *			TIM0_OUTPin_P04  	//TIM0输出端口为P04
  *			TIM0_OUTPin_P05  	//TIM0输出端口为P05
  *			TIM0_OUTPin_P06  	//TIM0输出端口为P06
  *			TIM0_OUTPin_P07		//TIM0输出端口为P07
  *			TIM0_OUTPin_P10		//TIM0输出端口为P10
  * 		TIM0_OUTPin_P11		//TIM0输出端口为P11
  *			TIM0_OUTPin_P20		//TIM0输出端口为P20
  *			TIM0_OUTPin_P21		//TIM0输出端口为P21
  *			TIM0_OUTPin_P22		//TIM0输出端口为P22
  *			TIM0_OUTPin_P23		//TIM0输出端口为P23
  *			TIM0_OUTPin_P24		//TIM0输出端口为P24
  *			TIM0_OUTPin_P25		//TIM0输出端口为P25
  *			TIM0_OUTPin_P26		//TIM0输出端口为P26
  *			TIM0_OUTPin_P27		//TIM0输出端口为P27
  * @返回值 无
  * @注 	无
  */
void TIM0_PinRemapConfig(TIM0_OutPin_TypeDef OutPin)
{
	T0_MAP = OutPin;
}

/**
  * @说明  	使能或失能TIM0中断
  * @参数  	NewState ：TIM0中断新状态
  *			DISABLE		//失能
  *			ENABLE 		//使能
  * @返回值 无
  * @注 	无
  */
void TIM0_ITCmd(FunctionalState NewState)
{
	if(NewState)IE |= INT_ET0;
	else IE &=~ INT_ET0;
}

/**
  * @说明  	读取TIM0中断标志状态
  * @参数  	无
  * @返回值 FlagStatus ：TIM0中断标志状态
  *			参考 FlagStatus 枚举类
  * @注 	无
  */
FlagStatus TIM0_GetITFlag(void)
{
	if(TCON&TIMER_TF0) return SET;
	return RESET;
}

/**
  * @说明  	清除TIM0中断标志状态
  * @参数  	无
  * @返回值 无
  * @注 	无
  */
void TIM0_ClearITFlag(void)
{
	TCON &=~ TIMER_TF0;	
}

/**
  * @说明  	设置TIM0中断优先级
  * @参数  	ITPriority ：TIM0中断优先级
  *			TIM0_ITPriority_0	//TIM0中断优先级0(最低)
  *			TIM0_ITPriority_1	//TIM0中断优先级1
  *			TIM0_ITPriority_2	//TIM0中断优先级2
  *			TIM0_ITPriority_3	//TIM0中断优先级3(最高)
  * @返回值 无
  * @注 	无
  */
void TIM0_ITPriorityConfig(TIM0_ITPriority_TypeDef ITPriority)
{
	IP0 = (IP0&(~INT_PT0))|ITPriority;		
}