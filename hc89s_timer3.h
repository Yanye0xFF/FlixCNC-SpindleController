#ifndef __hc89s_TIMER3_H__
#define __hc89s_TIMER3_H__

#include "HC89S003F4.h"

/*定时器3时钟分频系数枚举类*/
typedef enum
{
	TIM3_CLK_1		= ((u8)0x00),	//时钟1分频
	TIM3_CLK_8		= ((u8)0x10),	//时钟8分频
	TIM3_CLK_64		= ((u8)0x20),	//时钟64分频
	TIM3_CLK_256	= ((u8)0x30),	//时钟256分频	
}TIM3_ClkPrescaler_Typedef;

/*定时器3时钟源枚举类*/
typedef enum
{
	TIM3_SYSCLK		= ((u8)0x00),	//OSC时钟源
	TIM3_OUTCLK		= ((u8)0x01),	//T3端口时钟源
}TIM3_ClkSource_Typedef;

/*TIM3输出端口枚举类*/
typedef enum {
  	TIM3_OUTPin_P00       	= (u8)0x00,	//TIM3输出端口为P00
  	TIM3_OUTPin_P01       	= (u8)0x01,	//TIM3输出端口为P01
  	TIM3_OUTPin_P02       	= (u8)0x02,	//TIM3输出端口为P02
  	TIM3_OUTPin_P03  		= (u8)0x03,	//TIM3输出端口为P03
  	TIM3_OUTPin_P04  		= (u8)0x04,	//TIM3输出端口为P04
  	TIM3_OUTPin_P05  		= (u8)0x05,	//TIM3输出端口为P05
  	TIM3_OUTPin_P06  		= (u8)0x06,	//TIM3输出端口为P06
	TIM3_OUTPin_P07			= (u8)0x07,	//TIM3输出端口为P07
	TIM3_OUTPin_P10			= (u8)0x10,	//TIM3输出端口为P10
	TIM3_OUTPin_P11			= (u8)0x11,	//TIM3输出端口为P11
	TIM3_OUTPin_P20		    = (u8)0x20,	//TIM3输出端口为P20
	TIM3_OUTPin_P21			= (u8)0x21,	//TIM3输出端口为P21
	TIM3_OUTPin_P22			= (u8)0x22,	//TIM3输出端口为P22
	TIM3_OUTPin_P23			= (u8)0x23,	//TIM3输出端口为P23
	TIM3_OUTPin_P24			= (u8)0x24,	//TIM3输出端口为P24
	TIM3_OUTPin_P25			= (u8)0x25,	//TIM3输出端口为P25
	TIM3_OUTPin_P26			= (u8)0x26,	//TIM3输出端口为P26
	TIM3_OUTPin_P27			= (u8)0x27,	//TIM3输出端口为P27
}TIM3_OutPin_TypeDef;

/*TIM3中断优先级枚举类*/
typedef enum {
	TIM3_ITPriority_0		= (u8)0x00, //TIM3中断优先级0(最低)
	TIM3_ITPriority_1		= (u8)0x04, //TIM3中断优先级1
	TIM3_ITPriority_2		= (u8)0x08, //TIM3中断优先级2
	TIM3_ITPriority_3		= (u8)0x0C, //TIM3中断优先级3(最高)
}TIM3_ITPriority_TypeDef;

/*******TIM3函数********/
void TIM3_DeInit(void);												//恢复TIM3寄存器到默认值
void TIM3_Init(TIM3_ClkSource_Typedef ClkSource,TIM3_ClkPrescaler_Typedef ClkPrescaler,FunctionalState NewState);//根据指定的参数初始化TIM3
void TIM3_TimerCnt(u16 Value);										//TIM3定时时间设置
void TIM3_PinRemapConfig(TIM3_OutPin_TypeDef OutPin);				//配置TIM3输出的管脚映射
void TIM3_PDModeCmd(FunctionalState NewState);						//使能或失能TIM3掉电模式下运行
void TIM3_ITCmd(FunctionalState NewState);							//使能或失能TIM3中断
void TIM3_Cmd(FunctionalState NewState);							//使能或失能TIM3
FlagStatus TIM3_GetITFlag(void);									//读取TIM3中断标志状态
void TIM3_ClearITFlag(void);										//清除TIM3中断标志状态
void TIM3_ITPriorityConfig(TIM3_ITPriority_TypeDef ITPriority);		//TIM3中断优先级配置

#endif