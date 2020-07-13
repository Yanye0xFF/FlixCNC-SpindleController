/**
*   ************************************************************************************
*								上海芯圣电子股份有限公司
*								    www.holychip.cn
*	************************************************************************************
*	@File				hc89s_gpio.h
*	@Library Version	V1.0.0.0
*	@Date				2017.08.18
*	************************************************************************************
*  								       客户服务
*	 感谢您使用我们的单片机，若发现错误或对函数的使用存在疑问。请添加上海芯圣电子官方QQ
*	****************************技术支持QQ：201030494**********************************
*   ************************************************************************************
**/

#ifndef __HC89S_GPIO_H__
#define __HC89S_GPIO_H__

#include "HC89S003F4.h"

typedef enum
{
	GPIO_MODE_IN      		= ((u8)0x00), 	//数字输入
	GPIO_MODE_IN_PD      	= ((u8)0x01), 	//带下拉输入
	GPIO_MODE_IN_PU         = ((u8)0x02), 	//带上拉输入
	GPIO_MODE_IN_AN        	= ((u8)0x03), 	//模拟输入
	GPIO_MODE_IN_SMT  		= ((u8)0x04), 	//数字输入   SMT
	GPIO_MODE_IN_PD_SMT  	= ((u8)0x05), 	//带下拉输入 SMT
	GPIO_MODE_IN_PU_SMT  	= ((u8)0x06), 	//带上拉输入 SMT
	GPIO_MODE_IN_AN_PU_PD	= ((u8)0x07), 	//模拟输入上下拉同时打开，B版以后芯片支持
	GPIO_MODE_OUT_PP 		= ((u8)0x08), 	//推挽输出
	GPIO_MODE_OUT_OD  		= ((u8)0x09), 	//开漏输出
	GPIO_MODE_OUT_OD_PU 	= ((u8)0x0a)  	//开漏带上拉输出
}GPIO_Mode_TypeDef;

typedef enum
{
	GPIO_PIN_0    			= ((u8)0x01),  	//选择端口0
	GPIO_PIN_1    			= ((u8)0x02),  	//选择端口1
	GPIO_PIN_2    			= ((u8)0x04),  	//选择端口2
	GPIO_PIN_3    			= ((u8)0x08),  	//选择端口3
	GPIO_PIN_4    			= ((u8)0x10),  	//选择端口4
	GPIO_PIN_5    			= ((u8)0x20),  	//选择端口5
	GPIO_PIN_6    			= ((u8)0x40),  	//选择端口6
	GPIO_PIN_7    			= ((u8)0x80)  	//选择端口7
}GPIO_Pin_TypeDef;

typedef enum
{
	GPIOT0 					= ((u8)0x01),	//端口0组
	GPIOT1 					= ((u8)0x02),	//端口1组
	GPIOT2 					= ((u8)0x03)	//端口2组
}GPIO_TypeDef; 

typedef enum
{
	R050K 					= ((u8)0x00),	//上拉电阻50K
	R100K 					= ((u8)0x10),	//上拉电阻100K
	R150K 					= ((u8)0x20),	//上拉电阻150K
	R300K 					= ((u8)0x30)	//上拉电阻300K
}GPIO_PullResValve;

typedef enum
{
	FOSC_1  				= ((u8)0x00),	//消抖时钟1分频
	FOSC_4  				= ((u8)0x40),	//消抖时钟4分频
	FOSC_16 				= ((u8)0x80),	//消抖时钟16分频
	FOSC_64 				= ((u8)0xC0)	//消抖时钟64分频
}GPIO_DbcClockDevice;


/*******GPIO函数********/
void GPIO_DeInit(GPIO_TypeDef GPIOx);																//恢复GPIOx寄存器到默认值
void GPIO_Init(GPIO_TypeDef GPIOx,GPIO_Pin_TypeDef GPIO_PIN,GPIO_Mode_TypeDef GPIO_Mode);			//对一组端口中的一个或多个IO设置模式
void GPIO_Write(GPIO_TypeDef GPIOx,u8 Value);														//对一组端口赋值
void GPIO_WriteHigh(GPIO_TypeDef GPIOx,GPIO_Pin_TypeDef GPIO_PIN);									//对一组端口中的PIN置高
void GPIO_WriteLow(GPIO_TypeDef GPIOx,GPIO_Pin_TypeDef GPIO_PIN);									//对一组端口中的PIN置低
void GPIO_WriteReverse(GPIO_TypeDef GPIOx,GPIO_Pin_TypeDef GPIO_PIN);								//对一组端口中的PIN取反
u8 	 GPIO_ReadInputData(GPIO_TypeDef GPIOx);														//获取当前端口组的输入状态
u8 	 GPIO_ReadOutputData(GPIO_TypeDef GPIOx);														//获取当前端口组的输出状态
bool GPIO_ReadInputPin(GPIO_TypeDef GPIOx,GPIO_Pin_TypeDef GPIO_PIN);								//获取一组端口中的一个端口的状态
void GPIO_P02ExternalPullConfig(GPIO_PullResValve Res_Value);										//设置P02端口上拉电阻阻值
void GPIO_GPIO0DbcTimerConfig(GPIO_Pin_TypeDef GPIO_PIN,GPIO_DbcClockDevice ClockDevice,u8 Value);	//设置端口消抖功能的消抖时间只有P00，P01，P02有此功能



#endif