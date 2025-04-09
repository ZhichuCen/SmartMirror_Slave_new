////////////////////////////////////////////////////////////////////////////////
//  �ļ�����    MCU.h
//  ����������  MCU��ʼ��ʱ�ӵȵ�ʵ���ļ�
//  ���룺      CodeWarrior for HCS12(X) V4.7 
//  
//  �޸���ʷ��
//  �汾        ������Ա        ����            �޸�˵��
//  --------    --------------  ------------    --------------------------------
//  1.0.0       ������          2009-08-01      �����ļ�
////////////////////////////////////////////////////////////////////////////////
#include "MCU.h"
#include "Platform_Types.h"

const MCU_ConfigType MCU_Config_Par =
{
	CONF_INCLK,     //��Ƭ��������ʱ��
	CONF_OUTCLK,    //��Ƭ��������ʱ��
};




////////////////////////////////////////////////////////////////////////////////
//  ��������	MCU_Init
//  ����������	��ʼ��MCU
//  
//  ����ֵ������void
//				
//  ����������	void	
//  
//  �޸���ʷ��
//  �汾        ������Ա        ����            �޸�˵��
//  --------    --------------  ------------    --------------------------------
//  1.0.0       ������        	2009-08-02      ��������
////////////////////////////////////////////////////////////////////////////////
void MCU_Init(void)
{
    (void)MCU_InitClock(MCU_Config_Par.OutClk);
    
    
       
}




////////////////////////////////////////////////////////////////////////////////
//  ��������	MCU_InitClock
//  ����������	�趨MCU���е�����ʱ��Ƶ��
//  
//  ����ֵ������E_OK			����ʱ���趨��ȷ
//				E_FAIL			����ʱ������ʧ��
//  ����������	clockSetting	����Ŀ��Ƶ�ʣ���HzΪ��λ	
//  
//  �޸���ʷ��
//  �汾        ������Ա        ����            �޸�˵��
//  --------    --------------  ------------    --------------------------------
//  1.0.0       ������        	2009-08-02      ��������
////////////////////////////////////////////////////////////////////////////////
Std_ReturnType MCU_InitClock(MCU_ClockType clockSetting)
{
    UINT16 	l_PLLLockWait;
    UINT8	l_VCOCLK;								//fvco����MΪ��λ 
    
    if(clockSetting > MCU_MAXBUSFREQ)
    {
    	return E_FAIL;
    }
    
    PLLCTL  		= 0x00;							//��ֹPLL
    POSTDIV 		= (UINT8)(((32000000UL / clockSetting) > 16) ? 16 : (32000000UL / clockSetting));
    												//fvco�ô���32M����ˣ�����Ƶ��Сʱ���ô˷�Ƶ

    REFDV_REFDIV	= MCU_Config_Par.InClk / 1000000UL - 1;//fref=fosc/(REFDIV+1)=1MHz
	REFDV_REFFRQ	= 0x00;							//fref=1MHz,��ʱREFFRQ=00
    
    SYNR_SYNDIV		= (clockSetting / 1000000UL) * (POSTDIV * 2 + (POSTDIV == 0)) - 1;	
    	//POSTDIV!=0ʱ,fbus=fpll/2=fvco/(4*POSTDIV)=fosc*(SYNDIV+1)/(2*POSTDIV*(REFDIV+1))
    	//POSTDIV=0ʱ, fbus=fpll/2=fvco/2=fosc*(SYNDIV+1)/(REFDIV+1)
    	
    	
    l_VCOCLK = (UINT8)(2 * (MCU_Config_Par.InClk / 1000000UL) * (SYNR_SYNDIV + 1)  / (REFDV_REFDIV + 1));
    																		//fvco=2*fosc*(SYNDIV+1)/(REFDIV+1)
    
    if ((l_VCOCLK >= 32) && (l_VCOCLK <= 48))
    {
	    SYNR_VCOFRQ	= 0x00;
    }
    else if((l_VCOCLK > 48) && (l_VCOCLK <= 80))
    {
	    SYNR_VCOFRQ	= 0x01;							
    }
    else
    {
	    SYNR_VCOFRQ	= 0x03;							
    }
	    

    PLLCTL  		= 0X5F;                     	//��PLL
    COPCTL  		= 0x00;                     	//��ֹ���Ź�     
    for (l_PLLLockWait = 0; l_PLLLockWait < 60000; l_PLLLockWait++)
    {
    	if (CRGFLG_LOCK == 1)						//�ȴ�Ƶ���ȶ�
    	{
    		CLKSEL  = 0X80;							//fbus=fpll/2
		    //ECLKCTL = 0x00;							//PE7�������ʱ�ӵ�������PE4�������ʱ��
    		return E_OK;							//������ȷ
    	}
    }
    return E_FAIL;									//��ʱ������ʧ��
}



////////////////////////////////////////////////////////////////////////////////
//  ��������	MCU_GetBusClock
//  ����������	��ȡMCU���е�����ʱ��Ƶ��
//  
//  ����ֵ��������Ƭ��������Ƶ�ʣ���HzΪ��λ	
//
//  ����������	void
//  
//  �޸���ʷ��
//  �汾        ������Ա        ����            �޸�˵��
//  --------    --------------  ------------    --------------------------------
//  1.0.0       ������        	2009-08-02      ��������
////////////////////////////////////////////////////////////////////////////////
MCU_ClockType MCU_GetBusClock(void)
{
	if(CLKSEL_PLLSEL==1)//�Ѿ�����fbus=fpll/2;
	{
		return MCU_Config_Par.OutClk;
	}
	else//����ʱ�����Ծ����һ�룬��fbus=fosc/2
	{
		return MCU_Config_Par.InClk;
	}
}



////////////////////////////////////////////////////////////////////////////////
//  ��������	MCU_GetPllStatus
//  ����������	��ȡMCU���໷��״̬
//  
//  ����ֵ������MCU_PLL_LOCKED		���໷�Ѿ�������һ�����ݲΧ��
//				MCU_PLL_UNLOCKED	���໷û������	
//
//  ����������	void
//  
//  �޸���ʷ��
//  �汾        ������Ա        ����            �޸�˵��
//  --------    --------------  ------------    --------------------------------
//  1.0.0       ������        	2009-08-02      ��������
////////////////////////////////////////////////////////////////////////////////
MCU_PllStatusType MCU_GetPllStatus(void)
{
    if(1 == CRGFLG_LOCK)
    {
        return MCU_PLL_LOCKED;
    }
    else
    {
        return MCU_PLL_UNLOCKED;
    }

}




////////////////////////////////////////////////////////////////////////////////
//  ��������	MCU_GetResetRawValue
//  ����������	��ȡ��λ�Ĵ����ı������ڴ�ֻ����λ��Ч
//  
//  ����ֵ��������λ�Ĵ�����Ӧλ
//			
//  ����������	void
//  
//  �޸���ʷ��
//  �汾        ������Ա        ����            �޸�˵��
//  --------    --------------  ------------    --------------------------------
//  1.0.0       ������        	2009-08-02      ��������
////////////////////////////////////////////////////////////////////////////////
MCU_RawResetType MCU_GetResetRawValue(void)
{
    return (CRGFLG & 0x64);   
}



////////////////////////////////////////////////////////////////////////////////
//  ��������	MCU_GetResetReason
//  ����������	��ȡ��Ƭ����λ��ԭ��
//  
//  ����ֵ������MCU_POWER_ON_RESET		���縴λ
//				MCU_LOW_VOL_RESET		�͵�ѹ��λ
//				MCU_ILLEGAL_ADDR_RESET	�Ƿ���ַ��λ
//				MCU_RESET_UNDEFINED		δ���帴λ
//			
//  ����������	void
//  
//  �޸���ʷ��
//  �汾        ������Ա        ����            �޸�˵��
//  --------    --------------  ------------    --------------------------------
//  1.0.0       ������        	2009-08-02      ��������
////////////////////////////////////////////////////////////////////////////////
MCU_ResetType MCU_GetResetReason(void)
{
    if(1 == CRGFLG_PORF)
    {
        return MCU_POWER_ON_RESET;
    }
    else if(1 == CRGFLG_LVRF)
    {
        return MCU_LOW_VOL_RESET; 
    }
    else if(1 == CRGFLG_ILAF)
    {
        return MCU_ILLEGAL_ADDR_RESET; 
    }
    else
    {
        return MCU_RESET_UNDEFINED; 
    }
}

