/**
*									 ģ�����ܽ���
*	1����ʱ��3ֻ��һ��������ʽ��16λ�Զ����ؼ�����/��ʱ������������Ԥ��Ƶ�ȣ������Թ���
*	   �ڵ���ģʽ��
*	2��TH3��TL3��д������ѭ����˳���ȸ�λ���λ��
*	************************************************************************************
*									 Ӧ��ע������
*	1��T3�ⲿʱ�ӱ�������һ��������
*	2��T3�ⲿʱ������ڱ�������Ϊ�������롣
*	3�����������ж����ж��жϱ�־λ��
*	4��T3ʱ��ʹ���ڲ�ʱ��ʱ�޷������ڵ���ģʽ�¡�
*	5��������ʹ��ǰ��TH3��TL3��ֵ��
*	6����T3����ʱ��Դѡ��˿������ⲿʱ�ӣ�TR3��T3CLKS��������Ҫͬʱ���У���ʹ��һ��ָ
*	   ��������á�
*	************************************************************************************
*								      ��������
*	TIM3_DeInit()						//�ָ�TIM3�Ĵ�����Ĭ��ֵ
*	TIM3_Init()							//����ָ���Ĳ�����ʼ��TIM3
*	TIM3_TimerCnt()						//TIM3��ʱʱ������
*	TIM3_PinRemapConfig()				//����TIM3����Ĺܽ�ӳ��
*	TIM3_PDModeCmd()					//ʹ�ܻ�ʧ��TIM3����ģʽ������
*	TIM3_ITCmd()						//ʹ�ܻ�ʧ��TIM3�ж�
*	TIM3_Cmd()							//ʹ�ܻ�ʧ��TIM3
*	TIM3_GetITFlag()					//��ȡTIM3�жϱ�־״̬
*	TIM3_ClearITFlag()					//���TIM3�жϱ�־״̬
*	TIM3_ITPriorityConfig();			//TIM3�ж����ȼ�����
*	************************************************************************************
*									   ���㹫ʽ��ע���ù�ʽ����ֱ�������뼶��ʱ���ӣ�
*	1��Tim3����ʱ�䡣
*	   ��ʱʱ�� = (65536 - T3) * (1 / (Fosc /Timer��Ƶϵ��))
*	2��Tim3���Ƴ�ֵ��
*	   ��ֵ = 65536 - ((��ʱʱ��) / (1/(Fosc / Timer��Ƶϵ��)))
*	3��Tim3�ⲿʱ�Ӽ���ʱ�䡣
*	   ��ʱʱ��  = (65536 - T3) * (1 / �ⲿ����ʱ��)
*	2��Tim3�ⲿʱ�ӷ��Ƴ�ֵ��
*	   ��ֵ  = 65536 - ((��ʱʱ��) / (1 / �ⲿ����ʱ��))
*	************************************************************************************
**/

#include "hc89s_timer3.h"

/**
  * @˵��  	�ָ�TIM3�Ĵ�����Ĭ��ֵ
  * @����  	��
  * @����ֵ ��
  * @ע 	��
  */
void TIM3_DeInit(void)
{
	T3CON = TIM3_T3CON_RESET_VALUE;
	T3 = TIM3_T3_RESET_VALUE;
	T3_MAP = TIMER_T3MAP_RESET_VALUE;
}

/**
  * @˵��  	����ָ���Ĳ�����ʼ��TIM3
  * @����  	ClockSource ��TIM3ʱ��Դ
  *			TIM3_SYSCLK		//OSCʱ��Դ
  *			TIM3_OUTCLK		//T3�˿�ʱ��Դ
  * @����  	ClockPrescaler ��TIM3ʱ��Դ��Ƶϵ��
  *			TIM3_CLK_1		//ʱ��1��Ƶ
  *			TIM3_CLK_8		//ʱ��8��Ƶ
  *			TIM3_CLK_64		//ʱ��64��Ƶ
  *			TIM3_CLK_256	//ʱ��256��Ƶ	
  * @����ֵ ��
  * @ע 	��
  */
void TIM3_Init(TIM3_ClkSource_Typedef ClkSource,TIM3_ClkPrescaler_Typedef ClkPrescaler,FunctionalState NewState)
{
	T3CON = (T3CON&(~(TIM3_T3PS|TIM3_T3CLKS|TIM3_TR3)))|(ClkPrescaler&TIM3_T3PS)|(ClkSource&TIM3_T3CLKS)|((unsigned char)NewState<<2);	
}

/**
  * @˵��  	TIM3��ʱʱ������
  * @����  	Value ��TIM3��ʱʱ��
  *         ȡֵ��Χ��0x0000 - 0xFFFF
  * @����ֵ ��
  * @ע 	��
  */
void TIM3_TimerCnt(u16 Value)
{
	T3 = Value;	
}


/**
  * @˵��  	����TIM3����Ĺܽ�ӳ��
  * @����  	OutPin ���ܽ����Դ
  *			TIM3_OUTPin_P00  	//TIM3����˿�ΪP00
  *			TIM3_OUTPin_P01  	//TIM3����˿�ΪP01
  *			TIM3_OUTPin_P02  	//TIM3����˿�ΪP02
  *			TIM3_OUTPin_P03  	//TIM3����˿�ΪP03
  *			TIM3_OUTPin_P04  	//TIM3����˿�ΪP04
  *			TIM3_OUTPin_P05  	//TIM3����˿�ΪP05
  *			TIM3_OUTPin_P06  	//TIM3����˿�ΪP06
  *			TIM3_OUTPin_P07		//TIM3����˿�ΪP07
  *			TIM3_OUTPin_P10		//TIM3����˿�ΪP10
  * 		TIM3_OUTPin_P11		//TIM3����˿�ΪP11
  *			TIM3_OUTPin_P20		//TIM3����˿�ΪP20
  *			TIM3_OUTPin_P21		//TIM3����˿�ΪP21
  *			TIM3_OUTPin_P22		//TIM3����˿�ΪP22
  *			TIM3_OUTPin_P23		//TIM3����˿�ΪP23
  *			TIM3_OUTPin_P24		//TIM3����˿�ΪP24
  *			TIM3_OUTPin_P25		//TIM3����˿�ΪP25
  *			TIM3_OUTPin_P26		//TIM3����˿�ΪP26
  *			TIM3_OUTPin_P27		//TIM3����˿�ΪP27
  * @����ֵ ��
  * @ע 	��
  */
void TIM3_PinRemapConfig(TIM3_OutPin_TypeDef OutPin)
{
	T3_MAP = OutPin;
}

/**
  * @˵��  	ʹ�ܻ�ʧ��TIM3����ģʽ������
  * @����  	NewState ��TIM3����ģʽ��������״̬
  *			DISABLE		//ʧ��
  *			ENABLE		//ʹ��
  * @����ֵ ��
  * @ע 	��
  */
void TIM3_PDModeCmd(FunctionalState NewState)
{
	if(NewState)T3CON |= TIM3_T3PD_EN;
	else T3CON &=~ TIM3_T3PD_EN;
}

/**
  * @˵��  	ʹ�ܻ�ʧ��TIM3�ж�
  * @����  	NewState ��TIM3�ж���״̬
  *			DISABLE		//ʧ��
  *			ENABLE		//ʹ��
  * @����ֵ ��
  * @ע 	��
  */
void TIM3_ITCmd(FunctionalState NewState)
{
	if(NewState)IE1 |= INT_ET3;
	else IE1 &=~ INT_ET3;
}

/**
  * @˵��  	ʹ�ܻ�ʧ��TIM3
  * @����  	NewState ��TIM3��״̬
  *			DISABLE		//ʧ��
  *			ENABLE		//ʹ��
  * @����ֵ ��
  * @ע 	��
  */
void TIM3_Cmd(FunctionalState NewState)
{
	if(NewState)T3CON |= TIM3_TR3;
	else T3CON &=~ TIM3_TR3;
}

/**
  * @˵��  	��ȡTIM3�жϱ�־״̬
  * @����  	��
  * @����ֵ FlagStatus ��TIM3�жϱ�־״̬
  *			�ο� FlagStatus ö����
  * @ע 	��
  */
FlagStatus TIM3_GetITFlag(void)
{
	if(T3CON&TIM3_TF3) return SET;
	return RESET;
}

/**
  * @˵��  	���TIM3�жϱ�־״̬
  * @����  	��
  * @����ֵ ��
  * @ע 	��
  */
void TIM3_ClearITFlag(void)
{
	T3CON &=~ TIM3_TF3;	
}

/**
  * @˵��  	����TIM3�ж����ȼ�
  * @����  	ITPriority ��TIM3�ж����ȼ�
  *			TIM3_ITPriority_0	//TIM3�ж����ȼ�0(���)
  *			TIM3_ITPriority_1	//TIM3�ж����ȼ�1
  *			TIM3_ITPriority_2	//TIM3�ж����ȼ�2
  *			TIM3_ITPriority_3	//TIM3�ж����ȼ�3(���)
  * @����ֵ ��
  * @ע 	��
  */
void TIM3_ITPriorityConfig(TIM3_ITPriority_TypeDef ITPriority)
{
	IP2 = (IP2&(~INT_PT3))|ITPriority;		
}
