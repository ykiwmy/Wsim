/******************************************************************************
 *File Name	     :	GL_Basic_Type_Definition.h
 *Copyright	     :	Zhuzhou CSR Times Electric Co.,Ltd. All Rights Reserved.
 *Create Date    :	2011/03/06
 *Description    :	General Library's basic type definition
 *
 *+----------+---------------------------------------------------------------+
 *| Design   |    �����ͨ�ź�ϵͳͨ�ð�ȫ�����ƽ̨���GLͨ�ÿ�             |
 *|  File    |    ��ϸ���˵����-GL_Basic_Type_Definition���A.4             |
 *+----------+--------------+------------+-----------------------------------+
 *| Version  |    Author    |    Date    |            Description            |
 *+----------+--------------+------------+-----------------------------------+
 *| C.A.0001 | YanGuang     | 2011/03/06 |  File Create                      |
 *+----------+--------------+------------+-----------------------------------+
 *| C.A.0002 | YanGuang     | 2013/03/19 |(1)����F_GL_ReverseByte()          |
 *|          |              |            |(2)��INTEGER��ΪINTEGER32          |
 *|          |              |            |                                   |
 *+----------+--------------+------------+-----------------------------------+
 *| C.A.0003 | YanGuang     | 2013/10/16 |(1)�������ñ��뿪�ص��ļ�          |
 *|          |              |            |(2)�����뿪��ɾ��                  |
 *+----------+--------------+------------+-----------------------------------+
 *| C.A.0004 | YanGuang     | 2014/03/10 |(1)ɾ�� T_Cell �ṹ����            |
 *|          |              |            |(2)���UINTEGER32 ��������         |
 *+----------+--------------+------------+-----------------------------------+
 *
 ******************************************************************************/

/******************************************************************************
*    Multi-Include-Prevent Start Section
******************************************************************************/
#ifndef GL_BASIC_TYPE_DEFINITION_H_
#define GL_BASIC_TYPE_DEFINITION_H_

/******************************************************************************
*    Debug switch Section
******************************************************************************/



/******************************************************************************
*    Include File Section
******************************************************************************/
#include "GL_Compile_Switch_Definition.h"


/******************************************************************************
*    Global Macro Define Section
******************************************************************************/
/*�Զ��������������*/
typedef unsigned char  UINT8     ;
typedef signed   char  INT8      ;
typedef char           CHARACTER ;
typedef int            INTEGER32 ;
typedef unsigned int   UINTEGER32;
typedef float          REAL32    ;
typedef double         REAL64    ;

typedef unsigned short int UINT16;
typedef signed   short int INT16 ;
typedef unsigned /*long*/  int UINT32;
typedef signed   /*long*/  int INT32 ;



/*�����ָ��*/
#define ptrNULL          ((void*)0)

/*����1���Ӧ��Сʱ*/
#define c_DayToHour      (24UL)

/*����1Сʱ��Ӧ�ķ���*/
#define c_HourToMinute   (60UL)

/*����1���Ӷ�Ӧ������*/
#define c_MinuteToSecond (60UL)

/*����1���Ӷ�Ӧ�ĺ�����*/
#define c_SecondToMilSec (1000UL)

/*����1�����Ӧ��΢����*/
#define c_MilSecToMicSec (1000UL)

/*����1���Ӷ�Ӧ��΢����[1,000,000]*/
#define c_SecondToMicSec (c_SecondToMilSec*c_MilSecToMicSec)

/*����1���Ӷ�Ӧ�ĺ�����[60,000]*/
#define c_MinuteToMilSec (c_MinuteToSecond*c_SecondToMilSec)

/*����1Сʱ��Ӧ������[3,600]*/
#define c_HourToSecond   (c_HourToMinute*c_MinuteToSecond)

/*����1Сʱ��Ӧ�ĺ���[3,600,000]*/
#define c_HourToMilSec   (c_HourToSecond*c_SecondToMilSec)

/*����1���Ӧ������[86,400]*/
#define c_DayToSecond    (c_DayToHour*c_HourToSecond)

/*����1���Ӧ�ĺ�����[86,400,000]*/
#define c_DayToMilSec    (c_DayToSecond*c_SecondToMilSec)



/******************************************************************************
*    Global Struct    Define Section
******************************************************************************/

/*�Զ��岼��ֵ*/
typedef enum TAG_SysBool
{
  e_TRUE  = 0x05555555, /*��ʾ�߼�"��" */
  e_FALSE = 0x0AAAAAAA  /*��ʾ�߼�"��" */
}E_SysBool;

/*����ͨ��У���*/
typedef struct TAG_Checksum
{
  INT32  C1; /* [4 Bytes] ȡֵ[-0x80000000, 0x7FFFFFFF]*/
  INT32  C2; /* [4 Bytes] ȡֵ[-0x80000000, 0x7FFFFFFF]*/
}T_Checksum; /* [8 Bytes] */

/*����ϵͳʱ�ӵĸ�ʽ*/
typedef struct TAG_SysClk
{
  UINT32 Sec ; /*[4 Byte] ����,ȡֵ[0, 0xFFFFFFFF]*/
  UINT32 uSec; /*[4 Byte] ΢��,ȡֵ[0, 999999]    */
}T_SysClk;

/*����ʵʱʱ�ӵĸ�ʽ*/
typedef struct TAG_RealClk
{
  UINT16 Year ; /*[2 Byte] ���,ȡֵ[0~10000]   */
  UINT8  Month; /*[1 Byte] �·�,ȡֵ[1~12]      */
  UINT8  Day  ; /*[1 Byte] ����,ȡֵ[1~31]      */
  UINT32 MiSec; /*[4 Byte] ����,ȡֵ[0~86399999]*/
}T_RealClk;     /*[8 Byte]*/

/*����ͨ��ԭ����Ϣ*/
typedef struct TAG_CellMsg
{
  UINT8*  ptrMsg ; /* [4 Bytes] ȡֵ[�����ַ]     */
  UINT32  dataLen; /* [4 Bytes] ȡֵ[0, 0xFFFFFFFF]*/
}T_CellMsg;        /* [8 Bytes] */



/******************************************************************************
*    Global Variable Declare Section
******************************************************************************/
extern const T_RealClk c_DefaultRealClkF;
extern const T_RealClk c_DefaultRealClkM;
extern const T_SysClk  c_DefaultSysClk  ;
/******************************************************************************
*    Global Prototype Declare Section
******************************************************************************/
//void F_reverseByte(void* const ioData, const UINT32 inDataLen);
//void F_GL_ReverseByte(void* const ioData, const UINT32 inDataLen);
//E_SysBool F_GL_IsDefaultRealClk(const T_RealClk inRealClk);

/******************************************************************************
*    Multi-Include-Prevent End Section
******************************************************************************/

#endif



