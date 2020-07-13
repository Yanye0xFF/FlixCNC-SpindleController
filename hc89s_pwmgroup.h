#ifndef __hc89s_PWMGROUP_H__
#define __hc89s_PWMGROUP_H__

#include "HC89S003F4.h"

/*PWM通道枚举类*/
typedef enum {
	PWM_0	  					= (u8)0x01,	//PWM0通道
	PWM_0_1	  					= (u8)0x02,	//PWM01通道
	PWM_0_All					= (u8)0x03,	//PWM0全部通道
	PWM_1	  					= (u8)0x04,	//PWM1通道
	PWM_1_1	  					= (u8)0x05,	//PWM11通道
	PWM_1_All					= (u8)0x06,	//PWM1全部通道
	PWM_2	  					= (u8)0x07,	//PWM2通道
	PWM_2_1	  					= (u8)0x08,	//PWM21通道
	PWM_2_All					= (u8)0x09,	//PWM2全部通道
}PWM_Channel_TypeDef;

/*PWM组枚举类*/
typedef enum {
	PWM0_Group  				= (u8)0x11,	//PWM0组
	PWM1_Group  				= (u8)0x12,	//PWM1组
	PWM2_Group  				= (u8)0x13,	//PWM2组
}PWM_Group_TypeDef;


/*PWM组输出控制枚举类*/
typedef enum {
	PWMX_DISABLE_PWMX1_DISABLE 	= (u8)0x00,	//例：PWM0组。即PWM0不输出，PWM01不输出
	PWMX_ENABLE_PWMX1_DISABLE  	= (u8)0x02,	//例：PWM0组。即PWM0输出，PWM01不输出
	PWMX_DISABLE_PWMX1_ENABLE  	= (u8)0x04,	//例：PWM0组。即PWM0不输出，PWM01输出
	PWMX_ENABLE_PWMX1_ENABLE  	= (u8)0x06,	//例：PWM0组。即PWM0输出，PWM01输出
}PWM_GroupOutMode_TypeDef;

/*PWM输出模式枚举类*/						
typedef enum {
	PWMX_PWX1_HIGH_VALID 		= (u8)0x00,	//例：PWM0组。即PWM0，PWM01均为高有效
	PWMX_HIGH_PWX1_LOW_VALID	= (u8)0x04,	//例：PWM0组。即PWM0为高有效，PWM01低有效
	PWMX_LOW_PWX1_HIGH_VALID	= (u8)0x08,	//例：PWM0组。即PWM0为低有效，PWM01高有效
	PWMX_PWX1_LOW_VALID			= (u8)0x0C,	//例：PWM0组。即PWM0，PWM01均为低有效
}PWM_VoltValid_TypeDef;

/*PWM组模式枚举类*/
typedef enum {
	COMPLEMENTARY_MODE  		= (u8)0x00,	//互补模式
	INDEPENDENT_MODE  			= (u8)0x08,	//独立模式
}PWM_GroupMode_TypeDef;

/*故障状态下PWM输出电平枚举类*/						
typedef enum {
	PWMX_PWX1_ALL_LOW			= (u8)0x00,	//例：PWM0组。即故障时PWM0，PWM01输出低电平
	PWMX_LOW_PWX1_HIGH			= (u8)0x20,	//例：PWM0组。即故障时PWM0输出低电平，PWM01输出高电平
	PWMX_HIGH_PWX1_LOW			= (u8)0x40,	//例：PWM0组。即故障时PWM0输出高电平，PWM01输出低电平
	PWMX_PWX1_ALL_HIGH			= (u8)0x60,	//例：PWM0组。即故障时PWM0，PWM01输出高电平
}PWM_FltPWMOut_TypeDef;

/*故障电平枚举类*/						
typedef enum {
	FLFPIN_LOW_PWMOFF			= (u8)0x00,	//故障检测脚为低电平时，故障
	FLFPIN_HIGH_PWMOFF			= (u8)0x10,	//故障检测脚为高电平时，故障
}PWM_FltPinVolt_TypeDef;

/*PWM时钟分频系数枚举类*/
typedef enum {
	PWMCK_1						= (u8)0x00,	//PWM时钟为OSC时钟1分频
	PWMCK_8						= (u8)0x01,	//PWM时钟为OSC时钟8分频
	PWMCK_32					= (u8)0x02,	//PWM时钟为OSC时钟32分频
	PWMCK_128					= (u8)0x03,	//PWM时钟为OSC时钟128分频
}PWM_ClkDevice_TypeDef;

/*PWMGroup输出端口枚举类*/
typedef enum {
  	PWM_OUTPin_P00       		= (u8)0x00,	//PWM输出端口为P00
  	PWM_OUTPin_P01       		= (u8)0x01,	//PWM输出端口为P01
  	PWM_OUTPin_P02       		= (u8)0x02,	//PWM输出端口为P02
  	PWM_OUTPin_P03  			= (u8)0x03,	//PWM输出端口为P03
  	PWM_OUTPin_P04  			= (u8)0x04,	//PWM输出端口为P04
  	PWM_OUTPin_P05  			= (u8)0x05,	//PWM输出端口为P05
  	PWM_OUTPin_P06  			= (u8)0x06,	//PWM输出端口为P06
	PWM_OUTPin_P07				= (u8)0x07,	//PWM输出端口为P07
	PWM_OUTPin_P10				= (u8)0x10,	//PWM输出端口为P10
	PWM_OUTPin_P11				= (u8)0x11,	//PWM输出端口为P11
	PWM_OUTPin_P20		    	= (u8)0x20,	//PWM输出端口为P20
	PWM_OUTPin_P21				= (u8)0x21,	//PWM输出端口为P21
	PWM_OUTPin_P22				= (u8)0x22,	//PWM输出端口为P22
	PWM_OUTPin_P23				= (u8)0x23,	//PWM输出端口为P23
	PWM_OUTPin_P24				= (u8)0x24,	//PWM输出端口为P24
	PWM_OUTPin_P25				= (u8)0x25,	//PWM输出端口为P25
	PWM_OUTPin_P26				= (u8)0x26,	//PWM输出端口为P26
	PWM_OUTPin_P27				= (u8)0x27,	//PWM输出端口为P27
}PWM_OutPin_TypeDef;

/*PWM中断优先级枚举类*/
typedef enum {
	PWM_ITPriority_0			= (u8)0x00, //PWM中断优先级0(最低)
	PWM_ITPriority_1			= (u8)0x40, //PWM中断优先级1
	PWM_ITPriority_2			= (u8)0x80, //PWM中断优先级2)
	PWM_ITPriority_3			= (u8)0xC0, //PWM中断优先级3(最高)
}PWM_ITPriority_TypeDef;

/*******PWM函数********/
void PWM_DeInit(PWM_Group_TypeDef PWM_Group);										//恢复POWER寄存器到默认值
void PWM_Init(PWM_Group_TypeDef Group,PWM_ClkDevice_TypeDef ClkDevice,PWM_VoltValid_TypeDef GroupOutMode,PWM_GroupMode_TypeDef GroupMode);	//PWM设置
void PWM_FLTConfig(PWM_Group_TypeDef Group,PWM_FltPinVolt_TypeDef FltPinVolt,PWM_FltPWMOut_TypeDef FltPWMOut,FunctionalState NewState);	//PWM故障设置
void PWM_PinRemapConfig(PWM_Channel_TypeDef Channel,PWM_OutPin_TypeDef OutPin);		//PWM输出管脚映射
void PWM_OutPutCmd(PWM_Channel_TypeDef Channel,FunctionalState NewState);			//PWM端口输出使能/失能
void PWM_PeriodConfig(PWM_Group_TypeDef Group,u16 PwmPeriod);						//PWM周期设置
void PWM_DutyCycleConfig(PWM_Group_TypeDef Group,u16 PwmDuty);						//PWM占空比设置
void PWM_DeadZoneConfig(PWM_Group_TypeDef Group,u16 PwmDeadZone);					//PWM死区时间设置
void PWM_Cmd(PWM_Group_TypeDef Group,FunctionalState NewState);						//使能或失能PWM模块
void PWM_ITCmd(PWM_Group_TypeDef Group,FunctionalState NewState);					//使能或失能PWM中断
FlagStatus PWM_GetITFlag(PWM_Group_TypeDef Group);									//获取PWM中断标志位
void PWM_ClearITFlag(PWM_Group_TypeDef Group);										//清除PWM中断标志位	
FlagStatus PWM_GetFLTFlag(PWM_Group_TypeDef Group);									//获取PWM故障状态，关闭中/正常运行
void PWM_ITPriorityConfig(PWM_ITPriority_TypeDef ITPriority);						//PWM中断优先级配置
							
#endif