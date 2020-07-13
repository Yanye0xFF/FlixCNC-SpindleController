#ifndef __hc89s_TIMER3_H__
#define __hc89s_TIMER3_H__

#include "HC89S003F4.h"

/*��ʱ��3ʱ�ӷ�Ƶϵ��ö����*/
typedef enum
{
	TIM3_CLK_1		= ((u8)0x00),	//ʱ��1��Ƶ
	TIM3_CLK_8		= ((u8)0x10),	//ʱ��8��Ƶ
	TIM3_CLK_64		= ((u8)0x20),	//ʱ��64��Ƶ
	TIM3_CLK_256	= ((u8)0x30),	//ʱ��256��Ƶ	
}TIM3_ClkPrescaler_Typedef;

/*��ʱ��3ʱ��Դö����*/
typedef enum
{
	TIM3_SYSCLK		= ((u8)0x00),	//OSCʱ��Դ
	TIM3_OUTCLK		= ((u8)0x01),	//T3�˿�ʱ��Դ
}TIM3_ClkSource_Typedef;

/*TIM3����˿�ö����*/
typedef enum {
  	TIM3_OUTPin_P00       	= (u8)0x00,	//TIM3����˿�ΪP00
  	TIM3_OUTPin_P01       	= (u8)0x01,	//TIM3����˿�ΪP01
  	TIM3_OUTPin_P02       	= (u8)0x02,	//TIM3����˿�ΪP02
  	TIM3_OUTPin_P03  		= (u8)0x03,	//TIM3����˿�ΪP03
  	TIM3_OUTPin_P04  		= (u8)0x04,	//TIM3����˿�ΪP04
  	TIM3_OUTPin_P05  		= (u8)0x05,	//TIM3����˿�ΪP05
  	TIM3_OUTPin_P06  		= (u8)0x06,	//TIM3����˿�ΪP06
	TIM3_OUTPin_P07			= (u8)0x07,	//TIM3����˿�ΪP07
	TIM3_OUTPin_P10			= (u8)0x10,	//TIM3����˿�ΪP10
	TIM3_OUTPin_P11			= (u8)0x11,	//TIM3����˿�ΪP11
	TIM3_OUTPin_P20		    = (u8)0x20,	//TIM3����˿�ΪP20
	TIM3_OUTPin_P21			= (u8)0x21,	//TIM3����˿�ΪP21
	TIM3_OUTPin_P22			= (u8)0x22,	//TIM3����˿�ΪP22
	TIM3_OUTPin_P23			= (u8)0x23,	//TIM3����˿�ΪP23
	TIM3_OUTPin_P24			= (u8)0x24,	//TIM3����˿�ΪP24
	TIM3_OUTPin_P25			= (u8)0x25,	//TIM3����˿�ΪP25
	TIM3_OUTPin_P26			= (u8)0x26,	//TIM3����˿�ΪP26
	TIM3_OUTPin_P27			= (u8)0x27,	//TIM3����˿�ΪP27
}TIM3_OutPin_TypeDef;

/*TIM3�ж����ȼ�ö����*/
typedef enum {
	TIM3_ITPriority_0		= (u8)0x00, //TIM3�ж����ȼ�0(���)
	TIM3_ITPriority_1		= (u8)0x04, //TIM3�ж����ȼ�1
	TIM3_ITPriority_2		= (u8)0x08, //TIM3�ж����ȼ�2
	TIM3_ITPriority_3		= (u8)0x0C, //TIM3�ж����ȼ�3(���)
}TIM3_ITPriority_TypeDef;

/*******TIM3����********/
void TIM3_DeInit(void);												//�ָ�TIM3�Ĵ�����Ĭ��ֵ
void TIM3_Init(TIM3_ClkSource_Typedef ClkSource,TIM3_ClkPrescaler_Typedef ClkPrescaler,FunctionalState NewState);//����ָ���Ĳ�����ʼ��TIM3
void TIM3_TimerCnt(u16 Value);										//TIM3��ʱʱ������
void TIM3_PinRemapConfig(TIM3_OutPin_TypeDef OutPin);				//����TIM3����Ĺܽ�ӳ��
void TIM3_PDModeCmd(FunctionalState NewState);						//ʹ�ܻ�ʧ��TIM3����ģʽ������
void TIM3_ITCmd(FunctionalState NewState);							//ʹ�ܻ�ʧ��TIM3�ж�
void TIM3_Cmd(FunctionalState NewState);							//ʹ�ܻ�ʧ��TIM3
FlagStatus TIM3_GetITFlag(void);									//��ȡTIM3�жϱ�־״̬
void TIM3_ClearITFlag(void);										//���TIM3�жϱ�־״̬
void TIM3_ITPriorityConfig(TIM3_ITPriority_TypeDef ITPriority);		//TIM3�ж����ȼ�����

#endif