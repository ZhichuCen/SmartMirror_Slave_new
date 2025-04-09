////////////////////////////////////////////////////////////////////////////////
//  文件名：    MCU_CAN.c
//  作用描述：  CAN模块的实现文件
//  
//  修改历史：
//  版本        参与人员        日期            修改说明
//  --------    --------------  ------------    --------------------------------
//  1.0.0       黄文卿          2009-07-06      创建文件
////////////////////////////////////////////////////////////////////////////////
#include "CAN.h"
#include "Platform_Types.h"


#define CONF_INCLK		12000000UL      //输入单片机的时钟频率，在此为8M的晶振时钟
#define CONF_OUTCLK		32000000UL      //输出频率，在此为系统的总线频率，32M


#define CANBuffMaxLength  	(8)         //每一通道发送和接收缓冲区的大小    
#define CAN_BUSFREQ			CONF_OUTCLK

const UINT8 c_MCU_CAN_ChannelMapping[5] = {0x00,0x02,0x03,0x04,0x01};
//XE中如果它有一路CAN，则为CAN0，如果有两路，则为CAN4,三路则为CAN0,CAN1,CAN2
//四路为CAN0,CAN1,CAN2,CAN4,五路为CAN0,CAN1,CAN2,CAN3,CAN4
//CAN4的地位相当于插到了CAN0和CAN1之间，因此用此常数组作一映射


MCU_CAN_MSG g_MCU_CAN_RXBuff[CANMaxChannels][CANBuffMaxLength]; //接收缓冲区
Boolean		g_MCU_CAN_RXFull[CANMaxChannels];                   //接收缓冲区满标志
UINT8		g_MCU_CAN_RXFront[CANMaxChannels];                  //接收缓冲区队头
UINT8   	g_MCU_CAN_RXRear[CANMaxChannels];                   //接收缓冲区队尾


//模块内部使用函数声明
ERROR MCU_CAN_AddRxData(UINT8 channel, MCU_CAN_MSG   data);
ERROR MCU_CAN_GetRxData(UINT8 channel, MCU_CAN_MSG * data);

////////////////////////////////////////////////////////////////////////////////
//  函数名：	MCU_CAN_Enable
//  作用描述：	使能CAN模块
//  
//  返回值描述：E_OK		设定成功
//				
//  参数描述： 	channel		通道
//			
//  
//  修改历史：
//  版本        参与人员        日期            修改说明
//  --------    --------------  ------------    --------------------------------
//  1.0.0       黄文卿        	2009-07-11      创建函数
////////////////////////////////////////////////////////////////////////////////
ERROR MCU_CAN_Enable(UINT8 channel)
{
	switch(channel)
	{
		case 0:
			CAN0CTL1_CANE  = 0x01;     //使能CAN模块			
			return E_OK;
			
			
#if CANMaxChannels > 2	//XE中CAN数目大于2,就有CAN1			
		case 1:
			CAN1CTL1_CANE  = 0x01;     //使能CAN模块			
			return E_OK;
#endif

#if CANMaxChannels > 3	//XE中CAN数目大于3,就有CAN2			
		case 2:
			CAN2CTL1_CANE  = 0x01;     //使能CAN模块			
			return E_OK;
#endif

#if CANMaxChannels > 4	//XE中CAN数目大于4,就有CAN3		
		case 3:
			CAN3CTL1_CANE  = 0x01;     //使能CAN模块			
			return E_OK;
#endif

#if CANMaxChannels > 1	//XE中CAN数目大于1,就有CAN4			
		case 4:
			CAN4CTL1_CANE  = 0x01;     //使能CAN模块			
			return E_OK;
#endif
        default:
            return E_FAIL;

	}
}




////////////////////////////////////////////////////////////////////////////////
//  函数名：	MCU_CAN_EnterInitMode
//  作用描述：	设定CAN模块进入初始化模式
//  
//  返回值描述：E_OK		成功进入初始化状态
//				E_FAIL      超时，设定初始化状态失败
//  参数描述：	channel		通道
//			
//  
//  修改历史：
//  版本        参与人员        日期            修改说明
//  --------    --------------  ------------    --------------------------------
//  1.0.0       黄文卿        	2009-07-11      创建函数
////////////////////////////////////////////////////////////////////////////////
ERROR MCU_CAN_EnterInitMode(UINT8 channel)
{
	UINT16 l_InitModeRetry;                     //等待变量
		
	switch(channel)
	{
		case 0:
			CAN0CTL0_INITRQ = 0x01;             //进入初始化设置模式
			for(l_InitModeRetry = 0; l_InitModeRetry < 60000; l_InitModeRetry++)
			{
				if(CAN0CTL1_INITAK == 0x01)
				{
					return E_OK;					
				}
			}
			return E_FAIL;
			
			
#if CANMaxChannels > 2			
		case 1:
			CAN1CTL0_INITRQ = 0x01;             //进入初始化设置模式
			for(l_InitModeRetry = 0; l_InitModeRetry < 60000; l_InitModeRetry++)
			{
				if(CAN1CTL1_INITAK == 0x01)
				{
					return E_OK;					
				}
			}
			return E_FAIL;
#endif

#if CANMaxChannels > 3			
		case 2:
			CAN2CTL0_INITRQ = 0x01;             //进入初始化设置模式
			for(l_InitModeRetry = 0; l_InitModeRetry < 60000; l_InitModeRetry++)
			{
				if(CAN2CTL1_INITAK == 0x01)
				{
					return E_OK;					
				}
			}
			return E_FAIL;
#endif

#if CANMaxChannels > 4			
		case 3:
			CAN3CTL0_INITRQ = 0x01;             //进入初始化设置模式
			for(l_InitModeRetry = 0; l_InitModeRetry < 60000; l_InitModeRetry++)
			{
				if(CAN3CTL1_INITAK == 0x01)
				{
					return E_OK;					
				}
			}
			return E_FAIL;
#endif

#if CANMaxChannels > 1			
		case 4:
			CAN4CTL0_INITRQ = 0x01;             //进入初始化设置模式
			for(l_InitModeRetry = 0; l_InitModeRetry < 60000; l_InitModeRetry++)
			{
				if(CAN4CTL1_INITAK == 0x01)
				{
					return E_OK;					
				}
			}
			return E_FAIL;
#endif

		default:
			return E_FAIL;
			
	}
	
}

////////////////////////////////////////////////////////////////////////////////
//  函数名：	MCU_CAN_Init
//  作用描述：	设定CAN的一些初始化
//  
//  返回值描述：E_OK		设定成功
//				
//  参数描述：	无
//			
//  修改历史：
//  版本        参与人员        日期            修改说明
//  --------    --------------  ------------    --------------------------------
//  1.0.0       黄文卿        	2009-07-11      创建函数
////////////////////////////////////////////////////////////////////////////////
ERROR MCU_CAN_Init(void)
{
    UINT16  l_i, l_j;       //临时行、列索引
    
    //初始化数据
    MCU_CAN_MSG l_TmpMsg={{0,0,0,0,0,0,0,0},0,0};
    
	if(CAN0CTL1_INITAK == 0x00)
	{
		return E_FAIL;
	}

    
    for(l_j = 0; l_j < CANMaxChannels; l_j++)
    {
        for(l_i = 0; l_i < CANBuffMaxLength; l_i++)
        {
            g_MCU_CAN_RXBuff[l_j][l_i]  = l_TmpMsg;
        };
        
        g_MCU_CAN_RXFront[l_j]          = 0;
        g_MCU_CAN_RXRear[l_j]           = 0;
        g_MCU_CAN_RXFull[l_j]           = FALSE;
    };


    for(l_j = 0; l_j < CANMaxChannels; l_j++)
    {
        
    	switch(l_j)
    	{
    		case 0:
    			CAN0CTL1_LOOPB          = 0x00;     //禁止自发自收模式
    			CAN0CTL1_LISTEN         = 0x00;     //禁止只听模式
    			CAN0CTL1_BORM           = 0x00;     //自动掉线恢复
    			CAN0CTL1_WUPM           = 0x00;     //CAN总线上的任何显性位唤醒CAN模块(SLEEP模式下)
    			break;
    			
#if CANMaxChannels > 2			
    		case 2:
    			CAN1CTL1_LOOPB          = 0x00;     //禁止自发自收模式
    			CAN1CTL1_LISTEN         = 0x00;     //禁止只听模式
    			CAN1CTL1_BORM           = 0x00;     //自动掉线恢复
    			CAN1CTL1_WUPM           = 0x00;     //CAN总线上的任何显性位唤醒CAN模块(SLEEP模式下)
				break;
#endif

#if CANMaxChannels > 3			
    		case 3:
    			CAN2CTL1_LOOPB          = 0x00;     //禁止自发自收模式
    			CAN2CTL1_LISTEN         = 0x00;     //禁止只听模式
    			CAN2CTL1_BORM           = 0x00;     //自动掉线恢复
    			CAN2CTL1_WUPM           = 0x00;     //CAN总线上的任何显性位唤醒CAN模块(SLEEP模式下)
				break;
#endif

#if CANMaxChannels > 4			
    		case 4:
    			CAN3CTL1_LOOPB          = 0x00;     //禁止自发自收模式
    			CAN3CTL1_LISTEN         = 0x00;     //禁止只听模式
    			CAN3CTL1_BORM           = 0x00;     //自动掉线恢复
    			CAN3CTL1_WUPM           = 0x00;     //CAN总线上的任何显性位唤醒CAN模块(SLEEP模式下)
				break;
#endif

#if CANMaxChannels > 1			
    		case 1:
    			CAN4CTL1_LOOPB          = 0x00;     //禁止自发自收模式
    			CAN4CTL1_LISTEN         = 0x00;     //禁止只听模式
    			CAN4CTL1_BORM           = 0x00;     //自动掉线恢复
    			CAN4CTL1_WUPM           = 0x00;     //CAN总线上的任何显性位唤醒CAN模块(SLEEP模式下)
				break;
#endif
			default:
				return E_FAIL;

    	}
    }
    return E_OK;
	
	
}




////////////////////////////////////////////////////////////////////////////////
//  函数名：	MCU_CAN_SetFreq
//  作用描述：	设定CAN模块的频率
//  
//  返回值描述：E_OK	    设定成功	
//				E_FAIL      未找到合适的分频或未进入初始化模式，设定失败
//			
//  参数描述：	channel 	CAN的通道数
//			   	frequence	极举的CAN的频率值
//  
//  修改历史：
//  版本        参与人员        日期            修改说明
//  --------    --------------  ------------    --------------------------------
//  1.0.0       黄文卿        	2009-07-11      创建函数
////////////////////////////////////////////////////////////////////////////////
ERROR MCU_CAN_SetFreq(UINT8 channel, MCU_CAN_FREQ frequence)
{
	UINT8 	l_Prescale;		//总线频率或者是晶振频率与目标频率的比值，即总分频
	UINT8 	l_TimSeg;		//(1+TimeSegment1+TimeSegment2)的总和
	switch(channel)
	{
		case 0:
			if(CAN0CTL1_INITAK == 0x01)		//如果没有进入初始化模式，设置无效
			{
				CAN0BTR0_SJW 	= 0x00;		//同步跳转宽度为1Tq
				l_Prescale = (UINT8)(CONF_INCLK / 1000U / frequence);
				if(l_Prescale >= 8)			//尽量使用晶振时钟
				{
					CAN0CTL1_CLKSRC	= 0x00;		//CAN时钟来自于晶振时钟
				}
				else
				{
				
					CAN0CTL1_CLKSRC	= 0x01;		//CAN时钟来自于总线时钟
					l_Prescale = (UINT8)(CAN_BUSFREQ / 1000U / frequence);
				}
				for(l_TimSeg = 8; l_TimSeg <= 25; l_TimSeg++)	//遍历寻找合适的分频值
				{
					if(((l_Prescale / l_TimSeg) * l_TimSeg) ==  l_Prescale)
					{									 		//刚好能除尽，则分频合适
						CAN0BTR0_BRP		= l_Prescale / l_TimSeg - 1;
																//设置预分频数
						CAN0BTR1_TSEG_20	= (UINT8)((l_TimSeg >= 18) ? 7 : (l_TimSeg / 2 - 2));
						CAN0BTR1_TSEG_10	= l_TimSeg  - CAN0BTR1_TSEG_20 - 3 ;
						return E_OK;										   
					}
				}
				
			}
			return E_FAIL;	//未进入初始化模式或没有合适的分频，设置频率失败
			
			
#if CANMaxChannels > 2			
		case 1:
			if(CAN1CTL1_INITAK == 0x01)		//如果没有进入初始化模式，设置无效
			{
				CAN1BTR0_SJW 	= 0x00;		//同步跳转宽度为1Tq
				l_Prescale = (UINT8)(CONF_INCLK / 1000U / frequence);
				if(l_Prescale >= 8)			//尽量使用晶振时钟
				{
					CAN1CTL1_CLKSRC	= 0x00;		//CAN时钟来自于晶振时钟
				}
				else
				{
				
					CAN1CTL1_CLKSRC	= 0x01;		//CAN时钟来自于总线时钟
					l_Prescale = (UINT8)(CAN_BUSFREQ / 1000U / frequence);
				}
				for(l_TimSeg = 8; l_TimSeg <= 25; l_TimSeg++)	//遍历寻找合适的分频值
				{
					if(((l_Prescale / l_TimSeg) * l_TimSeg) ==  l_Prescale)
					{									 		//刚好能除尽，则分频合适
						CAN1BTR0_BRP		= l_Prescale / l_TimSeg - 1;
																//设置预分频数
						CAN1BTR1_TSEG_20	= (UINT8)((l_TimSeg >= 18) ? 7 : (l_TimSeg / 2 - 2));
						CAN1BTR1_TSEG_10	= l_TimSeg  - CAN1BTR1_TSEG_20 - 3 ;
						return E_OK;										   
					}
				}
				
			}
			return E_FAIL;	//未进入初始化模式或没有合适的分频，设置频率失败
#endif

#if CANMaxChannels > 3			
		case 2:
			if(CAN2CTL1_INITAK == 0x01)		//如果没有进入初始化模式，设置无效
			{
				CAN2BTR0_SJW 	= 0x00;		//同步跳转宽度为1Tq
				l_Prescale = (UINT8)(CONF_INCLK / 1000U / frequence);
				if(l_Prescale >= 8)			//尽量使用晶振时钟
				{
					CAN2CTL1_CLKSRC	= 0x00;		//CAN时钟来自于晶振时钟
				}
				else
				{
				
					CAN2CTL1_CLKSRC	= 0x01;		//CAN时钟来自于总线时钟
					l_Prescale = (UINT8)(CAN_BUSFREQ / 1000U / frequence);
				}
				for(l_TimSeg = 8; l_TimSeg <= 25; l_TimSeg++)	//遍历寻找合适的分频值
				{
					if(((l_Prescale / l_TimSeg) * l_TimSeg) ==  l_Prescale)
					{									 		//刚好能除尽，则分频合适
						CAN2BTR0_BRP		= l_Prescale / l_TimSeg - 1;
																//设置预分频数
						CAN2BTR1_TSEG_20	= (UINT8)((l_TimSeg >= 18) ? 7 : (l_TimSeg / 2 - 2));
						CAN2BTR1_TSEG_10	= l_TimSeg  - CAN2BTR1_TSEG_20 - 3 ;
						return E_OK;										   
					}
				}
				
			}
			return E_FAIL;	//未进入初始化模式或没有合适的分频，设置频率失败
#endif

#if CANMaxChannels > 4			
		case 3:
			if(CAN3CTL1_INITAK == 0x01)		//如果没有进入初始化模式，设置无效
			{
				CAN3BTR0_SJW 	= 0x00;		//同步跳转宽度为1Tq
				l_Prescale = (UINT8)(CONF_INCLK / 1000U / frequence);
				if(l_Prescale >= 8)			//尽量使用晶振时钟
				{
					CAN3CTL1_CLKSRC	= 0x00;		//CAN时钟来自于晶振时钟
				}
				else
				{
				
					CAN3CTL1_CLKSRC	= 0x01;		//CAN时钟来自于总线时钟
					l_Prescale = (UINT8)(CAN_BUSFREQ / 1000U / frequence);
				}
				for(l_TimSeg = 8; l_TimSeg <= 25; l_TimSeg++)	//遍历寻找合适的分频值
				{
					if(((l_Prescale / l_TimSeg) * l_TimSeg) ==  l_Prescale)
					{									 		//刚好能除尽，则分频合适
						CAN3BTR0_BRP		= l_Prescale / l_TimSeg - 1;
																//设置预分频数
						CAN3BTR1_TSEG_20	= (UINT8)((l_TimSeg >= 18) ? 7 : (l_TimSeg / 2 - 2));
						CAN3BTR1_TSEG_10	= l_TimSeg  - CAN3BTR1_TSEG_20 - 3 ;
						return E_OK;										   
					}
				}
				
			}
			return E_FAIL;	//未进入初始化模式或没有合适的分频，设置频率失败
#endif

#if CANMaxChannels > 1			
		case 4:
			if(CAN4CTL1_INITAK == 0x01)		//如果没有进入初始化模式，设置无效
			{
				CAN4BTR0_SJW 	= 0x00;		//同步跳转宽度为1Tq
				l_Prescale = (UINT8)(CONF_INCLK / 1000U / frequence);
				if(l_Prescale >= 8)			//尽量使用晶振时钟
				{
					CAN4CTL1_CLKSRC	= 0x00;		//CAN时钟来自于晶振时钟
				}
				else
				{
				
					CAN4CTL1_CLKSRC	= 0x01;		//CAN时钟来自于总线时钟
					l_Prescale = (UINT8)(CAN_BUSFREQ / 1000U / frequence);
				}
				for(l_TimSeg = 8; l_TimSeg <= 25; l_TimSeg++)	//遍历寻找合适的分频值
				{
					if(((l_Prescale / l_TimSeg) * l_TimSeg) ==  l_Prescale)
					{									 		//刚好能除尽，则分频合适
						CAN4BTR0_BRP		= l_Prescale / l_TimSeg - 1;
																//设置预分频数
						CAN4BTR1_TSEG_20	= (UINT8)((l_TimSeg >= 18) ? 7 : (l_TimSeg / 2 - 2));
						CAN4BTR1_TSEG_10	= l_TimSeg  - CAN4BTR1_TSEG_20 - 3 ;
						return E_OK;										   
					}
				}
				
			}
			return E_FAIL;	//未进入初始化模式或没有合适的分频，设置频率失败
#endif

		default:
			return E_FAIL;
			
	}
}

////////////////////////////////////////////////////////////////////////////////
//  函数名：	MCU_CAN_SetSampling
//  作用描述：	设定CAN总线上每一位的采样次数
//  
//  返回值描述：E_OK	    设定成功	
//				E_FAIL      参数有误或未进入初始化模式，设定失败
//			
//  参数描述：	channel 		CAN的通道数
//			   	samplingMethod	枚举的CAN的采样次数
//  
//  修改历史：
//  版本        参与人员        日期            修改说明
//  --------    --------------  ------------    --------------------------------
//  1.0.0       黄文卿        	2009-07-11      创建函数
////////////////////////////////////////////////////////////////////////////////
ERROR MCU_CAN_SetSampling(UINT8 channel, MCU_CAN_SAMPLING samplingMethod)
{
	switch(channel)
	{
		case 0:
			if(CAN0CTL1_INITAK == 0x01)
			{
				switch(samplingMethod)
				{
					case CAN_LONG:
					
						CAN0BTR1_SAMP	= 0x01;			//每一位三个采样
						return E_OK;
						
					case CAN_SHORT:
					
						CAN0BTR1_SAMP	= 0x00;			//每一位一个采样
						return E_OK;
						
					default:
					
						return E_FAIL;
						
				} 
			}
			return E_FAIL;			//未进入初始化模式，设置失败
			
			
#if CANMaxChannels > 2			
		case 1:
			if(CAN1CTL1_INITAK == 0x01)
			{
				switch(samplingMethod)
				{
					case CAN_LONG:
					
						CAN1BTR1_SAMP	= 0x01;			//每一位三个采样
						return E_OK;
						
					case CAN_SHORT:
					
						CAN1BTR1_SAMP	= 0x00;			//每一位一个采样
						return E_OK;
						
					default:
					
						return E_FAIL;
						
				} 
			}
			return E_FAIL;			//未进入初始化模式，设置失败
#endif

#if CANMaxChannels > 3			
		case 2:
			if(CAN2CTL1_INITAK == 0x01)
			{
				switch(samplingMethod)
				{
					case CAN_LONG:
					
						CAN2BTR1_SAMP	= 0x01;			//每一位三个采样
						return E_OK;
						
					case CAN_SHORT:
					
						CAN2BTR1_SAMP	= 0x00;			//每一位一个采样
						return E_OK;
						
					default:
					
						return E_FAIL;
						
				} 
			}
			return E_FAIL;			//未进入初始化模式，设置失败
#endif

#if CANMaxChannels > 4			
		case 3:
			if(CAN3CTL1_INITAK == 0x01)
			{
				switch(samplingMethod)
				{
					case CAN_LONG:
					
						CAN3BTR1_SAMP	= 0x01;			//每一位三个采样
						return E_OK;
						
					case CAN_SHORT:
					
						CAN3BTR1_SAMP	= 0x00;			//每一位一个采样
						return E_OK;
						
					default:
					
						return E_FAIL;
						
				} 
			}
			return E_FAIL;			//未进入初始化模式，设置失败
#endif

#if CANMaxChannels > 1			
		case 4:
			if(CAN4CTL1_INITAK == 0x01)
			{
				switch(samplingMethod)
				{
					case CAN_LONG:
					
						CAN4BTR1_SAMP	= 0x01;			//每一位三个采样
						return E_OK;
						
					case CAN_SHORT:
					
						CAN4BTR1_SAMP	= 0x00;			//每一位一个采样
						return E_OK;
						
					default:
					
						return E_FAIL;
						
				} 
			}
			return E_FAIL;			//未进入初始化模式，设置失败
#endif

		default:
			return E_FAIL;
			
	}
}



////////////////////////////////////////////////////////////////////////////////
//  函数名：	MCU_CAN_SetIDAcceptMode
//  作用描述：	设定CAN模块接收时ID比对的模式
//  
//  返回值描述：E_OK	    设定成功	
//				E_FAIL      参数有误或未进入初始化模式，设定失败
//			
//  参数描述：	channel 	CAN的通道数
//			   	idMode		极举的CAN的接收ID比对的模式
//  
//  修改历史：
//  版本        参与人员        日期            修改说明
//  --------    --------------  ------------    --------------------------------
//  1.0.0       黄文卿        	2009-07-11      创建函数
////////////////////////////////////////////////////////////////////////////////
ERROR MCU_CAN_SetIDAcceptMode(UINT8 channel, MCU_CAN_ID_MODE idMode)
{
	if(idMode > 0x03)	//模式参数有误，设定失败
	{
		return E_FAIL;
	}
	
	switch(channel)
	{
		case 0:
		
			if(CAN0CTL1_INITAK == 0x01)		//确保进入初始化模式
			{
				CAN0IDAC_IDAM	= (UINT8)(idMode);
				return E_OK;
			}
			return E_FAIL;					//未进入初始化模式，设定失败
			
			
#if CANMaxChannels > 2			
		case 1:
			if(CAN1CTL1_INITAK == 0x01)		//确保进入初始化模式
			{
				CAN1IDAC_IDAM	= (UINT8)(idMode);
				return E_OK;
			}
			return E_FAIL;					//未进入初始化模式，设定失败
#endif

#if CANMaxChannels > 3			
		case 2:
			if(CAN2CTL1_INITAK == 0x01)		//确保进入初始化模式
			{
				CAN2IDAC_IDAM	= (UINT8)(idMode);
				return E_OK;
			}
			return E_FAIL;					//未进入初始化模式，设定失败
#endif

#if CANMaxChannels > 4			
		case 3:
			if(CAN3CTL1_INITAK == 0x01)		//确保进入初始化模式
			{
				CAN3IDAC_IDAM	= (UINT8)(idMode);
				return E_OK;
			}
			return E_FAIL;					//未进入初始化模式，设定失败
#endif

#if CANMaxChannels > 1			
		case 4:
			if(CAN4CTL1_INITAK == 0x01)		//确保进入初始化模式
			{
				CAN4IDAC_IDAM	= (UINT8)(idMode);
				return E_OK;
			}
			return E_FAIL;					//未进入初始化模式，设定失败
#endif

		default:
			return E_FAIL; 
	}
			
}




////////////////////////////////////////////////////////////////////////////////
//  函数名：	MCU_CAN_SetRXAcceptID
//  作用描述：	设定CAN模块接收时筛选的ID
//  
//  返回值描述：E_OK	    设定成功	
//				E_FAIL      参数有误或未进入初始化模式，设定失败
//			
//  参数描述：	channel 	CAN的通道数
//			   	idAcceptOne	第一组(即CANIDAR0~CANIDAR3)ID值
//				idAcceptTwo 第二组(即CANIDAR4~CANIDAR7)ID值	
//  
//  修改历史：
//  版本        参与人员        日期            修改说明
//  --------    --------------  ------------    --------------------------------
//  1.0.0       黄文卿        	2009-07-11      创建函数
////////////////////////////////////////////////////////////////////////////////
ERROR MCU_CAN_SetRXAcceptID(UINT8 channel, UINT32 idAcceptOne, UINT32 idAcceptTwo)
{
	switch(channel)
	{
		case 0:
			if(CAN0CTL1_INITAK == 0x01)
			{
			    CAN0IDAR0 = (UINT8)((idAcceptOne & 0xff000000) >> 24);
			    CAN0IDAR1 = (UINT8)((idAcceptOne & 0x00ff0000) >> 16);
			    CAN0IDAR2 = (UINT8)((idAcceptOne & 0x0000ff00) >> 8);
			    CAN0IDAR3 = (UINT8)((idAcceptOne & 0x000000ff));
			    
			    CAN0IDAR4 = (UINT8)((idAcceptTwo & 0xff000000) >> 24);
			    CAN0IDAR5 = (UINT8)((idAcceptTwo & 0x00ff0000) >> 16);
			    CAN0IDAR6 = (UINT8)((idAcceptTwo & 0x0000ff00) >> 8);
			    CAN0IDAR7 = (UINT8)((idAcceptTwo & 0x000000ff));
			    return E_OK;

			}
			return E_FAIL;
			
			
#if CANMaxChannels > 2			
		case 1:
			if(CAN1CTL1_INITAK == 0x01)
			{
			    CAN1IDAR0 = (UINT8)((idAcceptOne & 0xff000000) >> 24);
			    CAN1IDAR1 = (UINT8)((idAcceptOne & 0x00ff0000) >> 16);
			    CAN1IDAR2 = (UINT8)((idAcceptOne & 0x0000ff00) >> 8);
			    CAN1IDAR3 = (UINT8)((idAcceptOne & 0x000000ff));
			    
			    CAN1IDAR4 = (UINT8)((idAcceptTwo & 0xff000000) >> 24);
			    CAN1IDAR5 = (UINT8)((idAcceptTwo & 0x00ff0000) >> 16);
			    CAN1IDAR6 = (UINT8)((idAcceptTwo & 0x0000ff00) >> 8);
			    CAN1IDAR7 = (UINT8)((idAcceptTwo & 0x000000ff));
			    return E_OK;

			}
			return E_FAIL;
#endif

#if CANMaxChannels > 3			
		case 2:
			if(CAN2CTL1_INITAK == 0x01)
			{
			    CAN2IDAR0 = (UINT8)((idAcceptOne & 0xff000000) >> 24);
			    CAN2IDAR1 = (UINT8)((idAcceptOne & 0x00ff0000) >> 16);
			    CAN2IDAR2 = (UINT8)((idAcceptOne & 0x0000ff00) >> 8);
			    CAN2IDAR3 = (UINT8)((idAcceptOne & 0x000000ff));
			    
			    CAN2IDAR4 = (UINT8)((idAcceptTwo & 0xff000000) >> 24);
			    CAN2IDAR5 = (UINT8)((idAcceptTwo & 0x00ff0000) >> 16);
			    CAN2IDAR6 = (UINT8)((idAcceptTwo & 0x0000ff00) >> 8);
			    CAN2IDAR7 = (UINT8)((idAcceptTwo & 0x000000ff));
			    return E_OK;

			}
			return E_FAIL;
#endif

#if CANMaxChannels > 4			
		case 3:
			if(CAN3CTL1_INITAK == 0x01)
			{
			    CAN3IDAR0 = (UINT8)((idAcceptOne & 0xff000000) >> 24);
			    CAN3IDAR1 = (UINT8)((idAcceptOne & 0x00ff0000) >> 16);
			    CAN3IDAR2 = (UINT8)((idAcceptOne & 0x0000ff00) >> 8);
			    CAN3IDAR3 = (UINT8)((idAcceptOne & 0x000000ff));
			    
			    CAN3IDAR4 = (UINT8)((idAcceptTwo & 0xff000000) >> 24);
			    CAN3IDAR5 = (UINT8)((idAcceptTwo & 0x00ff0000) >> 16);
			    CAN3IDAR6 = (UINT8)((idAcceptTwo & 0x0000ff00) >> 8);
			    CAN3IDAR7 = (UINT8)((idAcceptTwo & 0x000000ff));
			    return E_OK;

			}
			return E_FAIL;
#endif

#if CANMaxChannels > 1			
		case 4:
			if(CAN4CTL1_INITAK == 0x01)
			{
			    CAN4IDAR0 = (UINT8)((idAcceptOne & 0xff000000) >> 24);
			    CAN4IDAR1 = (UINT8)((idAcceptOne & 0x00ff0000) >> 16);
			    CAN4IDAR2 = (UINT8)((idAcceptOne & 0x0000ff00) >> 8);
			    CAN4IDAR3 = (UINT8)((idAcceptOne & 0x000000ff));
			    
			    CAN4IDAR4 = (UINT8)((idAcceptTwo & 0xff000000) >> 24);
			    CAN4IDAR5 = (UINT8)((idAcceptTwo & 0x00ff0000) >> 16);
			    CAN4IDAR6 = (UINT8)((idAcceptTwo & 0x0000ff00) >> 8);
			    CAN4IDAR7 = (UINT8)((idAcceptTwo & 0x000000ff));
			    return E_OK;

			}
			return E_FAIL;
#endif

		default:
			return E_FAIL; 
	}
			
}


////////////////////////////////////////////////////////////////////////////////
//  函数名：	MCU_CAN_SetRxMask
//  作用描述：	设定CAN模块接收时筛选的ID掩码
//  
//  返回值描述：E_OK	    设定成功	
//				E_FAIL      参数有误或未进入初始化模式，设定失败
//			
//  参数描述：	channel 	CAN的通道数
//			   	maskOne		第一组(即CANIDMR0~CANIDMR3)掩码值
//				idAcceptTwo 第二组(即CANIDMR4~CANIDMR7)掩码值	
//  
//  修改历史：
//  版本        参与人员        日期            修改说明
//  --------    --------------  ------------    --------------------------------
//  1.0.0       黄文卿        	2009-07-11      创建函数
////////////////////////////////////////////////////////////////////////////////
ERROR MCU_CAN_SetRxMask(UINT8 channel, UINT32 maskOne, UINT32 maskTwo)
{
	switch(channel)
	{
		case 0:
			if(CAN0CTL1_INITAK == 0x01)
			{
			    CAN0IDMR0 = (UINT8)((maskOne & 0xff000000) >> 24);
			    CAN0IDMR1 = (UINT8)((maskOne & 0x00ff0000) >> 16);
			    CAN0IDMR2 = (UINT8)((maskOne & 0x0000ff00) >>  8);
			    CAN0IDMR3 = (UINT8)((maskOne & 0x000000ff));
			    
			    CAN0IDMR4 = (UINT8)((maskTwo & 0xff000000) >> 24);
			    CAN0IDMR5 = (UINT8)((maskTwo & 0x00ff0000) >> 16);
			    CAN0IDMR6 = (UINT8)((maskTwo & 0x0000ff00) >>  8);
			    CAN0IDMR7 = (UINT8)((maskTwo & 0x000000ff));
			    
				return E_OK;
			}
			return E_FAIL;
			
			
#if CANMaxChannels > 2			
		case 1:
			if(CAN1CTL1_INITAK == 0x01)
			{
			    CAN1IDMR0 = (UINT8)((maskOne & 0xff000000) >> 24);
			    CAN1IDMR1 = (UINT8)((maskOne & 0x00ff0000) >> 16);
			    CAN1IDMR2 = (UINT8)((maskOne & 0x0000ff00) >>  8);
			    CAN1IDMR3 = (UINT8)((maskOne & 0x000000ff));
			    
			    CAN1IDMR4 = (UINT8)((maskTwo & 0xff000000) >> 24);
			    CAN1IDMR5 = (UINT8)((maskTwo & 0x00ff0000) >> 16);
			    CAN1IDMR6 = (UINT8)((maskTwo & 0x0000ff00) >>  8);
			    CAN1IDMR7 = (UINT8)((maskTwo & 0x000000ff));
			    
				return E_OK;
			}
			return E_FAIL;
#endif

#if CANMaxChannels > 3			
		case 2:
			if(CAN2CTL1_INITAK == 0x01)
			{
			    CAN2IDMR0 = (UINT8)((maskOne & 0xff000000) >> 24);
			    CAN2IDMR1 = (UINT8)((maskOne & 0x00ff0000) >> 16);
			    CAN2IDMR2 = (UINT8)((maskOne & 0x0000ff00) >>  8);
			    CAN2IDMR3 = (UINT8)((maskOne & 0x000000ff));
			    
			    CAN2IDMR4 = (UINT8)((maskTwo & 0xff000000) >> 24);
			    CAN2IDMR5 = (UINT8)((maskTwo & 0x00ff0000) >> 16);
			    CAN2IDMR6 = (UINT8)((maskTwo & 0x0000ff00) >>  8);
			    CAN2IDMR7 = (UINT8)((maskTwo & 0x000000ff));
			    
				return E_OK;
			}
			return E_FAIL;
#endif

#if CANMaxChannels > 4			
		case 3:
			if(CAN3CTL1_INITAK == 0x01)
			{
			    CAN3IDMR0 = (UINT8)((maskOne & 0xff000000) >> 24);
			    CAN3IDMR1 = (UINT8)((maskOne & 0x00ff0000) >> 16);
			    CAN3IDMR2 = (UINT8)((maskOne & 0x0000ff00) >>  8);
			    CAN3IDMR3 = (UINT8)((maskOne & 0x000000ff));
			    
			    CAN3IDMR4 = (UINT8)((maskTwo & 0xff000000) >> 24);
			    CAN3IDMR5 = (UINT8)((maskTwo & 0x00ff0000) >> 16);
			    CAN3IDMR6 = (UINT8)((maskTwo & 0x0000ff00) >>  8);
			    CAN3IDMR7 = (UINT8)((maskTwo & 0x000000ff));
			    
				return E_OK;
			}
			return E_FAIL;
#endif

#if CANMaxChannels > 1			
		case 4:
			if(CAN4CTL1_INITAK == 0x01)
			{
			    CAN4IDMR0 = (UINT8)((maskOne & 0xff000000) >> 24);
			    CAN4IDMR1 = (UINT8)((maskOne & 0x00ff0000) >> 16);
			    CAN4IDMR2 = (UINT8)((maskOne & 0x0000ff00) >>  8);
			    CAN4IDMR3 = (UINT8)((maskOne & 0x000000ff));
			    
			    CAN4IDMR4 = (UINT8)((maskTwo & 0xff000000) >> 24);
			    CAN4IDMR5 = (UINT8)((maskTwo & 0x00ff0000) >> 16);
			    CAN4IDMR6 = (UINT8)((maskTwo & 0x0000ff00) >>  8);
			    CAN4IDMR7 = (UINT8)((maskTwo & 0x000000ff));
			    
				return E_OK;
			}
			return E_FAIL;
#endif

		default:
			return E_FAIL;
			
	}
}



////////////////////////////////////////////////////////////////////////////////
//  函数名：	MCU_CAN_CheckOutInitMode
//  作用描述：	设定CAN模块退出初始化模式
//  
//  返回值描述：E_OK		成功退出初始化状态，CAN模块正常运行
//				E_FAIL      超时，退出初始化状态失败
//  参数描述：	channel		通道
//			
//  
//  修改历史：
//  版本        参与人员        日期            修改说明
//  --------    --------------  ------------    --------------------------------
//  1.0.0       黄文卿        	2009-07-11      创建函数
////////////////////////////////////////////////////////////////////////////////
ERROR MCU_CAN_CheckOutInitMode(UINT8 channel)
{
	UINT16 l_CheckOutModeRetry;
	
	switch(channel)
	{
		case 0:
			CAN0CTL1_INITAK = 0;
			CAN0CTL0_INITRQ = 0;
			      
			for(l_CheckOutModeRetry = 0; l_CheckOutModeRetry < 60000; l_CheckOutModeRetry++)
			{
				
				if((CAN0CTL1_INITAK == 0x00) && (CAN0CTL0_INITRQ == 0))
				{
					CAN0RIER_RXFIE			= 0x01;		//允许接收中断，这得正常模式下执行
					return E_OK;					
				}
			}
			return E_FAIL;
			
#if CANMaxChannels > 2			
		case 1:
			CAN1CTL1_INITAK = 0;
			CAN1CTL0_INITRQ = 0;
			      
			for(l_CheckOutModeRetry = 0; l_CheckOutModeRetry < 60000; l_CheckOutModeRetry++)
			{
				
				if((CAN1CTL1_INITAK == 0x00) && (CAN1CTL0_INITRQ == 0))
				{
					CAN1RIER_RXFIE			= 0x01;		//允许接收中断，这得正常模式下执行
					return E_OK;					
				}
			}
			return E_FAIL;
#endif

#if CANMaxChannels > 3			
		case 2:
			CAN2CTL1_INITAK = 0;
			CAN2CTL0_INITRQ = 0;
			      
			for(l_CheckOutModeRetry = 0; l_CheckOutModeRetry < 60000; l_CheckOutModeRetry++)
			{
				
				if((CAN2CTL1_INITAK == 0x00) && (CAN2CTL0_INITRQ == 0))
				{
					CAN2RIER_RXFIE			= 0x01;		//允许接收中断，这得正常模式下执行
					return E_OK;					
				}
			}
			return E_FAIL;
#endif

#if CANMaxChannels > 4			
		case 3:
			CAN3CTL1_INITAK = 0;
			CAN3CTL0_INITRQ = 0;
			      
			for(l_CheckOutModeRetry = 0; l_CheckOutModeRetry < 60000; l_CheckOutModeRetry++)
			{
				
				if((CAN3CTL1_INITAK == 0x00) && (CAN3CTL0_INITRQ == 0))
				{
					CAN3RIER_RXFIE			= 0x01;		//允许接收中断，这得正常模式下执行
					return E_OK;					
				}
			}
			return E_FAIL;
#endif

#if CANMaxChannels > 1			
		case 4:
			CAN4CTL1_INITAK = 0;
			CAN4CTL0_INITRQ = 0;
			      
			for(l_CheckOutModeRetry = 0; l_CheckOutModeRetry < 60000; l_CheckOutModeRetry++)
			{
				
				if((CAN4CTL1_INITAK == 0x00) && (CAN4CTL0_INITRQ == 0))
				{
					CAN4RIER_RXFIE			= 0x01;		//允许接收中断，这得正常模式下执行
					return E_OK;					
				}
			}
			return E_FAIL;
#endif

		default:
			return E_FAIL;
			
	}
	
}




////////////////////////////////////////////////////////////////////////////////
//  函数名：	MCU_CAN_Transmit
//  作用描述：	发送CAN数据
//  
//  返回值描述：E_OK	    发送成功或者是成功存入发送缓冲区，等待接下来发送	
//				E_FULL		缓冲区已满，发送失败
//			
//  参数描述：	channel 	CAN的通道数
//			   	msgSend		待发送的数据
//  
//  修改历史：
//  版本        参与人员        日期            修改说明
//  --------    --------------  ------------    --------------------------------
//  1.0.0       黄文卿        	2009-07-11      创建函数
////////////////////////////////////////////////////////////////////////////////
ERROR MCU_CAN_Transmit(UINT8 channel, MCU_CAN_MSG msgSend)
{
	UINT8 	l_ChoseBuff;
	UINT8 	l_TxIndex;
	switch(channel)
	{
		case 0:
			if((CAN0TFLG_TXE & 0x03) == 0)
			{
				return E_FULL;
			}
	    	CAN0TBSEL	= CAN0TFLG;     
		    l_ChoseBuff = CAN0TBSEL;
		    
			CAN0TXIDR0	= (UINT8)((msgSend.ID & 0xff000000)>>24);    //报文标示符区
			CAN0TXIDR1	= (UINT8)((msgSend.ID & 0x00ff0000)>>16);    
			CAN0TXIDR2	= (UINT8)((msgSend.ID & 0x0000ff00)>>8); 
			CAN0TXIDR3	= (UINT8)((msgSend.ID & 0x000000ff)); 
		
			if(msgSend.DataLength != 0)
			{
				for(l_TxIndex = 0; l_TxIndex < msgSend.DataLength; l_TxIndex++)
				{
					 *((&CAN0TXDSR0) + l_TxIndex) = msgSend.Data[l_TxIndex];
				}
			}
			
		    CAN0TXDLR	= msgSend.DataLength;        //报文长度，定义数据区为8个数据字节
		    CAN0TFLG	= l_ChoseBuff; 
			return E_OK;
			
						
#if CANMaxChannels > 2			
		case 1:
			if((CAN1TFLG_TXE & 0x03) == 0)
			{
				return E_FULL;
			}
	    	CAN1TBSEL	= CAN1TFLG;     
		    l_ChoseBuff = CAN1TBSEL;
		    
			CAN1TXIDR0	= (UINT8)((msgSend.ID & 0xff000000)>>24);    //报文标示符区
			CAN1TXIDR1	= (UINT8)((msgSend.ID & 0x00ff0000)>>16);    
			CAN1TXIDR2	= (UINT8)((msgSend.ID & 0x0000ff00)>>8); 
			CAN1TXIDR3	= (UINT8)((msgSend.ID & 0x000000ff)); 
		
			if(msgSend.DataLength != 0)
			{
				for(l_TxIndex = 0; l_TxIndex < msgSend.DataLength; l_TxIndex++)
				{
					 *((&CAN1TXDSR0) + l_TxIndex) = msgSend.Data[l_TxIndex];
				}
			}
			
		    CAN1TXDLR	= msgSend.DataLength;        //报文长度，定义数据区为8个数据字节
		    CAN1TFLG	= l_ChoseBuff; 
			return E_OK;
#endif

#if CANMaxChannels > 3			
		case 2:
			if((CAN2TFLG_TXE & 0x03) == 0)
			{
				return E_FULL;
			}
	    	CAN2TBSEL	= CAN2TFLG;     
		    l_ChoseBuff = CAN2TBSEL;
		    
			CAN2TXIDR0	= (UINT8)((msgSend.ID & 0xff000000)>>24);    //报文标示符区
			CAN2TXIDR1	= (UINT8)((msgSend.ID & 0x00ff0000)>>16);    
			CAN2TXIDR2	= (UINT8)((msgSend.ID & 0x0000ff00)>>8); 
			CAN2TXIDR3	= (UINT8)((msgSend.ID & 0x000000ff)); 
		
			if(msgSend.DataLength != 0)
			{
				for(l_TxIndex = 0; l_TxIndex < msgSend.DataLength; l_TxIndex++)
				{
					 *((&CAN2TXDSR0) + l_TxIndex) = msgSend.Data[l_TxIndex];
				}
			}
			
		    CAN2TXDLR	= msgSend.DataLength;        //报文长度，定义数据区为8个数据字节
		    CAN2TFLG	= l_ChoseBuff; 
			return E_OK;
#endif

#if CANMaxChannels > 4			
		case 3:
			if((CAN3TFLG_TXE & 0x03) == 0)
			{
				return E_FULL;
			}
	    	CAN3TBSEL	= CAN3TFLG;     
		    l_ChoseBuff = CAN3TBSEL;
		    
			CAN3TXIDR0	= (UINT8)((msgSend.ID & 0xff000000)>>24);    //报文标示符区
			CAN3TXIDR1	= (UINT8)((msgSend.ID & 0x00ff0000)>>16);    
			CAN3TXIDR2	= (UINT8)((msgSend.ID & 0x0000ff00)>>8); 
			CAN3TXIDR3	= (UINT8)((msgSend.ID & 0x000000ff)); 
		
			if(msgSend.DataLength != 0)
			{
				for(l_TxIndex = 0; l_TxIndex < msgSend.DataLength; l_TxIndex++)
				{
					 *((&CAN3TXDSR0) + l_TxIndex) = msgSend.Data[l_TxIndex];
				}
			}
			
		    CAN3TXDLR	= msgSend.DataLength;        //报文长度，定义数据区为8个数据字节
		    CAN3TFLG	= l_ChoseBuff; 
			return E_OK;
#endif

#if CANMaxChannels > 1			
		case 4:
			if((CAN4TFLG_TXE & 0x03) == 0)
			{
				return E_FULL;
			}
	    	CAN4TBSEL	= CAN4TFLG;     
		    l_ChoseBuff = CAN4TBSEL;
		    
			CAN4TXIDR0	= (UINT8)((msgSend.ID & 0x1fE00000)>>21);    //报文标示符区
			CAN4TXIDR1	= ((UINT8)(((msgSend.ID & 0x001C0000)>>18)<<5))|(0x18)|((uint8)((msgSend.ID & 0x00038000)>>15));    
			CAN4TXIDR2	= (UINT8)((msgSend.ID & 0x00007f80)>>7); 
			CAN4TXIDR3	= (UINT8)((msgSend.ID & 0x0000007f)<<1); 
		
			if(msgSend.DataLength != 0)
			{
				for(l_TxIndex = 0; l_TxIndex < msgSend.DataLength; l_TxIndex++)
				{
					 *((&CAN4TXDSR0) + l_TxIndex) = msgSend.Data[l_TxIndex];
				}
			}
			
		    CAN4TXDLR	= msgSend.DataLength;        //报文长度，定义数据区为8个数据字节
		    CAN4TFLG	= l_ChoseBuff; 
			return E_OK;
#endif

		default:
			return E_FAIL;
			
	}
}





////////////////////////////////////////////////////////////////////////////////
//  函数名：	MCU_CAN_Receive
//  作用描述：	读取接收到的数据
//  
//  返回值描述：E_OK	    接收成功	
//				E_FAIL      通道数有误，读取失败
//				E_EMPTY		没有接收到数据，接收缓冲区为空
//  参数描述：
//			
//  
//  修改历史：
//  版本        参与人员        日期            修改说明
//  --------    --------------  ------------    --------------------------------
//  1.0.0       黄文卿        	2009-07-11      创建函数
////////////////////////////////////////////////////////////////////////////////
ERROR MCU_CAN_Receive(UINT8 channel, MCU_CAN_MSG * msgRec)
{
    return MCU_CAN_GetRxData(channel, msgRec);	//
}



////////////////////////////////////////////////////////////////////////////////
//  函数名：	MCU_CAN_IsReceived
//  作用描述：	判断CAN模块有无接收到数据
//  
//  返回值描述：E_OK		接收缓冲区有接收到的数据
//				E_FAIL      没有接到数据或通数有误
//  参数描述： 	channel		通道
//			
//  
//  修改历史：
//  版本        参与人员        日期            修改说明
//  --------    --------------  ------------    --------------------------------
//  1.0.0       黄文卿        	2009-07-11      创建函数
////////////////////////////////////////////////////////////////////////////////
ERROR MCU_CAN_IsReceived(UINT8 channel)
{
	if(c_MCU_CAN_ChannelMapping[channel] >= CANMaxChannels)
	{
		return E_FAIL;
	}

    if((g_MCU_CAN_RXFull[c_MCU_CAN_ChannelMapping[channel]] == FALSE) && (g_MCU_CAN_RXFront[c_MCU_CAN_ChannelMapping[channel]] == g_MCU_CAN_RXRear[c_MCU_CAN_ChannelMapping[channel]]))
	{
	    return E_FAIL;                              //队列不满，且头尾相等，则没有数据接收到
	}
	else
	{
	    return E_OK;                                //接收到数据
	};	
	
}









////////////////////////////////////////////////////////////////////////////////
//  函数名：	MCU_CAN_AddRxData
//  作用描述：	将CAN数据存入接收缓冲区
//  
//  返回值描述：E_OK	    成功存入缓冲区
//				E_FAIL      参数有误，存入失败
//				E_FULL		缓冲区已满，存入失败
//			
//  参数描述：	channel 	CAN的通道数
//			   	data		待存入的数据
//  
//  修改历史：
//  版本        参与人员        日期            修改说明
//  --------    --------------  ------------    --------------------------------
//  1.0.0       黄文卿        	2009-07-11      创建函数
////////////////////////////////////////////////////////////////////////////////

ERROR MCU_CAN_AddRxData(UINT8 channel, MCU_CAN_MSG data)
{
	if(c_MCU_CAN_ChannelMapping[channel] >= CANMaxChannels)
	{
		return E_FAIL;
	}
	if(g_MCU_CAN_RXFull[c_MCU_CAN_ChannelMapping[channel]] == TRUE)
	{
        return E_FULL;                                  //队列已满
	}
    g_MCU_CAN_RXBuff[c_MCU_CAN_ChannelMapping[channel]][g_MCU_CAN_RXRear[c_MCU_CAN_ChannelMapping[channel]]] = data;   //数据入列
    
    g_MCU_CAN_RXRear[c_MCU_CAN_ChannelMapping[channel]]++;
    if(g_MCU_CAN_RXRear[c_MCU_CAN_ChannelMapping[channel]] >= CANBuffMaxLength)
    {
     	g_MCU_CAN_RXRear[c_MCU_CAN_ChannelMapping[channel]] = 0;
    }

    if(g_MCU_CAN_RXRear[c_MCU_CAN_ChannelMapping[channel]] == g_MCU_CAN_RXFront[c_MCU_CAN_ChannelMapping[channel]])
    {
        g_MCU_CAN_RXFull[c_MCU_CAN_ChannelMapping[channel]] = TRUE;                     //队列满
    };
    
    return E_OK;
}

 


////////////////////////////////////////////////////////////////////////////////
//  函数名：	MCU_CAN_GetRxData
//  作用描述：	读取接收到的数据
//  
//  返回值描述：E_OK	    接收成功	
//				E_FAIL      通道数有误，读取失败
//				E_EMPTY		没有接收到数据，接收缓冲区为空
//  参数描述：	channel		CAN通道数
//			  	data		指向存放取出的数据的指针	
//  
//  修改历史：
//  版本        参与人员        日期            修改说明
//  --------    --------------  ------------    --------------------------------
//  1.0.0       黄文卿        	2009-07-11      创建函数
////////////////////////////////////////////////////////////////////////////////

ERROR MCU_CAN_GetRxData(UINT8 channel, MCU_CAN_MSG * data)
{
	if(c_MCU_CAN_ChannelMapping[channel] >= CANMaxChannels)
	{
		return E_FAIL;
	}
	if((g_MCU_CAN_RXFull[c_MCU_CAN_ChannelMapping[channel]] == FALSE) && (g_MCU_CAN_RXRear[c_MCU_CAN_ChannelMapping[channel]] == g_MCU_CAN_RXFront[c_MCU_CAN_ChannelMapping[channel]]))
	{
        return E_EMPTY;                                  //队列已空
	}
	*data = g_MCU_CAN_RXBuff[c_MCU_CAN_ChannelMapping[channel]][g_MCU_CAN_RXFront[c_MCU_CAN_ChannelMapping[channel]]];//出列队
    
    g_MCU_CAN_RXFront[c_MCU_CAN_ChannelMapping[channel]]++;
    if(g_MCU_CAN_RXFront[c_MCU_CAN_ChannelMapping[channel]] >= CANBuffMaxLength)
    {
     	g_MCU_CAN_RXFront[c_MCU_CAN_ChannelMapping[channel]] = 0;
    }

    g_MCU_CAN_RXFull[c_MCU_CAN_ChannelMapping[channel]] = FALSE;                     //队列未满
    
    return E_OK;
}
 
/*
////////////////////////////////////////////////////////////////////////////////
//  函数名：	MCU_CAN_Ch0RXInt
//  作用描述：	CAN0的接收中断
//  
//  返回值描述：无	
//			
//  参数描述：	无
//			  	
//  
//  修改历史：
//  版本        参与人员        日期            修改说明
//  --------    --------------  ------------    --------------------------------
//  1.0.0       黄文卿        	2009-07-11      创建函数
//  1.0.1       简耀佳          2011-03-30      增加ID获取
////////////////////////////////////////////////////////////////////////////////
#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt VectorNumber_Vcan0rx void MCU_CAN_Ch0RXInt(void)
{
	MCU_CAN_MSG l_RxData={{0,0,0,0,0,0,0,0},0,0};
	UINT8		l_RxIndex;
	l_RxData.DataLength	= CAN0RXDLR_DLC; //清标志位前读取数据，否则数据会出错
	
	l_RxData.ID |= (UINT32)CAN0RXIDR0<<24;
	l_RxData.ID |= (UINT32)CAN0RXIDR1<<16;
	l_RxData.ID |= (UINT32)CAN0RXIDR2<<8;
	l_RxData.ID |= (UINT32)CAN0RXIDR3;
	for(l_RxIndex = 0; l_RxIndex < l_RxData.DataLength; l_RxIndex++)
	{
		l_RxData.Data[l_RxIndex]	= *((&CAN0RXDSR0) + l_RxIndex);	
	}
	(void)MCU_CAN_AddRxData(0,l_RxData);
    CAN0RFLG    = 0x01;
}
#pragma CODE_SEG DEFAULT
 */



#if CANMaxChannels > 2			
////////////////////////////////////////////////////////////////////////////////
//  函数名：	MCU_CAN_Ch1RXInt
//  作用描述：	CAN1的接收中断
//  
//  返回值描述：无	
//			
//  参数描述：	无
//			  	
//  
//  修改历史：
//  版本        参与人员        日期            修改说明
//  --------    --------------  ------------    --------------------------------
//  1.0.0       黄文卿        	2009-07-11      创建函数
//  1.0.1       简耀佳          2011-03-30      增加ID获取
////////////////////////////////////////////////////////////////////////////////
#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt VectorNumber_Vcan1rx void MCU_CAN_Ch1RXInt(void)
{
	MCU_CAN_MSG l_RxData={{0,0,0,0,0,0,0,0},0,0};
	UINT8		l_RxIndex;
	l_RxData.DataLength	= CAN1RXDLR_DLC; //清标志位前读取数据，否则数据会出错
	
	l_RxData.ID |= (UINT32)CAN1RXIDR0<<24;
	l_RxData.ID |= (UINT32)CAN1RXIDR1<<16;
	l_RxData.ID |= (UINT32)CAN1RXIDR2<<8;
	l_RxData.ID |= (UINT32)CAN1RXIDR3;
	for(l_RxIndex = 0; l_RxIndex < l_RxData.DataLength; l_RxIndex++)
	{
		l_RxData.Data[l_RxIndex]	= *((&CAN1RXDSR0) + l_RxIndex);	
	}
	(void)MCU_CAN_AddRxData(1,l_RxData);
    CAN1RFLG    = 0x01;
}
#pragma CODE_SEG DEFAULT
#endif

#if CANMaxChannels > 3			
////////////////////////////////////////////////////////////////////////////////
//  函数名：	MCU_CAN_Ch2RXInt
//  作用描述：	CAN2的接收中断
//  
//  返回值描述：无	
//			
//  参数描述：	无
//			  	
//  
//  修改历史：
//  版本        参与人员        日期            修改说明
//  --------    --------------  ------------    --------------------------------
//  1.0.0       黄文卿        	2009-07-11      创建函数
//  1.0.1       简耀佳          2011-03-30      增加ID获取
////////////////////////////////////////////////////////////////////////////////
#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt VectorNumber_Vcan2rx void MCU_CAN_Ch2RXInt(void)
{
	MCU_CAN_MSG l_RxData={{0,0,0,0,0,0,0,0},0,0};
	UINT8		l_RxIndex;
	l_RxData.DataLength	= CAN2RXDLR_DLC; //清标志位前读取数据，否则数据会出错
	
	l_RxData.ID |= (UINT32)CAN2RXIDR0<<24;
	l_RxData.ID |= (UINT32)CAN2RXIDR1<<16;
	l_RxData.ID |= (UINT32)CAN2RXIDR2<<8;
	l_RxData.ID |= (UINT32)CAN2RXIDR3;
	for(l_RxIndex = 0; l_RxIndex < l_RxData.DataLength; l_RxIndex++)
	{
		l_RxData.Data[l_RxIndex]	= *((&CAN2RXDSR0) + l_RxIndex);	
	}
	(void)MCU_CAN_AddRxData(2,l_RxData);
    CAN2RFLG    = 0x01;
}
#pragma CODE_SEG DEFAULT
#endif


#if CANMaxChannels > 4			
////////////////////////////////////////////////////////////////////////////////
//  函数名：	MCU_CAN_Ch3RXInt
//  作用描述：	CAN3的接收中断
//  
//  返回值描述：无	
//			
//  参数描述：	无
//			  	
//  
//  修改历史：
//  版本        参与人员        日期            修改说明
//  --------    --------------  ------------    --------------------------------
//  1.0.0       黄文卿        	2009-07-11      创建函数
//  1.0.1       简耀佳          2011-03-30      增加ID获取
////////////////////////////////////////////////////////////////////////////////
#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt VectorNumber_Vcan3rx void MCU_CAN_Ch3RXInt(void)
{
	MCU_CAN_MSG l_RxData={{0,0,0,0,0,0,0,0},0,0};
	UINT8		l_RxIndex;
	l_RxData.DataLength	= CAN3RXDLR_DLC; //清标志位前读取数据，否则数据会出错
	
	l_RxData.ID |= (UINT32)CAN3RXIDR0<<24;
	l_RxData.ID |= (UINT32)CAN3RXIDR1<<16;
	l_RxData.ID |= (UINT32)CAN3RXIDR2<<8;
	l_RxData.ID |= (UINT32)CAN3RXIDR3;
	for(l_RxIndex = 0; l_RxIndex < l_RxData.DataLength; l_RxIndex++)
	{
		l_RxData.Data[l_RxIndex]	= *((&CAN3RXDSR0) + l_RxIndex);	
	}
	(void)MCU_CAN_AddRxData(3,l_RxData);
    CAN3RFLG    = 0x01;
}
#pragma CODE_SEG DEFAULT
#endif



#if CANMaxChannels > 1			
////////////////////////////////////////////////////////////////////////////////
//  函数名：	MCU_CAN_Ch4RXInt
//  作用描述：	CAN4的接收中断
//  
//  返回值描述：无	
//			
//  参数描述：	无
//			  	
//  
//  修改历史：
//  版本        参与人员        日期            修改说明
//  --------    --------------  ------------    --------------------------------
//  1.0.0       黄文卿        	2009-07-11      创建函数
//  1.0.1       简耀佳          2011-03-30      增加ID获取
////////////////////////////////////////////////////////////////////////////////
#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt VectorNumber_Vcan4rx void MCU_CAN_Ch4RXInt(void)
{
	MCU_CAN_MSG l_RxData={{0,0,0,0,0,0,0,0},0,0};
	UINT8		l_RxIndex;
	l_RxData.DataLength	= CAN4RXDLR_DLC; //清标志位前读取数据，否则数据会出错
	
	l_RxData.ID |= (UINT32)CAN4RXIDR0<<24;
	l_RxData.ID |= (UINT32)CAN4RXIDR1<<16;
	l_RxData.ID |= (UINT32)CAN4RXIDR2<<8;
	l_RxData.ID |= (UINT32)CAN4RXIDR3;
	for(l_RxIndex = 0; l_RxIndex < l_RxData.DataLength; l_RxIndex++)
	{
		l_RxData.Data[l_RxIndex]	= *((&CAN4RXDSR0) + l_RxIndex);	
	}
	(void)MCU_CAN_AddRxData(4,l_RxData);
    CAN4RFLG    = 0x01;
}
#pragma CODE_SEG DEFAULT
#endif