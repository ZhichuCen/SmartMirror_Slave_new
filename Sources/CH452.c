/* 数码管初始化及显示函数 */
#include "CH452.h" 
#include "Platform_Types.h"        

/* 初始化SPI */
void SPI0_Init(void)                              
{
  PTM_PTM3=1;                // 将SS置位
  DDRM_DDRM3=1;               // 将5，6，7三位置1，输出使能；第4位用作输入
  DDRM_DDRM2=0;
  DDRM_DDRM4=1;
  DDRM_DDRM5=1;
  SPI0BR=0x01;             // 4分频，产生1M时钟
  SPI0CR2=0x00;            // 禁止模式错误标志使能，4线模式，等待模式SPI停止工作
  SPI0CR1=0x1C;            // 关中断，主模式，从高位开始发送
  SPI0CR1_SPE=1;           // SPI使能
}


/* 向CH452发出操作命令 */ 
void CH452_Write(UINT16 cmd)                      
{
  UINT8 STATUS=0;      
  while(SPI0SR_SPTEF==0);      // 等待缓冲器为空
  SPI0DR=(byte)(cmd>>8);       // 16位数据右移8位，强制类型转换，送出高8位
  while(SPI0SR_SPIF==0);
  CH452_LOAD=0;                                   
  STATUS=SPI0SR;
  STATUS=SPI0DR;
   
  while(SPI0SR_SPTEF==0);                   
  SPI0DR=(byte)(cmd);          // 送出低8位数据   
  while(SPI0SR_SPIF==0);       // 等待传输完成 
  CH452_LOAD=1;                // 设置load信号         
  STATUS=SPI0SR;
  STATUS=SPI0DR;
}


/* 选择所要显示的数码位数及BCD码 */
void CH452_WriteBCD(UINT8 dig,UINT8 dataBCD)              
{
  UINT16 dataH,dataL;  
  switch(dig)                   // 设定显示字位  
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
  switch(dataBCD)               // 设定相应位的数值 
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
  CH452_Write(dataH+dataL);     // 向CH452发送显示位及其数值的指令
}


/* 将输入的8位2进制数转换为10进制数并用数码管显示 */
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


/* 选择闪烁位 */
void CH452_Twinkle(UINT8 num) 
{
  switch(num)                            
  {
    case 0: CH452_Write(CH452_TWINKLE+0x0800);  
            break;                                   // 设定闪烁字位为0
    
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
             break;                                 // 关闭闪烁
  }
}


/* 选择小数点位 */
void CH452_BCDdot(UINT8 num)
{
  switch(num)
  {  
    case 0: CH452_Write(0x01C0+CH452_SET_BIT);
            break;                                 // 点亮第0位小数点
       
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


/* CH452驱动LED初始化，BCD译码方式 */ 
void CH452_Write_Init(void)                            
{
  CH452_Write(CH452_RESET);               // CH452复位 
  CH452_Write(CH452_SYSON1);              // 开启显示数据   
  CH452_Write(CH452_BCD);                 // 设置BCD译码方式 
  
  CH452_Write(CH452_DIG0+CH452_BCD_0);    // 第0为数码管显示数字0  
  CH452_Write(CH452_DIG1+CH452_BCD_1);    
  CH452_Write(CH452_DIG2+CH452_BCD_2);     
  CH452_Write(CH452_DIG3+CH452_BCD_3);    
  CH452_Write(CH452_DIG4+CH452_BCD_4);    
  CH452_Write(CH452_DIG5+CH452_BCD_5);     
  CH452_Write(CH452_DIG6+CH452_BCD_6);   
  CH452_Write(CH452_DIG7+CH452_BCD_7);    
}                                         // LED初始化显示8位数字0-7
                                                   

/* CH452按键输入初始化 */      
void CH452_KeyWrite_Init(void)
{
  CH452_Write(CH452_RESET);               // CH452复位 
  CH452_Write(CH452_SYSON2);              // 开启显示数据、键盘扫描   
  CH452_Write(CH452_BCD);                 // 设置BCD译码方式 
  
  CH452_Write(CH452_DIG7+CH452_BCD_0);    // 第7位显示数字0
}


/* 读取按键地址，并用LED显示按键数字 */
extern UINT8 num;
UINT8 CH452_KeyWrite(UINT8 i)
{  
  UINT8 value;  
  while(CH452_DOUT==0)                    // 等待按键输入
  {   
    if(num==0)                            // 判断8位数码管是否已全部有按键值显示
    {  
      CH452_KeyWrite_Init();              // 将数据全部清除
      num=i;
    } 
    else
    {  
      UINT8 j=0,KeyCode=0;
      CH452_Write(CH452_GET_KEY);         // 向CH452发送返回按键地址指令  
      SPI0CR1=0x1C;                       // 关闭SPI，准备接收按键地址
        
      for(j;j<7;j++)                      // 开始接收7位地址数据
      {
        KeyCode<<=1;                      // 数据移入
        KeyCode|=CH452_DOUT;              // 从高位到低位读入按键地址
        CH452_DCLK=0;                     // 产生时钟下降沿，通知CH452输入下一位数据
        CH452_DCLK=1;
      }
    
      SPI0CR1_SPE=1;                      // SPI恢复使能
      switch(KeyCode)
      {
        case 0x60:  if(num==i)                      // 判断是否为第一位
                    {
                      CH452_WriteBCD(i,1);
                      num--;
                    } 
                    else
                    {
                      CH452_Write(CH452_RIGHTMOV); // 不是首位，则先移位后写数
                      CH452_WriteBCD(i,1);
                      num--;
                    }
                    value=1; 
                    break;                         // 显示数字1
                                          
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
                    break;                            // 显示字母A
                        
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
                    break;                           // 显示字母b
                           
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
                    break;                            // 显示小数点
              
        case 0x73:  for(j=0;j<8;j++)
                    {
                      CH452_WriteBCD(j,CH452_BCD_SPACE);
                    }
                    CH452_Write(CH452_SLEEP);          
                    break;                                              
      }    
    }
  }          
  return(value);                                     // 返回按键值
} 














