/* �������ʾ�����������˿ڶ��� */
#ifndef  _CH452_h_
#define  _CH452_h_
#include <MC9S12XS128.h>    

/* �˿ڶ��� */
#define  CH452_LOAD        PTM_PTM3         // ͨ��I/O�ڣ����ڿ���CH452������յ�������
#define  CH452_DOUT        PTM_PTM2         // ���ڽ���CH452���صİ�����ַ 
#define  CH452_DIN         PTM_PTM4         // ��CH452���Ϳ���ָ������
#define  CH452_DCLK        PTM_PTM5         // ����ʱ������

/* �����������ƶ��� */
typedef unsigned char   UINT8;
typedef unsigned int    UINT16;
typedef unsigned long   UINT32;
typedef signed   char   INT8;
typedef signed   int    INT16;
typedef signed   long   INT32;


/* CH452����������
   CH452�����������Ϊ12λ�� ͨ��SPI����2���ֽ�������ʵ��
  ��1�ֽڵ�ǰ4λ��Ч
*/

#define  StartL 0
 #if StartL                                // �ӵ�λ��ʼ����
  #define  CH452_NOP         0x0000        // �ղ���
  #define  CH452_RESET       0x0201        // оƬ�ڲ���λ
  #define  CH452_LEVEL       0x0100        // ���ع���ֵ�������7λѡ�����ֵ
  #define  CH452_CLR_BIT     0x0180        // ��λѰַ��0�������6λ��ַ
  #define  CH452_SET_BIT     0x01c0        // ��λѰַ��1�������6λ��ַ
  #define  CH452_SLEEP       0x0202        // ����˯��״̬ 
  #define  CH452_LEFTMOV     0x0300        // ����������
  #define  CH452_LEFTCYC     0x0301        // ��������ѭ��
  #define  CH452_RIGHTMOV    0x0302        // ����������   
  #define  CH452_RIGHTCYC    0x0303        // ��������ѭ��
  #define  CH452_SELF_BCD    0x0380        // �Զ���BCD��
  #define  CH452_SYSOFF      0x0400        // �ر���ʾɨ�裬�رռ���ɨ��
  #define  CH452_SYSON1      0x0401        // ������ʾɨ�裬�رռ���ɨ��
  #define  CH452_SYSON2      0x0403        // ������ʾɨ�裬�������ɨ��
  #define  CH452_SYSON2W     0x0423        // ������ʾ�����̣������жϷ�ʽΪ�����ն�
  #define  CH452_NO_BCD      0x0500        // �����뷽ʽ�����趨3λɨ�輫�ޣ�λ6-λ4��
  #define  CH452_BCD         0x0580        // BCD���뷽ʽ�����趨3λɨ�輫�ޣ�λ6-λ4�� 
  #define  CH452_TWINKLE     0x0600        // �趨��˸���ƣ������8ѡ��λ  
  #define  CH452_GET_KEY     0x0700        // ��ȡ��������  
  #define  CH452_DIG0        0x0800        // ����������0�������8λ������   
  #define  CH452_DIG1        0x0900        // ����������1�������8λ������ 
  #define  CH452_DIG2        0x0a00        // ����������2�������8λ������ 
  #define  CH452_DIG3        0x0b00        // ����������3�������8λ������  ��
  #define  CH452_DIG4        0x0c00        // ����������4�������8λ������ ��
  #define  CH452_DIG5        0x0d00        // ����������5�������8λ������ ��
  #define  CH452_DIG6        0x0e00        // ����������6�������8λ������ ��
  #define  CH452_DIG7        0x0f00        // ����������7�������8λ������ ��
         
  //BCD���뷽ʽ�µ���ʾ�ַ� 
  #define  CH452_BCD_0       0x0000        // ��ʾ�ַ� 0��
  #define  CH452_BCD_1       0x0001        // ��ʾ�ַ� 1
  #define  CH452_BCD_2       0x0002        // ��ʾ�ַ� 2 
  #define  CH452_BCD_3       0x0003        // ��ʾ�ַ� 3  
  #define  CH452_BCD_4       0x0004        // ��ʾ�ַ� 4 
  #define  CH452_BCD_5       0x0005        // ��ʾ�ַ� 5
  #define  CH452_BCD_6       0x0006        // ��ʾ�ַ� 6  
  #define  CH452_BCD_7       0x0007        // ��ʾ�ַ� 7 
  #define  CH452_BCD_8       0x0008        // ��ʾ�ַ� 8
  #define  CH452_BCD_9       0x0009        // ��ʾ�ַ� 9
  #define  CH452_BCD_A       0x000A        // ��ʾ�ַ� A 
  #define  CH452_BCD_B       0x000B        // ��ʾ�ַ� B
  #define  CH452_BCD_C       0x000C        // ��ʾ�ַ� C
  #define  CH452_BCD_D       0x000D        // ��ʾ�ַ� D
  #define  CH452_BCD_E       0x000E        // ��ʾ�ַ� E
  #define  CH452_BCD_F       0x000F        // ��ʾ�ַ� F

  //BCD���뷽ʽ����ʾ�����ַ�     
  #define 	CH452_BCD_SPACE	  0x0010      // �ո����������ʾ   
  #define	CH452_BCD_PLUS	  0x0011        // -1��Ӻ�  
  #define	CH452_BCD_MINUS	  0x0012        // ���ż���  
  #define	CH452_BCD_EQU	    0x0013        // ���ں� 
  #define	CH452_BCD_LEFT	  0x0014        // ������  
  #define	CH452_BCD_RIGHT	  0x0015        // �ҷ����� 
  #define	CH452_BCD_UNDER  	0x0016        // �»��� 
  #define	CH452_BCD_CH_H	  0x0017        // ��ĸH  
  #define	CH452_BCD_CH_L  	0x0018        // ��ĸL    
  #define	CH452_BCD_CH_P	  0x0019        // ��ĸP   
  #define	CH452_BCD_DOT	    0x001A        // С���� 
  #define	CH452_BCD_SELF  	0x001E        // �Զ����ַ�  
  #define	CH452_BCD_TEST	  0x0088        //
  #define	CH452_BCD_DOT_X	  0x0080        //
       
#else                                     // �Ӹ�λ��ʼ����    
  #define  CH452_NOP	    	  0x0000			// �ղ���
  #define  CH452_RESET       0x0804				// ��λ
  #define  CH452_LEVEL	    	0x0008			// ���ع���ֵ,�����7λ����
  #define  CH452_CLR_BIT	    0x0018			// ��λ��0,�����6λ����
  #define  CH452_SET_BIT	    0x0038			// ��λ��1,�����6λ����
  #define  CH452_SLEEP	   	  0x0404			// ����˯��״̬
  #define  CH452_LEFTMOV     0x000C		    // �����ƶ���ʽ-����
  #define  CH452_LEFTCYC     0x080C		    // �����ƶ���ʽ-��ѭ��
  #define  CH452_RIGHTMOV    0x040C		    // �����ƶ���ʽ-����
  #define  CH452_RIGHTCYC    0x0C0C		    // �����ƶ���ʽ-��ѭ��	
  #define  CH452_SELF_BCD	  0x0580				// �Զ���BCD��,�����7λ����
  #define  CH452_SYSOFF      0x0002				// �ر���ʾ���رռ���
  #define  CH452_SYSON1      0x0802				// ������ʾ
  #define  CH452_SYSON2      0x0C02			  // ������ʾ������
  #define  CH452_SYSON2W     0x0C42				// ������ʾ������, ����2�߽ӿ�
  #define  CH452_NO_BCD      0x000A				// ����Ĭ����ʾ��ʽ,�����3λɨ�輫��
  #define  CH452_BCD         0x001A				// ����BCD���뷽ʽ,�����3λɨ�輫��
  #define  CH452_TWINKLE     0x0006		    // ������˸����,�����8λ����
  #define  CH452_GET_KEY	    0x000E			// ��ȡ����,���ذ�������
  #define  CH452_DIG0        0x0001				// �����λ0��ʾ,�����8λ����
  #define  CH452_DIG1        0x0009		    // �����λ1��ʾ,�����8λ����
  #define  CH452_DIG2        0x0005		    // �����λ2��ʾ,�����8λ����
  #define  CH452_DIG3        0x000d		    // �����λ3��ʾ,�����8λ����
  #define  CH452_DIG4        0x0003		    // �����λ4��ʾ,�����8λ����
  #define  CH452_DIG5        0x000B				// �����λ5��ʾ,�����8λ����
  #define  CH452_DIG6        0x0007				// �����λ6��ʾ,�����8λ����
  #define  CH452_DIG7        0x000F		    // �����λ7��ʾ,�����8λ����
        
  // 0 1 2 3 4 5 6 7 8 9 A B C D E F ��������
  // 0 8 4 C 2 A 6 E 1 9 5 D 3 B 7 F �ߵ�λ����������
  // BCD���뷽ʽ�µ���ͨ�ַ� 
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

  // BCD���뷽ʽ�µ������ַ� 
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

/* �ⲿ�������� */
extern void SPI0_Init(void);                            // ��ʼ��SPI               

extern void CH452_Write(UINT16 cmd);                    // ��CH452������������     
extern void CH452_WriteBCD(UINT8 dig,UINT8 dataBCD);    // �趨��ʾ�ַ�           
extern void CH452_Write_Init(void);                     // CH452��ʼ��������LED��ʾ����    
extern void CH452_Decimal(UINT8 data);                  // �������8λ2������ת��Ϊ10�����������������ʾ 
extern void CH452_Twinkle(UINT8 num);                   // ѡ����˸λ 
extern void CH452_BCDdot(UINT8 num);                    // ѡ��С����λ 
                                                        
extern void CH452_KeyWrite_Init(void);                  // CH452�������ʼ��
extern UINT8 CH452_KeyWrite(UINT8 num);                 // CH452��ȡ������ַ�����ذ���ֵ�������������ʾ����ֵ 

#endif