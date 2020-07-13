#ifndef __HC89S_INIT_H__
#define __HC89S_INIT_H__

#include "HC89S003F4.h"

/*外部中断触发枚举类*/
typedef enum {
  	EXTI_SENSITIVITY_LOW_ONLY  		= (u8)0x00,		//低电平触发
  	EXTI_SENSITIVITY_FALL_ONLY  	= (u8)0x01,		//下降沿触发
  	EXTI_SENSITIVITY_RISE_ONLY  	= (u8)0x02,		//上升沿触发
  	EXTI_SENSITIVITY_RISE_FALL  	= (u8)0x03,		//双沿触发
} EXTI_Sensitivity_TypeDef;

/*外部中断源枚举类*/
typedef enum {
	INT0	= (u8)0,								//外部中断0	
	INT1	= (u8)2,								//外部中断1
	INT2	= (u8)4,								//外部中断2
	INT3	= (u8)6,								//外部中断3
	INT4	= (u8)8,								//外部中断4
	INT5	= (u8)10,								//外部中断5
	INT6	= (u8)12,								//外部中断6
	INT7	= (u8)14,								//外部中断7
	INT8	= (u8)16,								//外部中断8
	INT9   	= (u8)18,								//外部中断9
	INT10   = (u8)20,								//外部中断10
	INT11	= (u8)22,								//外部中断11
	INT12	= (u8)24,								//外部中断12
	INT13	= (u8)26,								//外部中断13
	INT14	= (u8)28,								//外部中断14
	INT15	= (u8)30,								//外部中断15
} EXTI_Number_TypeDef;


/*INT0管脚选择源枚举类*/
typedef enum {
	INT0_P00	= (u8)0x00,							//INT0管脚选择P00	
	INT0_P10	= (u8)0x01,							//INT0管脚选择P10
} EXTI_INT0PinChoose_TypeDef;		   	

/*INT1管脚选择源枚举类*/
typedef enum {
	INT1_P01	= (u8)0x00,							//INT1管脚选择P01
	INT1_P11	= (u8)0x02,							//INT1管脚选择P11
} EXTI_INT1PinChoose_TypeDef;

/*INT中断优先级源枚举类*/
typedef enum {
	EXTI_ITPriority_0	= (u8)0x00,					//优先级0(最低)
	EXTI_ITPriority_1	= (u8)0x01,					//优先级1
	EXTI_ITPriority_2	= (u8)0x02,					//优先级2
	EXTI_ITPriority_3	= (u8)0x03,					//优先级3(最高)
} EXTI_ITPriority_TypeDef;


/*******外部中断函数********/
void EXTI_DeInit(void);												//恢复外部中断寄存器到默认值
void EXTI_Init(EXTI_Number_TypeDef Number,EXTI_Sensitivity_TypeDef Sensitivity);//根据指定的参数初始化外部中断
void EXTI_Cmd(EXTI_Number_TypeDef Number,FunctionalState NewState);	//使能或失能外部中断
FlagStatus EXTI_GetFlag(EXTI_Number_TypeDef Number);				//获取外部中断的标志状态
void EXTI_ClearFlag(EXTI_Number_TypeDef Number);					//清除外部中断标志位
void EXTI_INT0PinChoose(EXTI_INT0PinChoose_TypeDef INT0PinChoose);	//INT0管脚选择
void EXTI_INT1PinChoose(EXTI_INT1PinChoose_TypeDef INT1PinChoose);	//INT1管脚选择
void EXTI_ITPriorityConfig(EXTI_Number_TypeDef Number,EXTI_ITPriority_TypeDef ITPriority);//设置EXTI中断优先级

#endif