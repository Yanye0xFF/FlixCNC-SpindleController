/**
*   ************************************************************************************
*								�Ϻ�оʥ���ӹɷ����޹�˾
*								    www.holychip.cn
*	************************************************************************************
*	@File				hc89s_gpio.h
*	@Library Version	V1.0.0.0
*	@Date				2017.08.18
*	************************************************************************************
*  								       �ͻ�����
*	 ��л��ʹ�����ǵĵ�Ƭ���������ִ����Ժ�����ʹ�ô������ʡ�������Ϻ�оʥ���ӹٷ�QQ
*	****************************����֧��QQ��201030494**********************************
*   ************************************************************************************
**/

#ifndef __HC89S_GPIO_H__
#define __HC89S_GPIO_H__

#include "HC89S003F4.h"

typedef enum
{
	GPIO_MODE_IN      		= ((u8)0x00), 	//��������
	GPIO_MODE_IN_PD      	= ((u8)0x01), 	//����������
	GPIO_MODE_IN_PU         = ((u8)0x02), 	//����������
	GPIO_MODE_IN_AN        	= ((u8)0x03), 	//ģ������
	GPIO_MODE_IN_SMT  		= ((u8)0x04), 	//��������   SMT
	GPIO_MODE_IN_PD_SMT  	= ((u8)0x05), 	//���������� SMT
	GPIO_MODE_IN_PU_SMT  	= ((u8)0x06), 	//���������� SMT
	GPIO_MODE_IN_AN_PU_PD	= ((u8)0x07), 	//ģ������������ͬʱ�򿪣�B���Ժ�оƬ֧��
	GPIO_MODE_OUT_PP 		= ((u8)0x08), 	//�������
	GPIO_MODE_OUT_OD  		= ((u8)0x09), 	//��©���
	GPIO_MODE_OUT_OD_PU 	= ((u8)0x0a)  	//��©���������
}GPIO_Mode_TypeDef;

typedef enum
{
	GPIO_PIN_0    			= ((u8)0x01),  	//ѡ��˿�0
	GPIO_PIN_1    			= ((u8)0x02),  	//ѡ��˿�1
	GPIO_PIN_2    			= ((u8)0x04),  	//ѡ��˿�2
	GPIO_PIN_3    			= ((u8)0x08),  	//ѡ��˿�3
	GPIO_PIN_4    			= ((u8)0x10),  	//ѡ��˿�4
	GPIO_PIN_5    			= ((u8)0x20),  	//ѡ��˿�5
	GPIO_PIN_6    			= ((u8)0x40),  	//ѡ��˿�6
	GPIO_PIN_7    			= ((u8)0x80)  	//ѡ��˿�7
}GPIO_Pin_TypeDef;

typedef enum
{
	GPIOT0 					= ((u8)0x01),	//�˿�0��
	GPIOT1 					= ((u8)0x02),	//�˿�1��
	GPIOT2 					= ((u8)0x03)	//�˿�2��
}GPIO_TypeDef; 

typedef enum
{
	R050K 					= ((u8)0x00),	//��������50K
	R100K 					= ((u8)0x10),	//��������100K
	R150K 					= ((u8)0x20),	//��������150K
	R300K 					= ((u8)0x30)	//��������300K
}GPIO_PullResValve;

typedef enum
{
	FOSC_1  				= ((u8)0x00),	//����ʱ��1��Ƶ
	FOSC_4  				= ((u8)0x40),	//����ʱ��4��Ƶ
	FOSC_16 				= ((u8)0x80),	//����ʱ��16��Ƶ
	FOSC_64 				= ((u8)0xC0)	//����ʱ��64��Ƶ
}GPIO_DbcClockDevice;


/*******GPIO����********/
void GPIO_DeInit(GPIO_TypeDef GPIOx);																//�ָ�GPIOx�Ĵ�����Ĭ��ֵ
void GPIO_Init(GPIO_TypeDef GPIOx,GPIO_Pin_TypeDef GPIO_PIN,GPIO_Mode_TypeDef GPIO_Mode);			//��һ��˿��е�һ������IO����ģʽ
void GPIO_Write(GPIO_TypeDef GPIOx,u8 Value);														//��һ��˿ڸ�ֵ
void GPIO_WriteHigh(GPIO_TypeDef GPIOx,GPIO_Pin_TypeDef GPIO_PIN);									//��һ��˿��е�PIN�ø�
void GPIO_WriteLow(GPIO_TypeDef GPIOx,GPIO_Pin_TypeDef GPIO_PIN);									//��һ��˿��е�PIN�õ�
void GPIO_WriteReverse(GPIO_TypeDef GPIOx,GPIO_Pin_TypeDef GPIO_PIN);								//��һ��˿��е�PINȡ��
u8 	 GPIO_ReadInputData(GPIO_TypeDef GPIOx);														//��ȡ��ǰ�˿��������״̬
u8 	 GPIO_ReadOutputData(GPIO_TypeDef GPIOx);														//��ȡ��ǰ�˿�������״̬
bool GPIO_ReadInputPin(GPIO_TypeDef GPIOx,GPIO_Pin_TypeDef GPIO_PIN);								//��ȡһ��˿��е�һ���˿ڵ�״̬
void GPIO_P02ExternalPullConfig(GPIO_PullResValve Res_Value);										//����P02�˿�����������ֵ
void GPIO_GPIO0DbcTimerConfig(GPIO_Pin_TypeDef GPIO_PIN,GPIO_DbcClockDevice ClockDevice,u8 Value);	//���ö˿��������ܵ�����ʱ��ֻ��P00��P01��P02�д˹���



#endif