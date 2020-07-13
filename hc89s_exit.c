/**
*									 模块性能介绍
*	1、HC89S003总共提供了16个外部中断，可以设置4级中断的优先级。
*	2、有4个外部中断向量入口，INT0/INT1分别有一个独立的中断向量入口，INT2-7共用一个中断
*	   向量入口，INT8-15共用一个中断向量入口。
*	3、所有外部中断都可以设定4种触发方式，分别为上升沿、下降沿、双沿以及低电平。
*	4、INT0、INT1中断标志位硬件自动清0，INT2-INT15需软件清0。
*	************************************************************************************
*									 应用注意事项
*	1、只要相应的EINTx(x=2...15)被允许，对应的中断标志就可能被置1，反之，对应标志不会被置1。
*	2、外部中断在使用查询方式时，无法正常清除中断标志位。可以通过先禁止中断使能位再清除
*	   中断标志，在完成清除中断标志位后再使能外部中断来进行正常的外部中断产生查询。使用
*	   中断方式时，无该问题。
*	************************************************************************************
*								       函数总览
*	EXTI_DeInit()						//恢复外部中断寄存器到默认值
*	EXTI_Init()							//根据指定的参数初始化外部中断
*	EXTI_Cmd()							//使能或失能外部中断
*	EXTI_GetFlag()						//获取外部中断的标志状态
*	EXTI_ClearFlag()					//清除外部中断标志位
*	EXTI_INT0PinChoose()				//INT0管脚选择
*	EXTI_INT1PinChoose()				//INT1管脚选择
*	EXTI_ITPriorityConfig()				//设置EXTI中断优先级
*	************************************************************************************
**/


#include "hc89s_exit.h"

/**
  * @说明  	恢复外部中断寄存器到默认值
  * @参数  	无
  * @返回值 无
  * @注		无
  */
void EXTI_DeInit(void)
{
	PITS  = INT_PITS_RESET_VALUE;
	PINTE =	INT_PINTE_RESET_VALUE;
	PINTF = INT_PINTF_RESET_VALUE;
}

/**
  * @说明  	根据指定的参数初始化外部中断
  * @参数  	Number ：外部中断通道
  *			INT0	//外部中断0	
  *			INT1	//外部中断1
  *			INT2	//外部中断2
  *			INT3	//外部中断3
  *			INT4	//外部中断4
  *			INT5	//外部中断5
  *			INT6	//外部中断6
  *			INT7	//外部中断7
  *			INT8	//外部中断8
  *			INT9 	//外部中断9
  *			INT10	//外部中断10
  *			INT11	//外部中断11
  *			INT12	//外部中断12
  *			INT13	//外部中断13
  *			INT14	//外部中断14
  *			INT15	//外部中断15
  * @参数  	Sensitivity ：外部中断触发方式
  *		  	EXTI_SENSITIVITY_LOW_ONLY	//低电平触发
  *		  	EXTI_SENSITIVITY_FALL_ONLY	//下降沿触发
  *		  	EXTI_SENSITIVITY_RISE_ONLY	//上升沿触发
  *		  	EXTI_SENSITIVITY_RISE_FALL	//双沿触发
  * @返回值 无
  * @注 	无
  */
void EXTI_Init(EXTI_Number_TypeDef Number,EXTI_Sensitivity_TypeDef Sensitivity)
{
	PITS = Tranverse32((PITS & (~(0x00000003<<Number))) |((0x00000003&(u32)Sensitivity)<<Number));
}

/**
  * @说明  	使能或失能外部中断
  * @参数  	Number ：外部中断通道
  *			INT0	//外部中断0	
  *			INT1	//外部中断1
  *			INT2	//外部中断2
  *			INT3	//外部中断3
  *			INT4	//外部中断4
  *			INT5	//外部中断5
  *			INT6	//外部中断6
  *			INT7	//外部中断7
  *			INT8	//外部中断8
  *			INT9 	//外部中断9
  *			INT10	//外部中断10
  *			INT11	//外部中断11
  *			INT12	//外部中断12
  *			INT13	//外部中断13
  *			INT14	//外部中断14
  *			INT15	//外部中断15
  * @参数  	NewState ：外部中断Number通道新状态
			DISABLE		//失能
			ENABLE		//使能
  * @返回值 无
  * @注 	无
  */
void EXTI_Cmd(EXTI_Number_TypeDef Number,FunctionalState NewState)
{
	if(NewState)
	{
		if(Number == 0)IE |= INT_EX0;
		else if(Number == 2)IE |= INT_EX1;
		else if(Number <= INT7){
			IE1 |= INT_EX2_7;
			PINTE |= Tranverse16(0x0001 << (Number/2));	
		}
		else if(Number <= INT15){
			IE1 |= INT_EX8_15;
			PINTE |= Tranverse16(0x0001 << (Number/2));	
		}
	}
	else
	{
		if(Number == 0)IE &=~ INT_EX0;
		else if(Number == 2)IE &=~ INT_EX1;
		else if(Number <= INT7){
			IE1 &=~ INT_EX2_7;
			PINTE &=~ Tranverse16(0x0001 << (Number/2));	
		}
		else if(Number <= INT15){
			IE1 &=~ INT_EX8_15;
			PINTE &=~ Tranverse16(0x0001 << (Number/2));	
		}
	}
}

/**
  * @说明  	获取外部中断的标志状态
  * @参数  	Number ：外部中断通道
  *			INT0	//外部中断0	
  *			INT1	//外部中断1
  *			INT2	//外部中断2
  *			INT3	//外部中断3
  *			INT4	//外部中断4
  *			INT5	//外部中断5
  *			INT6	//外部中断6
  *			INT7	//外部中断7
  *			INT8	//外部中断8
  *			INT9 	//外部中断9
  *			INT10	//外部中断10
  *			INT11	//外部中断11
  *			INT12	//外部中断12
  *			INT13	//外部中断13
  *			INT14	//外部中断14
  *			INT15	//外部中断15
  * @返回值 FlagStatus ：外部中断标志状态
  *			参考 FlagStatus 枚举类
  * @注 	无
  */
FlagStatus EXTI_GetFlag(EXTI_Number_TypeDef Number)
{
	if(PINTF&(0x0001<<(Number/2)))return SET;
	return RESET;
}

/**
  * @说明  	清除外部中断标志位
  * @参数  	Number ：外部中断通道
  *			INT0	//外部中断0	
  *			INT1	//外部中断1
  *			INT2	//外部中断2
  *			INT3	//外部中断3
  *			INT4	//外部中断4
  *			INT5	//外部中断5
  *			INT6	//外部中断6
  *			INT7	//外部中断7
  *			INT8	//外部中断8
  *			INT9 	//外部中断9
  *			INT10	//外部中断10
  *			INT11	//外部中断11
  *			INT12	//外部中断12
  *			INT13	//外部中断13
  *			INT14	//外部中断14
  *			INT15	//外部中断15
  * @返回值 无
  * @注 	无
  */
void EXTI_ClearFlag(EXTI_Number_TypeDef Number)
{
	PINTF &=~ (0x0001<<(Number/2));	
}

/**
  * @说明  	INT0管脚选择
  * @参数  	INT0PinChoose : INT0管脚选择
  *			INT0_P00	//INT0管脚选择P00	
  *			INT0_P10	//INT0管脚选择P10
  * @返回值 无
  * @注 	无
  */
void EXTI_INT0PinChoose(EXTI_INT0PinChoose_TypeDef INT0PinChoose)
{
	INT01_PINS = (INT01_PINS&(~INT_INT0_PINS))|INT0PinChoose;	
}

/**
  * @说明  	INT1管脚选择
  * @参数  	INT1PinChoose : INT1管脚选择
  *			INT1_P01	//INT1管脚选择P01
  *			INT1_P11	//INT1管脚选择P11
  * @返回值 无
  * @注 	无
  */
void EXTI_INT1PinChoose(EXTI_INT0PinChoose_TypeDef INT1PinChoose)
{
	INT01_PINS = (INT01_PINS&(~INT_INT1_PINS))|INT1PinChoose;	
}

/**
  * @说明  	EXTI中断优先级设置
  * @参数  	Number ：外部中断通道
  *			INT0	//外部中断0	
  *			INT1	//外部中断1
  *			INT2	//外部中断2
  *			INT3	//外部中断3
  *			INT4	//外部中断4
  *			INT5	//外部中断5
  *			INT6	//外部中断6
  *			INT7	//外部中断7
  *			INT8	//外部中断8
  *			INT9 	//外部中断9
  *			INT10	//外部中断10
  *			INT11	//外部中断11
  *			INT12	//外部中断12
  *			INT13	//外部中断13
  *			INT14	//外部中断14
  *			INT15	//外部中断15
  * @参数  	ITPriority ：中断优先级
  *			EXTI_ITPriority_0	//优先级0(最低)
  *			EXTI_ITPriority_1	//优先级1
  *			EXTI_ITPriority_2	//优先级2
  *			EXTI_ITPriority_3	//优先级3(最高)
  * @返回值 无
  * @注 	INT2-7共用一个控制位，INT8-15共用一个控制位
  */
void EXTI_ITPriorityConfig(EXTI_Number_TypeDef Number,EXTI_ITPriority_TypeDef ITPriority)
{
	if(Number == 0)IP0 = (IP0&(~INT_PX0))|ITPriority;
	else if(Number == 2)IP0 = (IP0&(~INT_PX1))|(ITPriority<<4);
	else if(Number <= INT7)IP3 = (IP3&(~INT_PX2_7))|(ITPriority<<4);
	else if(Number <= INT15)IP3 = (IP3&(~INT_PX8_15))|(ITPriority<<6);
}