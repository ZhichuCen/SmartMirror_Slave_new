////////////////////////////////////////////////////////////////////////////////
//  文件名：    MCU.h
//  作用描述：  MCU初始化时钟等的实现文件
//  编译：      CodeWarrior for HCS12(X) V4.7 
//  
//  修改历史：
//  版本        参与人员        日期            修改说明
//  --------    --------------  ------------    --------------------------------
//  1.0.0       黄文卿          2009-08-01      创建文件
////////////////////////////////////////////////////////////////////////////////
#include "MCU.h"
#include "Platform_Types.h"

const MCU_ConfigType MCU_Config_Par =
{
	CONF_INCLK,     //单片机的输入时钟
	CONF_OUTCLK,    //单片机的总线时钟
};




////////////////////////////////////////////////////////////////////////////////
//  函数名：	MCU_Init
//  作用描述：	初始化MCU
//  
//  返回值描述：void
//				
//  参数描述：	void	
//  
//  修改历史：
//  版本        参与人员        日期            修改说明
//  --------    --------------  ------------    --------------------------------
//  1.0.0       黄文卿        	2009-08-02      创建函数
////////////////////////////////////////////////////////////////////////////////
void MCU_Init(void)
{
    (void)MCU_InitClock(MCU_Config_Par.OutClk);
    
    
       
}




////////////////////////////////////////////////////////////////////////////////
//  函数名：	MCU_InitClock
//  作用描述：	设定MCU运行的总线时钟频率
//  
//  返回值描述：E_OK			总线时钟设定正确
//				E_FAIL			总线时钟设置失败
//  参数描述：	clockSetting	总线目标频率，以Hz为单位	
//  
//  修改历史：
//  版本        参与人员        日期            修改说明
//  --------    --------------  ------------    --------------------------------
//  1.0.0       黄文卿        	2009-08-02      创建函数
////////////////////////////////////////////////////////////////////////////////
Std_ReturnType MCU_InitClock(MCU_ClockType clockSetting)
{
    UINT16 	l_PLLLockWait;
    UINT8	l_VCOCLK;								//fvco，以M为单位 
    
    if(clockSetting > MCU_MAXBUSFREQ)
    {
    	return E_FAIL;
    }
    
    PLLCTL  		= 0x00;							//禁止PLL
    POSTDIV 		= (UINT8)(((32000000UL / clockSetting) > 16) ? 16 : (32000000UL / clockSetting));
    												//fvco得大于32M，因此，总线频率小时得用此分频

    REFDV_REFDIV	= MCU_Config_Par.InClk / 1000000UL - 1;//fref=fosc/(REFDIV+1)=1MHz
	REFDV_REFFRQ	= 0x00;							//fref=1MHz,此时REFFRQ=00
    
    SYNR_SYNDIV		= (clockSetting / 1000000UL) * (POSTDIV * 2 + (POSTDIV == 0)) - 1;	
    	//POSTDIV!=0时,fbus=fpll/2=fvco/(4*POSTDIV)=fosc*(SYNDIV+1)/(2*POSTDIV*(REFDIV+1))
    	//POSTDIV=0时, fbus=fpll/2=fvco/2=fosc*(SYNDIV+1)/(REFDIV+1)
    	
    	
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
	    

    PLLCTL  		= 0X5F;                     	//打开PLL
    COPCTL  		= 0x00;                     	//禁止看门狗     
    for (l_PLLLockWait = 0; l_PLLLockWait < 60000; l_PLLLockWait++)
    {
    	if (CRGFLG_LOCK == 1)						//等待频率稳定
    	{
    		CLKSEL  = 0X80;							//fbus=fpll/2
		    //ECLKCTL = 0x00;							//PE7输出总线时钟的两倍，PE4输出总线时钟
    		return E_OK;							//设置正确
    	}
    }
    return E_FAIL;									//超时，设置失败
}



////////////////////////////////////////////////////////////////////////////////
//  函数名：	MCU_GetBusClock
//  作用描述：	获取MCU运行的总线时钟频率
//  
//  返回值描述：单片机的总线频率，以Hz为单位	
//
//  参数描述：	void
//  
//  修改历史：
//  版本        参与人员        日期            修改说明
//  --------    --------------  ------------    --------------------------------
//  1.0.0       黄文卿        	2009-08-02      创建函数
////////////////////////////////////////////////////////////////////////////////
MCU_ClockType MCU_GetBusClock(void)
{
	if(CLKSEL_PLLSEL==1)//已经设置fbus=fpll/2;
	{
		return MCU_Config_Par.OutClk;
	}
	else//总线时钟来自晶振的一半，即fbus=fosc/2
	{
		return MCU_Config_Par.InClk;
	}
}



////////////////////////////////////////////////////////////////////////////////
//  函数名：	MCU_GetPllStatus
//  作用描述：	获取MCU锁相环的状态
//  
//  返回值描述：MCU_PLL_LOCKED		锁相环已经锁定在一定的容差范围内
//				MCU_PLL_UNLOCKED	锁相环没有锁定	
//
//  参数描述：	void
//  
//  修改历史：
//  版本        参与人员        日期            修改说明
//  --------    --------------  ------------    --------------------------------
//  1.0.0       黄文卿        	2009-08-02      创建函数
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
//  函数名：	MCU_GetResetRawValue
//  作用描述：	获取复位寄存器的变量，在此只有三位有效
//  
//  返回值描述：复位寄存器相应位
//			
//  参数描述：	void
//  
//  修改历史：
//  版本        参与人员        日期            修改说明
//  --------    --------------  ------------    --------------------------------
//  1.0.0       黄文卿        	2009-08-02      创建函数
////////////////////////////////////////////////////////////////////////////////
MCU_RawResetType MCU_GetResetRawValue(void)
{
    return (CRGFLG & 0x64);   
}



////////////////////////////////////////////////////////////////////////////////
//  函数名：	MCU_GetResetReason
//  作用描述：	获取单片机复位的原因
//  
//  返回值描述：MCU_POWER_ON_RESET		来电复位
//				MCU_LOW_VOL_RESET		低电压复位
//				MCU_ILLEGAL_ADDR_RESET	非法地址复位
//				MCU_RESET_UNDEFINED		未定义复位
//			
//  参数描述：	void
//  
//  修改历史：
//  版本        参与人员        日期            修改说明
//  --------    --------------  ------------    --------------------------------
//  1.0.0       黄文卿        	2009-08-02      创建函数
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

