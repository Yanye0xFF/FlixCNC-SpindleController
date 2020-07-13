/**
*									 ģ�����ܽ���
*	1�����루��SMT��ģʽ��VDD=5Vʱ���͵�ƽ������͵�ѹ��VIL1����ΧΪ0~1.5V���ߵ�ƽ������
*	   �ߵ�ѹ��VIH1����ΧΪ3.5~5V��
*	2�����루SMT��ģʽ��VDD=5Vʱ���͵�ƽ������͵�ѹ��VIL1����ΧΪ0~1V���ߵ�ƽ������ߵ�
*	   ѹ��VIH1����ΧΪ4~5V��
*	3��HC89S003�ϵ縴λ�Լ�������λ���ܵĸ�λ�����е�IO��ؼĴ�������ָ�ΪĬ��ֵ��
*	************************************************************************************
*									 Ӧ��ע������
*	1����ʹ�÷�����ʱ����JTAG���õ�IO�ڻ��ж�ȡ��д�������쳣����˽���ʹ�÷�����ʱ��Ҫ
*	   �����⼸��IO�ڡ��Ͽ�������ʹ�õ�Դ���缴��������������
*	2�����䵽P0.0/P0.1/P0.2�������ܽ��ϵĹ��ܽš��ⲿ�ж����롢���ϼ�����Ŷ����������ơ�
*	3��P2.3/P2.4/P2.5/P2.7�ĸ��ڴ�������ͬʱʹ�ܹ��ܣ���û�д����������Լ����������룬��
*      ������ʹ��ʩ���ش����������Լ�ʩ���ش��������룬ʹ��ʱ��ע�⣨��������ֲᣩ��
*	************************************************************************************
*								   	  ��������
* 	GPIO_DeInit()					//�ָ�GPIOx�Ĵ�����Ĭ��ֵ
* 	GPIO_Init()						//��һ��˿��е�һ������IO����ģʽ
* 	GPIO_Write()					//��һ��˿ڸ�ֵ
* 	GPIO_WriteHigh()				//��һ��˿��е�PIN�ø�
* 	GPIO_WriteLow()					//��һ��˿��е�PIN�õ�
* 	GPIO_WriteReverse()				//��һ��˿��е�PINȡ��
* 	GPIO_ReadInputData()			//��ȡ��ǰ�˿��������״̬
* 	GPIO_ReadOutputData()			//��ȡ��ǰ�˿�������״̬
* 	GPIO_ReadInputPin()				//��ȡһ��˿��е�һ���˿ڵ�״̬
* 	GPIO_P02ExternalPullConfig()	//����P02�˿�����������ֵ
* 	GPIO_GPIO0DbcTimerConfig()		//���ö˿��������ܵ�����ʱ��ֻ��P00��P01��P02�д˹���
*	************************************************************************************
*									  ���㹫ʽ
*	1��P0xDBCT [5:0]���õ�����ʱ����һ����Χ��
*	   ��Ƶϵ��*Tosc*P0xDBCT[5:0]-Tosc<����ʱ��<��Ƶϵ��*Tosc*(P0xDBCT[5:0]+1)-Tosc��
*	************************************************************************************
*  								       �ͻ�����
*	 ��л��ʹ�����ǵĵ�Ƭ���������ִ����Ժ�����ʹ�ô������ʡ�������Ϻ�оʥ���ӹٷ�QQ��
*	****************************����֧��Ⱥ��201030494**********************************
*   ************************************************************************************
**/

#include "hc89s_gpio.h"

/**
  * @˵��  	�ָ�GPIOx�Ĵ�����Ĭ��ֵ
  * @����  	GPIOx : ��ʼ���Ķ˿��飨x:0-2��
  *			GPIOT0	//�˿�0��
  *			GPIOT1 	//�˿�1��
  *			GPIOT2 	//�˿�2��
  * @����ֵ ��
  * @ע		��
  */
void GPIO_DeInit(GPIO_TypeDef GPIOx)
{
	if(GPIOx == GPIOT0)
	{
		P0   	= GPIO_P0_RESET_VALUE;
		PM0F 	= GPIO_PM0F_RESET_VALUE;
	}else if(GPIOx == GPIOT1)
	{
		P1 		= GPIO_P1_RESET_VALUE;
		PM1F 	= GPIO_PM1F_RESET_VALUE;
	}else if(GPIOx == GPIOT2)
	{
		P2 		= GPIO_P2_RESET_VALUE;
		PM2F 	= GPIO_PM2F_RESET_VALUE;	
	}
}

/**
  * @˵��  	��һ��˿��е�һ������IO����ģʽ
  * @����  	GPIOx : ��ʼ���Ķ˿��飨x:0-2��
  *			GPIOT0	//�˿�0��
  *			GPIOT1 	//�˿�1��
  *			GPIOT2 	//�˿�2��
  * @����  	GPIO_PIN : 	��ʼ���Ķ˿ں�
  *			GPIO_PIN_0	//ѡ��˿�0
  *			GPIO_PIN_1  //ѡ��˿�1
  *			GPIO_PIN_2  //ѡ��˿�2
  *			GPIO_PIN_3  //ѡ��˿�3
  *			GPIO_PIN_4  //ѡ��˿�4
  *			GPIO_PIN_5  //ѡ��˿�5
  *			GPIO_PIN_6  //ѡ��˿�6
  *			GPIO_PIN_7  //ѡ��˿�7
  * @����  	GPIO_Mode : ��ʼ��ģʽ
  *			GPIO_MODE_IN      	 	//��������
  *			GPIO_MODE_IN_PD       	//����������
  *			GPIO_MODE_IN_PU       	//����������
  *			GPIO_MODE_IN_AN      	//ģ������
  *			GPIO_MODE_IN_SMT  	 	//��������   SMT
  *			GPIO_MODE_IN_PD_SMT   	//���������� SMT
  *			GPIO_MODE_IN_PU_SMT  	//���������� SMT
  *			GPIO_MODE_IN_AN_PU_PD	//ģ������������ͬʱ�򿪣�B���Ժ�оƬ֧��
  *			GPIO_MODE_OUT_PP 	 	//�������
  *			GPIO_MODE_OUT_OD  	 	//��©���
  *			GPIO_MODE_OUT_OD_PU 	//��©���������
  * @����ֵ ��
  * @ע		��
  */
void GPIO_Init(GPIO_TypeDef GPIOx,GPIO_Pin_TypeDef GPIO_PIN,GPIO_Mode_TypeDef GPIO_Mode)
{
	u8  i;
	u8  xdata *gpioset;
	gpioset	= 0xFF00 | ((GPIOx - 1)<<3);
	for(i=0;i<8;i++){
		if((GPIOx == GPIOT1)&&(i>1))break;
		if(GPIO_PIN&0x01){
			*(gpioset+i/2) =  ((*(gpioset+i/2)) & (0xF0 >> ((i%2)*0x04))) | (GPIO_Mode << ((i%2)*0x04));
		}
		GPIO_PIN >>= 1;
	}
}

/**
  * @˵��  	��һ��˿ڸ�ֵ
  * @����  	GPIOx : ��ʼ���Ķ˿��飨x:0-2��
  *			GPIOT0	//�˿�0��
  *			GPIOT1 	//�˿�1��
  *			GPIOT2 	//�˿�2��
  * @����  	Value : 	�˿����״̬
  *			ȡֵ��Χ��0x00 - 0xFF
  * @����ֵ ��
  * @ע		��
  */
void GPIO_Write(GPIO_TypeDef GPIOx,u8 Value)
{
	if(GPIOx == GPIOT0)P0   	= Value;
	else if(GPIOx == GPIOT1)P1  = Value;
	else if(GPIOx == GPIOT2)P2  = Value;
}


/**
  * @˵��  	��һ��˿��е�PIN�ø�
  * @����  	GPIOx : ���õĶ˿��飨x:0-2��
  *		    GPIOT0	//�˿�0��
  *			GPIOT1 	//�˿�1��
  *			GPIOT2 	//�˿�2��
  * @����  	GPIO_PIN : 	���õĶ˿ں�
  *			GPIO_PIN_0	//ѡ��˿�0
  *			GPIO_PIN_1	//ѡ��˿�1
  *			GPIO_PIN_2	//ѡ��˿�2
  *			GPIO_PIN_3	//ѡ��˿�3
  *			GPIO_PIN_4 	//ѡ��˿�4
  *			GPIO_PIN_5 	//ѡ��˿�5
  *			GPIO_PIN_6 	//ѡ��˿�6
  *			GPIO_PIN_7	//ѡ��˿�7
  * @����ֵ ��
  * @ע		��
  */
void GPIO_WriteHigh(GPIO_TypeDef GPIOx,GPIO_Pin_TypeDef GPIO_PIN)
{
	if(GPIOx == GPIOT0)P0   |= GPIO_PIN;
	else if(GPIOx == GPIOT1)P1   |= GPIO_PIN;
	else if(GPIOx == GPIOT2)P2   |= GPIO_PIN;
}

/**
  * @˵��  	��һ��˿��е�PIN�õ�
  * @����  	GPIOx : ���õĶ˿��飨x:0-2��
  *		    GPIOT0	//�˿�0��
  *			GPIOT1 	//�˿�1��
  *			GPIOT2 	//�˿�2��
  * @����  	GPIO_PIN : 	���õĶ˿ں�
  *			GPIO_PIN_0	//ѡ��˿�0
  *			GPIO_PIN_1	//ѡ��˿�1
  *			GPIO_PIN_2	//ѡ��˿�2
  *			GPIO_PIN_3	//ѡ��˿�3
  *			GPIO_PIN_4 	//ѡ��˿�4
  *			GPIO_PIN_5 	//ѡ��˿�5
  *			GPIO_PIN_6 	//ѡ��˿�6
  *			GPIO_PIN_7	//ѡ��˿�7
  * @����ֵ ��
  * @ע		��
  */
void GPIO_WriteLow(GPIO_TypeDef GPIOx,GPIO_Pin_TypeDef GPIO_PIN)
{
	if(GPIOx == GPIOT0)P0   &=~ GPIO_PIN;
	else if(GPIOx == GPIOT1)P1   &=~ GPIO_PIN;
	else if(GPIOx == GPIOT2)P2   &=~ GPIO_PIN;
}

/**
  * @˵��  	��һ��˿��е�PINȡ��
  * @����  	GPIOx : ���õĶ˿��飨x:0-2��
  *		    GPIOT0	//�˿�0��
  *			GPIOT1 	//�˿�1��
  *			GPIOT2 	//�˿�2��
  * @����  	GPIO_PIN : 	���õĶ˿ں�
  *			GPIO_PIN_0	//ѡ��˿�0
  *			GPIO_PIN_1	//ѡ��˿�1
  *			GPIO_PIN_2	//ѡ��˿�2
  *			GPIO_PIN_3	//ѡ��˿�3
  *			GPIO_PIN_4 	//ѡ��˿�4
  *			GPIO_PIN_5 	//ѡ��˿�5
  *			GPIO_PIN_6 	//ѡ��˿�6
  *			GPIO_PIN_7	//ѡ��˿�7
  * @����ֵ ��
  * @ע		��
  */
void GPIO_WriteReverse(GPIO_TypeDef GPIOx,GPIO_Pin_TypeDef GPIO_PIN)
{
	if(GPIOx == GPIOT0)P0   ^= GPIO_PIN;
	else if(GPIOx == GPIOT1)P1   ^= GPIO_PIN;
	else if(GPIOx == GPIOT2)P2   ^= GPIO_PIN;
}

/**
  * @˵��  	��ȡ��ǰ�˿��������״̬
  * @����  	GPIOx : ���õĶ˿��飨x:0-2��
  *		    GPIOT0	//�˿�0��
  *			GPIOT1 	//�˿�1��
  *			GPIOT2 	//�˿�2��
  * @����ֵ �˿�������״̬
  *			ȡֵ��Χ��	0x00 - 0xFF
  * @ע		��
  */
u8 GPIO_ReadInputData(GPIO_TypeDef GPIOx)
{
	if(GPIOx == GPIOT0)return P0;
	else if(GPIOx == GPIOT1)return P1;
	else if(GPIOx == GPIOT2)return P2;
	return 0x00;
}

/**
  * @˵��  	��ȡ��ǰ�˿�������״̬
  * @����  	GPIOx : ���õĶ˿��飨x:0-2��
  * 		GPIOT0	//�˿�0��
  *			GPIOT1 	//�˿�1��
  *			GPIOT2 	//�˿�2��
  * @����ֵ �˿������״̬
  *			ȡֵ��Χ��	0x00 - 0xFF
  * @ע		��
  */
u8 GPIO_ReadOutputData(GPIO_TypeDef GPIOx)
{
	if(GPIOx == GPIOT0)return P0;
	else if(GPIOx == GPIOT1)return P1;
	else if(GPIOx == GPIOT2)return P2;
	return 0x00;
}


/**
  * @˵��  	��ȡһ��˿��е�һ���˿ڵ�״̬
  * @����  	GPIOx : �˿��飨x:0-2��
  *		    GPIOT0	//�˿�0��
  *			GPIOT1 	//�˿�1��
  *			GPIOT2 	//�˿�2��
  * @����  	GPIO_PIN : 	�˿ں�
  *			GPIO_PIN_0	//ѡ��˿�0
  *			GPIO_PIN_1	//ѡ��˿�1
  *			GPIO_PIN_2	//ѡ��˿�2
  *			GPIO_PIN_3	//ѡ��˿�3
  *			GPIO_PIN_4 	//ѡ��˿�4
  *			GPIO_PIN_5 	//ѡ��˿�5
  *			GPIO_PIN_6 	//ѡ��˿�6
  *			GPIO_PIN_7	//ѡ��˿�7
  * @����ֵ �˿�״̬
  *			ȡֵ��Χ��	0x00 - 0x01
  * @ע		��
  */
bool GPIO_ReadInputPin(GPIO_TypeDef GPIOx,GPIO_Pin_TypeDef GPIO_PIN)
{
	if(GPIOx == GPIOT0)GPIO_PIN &= P0;
	else if(GPIOx == GPIOT1)GPIO_PIN &= P1;
	else if(GPIOx == GPIOT2)GPIO_PIN &= P2;
	if(GPIO_PIN)return 1;
	return 0;
}

/**
  * @˵��  	����P02�˿�����������ֵ
  * @����  	Res_Value : ����ֵ
  * 		R050K	//��������50K
  *			R100K	//��������100K
  *			R150K 	//��������150K
  *			R300K 	//��������300K
  * @����ֵ ��
  * @ע		��
  */
void GPIO_P02ExternalPullConfig(GPIO_PullResValve Res_Value)
{
		P0LPU = (P0LPU&(~GPIO_P02PU))|(Res_Value&GPIO_P02PU);	
}

/**
  * @˵��  	���ö˿��������ܵ�����ʱ��ֻ��P00��P01��P02�д˹���
  * @����  	GPIO_PIN : 	�˿ںţ�x:0-2��
  *			GPIO_PIN_0	//ѡ��˿�0
  *			GPIO_PIN_1	//ѡ��˿�1
  *			GPIO_PIN_2	//ѡ��˿�2
  * @����  	ClockDevice : ����ʱ�ӷ�Ƶ
			FOSC_1		//����ʱ��1��Ƶ
			FOSC_4		//����ʱ��4��Ƶ
			FOSC_16		//����ʱ��16��Ƶ
			FOSC_64		//����ʱ��64��Ƶ
  * @����  	Value : ����ʱ��
  *			ȡֵ��Χ: 0x00 - 0x3F
  * @����ֵ ��
  * @ע		����ʱ�䣨��Ƶϵ�� * Tosc*P0xDBCT [5:0] - Tosc <����ʱ��<��Ƶϵ�� * Tosc* (P0xDBCT [5:0] +1) -Tosc��
  */
void GPIO_GPIO0DbcTimerConfig(GPIO_Pin_TypeDef GPIO_PIN,GPIO_DbcClockDevice ClockDevice,u8 Value)
{
		switch(GPIO_PIN)
		{
			case GPIO_PIN_0:
							P00DBC &= 0x00;
							P00DBC |= (ClockDevice&GPIO_P0xDBCLK);
							P00DBC |= (Value&GPIO_P0xDBCT);
							break;
			case GPIO_PIN_1:
							P01DBC &= 0x00;
							P01DBC |= (ClockDevice&GPIO_P0xDBCLK);
							P01DBC |= (Value&GPIO_P0xDBCT);
							break;
			case GPIO_PIN_2:
							P02DBC &= 0x00;
							P02DBC |= (ClockDevice&GPIO_P0xDBCLK);
							P02DBC |= (Value&GPIO_P0xDBCT);
							break;
			default:break;
		}
}