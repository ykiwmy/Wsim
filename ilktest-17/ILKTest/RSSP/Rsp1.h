/******************************************************************************
*File Name     : rsp1.h
*Copyright     : Zhuzhou CSR Times Electric Co.,Ltd. All Rights Reserved.
*Create Date    :      2016/01/01
*Description    :      Place some description here.
*
*REV 1.0.0    Chenx   2016/01/01  File Create
*
******************************************************************************/

/******************************************************************************
*    Multi-Include-Prevent Start Section
******************************************************************************/
#ifndef RSP1_H
#define RSP1_H




/******************************************************************************
*    Debug switch Section
******************************************************************************/
//#define VCP/*非平台编译需要注释该句*/
/******************************************************************************
*    Include File Section
******************************************************************************/
#ifndef VCP
/*非安全计算机平台使用系统自带函数*/
#include <stdio.h>

/*内存设置*/
#define F_GL_MemSet memset

/*内存拷贝*/
#define F_GL_MemCpy memcpy
#else
/*使用安全计算机平台提供函数*/
#include "GL_Mem_Operation.h"
#endif
/******************************************************************************
*    Local Type Define Section
******************************************************************************/
typedef unsigned char UTiny;
typedef signed char STiny;
typedef unsigned short UShort;
typedef signed short SShort;
typedef unsigned short Word;
typedef unsigned long ULong;
typedef signed long SLong;
/******************************************************************************
*    Local Macro Define Section
******************************************************************************/
/*SSR包长度*/
#define c_SSR_SIZE (25U)

/*数据字节长度*/
#define c_BYTE_SIZE (1250U)

/*SSE包长度*/
#define c_SSE_SIZE (20U)

/*数据包整体长度*/
#define c_BUFF_SIZE (1300U)
/******************************************************************************
*    Local Struct  Define Section
******************************************************************************/
/* 自定义布尔类型 */
typedef enum TAG_Bool
{
    r_FALSE = 0x05555555U,/* 错误 */
    r_TRUE = 0x0AAAAAAAU/* 正确 */
}E_Bool;

/* 数据帧结构体 */
typedef struct TAG_Msg
{
    UShort length;/*实时安全数据长度*/
    UTiny buff[c_BUFF_SIZE];/*实时安全数据缓冲区*/
}S_Msg;

/* 对时帧结构体 */
typedef struct TAG_SsMsg
{
    UShort length;/*对时帧数据长度*/
    UTiny buff[c_SSR_SIZE];/*对时帧数据缓冲区*/
}S_SsMsg;

typedef enum E_MODE
{
	E_RSD = 0, E_BTM = 1
}E_MODE;
/******************************************************************************
*    Global Prototype Declare Section
******************************************************************************/
E_Bool Rsp1_Init(const UShort selfDevId_U16,
                 const UShort inCfgLen_U16,
                 UTiny * const inCfgBuf);
void Rsp1_UpdateClock(void);
UTiny Rsp1_CheckPackage(const UShort inDstDevId_U16,
                        const  UShort inLen_U16,
                        UTiny *inBuf,
                        UShort *outLen,
                        UTiny *outBuf);
E_Bool Rsp1_GetChanelStatus(const UShort inDstDevId_U16,
                            UShort * const outCycDiff);
E_Bool Rsp1_Close(const UShort inDstDevId_U16);
E_Bool Rsp1_Open(const UShort inDstDevId_U16);
UShort Rsp1_GetSynData(UTiny *outBuf);
void Rsp1_SetSynData(const UTiny *inBuf,
                     UShort inDataLength_U16);
UTiny Rsp1_PackageData(const UShort inDstDevId_U16,
                       S_Msg* inMsg,
                       S_Msg* outMsg,
                       S_SsMsg* outSseMsg,
                       S_SsMsg* outSsrMsg);
UShort Rsp1_GetRunningCode(UTiny *outBuf);

void LEU_Init();
void LEU_BtmCycle(const UTiny LEUIndex);
UTiny LEU_CheckBtmPackage(const UTiny LEUIndex,
							const UShort inLen_U16,
							UTiny *inBuf,
							UShort *outLen,
							UTiny *outBuf);
UTiny LEU_PackageBtmData(const UShort LEUIndex,
							S_Msg* outMsg,
							S_SsMsg* outSseMsg,
							S_SsMsg* outSsrMsg);
UTiny LEU_CheckRsdPackage(const UShort LinkIndex,
							const UShort inLen_U16,
							UTiny *inBuf);
UTiny LEU_PackageRsdData(const UShort LEUIndex,
						    S_Msg* inMsg,
							S_Msg* outMsg,
							S_SsMsg* outSseMsg,
							S_SsMsg* outSsrMsg);
UShort LEU_GetSynData(UTiny *outBuf);
void LEU_SetSynData(const UTiny *inBuf,
	UShort inDataLength_U16);

/******************************************************************************
*    Multi-Include-Prevent End Section
******************************************************************************/

#endif



