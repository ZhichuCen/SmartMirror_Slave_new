////////////////////////////////////////////////////////////////////////////////
//  �ļ�����    ADC.h
//  ����������  ģ��ת��ģ��ĺ����ȵ������ļ�
//  ���룺      CodeWarrior for HCS12(X) V4.7 
//  
//  �޸���ʷ��
//  �汾        ������Ա        ����            �޸�˵��
//  --------    --------------  ------------    --------------------------------
//  1.0.0       ������          2009-08-06      �����ļ�
////////////////////////////////////////////////////////////////////////////////
#ifndef _ADC_H_
#define _ADC_H_


#include "ADC_Cfg.h"
#include "Platform_Types.h"


typedef uint8 ADC_ResolutionType;		//ת��λ������

typedef uint8 ADC_SamplingTimeType;		//����ʱ������
typedef uint8 ADC_PrescaleType;			//ADʱ��Ԥ��Ƶ���� 

typedef uint8 	ADC_HwUnitType;
typedef uint8 	ADC_GroupPriorityType;
typedef uint8 	ADC_ChannelType;		//AD�ܵ�����
typedef uint16	ADC_ResultType;			//ת��������� 



#ifdef ADC_Resolution_8Bit				//ת��Ϊ8λ���
	#define	ADC_Resolution 	(0x00)
	#define ADC_MaxValue	(255)
#endif

#ifdef ADC_Resolution_10Bit				//ת��Ϊ10λ���
	#define	ADC_Resolution 	(0x01)	
	#define ADC_MaxValue	(1024)
#endif

#ifdef ADC_Resolution_12Bit		   		//ת��Ϊ12λ���
	#define	ADC_Resolution 	(0x02)	
	#define ADC_MaxValue	(4096)
#endif


	
void ADC_DeInit(void);	//��ʼ��ADC����λ״̬
void ADC_Init(void);	//ADC�ĳ�ʼ��
void ADC_StartChannelConversion		//��ʼͨ��ת��
								(
								ADC_ChannelType firstChannel,	//ת�����еĵ�һ��ͨ�� 
								ADC_ChannelType lastChannel		//ת�����е����һ��ͨ��
								);
ADC_ResultType ADC_GetChannelResult	//��ȡADת�����
								(
								ADC_ChannelType channel			//ͨ����
								);
ADC_ResultType ADC_GetVltg 			//��ȡͨ���ĵ�ѹֵ
								(
								ADC_ChannelType channel			//ͨ����
								);




#endif
	
	
		
	
	