////////////////////////////////////////////////////////////////////////////////
//	名称: Platform_Types.h
//	版本: 1.0    COPYRIGHT (c) AETC 2008 , All Rights Reserved
//	描述: MPC55xx基本数据类型定义
//	编译: CodeWarrior for 12 V4.7
//	修改历史： 
//	版本	作者	  日期		描述 
//	----	-------	----------	---------------------------
//	0.0		简耀佳	2008-09-08	创建文件
//	0.1     黄国钧				从32位处理器移植到16位上
//
////////////////////////////////////////////////////////////////////////////////

#include <MC9S12XS128.h>

#ifndef _PLATFORM_TYPES_H_
#define _PLATFORM_TYPES_H_

////////////////////////////////////////////////////////////////////////////////
//	Symbol definitions

#define	CPU_TYPE_8		(8)
#define CPU_TYPE_16		(16)
#define	CPU_TYPE_32		(32)

#define	MSB_FIRST		(0)
#define	LSB_FIRST		(1)

#define	HIGH_BYTE_FIRST	(0)
#define	LOW_BYTE_FIRST	(1)

#define	CPU_TYPE		CPU_TYPE_16
#define	CPU_BIT_ORDER	LSB_FIRST
#define	CPU_BYTE_ORDER	HIGH_BYTE_FIRST

////////////////////////////////////////////////////////////////////////////////
//	Type Definitions

typedef unsigned	char	boolean;

typedef	signed		char	sint8;
typedef unsigned	char	uint8;
typedef signed		int	    sint16;
typedef unsigned	int	    uint16;
typedef signed		long	sint32;
typedef unsigned	long	uint32;

typedef	signed		int	    sint8_least;
typedef unsigned	int	    uint8_least;
typedef signed		int	    sint16_least;
typedef unsigned	int	    uint16_least;
typedef signed		long	sint32_least;
typedef unsigned	long	uint32_least;

typedef				float	float32;
typedef				double	float64;


//兼容以前程序的基本数据类型
#ifdef TRUE
#undef TRUE
#endif

#ifdef FALSE
#undef FALSE
#endif
typedef enum
{
    FALSE   = 0x00,
    TRUE    = 0x01
}Boolean; 


typedef unsigned    char        UINT8;
typedef signed      char        INT8;
typedef unsigned    int         UINT16;
typedef signed      int         INT16;
typedef unsigned    long        UINT32;
typedef signed      long        INT32;

typedef             float       FLOAT32;
typedef             double      FLOAT64;

typedef volatile unsigned    char    VUINT8;
typedef volatile signed      char    VINT8;
typedef volatile unsigned    int     VUINT16;
typedef volatile signed      int     VINT16;
typedef volatile unsigned    long    VUINT32;
typedef volatile signed      long    VINT32;

typedef volatile             float   VFLOAT32;
typedef volatile             double  VFLOAT64;

typedef enum 
{
    DIG_HIGH    = 0x01,
    DIG_LOW     = 0x00,
    DIG_Z       = 0x02
}DigitalLevel;

typedef enum 
{
    DT_UINT8,
    DT_INT8,
    DT_UINT16,
    DT_INT16,
    DT_UINT32,
    DT_INT32,
    DT_FLOAT32,
    DT_FLOAT64
}DataTypes;

typedef union
{
    UINT8   UINT8Data;
    INT8    INT8Data;
    UINT16  UINT16Data;
    INT16   INT16Data;
    UINT32  UINT32Data;
    INT32   INT32Data;
    FLOAT32 FLOAT32Data;
    FLOAT64 FLOAT64Data;
} DataUnion;

typedef struct
{
    DataTypes   DataType;
    DataUnion   Data;
} ComplexData;

typedef struct
{
    UINT16  Year;
    UINT8   Month;
    UINT8   Day;
} Date;

typedef struct
{
    UINT8   Hour;
    UINT8   Minute;
    UINT8   Second;
    UINT8   Millisecond;
} Time;

#endif