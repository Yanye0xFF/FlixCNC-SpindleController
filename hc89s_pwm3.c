/**
*   ************************************************************************************
*								上海芯圣电子股份有限公司
*								    www.holychip.cn
*	************************************************************************************
*	@File				hc89s_pwm3.c
*	@Library Version	V1.0.0.0
*	@Date				2017.08.18
*	************************************************************************************
*									模块性能介绍
*	1、HC89S003拥有一个单路8位PWM3模块，PWM3提供周期溢出中断。
*	2、PWM3周期：[PWM3P]*PWM3工作时钟源周期。
*	3、PWM3占空比：[PWM3D]*PWM3工作时钟源周期。
*	************************************************************************************
*									 应用注意事项
*	1、占空比0%（PWM3D[0-7]=0x00）输出低电平，占空比100%（PWM3P≤PWM3D）输出高电平。
*	2、关闭PWM3模块时，PWM计数停止，输出立即关闭。再打开PWM3模块时，PWM计数器都重新从1开
*	   始计数，输出受PWM3OEN控制。
*	3、PWM3允许输出，必须在PWM3EN置1下才有效，否则为PWM3输出关闭状态（输出时对应端口必须
*	   设为输出模式）；即使都禁止输出，只要相关位被使能，PWM3都可以溢出中断，即此时PWM3
*	   可以作为定时器/计数器使用，此控制位修改立即生效。
*	************************************************************************************
*								      函数总览
*	PWM3_DeInit()						//恢复PWM3寄存器到默认值
*	PWM3_Init();						//根据指定的参数初始化PWM3
*	PWM3_OutPutCmd()					//PWM3端口输出使能/失能
*	PWM3_PeriodConfig()					//PWM3周期设置
*	PWM3_DutyCycleConfig()				//PWM3占空比设置
*	PWM3_PinRemapConfig()				//PWM3输出管脚映射
*	PWM3_Cmd()							//使能或失能PWM3模块
*	PWM3_ITCmd()						//使能或失能PWM3中断
*	PWM3_GetITFlag()					//获取PWM3中断标志位
*	PWM3_ClearITFlag()					//清除PWM3中断标志位
*	PWM3_ITPriorityConfig();				//PWM中断优先级配置
*	************************************************************************************
*									   计算公式
*	1、周期计算。
*	   周期 =  PWM3P[7:0] / (Fosc / PWM分频系数)		
*	2、有效电平时间计算（即占空比）。
*	   有效电平时间 = PWM3D[7:0] / (Fosc / PWM分频系数)
*	   占空比 = 有效电平时间 / 周期
*	************************************************************************************
*  								       客户服务
*	 感谢您使用我们的单片机，若发现错误或对函数的使用存在疑问。请添加上海芯圣电子官方QQ。
*	****************************技术支持群：201030494**********************************
*   ************************************************************************************
**/

#include "hc89s_pwm3.h"

/**
  * @说明  	恢复PWM3寄存器到默认值
  * @参数  	无
  * @返回值 无
  * @注 	无
  */
void PWM3_DeInit(void)
{
	PWM3C = PWM3_PWM3C_RESET_VALUE;
	PWM3P = PWM3_PWM3P_RESET_VALUE;
	PWM3D = PWM3_PWM3D_RESET_VALUE;
	PWM3_MAP = PWMX_PWM3MAP_RESET_VALUE;
}

/**
  * @说明  	根据指定的参数初始化PWM3
  * @参数  	VoltValid ：	PWM有效电平
  *         PWM3_HIGH_VALID	高电平为有效电平
  *			PWM3_LOW_VALID	低电平为有效电平 （有效期间是指占空比期间。）
  * @参数  	ClkDevice ：	PWM3时钟分频系数
  *         PWMCK_1			PWM时钟为osc时钟/1
  *         PWMCK_2			PWM时钟为osc时钟/2
  *         PWMCK_4			PWM时钟为osc时钟/4
  *         PWMCK_8			PWM时钟为osc时钟/8
  *         PWMCK_16		PWM时钟为osc时钟/16
  *         PWMCK_32		PWM时钟为osc时钟/32
  *         PWMCK_64		PWM时钟为osc时钟/64
  *         PWMCK_128		PWM时钟为osc时钟/128
  * @返回值 无
  * @注 	无
  */
void PWM3_Init(PWM3_VoltValid_TypeDef VoltValid,PWM3_ClkDevice_TypeDef ClkDevice)
{
	PWM3C = (PWM3C&(~(PWM3_S|PWM3_PTCK3)))|(VoltValid&PWM3_S)|(ClkDevice&PWM3_PTCK3);		
}

/**
  * @说明  	PWM3端口输出使能/失能
  * @参数  	NewState ：PWM3通道输出新状态
  *			DISABLE 	//失能
  *			ENABLE 		//使能
  * @返回值 无
  * @注 	无
  */
void PWM3_OutPutCmd(FunctionalState NewState)
{
	if(NewState)PWM3C |= PWM3_OEN;
	else PWM3C &=~ PWM3_OEN;
}


/**
  * @说明  	配置PWM3输出的管脚映射
  * @参数  	OutPin ：管脚输出源
  *			PWM3_OutPin_P00  	//PWM3输出端口为P00
  *			PWM3_OutPin_P01  	//PWM3输出端口为P01
  *			PWM3_OutPin_P02  	//PWM3输出端口为P02
  *			PWM3_OutPin_P03  	//PWM3输出端口为P03
  *			PWM3_OutPin_P04  	//PWM3输出端口为P04
  *			PWM3_OutPin_P05  	//PWM3输出端口为P05
  *			PWM3_OutPin_P06  	//PWM3输出端口为P06
  *			PWM3_OutPin_P07		//PWM3输出端口为P07
  *			PWM3_OutPin_P10		//PWM3输出端口为P10
  * 		PWM3_OutPin_P11		//PWM3输出端口为P11
  *			PWM3_OutPin_P20		//PWM3输出端口为P20
  *			PWM3_OutPin_P21		//PWM3输出端口为P21
  *			PWM3_OutPin_P22		//PWM3输出端口为P22
  *			PWM3_OutPin_P23		//PWM3输出端口为P23
  *			PWM3_OutPin_P24		//PWM3输出端口为P24
  *			PWM3_OutPin_P25		//PWM3输出端口为P25
  *			PWM3_OutPin_P26		//PWM3输出端口为P26
  *			PWM3_OutPin_P27		//PWM3输出端口为P27
  * @返回值 无
  * @注 	无
  */
void PWM3_PinRemapConfig(PWM3_OutPin_TypeDef OutPin)
{
	PWM3_MAP = OutPin;
}
 
/**
  * @说明  	PWM3周期设置
  * @参数  	PwmDeadZone ：PWM周期
  *         取值范围 0 - 0xFF
  * @返回值 无
  * @注 	周期时间 = PwmPeriod / (Fosc / PWM分频系数)
  */
void PWM3_PeriodConfig(u8 PwmPeriod)
{
	PWM3P = PwmPeriod;
}


/**
  * @说明  	PWM3占空比设置
  * @参数  	PwmDeadZone ：PWM3占空比
  *         取值范围 0 - 0xFF
  * @返回值 无
  * @注 	无
  */
void PWM3_DutyCycleConfig(u8 PwmDuty)
{
	PWM3D = PwmDuty;	
}
											
/**
  * @说明  	使能或失能PWM3模块
  * @参数  	NewState ：PWM3模块新状态
  *			DISABLE 	//失能
  *			ENABLE 		//使能
  * @返回值 无
  * @注 	无
  */
void PWM3_Cmd(FunctionalState NewState)
{
	if(NewState == ENABLE)PWM3C |= PWM3_EN;	
	else PWM3C &=~ PWM3_EN;
}


/**
  * @说明  	使能或失能PWM3中断
  * @参数  	NewState ：PWM3中断功能新状态
  *			DISABLE 	//失能
  *			ENABLE 		//使能
  * @返回值 无
  * @注 	无
  */
void PWM3_ITCmd(FunctionalState NewState)
{
	if(NewState)PWM3C |= PWM3_IE;
	else PWM3C &=~ PWM3_IE;
}

/**
  * @说明  	获取PWM3中断标志位
  * @参数  	无
  * @返回值 FlagStatus ：表明PWM3中断状态
  *			参考 FlagStatus 枚举类
  * @注 	无
  */
FlagStatus PWM3_GetITFlag(void)
{
	if(PWM3C & PWM3_IF)return SET;
	else return RESET;
}

/**
  * @说明  	清除PWM3中断标志位
  * @参数  	无
  * @返回值 无
  * @注 	需要软件清除PWM3中断标志位
  */
void PWM3_ClearITFlag(void)
{
	PWM3C &=~ PWM3_IF;
}

/**
  * @说明  	PWM设置中断优先级
  * @参数  	ITPriority ：PWM中断优先级
  *			PWM3_ITPriority_0	//PWM中断优先级0(最低)
  *			PWM3_ITPriority_1	//PWM中断优先级1
  *			PWM3_ITPriority_2	//PWM中断优先级2
  *			PWM3_ITPriority_3	//PWM中断优先级3(最高)
  * @返回值 无
  * @注 	PWM0，PWM0，PWM0，PWM3共用同一中断向量
  */
void PWM3_ITPriorityConfig(PWM3_ITPriority_TypeDef ITPriority)
{
	IP2 = (IP2&(~INT_PPWM))|ITPriority;		
}

















