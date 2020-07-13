#ifndef __HC89S_INIT_H__
#define __HC89S_INIT_H__

#include "HC89S003F4.h"

/*�ⲿ�жϴ���ö����*/
typedef enum {
  	EXTI_SENSITIVITY_LOW_ONLY  		= (u8)0x00,		//�͵�ƽ����
  	EXTI_SENSITIVITY_FALL_ONLY  	= (u8)0x01,		//�½��ش���
  	EXTI_SENSITIVITY_RISE_ONLY  	= (u8)0x02,		//�����ش���
  	EXTI_SENSITIVITY_RISE_FALL  	= (u8)0x03,		//˫�ش���
} EXTI_Sensitivity_TypeDef;

/*�ⲿ�ж�Դö����*/
typedef enum {
	INT0	= (u8)0,								//�ⲿ�ж�0	
	INT1	= (u8)2,								//�ⲿ�ж�1
	INT2	= (u8)4,								//�ⲿ�ж�2
	INT3	= (u8)6,								//�ⲿ�ж�3
	INT4	= (u8)8,								//�ⲿ�ж�4
	INT5	= (u8)10,								//�ⲿ�ж�5
	INT6	= (u8)12,								//�ⲿ�ж�6
	INT7	= (u8)14,								//�ⲿ�ж�7
	INT8	= (u8)16,								//�ⲿ�ж�8
	INT9   	= (u8)18,								//�ⲿ�ж�9
	INT10   = (u8)20,								//�ⲿ�ж�10
	INT11	= (u8)22,								//�ⲿ�ж�11
	INT12	= (u8)24,								//�ⲿ�ж�12
	INT13	= (u8)26,								//�ⲿ�ж�13
	INT14	= (u8)28,								//�ⲿ�ж�14
	INT15	= (u8)30,								//�ⲿ�ж�15
} EXTI_Number_TypeDef;


/*INT0�ܽ�ѡ��Դö����*/
typedef enum {
	INT0_P00	= (u8)0x00,							//INT0�ܽ�ѡ��P00	
	INT0_P10	= (u8)0x01,							//INT0�ܽ�ѡ��P10
} EXTI_INT0PinChoose_TypeDef;		   	

/*INT1�ܽ�ѡ��Դö����*/
typedef enum {
	INT1_P01	= (u8)0x00,							//INT1�ܽ�ѡ��P01
	INT1_P11	= (u8)0x02,							//INT1�ܽ�ѡ��P11
} EXTI_INT1PinChoose_TypeDef;

/*INT�ж����ȼ�Դö����*/
typedef enum {
	EXTI_ITPriority_0	= (u8)0x00,					//���ȼ�0(���)
	EXTI_ITPriority_1	= (u8)0x01,					//���ȼ�1
	EXTI_ITPriority_2	= (u8)0x02,					//���ȼ�2
	EXTI_ITPriority_3	= (u8)0x03,					//���ȼ�3(���)
} EXTI_ITPriority_TypeDef;


/*******�ⲿ�жϺ���********/
void EXTI_DeInit(void);												//�ָ��ⲿ�жϼĴ�����Ĭ��ֵ
void EXTI_Init(EXTI_Number_TypeDef Number,EXTI_Sensitivity_TypeDef Sensitivity);//����ָ���Ĳ�����ʼ���ⲿ�ж�
void EXTI_Cmd(EXTI_Number_TypeDef Number,FunctionalState NewState);	//ʹ�ܻ�ʧ���ⲿ�ж�
FlagStatus EXTI_GetFlag(EXTI_Number_TypeDef Number);				//��ȡ�ⲿ�жϵı�־״̬
void EXTI_ClearFlag(EXTI_Number_TypeDef Number);					//����ⲿ�жϱ�־λ
void EXTI_INT0PinChoose(EXTI_INT0PinChoose_TypeDef INT0PinChoose);	//INT0�ܽ�ѡ��
void EXTI_INT1PinChoose(EXTI_INT1PinChoose_TypeDef INT1PinChoose);	//INT1�ܽ�ѡ��
void EXTI_ITPriorityConfig(EXTI_Number_TypeDef Number,EXTI_ITPriority_TypeDef ITPriority);//����EXTI�ж����ȼ�

#endif