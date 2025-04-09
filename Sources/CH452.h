/* 数码管显示函数声明及端口定义 */
#ifndef  _CH452_h_
#define  _CH452_h_
#include <MC9S12XS128.h>    

/* 端口定义 */
#define  CH452_LOAD        PTM_PTM3         // 通用I/O口，用于控制CH452读入接收到的数据
#define  CH452_DOUT        PTM_PTM2         // 用于接收CH452返回的按键地址 
#define  CH452_DIN         PTM_PTM4         // 向CH452发送控制指令数据
#define  CH452_DCLK        PTM_PTM5         // 发送时钟脉冲

/* 数据类型名称定义 */
typedef unsigned char   UINT8;
typedef unsigned int    UINT16;
typedef unsigned long   UINT32;
typedef signed   char   INT8;
typedef signed   int    INT16;
typedef signed   long   INT32;


/* CH452常用命令码
   CH452操作命令码均为12位， 通过SPI发送2个字节数据来实现
  第1字节的前4位无效
*/

#define  StartL 0
 #if StartL                                // 从低位开始发送
  #define  CH452_NOP         0x0000        // 空操作
  #define  CH452_RESET       0x0201        // 芯片内部复位
  #define  CH452_LEVEL       0x0100        // 加载光柱值，需另加7位选择光柱值
  #define  CH452_CLR_BIT     0x0180        // 段位寻址清0，需另加6位地址
  #define  CH452_SET_BIT     0x01c0        // 段位寻址置1，需另加6位地址
  #define  CH452_SLEEP       0x0202        // 进入睡眠状态 
  #define  CH452_LEFTMOV     0x0300        // 字数据左移
  #define  CH452_LEFTCYC     0x0301        // 字数据左循环
  #define  CH452_RIGHTMOV    0x0302        // 字数据右移   
  #define  CH452_RIGHTCYC    0x0303        // 字数据右循环
  #define  CH452_SELF_BCD    0x0380        // 自定义BCD码
  #define  CH452_SYSOFF      0x0400        // 关闭显示扫描，关闭键盘扫描
  #define  CH452_SYSON1      0x0401        // 允许显示扫描，关闭键盘扫描
  #define  CH452_SYSON2      0x0403        // 允许显示扫描，允许键盘扫描
  #define  CH452_SYSON2W     0x0423        // 允许显示，键盘，按键中断方式为边沿终端
  #define  CH452_NO_BCD      0x0500        // 不译码方式，可设定3位扫描极限（位6-位4）
  #define  CH452_BCD         0x0580        // BCD译码方式，可设定3位扫描极限（位6-位4） 
  #define  CH452_TWINKLE     0x0600        // 设定闪烁控制，需另加8选择位  
  #define  CH452_GET_KEY     0x0700        // 读取按键代码  
  #define  CH452_DIG0        0x0800        // 加载字数据0，需另加8位段数据   
  #define  CH452_DIG1        0x0900        // 加载字数据1，需另加8位段数据 
  #define  CH452_DIG2        0x0a00        // 加载字数据2，需另加8位段数据 
  #define  CH452_DIG3        0x0b00        // 加载字数据3，需另加8位段数据  
  #define  CH452_DIG4        0x0c00        // 加载字数据4，需另加8位段数据 
  #define  CH452_DIG5        0x0d00        // 加载字数据5，需另加8位段数据 
  #define  CH452_DIG6        0x0e00        // 加载字数据6，需另加8位段数据 
  #define  CH452_DIG7        0x0f00        // 加载字数据7，需另加8位段数据 
         
  //BCD译码方式下的显示字符 
  #define  CH452_BCD_0       0x0000        // 显示字符 0
  #define  CH452_BCD_1       0x0001        // 显示字符 1
  #define  CH452_BCD_2       0x0002        // 显示字符 2 
  #define  CH452_BCD_3       0x0003        // 显示字符 3  
  #define  CH452_BCD_4       0x0004        // 显示字符 4 
  #define  CH452_BCD_5       0x0005        // 显示字符 5
  #define  CH452_BCD_6       0x0006        // 显示字符 6  
  #define  CH452_BCD_7       0x0007        // 显示字符 7 
  #define  CH452_BCD_8       0x0008        // 显示字符 8
  #define  CH452_BCD_9       0x0009        // 显示字符 9
  #define  CH452_BCD_A       0x000A        // 显示字符 A 
  #define  CH452_BCD_B       0x000B        // 显示字符 B
  #define  CH452_BCD_C       0x000C        // 显示字符 C
  #define  CH452_BCD_D       0x000D        // 显示字符 D
  #define  CH452_BCD_E       0x000E        // 显示字符 E
  #define  CH452_BCD_F       0x000F        // 显示字符 F

  //BCD译码方式下显示特殊字符     
  #define 	CH452_BCD_SPACE	  0x0010      // 空格，数码管无显示   
  #define	CH452_BCD_PLUS	  0x0011        // -1或加号  
  #define	CH452_BCD_MINUS	  0x0012        // 负号减号  
  #define	CH452_BCD_EQU	    0x0013        // 等于号 
  #define	CH452_BCD_LEFT	  0x0014        // 左方括号  
  #define	CH452_BCD_RIGHT	  0x0015        // 右方括号 
  #define	CH452_BCD_UNDER  	0x0016        // 下划线 
  #define	CH452_BCD_CH_H	  0x0017        // 字母H  
  #define	CH452_BCD_CH_L  	0x0018        // 字母L    
  #define	CH452_BCD_CH_P	  0x0019        // 字母P   
  #define	CH452_BCD_DOT	    0x001A        // 小数点 
  #define	CH452_BCD_SELF  	0x001E        // 自定义字符  
  #define	CH452_BCD_TEST	  0x0088        //
  #define	CH452_BCD_DOT_X	  0x0080        //
       
#else                                     // 从高位开始传输    
  #define  CH452_NOP	    	  0x0000			// 空操作
  #define  CH452_RESET       0x0804				// 复位
  #define  CH452_LEVEL	    	0x0008			// 加载光柱值,需另加7位数据
  #define  CH452_CLR_BIT	    0x0018			// 段位清0,需另加6位数据
  #define  CH452_SET_BIT	    0x0038			// 段位置1,需另加6位数据
  #define  CH452_SLEEP	   	  0x0404			// 进入睡眠状态
  #define  CH452_LEFTMOV     0x000C		    // 设置移动方式-左移
  #define  CH452_LEFTCYC     0x080C		    // 设置移动方式-左循环
  #define  CH452_RIGHTMOV    0x040C		    // 设置移动方式-右移
  #define  CH452_RIGHTCYC    0x0C0C		    // 设置移动方式-右循环	
  #define  CH452_SELF_BCD	  0x0580				// 自定义BCD码,需另加7位数据
  #define  CH452_SYSOFF      0x0002				// 关闭显示、关闭键盘
  #define  CH452_SYSON1      0x0802				// 开启显示
  #define  CH452_SYSON2      0x0C02			  // 开启显示、键盘
  #define  CH452_SYSON2W     0x0C42				// 开启显示、键盘, 真正2线接口
  #define  CH452_NO_BCD      0x000A				// 设置默认显示方式,可另加3位扫描极限
  #define  CH452_BCD         0x001A				// 设置BCD译码方式,可另加3位扫描极限
  #define  CH452_TWINKLE     0x0006		    // 设置闪烁控制,需另加8位数据
  #define  CH452_GET_KEY	    0x000E			// 获取按键,返回按键代码
  #define  CH452_DIG0        0x0001				// 数码管位0显示,需另加8位数据
  #define  CH452_DIG1        0x0009		    // 数码管位1显示,需另加8位数据
  #define  CH452_DIG2        0x0005		    // 数码管位2显示,需另加8位数据
  #define  CH452_DIG3        0x000d		    // 数码管位3显示,需另加8位数据
  #define  CH452_DIG4        0x0003		    // 数码管位4显示,需另加8位数据
  #define  CH452_DIG5        0x000B				// 数码管位5显示,需另加8位数据
  #define  CH452_DIG6        0x0007				// 数码管位6显示,需另加8位数据
  #define  CH452_DIG7        0x000F		    // 数码管位7显示,需另加8位数据
        
  // 0 1 2 3 4 5 6 7 8 9 A B C D E F 正常数据
  // 0 8 4 C 2 A 6 E 1 9 5 D 3 B 7 F 高低位互换后数据
  // BCD译码方式下的普通字符 
  #define CH452_BCD_0        0x0000
  #define CH452_BCD_1        0x0800
  #define CH452_BCD_2        0x0400
  #define CH452_BCD_3        0x0C00
  #define CH452_BCD_4        0x0200
  #define CH452_BCD_5        0x0A00
  #define CH452_BCD_6        0x0600
  #define CH452_BCD_7        0x0E00
  #define CH452_BCD_8        0x0100
  #define CH452_BCD_9        0x0900
  #define CH452_BCD_A        0x0500
  #define CH452_BCD_B        0x0D00
  #define CH452_BCD_C        0x0300
  #define CH452_BCD_D        0x0B00
  #define CH452_BCD_E        0x0700
  #define CH452_BCD_F        0x0F00

  // BCD译码方式下的特殊字符 
  #define	CH452_BCD_SPACE	  0x0080
  #define	CH452_BCD_PLUS	  0x0880
  #define	CH452_BCD_MINUS	  0x0480
  #define	CH452_BCD_EQU	    0x0C80
  #define	CH452_BCD_LEFT	  0x0280
  #define	CH452_BCD_RIGHT  	0x0A80
  #define	CH452_BCD_UNDER	  0x0680
  #define	CH452_BCD_CH_H	  0x0E80
  #define	CH452_BCD_CH_L  	0x0180
  #define	CH452_BCD_CH_P  	0x0980
  #define	CH452_BCD_DOT   	0x0580
  #define	CH452_BCD_SELF  	0x0780
  #define	CH452_BCD_TEST  	0x0110
  #define	CH452_BCD_DOT_X 	0x0010
#endif

/* 外部函数声明 */
extern void SPI0_Init(void);                            // 初始化SPI               

extern void CH452_Write(UINT16 cmd);                    // 向CH452发出操作命令     
extern void CH452_WriteBCD(UINT8 dig,UINT8 dataBCD);    // 设定显示字符           
extern void CH452_Write_Init(void);                     // CH452初始化，用于LED显示数据    
extern void CH452_Decimal(UINT8 data);                  // 将输入的8位2进制数转换为10进制数并用数码管显示 
extern void CH452_Twinkle(UINT8 num);                   // 选择闪烁位 
extern void CH452_BCDdot(UINT8 num);                    // 选择小数点位 
                                                        
extern void CH452_KeyWrite_Init(void);                  // CH452按输入初始化
extern UINT8 CH452_KeyWrite(UINT8 num);                 // CH452读取按键地址，返回按键值，并用数码管显示按键值 

#endif