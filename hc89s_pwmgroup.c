/**
*									 ģ�����ܽ���
*	1��HC89S003������3��12λPWMģ��PWM0��PWM1��PWM2��ÿ��ģ�����һ��������������PWMģ��
*	   �Ĺ����Լ�������ȫһ����
*	2���ṩÿ��PWM��������жϣ����жϹ���ͬһ������ڡ�
*	3���ṩ����֡�⹦�ܿɽ����ر�PWM�����
*	************************************************************************************
*									 Ӧ��ע������
*	1���������ģʽ���������ģʽ�����ܹ��ϼ��ſ��ơ�
*	2���޸�PWM0����ģʽʱ�����ȹر�PWM0ģ�顣
*	3���ر�ʱ��PWM0����ֹͣ����������رա���ʱ��PWM0�����������´�1��ʼ�����������
*	   PWM0_OEN��PWM01_OEN���ơ�
*	4��PWM0���������������PWM0_EN��1�²���Ч������PWM0����ر�״̬�����ʱ��Ӧ�˿ڱ���
*	   ��Ϊ���ģʽ������ʹ����ֹ�����ֻҪPWM0_ENλ��ʹ�ܣ�PWM0����������жϣ�����ʱ
*	   PWM0������Ϊ��ʱ��ʹ�ã��˿���λ�޸�������Ч��
*	5�����ڶ���ģʽ�����ģʽѡ��λͬ����Ч�����뻥��ģʽ��ͬ������Ч�ڼ�Ϊռ�ձ��ڼ䣻
*	   ������ģʽ�ж���PWM0����Ч�ڼ�Ϊռ�ձ��ڼ䣬PWM01����Ч�ڼ�Ϊռ�ձȵĻ����ڼ䡣
*	************************************************************************************
*								       ��������
*	PWM_DeInit()							//�ָ�PWM�Ĵ�����Ĭ��ֵ
*	PWM_Init()								//����ָ���Ĳ�����ʼ��PWM
*	PWM_FLTConfig()							//PWM����ϼ���������
*	PWM_PinRemapConfig()					//����PWM����Ĺܽ�ӳ��
*	PWM_OutPutCmd()							//PWM�˿����ʹ��/ʧ��
*	PWM_PeriodConfig()						//PWM��������
*	PWM_DutyCycleConfig()					//PWMռ�ձ�����
*	PWM_DeadZoneConfig()					//PWM����ʱ������
*	PWM_Cmd()								//ʹ�ܻ�ʧ��PWMģ��
*	PWM_ITCmd()								//ʹ�ܻ�ʧ��PWM�ж�
*	PWM_GetITFlag()							//��ȡPWM�жϱ�־λ
*	PWM_ClearITFlag()						//���PWM�жϱ�־λ
*	PWM_GetFLTFlag()						//��ȡPWM����״̬���ر���/��������
*	PWM_ITPriorityConfig();					//PWM�ж����ȼ�����
*	************************************************************************************
*									   ���㹫ʽ
*	1�����ڼ���
*	   ���� = PWMxP / (Fosc / PWM��Ƶϵ��)			 (x = 0,1,2)
*	2����Ч��ƽ���㣨��ռ�ձȣ�
*	   ��Ч��ƽ = PWMxD / (Fosc / PWM��Ƶϵ��)		 (x = 0,1,2)
*	   ռ�ձ� = ��Ч��ƽ / ����
*	3���������ڼ���
*	   ����	= PWMxDT  / (Fosc / PWM��Ƶϵ��)		 (x = 0,1,2)
*	   ����ʱ�� = ���� / ���� 
*	4��PWM��ʱʱ�����(PWM��Ϊ��ʱ��ʹ��ʱ)
*	   ��ʱʱ�� = PWMxP / (Fosc / PWM��Ƶϵ��)		 (x = 0,1,2)
*	************************************************************************************
**/

#include "hc89s_pwmgroup.h"

/**
  * @˵��  	�ָ�PWM�Ĵ�����Ĭ��ֵ
  * @����  	��
  * @����ֵ ��
  * @ע 	��
  */
void PWM_DeInit(PWM_Group_TypeDef PWM_Group)
{
	switch(PWM_Group)
	{
		case PWM0_Group:
				PWM0EN = PWMX_EN_RESET_VALUE;
				PWM0C = PWMX_C_RESET_VALUE;
				PWM0P = PWMX_P_RESET_VALUE;
				PWM0D = PWMX_D_RESET_VALUE;
				PWM0DT = PWMX_DT_RESET_VALUE;
				PWM0_MAP = PWMX_PWMXMAP_RESET_VALUE;
				PWM01_MAP = PWMX_PWMX1MAP_RESET_VALUE;				
				break;
		case PWM1_Group:
				PWM1EN = PWMX_EN_RESET_VALUE;
				PWM1C = PWMX_C_RESET_VALUE;
				PWM1P = PWMX_P_RESET_VALUE;
				PWM1D = PWMX_D_RESET_VALUE;
				PWM1DT = PWMX_DT_RESET_VALUE;
				PWM1_MAP = PWMX_PWMXMAP_RESET_VALUE;
				PWM11_MAP = PWMX_PWMX1MAP_RESET_VALUE;			
				break;
		case PWM2_Group:
				PWM2EN = PWMX_EN_RESET_VALUE;
				PWM2C = PWMX_C_RESET_VALUE;
				PWM2P = PWMX_P_RESET_VALUE;
				PWM2D = PWMX_D_RESET_VALUE;
				PWM2DT = PWMX_DT_RESET_VALUE;
				PWM2_MAP = PWMX_PWMXMAP_RESET_VALUE;
				PWM21_MAP = PWMX_PWMX1MAP_RESET_VALUE;			
				break;
		default:break;	

	}
}

/**
  * @˵��  	����ָ���Ĳ�����ʼ��PWM
  * @����  	Group ��PWM��
  *			PWM0_Group		//PWM0��
  *			PWM1_Group		//PWM1��
  *			PWM2_Group		//PWM2��
  * @����  	ClkDevice ��	PWMʱ�ӷ�Ƶϵ��
  *			PWMCK_1		//PWMʱ��ΪOSCʱ��1��Ƶ
  *			PWMCK_8		//PWMʱ��ΪOSCʱ��8��Ƶ
  *			PWMCK_32	//PWMʱ��ΪOSCʱ��32��Ƶ
  *			PWMCK_128	//PWMʱ��ΪOSCʱ��128��Ƶ
  * @����  	GroupOutMode ��	PWM���ģʽ
  *			PWMX_DISABLE_PWMX1_DISABLE	//����PWM0�顣��PWM0�������PWM01�����
  *			PWMX_ENABLE_PWMX1_DISABLE 	//����PWM0�顣��PWM0�����PWM01�����
  *			PWMX_DISABLE_PWMX1_ENABLE	//����PWM0�顣��PWM0�������PWM01���
  *			PWMX_ENABLE_PWMX1_ENABLE 	//����PWM0�顣��PWM0�����PWM01���
  * @����  	GroupMode ��	PWM��ģʽ
  *			COMPLEMENTARY_MODE	//����ģʽ
  *			INDEPENDENT_MODE 	//����ģʽ
  * @����ֵ ��
  * @ע 	��
  */
void PWM_Init(PWM_Group_TypeDef Group,PWM_ClkDevice_TypeDef ClkDevice,PWM_VoltValid_TypeDef GroupOutMode,PWM_GroupMode_TypeDef GroupMode)
{
	switch(Group)
	{
		case PWM0_Group:
					PWM0C	= (PWM0C&(~(PWMX_CKX|PWMX_XS)))|(ClkDevice&PWMX_CKX)|(GroupOutMode&PWMX_XS);
					PWM0EN  = (PWM0EN&(~PWMX_XM))|(GroupMode&PWMX_XM);
					break;
		case PWM1_Group:
					PWM1C	= (PWM1C&(~(PWMX_CKX|PWMX_XS)))|(ClkDevice&PWMX_CKX)|(GroupOutMode&PWMX_XS);
					PWM1EN  = (PWM1EN&(~PWMX_XM))|(GroupMode&PWMX_XM);
					break;
		case PWM2_Group:
					PWM1C	= (PWM2C&(~(PWMX_CKX|PWMX_XS)))|(ClkDevice&PWMX_CKX)|(GroupOutMode&PWMX_XS);
					PWM1EN  = (PWM1EN&(~PWMX_XM))|(GroupMode&PWMX_XM);
					break;
		default:break;
	}
}

/**
  * @˵��  	PWM����ϼ���������
  * @����  	Group ��PWM��
  *			PWM0_Group		//PWM0��
  *			PWM1_Group		//PWM1��
  *			PWM2_Group		//PWM2��
  * @����  	FltPinVolt ��	PWM���ϼ��ĵ�ƽ
  *			FLFPIN_LOW_PWMOFF	//���ϼ���Ϊ�͵�ƽʱ������
  *			FLFPIN_HIGH_PWMOFF	//���ϼ���Ϊ�ߵ�ƽʱ������
  * @����  	FltPWMOut ��	����ʱPWM�����ƽ״̬
  *			PWMX_PWX1_ALL_LOW	//����PWM0�顣������ʱPWM0��PWM01����ߵ�ƽ
  *			PWMX_LOW_PWX1_HIGH	//����PWM0�顣������ʱPWM0����͵�ƽ��PWM01����ߵ�ƽ
  *			PWMX_HIGH_PWX1_LOW	//����PWM0�顣������ʱPWM0����ߵ�ƽ��PWM01����͵�ƽ
  *			PWMX_PWX1_ALL_HIGH	//����PWM0�顣������ʱPWM0��PWM01����͵�ƽ
  * @����  	NewState ��		���ϼ����״̬
  *			DISABLE		//ʧ��
  *			ENABLE		//ʹ��
  * @����ֵ ��
  * @ע 	��
  */
void PWM_FLTConfig(PWM_Group_TypeDef Group,PWM_FltPinVolt_TypeDef FltPinVolt,PWM_FltPWMOut_TypeDef FltPWMOut,FunctionalState NewState)
{
	switch(Group)
	{
		case PWM0_Group:
					PWM0EN	= (PWM0EN&(~PWMX_FLTX_MODE))|(FltPWMOut&PWMX_FLTX_MODE);
					if(NewState)PWM0EN |= PWMX_EFLTX;
					else PWM0EN &=~ PWMX_EFLTX; 
					PWM0C	= (PWM0C&(~PWMX_FLTXC))|(FltPinVolt&PWMX_FLTXC);
					break;
		case PWM1_Group:
					PWM1EN	= (PWM0EN&(~PWMX_FLTX_MODE))|(FltPWMOut&PWMX_FLTX_MODE);
					if(NewState)PWM1EN |= PWMX_EFLTX;
					else PWM1EN &=~ PWMX_EFLTX; 
					PWM1C	= (PWM1C&(~PWMX_FLTXC))|(FltPinVolt&PWMX_FLTXC);
					break;
		case PWM2_Group:
					PWM2EN	= (PWM2EN&(~PWMX_FLTX_MODE))|(FltPWMOut&PWMX_FLTX_MODE);
					if(NewState)PWM2EN |= PWMX_EFLTX;
					else PWM2EN &=~ PWMX_EFLTX; 
					PWM2C	= (PWM2C&(~PWMX_FLTXC))|(FltPinVolt&PWMX_FLTXC);
					break;
		default:break;
	}
}

/**
  * @˵��  	����PWM����Ĺܽ�ӳ��
   * @����  Channel ��PWM���Դ
  *			PWM_0  			//PWM0ͨ��
  *			PWM_0_1  		//PWM01ͨ��
  *			PWM_1  			//PWM1ͨ��
  *			PWM_1_1  		//PWM11ͨ��
  *			PWM_2  			//PWM2ͨ��
  *			PWM_2_1  		//PWM21ͨ��
  * @����  	OutPin ���ܽ����Դ
  *			PWM_OutPin_P00  //PWM����˿�ΪP00
  *			PWM_OutPin_P01  //PWM����˿�ΪP01
  *			PWM_OutPin_P02  //PWM����˿�ΪP02
  *			PWM_OutPin_P03  //PWM����˿�ΪP03
  *			PWM_OutPin_P04  //PWM����˿�ΪP04
  *			PWM_OutPin_P05  //PWM����˿�ΪP05
  *			PWM_OutPin_P06  //PWM����˿�ΪP06
  *			PWM_OutPin_P07	//PWM����˿�ΪP07
  *			PWM_OutPin_P10	//PWM����˿�ΪP10
  * 		PWM_OutPin_P11	//PWM����˿�ΪP11
  *			PWM_OutPin_P20	//PWM����˿�ΪP20
  *			PWM_OutPin_P21	//PWM����˿�ΪP21
  *			PWM_OutPin_P22	//PWM����˿�ΪP22
  *			PWM_OutPin_P23	//PWM����˿�ΪP23
  *			PWM_OutPin_P24	//PWM����˿�ΪP24
  *			PWM_OutPin_P25	//PWM����˿�ΪP25
  *			PWM_OutPin_P26	//PWM����˿�ΪP26
  *			PWM_OutPin_P27	//PWM����˿�ΪP27
  * @����ֵ ��
  * @ע 	��
  */
void PWM_PinRemapConfig(PWM_Channel_TypeDef Channel,PWM_OutPin_TypeDef OutPin)
{
	switch(Channel)
	{
		case PWM_0: PWM0_MAP  	= OutPin; break;
		case PWM_0_1: PWM01_MAP = OutPin; break;
		case PWM_1: PWM1_MAP  	= OutPin; break;
		case PWM_1_1: PWM11_MAP = OutPin; break;
		case PWM_2: PWM2_MAP  	= OutPin; break;
		case PWM_2_1: PWM21_MAP = OutPin; break;
		default:break;
	}
}

/**
  * @˵��  	PWM�˿����ʹ��/ʧ��
  * @����  	Channel ��PWMͨ��
  *			PWM_0	 		//PWM0ͨ��
  *			PWM_0_1			//PWM01ͨ��
  *			PWM_0_All		//PWM0ȫ��ͨ��
  *			PWM_1			//PWM1ͨ��
  *			PWM_1_1	  		//PWM11ͨ��
  *			PWM_1_All		//PWM1ȫ��ͨ��
  *			PWM_2	  		//PWM2ͨ��
  *			PWM_2_1	  		//PWM21ͨ��
  *			PWM_2_All		//PWM2ȫ��ͨ��
  * @����  	NewState ��PWMͨ�������״̬
  *			DISABLE		//ʧ��
  *			ENABLE		//ʹ��
  * @����ֵ ��
  * @ע 	��
  */
void PWM_OutPutCmd(PWM_Channel_TypeDef Channel,FunctionalState NewState)
{
	switch(Channel)
	{
		case PWM_0:
					if(NewState)PWM0EN |= PWMX_X_OEN;
					else PWM0EN &=~ PWMX_X_OEN;
					break;
		case PWM_0_1:
					if(NewState)PWM0EN |= PWMX_X1_OEN;
					else PWM0EN &=~ PWMX_X1_OEN; 
					break;
		case PWM_0_All:
					if(NewState)PWM0EN |= (PWMX_X1_OEN|PWMX_X_OEN);
					else PWM0EN &=~ (PWMX_X1_OEN|PWMX_X_OEN); 
					break;
		case PWM_1:
					if(NewState)PWM1EN |= PWMX_X_OEN;
					else PWM1EN &=~ PWMX_X_OEN; 
					break;
		case PWM_1_1:
					if(NewState)PWM1EN |= PWMX_X1_OEN;
					else PWM1EN &=~ PWMX_X1_OEN; 
					break;
		case PWM_1_All:
					if(NewState)PWM1EN |= (PWMX_X1_OEN|PWMX_X_OEN);
					else PWM1EN &=~ (PWMX_X1_OEN|PWMX_X_OEN); 
					break;		
		case PWM_2:
					if(NewState)PWM2EN |= PWMX_X_OEN;
					else PWM2EN &=~ PWMX_X_OEN; 
					break;
		case PWM_2_1:
					if(NewState)PWM2EN |= PWMX_X1_OEN;
					else PWM2EN &=~ PWMX_X1_OEN; 
					break;
		case PWM_2_All:
					if(NewState)PWM2EN |= (PWMX_X1_OEN|PWMX_X_OEN);
					else PWM2EN &=~ (PWMX_X1_OEN|PWMX_X_OEN); 
					break;
		default:break;
	}
}
  
/**
  * @˵��  	PWM��������
  * @����  	Group ��PWM��
  *			PWM0_Group		//PWM0��
  *			PWM1_Group		//PWM1��
  *			PWM2_Group		//PWM2��
  
  * @����  	PwmDeadZone ��PWM����
  *         ȡֵ��Χ 0 - 0x0FFF
  * @����ֵ ��
  * @ע 	��
  */
void PWM_PeriodConfig(PWM_Group_TypeDef Group,u16 PwmPeriod)
{
	switch(Group)
	{
		case PWM0_Group:
				PWM0P = PwmPeriod;			
				break;
		case PWM1_Group:
				PWM1P = PwmPeriod;			
				break;
		case PWM2_Group:
				PWM2P = PwmPeriod;			
				break;
		default:break;
	}
}


/**
  * @˵��  	PWMռ�ձ�����
  * @����  	Group ��PWM��
  *			PWM0_Group		//PWM0��
  *			PWM1_Group		//PWM1��
  *			PWM2_Group		//PWM2��
  
  * @����  	PwmDeadZone ��PWMռ�ձ�
  *         ȡֵ��Χ 0 - 0x0FFF
  * @����ֵ ��
  * @ע 	��
  */
void PWM_DutyCycleConfig(PWM_Group_TypeDef Group,u16 PwmDuty)
{
	switch(Group)
	{
		case PWM0_Group:
				PWM0D = PwmDuty;			
				break;
		case PWM1_Group:
				PWM1D = PwmDuty;			
				break;
		case PWM2_Group:
				PWM2D = PwmDuty;	
				break;
		default:break;
	}
}


/**
  * @˵��  	PWM����ʱ������
  * @����  	Group ��PWM��
  *			PWM0_Group		//PWM0��
  *			PWM1_Group		//PWM1��
  *			PWM2_Group		//PWM2��
  
  * @����  	PwmDeadZone ��PWM����ʱ��
  *         ȡֵ��Χ 0 - 0x0FFF
  * @����ֵ ��
  * @ע 	��
  */
void PWM_DeadZoneConfig(PWM_Group_TypeDef Group,u16 PwmDeadZone)
{
	switch(Group)
	{
		case PWM0_Group:
				PWM0DT = PwmDeadZone;			
				break;
		case PWM1_Group:
				PWM1DT = PwmDeadZone;			
				break;
		case PWM2_Group:
				PWM2DT = PwmDeadZone;			
				break;
		default:break;
	}
}

											
/**
  * @˵��  	ʹ�ܻ�ʧ��PWMģ��
  * @����  	Group ��PWM��
  *			PWM0_Group		//PWM0��
  *			PWM1_Group		//PWM1��
  *			PWM2_Group		//PWM2��
  
  * @����  	NewState ��PWMģ����״̬
  *			DISABLE		//ʧ��
  *			ENABLE		//ʹ��
  * @����ֵ ��
  * @ע 	��
  */
void PWM_Cmd(PWM_Group_TypeDef Group,FunctionalState NewState)
{
	switch(Group)
	{
		case PWM0_Group:
				if(NewState == ENABLE)PWM0EN |= PWMX_XEN;	
				else PWM0EN &=~ PWMX_XEN;			
				break;
		case PWM1_Group:
				if(NewState == ENABLE)PWM1EN |= PWMX_XEN;	
				else PWM1EN &=~ PWMX_XEN;		
				break;
		case PWM2_Group:
				if(NewState == ENABLE)PWM2EN |= PWMX_XEN;	
				else PWM2EN &=~ PWMX_XEN;			
				break;
		default:break;
	}
}


/**
  * @˵��  	ʹ�ܻ�ʧ��PWM�ж�
  * @����  	Group ��PWM��
  *			PWM0_Group		//PWM0��
  *			PWM1_Group		//PWM1��
  *			PWM2_Group		//PWM2��
 
  * @����  	NewState ��PWM�жϹ�����״̬
  *			DISABLE		//ʧ��
  *			ENABLE		//ʹ��
  * @����ֵ ��
  * @ע 	��
  */
void PWM_ITCmd(PWM_Group_TypeDef Group,FunctionalState NewState)
{
	switch(Group)
	{
		case PWM0_Group:
				if(NewState == ENABLE)PWM0C |= PWMX_XIE;
				else PWM0C &=~ PWMX_XIE;			
				break;
		case PWM1_Group:
				if(NewState == ENABLE)PWM1C |= PWMX_XIE;
				else PWM1C &=~ PWMX_XIE;		
				break;
		case PWM2_Group:
				if(NewState == ENABLE)PWM2C |= PWMX_XIE;
				else PWM2C &=~ PWMX_XIE;			
				break;
		default:break;
	}
}

/**
  * @˵��  	��ȡPWM�жϱ�־λ
  * @����  	Group ��PWM��
  *			PWM0_Group		//PWM0��
  *			PWM1_Group		//PWM1��
  *			PWM2_Group		//PWM2��
  
  * @����ֵ FlagStatus ������PWM�ж�״̬
  *			�ο� FlagStatus ö����
  * @ע 	��
  */
FlagStatus PWM_GetITFlag(PWM_Group_TypeDef Group)
{
	switch(Group)
	{
		case PWM0_Group:
				if(PWM0C & PWMX_XIF)return SET;
				else return RESET;			
				break;
		case PWM1_Group:
				if(PWM1C & PWMX_XIF)return SET;
				else return RESET;		
				break;
		case PWM2_Group:
				if(PWM2C & PWMX_XIF)return SET;
				else return RESET;			
				break;
		default:break;
	}
	return RESET; 
}

/**
  * @˵��  	���PWM�жϱ�־λ
  * @����  	Group ��PWM��
  *			PWM0_Group		//PWM0��
  *			PWM1_Group		//PWM1��
  *			PWM2_Group		//PWM2��
  
* @����ֵ ��
  * @ע 	��Ҫ������PWM�жϱ�־λ
  */
void PWM_ClearITFlag(PWM_Group_TypeDef Group)
{
	switch(Group)
	{
		case PWM0_Group:
				PWM0C &=~ PWMX_XIF;;			
				break;
		case PWM1_Group:
				PWM1C &=~ PWMX_XIF;		
				break;
		case PWM2_Group:
				PWM2C &=~ PWMX_XIF;			
				break;
		default:break;
	}
}

											
/**
  * @˵��  	��ȡPWM����״̬���ر���/��������
  * @����  	Group ��PWM��
  *			PWM0_Group		//PWM0��
  *			PWM1_Group		//PWM1��
  *			PWM2_Group		//PWM2��
  
  * @����ֵ FlagStatus ������PWM����״̬
  *			�ο� FlagStatus ö����
  * @ע 	��
  */
FlagStatus PWM_GetFLTFlag(PWM_Group_TypeDef Group)
{
	switch(Group)
	{
		case PWM0_Group:
				if(PWM0C & PWMX_FLTXS)return SET;
				else return RESET;			
				break;
		case PWM1_Group:
				if(PWM1C & PWMX_FLTXS)return SET;
				else return RESET;		
				break;
		case PWM2_Group:
				if(PWM2C & PWMX_FLTXS)return SET;
				else return RESET;			
				break;
		default:break;
	}
	return RESET;
}

/**
  * @˵��  	PWM�����ж����ȼ�
  * @����  	ITPriority ��PWM�ж����ȼ�
  *			PWM_ITPriority_0	//PWM�ж����ȼ�0(���)
  *			PWM_ITPriority_1	//PWM�ж����ȼ�1
  *			PWM_ITPriority_2	//PWM�ж����ȼ�2
  *			PWM_ITPriority_3	//PWM�ж����ȼ�3(���)
  * @����ֵ ��
  * @ע 	PWM0��PWM0��PWM0��PWM3����ͬһ�ж�����
  */
void PWM_ITPriorityConfig(PWM_ITPriority_TypeDef ITPriority)
{
	IP2 = (IP2&(~INT_PPWM))|ITPriority;		
}