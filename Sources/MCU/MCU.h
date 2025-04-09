////////////////////////////////////////////////////////////////////////////////
//  �ļ�����    MCU.h
//  ����������  MCU��ʼ��ʱ�ӵȵ�ͷ�ļ�
//  ���룺      CodeWarrior for HCS12(X) V4.7 
//  
//  �޸���ʷ��
//  �汾        ������Ա        ����            �޸�˵��
//  --------    --------------  ------------    --------------------------------
//  1.0.0       ������          2009-08-01      �����ļ�
////////////////////////////////////////////////////////////////////////////////
#ifndef _MCU_H_
#define _MCU_H_



////////////////////////////////////includes///////////////////////////////////
#include "Std_Types.h"
#include "MCU_Cfg.h"



////////////////////////////////////typedefs///////////////////////////////////
typedef struct 
{
	uint32 InClk;       //���뵥Ƭ����ʱ��Ƶ�ʣ���Ϊ����ʱ��
	uint32 OutClk;      //���Ƶ�ʣ���Ϊ����Ƶ��
}MCU_ConfigType;


typedef enum
{
	MCU_PLL_LOCKED,			//PLL����
	MCU_PLL_UNLOCKED,		//PLLδ����
	MCU_PLL_STATUS_UNDEFINED//PLL״̬δ����
}MCU_PllStatusType;


typedef enum
{
	MCU_POWER_ON_RESET = 0x00,		//�ϵ縴λ
	MCU_LOW_VOL_RESET,		        //�͵�ѹ��λ
	MCU_ILLEGAL_ADDR_RESET,     	//�Ƿ���ַ��λ
	MCU_RESET_UNDEFINED		        //δ���帴λ
}MCU_ResetType;


typedef uint8 MCU_RawResetType;		//Mcu��λ��Ϣԭʼ�Ĵ���״̬

typedef uint32 MCU_ClockType;		//����ʱ������




////////////////////////////////////defines////////////////////////////////////
#define MCU_MAXBUSFREQ 40000000UL  //XS��߲��ܳ���40M������ʱ�ӣ�XE���ܳ���50M





////////////////////////////////////functions//////////////////////////////////
void MCU_Init(void);		//MCU�ĳ�ʼ����Ŀǰֻ�г�ʼ������ʱ��
Std_ReturnType MCU_InitClock
							(
							MCU_ClockType clockSetting	//��������ʱ��Ƶ��
							);	
MCU_ClockType MCU_GetBusClock(void);//��������ʱ��
MCU_PllStatusType MCU_GetPllStatus(void);//����ʱ��״̬,�Ƿ�����
MCU_ResetType MCU_GetResetReason(void);//��ȡ��λԭ��
MCU_RawResetType MCU_GetResetRawValue(void);//��ȡ��λ�Ĵ���ԭʼֵ







#endif// _MCU_H_
