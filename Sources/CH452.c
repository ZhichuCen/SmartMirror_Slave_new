/* ����ܳ�ʼ������ʾ���� */
#include "CH452.h" 
#include "Platform_Types.h"        

/* ��ʼ��SPI */
void SPI0_Init(void)                              
{
  PTM_PTM3=1;                // ��SS��λ
  DDRM_DDRM3=1;               // ��5��6��7��λ��1�����ʹ�ܣ���4λ��������
  DDRM_DDRM2=0;
  DDRM_DDRM4=1;
  DDRM_DDRM5=1;
  SPI0BR=0x01;             // 4��Ƶ������1Mʱ��
  SPI0CR2=0x00;            // ��ֹģʽ�����־ʹ�ܣ�4��ģʽ���ȴ�ģʽSPIֹͣ����
  SPI0CR1=0x1C;            // ���жϣ���ģʽ���Ӹ�λ��ʼ����
  SPI0CR1_SPE=1;           // SPIʹ��
}


/* ��CH452������������ */ 
void CH452_Write(UINT16 cmd)                      
{
  UINT8 STATUS=0;      
  while(SPI0SR_SPTEF==0);      // �ȴ�������Ϊ��
  SPI0DR=(byte)(cmd>>8);       // 16λ��������8λ��ǿ������ת�����ͳ���8λ
  while(SPI0SR_SPIF==0);
  CH452_LOAD=0;                                   
  STATUS=SPI0SR;
  STATUS=SPI0DR;
   
  while(SPI0SR_SPTEF==0);                   
  SPI0DR=(byte)(cmd);          // �ͳ���8λ����   
  while(SPI0SR_SPIF==0);       // �ȴ�������� 
  CH452_LOAD=1;                // ����load�ź�         
  STATUS=SPI0SR;
  STATUS=SPI0DR;
}


/* ѡ����Ҫ��ʾ������λ����BCD�� */
void CH452_WriteBCD(UINT8 dig,UINT8 dataBCD)              
{
  UINT16 dataH,dataL;  
  switch(dig)                   // �趨��ʾ��λ  
  {
    case 0: dataH=CH452_DIG0;  
            break;
  
    case 1: dataH=CH452_DIG1; 
            break;
   
    case 2: dataH=CH452_DIG2; 
            break;
   
    case 3: dataH=CH452_DIG3;  
            break;
   
    case 4: dataH=CH452_DIG4;  
            break;
   
    case 5: dataH=CH452_DIG5;  
            break;
   
    case 6: dataH=CH452_DIG6;  
            break;
   
    default: dataH=CH452_DIG7; 
             break;
  }
  switch(dataBCD)               // �趨��Ӧλ����ֵ 
  { 
    case 0: dataL=CH452_BCD_0;   
            break;
  
    case 1: dataL=CH452_BCD_1;    
            break;
   
    case 2: dataL=CH452_BCD_2;    
            break;
   
    case 3: dataL=CH452_BCD_3;    
            break;
   
    case 4: dataL=CH452_BCD_4;     
            break;
   
    case 5: dataL=CH452_BCD_5;    
            break;
   
    case 6: dataL=CH452_BCD_6;    
            break;
 
    case 7: dataL=CH452_BCD_7;    
            break;
  
    case 8: dataL=CH452_BCD_8;     
            break;
  
    case 9: dataL=CH452_BCD_9;    
            break;
  
    case 10: dataL=CH452_BCD_A;    
             break;
  
    case 11: dataL=CH452_BCD_B;    
             break;
  
    case 12: dataL=CH452_BCD_C;     
             break;
   
    case 13: dataL=CH452_BCD_D;     
             break;
   
    case 14: dataL=CH452_BCD_E;     
             break;
  
    case 15: dataL=CH452_BCD_F;    
             break;
   
    default: dataL=CH452_BCD_SPACE; 
             break;
  } 
  CH452_Write(dataH+dataL);     // ��CH452������ʾλ������ֵ��ָ��
}


/* �������8λ2������ת��Ϊ10�����������������ʾ */
void CH452_Decimal0(UINT16 data)
{
  CH452_WriteBCD(7,(UINT8)(data/1000));
  CH452_WriteBCD(6,(UINT8)((data%1000)/100));
  CH452_WriteBCD(5,(UINT8)((data%100)/10));
  CH452_WriteBCD(4,(UINT8)(data%10));
}
void CH452_Decimal1(UINT16 data)
{
  CH452_WriteBCD(3,(UINT8)(data/1000));
  CH452_WriteBCD(2,(UINT8)((data%1000)/100));
  CH452_WriteBCD(1,(UINT8)((data%100)/10));
  CH452_WriteBCD(0,(UINT8)(data%10));
}


/* ѡ����˸λ */
void CH452_Twinkle(UINT8 num) 
{
  switch(num)                            
  {
    case 0: CH452_Write(CH452_TWINKLE+0x0800);  
            break;                                   // �趨��˸��λΪ0
    
    case 1: CH452_Write(CH452_TWINKLE+0x0400);
            break;
    
    case 2: CH452_Write(CH452_TWINKLE+0x0200);
            break;
    
    case 3: CH452_Write(CH452_TWINKLE+0x0100);
            break;
    
    case 4: CH452_Write(CH452_TWINKLE+0x0080);
            break;
    
    case 5: CH452_Write(CH452_TWINKLE+0x0040); 
            break;
   
    case 6: CH452_Write(CH452_TWINKLE+0x0020); 
            break;
     
    case 7: CH452_Write(CH452_TWINKLE+0x0010);
            break;
   
    default: CH452_Write(CH452_TWINKLE+0x0000); 
             break;                                 // �ر���˸
  }
}


/* ѡ��С����λ */
void CH452_BCDdot(UINT8 num)
{
  switch(num)
  {  
    case 0: CH452_Write(0x01C0+CH452_SET_BIT);
            break;                                 // ������0λС����
       
    case 1: CH452_Write(0x09C0+CH452_SET_BIT); 
            break;   
      
    case 2: CH452_Write(0x05C0+CH452_SET_BIT); 
            break;
      
    case 3: CH452_Write(0x0DC0+CH452_SET_BIT); 
            break;
      
    case 4: CH452_Write(0x03C0+CH452_SET_BIT); 
            break;
      
    case 5: CH452_Write(0x0BC0+CH452_SET_BIT); 
            break;
      
    case 6: CH452_Write(0x07C0+CH452_SET_BIT); 
            break;
      
    default: CH452_Write(0x0FC0+CH452_SET_BIT); 
             break;
  }
}


/* CH452����LED��ʼ����BCD���뷽ʽ */ 
void CH452_Write_Init(void)                            
{
  CH452_Write(CH452_RESET);               // CH452��λ 
  CH452_Write(CH452_SYSON1);              // ������ʾ����   
  CH452_Write(CH452_BCD);                 // ����BCD���뷽ʽ 
  
  CH452_Write(CH452_DIG0+CH452_BCD_0);    // ��0Ϊ�������ʾ����0  
  CH452_Write(CH452_DIG1+CH452_BCD_1);    
  CH452_Write(CH452_DIG2+CH452_BCD_2);     
  CH452_Write(CH452_DIG3+CH452_BCD_3);    
  CH452_Write(CH452_DIG4+CH452_BCD_4);    
  CH452_Write(CH452_DIG5+CH452_BCD_5);     
  CH452_Write(CH452_DIG6+CH452_BCD_6);   
  CH452_Write(CH452_DIG7+CH452_BCD_7);    
}                                         // LED��ʼ����ʾ8λ����0-7
                                                   

/* CH452���������ʼ�� */      
void CH452_KeyWrite_Init(void)
{
  CH452_Write(CH452_RESET);               // CH452��λ 
  CH452_Write(CH452_SYSON2);              // ������ʾ���ݡ�����ɨ��   
  CH452_Write(CH452_BCD);                 // ����BCD���뷽ʽ 
  
  CH452_Write(CH452_DIG7+CH452_BCD_0);    // ��7λ��ʾ����0
}


/* ��ȡ������ַ������LED��ʾ�������� */
extern UINT8 num;
UINT8 CH452_KeyWrite(UINT8 i)
{  
  UINT8 value;  
  while(CH452_DOUT==0)                    // �ȴ���������
  {   
    if(num==0)                            // �ж�8λ������Ƿ���ȫ���а���ֵ��ʾ
    {  
      CH452_KeyWrite_Init();              // ������ȫ�����
      num=i;
    } 
    else
    {  
      UINT8 j=0,KeyCode=0;
      CH452_Write(CH452_GET_KEY);         // ��CH452���ͷ��ذ�����ַָ��  
      SPI0CR1=0x1C;                       // �ر�SPI��׼�����հ�����ַ
        
      for(j;j<7;j++)                      // ��ʼ����7λ��ַ����
      {
        KeyCode<<=1;                      // ��������
        KeyCode|=CH452_DOUT;              // �Ӹ�λ����λ���밴����ַ
        CH452_DCLK=0;                     // ����ʱ���½��أ�֪ͨCH452������һλ����
        CH452_DCLK=1;
      }
    
      SPI0CR1_SPE=1;                      // SPI�ָ�ʹ��
      switch(KeyCode)
      {
        case 0x60:  if(num==i)                      // �ж��Ƿ�Ϊ��һλ
                    {
                      CH452_WriteBCD(i,1);
                      num--;
                    } 
                    else
                    {
                      CH452_Write(CH452_RIGHTMOV); // ������λ��������λ��д��
                      CH452_WriteBCD(i,1);
                      num--;
                    }
                    value=1; 
                    break;                         // ��ʾ����1
                                          
        case 0x68:  if(num==i)
                    {
                      CH452_WriteBCD(i,2);
                      num--;
                    } 
                    else
                    {
                      CH452_Write(CH452_RIGHTMOV);
                      CH452_WriteBCD(i,2);
                      num--;
                    }
                    value=2;
                    break;
                               
        case 0x70:  if(num==i)
                    {
                      CH452_WriteBCD(i,3);
                      num--;
                    } 
                    else
                    {
                      CH452_Write(CH452_RIGHTMOV);
                      CH452_WriteBCD(i,3);
                      num--;
                    }
                    value=3; 
                    break;
                               
        case 0x61:  if(num==i)
                    {
                      CH452_WriteBCD(i,4);
                      num--;
                    } 
                    else
                    {
                      CH452_Write(CH452_RIGHTMOV);
                      CH452_WriteBCD(i,4);
                      num--;
                    }
                    value=4; 
                    break; 
                              
        case 0x69:  if(num==i)
                    {
                      CH452_WriteBCD(i,5);
                      num--;
                    } 
                    else
                    {
                      CH452_Write(CH452_RIGHTMOV);
                      CH452_WriteBCD(i,5);
                      num--;
                    }
                    value=5; 
                    break;
                              
        case 0x71:  if(num==i)
                    {
                      CH452_WriteBCD(i,6);
                      num--;
                    } 
                    else
                    {
                      CH452_Write(CH452_RIGHTMOV);
                      CH452_WriteBCD(i,6);
                      num--;
                    }
                    value=6;
                    break;
                         
        case 0x62:  if(num==i)
                    {
                      CH452_WriteBCD(i,7);
                      num--;
                    } 
                    else
                    {
                      CH452_Write(CH452_RIGHTMOV);
                      CH452_WriteBCD(i,7);
                      num--;
                    }
                    value=7; 
                    break;
                         
        case 0x6A:  if(num==i)
                    {
                      CH452_WriteBCD(i,8);
                      num--;
                    } 
                    else
                    {
                      CH452_Write(CH452_RIGHTMOV);
                      CH452_WriteBCD(i,8);
                      num--;
                    }
                    value=8;
                    break;
                          
        case 0x72:  if(num==i)
                    {
                      CH452_WriteBCD(i,9);
                      num--;
                    } 
                    else
                    {
                      CH452_Write(CH452_RIGHTMOV);
                      CH452_WriteBCD(i,9);
                      num--;
                    }
                    value=9; 
                    break;
                          
        case 0x6B:  if(num==i)
                    {
                      CH452_WriteBCD(i,0);
                      num--;
                    } 
                    else
                    {
                      CH452_Write(CH452_RIGHTMOV);
                      CH452_WriteBCD(i,0);
                      num--;
                    }
                    value=0; 
                    break;

        case 0x78:  if(num==i)
                    {
                      CH452_WriteBCD(i,10);
                      num--;
                    } 
                    else
                    {
                      CH452_Write(CH452_RIGHTMOV);
                      CH452_WriteBCD(i,10);
                      num--;
                    }
                    value=10; 
                    break;                            // ��ʾ��ĸA
                        
        case 0x79:  if(num==i)
                    {
                      CH452_WriteBCD(i,11);
                      num--;
                    } 
                    else
                    {
                      CH452_Write(CH452_RIGHTMOV);
                      CH452_WriteBCD(i,11);
                      num--;
                    }
                    value=11; 
                    break;                           // ��ʾ��ĸb
                           
        case 0x7A:  for(j=0;j<7;j++)
                    {
                      CH452_WriteBCD(j,CH452_BCD_SPACE);
                    }
                    CH452_WriteBCD(7,0);
                    num=i;
                    break;
                         
        case 0x7B:  if(num<i-1)
                    {                      
                      CH452_Write(CH452_LEFTMOV);
                      num++;
                    } 
                    else
                    {
                      CH452_WriteBCD(i,0);
                      num=i;
                    }
                    break;
                                   
        case 0x63:  CH452_BCDdot(i);
                    if(num==i)
                    {                      
                      num--;
                    }     
                    break;                            // ��ʾС����
              
        case 0x73:  for(j=0;j<8;j++)
                    {
                      CH452_WriteBCD(j,CH452_BCD_SPACE);
                    }
                    CH452_Write(CH452_SLEEP);          
                    break;                                              
      }    
    }
  }          
  return(value);                                     // ���ذ���ֵ
} 














