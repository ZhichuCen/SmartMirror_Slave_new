////////////////////////////////////////////////////////////////////////////////
//  �ļ�����    ADC_Cfg.h
//  ����������  ģ��ת��ģ������������ļ�
//  ���룺      CodeWarrior for HCS12(X) V4.7 
//  
//  �޸���ʷ��
//  �汾        ������Ա        ����            �޸�˵��
//  --------    --------------  ------------    --------------------------------
//  1.0.0       ������          2009-08-06      �����ļ�
////////////////////////////////////////////////////////////////////////////////
 
#ifndef _ADC_CFG_H_
#define _ADC_CFG_H_

/////////ADC��ת�����λ��������������ֻ��ѡһ��
//#define ADC_Resolution_8Bit
#define ADC_Resolution_10Bit
//#define ADC_Resolution_12Bit


/////////ADC�Ĳο���ѹ
#define ADC_RefVoltage		(5000)	//�ο���ѹΪ5V���ڴ���mVΪ��λ

#define ADC_4ClockCycles 	(0x00)	//4��ADCʱ�Ӳ���ʱ��
#define ADC_6ClockCycles	(0x01)	//6��ADCʱ�Ӳ���ʱ��
#define ADC_8ClockCycles	(0x02)  //8��ADCʱ�Ӳ���ʱ��
#define ADC_10ClockCycles	(0x03)  //10��ADCʱ�Ӳ���ʱ��
#define ADC_12ClockCycles	(0x04)  //12��ADCʱ�Ӳ���ʱ��
#define ADC_16ClockCycles	(0x05)  //16��ADCʱ�Ӳ���ʱ��
#define ADC_20ClockCycles	(0x06)  //20��ADCʱ�Ӳ���ʱ��
#define ADC_24ClockCycles	(0x07)  //24��ADCʱ�Ӳ���ʱ��


#endif