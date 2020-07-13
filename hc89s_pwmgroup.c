/**
*									 模块性能介绍
*	1、HC89S003集成了3个12位PWM模块PWM0、PWM1、PWM2，每个模块各有一个计数器，三个PWM模块
*	   的功能以及操作完全一样。
*	2、提供每个PWM周期溢出中断，但中断共用同一向量入口。
*	3、提供出错帧测功能可紧急关闭PWM输出。
*	************************************************************************************
*									 应用注意事项
*	1、互补输出模式及独立输出模式都可受故障检测脚控制。
*	2、修改PWM0工作模式时建议先关闭PWM0模块。
*	3、关闭时，PWM0计数停止，输出立即关闭。打开时，PWM0计数器都重新从1开始计数，输出受
*	   PWM0_OEN和PWM01_OEN控制。
*	4、PWM0允许输出，必须在PWM0_EN置1下才有效，否则PWM0输出关闭状态（输出时对应端口必须
*	   设为输出模式）；即使都禁止输出，只要PWM0_EN位被使能，PWM0都可以溢出中断，即此时
*	   PWM0可以作为定时器使用，此控制位修改立即生效。
*	5、对于独立模式，输出模式选择位同样有效，但与互补模式不同的是有效期间为占空比期间；
*	   而互补模式中对于PWM0的有效期间为占空比期间，PWM01的有效期间为占空比的互补期间。
*	************************************************************************************
*								       函数总览
*	PWM_DeInit()							//恢复PWM寄存器到默认值
*	PWM_Init()								//根据指定的参数初始化PWM
*	PWM_FLTConfig()							//PWM组故障检测相关设置
*	PWM_PinRemapConfig()					//配置PWM输出的管脚映射
*	PWM_OutPutCmd()							//PWM端口输出使能/失能
*	PWM_PeriodConfig()						//PWM周期设置
*	PWM_DutyCycleConfig()					//PWM占空比设置
*	PWM_DeadZoneConfig()					//PWM死区时间设置
*	PWM_Cmd()								//使能或失能PWM模块
*	PWM_ITCmd()								//使能或失能PWM中断
*	PWM_GetITFlag()							//获取PWM中断标志位
*	PWM_ClearITFlag()						//清除PWM中断标志位
*	PWM_GetFLTFlag()						//获取PWM故障状态，关闭中/正常运行
*	PWM_ITPriorityConfig();					//PWM中断优先级配置
*	************************************************************************************
*									   计算公式
*	1、周期计算
*	   周期 = PWMxP / (Fosc / PWM分频系数)			 (x = 0,1,2)
*	2、有效电平计算（即占空比）
*	   有效电平 = PWMxD / (Fosc / PWM分频系数)		 (x = 0,1,2)
*	   占空比 = 有效电平 / 周期
*	3、死区调节计算
*	   死区	= PWMxDT  / (Fosc / PWM分频系数)		 (x = 0,1,2)
*	   死区时间 = 死区 / 周期 
*	4、PWM定时时间计算(PWM作为定时器使用时)
*	   定时时间 = PWMxP / (Fosc / PWM分频系数)		 (x = 0,1,2)
*	************************************************************************************
**/

#include "hc89s_pwmgroup.h"

/**
  * @说明  	恢复PWM寄存器到默认值
  * @参数  	无
  * @返回值 无
  * @注 	无
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
  * @说明  	根据指定的参数初始化PWM
  * @参数  	Group ：PWM组
  *			PWM0_Group		//PWM0组
  *			PWM1_Group		//PWM1组
  *			PWM2_Group		//PWM2组
  * @参数  	ClkDevice ：	PWM时钟分频系数
  *			PWMCK_1		//PWM时钟为OSC时钟1分频
  *			PWMCK_8		//PWM时钟为OSC时钟8分频
  *			PWMCK_32	//PWM时钟为OSC时钟32分频
  *			PWMCK_128	//PWM时钟为OSC时钟128分频
  * @参数  	GroupOutMode ：	PWM输出模式
  *			PWMX_DISABLE_PWMX1_DISABLE	//例：PWM0组。即PWM0不输出，PWM01不输出
  *			PWMX_ENABLE_PWMX1_DISABLE 	//例：PWM0组。即PWM0输出，PWM01不输出
  *			PWMX_DISABLE_PWMX1_ENABLE	//例：PWM0组。即PWM0不输出，PWM01输出
  *			PWMX_ENABLE_PWMX1_ENABLE 	//例：PWM0组。即PWM0输出，PWM01输出
  * @参数  	GroupMode ：	PWM组模式
  *			COMPLEMENTARY_MODE	//互补模式
  *			INDEPENDENT_MODE 	//独立模式
  * @返回值 无
  * @注 	无
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
  * @说明  	PWM组故障检测相关设置
  * @参数  	Group ：PWM组
  *			PWM0_Group		//PWM0组
  *			PWM1_Group		//PWM1组
  *			PWM2_Group		//PWM2组
  * @参数  	FltPinVolt ：	PWM故障检测的电平
  *			FLFPIN_LOW_PWMOFF	//故障检测脚为低电平时，故障
  *			FLFPIN_HIGH_PWMOFF	//故障检测脚为高电平时，故障
  * @参数  	FltPWMOut ：	故障时PWM输出电平状态
  *			PWMX_PWX1_ALL_LOW	//例：PWM0组。即故障时PWM0，PWM01输出高电平
  *			PWMX_LOW_PWX1_HIGH	//例：PWM0组。即故障时PWM0输出低电平，PWM01输出高电平
  *			PWMX_HIGH_PWX1_LOW	//例：PWM0组。即故障时PWM0输出高电平，PWM01输出低电平
  *			PWMX_PWX1_ALL_HIGH	//例：PWM0组。即故障时PWM0，PWM01输出低电平
  * @参数  	NewState ：		故障检测新状态
  *			DISABLE		//失能
  *			ENABLE		//使能
  * @返回值 无
  * @注 	无
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
  * @说明  	配置PWM输出的管脚映射
   * @参数  Channel ：PWM输出源
  *			PWM_0  			//PWM0通道
  *			PWM_0_1  		//PWM01通道
  *			PWM_1  			//PWM1通道
  *			PWM_1_1  		//PWM11通道
  *			PWM_2  			//PWM2通道
  *			PWM_2_1  		//PWM21通道
  * @参数  	OutPin ：管脚输出源
  *			PWM_OutPin_P00  //PWM输出端口为P00
  *			PWM_OutPin_P01  //PWM输出端口为P01
  *			PWM_OutPin_P02  //PWM输出端口为P02
  *			PWM_OutPin_P03  //PWM输出端口为P03
  *			PWM_OutPin_P04  //PWM输出端口为P04
  *			PWM_OutPin_P05  //PWM输出端口为P05
  *			PWM_OutPin_P06  //PWM输出端口为P06
  *			PWM_OutPin_P07	//PWM输出端口为P07
  *			PWM_OutPin_P10	//PWM输出端口为P10
  * 		PWM_OutPin_P11	//PWM输出端口为P11
  *			PWM_OutPin_P20	//PWM输出端口为P20
  *			PWM_OutPin_P21	//PWM输出端口为P21
  *			PWM_OutPin_P22	//PWM输出端口为P22
  *			PWM_OutPin_P23	//PWM输出端口为P23
  *			PWM_OutPin_P24	//PWM输出端口为P24
  *			PWM_OutPin_P25	//PWM输出端口为P25
  *			PWM_OutPin_P26	//PWM输出端口为P26
  *			PWM_OutPin_P27	//PWM输出端口为P27
  * @返回值 无
  * @注 	无
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
  * @说明  	PWM端口输出使能/失能
  * @参数  	Channel ：PWM通道
  *			PWM_0	 		//PWM0通道
  *			PWM_0_1			//PWM01通道
  *			PWM_0_All		//PWM0全部通道
  *			PWM_1			//PWM1通道
  *			PWM_1_1	  		//PWM11通道
  *			PWM_1_All		//PWM1全部通道
  *			PWM_2	  		//PWM2通道
  *			PWM_2_1	  		//PWM21通道
  *			PWM_2_All		//PWM2全部通道
  * @参数  	NewState ：PWM通道输出新状态
  *			DISABLE		//失能
  *			ENABLE		//使能
  * @返回值 无
  * @注 	无
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
  * @说明  	PWM周期设置
  * @参数  	Group ：PWM组
  *			PWM0_Group		//PWM0组
  *			PWM1_Group		//PWM1组
  *			PWM2_Group		//PWM2组
  
  * @参数  	PwmDeadZone ：PWM周期
  *         取值范围 0 - 0x0FFF
  * @返回值 无
  * @注 	无
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
  * @说明  	PWM占空比设置
  * @参数  	Group ：PWM组
  *			PWM0_Group		//PWM0组
  *			PWM1_Group		//PWM1组
  *			PWM2_Group		//PWM2组
  
  * @参数  	PwmDeadZone ：PWM占空比
  *         取值范围 0 - 0x0FFF
  * @返回值 无
  * @注 	无
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
  * @说明  	PWM死区时间设置
  * @参数  	Group ：PWM组
  *			PWM0_Group		//PWM0组
  *			PWM1_Group		//PWM1组
  *			PWM2_Group		//PWM2组
  
  * @参数  	PwmDeadZone ：PWM死区时间
  *         取值范围 0 - 0x0FFF
  * @返回值 无
  * @注 	无
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
  * @说明  	使能或失能PWM模块
  * @参数  	Group ：PWM组
  *			PWM0_Group		//PWM0组
  *			PWM1_Group		//PWM1组
  *			PWM2_Group		//PWM2组
  
  * @参数  	NewState ：PWM模块新状态
  *			DISABLE		//失能
  *			ENABLE		//使能
  * @返回值 无
  * @注 	无
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
  * @说明  	使能或失能PWM中断
  * @参数  	Group ：PWM组
  *			PWM0_Group		//PWM0组
  *			PWM1_Group		//PWM1组
  *			PWM2_Group		//PWM2组
 
  * @参数  	NewState ：PWM中断功能新状态
  *			DISABLE		//失能
  *			ENABLE		//使能
  * @返回值 无
  * @注 	无
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
  * @说明  	获取PWM中断标志位
  * @参数  	Group ：PWM组
  *			PWM0_Group		//PWM0组
  *			PWM1_Group		//PWM1组
  *			PWM2_Group		//PWM2组
  
  * @返回值 FlagStatus ：表明PWM中断状态
  *			参考 FlagStatus 枚举类
  * @注 	无
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
  * @说明  	清除PWM中断标志位
  * @参数  	Group ：PWM组
  *			PWM0_Group		//PWM0组
  *			PWM1_Group		//PWM1组
  *			PWM2_Group		//PWM2组
  
* @返回值 无
  * @注 	需要软件清除PWM中断标志位
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
  * @说明  	获取PWM故障状态，关闭中/正常运行
  * @参数  	Group ：PWM组
  *			PWM0_Group		//PWM0组
  *			PWM1_Group		//PWM1组
  *			PWM2_Group		//PWM2组
  
  * @返回值 FlagStatus ：表明PWM故障状态
  *			参考 FlagStatus 枚举类
  * @注 	无
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
  * @说明  	PWM设置中断优先级
  * @参数  	ITPriority ：PWM中断优先级
  *			PWM_ITPriority_0	//PWM中断优先级0(最低)
  *			PWM_ITPriority_1	//PWM中断优先级1
  *			PWM_ITPriority_2	//PWM中断优先级2
  *			PWM_ITPriority_3	//PWM中断优先级3(最高)
  * @返回值 无
  * @注 	PWM0，PWM0，PWM0，PWM3共用同一中断向量
  */
void PWM_ITPriorityConfig(PWM_ITPriority_TypeDef ITPriority)
{
	IP2 = (IP2&(~INT_PPWM))|ITPriority;		
}