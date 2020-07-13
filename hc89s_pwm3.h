#ifndef __hc89s_PWM3_H__
#define __hc89s_PWM3_H__

#include "HC89S003F4.h"

/*PWM输出有效电平枚举类*/						
typedef enum {
	PWM3_HIGH_VALID 			= (u8)0x00,		//例：PWM3高有效
	PWM3_LOW_VALID				= (u8)0x08,		//例：PWM3低有效
}PWM3_VoltValid_TypeDef;

/*PWM时钟分频系数枚举类*/
typedef enum {
	PWM3CK_1						= (u8)0x00,		//PWM时钟为OSC时钟1分频
	PWM3CK_2						= (u8)0x01,		//PWM时钟为OSC时钟2分频
	PWM3CK_4						= (u8)0x02,		//PWM时钟为OSC时钟4分频
	PWM3CK_8						= (u8)0x03,		//PWM时钟为OSC时钟8分频
	PWM3CK_16					= (u8)0x04,		//PWM时钟为OSC时钟16分频
	PWM3CK_32					= (u8)0x05,		//PWM时钟为OSC时钟32分频
	PWM3CK_64					= (u8)0x06,		//PWM时钟为OSC时钟64分频
	PWM3CK_128					= (u8)0x07,		//PWM时钟为OSC时钟128分频
}PWM3_ClkDevice_TypeDef;

/*PWM3输出端口枚举类*/
typedef enum {
  	PWM3_OUTPin_P00       		= (u8)0x00,		//PWM3输出端口为P00
  	PWM3_OUTPin_P01       		= (u8)0x01,		//PWM3输出端口为P01
  	PWM3_OUTPin_P02       		= (u8)0x02,		//PWM3输出端口为P02
  	PWM3_OUTPin_P03  			= (u8)0x03,		//PWM3输出端口为P03
  	PWM3_OUTPin_P04  			= (u8)0x04,		//PWM3输出端口为P04
  	PWM3_OUTPin_P05  			= (u8)0x05,		//PWM3输出端口为P05
  	PWM3_OUTPin_P06  			= (u8)0x06,		//PWM3输出端口为P06
	PWM3_OUTPin_P07				= (u8)0x07,		//PWM3输出端口为P07
	PWM3_OUTPin_P10				= (u8)0x10,		//PWM3输出端口为P10
	PWM3_OUTPin_P11				= (u8)0x11,		//PWM3输出端口为P11
	PWM3_OUTPin_P20			    = (u8)0x20,		//PWM3输出端口为P20
	PWM3_OUTPin_P21				= (u8)0x21,		//PWM3输出端口为P21
	PWM3_OUTPin_P22				= (u8)0x22,		//PWM3输出端口为P22
	PWM3_OUTPin_P23				= (u8)0x23,		//PWM3输出端口为P23
	PWM3_OUTPin_P24				= (u8)0x24,		//PWM3输出端口为P24
	PWM3_OUTPin_P25				= (u8)0x25,		//PWM3输出端口为P25
	PWM3_OUTPin_P26				= (u8)0x26,		//PWM3输出端口为P26
	PWM3_OUTPin_P27				= (u8)0x27,		//PWM3输出端口为P27
}PWM3_OutPin_TypeDef;

/*PWM3中断优先级枚举类*/
typedef enum {
	PWM3_ITPriority_0			= (u8)0x00, 	//PWM3中断优先级0(最低)
	PWM3_ITPriority_1			= (u8)0x40, 	//PWM3中断优先级1
	PWM3_ITPriority_2			= (u8)0x80,	 	//PWM3中断优先级2
	PWM3_ITPriority_3			= (u8)0xC0, 	//PWM3中断优先级3(最高)
}PWM3_ITPriority_TypeDef;

void PWM3_DeInit(void);											//恢复PWM3寄存器到默认值
void PWM3_Init(PWM3_VoltValid_TypeDef VoltValid,PWM3_ClkDevice_TypeDef ClkDevice);//根据指定的参数初始化PWM3
void PWM3_OutPutCmd(FunctionalState NewState);					//PWM3端口输出使能/失能
void PWM3_PeriodConfig(u8 PwmPeriod);							//PWM3周期设置
void PWM3_DutyCycleConfig(u8 PwmDuty);							//PWM3占空比设置
void PWM3_PinRemapConfig(PWM3_OutPin_TypeDef OutPin);			//PWM3输出管脚映射
void PWM3_Cmd(FunctionalState NewState);						//使能或失能PWM3模块
void PWM3_ITCmd(FunctionalState NewState);						//使能或失能PWM3中断
FlagStatus PWM3_GetITFlag(void);								//获取PWM3中断标志位
void PWM3_ClearITFlag(void);									//清除PWM3中断标志位
void PWM3_ITPriorityConfig(PWM3_ITPriority_TypeDef ITPriority);	//PWM中断优先级配置
							
#endif