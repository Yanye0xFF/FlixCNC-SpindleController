#ifndef __hc89s_TIMER0_H__
#define __hc89s_TIMER0_H__

#include "HC89S003F4.h"

/*定时器1时钟分频系数枚举类*/
typedef enum
{
	TIM0_CLK_1						= ((u8)0x01),		//时钟1分频
	TIM0_CLK_12						= ((u8)0x00),		//时钟12分频
}TIM0_ClkPrescaler_Typedef;

/*定时器1门控模式枚举类*/
typedef enum
{
	TIM0_SOFT_CONTROL				= ((u8)0x00),		//只需软件置 TRx 即可启动 Tx
	TIM0_SOFT_AND_HARD_CONTROL		= ((u8)0x08),		//只有在 INTx 端口电平为高电平时 TRx 置 1， Tx 才工作
}TIM0_Gate_Typedef;

/*定时器1定时计数模式枚举类*/
typedef enum
{
	TIM0_INTIMING					= ((u8)0x00),		//Tx 用于内部定时
	TIM0_OUTCLKCNT					= ((u8)0x04),		//Tx 用于外部计数
}TIM0_CntOrTim_Typedef;

/*定时器1模式枚举类*/
typedef enum
{
	TIM0_16BIT_AUTO_CNT_TIMING		= ((u8)0x00),		//16位重装载定时器/计数器
	TIM0_16BIT_CNT_TIMING			= ((u8)0x01),		//16位定时器/计数器
	TIM0_8BIT_AUTO_CNT_TIMING		= ((u8)0x02),		//8位自动重装初值定时器/计数器
//	TIM0_TWO_8BIT_AUTO_CNT_TIMING	= ((u8)0x03),		//2路8位自动重装初值定时器/计数器
}TIM0_Mode_Typedef;

/*TIM0输出端口枚举类*/
typedef enum {
  	TIM0_OUTPin_P00       	= (u8)0x00,	//TIM0输出端口为P00
  	TIM0_OUTPin_P01       	= (u8)0x01,	//TIM0输出端口为P01
  	TIM0_OUTPin_P02       	= (u8)0x02,	//TIM0输出端口为P02
  	TIM0_OUTPin_P03  		= (u8)0x03,	//TIM0输出端口为P03
  	TIM0_OUTPin_P04  		= (u8)0x04,	//TIM0输出端口为P04
  	TIM0_OUTPin_P05  		= (u8)0x05,	//TIM0输出端口为P05
  	TIM0_OUTPin_P06  		= (u8)0x06,	//TIM0输出端口为P06
	TIM0_OUTPin_P07			= (u8)0x07,	//TIM0输出端口为P07
	TIM0_OUTPin_P10			= (u8)0x10,	//TIM0输出端口为P10
	TIM0_OUTPin_P11			= (u8)0x11,	//TIM0输出端口为P11
	TIM0_OUTPin_P20		    = (u8)0x20,	//TIM0输出端口为P20
	TIM0_OUTPin_P21			= (u8)0x21,	//TIM0输出端口为P21
	TIM0_OUTPin_P22			= (u8)0x22,	//TIM0输出端口为P22
	TIM0_OUTPin_P23			= (u8)0x23,	//TIM0输出端口为P23
	TIM0_OUTPin_P24			= (u8)0x24,	//TIM0输出端口为P24
	TIM0_OUTPin_P25			= (u8)0x25,	//TIM0输出端口为P25
	TIM0_OUTPin_P26			= (u8)0x26,	//TIM0输出端口为P26
	TIM0_OUTPin_P27			= (u8)0x27,	//TIM0输出端口为P27
}TIM0_OutPin_TypeDef;

/*TIM0中断优先级枚举类*/
typedef enum {
	TIM0_ITPriority_0		= (u8)0x00, //TIM0中断优先级0(最低)
	TIM0_ITPriority_1		= (u8)0x04, //TIM0中断优先级1
	TIM0_ITPriority_2		= (u8)0x08, //TIM0中断优先级2)
	TIM0_ITPriority_3		= (u8)0x0C, //TIM0中断优先级3(最高)
}TIM0_ITPriority_TypeDef;
												  	
void TIM0_DeInit(void);									//恢复TIM0寄存器到默认值
void TIM0_Init(TIM0_ClkPrescaler_Typedef ClkPrescaler,TIM0_Gate_Typedef Gate,TIM0_CntOrTim_Typedef CntOrTim,TIM0_Mode_Typedef Mode);//根据指定的参数初始化TIM0
void TIM0_TimerCnt(u16 Value);							//TIM0定时时间设置
void TIM0_OutPutCmd(FunctionalState NewState);			//TIM0端口输出使能/失能
void TIM0_PinRemapConfig(TIM0_OutPin_TypeDef OutPin);	//配置TIM0输出的管脚映射
void TIM0_Cmd(FunctionalState NewState);				//使能或失能TIM0
void TIM0_ITCmd(FunctionalState NewState);				//使能或失能TIM0中断
FlagStatus TIM0_GetITFlag(void);						//读取TIM0中断标志状态
void TIM0_ClearITFlag(void);							//清除TIM0中断标志状态
void TIM0_ITPriorityConfig(TIM0_ITPriority_TypeDef ITPriority);	//TIM0中断优先级配置

#endif








