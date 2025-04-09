////////////////////////////////////////////////////////////////////////////////
//	����: Std_Types.h
//	�汾: 1.0    COPYRIGHT (c) AETC 2008 , All Rights Reserved
//	����: AETCOS��׼�������Ͷ���
//	����: CodeWarrior for 12 V4.6
//	�޸���ʷ�� 
//	�汾	����	  ����		���� 
//	----	-------	----------	---------------------------
//	1.0		��ҫ��	2008-09-08	�����ļ�
//	1.1		�ƹ���	2009-07-28	װ����12���õİ汾
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _STD_TYPES_H_
#define _STD_TYPES_H_

#include <hidef.h>      /* common defines and macros */
#include <MC9S12XS128.h>
//#include <MC9S12XEQ512.h> 
#include "Platform_Types.h"
//#include "Compiler.h" 



////////////////////////////////////////////////////////////////////////////////
#ifndef STATUSTYPEDEFINED
	#define STATUSTYPEDEFINED
	#define E_OK	(0x00)
	
	typedef uint8_least StatusType;
#endif

#define	E_NOT_OK	(0x01)

#define	STD_HIGH	(0x01)
#define	STD_LOW		(0x00)

#define	STD_ACTIVE	(0x01)
#define	STD_IDLE	(0x00)

#define	STD_ON		(0x01)
#define	STD_OFF		(0x00)

////////////////////////////////////////////////////////////////////////////////
typedef	uint8	Std_ReturnType;

typedef	struct
{
	uint16	vendorID;
	uint16	moduleID;
	uint8	instanceID;
	uint8	sw_major_version;
	uint8	sw_minor_version;
	uint8	sw_patch_version;
} Std_VersionInfoType;

//��ջ�����������
typedef uint16 OS_STK;

//��������ֵö��
typedef enum
{
	//ע�ⷵ��ֵΪ0��ʾ�޴���
	E_OS_ACCESS     =1,
	E_OS_CALLEVEL,
	E_OS_ID,
	E_OS_LIMIT,
	E_OS_NOFUNC,
	E_OS_RESOURCE,
	E_OS_STATE,
	E_OS_VALUE,
	
	//AUTOSAR���ӵĹ�������
	E_OS_SERVICEID,
	E_OS_ILLEGAL_ADDRESS,
	E_OS_MISSINGEND,
	E_OS_DISABLEDING,
	E_OS_STACKFAULT,
	E_OS_PROTECTION_MEMORY,
	E_OS_PROTECTION_TIME,
	E_OS_PROTECTION_ARRUVAL,
	E_OS_PROTECTION_LOCKED,
	E_OS_PROTECTION_ECXCEPTION,
	
	//������ǰ��ӵ�����
	E_FAIL,
    E_FALSE,
    E_EMPTY,
    E_FULL,
    E_BUSY,
    E_OverFlow,
    E_OverMax,
    E_OverMin,
    E_ReachMax,
    E_ReachMin
}ERROR;



#endif