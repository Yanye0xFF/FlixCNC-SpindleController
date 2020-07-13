/**
*   ************************************************************************************
*								�Ϻ�оʥ���ӹɷ����޹�˾
*								    www.holychip.cn
*	************************************************************************************
*	@File				hc89s_timer0.c
*	@Library Version	V1.0.0.0
*	@Date				2019.07.19
*	************************************************************************************
*									 ģ�����ܽ���
*	1��HC89S003�Ķ�ʱ��0��1�������ֹ�����ʽ��
*	   ��ʽ0��16λ�Զ���װ��ʱ��/��������
*	   ��ʽ1��16λ��ʱ��/��������
*	   ��ʽ2��8λ�Զ���װ��ʱ��/��������
*	   ��ʽ3��T0�ֳ�������TL0/TH0��������8λ��ʱ��/��������T1�޴�ģʽ����
*	************************************************************************************
*									 Ӧ��ע������
*	1����ʽ3ʱT0ռ��T1��TR1��TF1���ж�Դ������TR1��T0ռ�ã���ʱ��Ҫ�ر�T1�ɽ�T1��Ϊ����
*	   ��ʽ3��
*	2���ڷ�ʽ0��Mode0��ʱTHx(x = 0,1)��TLx(x = 0,1)��д������ѭ�ȸ�λ���λ���������޸�
*	   ��������ʱ����λ�������޸ı䣬ֻҪ��λ���޸ģ���λ��������д��һ�Σ���ʽ 1��2��3
*	   ʱ�޴�Ҫ��
*	************************************************************************************
*								       ��������
*	TIM0_DeInit();						//�ָ�TIM0�Ĵ�����Ĭ��ֵ
*	TIM0_Init();						//����ָ���Ĳ�����ʼ��TIM0
*	TIM0_TimerCnt();					//TIM0��ʱʱ������
*	TIM0_OutPutCmd();					//TIM0�˿����ʹ��/ʧ��
*	TIM0_Cmd();							//ʹ�ܻ�ʧ��TIM0
*	TIM0_PinRemapConfig()				//����TIM0����Ĺܽ�ӳ��
*	TIM0_ITCmd();						//ʹ�ܻ�ʧ��TIM0�ж�
*	TIM0_GetITFlag();					//��ȡTIM0�жϱ�־״̬
*	TIM0_ClearITFlag();					//���TIM0�жϱ�־״̬
*	TIM0_ITPriorityConfig();			//TIM0�ж����ȼ�����
*	************************************************************************************
*									   ���㹫ʽ
*	1��Tim0����ʱ��
*	   ��ʱʱ�� = (65536 - T0) * (1 / (Fosc /Timer��Ƶϵ��))
*	2��Tim0���Ƴ�ֵ
*	   ��ֵ = 65536 - ((��ʱʱ��) / (1/(Fosc / Timer��Ƶϵ��)))
*	************************************************************************************
*  								       �ͻ�����
*	 ��л��ʹ�����ǵĵ�Ƭ���������ִ����Ժ�����ʹ�ô������ʡ�������Ϻ�оʥ���ӹٷ�QQ
*	****************************����֧��Ⱥ��201030494**********************************
*   ************************************************************************************
**/

#include "hc89s_timer0.h"

/**
  * @˵��  	�ָ�TIM0�Ĵ�����Ĭ��ֵ
  * @����  	��
  * @����ֵ ��
  * @ע 	��
  */
void TIM0_DeInit(void)
{
	TMOD  &=~ (TIMER_GATE0|TIMER_CT0|TIMER_M0);
	TCON1 &=~ (TIMER_T0OUT|TIMER_T0X12);
	TCON  &=~ (TIMER_TF0|TIMER_TR0);
	T0_MAP = TIMER_T0MAP_RESET_VALUE;
	TH0 = TIMER_T0_RESET_VALUE>>8;
	TL0 = TIMER_T0_RESET_VALUE&0x00FF;
}

/**
  * @˵��  	����ָ���Ĳ�����ʼ��TIM0
  * @����  	ClkPrescaler ��TIM0ʱ�ӷ�Ƶϵ��
  *			TIM0_CLK_1		//ʱ��1��Ƶ
  *			TIM0_CLK_12		//ʱ��12��Ƶ
  * @����  	Gate ��TIM0�������/���Ӳ��ͬʱ����
  *			TIM0_SOFT_CONTROL			//ֻ������� TRx �������� Tx
  *			TIM0_SOFT_AND_HARD_CONTROL	//ֻ���� INTx �˿ڵ�ƽΪ�ߵ�ƽʱ TRx �� 1�� Tx �Ź���
  * @����  	CntOrTim ��TIM0����/��ʱ
  *			TIM0_INTIMING		//Tx �����ڲ���ʱ
  *			TIM0_OUTCLKCNT		//Tx �����ⲿ����
  * @����  	Mode ��TIM0����ģʽ
  *			TIM0_16BIT_AUTO_CNT_TIMING		//16λ��װ�ض�ʱ��/������
  *			TIM0_16BIT_CNT_TIMING			//16λ��ʱ��/������
  *			TIM0_8BIT_AUTO_CNT_TIMING		//8λ�Զ���װ��ֵ��ʱ��/������
  * @����ֵ ��
  * @ע 	��
  */
void TIM0_Init(TIM0_ClkPrescaler_Typedef ClkPrescaler,TIM0_Gate_Typedef Gate,TIM0_CntOrTim_Typedef CntOrTim,TIM0_Mode_Typedef Mode)
{
	TCON1 = (TCON1&(~TIMER_T0X12))|(ClkPrescaler&TIMER_T0X12);
	TMOD  = (TMOD&(~(TIMER_GATE0|TIMER_CT0|TIMER_M0)))|(Gate&TIMER_GATE0)|(CntOrTim&TIMER_CT0)|(Mode&TIMER_M0);	
}

/**
  * @˵��  	TIM0��ʱʱ������
  * @����  	Value ��TIM0��ʱʱ��
  *         ȡֵ��Χ��0x0000 - 0xFFFF
  * @����ֵ ��
  * @ע 	��
  */
void TIM0_TimerCnt(u16 Value)
{
	TH0 = Value>>8;
	TL0 = Value&0x00FF;	
}

/**
  * @˵��  	TIM0�˿����ʹ��/ʧ��
  * @����  	NewState ��TIM0ͨ�������״̬
  *			DISABLE		//ʧ��
  *			ENABLE 		//ʹ��
  * @����ֵ ��
  * @ע 	��
  */
void TIM0_OutPutCmd(FunctionalState NewState)
{
	if(NewState)TCON1 |= TIMER_T0OUT;
	else TCON1 &=~ TIMER_T0OUT;
}

/**
  * @˵��  	ʹ�ܻ�ʧ��TIM0
  * @����  	NewState ��TIM0��״̬
  *			DISABLE		//ʧ��
  *			ENABLE 		//ʹ��
  * @����ֵ ��
  * @ע 	��
  */
void TIM0_Cmd(FunctionalState NewState)
{
	if(NewState)TCON |= TIMER_TR0;
	else TCON &=~ TIMER_TR0;
}

/**
  * @˵��  	����TIM0����Ĺܽ�ӳ��
  * @����  	OutPin ���ܽ����Դ
  *			TIM0_OUTPin_P00  	//TIM0����˿�ΪP00
  *			TIM0_OUTPin_P01  	//TIM0����˿�ΪP01
  *			TIM0_OUTPin_P02  	//TIM0����˿�ΪP02
  *			TIM0_OUTPin_P03  	//TIM0����˿�ΪP03
  *			TIM0_OUTPin_P04  	//TIM0����˿�ΪP04
  *			TIM0_OUTPin_P05  	//TIM0����˿�ΪP05
  *			TIM0_OUTPin_P06  	//TIM0����˿�ΪP06
  *			TIM0_OUTPin_P07		//TIM0����˿�ΪP07
  *			TIM0_OUTPin_P10		//TIM0����˿�ΪP10
  * 		TIM0_OUTPin_P11		//TIM0����˿�ΪP11
  *			TIM0_OUTPin_P20		//TIM0����˿�ΪP20
  *			TIM0_OUTPin_P21		//TIM0����˿�ΪP21
  *			TIM0_OUTPin_P22		//TIM0����˿�ΪP22
  *			TIM0_OUTPin_P23		//TIM0����˿�ΪP23
  *			TIM0_OUTPin_P24		//TIM0����˿�ΪP24
  *			TIM0_OUTPin_P25		//TIM0����˿�ΪP25
  *			TIM0_OUTPin_P26		//TIM0����˿�ΪP26
  *			TIM0_OUTPin_P27		//TIM0����˿�ΪP27
  * @����ֵ ��
  * @ע 	��
  */
void TIM0_PinRemapConfig(TIM0_OutPin_TypeDef OutPin)
{
	T0_MAP = OutPin;
}

/**
  * @˵��  	ʹ�ܻ�ʧ��TIM0�ж�
  * @����  	NewState ��TIM0�ж���״̬
  *			DISABLE		//ʧ��
  *			ENABLE 		//ʹ��
  * @����ֵ ��
  * @ע 	��
  */
void TIM0_ITCmd(FunctionalState NewState)
{
	if(NewState)IE |= INT_ET0;
	else IE &=~ INT_ET0;
}

/**
  * @˵��  	��ȡTIM0�жϱ�־״̬
  * @����  	��
  * @����ֵ FlagStatus ��TIM0�жϱ�־״̬
  *			�ο� FlagStatus ö����
  * @ע 	��
  */
FlagStatus TIM0_GetITFlag(void)
{
	if(TCON&TIMER_TF0) return SET;
	return RESET;
}

/**
  * @˵��  	���TIM0�жϱ�־״̬
  * @����  	��
  * @����ֵ ��
  * @ע 	��
  */
void TIM0_ClearITFlag(void)
{
	TCON &=~ TIMER_TF0;	
}

/**
  * @˵��  	����TIM0�ж����ȼ�
  * @����  	ITPriority ��TIM0�ж����ȼ�
  *			TIM0_ITPriority_0	//TIM0�ж����ȼ�0(���)
  *			TIM0_ITPriority_1	//TIM0�ж����ȼ�1
  *			TIM0_ITPriority_2	//TIM0�ж����ȼ�2
  *			TIM0_ITPriority_3	//TIM0�ж����ȼ�3(���)
  * @����ֵ ��
  * @ע 	��
  */
void TIM0_ITPriorityConfig(TIM0_ITPriority_TypeDef ITPriority)
{
	IP0 = (IP0&(~INT_PT0))|ITPriority;		
}