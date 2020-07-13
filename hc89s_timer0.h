#ifndef __hc89s_TIMER0_H__
#define __hc89s_TIMER0_H__

#include "HC89S003F4.h"

/*��ʱ��1ʱ�ӷ�Ƶϵ��ö����*/
typedef enum
{
	TIM0_CLK_1						= ((u8)0x01),		//ʱ��1��Ƶ
	TIM0_CLK_12						= ((u8)0x00),		//ʱ��12��Ƶ
}TIM0_ClkPrescaler_Typedef;

/*��ʱ��1�ſ�ģʽö����*/
typedef enum
{
	TIM0_SOFT_CONTROL				= ((u8)0x00),		//ֻ������� TRx �������� Tx
	TIM0_SOFT_AND_HARD_CONTROL		= ((u8)0x08),		//ֻ���� INTx �˿ڵ�ƽΪ�ߵ�ƽʱ TRx �� 1�� Tx �Ź���
}TIM0_Gate_Typedef;

/*��ʱ��1��ʱ����ģʽö����*/
typedef enum
{
	TIM0_INTIMING					= ((u8)0x00),		//Tx �����ڲ���ʱ
	TIM0_OUTCLKCNT					= ((u8)0x04),		//Tx �����ⲿ����
}TIM0_CntOrTim_Typedef;

/*��ʱ��1ģʽö����*/
typedef enum
{
	TIM0_16BIT_AUTO_CNT_TIMING		= ((u8)0x00),		//16λ��װ�ض�ʱ��/������
	TIM0_16BIT_CNT_TIMING			= ((u8)0x01),		//16λ��ʱ��/������
	TIM0_8BIT_AUTO_CNT_TIMING		= ((u8)0x02),		//8λ�Զ���װ��ֵ��ʱ��/������
//	TIM0_TWO_8BIT_AUTO_CNT_TIMING	= ((u8)0x03),		//2·8λ�Զ���װ��ֵ��ʱ��/������
}TIM0_Mode_Typedef;

/*TIM0����˿�ö����*/
typedef enum {
  	TIM0_OUTPin_P00       	= (u8)0x00,	//TIM0����˿�ΪP00
  	TIM0_OUTPin_P01       	= (u8)0x01,	//TIM0����˿�ΪP01
  	TIM0_OUTPin_P02       	= (u8)0x02,	//TIM0����˿�ΪP02
  	TIM0_OUTPin_P03  		= (u8)0x03,	//TIM0����˿�ΪP03
  	TIM0_OUTPin_P04  		= (u8)0x04,	//TIM0����˿�ΪP04
  	TIM0_OUTPin_P05  		= (u8)0x05,	//TIM0����˿�ΪP05
  	TIM0_OUTPin_P06  		= (u8)0x06,	//TIM0����˿�ΪP06
	TIM0_OUTPin_P07			= (u8)0x07,	//TIM0����˿�ΪP07
	TIM0_OUTPin_P10			= (u8)0x10,	//TIM0����˿�ΪP10
	TIM0_OUTPin_P11			= (u8)0x11,	//TIM0����˿�ΪP11
	TIM0_OUTPin_P20		    = (u8)0x20,	//TIM0����˿�ΪP20
	TIM0_OUTPin_P21			= (u8)0x21,	//TIM0����˿�ΪP21
	TIM0_OUTPin_P22			= (u8)0x22,	//TIM0����˿�ΪP22
	TIM0_OUTPin_P23			= (u8)0x23,	//TIM0����˿�ΪP23
	TIM0_OUTPin_P24			= (u8)0x24,	//TIM0����˿�ΪP24
	TIM0_OUTPin_P25			= (u8)0x25,	//TIM0����˿�ΪP25
	TIM0_OUTPin_P26			= (u8)0x26,	//TIM0����˿�ΪP26
	TIM0_OUTPin_P27			= (u8)0x27,	//TIM0����˿�ΪP27
}TIM0_OutPin_TypeDef;

/*TIM0�ж����ȼ�ö����*/
typedef enum {
	TIM0_ITPriority_0		= (u8)0x00, //TIM0�ж����ȼ�0(���)
	TIM0_ITPriority_1		= (u8)0x04, //TIM0�ж����ȼ�1
	TIM0_ITPriority_2		= (u8)0x08, //TIM0�ж����ȼ�2)
	TIM0_ITPriority_3		= (u8)0x0C, //TIM0�ж����ȼ�3(���)
}TIM0_ITPriority_TypeDef;
												  	
void TIM0_DeInit(void);									//�ָ�TIM0�Ĵ�����Ĭ��ֵ
void TIM0_Init(TIM0_ClkPrescaler_Typedef ClkPrescaler,TIM0_Gate_Typedef Gate,TIM0_CntOrTim_Typedef CntOrTim,TIM0_Mode_Typedef Mode);//����ָ���Ĳ�����ʼ��TIM0
void TIM0_TimerCnt(u16 Value);							//TIM0��ʱʱ������
void TIM0_OutPutCmd(FunctionalState NewState);			//TIM0�˿����ʹ��/ʧ��
void TIM0_PinRemapConfig(TIM0_OutPin_TypeDef OutPin);	//����TIM0����Ĺܽ�ӳ��
void TIM0_Cmd(FunctionalState NewState);				//ʹ�ܻ�ʧ��TIM0
void TIM0_ITCmd(FunctionalState NewState);				//ʹ�ܻ�ʧ��TIM0�ж�
FlagStatus TIM0_GetITFlag(void);						//��ȡTIM0�жϱ�־״̬
void TIM0_ClearITFlag(void);							//���TIM0�жϱ�־״̬
void TIM0_ITPriorityConfig(TIM0_ITPriority_TypeDef ITPriority);	//TIM0�ж����ȼ�����

#endif








