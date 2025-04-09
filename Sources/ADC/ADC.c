////////////////////////////////////////////////////////////////////////////////
//  文件名：    ADC.c
//  作用描述：  模数转换模块的实现文件
//  编译：      CodeWarrior for HCS12(X) V4.7 
//  
//  修改历史：
//  版本        参与人员        日期            修改说明
//  --------    --------------  ------------    --------------------------------
//  1.0.0       黄文卿          2009-08-06      创建文件
////////////////////////////////////////////////////////////////////////////////
#include "ADC.h"
#include "Platform_Types.h"

static uint8 ADC_ChannelMapping[16];
//ADC模块的通道和结果寄存器不是一一对应的。转换序列起始通道放在ATD0DR0上，
//后面以此类推，在此为方便，将其用一映射替换

//ADC_Cfg.c中的配置变量
extern const ADC_SamplingTimeType	ADC_SamplingTime;
extern const ADC_PrescaleType		ADC_Prescale;

////////////////////////////////////////////////////////////////////////////////
//  函数名：	ADC_DeInit
//  作用描述：	初始化ADC模块的寄存器至复位状态
//  
//  返回值描述：void
//				
//  参数描述：	void	
//  
//  修改历史：
//  版本        参与人员        日期            修改说明
//  --------    --------------  ------------    --------------------------------
//  1.0.0       黄文卿        	2009-08-06      创建函数
////////////////////////////////////////////////////////////////////////////////
void ADC_DeInit(void)
{
	ATD0CTL0	= 0x0F;
	ATD0CTL1	= 0x2F;
	ATD0CTL2	= 0x00;
	ATD0CTL3	= 0x20;
	ATD0CTL4	= 0x05;
	ATD0CTL5	= 0x00;
	ATD0STAT0	= 0xB0;
	ATD0CMPE	= 0x0000;
	ATD0DIEN	= 0x0000;
	ATD0CMPHT	= 0x0000;
	
	ATD0DR0		= 0x0000;
	ATD0DR1		= 0x0000;
	ATD0DR2		= 0x0000;
	ATD0DR3		= 0x0000;
	ATD0DR4		= 0x0000;
	ATD0DR5		= 0x0000;
	ATD0DR6		= 0x0000;
	ATD0DR7		= 0x0000;
	ATD0DR8		= 0x0000;
	ATD0DR9		= 0x0000;
	ATD0DR10	= 0x0000;
	ATD0DR11	= 0x0000;
	ATD0DR12	= 0x0000;
	ATD0DR13	= 0x0000;
	ATD0DR14	= 0x0000;
	ATD0DR15	= 0x0000;
}

////////////////////////////////////////////////////////////////////////////////
//  函数名：	ADC_Init
//  作用描述：	初始化ADC模块
//  
//  返回值描述：void
//				
//  参数描述：	void	
//  
//  修改历史：
//  版本        参与人员        日期            修改说明
//  --------    --------------  ------------    --------------------------------
//  1.0.0       黄文卿        	2009-08-06      创建函数
////////////////////////////////////////////////////////////////////////////////
void ADC_Init(void)
{
    ATD0CTL1_SRES		= ADC_Resolution;	//采样位数
	ATD0CTL1_SMP_DIS	= 0x01;		//采样之前内部采样电容先放电
    
	ATD0CTL2_AFFC		= 0x01;		//标志位快速清零
	ATD0CTL2_ICLKSTP	= 0x01;		//STP模式下AD继续转换
	ATD0CTL2_ETRIGE		= 0x00;		//禁止外部触发模式
	ATD0CTL2_ASCIE		= 0x00;		//禁止转换完成中断
	ATD0CTL2_ACMPIE		= 0x00;		//禁止比较中断
	
	ATD0CTL3_DJM		= 0x01;		//转换结果右对齐
	ATD0CTL3_FIFO		= 0x00;		//转换结果存放到相应的结果寄存器，不循环放置
	ATD0CTL3_FRZ		= 0x00;		//FRZ模式下继续转换

	ATD0CTL4_SMP		= ADC_SamplingTime;		//采样时间
	ATD0CTL4_PRS		= (ADC_Prescale / 2) - 1; //ADC时钟分频
	
}

////////////////////////////////////////////////////////////////////////////////
//  函数名：	ADC_StartChannelConversion
//  作用描述：	启动ADC模块的相应通道的转换
//  
//  返回值描述：void
//				
//  参数描述：	
//				firstChannel	转换序列的第一个通道
//				lastChannel		转换序列的最后一个通道	
//  
//  修改历史：
//  版本        参与人员        日期            修改说明
//  --------    --------------  ------------    --------------------------------
//  1.0.0       黄文卿        	2009-08-06      创建函数
////////////////////////////////////////////////////////////////////////////////
void ADC_StartChannelConversion(ADC_ChannelType firstChannel, ADC_ChannelType lastChannel)
{
	uint8 l_SeqLength;
	uint8 l_Channel;
	
	if((lastChannel > 15) || (firstChannel > 15))	//XS中通道数不能超过15
	{
		return;
	}
	
	for(l_Channel = 0; l_Channel < 16; l_Channel++)
	{
		ADC_ChannelMapping[l_Channel] = 0xff;		//初始化通道映射	
	}
	
	l_SeqLength = (lastChannel + 17 - firstChannel) & 0x0f;//总的转换序列的长度
													//如果长度为16,则赋为0
	
	
    ATD0CTL3_S1C	= l_SeqLength & 0x01;			//序列长度，将其转化为二进制赋值
    ATD0CTL3_S2C	= (l_SeqLength >> 1) & 0x01;	
	ATD0CTL3_S4C	= (l_SeqLength >> 2) & 0x01;	
	ATD0CTL3_S8C	= (l_SeqLength >> 3) & 0x01;
	

	ATD0CTL5_SC		= 0x00;			//禁止特殊转换
	ATD0CTL5_SCAN	= 0x01;			//连续转换
	if(l_SeqLength == 1)
	{
		ATD0CTL5_MULT	= 0x00;			//单通道转换
	}
	else
	{
		ATD0CTL5_MULT	= 0x01;			//多通道转换
	}
	ATD0CTL5_Cx		= firstChannel;		//从firstChannel通道开始转换

	if(l_SeqLength == 0)	//如果长度为0,实际为16个转换通道
	{
		l_SeqLength = 16;
	}
	for(l_Channel = 0; l_Channel < l_SeqLength; l_Channel++)
	{
		ADC_ChannelMapping[(firstChannel + l_Channel) & 0x0f] = l_Channel;	
	}
}

////////////////////////////////////////////////////////////////////////////////
//  函数名：	ADC_GetChannelResult
//  作用描述：	获取相应通道的AD转换结果
//  
//  返回值描述：相应通道的转换结果
//				
//  参数描述：	
//				channel			通道数
//  
//  修改历史：
//  版本        参与人员        日期            修改说明
//  --------    --------------  ------------    --------------------------------
//  1.0.0       黄文卿        	2009-08-06      创建函数
////////////////////////////////////////////////////////////////////////////////
ADC_ResultType ADC_GetChannelResult(ADC_ChannelType channel)
{
	uint8 l_ADCRetry;
	for(l_ADCRetry = 0; l_ADCRetry < 250; l_ADCRetry++)
	{
		if(1 == ATD0STAT0_SCF)	//判断一个序列是否转换完毕
		{
			break;
		}
	}
	
	if(l_ADCRetry >= 250)		//超时，返回0
	{
		return 0;
	}
	
	switch(ADC_ChannelMapping[channel])		//获取相应通道的结果
	{
		case 0:
			return ATD0DR0;
			
		case 1:
			return ATD0DR1;
			
		case 2:
			return ATD0DR2;
			
		case 3:
			return ATD0DR3;
			
		case 4:
			return ATD0DR4;
			
		case 5:
			return ATD0DR5;
			
		case 6:
			return ATD0DR6;
			
		case 7:
			return ATD0DR7;
			
		case 8:
			return ATD0DR8;
			
		case 9:
			return ATD0DR9;
			
		case 10:
			return ATD0DR10;
			
		case 11:
			return ATD0DR11;
			
		case 12:
			return ATD0DR12;
			
		case 13:
			return ATD0DR13;
			
		case 14:
			return ATD0DR14;
			
		case 15:
			return ATD0DR15;
			
		default:
			return 0;			//XS中通道数不能超过15
			
	}
}

////////////////////////////////////////////////////////////////////////////////
//  函数名：	ADC_GetVltg
//  作用描述：	获取相应通道电压值(将转换结果转化为电压值)
//  
//  返回值描述：相应通道的电压值
//				
//  参数描述：	
//				channel			通道数
//  
//  修改历史：
//  版本        参与人员        日期            修改说明
//  --------    --------------  ------------    --------------------------------
//  1.0.0       黄文卿        	2009-08-06      创建函数
////////////////////////////////////////////////////////////////////////////////
ADC_ResultType ADC_GetVltg(ADC_ChannelType channel)
{
	switch(ADC_ChannelMapping[channel])		//获取相应通道的结果，并转成电压值
	{
		case 0:
			return (uint16)((uint32)ATD0DR0 * ADC_RefVoltage / ADC_MaxValue);
			
		case 1:
			return (uint16)((uint32)ATD0DR1 * ADC_RefVoltage / ADC_MaxValue);
			
		case 2:
			return (uint16)((uint32)ATD0DR2 * ADC_RefVoltage / ADC_MaxValue);
			
		case 3:
			return (uint16)((uint32)ATD0DR3 * ADC_RefVoltage / ADC_MaxValue);
			
		case 4:
			return (uint16)((uint32)ATD0DR4 * ADC_RefVoltage / ADC_MaxValue);
			
		case 5:
			return (uint16)((uint32)ATD0DR5 * ADC_RefVoltage / ADC_MaxValue);
			
		case 6:
			return (uint16)((uint32)ATD0DR6 * ADC_RefVoltage / ADC_MaxValue);
			
		case 7:
			return (uint16)((uint32)ATD0DR7 * ADC_RefVoltage / ADC_MaxValue);
			
		case 8:
			return (uint16)((uint32)ATD0DR8 * ADC_RefVoltage / ADC_MaxValue);
			
		case 9:
			return (uint16)((uint32)ATD0DR9 * ADC_RefVoltage / ADC_MaxValue);
			
		case 10:
			return (uint16)((uint32)ATD0DR10 * ADC_RefVoltage / ADC_MaxValue);
			
		case 11:
			return (uint16)((uint32)ATD0DR11 * ADC_RefVoltage / ADC_MaxValue);
			
		case 12:
			return (uint16)((uint32)ATD0DR12 * ADC_RefVoltage / ADC_MaxValue);
			
		case 13:
			return (uint16)((uint32)ATD0DR13 * ADC_RefVoltage / ADC_MaxValue);
			
		case 14:
			return (uint16)((uint32)ATD0DR14 * ADC_RefVoltage / ADC_MaxValue);
			
		case 15:
			return (uint16)((uint32)ATD0DR15 * ADC_RefVoltage / ADC_MaxValue);
		
		default:
			return 0;
			
	}
}