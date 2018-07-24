/******************************************************************************
 *File Name	     :	Vcp_File_Interface.h
 *Copyright	     :	Zhuzhou CSR Times Electric Co.,Ltd. All Rights Reserved.
 *Create Date    :	2014/02/15
 *Module Id      :  SD-SM_PAB-1012
 *Description    :	SM�ṩ���й�PAB���ļ������ӿ�
 *                  �Կ⺯������ʽ�ṩ
 *
 *+----------+---------------------------------------------------------------+
 *| Design   |    �����ͨ�ź�ϵͳͨ�ð�ȫ�����ƽ̨�������SM_PAB           |
 *|  File    |    �����ϸ���˵����Vcp_File_Interface���_B.1               |
 *+----------+--------------+------------+-----------------------------------+
 *| Version  |    Author    |    Date    |            Description            |
 *+----------+--------------+------------+-----------------------------------+
 *| C.A.0001 | ZhuHui       | 2014/02/15 |  File Create                      |
 *+----------+--------------+------------+-----------------------------------+
 *
 ******************************************************************************/

/******************************************************************************
 *    Multi-Include-Prevent Start Section
 ******************************************************************************/
#ifndef VCP_FILE_INTERFACE_H_
#define VCP_FILE_INTERFACE_H_

/******************************************************************************
 *    Debug switch Section
 ******************************************************************************/

/******************************************************************************
 *    Include File Section
 ******************************************************************************/
#include "GL_Basic_Type_Definition.h"


/******************************************************************************
 *    Global Macro Define Section
 ******************************************************************************/
/*����ļ���*/
#define c_VCP_MaxFileNum      (128UL)
/*����ļ�������*/
#define c_VCP_MaxFileNameLen  (63UL)
/*���ͬʱ��д�ļ���*/
#define c_VCP_MaxFileRwNum    (6UL)
/*��󻺳�ɾ���ļ���*/
#define c_VCP_MaxDelFileNum   (6UL)
/*��д�����С*/
#define c_VCP_RwBuffSize      (1024UL * 1024UL)
/*���볤��*/
#define c_VCP_SAlignLen       (4UL)

/*���ļ�Ŀ¼ F_VCP_FileDir*/
/*ȱ�ٿռ�                    */
#define c_VCP_DirLackSpace          (   -1L)

/*�� F_VCP_FileOpen*/
/*�򿪳ɹ�                    */
#define c_VCP_FOpenOk               ( 0xF0L)
/*���ڴ�, �Ժ��ѯ���      */
#define c_VCP_FOpenIng              ( 0xF1L)
/*�򿪹���                    */
#define c_VCP_FOpenErrFatal         (-0xFFL)
/*������ļ�������            */
#define c_VCP_FOpenErrNameLen       (-0xF1L)
/*��������д��              */
#define c_VCP_FOpenErrOverNum       (-0xF2L)
/*��ͻ, �ļ���δ�����Ĳ���    */
#define c_VCP_FOpenErrConflict      (-0xF3L)
/*д�Ѵ��ڵ��ļ�              */
/*�������ڵ��ļ�              */
#define c_VCP_FOpenErrExistent      (-0xF4L)
/*Flash�ռ䲻��               */
#define c_VCP_FOpenErrLackSpace     (-0xA1L)

/*д F_VCP_FileWrite*/
/*����д                      */
#define c_VCP_FWriteIng             ( 0xA2L)
/*д���, (Init)              */
#define c_VCP_FWriteDone            ( 0xA3L)
/*дʧ��                      */
#define c_VCP_FWriteErrFatal        (-0xAFL)
/*��Ч�ľ��                  */
#define c_VCP_FWriteErrHandle       (-0xA0L)
/*�ȴ���                    */
#define c_VCP_FWriteErrWaitOpen     (-0xA1L)
/*���Ȳ���ȷ(δ����)          */
#define c_VCP_FWriteErrImproperLen  (-0xA2L)
/*���ڹر�                    */
#define c_VCP_FWriteErrClosing      (-0xA3L)
/*��������������,���Ƭ       */
#define c_VCP_FWriteErrOverSize     (-0xA4L)
/*���������������,��ȴ�     */
#define c_VCP_FWriteErrWait         (-0xA5L)
/*Flash�ռ䲻��               */
#define c_VCP_FWriteErrLackSpace    (-0xA6L)

/*�� F_VCP_FileRead*/
/*���ڶ�                      */
#define c_VCP_FReadIng              ( 0xB2L)
/*�����(��������), (Init)    */
#define c_VCP_FReadDone             ( 0xB3L)
/*��ʧ��                      */
#define c_VCP_FReadErrFatal         (-0xBFL)
/*��Ч�ľ��                  */
#define c_VCP_FReadErrHandle        (-0xB0L)
/*�ȴ���                    */
#define c_VCP_FReadErrWaitOpen      (-0xB1L)
/*���Ȳ���ȷ(δ����)          */
#define c_VCP_FReadErrImproperLen   (-0xB2L)
/*���ڹر�                    */
#define c_VCP_FReadErrClosing       (-0xB3L)
/*�ȴ������                  */
#define c_VCP_FReadErrWaitDone      (-0xB4L)

/*ɾ F_VCP_FileDelete*/
/*ɾ�����(Init)              */
#define c_VCP_FDeleteFinish         ( 0xC0L)
/*��������ɾ                  */
#define c_VCP_FDeleteReq            ( 0xC1L)
/*����ɾ                      */
#define c_VCP_FDeleteIng            ( 0xC2L)
/*ɾʧ��, ������              */
#define c_VCP_FDeleteErrFatal       (-0xCFL)
/*������ļ�������            */
#define c_VCP_FDeleteErrNameLen     (-0xC1L)
/*�������ɾ����              */
#define c_VCP_FDeleteErrOverNum     (-0xC2L)
/*��ͻ, �ļ�δ�ر�            */
#define c_VCP_FDeleteErrConflict    (-0xC3L)
/*ɾ�����ڵ��ļ�              */
#define c_VCP_FDeleteErrInexistent  (-0xC4L)

/*�ر� F_VCP_FileClose*/
/*�ر����                    */
#define c_VCP_FCloseFinish          ( 0xF0L)
/*�رս�����                  */
#define c_VCP_FCloseIng             ( 0xF2L)
/*�ر�ʧ��                    */
#define c_VCP_FCloseErrFatal        (-0xFFL)
/*��Ч�ľ��                  */
#define c_VCP_FCloseErrHandle       (-0xF0L)
/*�ܾ���һ��رձ����д���  */
#define c_VCP_FCloseErrReject       (-0xF1L)
/*У�����(Init)              */
#define c_VCP_FCloseWErrCheck       (-0xA7L)

/*��ѯ����״̬ */
/*F_VCP_FileStatusInquire & F_VCP_FileStatusInquireExt*/
/*ͨ��*/
/*���                        */
/*R: ���رճɹ�               */
/*W: д�رղ�У��ɹ�         */
/*D: ɾ���                   */
#define c_VCP_FInqFinish            ( 0xF0L)
/*���������/д/ɾ            */
#define c_VCP_FInqReq               ( 0xF1L)
/*���ڽ��ж�/д/ɾ            */
#define c_VCP_FInqIng               ( 0xF2L)
/*��������/��/д          */
#define c_VCP_FInqDone              ( 0xF3L)
/*��Ч�ľ��                  */
#define c_VCP_FInqErrHandle         (-0xF0L)
/*����ִ��ʧ��,�رվ��       */
#define c_VCP_FInqErrFatal          (-0xFFL)
/*��ѯ->��->д*/
/*�ռ䲻��   */
#define c_VCP_FInqOpenWErrLackSpace (c_VCP_FOpenErrLackSpace)
/*��ѯ->д*/
/*д����, ���ڼ��            */
#define c_VCP_FInqWriteCheck        ( 0xA4L)
/*�ռ䲻��                    */
#define c_VCP_FInqWriteLackSpace    ( 0xA5L)
/*У�����     */
#define c_VCP_FInqWriteErrCheck     (c_VCP_FCloseWErrCheck)

/*�ͷž�� F_VCP_FileHandleRelease*/
/*�ͷž���ɹ�               */
#define c_VCP_FRlsSuccess           ( 0xF0L)
/*����ľ��                 */
#define c_VCP_FRlsErrHandle         (-0xF0L)
/*ɾ��δ���, �������ͷ�     */
#define c_VCP_FRlsErrReject         (-0xF1L)
/*�����ļ�����ɾ��, �޷��ͷ� */
#define c_VCP_FRlsErrWait           (-0xF2L)



/******************************************************************************
 *    Global Struct    Define Section
 ******************************************************************************/
/*�����ļ��򿪷�ʽ*/
typedef enum TAG_VCP_FileOpenOption
{
  e_VCP_FOpenRead        = 0,  /*����ʽ���ļ�              */
  e_VCP_FOpenWrite       = 1,  /*д���ļ���ʽ���ļ�, ��У��*/
  e_VCP_FOpenExchange    = 2,  /*�ļ���Ϣ����                */
  e_VCP_FOpenWriteNCheck = 3   /*д���ļ���ʽ���ļ�, ��У��*/
}E_VCP_FileOpenOpt;

/*����*/
typedef struct TAG_VCP_Progress
{
  UINT32 Total    ; /* ��������             */
  UINT32 Done     ; /* �������             */
  UINT32 LastQuota; /* ���һ������������� */
}T_VCP_Progress;

/*�����ļ���ϸ��Ϣ*/
typedef struct TAG_VCP_FileDetail
{
  UINT32     Size ;  /*�ļ���С    */
  T_RealClk  CTime;  /*����ʱ��    */
  T_RealClk  LTime;  /*������ʱ��*/
}T_VCP_FileDetail;

/*�����ļ��б���Ϣ*/
typedef struct TAG_VCP_FileDir
{
  CHARACTER        Name[c_VCP_MaxFileNameLen + 1UL];  /*�ļ���  */
  T_VCP_FileDetail Detail                          ;  /*�ļ���Ϣ*/
}T_VCP_FileDir;

/*�ļ���������*/
typedef enum TAG_VCP_FileOperateType
{
  e_VCP_FOperateNone     = 0,  /*��*/
  e_VCP_FOperateReading  = 2,  /*��*/
  e_VCP_FOperateWriting  = 3,  /*д*/
  e_VCP_FOperateDeleting = 5   /*ɾ*/
}E_VCP_FileOperateType;

/*�ļ�����״̬*/
typedef enum TAG_VCP_FileExistStatus
{
  e_VCP_FExistUnknown = 0,  /*δ֪  */
  e_VCP_FExistTrue    = 1,  /*����  */
  e_VCP_FExistFalse   = 2   /*������*/
}E_VCP_FileExistStatus;

/*�ļ�������Ϣ*/
typedef struct TAG_VCP_FileOperateInfo
{
  E_VCP_FileOperateType Type    ;  /*����*/
  INT32                 Status  ;  /*״̬*/
  T_VCP_Progress        Progress;  /*����*/
  T_Checksum            CheckSum;  /*У��*/
}T_VCP_FileOperateInfo;

/*�ļ�������Ϣ*/
typedef struct TAG_VCP_FileExistInfo
{
  E_VCP_FileExistStatus Status;  /*״̬*/
  T_VCP_FileDetail      Detail;  /*��Ϣ*/
}T_VCP_FileExistInfo;

/*�ļ�������Ϣ*/
typedef struct TAG_VCP_FileExInfo
{
  T_VCP_FileExistInfo   ExistInfo  ;  /*������Ϣ*/
  T_VCP_FileOperateInfo OperateInfo;  /*������Ϣ*/
}T_VCP_FileExInfo;


///******************************************************************************
// *    Global Variable Declare Section
// ******************************************************************************/
//
//
///******************************************************************************
// *    Global Prototype Declare Section
// ******************************************************************************/
///******************************************************************************
// *Name      : F_VCP_GetIdleSpace
// *Function  : ��ȡFlash���пռ�
// *Para      : void
// *Return    : ���пռ��С(�ֽ�)
// *Note      :
// ******************************************************************************/
//INT32 F_VCP_GetIdleSpace(void);
//
//
///******************************************************************************
// *Name      : F_VCP_FileExist
// *Function  : ��ѯ�ļ��Ƿ����
// *Para      : @inFileName : �����ļ���
// *Return    : ����ļ����ڷ���e_TRUE�����򷵻�e_FALSE
// *Note      :
// ******************************************************************************/
//E_SysBool F_VCP_FileExist(const CHARACTER* inFileName);
//
//
///******************************************************************************
// *Name      : F_VCP_FileInquire
// *Function  : ��ȡ�ļ���Ϣ
// *Para      : @inFileName    : �����ļ���
// *            @outFileDetail : ����ļ���Ϣ(����ΪptrNULL)
// *Return    : �����ȡ�ɹ�=>����e_TRUE�����򷵻�e_FALSE
// *Note      : ��һ��������д���ļ��ú�����Ч
// ******************************************************************************/
//E_SysBool F_VCP_FileInquire(const CHARACTER*  inFileName,
//                            T_VCP_FileDetail* outFileDetail);
//
//
///******************************************************************************
// *Name      : F_VCP_FileDir
// *Function  : �г��ļ��б�
// *Para      : @outFileDir : ����ļ��б�
// *            @inNum      : �����ļ��б�������
// *Return    : c_VCP_DirLackSpace��ʾ������̫С��������ʾ��ȡ���ļ�����
// *Note      :
// ******************************************************************************/
//INT32 F_VCP_FileDir(T_VCP_FileDir outFileDir[],
//                    const INT32   inNum);
//
//
///******************************************************************************
// *Name      : F_VCP_FileOpen
// *Function  : ���ļ�(R/W/E)
// *Para      : @inFileName : �����ļ���
// *            @inOpenOpt  : ���������
// *            @outHUsr    : ����û��ļ����
// *Return    : ���ش�״̬
// *Note      :
// ******************************************************************************/
//INT32 F_VCP_FileOpen(const CHARACTER*        inFileName,
//                     const E_VCP_FileOpenOpt inOpenOpt,
//                     INT32* const            outHUsr);
//
//
///******************************************************************************
// *Name      : F_VCP_FileRead
// *Function  : ��ȡ�ļ�
// *Para      : @inHUsr     : �����û����
// *            @inReadLen  : �����ȡ����
// *            @inReadBuff : �����ȡ���ݻ���
// *Return    : ���ض�ȡ״̬
// *Note      : �����HUsr֮ǰ�Ķ������Ѿ���ɣ�������Ӧ�˴�����
// ******************************************************************************/
//INT32 F_VCP_FileRead(const INT32  inHUsr,
//                     const UINT32 inReadLen,
//                     UINT8*       inReadBuff);
//
//
///******************************************************************************
// *Name      : F_VCP_FileWrite
// *Function  : д���ļ�
// *Para      : @inHUsr      : �����û����
// *            @inWriteLen  : ����д�볤��
// *            @inWriteBuff : ����д�����ݻ���
// *Return    : ����д��״̬
// *Note      :
// ******************************************************************************/
//INT32 F_VCP_FileWrite(const INT32  inHUsr,
//                      const UINT32 inWriteLen,
//                      UINT8*       inWriteBuff);
//
//
///******************************************************************************
// *Name      : F_VCP_FileClose / F_VCP_FileCloseExt
// *Function  : �رվ��
// *Para      : @inHUsr    : �����û����
// *            @inIsForce : �����Ƿ�ǿ�ƹر� (��Ч����)
// *Return    : ���عر�״̬
// *Note      : Ϊ��������, ��������Ϊ������������
// *            F_VCP_FileClose    : �����ڽ׶�, ϵͳ�������ͷž��
// *            F_VCP_FileCloseExt : �����ڽ׶�, ϵͳ���������ͷž��
// ******************************************************************************/
//INT32 F_VCP_FileClose(const INT32     inHUsr,
//                      const E_SysBool inIsForce);
//INT32 F_VCP_FileCloseExt(const INT32 inHUsr);
//
//
///******************************************************************************
// *Name      : F_VCP_FileDelete
// *Function  : ɾ���ļ�
// *Para      : @inFileName : �����ļ���
// *            @outHUsr    : ����û����
// *Return    : ����ɾ��״̬
// *Note      :
// ******************************************************************************/
//INT32 F_VCP_FileDelete(const CHARACTER* inFileName,
//                       INT32*           outHUsr);
//
//
///******************************************************************************
// *Name      : F_VCP_FileStatusInquire / F_VCP_FileStatusInquireExt
// *Function  : ��ѯ�ļ����״̬
// *Para      : @inHUsr         : �����û����
// *            @outProgress    : �������
// *            @outOperateInfo : ���������Ϣ
// *Return    : �����ļ������Ӧ״̬
// *Note      : 1. Ϊ��֤������, F_VCP_FileStatusInquire��ѯ����c_VCP_FInqFinish��
// *               ��ֵ, ƽ̨�������ͷž��
// *            2. Ϊ�����û�ʹ��, F_VCP_FileStatusInquireExt�κ������,
// *               ƽ̨�����ͷž��, ��Ҫ�û�ͨ��F_VCP_FileHandleRelease�ͷ�
// ******************************************************************************/
//INT32 F_VCP_FileStatusInquire(const INT32     inHUsr,
//                              T_VCP_Progress* outProgress);
//INT32 F_VCP_FileStatusInquireExt(const INT32            inHUsr,
//                                 T_VCP_FileOperateInfo* outOperateInfo);
//
//
//
///******************************************************************************
// *Name      : F_VCP_FileHandleRelease
// *Function  : �ͷž��
// *Para      : @inHUsr : �����û����
// *Return    : �����ļ�����ͷ�״̬
// *Note      :
// ******************************************************************************/
//INT32 F_VCP_FileHandleRelease(const INT32 inHUsr);
//
//
///******************************************************************************
// *Name      : F_VCP_FileExInfoInquire
// *Function  : ��ѯ�������ļ���Ϣ
// *Para      : @inHUsr    : �����û����
// *            @outExInfo : ���������Ϣ
// *Return    : ��������ȷ����e_TRUE�����򷵻�e_FALSE
// *Note      :
// ******************************************************************************/
//E_SysBool F_VCP_FileExInfoInquire(const INT32       inHUsr,
//                                  T_VCP_FileExInfo* outExInfo);

#endif



