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
//#define VCP/*��ƽ̨������Ҫע�͸þ�*/
/******************************************************************************
*    Include File Section
******************************************************************************/
#ifndef VCP
/*�ǰ�ȫ�����ƽ̨ʹ��ϵͳ�Դ�����*/
#include <stdio.h>

/*�ڴ�����*/
#define F_GL_MemSet memset

/*�ڴ濽��*/
#define F_GL_MemCpy memcpy
#else
/*ʹ�ð�ȫ�����ƽ̨�ṩ����*/
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
/*SSR������*/
#define c_SSR_SIZE (25U)

/*�����ֽڳ���*/
#define c_BYTE_SIZE (1250U)

/*SSE������*/
#define c_SSE_SIZE (20U)

/*���ݰ����峤��*/
#define c_BUFF_SIZE (1300U)
/******************************************************************************
*    Local Struct  Define Section
******************************************************************************/
/* �Զ��岼������ */
typedef enum TAG_Bool
{
    r_FALSE = 0x05555555U,/* ���� */
    r_TRUE = 0x0AAAAAAAU/* ��ȷ */
}E_Bool;

/* ����֡�ṹ�� */
typedef struct TAG_Msg
{
    UShort length;/*ʵʱ��ȫ���ݳ���*/
    UTiny buff[c_BUFF_SIZE];/*ʵʱ��ȫ���ݻ�����*/
}S_Msg;

/* ��ʱ֡�ṹ�� */
typedef struct TAG_SsMsg
{
    UShort length;/*��ʱ֡���ݳ���*/
    UTiny buff[c_SSR_SIZE];/*��ʱ֡���ݻ�����*/
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



