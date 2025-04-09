////////////////////////////////////////////////////////////////////////////////
//  文件名：    MCU.h
//  作用描述：  MCU初始化时钟等的头文件
//  编译：      CodeWarrior for HCS12(X) V4.7 
//  
//  修改历史：
//  版本        参与人员        日期            修改说明
//  --------    --------------  ------------    --------------------------------
//  1.0.0       黄文卿          2009-08-01      创建文件
////////////////////////////////////////////////////////////////////////////////
#ifndef _MCU_H_
#define _MCU_H_



////////////////////////////////////includes///////////////////////////////////
#include "Std_Types.h"
#include "MCU_Cfg.h"



////////////////////////////////////typedefs///////////////////////////////////
typedef struct 
{
	uint32 InClk;       //输入单片机的时钟频率，即为晶振时钟
	uint32 OutClk;      //输出频率，即为总线频率
}MCU_ConfigType;


typedef enum
{
	MCU_PLL_LOCKED,			//PLL锁定
	MCU_PLL_UNLOCKED,		//PLL未锁定
	MCU_PLL_STATUS_UNDEFINED//PLL状态未定义
}MCU_PllStatusType;


typedef enum
{
	MCU_POWER_ON_RESET = 0x00,		//上电复位
	MCU_LOW_VOL_RESET,		        //低电压复位
	MCU_ILLEGAL_ADDR_RESET,     	//非法地址复位
	MCU_RESET_UNDEFINED		        //未定义复位
}MCU_ResetType;


typedef uint8 MCU_RawResetType;		//Mcu复位信息原始寄存器状态

typedef uint32 MCU_ClockType;		//总线时钟类型




////////////////////////////////////defines////////////////////////////////////
#define MCU_MAXBUSFREQ 40000000UL  //XS最高不能超过40M的总线时钟，XE不能超过50M





////////////////////////////////////functions//////////////////////////////////
void MCU_Init(void);		//MCU的初始化，目前只有初始化总线时钟
Std_ReturnType MCU_InitClock
							(
							MCU_ClockType clockSetting	//期望总线时钟频率
							);	
MCU_ClockType MCU_GetBusClock(void);//返回总线时钟
MCU_PllStatusType MCU_GetPllStatus(void);//返回时钟状态,是否锁定
MCU_ResetType MCU_GetResetReason(void);//获取复位原因
MCU_RawResetType MCU_GetResetRawValue(void);//获取复位寄存器原始值







#endif// _MCU_H_
