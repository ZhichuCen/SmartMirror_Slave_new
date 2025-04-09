////////////////////////////////////////////////////////////////////////////////
//  文件名：    ADC.h
//  作用描述：  模数转换模块的函数等的声明文件
//  编译：      CodeWarrior for HCS12(X) V4.7 
//  
//  修改历史：
//  版本        参与人员        日期            修改说明
//  --------    --------------  ------------    --------------------------------
//  1.0.0       黄文卿          2009-08-06      创建文件
////////////////////////////////////////////////////////////////////////////////
#ifndef _ADC_H_
#define _ADC_H_


#include "ADC_Cfg.h"
#include "Platform_Types.h"


typedef uint8 ADC_ResolutionType;		//转换位数类型

typedef uint8 ADC_SamplingTimeType;		//采样时间类型
typedef uint8 ADC_PrescaleType;			//AD时钟预分频类型 

typedef uint8 	ADC_HwUnitType;
typedef uint8 	ADC_GroupPriorityType;
typedef uint8 	ADC_ChannelType;		//AD能道类型
typedef uint16	ADC_ResultType;			//转换结果类型 



#ifdef ADC_Resolution_8Bit				//转换为8位结果
	#define	ADC_Resolution 	(0x00)
	#define ADC_MaxValue	(255)
#endif

#ifdef ADC_Resolution_10Bit				//转换为10位结果
	#define	ADC_Resolution 	(0x01)	
	#define ADC_MaxValue	(1024)
#endif

#ifdef ADC_Resolution_12Bit		   		//转换为12位结果
	#define	ADC_Resolution 	(0x02)	
	#define ADC_MaxValue	(4096)
#endif


	
void ADC_DeInit(void);	//初始化ADC至复位状态
void ADC_Init(void);	//ADC的初始化
void ADC_StartChannelConversion		//开始通道转换
								(
								ADC_ChannelType firstChannel,	//转换序列的第一个通道 
								ADC_ChannelType lastChannel		//转换序列的最后一个通道
								);
ADC_ResultType ADC_GetChannelResult	//获取AD转换结果
								(
								ADC_ChannelType channel			//通道数
								);
ADC_ResultType ADC_GetVltg 			//获取通道的电压值
								(
								ADC_ChannelType channel			//通道数
								);




#endif
	
	
		
	
	