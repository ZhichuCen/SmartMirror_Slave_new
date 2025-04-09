////////////////////////////////////////////////////////////////////////////////
//  文件名：    ADC_Cfg.h
//  作用描述：  模数转换模块的配置声明文件
//  编译：      CodeWarrior for HCS12(X) V4.7 
//  
//  修改历史：
//  版本        参与人员        日期            修改说明
//  --------    --------------  ------------    --------------------------------
//  1.0.0       黄文卿          2009-08-06      创建文件
////////////////////////////////////////////////////////////////////////////////
 
#ifndef _ADC_CFG_H_
#define _ADC_CFG_H_

/////////ADC的转换结果位数，以下三种中只能选一种
//#define ADC_Resolution_8Bit
#define ADC_Resolution_10Bit
//#define ADC_Resolution_12Bit


/////////ADC的参考电压
#define ADC_RefVoltage		(5000)	//参考电压为5V，在此以mV为单位

#define ADC_4ClockCycles 	(0x00)	//4个ADC时钟采样时间
#define ADC_6ClockCycles	(0x01)	//6个ADC时钟采样时间
#define ADC_8ClockCycles	(0x02)  //8个ADC时钟采样时间
#define ADC_10ClockCycles	(0x03)  //10个ADC时钟采样时间
#define ADC_12ClockCycles	(0x04)  //12个ADC时钟采样时间
#define ADC_16ClockCycles	(0x05)  //16个ADC时钟采样时间
#define ADC_20ClockCycles	(0x06)  //20个ADC时钟采样时间
#define ADC_24ClockCycles	(0x07)  //24个ADC时钟采样时间


#endif