////////////////////////////////////////////////////////////////////////////////
//  文件名：    MCU_CAN.h
//  作用描述：  CAN模块的配置文件
//  
//  修改历史：
//  版本        参与人员        日期            修改说明
//  --------    --------------  ------------    --------------------------------
//  1.0.0       黄文卿          2009-07-06      创建文件
////////////////////////////////////////////////////////////////////////////////
#ifndef _CAN_H_
#define _CAN_H_

////////////////////////////////////includes///////////////////////////////////
#include "Std_Types.h"

#include "CAN_Cfg.h"
#include "CH452.h"
////////////////////////////////////typedefs///////////////////////////////////
typedef enum
{
	CAN1M	= 0x03E8,		//1000
	CAN500K	= 0x01F4,		//500
	CAN250K	= 0x00FA		//250
}	MCU_CAN_FREQ;

typedef enum
{
	CAN_SHORT	= 0x00,
	CAN_LONG	= 0x01
}	MCU_CAN_SAMPLING;

typedef enum
{
	CAN_VER1	= 0x00,
	CAN_VER2	= 0x01
}	MCU_CAN_VERSION;


typedef struct
{
	UINT8 	Data[8];
	UINT32	ID;
	UINT8	DataLength;
}	MCU_CAN_MSG;


typedef enum
{
	TWO_32_BIT		= 0x00,
	FOUR_16_BIT		= 0x01,
	EIGHT_8_BIT		= 0x02,
	FILTER_CLOSED	= 0x03,
}	MCU_CAN_ID_MODE;

////////////////////////////////////defines////////////////////////////////////
#define CANMaxChannels		(1)         //CAN的通道数

//将ID值转化为标准帧、数据帧ID，注意此时ID只有11位，高位无效
#define	ToStandDataID(CAN_ID)		((((UINT32)CAN_ID)  & 0x7FF) << 21)
//将ID值转化为标准帧、远程帧ID，注意此时ID只有11位，高位无效
#define ToStandRemoteID(CAN_ID)		(((((UINT32)CAN_ID) & 0x7FF) << 21) | 0x00100000)
//将ID值转化为扩展帧、数据帧ID，注意此时ID为29位
#define	ToExtendDataID(CAN_ID)		((((UINT32)CAN_ID) << 1) & 0x7FFFE | 0x00180000 | ((((UINT32)CAN_ID) & 0x1FFC0000) << 3))
//将ID值转化为扩展帧、远程帧ID
#define	ToExtendRemoteID(CAN_ID)    ((((UINT32)CAN_ID) << 1) & 0x7FFFE | 0x00180001 | ((((UINT32)CAN_ID) & 0x1FFC0000) << 3))

////////////////////////////////////functions//////////////////////////////////
ERROR MCU_CAN_Enable 		//使能CAN模块
					(
					UINT8 channel				//通道数
					);
ERROR MCU_CAN_Init(void);//CAN的其它一些初始化程序
ERROR MCU_CAN_EnterInitMode	//进入初始化模式
					(
					UINT8 channel			//通道数
					);
ERROR MCU_CAN_SetFreq		//设定CAN的频率
					(
					UINT8 channel,			//通道数 
					MCU_CAN_FREQ frequence 	//目标频率
					);
ERROR MCU_CAN_SetSampling	//设定CAN总线上每一位的采样次数
					(
					UINT8 channel,			//通道数 
					MCU_CAN_SAMPLING samplingMethod //采样次数
					);
ERROR MCU_CAN_SetIDAcceptMode	//设定CAN接收数据时ID比对的模式
					(
					UINT8 channel,			//通道数 
					MCU_CAN_ID_MODE idMode	//ID比对的模式
					);
ERROR MCU_CAN_SetRXAcceptID		//设定CAN模块接收时筛选的ID
					(
					UINT8 channel,			//通道数 
					UINT32 idAcceptOne,		//第一组(即CANIDAR0~CANIDAR3)ID值 
					UINT32 idAcceptTwo	   	//第二组(即CANIDAR4~CANIDAR7)ID值
					);
ERROR MCU_CAN_SetRxMask	 		//设定CAN模块接收时筛选的ID掩码
					(
					UINT8 channel,			//通道数  
					UINT32 maskOne,			//第一组(即CANIDMR0~CANIDMR3)掩码值 
					UINT32 maskTwo		 	//第二组(即CANIDMR4~CANIDMR7)掩码值
					);
ERROR MCU_CAN_CheckOutInitMode 	//设定CAN模块退出初始化模式
					(
					UINT8 channel   		//通道数  
					);
ERROR MCU_CAN_Transmit			//发送CAN数据
					(
					UINT8 channel,  		//通道数
					MCU_CAN_MSG msgSend		//待发送的数据
					);
ERROR MCU_CAN_Receive			//读取接收到的数据
					(
					UINT8 channel, 			//通道数
					MCU_CAN_MSG * msgRec   	//存放数据的地址
					);
ERROR MCU_CAN_IsReceived  		//判断CAN模块有无接收到数据
					(
					UINT8 channel  			//通道数
					);

#endif