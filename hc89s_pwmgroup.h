#ifndef __hc89s_PWMGROUP_H__
#define __hc89s_PWMGROUP_H__

#include "HC89S003F4.h"

/*PWMͨ��ö����*/
typedef enum {
	PWM_0	  					= (u8)0x01,	//PWM0ͨ��
	PWM_0_1	  					= (u8)0x02,	//PWM01ͨ��
	PWM_0_All					= (u8)0x03,	//PWM0ȫ��ͨ��
	PWM_1	  					= (u8)0x04,	//PWM1ͨ��
	PWM_1_1	  					= (u8)0x05,	//PWM11ͨ��
	PWM_1_All					= (u8)0x06,	//PWM1ȫ��ͨ��
	PWM_2	  					= (u8)0x07,	//PWM2ͨ��
	PWM_2_1	  					= (u8)0x08,	//PWM21ͨ��
	PWM_2_All					= (u8)0x09,	//PWM2ȫ��ͨ��
}PWM_Channel_TypeDef;

/*PWM��ö����*/
typedef enum {
	PWM0_Group  				= (u8)0x11,	//PWM0��
	PWM1_Group  				= (u8)0x12,	//PWM1��
	PWM2_Group  				= (u8)0x13,	//PWM2��
}PWM_Group_TypeDef;


/*PWM���������ö����*/
typedef enum {
	PWMX_DISABLE_PWMX1_DISABLE 	= (u8)0x00,	//����PWM0�顣��PWM0�������PWM01�����
	PWMX_ENABLE_PWMX1_DISABLE  	= (u8)0x02,	//����PWM0�顣��PWM0�����PWM01�����
	PWMX_DISABLE_PWMX1_ENABLE  	= (u8)0x04,	//����PWM0�顣��PWM0�������PWM01���
	PWMX_ENABLE_PWMX1_ENABLE  	= (u8)0x06,	//����PWM0�顣��PWM0�����PWM01���
}PWM_GroupOutMode_TypeDef;

/*PWM���ģʽö����*/						
typedef enum {
	PWMX_PWX1_HIGH_VALID 		= (u8)0x00,	//����PWM0�顣��PWM0��PWM01��Ϊ����Ч
	PWMX_HIGH_PWX1_LOW_VALID	= (u8)0x04,	//����PWM0�顣��PWM0Ϊ����Ч��PWM01����Ч
	PWMX_LOW_PWX1_HIGH_VALID	= (u8)0x08,	//����PWM0�顣��PWM0Ϊ����Ч��PWM01����Ч
	PWMX_PWX1_LOW_VALID			= (u8)0x0C,	//����PWM0�顣��PWM0��PWM01��Ϊ����Ч
}PWM_VoltValid_TypeDef;

/*PWM��ģʽö����*/
typedef enum {
	COMPLEMENTARY_MODE  		= (u8)0x00,	//����ģʽ
	INDEPENDENT_MODE  			= (u8)0x08,	//����ģʽ
}PWM_GroupMode_TypeDef;

/*����״̬��PWM�����ƽö����*/						
typedef enum {
	PWMX_PWX1_ALL_LOW			= (u8)0x00,	//����PWM0�顣������ʱPWM0��PWM01����͵�ƽ
	PWMX_LOW_PWX1_HIGH			= (u8)0x20,	//����PWM0�顣������ʱPWM0����͵�ƽ��PWM01����ߵ�ƽ
	PWMX_HIGH_PWX1_LOW			= (u8)0x40,	//����PWM0�顣������ʱPWM0����ߵ�ƽ��PWM01����͵�ƽ
	PWMX_PWX1_ALL_HIGH			= (u8)0x60,	//����PWM0�顣������ʱPWM0��PWM01����ߵ�ƽ
}PWM_FltPWMOut_TypeDef;

/*���ϵ�ƽö����*/						
typedef enum {
	FLFPIN_LOW_PWMOFF			= (u8)0x00,	//���ϼ���Ϊ�͵�ƽʱ������
	FLFPIN_HIGH_PWMOFF			= (u8)0x10,	//���ϼ���Ϊ�ߵ�ƽʱ������
}PWM_FltPinVolt_TypeDef;

/*PWMʱ�ӷ�Ƶϵ��ö����*/
typedef enum {
	PWMCK_1						= (u8)0x00,	//PWMʱ��ΪOSCʱ��1��Ƶ
	PWMCK_8						= (u8)0x01,	//PWMʱ��ΪOSCʱ��8��Ƶ
	PWMCK_32					= (u8)0x02,	//PWMʱ��ΪOSCʱ��32��Ƶ
	PWMCK_128					= (u8)0x03,	//PWMʱ��ΪOSCʱ��128��Ƶ
}PWM_ClkDevice_TypeDef;

/*PWMGroup����˿�ö����*/
typedef enum {
  	PWM_OUTPin_P00       		= (u8)0x00,	//PWM����˿�ΪP00
  	PWM_OUTPin_P01       		= (u8)0x01,	//PWM����˿�ΪP01
  	PWM_OUTPin_P02       		= (u8)0x02,	//PWM����˿�ΪP02
  	PWM_OUTPin_P03  			= (u8)0x03,	//PWM����˿�ΪP03
  	PWM_OUTPin_P04  			= (u8)0x04,	//PWM����˿�ΪP04
  	PWM_OUTPin_P05  			= (u8)0x05,	//PWM����˿�ΪP05
  	PWM_OUTPin_P06  			= (u8)0x06,	//PWM����˿�ΪP06
	PWM_OUTPin_P07				= (u8)0x07,	//PWM����˿�ΪP07
	PWM_OUTPin_P10				= (u8)0x10,	//PWM����˿�ΪP10
	PWM_OUTPin_P11				= (u8)0x11,	//PWM����˿�ΪP11
	PWM_OUTPin_P20		    	= (u8)0x20,	//PWM����˿�ΪP20
	PWM_OUTPin_P21				= (u8)0x21,	//PWM����˿�ΪP21
	PWM_OUTPin_P22				= (u8)0x22,	//PWM����˿�ΪP22
	PWM_OUTPin_P23				= (u8)0x23,	//PWM����˿�ΪP23
	PWM_OUTPin_P24				= (u8)0x24,	//PWM����˿�ΪP24
	PWM_OUTPin_P25				= (u8)0x25,	//PWM����˿�ΪP25
	PWM_OUTPin_P26				= (u8)0x26,	//PWM����˿�ΪP26
	PWM_OUTPin_P27				= (u8)0x27,	//PWM����˿�ΪP27
}PWM_OutPin_TypeDef;

/*PWM�ж����ȼ�ö����*/
typedef enum {
	PWM_ITPriority_0			= (u8)0x00, //PWM�ж����ȼ�0(���)
	PWM_ITPriority_1			= (u8)0x40, //PWM�ж����ȼ�1
	PWM_ITPriority_2			= (u8)0x80, //PWM�ж����ȼ�2)
	PWM_ITPriority_3			= (u8)0xC0, //PWM�ж����ȼ�3(���)
}PWM_ITPriority_TypeDef;

/*******PWM����********/
void PWM_DeInit(PWM_Group_TypeDef PWM_Group);										//�ָ�POWER�Ĵ�����Ĭ��ֵ
void PWM_Init(PWM_Group_TypeDef Group,PWM_ClkDevice_TypeDef ClkDevice,PWM_VoltValid_TypeDef GroupOutMode,PWM_GroupMode_TypeDef GroupMode);	//PWM����
void PWM_FLTConfig(PWM_Group_TypeDef Group,PWM_FltPinVolt_TypeDef FltPinVolt,PWM_FltPWMOut_TypeDef FltPWMOut,FunctionalState NewState);	//PWM��������
void PWM_PinRemapConfig(PWM_Channel_TypeDef Channel,PWM_OutPin_TypeDef OutPin);		//PWM����ܽ�ӳ��
void PWM_OutPutCmd(PWM_Channel_TypeDef Channel,FunctionalState NewState);			//PWM�˿����ʹ��/ʧ��
void PWM_PeriodConfig(PWM_Group_TypeDef Group,u16 PwmPeriod);						//PWM��������
void PWM_DutyCycleConfig(PWM_Group_TypeDef Group,u16 PwmDuty);						//PWMռ�ձ�����
void PWM_DeadZoneConfig(PWM_Group_TypeDef Group,u16 PwmDeadZone);					//PWM����ʱ������
void PWM_Cmd(PWM_Group_TypeDef Group,FunctionalState NewState);						//ʹ�ܻ�ʧ��PWMģ��
void PWM_ITCmd(PWM_Group_TypeDef Group,FunctionalState NewState);					//ʹ�ܻ�ʧ��PWM�ж�
FlagStatus PWM_GetITFlag(PWM_Group_TypeDef Group);									//��ȡPWM�жϱ�־λ
void PWM_ClearITFlag(PWM_Group_TypeDef Group);										//���PWM�жϱ�־λ	
FlagStatus PWM_GetFLTFlag(PWM_Group_TypeDef Group);									//��ȡPWM����״̬���ر���/��������
void PWM_ITPriorityConfig(PWM_ITPriority_TypeDef ITPriority);						//PWM�ж����ȼ�����
							
#endif