////////////////////////////////////////////////////////////////////////////////
//  �ļ�����    ADC.c
//  ����������  ģ��ת��ģ���ʵ���ļ�
//  ���룺      CodeWarrior for HCS12(X) V4.7 
//  
//  �޸���ʷ��
//  �汾        ������Ա        ����            �޸�˵��
//  --------    --------------  ------------    --------------------------------
//  1.0.0       ������          2009-08-06      �����ļ�
////////////////////////////////////////////////////////////////////////////////
#include "ADC.h"
#include "Platform_Types.h"

static uint8 ADC_ChannelMapping[16];
//ADCģ���ͨ���ͽ���Ĵ�������һһ��Ӧ�ġ�ת��������ʼͨ������ATD0DR0�ϣ�
//�����Դ����ƣ��ڴ�Ϊ���㣬������һӳ���滻

//ADC_Cfg.c�е����ñ���
extern const ADC_SamplingTimeType	ADC_SamplingTime;
extern const ADC_PrescaleType		ADC_Prescale;

////////////////////////////////////////////////////////////////////////////////
//  ��������	ADC_DeInit
//  ����������	��ʼ��ADCģ��ļĴ�������λ״̬
//  
//  ����ֵ������void
//				
//  ����������	void	
//  
//  �޸���ʷ��
//  �汾        ������Ա        ����            �޸�˵��
//  --------    --------------  ------------    --------------------------------
//  1.0.0       ������        	2009-08-06      ��������
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
//  ��������	ADC_Init
//  ����������	��ʼ��ADCģ��
//  
//  ����ֵ������void
//				
//  ����������	void	
//  
//  �޸���ʷ��
//  �汾        ������Ա        ����            �޸�˵��
//  --------    --------------  ------------    --------------------------------
//  1.0.0       ������        	2009-08-06      ��������
////////////////////////////////////////////////////////////////////////////////
void ADC_Init(void)
{
    ATD0CTL1_SRES		= ADC_Resolution;	//����λ��
	ATD0CTL1_SMP_DIS	= 0x01;		//����֮ǰ�ڲ����������ȷŵ�
    
	ATD0CTL2_AFFC		= 0x01;		//��־λ��������
	ATD0CTL2_ICLKSTP	= 0x01;		//STPģʽ��AD����ת��
	ATD0CTL2_ETRIGE		= 0x00;		//��ֹ�ⲿ����ģʽ
	ATD0CTL2_ASCIE		= 0x00;		//��ֹת������ж�
	ATD0CTL2_ACMPIE		= 0x00;		//��ֹ�Ƚ��ж�
	
	ATD0CTL3_DJM		= 0x01;		//ת������Ҷ���
	ATD0CTL3_FIFO		= 0x00;		//ת�������ŵ���Ӧ�Ľ���Ĵ�������ѭ������
	ATD0CTL3_FRZ		= 0x00;		//FRZģʽ�¼���ת��

	ATD0CTL4_SMP		= ADC_SamplingTime;		//����ʱ��
	ATD0CTL4_PRS		= (ADC_Prescale / 2) - 1; //ADCʱ�ӷ�Ƶ
	
}

////////////////////////////////////////////////////////////////////////////////
//  ��������	ADC_StartChannelConversion
//  ����������	����ADCģ�����Ӧͨ����ת��
//  
//  ����ֵ������void
//				
//  ����������	
//				firstChannel	ת�����еĵ�һ��ͨ��
//				lastChannel		ת�����е����һ��ͨ��	
//  
//  �޸���ʷ��
//  �汾        ������Ա        ����            �޸�˵��
//  --------    --------------  ------------    --------------------------------
//  1.0.0       ������        	2009-08-06      ��������
////////////////////////////////////////////////////////////////////////////////
void ADC_StartChannelConversion(ADC_ChannelType firstChannel, ADC_ChannelType lastChannel)
{
	uint8 l_SeqLength;
	uint8 l_Channel;
	
	if((lastChannel > 15) || (firstChannel > 15))	//XS��ͨ�������ܳ���15
	{
		return;
	}
	
	for(l_Channel = 0; l_Channel < 16; l_Channel++)
	{
		ADC_ChannelMapping[l_Channel] = 0xff;		//��ʼ��ͨ��ӳ��	
	}
	
	l_SeqLength = (lastChannel + 17 - firstChannel) & 0x0f;//�ܵ�ת�����еĳ���
													//�������Ϊ16,��Ϊ0
	
	
    ATD0CTL3_S1C	= l_SeqLength & 0x01;			//���г��ȣ�����ת��Ϊ�����Ƹ�ֵ
    ATD0CTL3_S2C	= (l_SeqLength >> 1) & 0x01;	
	ATD0CTL3_S4C	= (l_SeqLength >> 2) & 0x01;	
	ATD0CTL3_S8C	= (l_SeqLength >> 3) & 0x01;
	

	ATD0CTL5_SC		= 0x00;			//��ֹ����ת��
	ATD0CTL5_SCAN	= 0x01;			//����ת��
	if(l_SeqLength == 1)
	{
		ATD0CTL5_MULT	= 0x00;			//��ͨ��ת��
	}
	else
	{
		ATD0CTL5_MULT	= 0x01;			//��ͨ��ת��
	}
	ATD0CTL5_Cx		= firstChannel;		//��firstChannelͨ����ʼת��

	if(l_SeqLength == 0)	//�������Ϊ0,ʵ��Ϊ16��ת��ͨ��
	{
		l_SeqLength = 16;
	}
	for(l_Channel = 0; l_Channel < l_SeqLength; l_Channel++)
	{
		ADC_ChannelMapping[(firstChannel + l_Channel) & 0x0f] = l_Channel;	
	}
}

////////////////////////////////////////////////////////////////////////////////
//  ��������	ADC_GetChannelResult
//  ����������	��ȡ��Ӧͨ����ADת�����
//  
//  ����ֵ��������Ӧͨ����ת�����
//				
//  ����������	
//				channel			ͨ����
//  
//  �޸���ʷ��
//  �汾        ������Ա        ����            �޸�˵��
//  --------    --------------  ------------    --------------------------------
//  1.0.0       ������        	2009-08-06      ��������
////////////////////////////////////////////////////////////////////////////////
ADC_ResultType ADC_GetChannelResult(ADC_ChannelType channel)
{
	uint8 l_ADCRetry;
	for(l_ADCRetry = 0; l_ADCRetry < 250; l_ADCRetry++)
	{
		if(1 == ATD0STAT0_SCF)	//�ж�һ�������Ƿ�ת�����
		{
			break;
		}
	}
	
	if(l_ADCRetry >= 250)		//��ʱ������0
	{
		return 0;
	}
	
	switch(ADC_ChannelMapping[channel])		//��ȡ��Ӧͨ���Ľ��
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
			return 0;			//XS��ͨ�������ܳ���15
			
	}
}

////////////////////////////////////////////////////////////////////////////////
//  ��������	ADC_GetVltg
//  ����������	��ȡ��Ӧͨ����ѹֵ(��ת�����ת��Ϊ��ѹֵ)
//  
//  ����ֵ��������Ӧͨ���ĵ�ѹֵ
//				
//  ����������	
//				channel			ͨ����
//  
//  �޸���ʷ��
//  �汾        ������Ա        ����            �޸�˵��
//  --------    --------------  ------------    --------------------------------
//  1.0.0       ������        	2009-08-06      ��������
////////////////////////////////////////////////////////////////////////////////
ADC_ResultType ADC_GetVltg(ADC_ChannelType channel)
{
	switch(ADC_ChannelMapping[channel])		//��ȡ��Ӧͨ���Ľ������ת�ɵ�ѹֵ
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