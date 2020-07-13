/**
*									 模块性能介绍
*	1、输入（无SMT）模式下VDD=5V时，低电平即输入低电压（VIL1）范围为0~1.5V、高电平即输入
*	   高电压（VIH1）范围为3.5~5V。
*	2、输入（SMT）模式下VDD=5V时，低电平即输入低电压（VIL1）范围为0~1V、高电平即输入高电
*	   压（VIH1）范围为4~5V。
*	3、HC89S003上电复位以及其它复位功能的复位，所有的IO相关寄存器都会恢复为默认值。
*	************************************************************************************
*									 应用注意事项
*	1、在使用仿真器时，与JTAG复用的IO口会有读取或写入数据异常，因此建议使用仿真器时不要
*	   操作这几个IO口。断开仿真器使用电源供电即可以正常操作。
*	2、分配到P0.0/P0.1/P0.2这三个管脚上的功能脚、外部中断输入、故障检测引脚都受消抖控制。
*	3、P2.3/P2.4/P2.5/P2.7四个口带上下拉同时使能功能，但没有带上拉输入以及带下拉输入，不
*      过可以使用施密特带下拉输入以及施密特带上拉输入，使用时请注意（详见数据手册）。
*	************************************************************************************
*								   	  函数总览
* 	GPIO_DeInit()					//恢复GPIOx寄存器到默认值
* 	GPIO_Init()						//对一组端口中的一个或多个IO设置模式
* 	GPIO_Write()					//对一组端口赋值
* 	GPIO_WriteHigh()				//对一组端口中的PIN置高
* 	GPIO_WriteLow()					//对一组端口中的PIN置低
* 	GPIO_WriteReverse()				//对一组端口中的PIN取反
* 	GPIO_ReadInputData()			//获取当前端口组的输入状态
* 	GPIO_ReadOutputData()			//获取当前端口组的输出状态
* 	GPIO_ReadInputPin()				//获取一组端口中的一个端口的状态
* 	GPIO_P02ExternalPullConfig()	//设置P02端口上拉电阻阻值
* 	GPIO_GPIO0DbcTimerConfig()		//设置端口消抖功能的消抖时间只有P00，P01，P02有此功能
*	************************************************************************************
*									  计算公式
*	1、P0xDBCT [5:0]配置的消抖时间是一个范围。
*	   分频系数*Tosc*P0xDBCT[5:0]-Tosc<消抖时间<分频系数*Tosc*(P0xDBCT[5:0]+1)-Tosc。
*	************************************************************************************
*  								       客户服务
*	 感谢您使用我们的单片机，若发现错误或对函数的使用存在疑问。请添加上海芯圣电子官方QQ。
*	****************************技术支持群：201030494**********************************
*   ************************************************************************************
**/

#include "hc89s_gpio.h"

/**
  * @说明  	恢复GPIOx寄存器到默认值
  * @参数  	GPIOx : 初始化的端口组（x:0-2）
  *			GPIOT0	//端口0组
  *			GPIOT1 	//端口1组
  *			GPIOT2 	//端口2组
  * @返回值 无
  * @注		无
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
  * @说明  	对一组端口中的一个或多个IO设置模式
  * @参数  	GPIOx : 初始化的端口组（x:0-2）
  *			GPIOT0	//端口0组
  *			GPIOT1 	//端口1组
  *			GPIOT2 	//端口2组
  * @参数  	GPIO_PIN : 	初始化的端口号
  *			GPIO_PIN_0	//选择端口0
  *			GPIO_PIN_1  //选择端口1
  *			GPIO_PIN_2  //选择端口2
  *			GPIO_PIN_3  //选择端口3
  *			GPIO_PIN_4  //选择端口4
  *			GPIO_PIN_5  //选择端口5
  *			GPIO_PIN_6  //选择端口6
  *			GPIO_PIN_7  //选择端口7
  * @参数  	GPIO_Mode : 初始化模式
  *			GPIO_MODE_IN      	 	//数字输入
  *			GPIO_MODE_IN_PD       	//带下拉输入
  *			GPIO_MODE_IN_PU       	//带上拉输入
  *			GPIO_MODE_IN_AN      	//模拟输入
  *			GPIO_MODE_IN_SMT  	 	//数字输入   SMT
  *			GPIO_MODE_IN_PD_SMT   	//带下拉输入 SMT
  *			GPIO_MODE_IN_PU_SMT  	//带上拉输入 SMT
  *			GPIO_MODE_IN_AN_PU_PD	//模拟输入上下拉同时打开，B版以后芯片支持
  *			GPIO_MODE_OUT_PP 	 	//推挽输出
  *			GPIO_MODE_OUT_OD  	 	//开漏输出
  *			GPIO_MODE_OUT_OD_PU 	//开漏带上拉输出
  * @返回值 无
  * @注		无
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
  * @说明  	对一组端口赋值
  * @参数  	GPIOx : 初始化的端口组（x:0-2）
  *			GPIOT0	//端口0组
  *			GPIOT1 	//端口1组
  *			GPIOT2 	//端口2组
  * @参数  	Value : 	端口输出状态
  *			取值范围：0x00 - 0xFF
  * @返回值 无
  * @注		无
  */
void GPIO_Write(GPIO_TypeDef GPIOx,u8 Value)
{
	if(GPIOx == GPIOT0)P0   	= Value;
	else if(GPIOx == GPIOT1)P1  = Value;
	else if(GPIOx == GPIOT2)P2  = Value;
}


/**
  * @说明  	对一组端口中的PIN置高
  * @参数  	GPIOx : 设置的端口组（x:0-2）
  *		    GPIOT0	//端口0组
  *			GPIOT1 	//端口1组
  *			GPIOT2 	//端口2组
  * @参数  	GPIO_PIN : 	设置的端口号
  *			GPIO_PIN_0	//选择端口0
  *			GPIO_PIN_1	//选择端口1
  *			GPIO_PIN_2	//选择端口2
  *			GPIO_PIN_3	//选择端口3
  *			GPIO_PIN_4 	//选择端口4
  *			GPIO_PIN_5 	//选择端口5
  *			GPIO_PIN_6 	//选择端口6
  *			GPIO_PIN_7	//选择端口7
  * @返回值 无
  * @注		无
  */
void GPIO_WriteHigh(GPIO_TypeDef GPIOx,GPIO_Pin_TypeDef GPIO_PIN)
{
	if(GPIOx == GPIOT0)P0   |= GPIO_PIN;
	else if(GPIOx == GPIOT1)P1   |= GPIO_PIN;
	else if(GPIOx == GPIOT2)P2   |= GPIO_PIN;
}

/**
  * @说明  	对一组端口中的PIN置低
  * @参数  	GPIOx : 设置的端口组（x:0-2）
  *		    GPIOT0	//端口0组
  *			GPIOT1 	//端口1组
  *			GPIOT2 	//端口2组
  * @参数  	GPIO_PIN : 	设置的端口号
  *			GPIO_PIN_0	//选择端口0
  *			GPIO_PIN_1	//选择端口1
  *			GPIO_PIN_2	//选择端口2
  *			GPIO_PIN_3	//选择端口3
  *			GPIO_PIN_4 	//选择端口4
  *			GPIO_PIN_5 	//选择端口5
  *			GPIO_PIN_6 	//选择端口6
  *			GPIO_PIN_7	//选择端口7
  * @返回值 无
  * @注		无
  */
void GPIO_WriteLow(GPIO_TypeDef GPIOx,GPIO_Pin_TypeDef GPIO_PIN)
{
	if(GPIOx == GPIOT0)P0   &=~ GPIO_PIN;
	else if(GPIOx == GPIOT1)P1   &=~ GPIO_PIN;
	else if(GPIOx == GPIOT2)P2   &=~ GPIO_PIN;
}

/**
  * @说明  	对一组端口中的PIN取反
  * @参数  	GPIOx : 设置的端口组（x:0-2）
  *		    GPIOT0	//端口0组
  *			GPIOT1 	//端口1组
  *			GPIOT2 	//端口2组
  * @参数  	GPIO_PIN : 	设置的端口号
  *			GPIO_PIN_0	//选择端口0
  *			GPIO_PIN_1	//选择端口1
  *			GPIO_PIN_2	//选择端口2
  *			GPIO_PIN_3	//选择端口3
  *			GPIO_PIN_4 	//选择端口4
  *			GPIO_PIN_5 	//选择端口5
  *			GPIO_PIN_6 	//选择端口6
  *			GPIO_PIN_7	//选择端口7
  * @返回值 无
  * @注		无
  */
void GPIO_WriteReverse(GPIO_TypeDef GPIOx,GPIO_Pin_TypeDef GPIO_PIN)
{
	if(GPIOx == GPIOT0)P0   ^= GPIO_PIN;
	else if(GPIOx == GPIOT1)P1   ^= GPIO_PIN;
	else if(GPIOx == GPIOT2)P2   ^= GPIO_PIN;
}

/**
  * @说明  	获取当前端口组的输入状态
  * @参数  	GPIOx : 设置的端口组（x:0-2）
  *		    GPIOT0	//端口0组
  *			GPIOT1 	//端口1组
  *			GPIOT2 	//端口2组
  * @返回值 端口组输入状态
  *			取值范围：	0x00 - 0xFF
  * @注		无
  */
u8 GPIO_ReadInputData(GPIO_TypeDef GPIOx)
{
	if(GPIOx == GPIOT0)return P0;
	else if(GPIOx == GPIOT1)return P1;
	else if(GPIOx == GPIOT2)return P2;
	return 0x00;
}

/**
  * @说明  	获取当前端口组的输出状态
  * @参数  	GPIOx : 设置的端口组（x:0-2）
  * 		GPIOT0	//端口0组
  *			GPIOT1 	//端口1组
  *			GPIOT2 	//端口2组
  * @返回值 端口组输出状态
  *			取值范围：	0x00 - 0xFF
  * @注		无
  */
u8 GPIO_ReadOutputData(GPIO_TypeDef GPIOx)
{
	if(GPIOx == GPIOT0)return P0;
	else if(GPIOx == GPIOT1)return P1;
	else if(GPIOx == GPIOT2)return P2;
	return 0x00;
}


/**
  * @说明  	获取一组端口中的一个端口的状态
  * @参数  	GPIOx : 端口组（x:0-2）
  *		    GPIOT0	//端口0组
  *			GPIOT1 	//端口1组
  *			GPIOT2 	//端口2组
  * @参数  	GPIO_PIN : 	端口号
  *			GPIO_PIN_0	//选择端口0
  *			GPIO_PIN_1	//选择端口1
  *			GPIO_PIN_2	//选择端口2
  *			GPIO_PIN_3	//选择端口3
  *			GPIO_PIN_4 	//选择端口4
  *			GPIO_PIN_5 	//选择端口5
  *			GPIO_PIN_6 	//选择端口6
  *			GPIO_PIN_7	//选择端口7
  * @返回值 端口状态
  *			取值范围：	0x00 - 0x01
  * @注		无
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
  * @说明  	设置P02端口上拉电阻阻值
  * @参数  	Res_Value : 电阻值
  * 		R050K	//上拉电阻50K
  *			R100K	//上拉电阻100K
  *			R150K 	//上拉电阻150K
  *			R300K 	//上拉电阻300K
  * @返回值 无
  * @注		无
  */
void GPIO_P02ExternalPullConfig(GPIO_PullResValve Res_Value)
{
		P0LPU = (P0LPU&(~GPIO_P02PU))|(Res_Value&GPIO_P02PU);	
}

/**
  * @说明  	设置端口消抖功能的消抖时间只有P00，P01，P02有此功能
  * @参数  	GPIO_PIN : 	端口号（x:0-2）
  *			GPIO_PIN_0	//选择端口0
  *			GPIO_PIN_1	//选择端口1
  *			GPIO_PIN_2	//选择端口2
  * @参数  	ClockDevice : 消抖时钟分频
			FOSC_1		//消抖时钟1分频
			FOSC_4		//消抖时钟4分频
			FOSC_16		//消抖时钟16分频
			FOSC_64		//消抖时钟64分频
  * @参数  	Value : 消抖时间
  *			取值范围: 0x00 - 0x3F
  * @返回值 无
  * @注		消抖时间（分频系数 * Tosc*P0xDBCT [5:0] - Tosc <消抖时间<分频系数 * Tosc* (P0xDBCT [5:0] +1) -Tosc）
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