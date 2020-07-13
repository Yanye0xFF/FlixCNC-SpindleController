/**
*									 ģ�����ܽ���
*	1��HC89S003�ܹ��ṩ��16���ⲿ�жϣ���������4���жϵ����ȼ���
*	2����4���ⲿ�ж�������ڣ�INT0/INT1�ֱ���һ���������ж�������ڣ�INT2-7����һ���ж�
*	   ������ڣ�INT8-15����һ���ж�������ڡ�
*	3�������ⲿ�ж϶������趨4�ִ�����ʽ���ֱ�Ϊ�����ء��½��ء�˫���Լ��͵�ƽ��
*	4��INT0��INT1�жϱ�־λӲ���Զ���0��INT2-INT15�������0��
*	************************************************************************************
*									 Ӧ��ע������
*	1��ֻҪ��Ӧ��EINTx(x=2...15)��������Ӧ���жϱ�־�Ϳ��ܱ���1����֮����Ӧ��־���ᱻ��1��
*	2���ⲿ�ж���ʹ�ò�ѯ��ʽʱ���޷���������жϱ�־λ������ͨ���Ƚ�ֹ�ж�ʹ��λ�����
*	   �жϱ�־�����������жϱ�־λ����ʹ���ⲿ�ж��������������ⲿ�жϲ�����ѯ��ʹ��
*	   �жϷ�ʽʱ���޸����⡣
*	************************************************************************************
*								       ��������
*	EXTI_DeInit()						//�ָ��ⲿ�жϼĴ�����Ĭ��ֵ
*	EXTI_Init()							//����ָ���Ĳ�����ʼ���ⲿ�ж�
*	EXTI_Cmd()							//ʹ�ܻ�ʧ���ⲿ�ж�
*	EXTI_GetFlag()						//��ȡ�ⲿ�жϵı�־״̬
*	EXTI_ClearFlag()					//����ⲿ�жϱ�־λ
*	EXTI_INT0PinChoose()				//INT0�ܽ�ѡ��
*	EXTI_INT1PinChoose()				//INT1�ܽ�ѡ��
*	EXTI_ITPriorityConfig()				//����EXTI�ж����ȼ�
*	************************************************************************************
**/


#include "hc89s_exit.h"

/**
  * @˵��  	�ָ��ⲿ�жϼĴ�����Ĭ��ֵ
  * @����  	��
  * @����ֵ ��
  * @ע		��
  */
void EXTI_DeInit(void)
{
	PITS  = INT_PITS_RESET_VALUE;
	PINTE =	INT_PINTE_RESET_VALUE;
	PINTF = INT_PINTF_RESET_VALUE;
}

/**
  * @˵��  	����ָ���Ĳ�����ʼ���ⲿ�ж�
  * @����  	Number ���ⲿ�ж�ͨ��
  *			INT0	//�ⲿ�ж�0	
  *			INT1	//�ⲿ�ж�1
  *			INT2	//�ⲿ�ж�2
  *			INT3	//�ⲿ�ж�3
  *			INT4	//�ⲿ�ж�4
  *			INT5	//�ⲿ�ж�5
  *			INT6	//�ⲿ�ж�6
  *			INT7	//�ⲿ�ж�7
  *			INT8	//�ⲿ�ж�8
  *			INT9 	//�ⲿ�ж�9
  *			INT10	//�ⲿ�ж�10
  *			INT11	//�ⲿ�ж�11
  *			INT12	//�ⲿ�ж�12
  *			INT13	//�ⲿ�ж�13
  *			INT14	//�ⲿ�ж�14
  *			INT15	//�ⲿ�ж�15
  * @����  	Sensitivity ���ⲿ�жϴ�����ʽ
  *		  	EXTI_SENSITIVITY_LOW_ONLY	//�͵�ƽ����
  *		  	EXTI_SENSITIVITY_FALL_ONLY	//�½��ش���
  *		  	EXTI_SENSITIVITY_RISE_ONLY	//�����ش���
  *		  	EXTI_SENSITIVITY_RISE_FALL	//˫�ش���
  * @����ֵ ��
  * @ע 	��
  */
void EXTI_Init(EXTI_Number_TypeDef Number,EXTI_Sensitivity_TypeDef Sensitivity)
{
	PITS = Tranverse32((PITS & (~(0x00000003<<Number))) |((0x00000003&(u32)Sensitivity)<<Number));
}

/**
  * @˵��  	ʹ�ܻ�ʧ���ⲿ�ж�
  * @����  	Number ���ⲿ�ж�ͨ��
  *			INT0	//�ⲿ�ж�0	
  *			INT1	//�ⲿ�ж�1
  *			INT2	//�ⲿ�ж�2
  *			INT3	//�ⲿ�ж�3
  *			INT4	//�ⲿ�ж�4
  *			INT5	//�ⲿ�ж�5
  *			INT6	//�ⲿ�ж�6
  *			INT7	//�ⲿ�ж�7
  *			INT8	//�ⲿ�ж�8
  *			INT9 	//�ⲿ�ж�9
  *			INT10	//�ⲿ�ж�10
  *			INT11	//�ⲿ�ж�11
  *			INT12	//�ⲿ�ж�12
  *			INT13	//�ⲿ�ж�13
  *			INT14	//�ⲿ�ж�14
  *			INT15	//�ⲿ�ж�15
  * @����  	NewState ���ⲿ�ж�Numberͨ����״̬
			DISABLE		//ʧ��
			ENABLE		//ʹ��
  * @����ֵ ��
  * @ע 	��
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
  * @˵��  	��ȡ�ⲿ�жϵı�־״̬
  * @����  	Number ���ⲿ�ж�ͨ��
  *			INT0	//�ⲿ�ж�0	
  *			INT1	//�ⲿ�ж�1
  *			INT2	//�ⲿ�ж�2
  *			INT3	//�ⲿ�ж�3
  *			INT4	//�ⲿ�ж�4
  *			INT5	//�ⲿ�ж�5
  *			INT6	//�ⲿ�ж�6
  *			INT7	//�ⲿ�ж�7
  *			INT8	//�ⲿ�ж�8
  *			INT9 	//�ⲿ�ж�9
  *			INT10	//�ⲿ�ж�10
  *			INT11	//�ⲿ�ж�11
  *			INT12	//�ⲿ�ж�12
  *			INT13	//�ⲿ�ж�13
  *			INT14	//�ⲿ�ж�14
  *			INT15	//�ⲿ�ж�15
  * @����ֵ FlagStatus ���ⲿ�жϱ�־״̬
  *			�ο� FlagStatus ö����
  * @ע 	��
  */
FlagStatus EXTI_GetFlag(EXTI_Number_TypeDef Number)
{
	if(PINTF&(0x0001<<(Number/2)))return SET;
	return RESET;
}

/**
  * @˵��  	����ⲿ�жϱ�־λ
  * @����  	Number ���ⲿ�ж�ͨ��
  *			INT0	//�ⲿ�ж�0	
  *			INT1	//�ⲿ�ж�1
  *			INT2	//�ⲿ�ж�2
  *			INT3	//�ⲿ�ж�3
  *			INT4	//�ⲿ�ж�4
  *			INT5	//�ⲿ�ж�5
  *			INT6	//�ⲿ�ж�6
  *			INT7	//�ⲿ�ж�7
  *			INT8	//�ⲿ�ж�8
  *			INT9 	//�ⲿ�ж�9
  *			INT10	//�ⲿ�ж�10
  *			INT11	//�ⲿ�ж�11
  *			INT12	//�ⲿ�ж�12
  *			INT13	//�ⲿ�ж�13
  *			INT14	//�ⲿ�ж�14
  *			INT15	//�ⲿ�ж�15
  * @����ֵ ��
  * @ע 	��
  */
void EXTI_ClearFlag(EXTI_Number_TypeDef Number)
{
	PINTF &=~ (0x0001<<(Number/2));	
}

/**
  * @˵��  	INT0�ܽ�ѡ��
  * @����  	INT0PinChoose : INT0�ܽ�ѡ��
  *			INT0_P00	//INT0�ܽ�ѡ��P00	
  *			INT0_P10	//INT0�ܽ�ѡ��P10
  * @����ֵ ��
  * @ע 	��
  */
void EXTI_INT0PinChoose(EXTI_INT0PinChoose_TypeDef INT0PinChoose)
{
	INT01_PINS = (INT01_PINS&(~INT_INT0_PINS))|INT0PinChoose;	
}

/**
  * @˵��  	INT1�ܽ�ѡ��
  * @����  	INT1PinChoose : INT1�ܽ�ѡ��
  *			INT1_P01	//INT1�ܽ�ѡ��P01
  *			INT1_P11	//INT1�ܽ�ѡ��P11
  * @����ֵ ��
  * @ע 	��
  */
void EXTI_INT1PinChoose(EXTI_INT0PinChoose_TypeDef INT1PinChoose)
{
	INT01_PINS = (INT01_PINS&(~INT_INT1_PINS))|INT1PinChoose;	
}

/**
  * @˵��  	EXTI�ж����ȼ�����
  * @����  	Number ���ⲿ�ж�ͨ��
  *			INT0	//�ⲿ�ж�0	
  *			INT1	//�ⲿ�ж�1
  *			INT2	//�ⲿ�ж�2
  *			INT3	//�ⲿ�ж�3
  *			INT4	//�ⲿ�ж�4
  *			INT5	//�ⲿ�ж�5
  *			INT6	//�ⲿ�ж�6
  *			INT7	//�ⲿ�ж�7
  *			INT8	//�ⲿ�ж�8
  *			INT9 	//�ⲿ�ж�9
  *			INT10	//�ⲿ�ж�10
  *			INT11	//�ⲿ�ж�11
  *			INT12	//�ⲿ�ж�12
  *			INT13	//�ⲿ�ж�13
  *			INT14	//�ⲿ�ж�14
  *			INT15	//�ⲿ�ж�15
  * @����  	ITPriority ���ж����ȼ�
  *			EXTI_ITPriority_0	//���ȼ�0(���)
  *			EXTI_ITPriority_1	//���ȼ�1
  *			EXTI_ITPriority_2	//���ȼ�2
  *			EXTI_ITPriority_3	//���ȼ�3(���)
  * @����ֵ ��
  * @ע 	INT2-7����һ������λ��INT8-15����һ������λ
  */
void EXTI_ITPriorityConfig(EXTI_Number_TypeDef Number,EXTI_ITPriority_TypeDef ITPriority)
{
	if(Number == 0)IP0 = (IP0&(~INT_PX0))|ITPriority;
	else if(Number == 2)IP0 = (IP0&(~INT_PX1))|(ITPriority<<4);
	else if(Number <= INT7)IP3 = (IP3&(~INT_PX2_7))|(ITPriority<<4);
	else if(Number <= INT15)IP3 = (IP3&(~INT_PX8_15))|(ITPriority<<6);
}