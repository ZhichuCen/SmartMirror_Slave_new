////////////////////////////////////////////////////////////////////////////////
//  �ļ�����    MCU_CAN.h
//  ����������  CANģ��������ļ�
//  
//  �޸���ʷ��
//  �汾        ������Ա        ����            �޸�˵��
//  --------    --------------  ------------    --------------------------------
//  1.0.0       ������          2009-07-06      �����ļ�
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
#define CANMaxChannels		(1)         //CAN��ͨ����

//��IDֵת��Ϊ��׼֡������֡ID��ע���ʱIDֻ��11λ����λ��Ч
#define	ToStandDataID(CAN_ID)		((((UINT32)CAN_ID)  & 0x7FF) << 21)
//��IDֵת��Ϊ��׼֡��Զ��֡ID��ע���ʱIDֻ��11λ����λ��Ч
#define ToStandRemoteID(CAN_ID)		(((((UINT32)CAN_ID) & 0x7FF) << 21) | 0x00100000)
//��IDֵת��Ϊ��չ֡������֡ID��ע���ʱIDΪ29λ
#define	ToExtendDataID(CAN_ID)		((((UINT32)CAN_ID) << 1) & 0x7FFFE | 0x00180000 | ((((UINT32)CAN_ID) & 0x1FFC0000) << 3))
//��IDֵת��Ϊ��չ֡��Զ��֡ID
#define	ToExtendRemoteID(CAN_ID)    ((((UINT32)CAN_ID) << 1) & 0x7FFFE | 0x00180001 | ((((UINT32)CAN_ID) & 0x1FFC0000) << 3))

////////////////////////////////////functions//////////////////////////////////
ERROR MCU_CAN_Enable 		//ʹ��CANģ��
					(
					UINT8 channel				//ͨ����
					);
ERROR MCU_CAN_Init(void);//CAN������һЩ��ʼ������
ERROR MCU_CAN_EnterInitMode	//�����ʼ��ģʽ
					(
					UINT8 channel			//ͨ����
					);
ERROR MCU_CAN_SetFreq		//�趨CAN��Ƶ��
					(
					UINT8 channel,			//ͨ���� 
					MCU_CAN_FREQ frequence 	//Ŀ��Ƶ��
					);
ERROR MCU_CAN_SetSampling	//�趨CAN������ÿһλ�Ĳ�������
					(
					UINT8 channel,			//ͨ���� 
					MCU_CAN_SAMPLING samplingMethod //��������
					);
ERROR MCU_CAN_SetIDAcceptMode	//�趨CAN��������ʱID�ȶԵ�ģʽ
					(
					UINT8 channel,			//ͨ���� 
					MCU_CAN_ID_MODE idMode	//ID�ȶԵ�ģʽ
					);
ERROR MCU_CAN_SetRXAcceptID		//�趨CANģ�����ʱɸѡ��ID
					(
					UINT8 channel,			//ͨ���� 
					UINT32 idAcceptOne,		//��һ��(��CANIDAR0~CANIDAR3)IDֵ 
					UINT32 idAcceptTwo	   	//�ڶ���(��CANIDAR4~CANIDAR7)IDֵ
					);
ERROR MCU_CAN_SetRxMask	 		//�趨CANģ�����ʱɸѡ��ID����
					(
					UINT8 channel,			//ͨ����  
					UINT32 maskOne,			//��һ��(��CANIDMR0~CANIDMR3)����ֵ 
					UINT32 maskTwo		 	//�ڶ���(��CANIDMR4~CANIDMR7)����ֵ
					);
ERROR MCU_CAN_CheckOutInitMode 	//�趨CANģ���˳���ʼ��ģʽ
					(
					UINT8 channel   		//ͨ����  
					);
ERROR MCU_CAN_Transmit			//����CAN����
					(
					UINT8 channel,  		//ͨ����
					MCU_CAN_MSG msgSend		//�����͵�����
					);
ERROR MCU_CAN_Receive			//��ȡ���յ�������
					(
					UINT8 channel, 			//ͨ����
					MCU_CAN_MSG * msgRec   	//������ݵĵ�ַ
					);
ERROR MCU_CAN_IsReceived  		//�ж�CANģ�����޽��յ�����
					(
					UINT8 channel  			//ͨ����
					);

#endif