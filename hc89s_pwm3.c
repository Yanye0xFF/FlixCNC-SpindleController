/**
*   ************************************************************************************
*								�Ϻ�оʥ���ӹɷ����޹�˾
*								    www.holychip.cn
*	************************************************************************************
*	@File				hc89s_pwm3.c
*	@Library Version	V1.0.0.0
*	@Date				2017.08.18
*	************************************************************************************
*									ģ�����ܽ���
*	1��HC89S003ӵ��һ����·8λPWM3ģ�飬PWM3�ṩ��������жϡ�
*	2��PWM3���ڣ�[PWM3P]*PWM3����ʱ��Դ���ڡ�
*	3��PWM3ռ�ձȣ�[PWM3D]*PWM3����ʱ��Դ���ڡ�
*	************************************************************************************
*									 Ӧ��ע������
*	1��ռ�ձ�0%��PWM3D[0-7]=0x00������͵�ƽ��ռ�ձ�100%��PWM3P��PWM3D������ߵ�ƽ��
*	2���ر�PWM3ģ��ʱ��PWM����ֹͣ����������رա��ٴ�PWM3ģ��ʱ��PWM�����������´�1��
*	   ʼ�����������PWM3OEN���ơ�
*	3��PWM3���������������PWM3EN��1�²���Ч������ΪPWM3����ر�״̬�����ʱ��Ӧ�˿ڱ���
*	   ��Ϊ���ģʽ������ʹ����ֹ�����ֻҪ���λ��ʹ�ܣ�PWM3����������жϣ�����ʱPWM3
*	   ������Ϊ��ʱ��/������ʹ�ã��˿���λ�޸�������Ч��
*	************************************************************************************
*								      ��������
*	PWM3_DeInit()						//�ָ�PWM3�Ĵ�����Ĭ��ֵ
*	PWM3_Init();						//����ָ���Ĳ�����ʼ��PWM3
*	PWM3_OutPutCmd()					//PWM3�˿����ʹ��/ʧ��
*	PWM3_PeriodConfig()					//PWM3��������
*	PWM3_DutyCycleConfig()				//PWM3ռ�ձ�����
*	PWM3_PinRemapConfig()				//PWM3����ܽ�ӳ��
*	PWM3_Cmd()							//ʹ�ܻ�ʧ��PWM3ģ��
*	PWM3_ITCmd()						//ʹ�ܻ�ʧ��PWM3�ж�
*	PWM3_GetITFlag()					//��ȡPWM3�жϱ�־λ
*	PWM3_ClearITFlag()					//���PWM3�жϱ�־λ
*	PWM3_ITPriorityConfig();				//PWM�ж����ȼ�����
*	************************************************************************************
*									   ���㹫ʽ
*	1�����ڼ��㡣
*	   ���� =  PWM3P[7:0] / (Fosc / PWM��Ƶϵ��)		
*	2����Ч��ƽʱ����㣨��ռ�ձȣ���
*	   ��Ч��ƽʱ�� = PWM3D[7:0] / (Fosc / PWM��Ƶϵ��)
*	   ռ�ձ� = ��Ч��ƽʱ�� / ����
*	************************************************************************************
*  								       �ͻ�����
*	 ��л��ʹ�����ǵĵ�Ƭ���������ִ����Ժ�����ʹ�ô������ʡ�������Ϻ�оʥ���ӹٷ�QQ��
*	****************************����֧��Ⱥ��201030494**********************************
*   ************************************************************************************
**/

#include "hc89s_pwm3.h"

/**
  * @˵��  	�ָ�PWM3�Ĵ�����Ĭ��ֵ
  * @����  	��
  * @����ֵ ��
  * @ע 	��
  */
void PWM3_DeInit(void)
{
	PWM3C = PWM3_PWM3C_RESET_VALUE;
	PWM3P = PWM3_PWM3P_RESET_VALUE;
	PWM3D = PWM3_PWM3D_RESET_VALUE;
	PWM3_MAP = PWMX_PWM3MAP_RESET_VALUE;
}

/**
  * @˵��  	����ָ���Ĳ�����ʼ��PWM3
  * @����  	VoltValid ��	PWM��Ч��ƽ
  *         PWM3_HIGH_VALID	�ߵ�ƽΪ��Ч��ƽ
  *			PWM3_LOW_VALID	�͵�ƽΪ��Ч��ƽ ����Ч�ڼ���ָռ�ձ��ڼ䡣��
  * @����  	ClkDevice ��	PWM3ʱ�ӷ�Ƶϵ��
  *         PWMCK_1			PWMʱ��Ϊoscʱ��/1
  *         PWMCK_2			PWMʱ��Ϊoscʱ��/2
  *         PWMCK_4			PWMʱ��Ϊoscʱ��/4
  *         PWMCK_8			PWMʱ��Ϊoscʱ��/8
  *         PWMCK_16		PWMʱ��Ϊoscʱ��/16
  *         PWMCK_32		PWMʱ��Ϊoscʱ��/32
  *         PWMCK_64		PWMʱ��Ϊoscʱ��/64
  *         PWMCK_128		PWMʱ��Ϊoscʱ��/128
  * @����ֵ ��
  * @ע 	��
  */
void PWM3_Init(PWM3_VoltValid_TypeDef VoltValid,PWM3_ClkDevice_TypeDef ClkDevice)
{
	PWM3C = (PWM3C&(~(PWM3_S|PWM3_PTCK3)))|(VoltValid&PWM3_S)|(ClkDevice&PWM3_PTCK3);		
}

/**
  * @˵��  	PWM3�˿����ʹ��/ʧ��
  * @����  	NewState ��PWM3ͨ�������״̬
  *			DISABLE 	//ʧ��
  *			ENABLE 		//ʹ��
  * @����ֵ ��
  * @ע 	��
  */
void PWM3_OutPutCmd(FunctionalState NewState)
{
	if(NewState)PWM3C |= PWM3_OEN;
	else PWM3C &=~ PWM3_OEN;
}


/**
  * @˵��  	����PWM3����Ĺܽ�ӳ��
  * @����  	OutPin ���ܽ����Դ
  *			PWM3_OutPin_P00  	//PWM3����˿�ΪP00
  *			PWM3_OutPin_P01  	//PWM3����˿�ΪP01
  *			PWM3_OutPin_P02  	//PWM3����˿�ΪP02
  *			PWM3_OutPin_P03  	//PWM3����˿�ΪP03
  *			PWM3_OutPin_P04  	//PWM3����˿�ΪP04
  *			PWM3_OutPin_P05  	//PWM3����˿�ΪP05
  *			PWM3_OutPin_P06  	//PWM3����˿�ΪP06
  *			PWM3_OutPin_P07		//PWM3����˿�ΪP07
  *			PWM3_OutPin_P10		//PWM3����˿�ΪP10
  * 		PWM3_OutPin_P11		//PWM3����˿�ΪP11
  *			PWM3_OutPin_P20		//PWM3����˿�ΪP20
  *			PWM3_OutPin_P21		//PWM3����˿�ΪP21
  *			PWM3_OutPin_P22		//PWM3����˿�ΪP22
  *			PWM3_OutPin_P23		//PWM3����˿�ΪP23
  *			PWM3_OutPin_P24		//PWM3����˿�ΪP24
  *			PWM3_OutPin_P25		//PWM3����˿�ΪP25
  *			PWM3_OutPin_P26		//PWM3����˿�ΪP26
  *			PWM3_OutPin_P27		//PWM3����˿�ΪP27
  * @����ֵ ��
  * @ע 	��
  */
void PWM3_PinRemapConfig(PWM3_OutPin_TypeDef OutPin)
{
	PWM3_MAP = OutPin;
}
 
/**
  * @˵��  	PWM3��������
  * @����  	PwmDeadZone ��PWM����
  *         ȡֵ��Χ 0 - 0xFF
  * @����ֵ ��
  * @ע 	����ʱ�� = PwmPeriod / (Fosc / PWM��Ƶϵ��)
  */
void PWM3_PeriodConfig(u8 PwmPeriod)
{
	PWM3P = PwmPeriod;
}


/**
  * @˵��  	PWM3ռ�ձ�����
  * @����  	PwmDeadZone ��PWM3ռ�ձ�
  *         ȡֵ��Χ 0 - 0xFF
  * @����ֵ ��
  * @ע 	��
  */
void PWM3_DutyCycleConfig(u8 PwmDuty)
{
	PWM3D = PwmDuty;	
}
											
/**
  * @˵��  	ʹ�ܻ�ʧ��PWM3ģ��
  * @����  	NewState ��PWM3ģ����״̬
  *			DISABLE 	//ʧ��
  *			ENABLE 		//ʹ��
  * @����ֵ ��
  * @ע 	��
  */
void PWM3_Cmd(FunctionalState NewState)
{
	if(NewState == ENABLE)PWM3C |= PWM3_EN;	
	else PWM3C &=~ PWM3_EN;
}


/**
  * @˵��  	ʹ�ܻ�ʧ��PWM3�ж�
  * @����  	NewState ��PWM3�жϹ�����״̬
  *			DISABLE 	//ʧ��
  *			ENABLE 		//ʹ��
  * @����ֵ ��
  * @ע 	��
  */
void PWM3_ITCmd(FunctionalState NewState)
{
	if(NewState)PWM3C |= PWM3_IE;
	else PWM3C &=~ PWM3_IE;
}

/**
  * @˵��  	��ȡPWM3�жϱ�־λ
  * @����  	��
  * @����ֵ FlagStatus ������PWM3�ж�״̬
  *			�ο� FlagStatus ö����
  * @ע 	��
  */
FlagStatus PWM3_GetITFlag(void)
{
	if(PWM3C & PWM3_IF)return SET;
	else return RESET;
}

/**
  * @˵��  	���PWM3�жϱ�־λ
  * @����  	��
  * @����ֵ ��
  * @ע 	��Ҫ������PWM3�жϱ�־λ
  */
void PWM3_ClearITFlag(void)
{
	PWM3C &=~ PWM3_IF;
}

/**
  * @˵��  	PWM�����ж����ȼ�
  * @����  	ITPriority ��PWM�ж����ȼ�
  *			PWM3_ITPriority_0	//PWM�ж����ȼ�0(���)
  *			PWM3_ITPriority_1	//PWM�ж����ȼ�1
  *			PWM3_ITPriority_2	//PWM�ж����ȼ�2
  *			PWM3_ITPriority_3	//PWM�ж����ȼ�3(���)
  * @����ֵ ��
  * @ע 	PWM0��PWM0��PWM0��PWM3����ͬһ�ж�����
  */
void PWM3_ITPriorityConfig(PWM3_ITPriority_TypeDef ITPriority)
{
	IP2 = (IP2&(~INT_PPWM))|ITPriority;		
}

















