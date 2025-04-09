#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "Platform_Types.h"
#include "ADC.h"
#include "CAN.h"
#include "CH452.h"
#include "MCU.h"
#include "Std_Types.h"
MCU_CAN_MSG l_RxData={{0,0,0,0,0,0,0,0},0,0};
//extern UINT16 AD_value1;
///extern UINT16 AD_value2;
MCU_CAN_MSG can_msg={0};
#define LEDCPU1 PORTA_PA1
#define LEDCPU2 PORTA_PA2
#define LEDCPU3 PORTA_PA3

// Define system states
typedef enum {
    STATE_WAIT_BUTTON,
    STATE_WAIT_RESPONSE
} SystemState;

// Variables for the received values
sint32 receivedValue1 = 123;
sint32 receivedValue2 = 456;
SystemState currentState = STATE_WAIT_BUTTON;
uint8 responseReceived = 0;

unsigned char count1,count2,count3;//����������ÿ��һ�����ּ�1/

unsigned char flag; //�жϷ�����־//
UINT16 Throttle=0;
UINT16 Brake=0;
UINT16 Speed=0;
long Speed_high = 0;
long Speed_middle = 0;
long Speed_low = 0;
long Engine_Throttle_high = 0;
long Engine_Throttle_low = 0;	 
long Motor_Speed_high = 0;
long Motor_Speed_low = 0;
//long Throttle = 0;
UINT16 start_system_flag=1; 
MCU_CAN_MSG SendMotor_msg={0};
MCU_CAN_MSG ReceiveMotor_msg={0};
MCU_CAN_MSG ReceiveVehicle_msg={0};
sint16 Engine_Throttle;  
sint16 Motor_Speed=0; 
void PLL_Init(void)      
{ 
	CLKSEL=0;			            	//��ʹ�����໷ģ��,��ʼ��PLL��ǰʹ�õ��ⲿ����

	REFDV=0X41;                 /*REF_CLOCK=OSC_CLOCK/(REFDV+1)=12/2=6
	                           REFFRQ_CLOCK[1:0]����REF_CLOCK��(2,6]֮��
	                         */

	SYNR=0X03;                  /*VCO_CLOCK(ѹ����Ƶ��)=2*OSC_CLOCK��oscclk �ⲿ����Ƶ�ʣ�*(1+SYNDIV)/(1+REFDIV)
	                                                 =2*12*(1+3)/(1+1)=48MHZ
	                            VCOFRQ[0:0]����VCO_CLOCK��[32,48]֮��*/  
	         
	POSTDIV=0X02;              /*POSTDIV=2   PLL_CLOCK=VCO_CLOCK/(2*POSTDIV)=12MHz ;
									 		BUS=PLL_CLOCK/2=6MHZ
	                      		 */              
	        	
	_asm(nop);                 //������ʱ���ȴ��ȶ�     ����ʱ�䣺����Ƶ�ʵĵ���
	_asm(nop); 
	while(!(CRGFLG&0x08));
	PLLCTL_PLLON=1;	        //���໷��·���� 
	CLKSEL=0x80;              //ʹ�����໷ʱ��
}
//----------------------1s���ڶ�ʱ--------------------//
void PIT_init()
{
	PITCFLMT=0x00;		//��ֹPITģ��
	PITCE_PCE0=1;		//ʹ��PIT��ʱ��ͨ��0		
	PITMUX_PMUX0=1;   //PIT��ʱ��ͨ��0�ĸ���λΪ1����ʹ��΢��ʱ��׼1����
	PITINTE=0x01;		//ʹ��PITͨ��0�ж�
	PITMTLD1=0xC7;		//����΢��ʱ�Ĵ���1��ֵ��199
	PITLD0=299;     //����16λװ�ؼĴ���0��ֵ
	//PITLD0=0x752F;		//����16λװ�ؼĴ���0��ֵ,29999
  //PITLD0=0xEA5F;		//����16λװ�ؼĴ���0��ֵ,59999						
  	//��ʱ����T=(PITMTLD1+1)*(PITLD0+1)/f=200*300/6M=0.01s;
	PITCFLMT=0x80;		//ʹ��PITģ��
}
/*************************************************************/
/*                         ��ʱ����                          */
/*************************************************************/
void delay1ms(unsigned int n) 
{   unsigned int a,b;
    for(a=0;a<=n;a++) 
    {  
      for(b=0;b<=1000;b++);
    }
}
void delay(void)//��ʱ����
{   unsigned int a,b;
    for(a=0;a<=100;a++) 
    {  
      for(b=0;b<=200;b++);
    }
}

//////////////////////////////////////////////////////////////////////////////////
void key(){    //�����жϳ���//
if(PTIJ_PTIJ0==0) {            //�ж��Ƿ��°���//
  delay1ms(10);                 //��ʱ������ȥ����//
if(PTIJ_PTIJ0==0)              //ȷ�ϰ�������//
  count1++;                      //����������1//
}
if(PTIJ_PTIJ1==0) {            //�ж��Ƿ��°���//
  delay1ms(10); //��ʱ������ȥ����//
  if(PTIJ_PTIJ1==0){
    delay1ms(250);
    if(PTIJ_PTIJ1==0)              
      count3++;
    else
      count2++;
    }
    
  }
                      
while(PTIJ_PTIJ0==0||PTIJ_PTIJ1==0);            //������������һ��ֻ��1//
}
  

//--------------------pit�жϳ���------------------------//


#pragma CODE_SEG __NEAR_SEG NON_BANKED 
void interrupt VectorNumber_Vpit0 pit()
{ 
PITTF_PTF0=1;//����жϱ�־//
flag=1;//�ñ�־//

    //key();
    if(count1>1)
    count1=0;
    if(count2>1)
    count2=0;
    if(count3>1) 
    count3=0;  
    
    LEDCPU1=count1;        //���״̬
    LEDCPU2=count2;        //��ȴ״̬
    LEDCPU3=count3;        //����״̬
         
}
#pragma CODE_SEG DEAFAULT	

//void Tsend()
//{
//
//    SendMotor_msg.DataLength=8;
//    SendMotor_msg.ID=ToStandDataID(0x100);    //ID==C100327///ԭ����   ToExtendDataID(0x611807DE)
//    SendMotor_msg.Data[0]=0b00001011;
//    SendMotor_msg.Data[1]=Speed_high;
//    SendMotor_msg.Data[2]=Speed_middle;
//    SendMotor_msg.Data[3]=Speed_low<<6;
//    SendMotor_msg.Data[4]=0;
//    SendMotor_msg.Data[5]=1;
//    SendMotor_msg.Data[6]=Throttle*250/98;
//    SendMotor_msg.Data[7]=0;
//    MCU_CAN_Transmit(0,SendMotor_msg);                        // CLEAN THE CAN0TFLG
//
//    }


// Function to send empty CAN message with ID 0x200
void SendEmptyCanMessage(void) {
    MCU_CAN_MSG emptyMsg = {0};
    emptyMsg.ID = ToExtendDataID(0x200);
    emptyMsg.DataLength = 8;
    // Data is already all zeros by initialization
    MCU_CAN_Transmit(0, emptyMsg);
}

 
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

	// Check if message has ID 0x100
	// if(l_RxData.ID == ToStandDataID(0x100)) {
    if(1){
//		for(l_RxIndex = 0; l_RxIndex < l_RxData.DataLength; l_RxIndex++) {
//			l_RxData.Data[l_RxIndex] = *((&CAN0RXDSR0) + l_RxIndex);
//		}
		
		// Extract two int32 values (little-endian format from Python struct.pack)
		receivedValue1 = (sint32)(((sint32)l_RxData.Data[3] << 24) | ((sint32)l_RxData.Data[2] << 16) |
                         ((sint32)l_RxData.Data[1] << 8) | (sint32)l_RxData.Data[0]);
		receivedValue2 = (sint32)(((sint32)l_RxData.Data[7] << 24) | ((sint32)l_RxData.Data[6] << 16) |
                         ((sint32)l_RxData.Data[5] << 8) | (sint32)l_RxData.Data[4]);
		
		// Set flag to indicate response received
		responseReceived = 1;
	}
	
	// ...existing code for other IDs...
	
    (void)MCU_CAN_AddRxData(0,l_RxData);
    CAN0RFLG = 0x01;	
}
#pragma CODE_SEG DEFAULT 


//////////////////////////////////////////////////////////////////////////////////


 int bbb;         
void main(void) {
  DisableInterrupts;
  flag=0;
  DDRA=0xff;
  //DDRJ=0x00;
  /* put your own code here */
  (void)MCU_Init();
  ADC_Init();
  ADC_StartChannelConversion(0, 2);	
  ATD0DIEN = 0x0000;					           	//��ֹ����ANx�ܽŴ����������� 
    
  MODRR_MODRR4=1;                         //�������PM
  SPI0_Init();
    
  (void)  MCU_CAN_Enable(0);
  (void)  MCU_CAN_Init();
  (void)  MCU_CAN_EnterInitMode(0);
  (void)  MCU_CAN_SetFreq(0, CAN500K);
  (void)  MCU_CAN_SetSampling(0, CAN_SHORT);
  (void)  MCU_CAN_SetIDAcceptMode(0, TWO_32_BIT);
  // Change CAN filter to accept ID 0x200
  (void)  MCU_CAN_SetRXAcceptID(0, 0x00200000, 0x00200000);
  (void)  MCU_CAN_SetRxMask(0, 0xffffffff, 0xffffffff);
  (void)  MCU_CAN_CheckOutInitMode(0);
  
  PLL_Init();
  PIT_init();
  CH452_Write(CH452_RESET);//CH452��λ
  CH452_Write(CH452_SYSON1);//������ʾ����
  CH452_Write(CH452_BCD); //����BCD���뷽ʽ
  CH452_Write_Init();                          // CH452��ʼ��������LED��ʾ���� 
  EnableInterrupts;


  for(;;) 
  {
    CH452_Decimal0((UINT16)receivedValue1);
  	CH452_Decimal1((UINT16)receivedValue2);
    switch(currentState) {

      case STATE_WAIT_BUTTON:
        if(PTIJ_PTIJ1 == 0) { // Button PJ1 is pressed (voltage dropped to zero)
          delay1ms(10); // Debounce
          if(PTIJ_PTIJ1 == 0) {
            // Send empty CAN message with ID 0x200
            SendEmptyCanMessage();
            currentState = STATE_WAIT_RESPONSE;
            responseReceived = 0;
            
            // Wait for button release
            while(PTIJ_PTIJ1 == 0) {
              _FEED_COP(); // Feed the watchdog
            }
          }
        }
        break;
        
      case STATE_WAIT_RESPONSE:
        if(responseReceived) {
          // Display received values on CH452

          
          // Return to waiting for button
          currentState = STATE_WAIT_BUTTON;
        }
        break;
    }
    
    bbb++;
    //delay();
    //��ȡAD1����ֵ����ת��Ϊ0~255����
//    if(flag==1){
//        flag=0;
//        Throttle = ADC_GetChannelResult(0)/4*98/250;
//        Brake = ADC_GetChannelResult(1)/4;
//
//        CH452_Decimal0(Throttle);
//        //CH452_Decimal1(AD_value2);
//
//
////        Tsend();
//    }
    

    
    
    //Treceive();
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}

