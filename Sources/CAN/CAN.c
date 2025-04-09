////////////////////////////////////////////////////////////////////////////////
//  �ļ�����    MCU_CAN.c
//  ����������  CANģ���ʵ���ļ�
//  
//  �޸���ʷ��
//  �汾        ������Ա        ����            �޸�˵��
//  --------    --------------  ------------    --------------------------------
//  1.0.0       ������          2009-07-06      �����ļ�
////////////////////////////////////////////////////////////////////////////////
#include "CAN.h"
#include "Platform_Types.h"


#define CONF_INCLK		12000000UL      //���뵥Ƭ����ʱ��Ƶ�ʣ��ڴ�Ϊ8M�ľ���ʱ��
#define CONF_OUTCLK		32000000UL      //���Ƶ�ʣ��ڴ�Ϊϵͳ������Ƶ�ʣ�32M


#define CANBuffMaxLength  	(8)         //ÿһͨ�����ͺͽ��ջ������Ĵ�С    
#define CAN_BUSFREQ			CONF_OUTCLK

const UINT8 c_MCU_CAN_ChannelMapping[5] = {0x00,0x02,0x03,0x04,0x01};
//XE���������һ·CAN����ΪCAN0���������·����ΪCAN4,��·��ΪCAN0,CAN1,CAN2
//��·ΪCAN0,CAN1,CAN2,CAN4,��·ΪCAN0,CAN1,CAN2,CAN3,CAN4
//CAN4�ĵ�λ�൱�ڲ嵽��CAN0��CAN1֮�䣬����ô˳�������һӳ��


MCU_CAN_MSG g_MCU_CAN_RXBuff[CANMaxChannels][CANBuffMaxLength]; //���ջ�����
Boolean		g_MCU_CAN_RXFull[CANMaxChannels];                   //���ջ���������־
UINT8		g_MCU_CAN_RXFront[CANMaxChannels];                  //���ջ�������ͷ
UINT8   	g_MCU_CAN_RXRear[CANMaxChannels];                   //���ջ�������β


//ģ���ڲ�ʹ�ú�������
ERROR MCU_CAN_AddRxData(UINT8 channel, MCU_CAN_MSG   data);
ERROR MCU_CAN_GetRxData(UINT8 channel, MCU_CAN_MSG * data);

////////////////////////////////////////////////////////////////////////////////
//  ��������	MCU_CAN_Enable
//  ����������	ʹ��CANģ��
//  
//  ����ֵ������E_OK		�趨�ɹ�
//				
//  ���������� 	channel		ͨ��
//			
//  
//  �޸���ʷ��
//  �汾        ������Ա        ����            �޸�˵��
//  --------    --------------  ------------    --------------------------------
//  1.0.0       ������        	2009-07-11      ��������
////////////////////////////////////////////////////////////////////////////////
ERROR MCU_CAN_Enable(UINT8 channel)
{
	switch(channel)
	{
		case 0:
			CAN0CTL1_CANE  = 0x01;     //ʹ��CANģ��			
			return E_OK;
			
			
#if CANMaxChannels > 2	//XE��CAN��Ŀ����2,����CAN1			
		case 1:
			CAN1CTL1_CANE  = 0x01;     //ʹ��CANģ��			
			return E_OK;
#endif

#if CANMaxChannels > 3	//XE��CAN��Ŀ����3,����CAN2			
		case 2:
			CAN2CTL1_CANE  = 0x01;     //ʹ��CANģ��			
			return E_OK;
#endif

#if CANMaxChannels > 4	//XE��CAN��Ŀ����4,����CAN3		
		case 3:
			CAN3CTL1_CANE  = 0x01;     //ʹ��CANģ��			
			return E_OK;
#endif

#if CANMaxChannels > 1	//XE��CAN��Ŀ����1,����CAN4			
		case 4:
			CAN4CTL1_CANE  = 0x01;     //ʹ��CANģ��			
			return E_OK;
#endif
        default:
            return E_FAIL;

	}
}




////////////////////////////////////////////////////////////////////////////////
//  ��������	MCU_CAN_EnterInitMode
//  ����������	�趨CANģ������ʼ��ģʽ
//  
//  ����ֵ������E_OK		�ɹ������ʼ��״̬
//				E_FAIL      ��ʱ���趨��ʼ��״̬ʧ��
//  ����������	channel		ͨ��
//			
//  
//  �޸���ʷ��
//  �汾        ������Ա        ����            �޸�˵��
//  --------    --------------  ------------    --------------------------------
//  1.0.0       ������        	2009-07-11      ��������
////////////////////////////////////////////////////////////////////////////////
ERROR MCU_CAN_EnterInitMode(UINT8 channel)
{
	UINT16 l_InitModeRetry;                     //�ȴ�����
		
	switch(channel)
	{
		case 0:
			CAN0CTL0_INITRQ = 0x01;             //�����ʼ������ģʽ
			for(l_InitModeRetry = 0; l_InitModeRetry < 60000; l_InitModeRetry++)
			{
				if(CAN0CTL1_INITAK == 0x01)
				{
					return E_OK;					
				}
			}
			return E_FAIL;
			
			
#if CANMaxChannels > 2			
		case 1:
			CAN1CTL0_INITRQ = 0x01;             //�����ʼ������ģʽ
			for(l_InitModeRetry = 0; l_InitModeRetry < 60000; l_InitModeRetry++)
			{
				if(CAN1CTL1_INITAK == 0x01)
				{
					return E_OK;					
				}
			}
			return E_FAIL;
#endif

#if CANMaxChannels > 3			
		case 2:
			CAN2CTL0_INITRQ = 0x01;             //�����ʼ������ģʽ
			for(l_InitModeRetry = 0; l_InitModeRetry < 60000; l_InitModeRetry++)
			{
				if(CAN2CTL1_INITAK == 0x01)
				{
					return E_OK;					
				}
			}
			return E_FAIL;
#endif

#if CANMaxChannels > 4			
		case 3:
			CAN3CTL0_INITRQ = 0x01;             //�����ʼ������ģʽ
			for(l_InitModeRetry = 0; l_InitModeRetry < 60000; l_InitModeRetry++)
			{
				if(CAN3CTL1_INITAK == 0x01)
				{
					return E_OK;					
				}
			}
			return E_FAIL;
#endif

#if CANMaxChannels > 1			
		case 4:
			CAN4CTL0_INITRQ = 0x01;             //�����ʼ������ģʽ
			for(l_InitModeRetry = 0; l_InitModeRetry < 60000; l_InitModeRetry++)
			{
				if(CAN4CTL1_INITAK == 0x01)
				{
					return E_OK;					
				}
			}
			return E_FAIL;
#endif

		default:
			return E_FAIL;
			
	}
	
}

////////////////////////////////////////////////////////////////////////////////
//  ��������	MCU_CAN_Init
//  ����������	�趨CAN��һЩ��ʼ��
//  
//  ����ֵ������E_OK		�趨�ɹ�
//				
//  ����������	��
//			
//  �޸���ʷ��
//  �汾        ������Ա        ����            �޸�˵��
//  --------    --------------  ------------    --------------------------------
//  1.0.0       ������        	2009-07-11      ��������
////////////////////////////////////////////////////////////////////////////////
ERROR MCU_CAN_Init(void)
{
    UINT16  l_i, l_j;       //��ʱ�С�������
    
    //��ʼ������
    MCU_CAN_MSG l_TmpMsg={{0,0,0,0,0,0,0,0},0,0};
    
	if(CAN0CTL1_INITAK == 0x00)
	{
		return E_FAIL;
	}

    
    for(l_j = 0; l_j < CANMaxChannels; l_j++)
    {
        for(l_i = 0; l_i < CANBuffMaxLength; l_i++)
        {
            g_MCU_CAN_RXBuff[l_j][l_i]  = l_TmpMsg;
        };
        
        g_MCU_CAN_RXFront[l_j]          = 0;
        g_MCU_CAN_RXRear[l_j]           = 0;
        g_MCU_CAN_RXFull[l_j]           = FALSE;
    };


    for(l_j = 0; l_j < CANMaxChannels; l_j++)
    {
        
    	switch(l_j)
    	{
    		case 0:
    			CAN0CTL1_LOOPB          = 0x00;     //��ֹ�Է�����ģʽ
    			CAN0CTL1_LISTEN         = 0x00;     //��ֹֻ��ģʽ
    			CAN0CTL1_BORM           = 0x00;     //�Զ����߻ָ�
    			CAN0CTL1_WUPM           = 0x00;     //CAN�����ϵ��κ�����λ����CANģ��(SLEEPģʽ��)
    			break;
    			
#if CANMaxChannels > 2			
    		case 2:
    			CAN1CTL1_LOOPB          = 0x00;     //��ֹ�Է�����ģʽ
    			CAN1CTL1_LISTEN         = 0x00;     //��ֹֻ��ģʽ
    			CAN1CTL1_BORM           = 0x00;     //�Զ����߻ָ�
    			CAN1CTL1_WUPM           = 0x00;     //CAN�����ϵ��κ�����λ����CANģ��(SLEEPģʽ��)
				break;
#endif

#if CANMaxChannels > 3			
    		case 3:
    			CAN2CTL1_LOOPB          = 0x00;     //��ֹ�Է�����ģʽ
    			CAN2CTL1_LISTEN         = 0x00;     //��ֹֻ��ģʽ
    			CAN2CTL1_BORM           = 0x00;     //�Զ����߻ָ�
    			CAN2CTL1_WUPM           = 0x00;     //CAN�����ϵ��κ�����λ����CANģ��(SLEEPģʽ��)
				break;
#endif

#if CANMaxChannels > 4			
    		case 4:
    			CAN3CTL1_LOOPB          = 0x00;     //��ֹ�Է�����ģʽ
    			CAN3CTL1_LISTEN         = 0x00;     //��ֹֻ��ģʽ
    			CAN3CTL1_BORM           = 0x00;     //�Զ����߻ָ�
    			CAN3CTL1_WUPM           = 0x00;     //CAN�����ϵ��κ�����λ����CANģ��(SLEEPģʽ��)
				break;
#endif

#if CANMaxChannels > 1			
    		case 1:
    			CAN4CTL1_LOOPB          = 0x00;     //��ֹ�Է�����ģʽ
    			CAN4CTL1_LISTEN         = 0x00;     //��ֹֻ��ģʽ
    			CAN4CTL1_BORM           = 0x00;     //�Զ����߻ָ�
    			CAN4CTL1_WUPM           = 0x00;     //CAN�����ϵ��κ�����λ����CANģ��(SLEEPģʽ��)
				break;
#endif
			default:
				return E_FAIL;

    	}
    }
    return E_OK;
	
	
}




////////////////////////////////////////////////////////////////////////////////
//  ��������	MCU_CAN_SetFreq
//  ����������	�趨CANģ���Ƶ��
//  
//  ����ֵ������E_OK	    �趨�ɹ�	
//				E_FAIL      δ�ҵ����ʵķ�Ƶ��δ�����ʼ��ģʽ���趨ʧ��
//			
//  ����������	channel 	CAN��ͨ����
//			   	frequence	���ٵ�CAN��Ƶ��ֵ
//  
//  �޸���ʷ��
//  �汾        ������Ա        ����            �޸�˵��
//  --------    --------------  ------------    --------------------------------
//  1.0.0       ������        	2009-07-11      ��������
////////////////////////////////////////////////////////////////////////////////
ERROR MCU_CAN_SetFreq(UINT8 channel, MCU_CAN_FREQ frequence)
{
	UINT8 	l_Prescale;		//����Ƶ�ʻ����Ǿ���Ƶ����Ŀ��Ƶ�ʵı�ֵ�����ܷ�Ƶ
	UINT8 	l_TimSeg;		//(1+TimeSegment1+TimeSegment2)���ܺ�
	switch(channel)
	{
		case 0:
			if(CAN0CTL1_INITAK == 0x01)		//���û�н����ʼ��ģʽ��������Ч
			{
				CAN0BTR0_SJW 	= 0x00;		//ͬ����ת���Ϊ1Tq
				l_Prescale = (UINT8)(CONF_INCLK / 1000U / frequence);
				if(l_Prescale >= 8)			//����ʹ�þ���ʱ��
				{
					CAN0CTL1_CLKSRC	= 0x00;		//CANʱ�������ھ���ʱ��
				}
				else
				{
				
					CAN0CTL1_CLKSRC	= 0x01;		//CANʱ������������ʱ��
					l_Prescale = (UINT8)(CAN_BUSFREQ / 1000U / frequence);
				}
				for(l_TimSeg = 8; l_TimSeg <= 25; l_TimSeg++)	//����Ѱ�Һ��ʵķ�Ƶֵ
				{
					if(((l_Prescale / l_TimSeg) * l_TimSeg) ==  l_Prescale)
					{									 		//�պ��ܳ��������Ƶ����
						CAN0BTR0_BRP		= l_Prescale / l_TimSeg - 1;
																//����Ԥ��Ƶ��
						CAN0BTR1_TSEG_20	= (UINT8)((l_TimSeg >= 18) ? 7 : (l_TimSeg / 2 - 2));
						CAN0BTR1_TSEG_10	= l_TimSeg  - CAN0BTR1_TSEG_20 - 3 ;
						return E_OK;										   
					}
				}
				
			}
			return E_FAIL;	//δ�����ʼ��ģʽ��û�к��ʵķ�Ƶ������Ƶ��ʧ��
			
			
#if CANMaxChannels > 2			
		case 1:
			if(CAN1CTL1_INITAK == 0x01)		//���û�н����ʼ��ģʽ��������Ч
			{
				CAN1BTR0_SJW 	= 0x00;		//ͬ����ת���Ϊ1Tq
				l_Prescale = (UINT8)(CONF_INCLK / 1000U / frequence);
				if(l_Prescale >= 8)			//����ʹ�þ���ʱ��
				{
					CAN1CTL1_CLKSRC	= 0x00;		//CANʱ�������ھ���ʱ��
				}
				else
				{
				
					CAN1CTL1_CLKSRC	= 0x01;		//CANʱ������������ʱ��
					l_Prescale = (UINT8)(CAN_BUSFREQ / 1000U / frequence);
				}
				for(l_TimSeg = 8; l_TimSeg <= 25; l_TimSeg++)	//����Ѱ�Һ��ʵķ�Ƶֵ
				{
					if(((l_Prescale / l_TimSeg) * l_TimSeg) ==  l_Prescale)
					{									 		//�պ��ܳ��������Ƶ����
						CAN1BTR0_BRP		= l_Prescale / l_TimSeg - 1;
																//����Ԥ��Ƶ��
						CAN1BTR1_TSEG_20	= (UINT8)((l_TimSeg >= 18) ? 7 : (l_TimSeg / 2 - 2));
						CAN1BTR1_TSEG_10	= l_TimSeg  - CAN1BTR1_TSEG_20 - 3 ;
						return E_OK;										   
					}
				}
				
			}
			return E_FAIL;	//δ�����ʼ��ģʽ��û�к��ʵķ�Ƶ������Ƶ��ʧ��
#endif

#if CANMaxChannels > 3			
		case 2:
			if(CAN2CTL1_INITAK == 0x01)		//���û�н����ʼ��ģʽ��������Ч
			{
				CAN2BTR0_SJW 	= 0x00;		//ͬ����ת���Ϊ1Tq
				l_Prescale = (UINT8)(CONF_INCLK / 1000U / frequence);
				if(l_Prescale >= 8)			//����ʹ�þ���ʱ��
				{
					CAN2CTL1_CLKSRC	= 0x00;		//CANʱ�������ھ���ʱ��
				}
				else
				{
				
					CAN2CTL1_CLKSRC	= 0x01;		//CANʱ������������ʱ��
					l_Prescale = (UINT8)(CAN_BUSFREQ / 1000U / frequence);
				}
				for(l_TimSeg = 8; l_TimSeg <= 25; l_TimSeg++)	//����Ѱ�Һ��ʵķ�Ƶֵ
				{
					if(((l_Prescale / l_TimSeg) * l_TimSeg) ==  l_Prescale)
					{									 		//�պ��ܳ��������Ƶ����
						CAN2BTR0_BRP		= l_Prescale / l_TimSeg - 1;
																//����Ԥ��Ƶ��
						CAN2BTR1_TSEG_20	= (UINT8)((l_TimSeg >= 18) ? 7 : (l_TimSeg / 2 - 2));
						CAN2BTR1_TSEG_10	= l_TimSeg  - CAN2BTR1_TSEG_20 - 3 ;
						return E_OK;										   
					}
				}
				
			}
			return E_FAIL;	//δ�����ʼ��ģʽ��û�к��ʵķ�Ƶ������Ƶ��ʧ��
#endif

#if CANMaxChannels > 4			
		case 3:
			if(CAN3CTL1_INITAK == 0x01)		//���û�н����ʼ��ģʽ��������Ч
			{
				CAN3BTR0_SJW 	= 0x00;		//ͬ����ת���Ϊ1Tq
				l_Prescale = (UINT8)(CONF_INCLK / 1000U / frequence);
				if(l_Prescale >= 8)			//����ʹ�þ���ʱ��
				{
					CAN3CTL1_CLKSRC	= 0x00;		//CANʱ�������ھ���ʱ��
				}
				else
				{
				
					CAN3CTL1_CLKSRC	= 0x01;		//CANʱ������������ʱ��
					l_Prescale = (UINT8)(CAN_BUSFREQ / 1000U / frequence);
				}
				for(l_TimSeg = 8; l_TimSeg <= 25; l_TimSeg++)	//����Ѱ�Һ��ʵķ�Ƶֵ
				{
					if(((l_Prescale / l_TimSeg) * l_TimSeg) ==  l_Prescale)
					{									 		//�պ��ܳ��������Ƶ����
						CAN3BTR0_BRP		= l_Prescale / l_TimSeg - 1;
																//����Ԥ��Ƶ��
						CAN3BTR1_TSEG_20	= (UINT8)((l_TimSeg >= 18) ? 7 : (l_TimSeg / 2 - 2));
						CAN3BTR1_TSEG_10	= l_TimSeg  - CAN3BTR1_TSEG_20 - 3 ;
						return E_OK;										   
					}
				}
				
			}
			return E_FAIL;	//δ�����ʼ��ģʽ��û�к��ʵķ�Ƶ������Ƶ��ʧ��
#endif

#if CANMaxChannels > 1			
		case 4:
			if(CAN4CTL1_INITAK == 0x01)		//���û�н����ʼ��ģʽ��������Ч
			{
				CAN4BTR0_SJW 	= 0x00;		//ͬ����ת���Ϊ1Tq
				l_Prescale = (UINT8)(CONF_INCLK / 1000U / frequence);
				if(l_Prescale >= 8)			//����ʹ�þ���ʱ��
				{
					CAN4CTL1_CLKSRC	= 0x00;		//CANʱ�������ھ���ʱ��
				}
				else
				{
				
					CAN4CTL1_CLKSRC	= 0x01;		//CANʱ������������ʱ��
					l_Prescale = (UINT8)(CAN_BUSFREQ / 1000U / frequence);
				}
				for(l_TimSeg = 8; l_TimSeg <= 25; l_TimSeg++)	//����Ѱ�Һ��ʵķ�Ƶֵ
				{
					if(((l_Prescale / l_TimSeg) * l_TimSeg) ==  l_Prescale)
					{									 		//�պ��ܳ��������Ƶ����
						CAN4BTR0_BRP		= l_Prescale / l_TimSeg - 1;
																//����Ԥ��Ƶ��
						CAN4BTR1_TSEG_20	= (UINT8)((l_TimSeg >= 18) ? 7 : (l_TimSeg / 2 - 2));
						CAN4BTR1_TSEG_10	= l_TimSeg  - CAN4BTR1_TSEG_20 - 3 ;
						return E_OK;										   
					}
				}
				
			}
			return E_FAIL;	//δ�����ʼ��ģʽ��û�к��ʵķ�Ƶ������Ƶ��ʧ��
#endif

		default:
			return E_FAIL;
			
	}
}

////////////////////////////////////////////////////////////////////////////////
//  ��������	MCU_CAN_SetSampling
//  ����������	�趨CAN������ÿһλ�Ĳ�������
//  
//  ����ֵ������E_OK	    �趨�ɹ�	
//				E_FAIL      ���������δ�����ʼ��ģʽ���趨ʧ��
//			
//  ����������	channel 		CAN��ͨ����
//			   	samplingMethod	ö�ٵ�CAN�Ĳ�������
//  
//  �޸���ʷ��
//  �汾        ������Ա        ����            �޸�˵��
//  --------    --------------  ------------    --------------------------------
//  1.0.0       ������        	2009-07-11      ��������
////////////////////////////////////////////////////////////////////////////////
ERROR MCU_CAN_SetSampling(UINT8 channel, MCU_CAN_SAMPLING samplingMethod)
{
	switch(channel)
	{
		case 0:
			if(CAN0CTL1_INITAK == 0x01)
			{
				switch(samplingMethod)
				{
					case CAN_LONG:
					
						CAN0BTR1_SAMP	= 0x01;			//ÿһλ��������
						return E_OK;
						
					case CAN_SHORT:
					
						CAN0BTR1_SAMP	= 0x00;			//ÿһλһ������
						return E_OK;
						
					default:
					
						return E_FAIL;
						
				} 
			}
			return E_FAIL;			//δ�����ʼ��ģʽ������ʧ��
			
			
#if CANMaxChannels > 2			
		case 1:
			if(CAN1CTL1_INITAK == 0x01)
			{
				switch(samplingMethod)
				{
					case CAN_LONG:
					
						CAN1BTR1_SAMP	= 0x01;			//ÿһλ��������
						return E_OK;
						
					case CAN_SHORT:
					
						CAN1BTR1_SAMP	= 0x00;			//ÿһλһ������
						return E_OK;
						
					default:
					
						return E_FAIL;
						
				} 
			}
			return E_FAIL;			//δ�����ʼ��ģʽ������ʧ��
#endif

#if CANMaxChannels > 3			
		case 2:
			if(CAN2CTL1_INITAK == 0x01)
			{
				switch(samplingMethod)
				{
					case CAN_LONG:
					
						CAN2BTR1_SAMP	= 0x01;			//ÿһλ��������
						return E_OK;
						
					case CAN_SHORT:
					
						CAN2BTR1_SAMP	= 0x00;			//ÿһλһ������
						return E_OK;
						
					default:
					
						return E_FAIL;
						
				} 
			}
			return E_FAIL;			//δ�����ʼ��ģʽ������ʧ��
#endif

#if CANMaxChannels > 4			
		case 3:
			if(CAN3CTL1_INITAK == 0x01)
			{
				switch(samplingMethod)
				{
					case CAN_LONG:
					
						CAN3BTR1_SAMP	= 0x01;			//ÿһλ��������
						return E_OK;
						
					case CAN_SHORT:
					
						CAN3BTR1_SAMP	= 0x00;			//ÿһλһ������
						return E_OK;
						
					default:
					
						return E_FAIL;
						
				} 
			}
			return E_FAIL;			//δ�����ʼ��ģʽ������ʧ��
#endif

#if CANMaxChannels > 1			
		case 4:
			if(CAN4CTL1_INITAK == 0x01)
			{
				switch(samplingMethod)
				{
					case CAN_LONG:
					
						CAN4BTR1_SAMP	= 0x01;			//ÿһλ��������
						return E_OK;
						
					case CAN_SHORT:
					
						CAN4BTR1_SAMP	= 0x00;			//ÿһλһ������
						return E_OK;
						
					default:
					
						return E_FAIL;
						
				} 
			}
			return E_FAIL;			//δ�����ʼ��ģʽ������ʧ��
#endif

		default:
			return E_FAIL;
			
	}
}



////////////////////////////////////////////////////////////////////////////////
//  ��������	MCU_CAN_SetIDAcceptMode
//  ����������	�趨CANģ�����ʱID�ȶԵ�ģʽ
//  
//  ����ֵ������E_OK	    �趨�ɹ�	
//				E_FAIL      ���������δ�����ʼ��ģʽ���趨ʧ��
//			
//  ����������	channel 	CAN��ͨ����
//			   	idMode		���ٵ�CAN�Ľ���ID�ȶԵ�ģʽ
//  
//  �޸���ʷ��
//  �汾        ������Ա        ����            �޸�˵��
//  --------    --------------  ------------    --------------------------------
//  1.0.0       ������        	2009-07-11      ��������
////////////////////////////////////////////////////////////////////////////////
ERROR MCU_CAN_SetIDAcceptMode(UINT8 channel, MCU_CAN_ID_MODE idMode)
{
	if(idMode > 0x03)	//ģʽ���������趨ʧ��
	{
		return E_FAIL;
	}
	
	switch(channel)
	{
		case 0:
		
			if(CAN0CTL1_INITAK == 0x01)		//ȷ�������ʼ��ģʽ
			{
				CAN0IDAC_IDAM	= (UINT8)(idMode);
				return E_OK;
			}
			return E_FAIL;					//δ�����ʼ��ģʽ���趨ʧ��
			
			
#if CANMaxChannels > 2			
		case 1:
			if(CAN1CTL1_INITAK == 0x01)		//ȷ�������ʼ��ģʽ
			{
				CAN1IDAC_IDAM	= (UINT8)(idMode);
				return E_OK;
			}
			return E_FAIL;					//δ�����ʼ��ģʽ���趨ʧ��
#endif

#if CANMaxChannels > 3			
		case 2:
			if(CAN2CTL1_INITAK == 0x01)		//ȷ�������ʼ��ģʽ
			{
				CAN2IDAC_IDAM	= (UINT8)(idMode);
				return E_OK;
			}
			return E_FAIL;					//δ�����ʼ��ģʽ���趨ʧ��
#endif

#if CANMaxChannels > 4			
		case 3:
			if(CAN3CTL1_INITAK == 0x01)		//ȷ�������ʼ��ģʽ
			{
				CAN3IDAC_IDAM	= (UINT8)(idMode);
				return E_OK;
			}
			return E_FAIL;					//δ�����ʼ��ģʽ���趨ʧ��
#endif

#if CANMaxChannels > 1			
		case 4:
			if(CAN4CTL1_INITAK == 0x01)		//ȷ�������ʼ��ģʽ
			{
				CAN4IDAC_IDAM	= (UINT8)(idMode);
				return E_OK;
			}
			return E_FAIL;					//δ�����ʼ��ģʽ���趨ʧ��
#endif

		default:
			return E_FAIL; 
	}
			
}




////////////////////////////////////////////////////////////////////////////////
//  ��������	MCU_CAN_SetRXAcceptID
//  ����������	�趨CANģ�����ʱɸѡ��ID
//  
//  ����ֵ������E_OK	    �趨�ɹ�	
//				E_FAIL      ���������δ�����ʼ��ģʽ���趨ʧ��
//			
//  ����������	channel 	CAN��ͨ����
//			   	idAcceptOne	��һ��(��CANIDAR0~CANIDAR3)IDֵ
//				idAcceptTwo �ڶ���(��CANIDAR4~CANIDAR7)IDֵ	
//  
//  �޸���ʷ��
//  �汾        ������Ա        ����            �޸�˵��
//  --------    --------------  ------------    --------------------------------
//  1.0.0       ������        	2009-07-11      ��������
////////////////////////////////////////////////////////////////////////////////
ERROR MCU_CAN_SetRXAcceptID(UINT8 channel, UINT32 idAcceptOne, UINT32 idAcceptTwo)
{
	switch(channel)
	{
		case 0:
			if(CAN0CTL1_INITAK == 0x01)
			{
			    CAN0IDAR0 = (UINT8)((idAcceptOne & 0xff000000) >> 24);
			    CAN0IDAR1 = (UINT8)((idAcceptOne & 0x00ff0000) >> 16);
			    CAN0IDAR2 = (UINT8)((idAcceptOne & 0x0000ff00) >> 8);
			    CAN0IDAR3 = (UINT8)((idAcceptOne & 0x000000ff));
			    
			    CAN0IDAR4 = (UINT8)((idAcceptTwo & 0xff000000) >> 24);
			    CAN0IDAR5 = (UINT8)((idAcceptTwo & 0x00ff0000) >> 16);
			    CAN0IDAR6 = (UINT8)((idAcceptTwo & 0x0000ff00) >> 8);
			    CAN0IDAR7 = (UINT8)((idAcceptTwo & 0x000000ff));
			    return E_OK;

			}
			return E_FAIL;
			
			
#if CANMaxChannels > 2			
		case 1:
			if(CAN1CTL1_INITAK == 0x01)
			{
			    CAN1IDAR0 = (UINT8)((idAcceptOne & 0xff000000) >> 24);
			    CAN1IDAR1 = (UINT8)((idAcceptOne & 0x00ff0000) >> 16);
			    CAN1IDAR2 = (UINT8)((idAcceptOne & 0x0000ff00) >> 8);
			    CAN1IDAR3 = (UINT8)((idAcceptOne & 0x000000ff));
			    
			    CAN1IDAR4 = (UINT8)((idAcceptTwo & 0xff000000) >> 24);
			    CAN1IDAR5 = (UINT8)((idAcceptTwo & 0x00ff0000) >> 16);
			    CAN1IDAR6 = (UINT8)((idAcceptTwo & 0x0000ff00) >> 8);
			    CAN1IDAR7 = (UINT8)((idAcceptTwo & 0x000000ff));
			    return E_OK;

			}
			return E_FAIL;
#endif

#if CANMaxChannels > 3			
		case 2:
			if(CAN2CTL1_INITAK == 0x01)
			{
			    CAN2IDAR0 = (UINT8)((idAcceptOne & 0xff000000) >> 24);
			    CAN2IDAR1 = (UINT8)((idAcceptOne & 0x00ff0000) >> 16);
			    CAN2IDAR2 = (UINT8)((idAcceptOne & 0x0000ff00) >> 8);
			    CAN2IDAR3 = (UINT8)((idAcceptOne & 0x000000ff));
			    
			    CAN2IDAR4 = (UINT8)((idAcceptTwo & 0xff000000) >> 24);
			    CAN2IDAR5 = (UINT8)((idAcceptTwo & 0x00ff0000) >> 16);
			    CAN2IDAR6 = (UINT8)((idAcceptTwo & 0x0000ff00) >> 8);
			    CAN2IDAR7 = (UINT8)((idAcceptTwo & 0x000000ff));
			    return E_OK;

			}
			return E_FAIL;
#endif

#if CANMaxChannels > 4			
		case 3:
			if(CAN3CTL1_INITAK == 0x01)
			{
			    CAN3IDAR0 = (UINT8)((idAcceptOne & 0xff000000) >> 24);
			    CAN3IDAR1 = (UINT8)((idAcceptOne & 0x00ff0000) >> 16);
			    CAN3IDAR2 = (UINT8)((idAcceptOne & 0x0000ff00) >> 8);
			    CAN3IDAR3 = (UINT8)((idAcceptOne & 0x000000ff));
			    
			    CAN3IDAR4 = (UINT8)((idAcceptTwo & 0xff000000) >> 24);
			    CAN3IDAR5 = (UINT8)((idAcceptTwo & 0x00ff0000) >> 16);
			    CAN3IDAR6 = (UINT8)((idAcceptTwo & 0x0000ff00) >> 8);
			    CAN3IDAR7 = (UINT8)((idAcceptTwo & 0x000000ff));
			    return E_OK;

			}
			return E_FAIL;
#endif

#if CANMaxChannels > 1			
		case 4:
			if(CAN4CTL1_INITAK == 0x01)
			{
			    CAN4IDAR0 = (UINT8)((idAcceptOne & 0xff000000) >> 24);
			    CAN4IDAR1 = (UINT8)((idAcceptOne & 0x00ff0000) >> 16);
			    CAN4IDAR2 = (UINT8)((idAcceptOne & 0x0000ff00) >> 8);
			    CAN4IDAR3 = (UINT8)((idAcceptOne & 0x000000ff));
			    
			    CAN4IDAR4 = (UINT8)((idAcceptTwo & 0xff000000) >> 24);
			    CAN4IDAR5 = (UINT8)((idAcceptTwo & 0x00ff0000) >> 16);
			    CAN4IDAR6 = (UINT8)((idAcceptTwo & 0x0000ff00) >> 8);
			    CAN4IDAR7 = (UINT8)((idAcceptTwo & 0x000000ff));
			    return E_OK;

			}
			return E_FAIL;
#endif

		default:
			return E_FAIL; 
	}
			
}


////////////////////////////////////////////////////////////////////////////////
//  ��������	MCU_CAN_SetRxMask
//  ����������	�趨CANģ�����ʱɸѡ��ID����
//  
//  ����ֵ������E_OK	    �趨�ɹ�	
//				E_FAIL      ���������δ�����ʼ��ģʽ���趨ʧ��
//			
//  ����������	channel 	CAN��ͨ����
//			   	maskOne		��һ��(��CANIDMR0~CANIDMR3)����ֵ
//				idAcceptTwo �ڶ���(��CANIDMR4~CANIDMR7)����ֵ	
//  
//  �޸���ʷ��
//  �汾        ������Ա        ����            �޸�˵��
//  --------    --------------  ------------    --------------------------------
//  1.0.0       ������        	2009-07-11      ��������
////////////////////////////////////////////////////////////////////////////////
ERROR MCU_CAN_SetRxMask(UINT8 channel, UINT32 maskOne, UINT32 maskTwo)
{
	switch(channel)
	{
		case 0:
			if(CAN0CTL1_INITAK == 0x01)
			{
			    CAN0IDMR0 = (UINT8)((maskOne & 0xff000000) >> 24);
			    CAN0IDMR1 = (UINT8)((maskOne & 0x00ff0000) >> 16);
			    CAN0IDMR2 = (UINT8)((maskOne & 0x0000ff00) >>  8);
			    CAN0IDMR3 = (UINT8)((maskOne & 0x000000ff));
			    
			    CAN0IDMR4 = (UINT8)((maskTwo & 0xff000000) >> 24);
			    CAN0IDMR5 = (UINT8)((maskTwo & 0x00ff0000) >> 16);
			    CAN0IDMR6 = (UINT8)((maskTwo & 0x0000ff00) >>  8);
			    CAN0IDMR7 = (UINT8)((maskTwo & 0x000000ff));
			    
				return E_OK;
			}
			return E_FAIL;
			
			
#if CANMaxChannels > 2			
		case 1:
			if(CAN1CTL1_INITAK == 0x01)
			{
			    CAN1IDMR0 = (UINT8)((maskOne & 0xff000000) >> 24);
			    CAN1IDMR1 = (UINT8)((maskOne & 0x00ff0000) >> 16);
			    CAN1IDMR2 = (UINT8)((maskOne & 0x0000ff00) >>  8);
			    CAN1IDMR3 = (UINT8)((maskOne & 0x000000ff));
			    
			    CAN1IDMR4 = (UINT8)((maskTwo & 0xff000000) >> 24);
			    CAN1IDMR5 = (UINT8)((maskTwo & 0x00ff0000) >> 16);
			    CAN1IDMR6 = (UINT8)((maskTwo & 0x0000ff00) >>  8);
			    CAN1IDMR7 = (UINT8)((maskTwo & 0x000000ff));
			    
				return E_OK;
			}
			return E_FAIL;
#endif

#if CANMaxChannels > 3			
		case 2:
			if(CAN2CTL1_INITAK == 0x01)
			{
			    CAN2IDMR0 = (UINT8)((maskOne & 0xff000000) >> 24);
			    CAN2IDMR1 = (UINT8)((maskOne & 0x00ff0000) >> 16);
			    CAN2IDMR2 = (UINT8)((maskOne & 0x0000ff00) >>  8);
			    CAN2IDMR3 = (UINT8)((maskOne & 0x000000ff));
			    
			    CAN2IDMR4 = (UINT8)((maskTwo & 0xff000000) >> 24);
			    CAN2IDMR5 = (UINT8)((maskTwo & 0x00ff0000) >> 16);
			    CAN2IDMR6 = (UINT8)((maskTwo & 0x0000ff00) >>  8);
			    CAN2IDMR7 = (UINT8)((maskTwo & 0x000000ff));
			    
				return E_OK;
			}
			return E_FAIL;
#endif

#if CANMaxChannels > 4			
		case 3:
			if(CAN3CTL1_INITAK == 0x01)
			{
			    CAN3IDMR0 = (UINT8)((maskOne & 0xff000000) >> 24);
			    CAN3IDMR1 = (UINT8)((maskOne & 0x00ff0000) >> 16);
			    CAN3IDMR2 = (UINT8)((maskOne & 0x0000ff00) >>  8);
			    CAN3IDMR3 = (UINT8)((maskOne & 0x000000ff));
			    
			    CAN3IDMR4 = (UINT8)((maskTwo & 0xff000000) >> 24);
			    CAN3IDMR5 = (UINT8)((maskTwo & 0x00ff0000) >> 16);
			    CAN3IDMR6 = (UINT8)((maskTwo & 0x0000ff00) >>  8);
			    CAN3IDMR7 = (UINT8)((maskTwo & 0x000000ff));
			    
				return E_OK;
			}
			return E_FAIL;
#endif

#if CANMaxChannels > 1			
		case 4:
			if(CAN4CTL1_INITAK == 0x01)
			{
			    CAN4IDMR0 = (UINT8)((maskOne & 0xff000000) >> 24);
			    CAN4IDMR1 = (UINT8)((maskOne & 0x00ff0000) >> 16);
			    CAN4IDMR2 = (UINT8)((maskOne & 0x0000ff00) >>  8);
			    CAN4IDMR3 = (UINT8)((maskOne & 0x000000ff));
			    
			    CAN4IDMR4 = (UINT8)((maskTwo & 0xff000000) >> 24);
			    CAN4IDMR5 = (UINT8)((maskTwo & 0x00ff0000) >> 16);
			    CAN4IDMR6 = (UINT8)((maskTwo & 0x0000ff00) >>  8);
			    CAN4IDMR7 = (UINT8)((maskTwo & 0x000000ff));
			    
				return E_OK;
			}
			return E_FAIL;
#endif

		default:
			return E_FAIL;
			
	}
}



////////////////////////////////////////////////////////////////////////////////
//  ��������	MCU_CAN_CheckOutInitMode
//  ����������	�趨CANģ���˳���ʼ��ģʽ
//  
//  ����ֵ������E_OK		�ɹ��˳���ʼ��״̬��CANģ����������
//				E_FAIL      ��ʱ���˳���ʼ��״̬ʧ��
//  ����������	channel		ͨ��
//			
//  
//  �޸���ʷ��
//  �汾        ������Ա        ����            �޸�˵��
//  --------    --------------  ------------    --------------------------------
//  1.0.0       ������        	2009-07-11      ��������
////////////////////////////////////////////////////////////////////////////////
ERROR MCU_CAN_CheckOutInitMode(UINT8 channel)
{
	UINT16 l_CheckOutModeRetry;
	
	switch(channel)
	{
		case 0:
			CAN0CTL1_INITAK = 0;
			CAN0CTL0_INITRQ = 0;
			      
			for(l_CheckOutModeRetry = 0; l_CheckOutModeRetry < 60000; l_CheckOutModeRetry++)
			{
				
				if((CAN0CTL1_INITAK == 0x00) && (CAN0CTL0_INITRQ == 0))
				{
					CAN0RIER_RXFIE			= 0x01;		//��������жϣ��������ģʽ��ִ��
					return E_OK;					
				}
			}
			return E_FAIL;
			
#if CANMaxChannels > 2			
		case 1:
			CAN1CTL1_INITAK = 0;
			CAN1CTL0_INITRQ = 0;
			      
			for(l_CheckOutModeRetry = 0; l_CheckOutModeRetry < 60000; l_CheckOutModeRetry++)
			{
				
				if((CAN1CTL1_INITAK == 0x00) && (CAN1CTL0_INITRQ == 0))
				{
					CAN1RIER_RXFIE			= 0x01;		//��������жϣ��������ģʽ��ִ��
					return E_OK;					
				}
			}
			return E_FAIL;
#endif

#if CANMaxChannels > 3			
		case 2:
			CAN2CTL1_INITAK = 0;
			CAN2CTL0_INITRQ = 0;
			      
			for(l_CheckOutModeRetry = 0; l_CheckOutModeRetry < 60000; l_CheckOutModeRetry++)
			{
				
				if((CAN2CTL1_INITAK == 0x00) && (CAN2CTL0_INITRQ == 0))
				{
					CAN2RIER_RXFIE			= 0x01;		//��������жϣ��������ģʽ��ִ��
					return E_OK;					
				}
			}
			return E_FAIL;
#endif

#if CANMaxChannels > 4			
		case 3:
			CAN3CTL1_INITAK = 0;
			CAN3CTL0_INITRQ = 0;
			      
			for(l_CheckOutModeRetry = 0; l_CheckOutModeRetry < 60000; l_CheckOutModeRetry++)
			{
				
				if((CAN3CTL1_INITAK == 0x00) && (CAN3CTL0_INITRQ == 0))
				{
					CAN3RIER_RXFIE			= 0x01;		//��������жϣ��������ģʽ��ִ��
					return E_OK;					
				}
			}
			return E_FAIL;
#endif

#if CANMaxChannels > 1			
		case 4:
			CAN4CTL1_INITAK = 0;
			CAN4CTL0_INITRQ = 0;
			      
			for(l_CheckOutModeRetry = 0; l_CheckOutModeRetry < 60000; l_CheckOutModeRetry++)
			{
				
				if((CAN4CTL1_INITAK == 0x00) && (CAN4CTL0_INITRQ == 0))
				{
					CAN4RIER_RXFIE			= 0x01;		//��������жϣ��������ģʽ��ִ��
					return E_OK;					
				}
			}
			return E_FAIL;
#endif

		default:
			return E_FAIL;
			
	}
	
}




////////////////////////////////////////////////////////////////////////////////
//  ��������	MCU_CAN_Transmit
//  ����������	����CAN����
//  
//  ����ֵ������E_OK	    ���ͳɹ������ǳɹ����뷢�ͻ��������ȴ�����������	
//				E_FULL		����������������ʧ��
//			
//  ����������	channel 	CAN��ͨ����
//			   	msgSend		�����͵�����
//  
//  �޸���ʷ��
//  �汾        ������Ա        ����            �޸�˵��
//  --------    --------------  ------------    --------------------------------
//  1.0.0       ������        	2009-07-11      ��������
////////////////////////////////////////////////////////////////////////////////
ERROR MCU_CAN_Transmit(UINT8 channel, MCU_CAN_MSG msgSend)
{
	UINT8 	l_ChoseBuff;
	UINT8 	l_TxIndex;
	switch(channel)
	{
		case 0:
			if((CAN0TFLG_TXE & 0x03) == 0)
			{
				return E_FULL;
			}
	    	CAN0TBSEL	= CAN0TFLG;     
		    l_ChoseBuff = CAN0TBSEL;
		    
			CAN0TXIDR0	= (UINT8)((msgSend.ID & 0xff000000)>>24);    //���ı�ʾ����
			CAN0TXIDR1	= (UINT8)((msgSend.ID & 0x00ff0000)>>16);    
			CAN0TXIDR2	= (UINT8)((msgSend.ID & 0x0000ff00)>>8); 
			CAN0TXIDR3	= (UINT8)((msgSend.ID & 0x000000ff)); 
		
			if(msgSend.DataLength != 0)
			{
				for(l_TxIndex = 0; l_TxIndex < msgSend.DataLength; l_TxIndex++)
				{
					 *((&CAN0TXDSR0) + l_TxIndex) = msgSend.Data[l_TxIndex];
				}
			}
			
		    CAN0TXDLR	= msgSend.DataLength;        //���ĳ��ȣ�����������Ϊ8�������ֽ�
		    CAN0TFLG	= l_ChoseBuff; 
			return E_OK;
			
						
#if CANMaxChannels > 2			
		case 1:
			if((CAN1TFLG_TXE & 0x03) == 0)
			{
				return E_FULL;
			}
	    	CAN1TBSEL	= CAN1TFLG;     
		    l_ChoseBuff = CAN1TBSEL;
		    
			CAN1TXIDR0	= (UINT8)((msgSend.ID & 0xff000000)>>24);    //���ı�ʾ����
			CAN1TXIDR1	= (UINT8)((msgSend.ID & 0x00ff0000)>>16);    
			CAN1TXIDR2	= (UINT8)((msgSend.ID & 0x0000ff00)>>8); 
			CAN1TXIDR3	= (UINT8)((msgSend.ID & 0x000000ff)); 
		
			if(msgSend.DataLength != 0)
			{
				for(l_TxIndex = 0; l_TxIndex < msgSend.DataLength; l_TxIndex++)
				{
					 *((&CAN1TXDSR0) + l_TxIndex) = msgSend.Data[l_TxIndex];
				}
			}
			
		    CAN1TXDLR	= msgSend.DataLength;        //���ĳ��ȣ�����������Ϊ8�������ֽ�
		    CAN1TFLG	= l_ChoseBuff; 
			return E_OK;
#endif

#if CANMaxChannels > 3			
		case 2:
			if((CAN2TFLG_TXE & 0x03) == 0)
			{
				return E_FULL;
			}
	    	CAN2TBSEL	= CAN2TFLG;     
		    l_ChoseBuff = CAN2TBSEL;
		    
			CAN2TXIDR0	= (UINT8)((msgSend.ID & 0xff000000)>>24);    //���ı�ʾ����
			CAN2TXIDR1	= (UINT8)((msgSend.ID & 0x00ff0000)>>16);    
			CAN2TXIDR2	= (UINT8)((msgSend.ID & 0x0000ff00)>>8); 
			CAN2TXIDR3	= (UINT8)((msgSend.ID & 0x000000ff)); 
		
			if(msgSend.DataLength != 0)
			{
				for(l_TxIndex = 0; l_TxIndex < msgSend.DataLength; l_TxIndex++)
				{
					 *((&CAN2TXDSR0) + l_TxIndex) = msgSend.Data[l_TxIndex];
				}
			}
			
		    CAN2TXDLR	= msgSend.DataLength;        //���ĳ��ȣ�����������Ϊ8�������ֽ�
		    CAN2TFLG	= l_ChoseBuff; 
			return E_OK;
#endif

#if CANMaxChannels > 4			
		case 3:
			if((CAN3TFLG_TXE & 0x03) == 0)
			{
				return E_FULL;
			}
	    	CAN3TBSEL	= CAN3TFLG;     
		    l_ChoseBuff = CAN3TBSEL;
		    
			CAN3TXIDR0	= (UINT8)((msgSend.ID & 0xff000000)>>24);    //���ı�ʾ����
			CAN3TXIDR1	= (UINT8)((msgSend.ID & 0x00ff0000)>>16);    
			CAN3TXIDR2	= (UINT8)((msgSend.ID & 0x0000ff00)>>8); 
			CAN3TXIDR3	= (UINT8)((msgSend.ID & 0x000000ff)); 
		
			if(msgSend.DataLength != 0)
			{
				for(l_TxIndex = 0; l_TxIndex < msgSend.DataLength; l_TxIndex++)
				{
					 *((&CAN3TXDSR0) + l_TxIndex) = msgSend.Data[l_TxIndex];
				}
			}
			
		    CAN3TXDLR	= msgSend.DataLength;        //���ĳ��ȣ�����������Ϊ8�������ֽ�
		    CAN3TFLG	= l_ChoseBuff; 
			return E_OK;
#endif

#if CANMaxChannels > 1			
		case 4:
			if((CAN4TFLG_TXE & 0x03) == 0)
			{
				return E_FULL;
			}
	    	CAN4TBSEL	= CAN4TFLG;     
		    l_ChoseBuff = CAN4TBSEL;
		    
			CAN4TXIDR0	= (UINT8)((msgSend.ID & 0x1fE00000)>>21);    //���ı�ʾ����
			CAN4TXIDR1	= ((UINT8)(((msgSend.ID & 0x001C0000)>>18)<<5))|(0x18)|((uint8)((msgSend.ID & 0x00038000)>>15));    
			CAN4TXIDR2	= (UINT8)((msgSend.ID & 0x00007f80)>>7); 
			CAN4TXIDR3	= (UINT8)((msgSend.ID & 0x0000007f)<<1); 
		
			if(msgSend.DataLength != 0)
			{
				for(l_TxIndex = 0; l_TxIndex < msgSend.DataLength; l_TxIndex++)
				{
					 *((&CAN4TXDSR0) + l_TxIndex) = msgSend.Data[l_TxIndex];
				}
			}
			
		    CAN4TXDLR	= msgSend.DataLength;        //���ĳ��ȣ�����������Ϊ8�������ֽ�
		    CAN4TFLG	= l_ChoseBuff; 
			return E_OK;
#endif

		default:
			return E_FAIL;
			
	}
}





////////////////////////////////////////////////////////////////////////////////
//  ��������	MCU_CAN_Receive
//  ����������	��ȡ���յ�������
//  
//  ����ֵ������E_OK	    ���ճɹ�	
//				E_FAIL      ͨ�������󣬶�ȡʧ��
//				E_EMPTY		û�н��յ����ݣ����ջ�����Ϊ��
//  ����������
//			
//  
//  �޸���ʷ��
//  �汾        ������Ա        ����            �޸�˵��
//  --------    --------------  ------------    --------------------------------
//  1.0.0       ������        	2009-07-11      ��������
////////////////////////////////////////////////////////////////////////////////
ERROR MCU_CAN_Receive(UINT8 channel, MCU_CAN_MSG * msgRec)
{
    return MCU_CAN_GetRxData(channel, msgRec);	//
}



////////////////////////////////////////////////////////////////////////////////
//  ��������	MCU_CAN_IsReceived
//  ����������	�ж�CANģ�����޽��յ�����
//  
//  ����ֵ������E_OK		���ջ������н��յ�������
//				E_FAIL      û�нӵ����ݻ�ͨ������
//  ���������� 	channel		ͨ��
//			
//  
//  �޸���ʷ��
//  �汾        ������Ա        ����            �޸�˵��
//  --------    --------------  ------------    --------------------------------
//  1.0.0       ������        	2009-07-11      ��������
////////////////////////////////////////////////////////////////////////////////
ERROR MCU_CAN_IsReceived(UINT8 channel)
{
	if(c_MCU_CAN_ChannelMapping[channel] >= CANMaxChannels)
	{
		return E_FAIL;
	}

    if((g_MCU_CAN_RXFull[c_MCU_CAN_ChannelMapping[channel]] == FALSE) && (g_MCU_CAN_RXFront[c_MCU_CAN_ChannelMapping[channel]] == g_MCU_CAN_RXRear[c_MCU_CAN_ChannelMapping[channel]]))
	{
	    return E_FAIL;                              //���в�������ͷβ��ȣ���û�����ݽ��յ�
	}
	else
	{
	    return E_OK;                                //���յ�����
	};	
	
}









////////////////////////////////////////////////////////////////////////////////
//  ��������	MCU_CAN_AddRxData
//  ����������	��CAN���ݴ�����ջ�����
//  
//  ����ֵ������E_OK	    �ɹ����뻺����
//				E_FAIL      �������󣬴���ʧ��
//				E_FULL		����������������ʧ��
//			
//  ����������	channel 	CAN��ͨ����
//			   	data		�����������
//  
//  �޸���ʷ��
//  �汾        ������Ա        ����            �޸�˵��
//  --------    --------------  ------------    --------------------------------
//  1.0.0       ������        	2009-07-11      ��������
////////////////////////////////////////////////////////////////////////////////

ERROR MCU_CAN_AddRxData(UINT8 channel, MCU_CAN_MSG data)
{
	if(c_MCU_CAN_ChannelMapping[channel] >= CANMaxChannels)
	{
		return E_FAIL;
	}
	if(g_MCU_CAN_RXFull[c_MCU_CAN_ChannelMapping[channel]] == TRUE)
	{
        return E_FULL;                                  //��������
	}
    g_MCU_CAN_RXBuff[c_MCU_CAN_ChannelMapping[channel]][g_MCU_CAN_RXRear[c_MCU_CAN_ChannelMapping[channel]]] = data;   //��������
    
    g_MCU_CAN_RXRear[c_MCU_CAN_ChannelMapping[channel]]++;
    if(g_MCU_CAN_RXRear[c_MCU_CAN_ChannelMapping[channel]] >= CANBuffMaxLength)
    {
     	g_MCU_CAN_RXRear[c_MCU_CAN_ChannelMapping[channel]] = 0;
    }

    if(g_MCU_CAN_RXRear[c_MCU_CAN_ChannelMapping[channel]] == g_MCU_CAN_RXFront[c_MCU_CAN_ChannelMapping[channel]])
    {
        g_MCU_CAN_RXFull[c_MCU_CAN_ChannelMapping[channel]] = TRUE;                     //������
    };
    
    return E_OK;
}

 


////////////////////////////////////////////////////////////////////////////////
//  ��������	MCU_CAN_GetRxData
//  ����������	��ȡ���յ�������
//  
//  ����ֵ������E_OK	    ���ճɹ�	
//				E_FAIL      ͨ�������󣬶�ȡʧ��
//				E_EMPTY		û�н��յ����ݣ����ջ�����Ϊ��
//  ����������	channel		CANͨ����
//			  	data		ָ����ȡ�������ݵ�ָ��	
//  
//  �޸���ʷ��
//  �汾        ������Ա        ����            �޸�˵��
//  --------    --------------  ------------    --------------------------------
//  1.0.0       ������        	2009-07-11      ��������
////////////////////////////////////////////////////////////////////////////////

ERROR MCU_CAN_GetRxData(UINT8 channel, MCU_CAN_MSG * data)
{
	if(c_MCU_CAN_ChannelMapping[channel] >= CANMaxChannels)
	{
		return E_FAIL;
	}
	if((g_MCU_CAN_RXFull[c_MCU_CAN_ChannelMapping[channel]] == FALSE) && (g_MCU_CAN_RXRear[c_MCU_CAN_ChannelMapping[channel]] == g_MCU_CAN_RXFront[c_MCU_CAN_ChannelMapping[channel]]))
	{
        return E_EMPTY;                                  //�����ѿ�
	}
	*data = g_MCU_CAN_RXBuff[c_MCU_CAN_ChannelMapping[channel]][g_MCU_CAN_RXFront[c_MCU_CAN_ChannelMapping[channel]]];//���ж�
    
    g_MCU_CAN_RXFront[c_MCU_CAN_ChannelMapping[channel]]++;
    if(g_MCU_CAN_RXFront[c_MCU_CAN_ChannelMapping[channel]] >= CANBuffMaxLength)
    {
     	g_MCU_CAN_RXFront[c_MCU_CAN_ChannelMapping[channel]] = 0;
    }

    g_MCU_CAN_RXFull[c_MCU_CAN_ChannelMapping[channel]] = FALSE;                     //����δ��
    
    return E_OK;
}
 
/*
////////////////////////////////////////////////////////////////////////////////
//  ��������	MCU_CAN_Ch0RXInt
//  ����������	CAN0�Ľ����ж�
//  
//  ����ֵ��������	
//			
//  ����������	��
//			  	
//  
//  �޸���ʷ��
//  �汾        ������Ա        ����            �޸�˵��
//  --------    --------------  ------------    --------------------------------
//  1.0.0       ������        	2009-07-11      ��������
//  1.0.1       ��ҫ��          2011-03-30      ����ID��ȡ
////////////////////////////////////////////////////////////////////////////////
#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt VectorNumber_Vcan0rx void MCU_CAN_Ch0RXInt(void)
{
	MCU_CAN_MSG l_RxData={{0,0,0,0,0,0,0,0},0,0};
	UINT8		l_RxIndex;
	l_RxData.DataLength	= CAN0RXDLR_DLC; //���־λǰ��ȡ���ݣ��������ݻ����
	
	l_RxData.ID |= (UINT32)CAN0RXIDR0<<24;
	l_RxData.ID |= (UINT32)CAN0RXIDR1<<16;
	l_RxData.ID |= (UINT32)CAN0RXIDR2<<8;
	l_RxData.ID |= (UINT32)CAN0RXIDR3;
	for(l_RxIndex = 0; l_RxIndex < l_RxData.DataLength; l_RxIndex++)
	{
		l_RxData.Data[l_RxIndex]	= *((&CAN0RXDSR0) + l_RxIndex);	
	}
	(void)MCU_CAN_AddRxData(0,l_RxData);
    CAN0RFLG    = 0x01;
}
#pragma CODE_SEG DEFAULT
 */



#if CANMaxChannels > 2			
////////////////////////////////////////////////////////////////////////////////
//  ��������	MCU_CAN_Ch1RXInt
//  ����������	CAN1�Ľ����ж�
//  
//  ����ֵ��������	
//			
//  ����������	��
//			  	
//  
//  �޸���ʷ��
//  �汾        ������Ա        ����            �޸�˵��
//  --------    --------------  ------------    --------------------------------
//  1.0.0       ������        	2009-07-11      ��������
//  1.0.1       ��ҫ��          2011-03-30      ����ID��ȡ
////////////////////////////////////////////////////////////////////////////////
#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt VectorNumber_Vcan1rx void MCU_CAN_Ch1RXInt(void)
{
	MCU_CAN_MSG l_RxData={{0,0,0,0,0,0,0,0},0,0};
	UINT8		l_RxIndex;
	l_RxData.DataLength	= CAN1RXDLR_DLC; //���־λǰ��ȡ���ݣ��������ݻ����
	
	l_RxData.ID |= (UINT32)CAN1RXIDR0<<24;
	l_RxData.ID |= (UINT32)CAN1RXIDR1<<16;
	l_RxData.ID |= (UINT32)CAN1RXIDR2<<8;
	l_RxData.ID |= (UINT32)CAN1RXIDR3;
	for(l_RxIndex = 0; l_RxIndex < l_RxData.DataLength; l_RxIndex++)
	{
		l_RxData.Data[l_RxIndex]	= *((&CAN1RXDSR0) + l_RxIndex);	
	}
	(void)MCU_CAN_AddRxData(1,l_RxData);
    CAN1RFLG    = 0x01;
}
#pragma CODE_SEG DEFAULT
#endif

#if CANMaxChannels > 3			
////////////////////////////////////////////////////////////////////////////////
//  ��������	MCU_CAN_Ch2RXInt
//  ����������	CAN2�Ľ����ж�
//  
//  ����ֵ��������	
//			
//  ����������	��
//			  	
//  
//  �޸���ʷ��
//  �汾        ������Ա        ����            �޸�˵��
//  --------    --------------  ------------    --------------------------------
//  1.0.0       ������        	2009-07-11      ��������
//  1.0.1       ��ҫ��          2011-03-30      ����ID��ȡ
////////////////////////////////////////////////////////////////////////////////
#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt VectorNumber_Vcan2rx void MCU_CAN_Ch2RXInt(void)
{
	MCU_CAN_MSG l_RxData={{0,0,0,0,0,0,0,0},0,0};
	UINT8		l_RxIndex;
	l_RxData.DataLength	= CAN2RXDLR_DLC; //���־λǰ��ȡ���ݣ��������ݻ����
	
	l_RxData.ID |= (UINT32)CAN2RXIDR0<<24;
	l_RxData.ID |= (UINT32)CAN2RXIDR1<<16;
	l_RxData.ID |= (UINT32)CAN2RXIDR2<<8;
	l_RxData.ID |= (UINT32)CAN2RXIDR3;
	for(l_RxIndex = 0; l_RxIndex < l_RxData.DataLength; l_RxIndex++)
	{
		l_RxData.Data[l_RxIndex]	= *((&CAN2RXDSR0) + l_RxIndex);	
	}
	(void)MCU_CAN_AddRxData(2,l_RxData);
    CAN2RFLG    = 0x01;
}
#pragma CODE_SEG DEFAULT
#endif


#if CANMaxChannels > 4			
////////////////////////////////////////////////////////////////////////////////
//  ��������	MCU_CAN_Ch3RXInt
//  ����������	CAN3�Ľ����ж�
//  
//  ����ֵ��������	
//			
//  ����������	��
//			  	
//  
//  �޸���ʷ��
//  �汾        ������Ա        ����            �޸�˵��
//  --------    --------------  ------------    --------------------------------
//  1.0.0       ������        	2009-07-11      ��������
//  1.0.1       ��ҫ��          2011-03-30      ����ID��ȡ
////////////////////////////////////////////////////////////////////////////////
#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt VectorNumber_Vcan3rx void MCU_CAN_Ch3RXInt(void)
{
	MCU_CAN_MSG l_RxData={{0,0,0,0,0,0,0,0},0,0};
	UINT8		l_RxIndex;
	l_RxData.DataLength	= CAN3RXDLR_DLC; //���־λǰ��ȡ���ݣ��������ݻ����
	
	l_RxData.ID |= (UINT32)CAN3RXIDR0<<24;
	l_RxData.ID |= (UINT32)CAN3RXIDR1<<16;
	l_RxData.ID |= (UINT32)CAN3RXIDR2<<8;
	l_RxData.ID |= (UINT32)CAN3RXIDR3;
	for(l_RxIndex = 0; l_RxIndex < l_RxData.DataLength; l_RxIndex++)
	{
		l_RxData.Data[l_RxIndex]	= *((&CAN3RXDSR0) + l_RxIndex);	
	}
	(void)MCU_CAN_AddRxData(3,l_RxData);
    CAN3RFLG    = 0x01;
}
#pragma CODE_SEG DEFAULT
#endif



#if CANMaxChannels > 1			
////////////////////////////////////////////////////////////////////////////////
//  ��������	MCU_CAN_Ch4RXInt
//  ����������	CAN4�Ľ����ж�
//  
//  ����ֵ��������	
//			
//  ����������	��
//			  	
//  
//  �޸���ʷ��
//  �汾        ������Ա        ����            �޸�˵��
//  --------    --------------  ------------    --------------------------------
//  1.0.0       ������        	2009-07-11      ��������
//  1.0.1       ��ҫ��          2011-03-30      ����ID��ȡ
////////////////////////////////////////////////////////////////////////////////
#pragma CODE_SEG __NEAR_SEG NON_BANKED
interrupt VectorNumber_Vcan4rx void MCU_CAN_Ch4RXInt(void)
{
	MCU_CAN_MSG l_RxData={{0,0,0,0,0,0,0,0},0,0};
	UINT8		l_RxIndex;
	l_RxData.DataLength	= CAN4RXDLR_DLC; //���־λǰ��ȡ���ݣ��������ݻ����
	
	l_RxData.ID |= (UINT32)CAN4RXIDR0<<24;
	l_RxData.ID |= (UINT32)CAN4RXIDR1<<16;
	l_RxData.ID |= (UINT32)CAN4RXIDR2<<8;
	l_RxData.ID |= (UINT32)CAN4RXIDR3;
	for(l_RxIndex = 0; l_RxIndex < l_RxData.DataLength; l_RxIndex++)
	{
		l_RxData.Data[l_RxIndex]	= *((&CAN4RXDSR0) + l_RxIndex);	
	}
	(void)MCU_CAN_AddRxData(4,l_RxData);
    CAN4RFLG    = 0x01;
}
#pragma CODE_SEG DEFAULT
#endif