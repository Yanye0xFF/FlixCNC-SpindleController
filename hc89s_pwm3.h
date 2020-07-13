#ifndef __hc89s_PWM3_H__
#define __hc89s_PWM3_H__

#include "HC89S003F4.h"

/*PWM�����Ч��ƽö����*/						
typedef enum {
	PWM3_HIGH_VALID 			= (u8)0x00,		//����PWM3����Ч
	PWM3_LOW_VALID				= (u8)0x08,		//����PWM3����Ч
}PWM3_VoltValid_TypeDef;

/*PWMʱ�ӷ�Ƶϵ��ö����*/
typedef enum {
	PWM3CK_1						= (u8)0x00,		//PWMʱ��ΪOSCʱ��1��Ƶ
	PWM3CK_2						= (u8)0x01,		//PWMʱ��ΪOSCʱ��2��Ƶ
	PWM3CK_4						= (u8)0x02,		//PWMʱ��ΪOSCʱ��4��Ƶ
	PWM3CK_8						= (u8)0x03,		//PWMʱ��ΪOSCʱ��8��Ƶ
	PWM3CK_16					= (u8)0x04,		//PWMʱ��ΪOSCʱ��16��Ƶ
	PWM3CK_32					= (u8)0x05,		//PWMʱ��ΪOSCʱ��32��Ƶ
	PWM3CK_64					= (u8)0x06,		//PWMʱ��ΪOSCʱ��64��Ƶ
	PWM3CK_128					= (u8)0x07,		//PWMʱ��ΪOSCʱ��128��Ƶ
}PWM3_ClkDevice_TypeDef;

/*PWM3����˿�ö����*/
typedef enum {
  	PWM3_OUTPin_P00       		= (u8)0x00,		//PWM3����˿�ΪP00
  	PWM3_OUTPin_P01       		= (u8)0x01,		//PWM3����˿�ΪP01
  	PWM3_OUTPin_P02       		= (u8)0x02,		//PWM3����˿�ΪP02
  	PWM3_OUTPin_P03  			= (u8)0x03,		//PWM3����˿�ΪP03
  	PWM3_OUTPin_P04  			= (u8)0x04,		//PWM3����˿�ΪP04
  	PWM3_OUTPin_P05  			= (u8)0x05,		//PWM3����˿�ΪP05
  	PWM3_OUTPin_P06  			= (u8)0x06,		//PWM3����˿�ΪP06
	PWM3_OUTPin_P07				= (u8)0x07,		//PWM3����˿�ΪP07
	PWM3_OUTPin_P10				= (u8)0x10,		//PWM3����˿�ΪP10
	PWM3_OUTPin_P11				= (u8)0x11,		//PWM3����˿�ΪP11
	PWM3_OUTPin_P20			    = (u8)0x20,		//PWM3����˿�ΪP20
	PWM3_OUTPin_P21				= (u8)0x21,		//PWM3����˿�ΪP21
	PWM3_OUTPin_P22				= (u8)0x22,		//PWM3����˿�ΪP22
	PWM3_OUTPin_P23				= (u8)0x23,		//PWM3����˿�ΪP23
	PWM3_OUTPin_P24				= (u8)0x24,		//PWM3����˿�ΪP24
	PWM3_OUTPin_P25				= (u8)0x25,		//PWM3����˿�ΪP25
	PWM3_OUTPin_P26				= (u8)0x26,		//PWM3����˿�ΪP26
	PWM3_OUTPin_P27				= (u8)0x27,		//PWM3����˿�ΪP27
}PWM3_OutPin_TypeDef;

/*PWM3�ж����ȼ�ö����*/
typedef enum {
	PWM3_ITPriority_0			= (u8)0x00, 	//PWM3�ж����ȼ�0(���)
	PWM3_ITPriority_1			= (u8)0x40, 	//PWM3�ж����ȼ�1
	PWM3_ITPriority_2			= (u8)0x80,	 	//PWM3�ж����ȼ�2
	PWM3_ITPriority_3			= (u8)0xC0, 	//PWM3�ж����ȼ�3(���)
}PWM3_ITPriority_TypeDef;

void PWM3_DeInit(void);											//�ָ�PWM3�Ĵ�����Ĭ��ֵ
void PWM3_Init(PWM3_VoltValid_TypeDef VoltValid,PWM3_ClkDevice_TypeDef ClkDevice);//����ָ���Ĳ�����ʼ��PWM3
void PWM3_OutPutCmd(FunctionalState NewState);					//PWM3�˿����ʹ��/ʧ��
void PWM3_PeriodConfig(u8 PwmPeriod);							//PWM3��������
void PWM3_DutyCycleConfig(u8 PwmDuty);							//PWM3ռ�ձ�����
void PWM3_PinRemapConfig(PWM3_OutPin_TypeDef OutPin);			//PWM3����ܽ�ӳ��
void PWM3_Cmd(FunctionalState NewState);						//ʹ�ܻ�ʧ��PWM3ģ��
void PWM3_ITCmd(FunctionalState NewState);						//ʹ�ܻ�ʧ��PWM3�ж�
FlagStatus PWM3_GetITFlag(void);								//��ȡPWM3�жϱ�־λ
void PWM3_ClearITFlag(void);									//���PWM3�жϱ�־λ
void PWM3_ITPriorityConfig(PWM3_ITPriority_TypeDef ITPriority);	//PWM�ж����ȼ�����
							
#endif