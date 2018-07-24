/******************************************************************************
 *File Name	     :	Vcp_Service_Interface.h
 *Copyright	     :	Zhuzhou CSR Times Electric Co.,Ltd. All Rights Reserved.
 *Create Date    :	2012/03/11
 *Module Id      :  SD-SM_PAB-1010
 *Description    :	SM�ṩ���й�PAB��BSP�ӿ�(���ļ�ֻʵ��ETH�ڰ�)
 *                  ����������ʵ�ֶ��ѿ⺯������ʽ�ṩ
 *
 *+----------+---------------------------------------------------------------+
 *| Design   |    �����ͨ�ź�ϵͳͨ�ð�ȫ�����ƽ̨�������SM_PAB           |
 *|  File    |    �����ϸ���˵����Vcp_Service_Interface���_B.2            |
 *+----------+--------------+------------+-----------------------------------+
 *| Version  |    Author    |    Date    |            Description            |
 *+----------+--------------+------------+-----------------------------------+
 *| C.A.0001 | YanGuang     | 2012/03/11 |  File Create                      |
 *+----------+--------------+------------+-----------------------------------+
 *| C.A.0002 | YanGuang     | 2013/04/17 |(1)�Ժ���ָ�����͵��β����const   |
 *|          |              |            |   �޶���                          |
 *|          |              |            |(2)�޸�TAG_CellMsg��˵��           |
 *|          |              |            |(3)�޸�F_VCP_UsrDataRecv�β�       |
 *|          |              |            |   �޸�F_VCP_UsrDataRecvMutual�β� |
 *|          |              |            |   �޸�F_VCP_DevInfSummary�β�     |
 *+----------+--------------+------------+-----------------------------------+
 *| C.A.0003 | YanGuang     | 2013/08/28 |(1)ɾ������״̬                    |
 *+----------+--------------+------------+-----------------------------------+
 *| C.A.0004 | YanGuang     | 2013/10/29 |���ӽӿں���:                      |
 *|          |              |            |(1)F_VCP_UsrIndicator              |
 *|          |              |            |(2)F_VCP_UsrInquiryParamVersion    |
 *|          |              |            |���ӽӿڳ���:                      |
 *|          |              |            |(1)c_UsrIndicatorA                 |
 *|          |              |            |(2)c_UsrIndicatorB                 |
 *|          |              |            |(3)c_UsrIndicatorOn                |
 *|          |              |            |(4)c_UsrIndicatorOff               |
 *|          |              |            |(5)c_InquiryLoc                    |
 *|          |              |            |(6)c_InquiryRem                    |
 *+----------+--------------+------------+-----------------------------------+
 *| C.A.0005 | ZhuHui       | 2014/03/20 |(1)ɾ���ļ�������ݽṹ������      |
 *|          |              |            |(2)��ӽӿ�F_VCP_GetVcpId          |
 *|          |              |            |           F_VCP_GetHwVersion      |
 *+----------+--------------+------------+-----------------------------------+
 *| C.A.0006 | ZhuHui       | 2014/08/20 |(1)��E_SysDevStatus�����һ��״̬  |
 *|          |              |            |   e_SysDevTesting                 |
 *+----------+--------------+------------+-----------------------------------+
 *| C.A.0007 | ZhuHui       | 2014/10/10 |(1)��ӽӿ�F_VCP_GetLocBspVersion  |
 *|          |              |            |(2)�޸�F_VCP_UsrDataSend           |
 *|          |              |            |   ����޷���ƽ̨���跢���������  |
 *+----------+--------------+------------+-----------------------------------+
 *| C.B.0001 | ZhuHui       | 2015/08/20 |(1)Ethģʽ��ͨ���½ӿڷ���/����    |
 *|          |              |            |   ϵ���û�����                    |
 *|          |              |            |   ��Ӻ��ʶϵ���û�������󳤶�  |
 *+----------+--------------+------------+-----------------------------------+
 *
 ******************************************************************************/

/******************************************************************************
*    Multi-Include-Prevent Start Section
******************************************************************************/
#ifndef VCP_SERVICE_INTERFACE_H_
#define VCP_SERVICE_INTERFACE_H_

/******************************************************************************
*    Debug switch Section
******************************************************************************/

/******************************************************************************
*    Include File Section
******************************************************************************/
#include "GL_Basic_Type_Definition.h"
#include "Vcp_Public_Definition.h"



/******************************************************************************
*    Global Macro Define Section
******************************************************************************/



/******************************************************************************
*    Global Struct    Define Section
******************************************************************************/
/*����ϵͳ�豸״̬*/
/*C.A.0003  ZhuHui  2013/08/21  ɾ������״̬*/
typedef  enum TAG_SysDevStatus
{
  e_SysDevNotExist= 1, /*�豸������   */
  e_SysDevInit    = 2, /*�豸��ʼ��   */
  e_SysDevNormal  = 4, /*�豸����״̬ */
  e_SysDevFault   = 5, /*�豸����״̬ */
  e_SysDevTesting = 6  /*�豸�Լ�     */
}E_SysDevStatus;

/*�����û����ݽ���״̬*/
typedef enum TAG_UsrRxStatus
{
  e_UsrRxLcEmpty  = 1,  /* ������   */
  e_UsrRxLcAppNew = 2,  /* ������   */
  e_UsrRxLcAppOld = 3,  /* ������   */
  e_UsrRxLcMsgErr = 4,  /* ���ݴ��� */ /* �������ݳ���������󳤶� */
  e_UsrRecvCfgErr = 5,  /* ���ô��� */ /* �û��ṩ������� */
  e_UsrRxErrDev   = 6   /* �����豸 */
}E_UsrRxStatus;

typedef enum TAG_UsrTxStatus
{
  e_UsrTxSuccess   = 1,  /* ���ͳɹ� */
  e_UsrTxErrDev    = 2,  /* �����豸 */
  e_UsrTxErrData   = 3,  /* �������� */ /* ��ָ��or���ݳ��� */
  e_UsrTxErrRepeat = 4,  /* �ظ����� */
  e_UsrTxErrStatus = 5   /* ����״̬ */ /* �������������״̬��Vcp�豸�������� */
}E_UsrTxStatus;

/*�����豸���ڵ�ϸ��*/
typedef struct TAG_DevDetail
{
  UINT16 DevType; /*[2 byte] �豸����           */
  UINT8  DevCab ; /*[1 byte] �豸�Ĳ�����     */
  UINT8  DevPos ; /*[1 byte] �豸�Ĳ��λ��     */
  UINT8  AccCab ; /*[1 byte] ����Ĳ�����     */
  UINT8  AccPos ; /*[1 byte] ����Ĳ���λ��     */
  UINT8  AccSys ; /*[1 byte] �����ϵ��         */
  UINT8  AppId  ; /*[1 byte] ָʾ�豸��"Ӧ��ID" */
}T_DevDetail;

/* �����豸������Ϣ */
typedef struct TAG_DevExInfo
{
  UINT32 TxMsgLen; /*[4 byte] �豸���ͳ���   */
  UINT32 RxMsgLen; /*[4 byte] �豸���ճ���   */
  UINT32 SendNum ; /*[4 byte] �豸���͹ܵ��� */
  UINT32 RecvNum ; /*[4 byte] �豸���չܵ��� */
}T_DevExInfo;

/*����һ��VCP�豸����Ϣ*/
typedef struct TAG_SysDevInf
{
  T_DevDetail    Detail; /*[8 byte]*/
  E_SysDevStatus Status; /*[4 byte]*/
}T_SysDevInf;

/*�豸�汾��Ϣ*/
typedef struct TAG_UsrDevVerInf
{
  /*����Id����*/
  #define c_VCP_ManIdLen (12UL)

  UINT32    SwProc               ; /* �ں˳�������汾��       */
  UINT32    SwComm               ; /* ͨ�ų�������汾��       */
  T_RealClk Date                 ; /* ��������                 */
  UINT32    HwVer                ; /* Ӳ���汾��               */
  UINT8     ManId[c_VCP_ManIdLen]; /* �������                 */
  UINT32    RunDura              ; /* ����ʱ��                 */
  UINT8     Resolution           ; /* ����ʱ��ֵ�ķֱ���(����) */
  UINT8     Reserve[2]           ; /* Ԥ��                     */
}T_UsrDevVerInf;

/*�����豸������ϸ��Ϣ*/
typedef struct TAG_DevCfgDetail
{
  /*����豸������*/
  #define c_VCP_MaxDevNameLen (16UL)

  CHARACTER    DevName[c_VCP_MaxDevNameLen]; /* �豸��   */
  T_DevDetail  BasicInfo                   ; /* ������Ϣ */
  T_DevExInfo  ExInfo                      ; /* ������Ϣ */
}T_DevCfgDetail;

/*�����û�ָʾ��*/
#define c_UsrIndicatorA (0xAAUL)
#define c_UsrIndicatorB (0xBBUL)

/*�����û�ָʾ������*/
#define c_UsrIndicatorOff (0UL)
#define c_UsrIndicatorOn  (1UL)

/*��ѯ�������������ļ���ʶ*/
#define c_InquiryLoc (0UL)
/*��ѯ��һϵ���������ļ���ʶ*/
#define c_InquiryRem (1UL)

/* ���ϵ�以�����ݳ��� */
#define c_VCP_MaxMutualDataLen4Eth (100UL * 1024UL)
#define c_VCP_MaxMutualDataLen4Flr (1200UL)



/******************************************************************************
*    Global Variable Declare Section
******************************************************************************/


/******************************************************************************
*    Global Prototype Declare Section
******************************************************************************/
/******************************************************************************
 *Name    : F_VCP_UsrDataWatch
 *Function: �ṩ���û�ÿ�����ڽ��е������ݼ��Ľӿ�
 *          (������ε���ʱ�ı����ڵ��ۼ�ʵʱ���ݳ����Ѿ�����1400�ֽ��򱾺���
 *           ����Դ˴����ݽ��з���,������e_FALSE)
 *Para    : @inData    : Ϊ��Ϣ�����׵�ַ
 *          @inDataLen : Ϊ��Ϣ���ݳ���(���ֽ�Ϊ��λ)
 *Return  : ����ɹ��򷵻�e_TRUE, ���򷵻�e_FALSE
 *Note    :
******************************************************************************/
E_SysBool F_VCP_UsrDataWatch(const UINT8* const inData,
                             const UINT32       inDataLen);


/******************************************************************************
 *Name    : F_VCP_UsrDataRecv
 *Function: ���û��ṩ�������豸ָʾ(���ṩinDevType��inAppId ��inDevSys)��ɻ�
 *          ��ָ���豸�����ݣ�����(���ṩinDevType��inAppId)�ɻ����ϵ�豸�����ݡ�
 *          �û���ȡ���ݳɹ���ͬ�������ٴλ�ȡ��ͬ�豸�����ݣ�
 *          ������᷵�������ظ���ʶ(e_RxLcAppOld).
 *Para    : @inDevType : �����豸���߼�����
 *          @inAppId   : �����豸��Ӧ��ID
 *          @inDevSys  : �����豸������ϵ��
 *          @ioMsg     : ����/������ݵĻ���,��ṹ����:
 *                      typedef  struct TAG_CellMsg
 *                      {
 *                        UINT8*  ptrMsg ; [>Out: �洢���׵�ַ               <]
 *                        UINT32  dataLen; [>I/O: �洢������/�洢�������ݳ���<]
 *                      } T_CellMsg;
 *Return  : ���� E_UsrRxStatus, ָʾ��ȡ���ݵ�״̬
 *           . e_UsrRxLcEmpty  = 1, �߼�ͨ������Ϊ��
 *           . e_UsrRxLcAppNew = 2, �߼�ͨ��(�û�)��ϢΪ��
 *           . e_UsrRxLcAppOld = 3, �߼�ͨ��(�û�)��ϢΪ��
 *           . e_UsrRxLcMsgErr = 4, �߼�ͨ������Ϣ����
 *           . e_UsrRecvCfgErr = 5  �û��ṩ�Ľ���������Ϣ����
 *Note    :
 ******************************************************************************/
E_UsrRxStatus F_VCP_UsrDataRecv(const UINT32 inDevType,
                                const UINT32 inAppId,
                                const UINT32 inDevSys,
                                T_CellMsg*   ioMsg);

/******************************************************************************
 *Name    : F_VCP_UsrDataRecvEx
 *Function: ���ָ���豸������ ��ǿ��
 *Para    : @inDevType: �����豸���߼�����
 *          @inAppId  : �����豸��Ӧ��ID
 *          @inDevSys : �����豸������ϵ��
 *          @inPipeId : ����ܵ���
 *          @ioUsrBuff: ����/������ݵĻ���
 *Return  : ���� E_UsrRxStatus, ָʾ��ȡ���ݵ�״̬
 *Note    :
 ******************************************************************************/
E_UsrRxStatus F_VCP_UsrDataRecvEx(const UINT32 inDevType,
                                  const UINT32 inAppId,
                                  const UINT32 inDevSys,
                                  const UINT32 inPipeId,
                                  T_UsrBuff*   ioUsrBuff);


/******************************************************************************
 *Name    : F_VCP_UsrDataRecvMutual
 *Function: ��ϵ�û�������ϵ�û����͵�����
 *Para    :  @ioMsg : ����/������ݵĻ���,��ṹ����:
 *                    typedef  struct TAG_CellMsg
 *                    {
 *                      UINT8*  ptrMsg ; [>Out: �洢���׵�ַ               <]
 *                      UINT32  dataLen; [>I/O: �洢������/�洢�������ݳ���<]
 *                    } T_CellMsg;
 *Return  : ���� E_UsrRxStatus, ָʾ��ȡ���ݵ�״̬
 *           . e_UsrRxLcEmpty  = 1, �߼�ͨ������Ϊ��
 *           . e_UsrRxLcAppNew = 2, �߼�ͨ��(�û�)��ϢΪ��
 *           . e_UsrRxLcAppOld = 3, �߼�ͨ��(�û�)��ϢΪ��
 *           . e_UsrRxLcMsgErr = 4, �߼�ͨ������Ϣ����
 *           . e_UsrRecvCfgErr = 5  �û��ṩ�Ľ���������Ϣ����
 *Note    :
******************************************************************************/
E_UsrRxStatus F_VCP_UsrDataRecvMutual(T_CellMsg* const ioMsg);


/******************************************************************************
 *Name    : F_VCP_UsrDataSend
 *Function: ��ָ���豸��������
 *Para    : @inDevType: �����豸���߼�����
 *          @inAppId  : �����豸��Ӧ��ID
 *          @inDevSys : �����豸������ϵ��
 *          @inData   : �����û��ṩ���ڴ����ݵĻ���
 *          @inDataLen: �����û��ṩ��������ݵĳ���
 *Return  : ���ͳɹ�=>����e_TRUE, ���򷵻�e_TRUE
 *Note    : ��������ͬһ�豸�ظ�����
******************************************************************************/
E_SysBool F_VCP_UsrDataSend(const UINT32 inDevType,
                            const UINT32 inAppId,
                            const UINT32 inDevSys,
                            const void*  inData,
                            const UINT32 inDataLen);



/******************************************************************************
 *Name    : F_VCP_UsrDataSendEx
 *Function: ��ָ���豸�������� ��ǿ��
 *Para    : @inDevType: �����豸���߼�����
 *          @inAppId  : �����豸��Ӧ��ID
 *          @inDevSys : �����豸������ϵ��
 *          @inPipeId : ����ָ���Ĺܵ���[0-14], �Ƕ�ܵ��豸��0
 *          @inData   : �����û��ṩ���ڴ����ݵĻ���
 *          @inDataLen: �����û��ṩ��������ݵĳ���
 *Return  : ����״̬
 *Note    : ��������ͬһ�豸�ظ�����
 ******************************************************************************/
E_UsrTxStatus F_VCP_UsrDataSendEx(const UINT32 inDevType,
                                  const UINT32 inAppId,
                                  const UINT32 inDevSys,
                                  const UINT32 inPipeId,
                                  const void*  inData,
                                  const UINT32 inDataLen);



/******************************************************************************
 *Name    : F_VCP_UsrDataSendMutual
 *Function: �����û��������û��������ݵĽӿ�
 *Para    : @inData    : ��������
 *          @inDataLen : �������ݵĳ���
 *Return  : �ɹ��򷵻�e_TRUE, ���򷵻�e_TRUE��
 *Note    : һ���������ظ����ã�ֻ�������һ������
******************************************************************************/
E_SysBool F_VCP_UsrDataSendMutual(const void* const inData,
                                  const UINT32      inDataLen);

/******************************************************************************
 *Name    : F_VCP_UsrHaltSystem
 *Function: ���û������жϳ����ش������ҪֹͣӦ�ô���ʱ���ô˽ӿں�����
 *          ���������洢���ϴ���(�Ա��û�����ά��)����5���ں���ִֹ���û�Ӧ�ô�
 *          �����
 *Para    : @inErrCode: ������ϴ���
 *Return  : void
 *Note    :
 *C.A.0002  YanGuang  2013/04/17  �޸�ԭ���
 *                                �����������ڿ�ʼ��ִֹ���û�Ӧ�ô������
 *                                ��Ϊ ��������ִֹ���û�Ӧ�ô������
******************************************************************************/
void F_VCP_UsrHaltSystem(const UINT32 inErrCode);


/******************************************************************************
 *Name    : F_VCP_DevInfSummary
 *Function: �û��ɲ�ѯ����VCP�豸�ĵ�ǰ״̬
 *Para    : @outDevInfArray: �������VCP�豸״̬�����飬״̬��������
 *           . e_SysDevNotExist = 1, �豸������
 *           . e_SysDevInit     = 2, �豸��ʼ��
 *           . e_SysDevNormal   = 4, �豸����״̬
 *           . e_SysDevFault    = 5, �豸����״̬
 *           . e_SysDevTesting  = 6  �豸�Լ�
 *          @ioDevInfTotal : �����û��ṩ���������
 *                           �������VCP�豸������(����ָʾ��һ���βε����鳤��)
 *Return  : void
 *Note    :
 *C.A.0002  YanGuang  2013/04/17  ���β���outDevInfTotal��ΪioDevInfTotal
******************************************************************************/
void  F_VCP_DevInfSummary(T_SysDevInf* const outDevInfArray,
                          UINT32* const      ioDevInfTotal);



/******************************************************************************
 *Name    : F_VCP_InquiryDevStatus
 *Function: �ṩ���û����������VCP�豸ָ����Ϣ��ȡVCP�豸�ĵ�ǰ״̬�Ľӿ�
 *Para    : @inDevType: �����豸���߼�����
 *          @inAppId  : �����豸��Ӧ�ñ��
 *          @inClique : �����豸����ϵ���ʶ
 *Return  : ����ָ���豸����ϸ��Ϣ(�����豸������ ״̬ E_SysDevStatus):
 *          . e_SysDevNotExist = 1, �豸������
 *          . e_SysDevInit     = 2, �豸��ʼ��
 *          . e_SysDevNormal   = 4, �豸����״̬
 *          . e_SysDevFault    = 5, �豸����״̬
 *          . e_SysDevTesting  = 6  �豸�Լ�
 *Note    :
******************************************************************************/
T_SysDevInf F_VCP_InquiryDevStatus(const UINT32 inDevType,
                                   const UINT32 inAppId,
                                   const UINT32 inClique);


/******************************************************************************
 *Name    : F_VCP_InquirySwVersion
 *Function: �ṩ���û���ѯ�������������汾��ͨ������汾�Ľӿ�
 *Para    : @inDevType      : �����豸���߼�����
 *          @inAppId        : �����豸��Ӧ�ñ��
 *          @inAccSys       : �����豸����ϵ���ʶ
 *          @outProcVersion : �����������汾
 *          @outCommVersion : ���ͨѶ����汾
 *
 *          |  �汾��ʶ  |  �汾����  |  ������  |  �汾���  |
 *          |-- 1 Byte --|-- 1 Byte --|-- 1 Byte --|-- 1 Byte --|
 *          �汾��ʶ��'V'
 *          �汾���ͣ�'A' - 'Z'
 *          �汾��ţ�1 - 15
 *          �����ţ���ʽ�汾��0
 *                    ���԰汾��'a' - 'z'
 *
 *          ��ʾʾ����
 *          0x56 -> 'V'
 *          0x43 -> 'C'
 *          0x61 -> 'a'
 *          0x56430001UL -> V.C.0001   ��ʽ��
 *          0x56436101UL -> V.C.0001a  ���԰�
 *
 *Return  : �����ѯ��ȷ�򷵻�e_TRUE, ����e_FALSE
 *Note    :
******************************************************************************/
E_SysBool  F_VCP_InquirySwVersion(const UINT32  inDevType,
                                  const UINT32  inAppId,
                                  const UINT32  inAccSys,
                                  UINT32* const outProcVersion,
                                  UINT32* const outCommVersion);


/******************************************************************************
 *Name    : F_VCP_SystemClkGet
 *Function: �ṩ���û���ȡϵͳʱ�ӵĽӿ�
 *Para    : @outSysClk: �����ǰϵͳʱ��(���ʽ����:)
 *          typedef  struct TAG_SysClk
 *          {
 *            UINT32 Sec ; [>[4 Byte] ����  <]
 *            UINT32 uSec; [>[4 Byte] ΢����<]
 *          }T_SysClk;
 *Return  : void
 *Note    :
******************************************************************************/
void  F_VCP_SystemClkGet(T_SysClk* const outSysClk);


/******************************************************************************
 *Name    : F_VCP_CalendarClkGet
 *Function: �ṩ���û���ȡ����ʱ�ӵĽӿ�
 *Para    : @outClk: �������ʱ��(���ʽ����:)
 *          typedef  struct TAG_CalendarClk
 *          {
 *            UINT32 Year    ;  [>[4 Byte] ���  <]
 *            UINT32 Mon     ;  [>[4 Byte] �·�  <]
 *            UINT32 Day     ;  [>[4 Byte] ����  <]
 *            UINT32 Hour    ;  [>[4 Byte] Сʱ  <]
 *            UINT32 Min     ;  [>[4 Byte] ����  <]
 *            UINT32 Sec     ;  [>[4 Byte] ����  <]
 *            UINT32 MiSec   ;  [>[4 Byte] ������<]
 *            UINT32 WeekDay ;  [>[4 Byte] ���ڼ�<]
 *          }T_CalendarClk;
 *Return  : void
 *Note    :
 *****************************************************************************/
E_SysBool  F_VCP_CalendarClkGet(T_CalendarClk* const outClk);


/******************************************************************************
 *Name    : F_VCP_CalendarClkSet
 *Function: ���ṩ���û���������ʱ�ӵĽӿ�(ע���ֶ�WeekDay�û����Բ���д)
 *Para    : @inClk: ��������ʱ��(���ʽ����:)
 *          typedef  struct TAG_CalendarClk
 *          {
 *            UINT32 Year    ;  [>[4 Byte] ���  <]
 *            UINT32 Mon     ;  [>[4 Byte] �·�  <]
 *            UINT32 Day     ;  [>[4 Byte] ����  <]
 *            UINT32 Hour    ;  [>[4 Byte] Сʱ  <]
 *            UINT32 Min     ;  [>[4 Byte] ����  <]
 *            UINT32 Sec     ;  [>[4 Byte] ����  <]
 *            UINT32 MiSec   ;  [>[4 Byte] ������<]
 *            UINT32 WeekDay ;  [>[4 Byte] ���ڼ�<]
 *          }T_CalendarClk;
 *Return  : ���óɹ�����e_TRUE,���򷵻�e_FALSE
 *Note    : 2���ں���Ч
******************************************************************************/
E_SysBool  F_VCP_CalendarClkSet(const T_CalendarClk* const inClk);


/******************************************************************************
 *Name    : F_VCP_GetSystemId
 *Function: �ṩ���û���ȡϵ����Ϣ�Ľӿ�
 *Para    : @outSysId: ���ϵ����Ϣ
 *           . 0x01��Iϵ;
 *           . 0x02��IIϵ;
 *Return  : void
 *Note    :
******************************************************************************/
void  F_VCP_GetSystemId(UINT32* const outSysId);


/******************************************************************************
 *Name    : F_VCP_GetSwId
 *Function: �ṩ���û���ȡ����ȷ�Ϸ�ʽ�Ľӿ�
 *Para    : @outSwId : �������ȷ�Ϸ�ʽ
 *           . 0x00: �Զ�ģʽ
 *           . 0x01���ֶ�ģʽ, I ϵ
 *           . 0x02���ֶ�ģʽ, IIϵ
 *Return  : void
 *Note    :
******************************************************************************/
void F_VCP_GetSwId(UINT32* const outSwId);


/******************************************************************************
 *Name    : F_VCP_GetMasterId
 *Function: �ṩ���û���ȡ����ϵ����Ϣ�Ľӿ�
 *Para    : @outSysId: ���ϵ����Ϣ
 *           . 0x01��Iϵ;
 *           . 0x02��IIϵ;
 *Return  : void
 *Note    :
******************************************************************************/
void  F_VCP_GetMasterId(UINT32* const outSysId);


/******************************************************************************
 *Name    : F_VCP_GetHwVersion
 *Function: �ṩ���û���ȡӲ���汾�ŵĽӿ�
 *Para    : @outHwVersion : ���Ӳ���汾��
 *Return  : void
 *Note    :
******************************************************************************/
void F_VCP_GetHwVersion(UINT32* const outHwVersion);


/******************************************************************************
 *Name    : F_VCP_GetVcpId
 *Function: �ṩ���û���ȡƽ̨Id�Ľӿ�
 *Para    : @outVcpId : ���VcpId
 *Return  : void
 *Note    :
******************************************************************************/
void F_VCP_GetVcpId(UINT32* const outVcpId);


/******************************************************************************
 *Name    : F_VCP_UsrIndicator
 *Function: �ṩ���û�����ָʾ���Ľӿ�
 *Para    : @inIndicatorNo : �����û�ָʾ����
 *          @inAction      : ���붯��
 *Return  : ������������ȷ����e_TRUE�����򷵻�e_FALSE
 *Note    :
******************************************************************************/
E_SysBool F_VCP_UsrIndicator(const UINT32 inIndicatorNo,
                             const UINT32 inAction);


/******************************************************************************
 *Name    : F_VCP_UsrInquiryParamVersion
 *Function: �ṩ���û���ѯ�����ļ��Ľӿ�
 *Para    : @inTarget        : �����ѯĿ��
 *          @outParamVersion : ��������ļ��汾
 *          @outParamCrc     : ��������ļ�Crc
 *          @inParamCrcLen   : �����û��ṩ�Ĵ�������ļ�Crc�ڴ泤��
 *Return  : �����ѯ��ȷ����e_TRUE�����򷵻�e_FALSE
 *Note    :
 ******************************************************************************/
E_SysBool F_VCP_UsrInquiryParamVersion(const UINT32 inTarget,
                                       UINT32*      outParamVersion,
                                       UINT8        outParamCrc[],
                                       const UINT32 inParamCrcLen);


/******************************************************************************
 *Name    : F_VCP_GetDevVerInf
 *Function: �û����������VCP�豸ָ����Ϣ��ȡVCP�豸�İ汾��Ϣ
 *Para    : @inDevType    : �����豸���߼�����
 *          @inAppId      : �����豸��Ӧ�ñ��
 *          @inDevSys     : �����豸����ϵ���ʶ
 *          @outDevVerInf : ����豸�汾��Ϣ
 *Return  : ��ȡ���豸�İ汾��Ϣ������e_TRUE�����򷵻�e_FALSE
 *Note    :
******************************************************************************/
E_SysBool F_VCP_GetDevVerInf(const UINT32     inDevType,
                             const UINT32     inAppId,
                             const UINT32     inDevSys,
                             T_UsrDevVerInf*  outDevVerInf);


/******************************************************************************
 *Name    : F_VCP_GetLocBspVersion
 *Function: �ṩ���û���ȡBsp�汾�ŵĽӿ�
 *Para    : @outBspVersion : ���Bsp�汾��
 *Return  : void
 *Note    :
******************************************************************************/
void F_VCP_GetLocBspVersion(UINT32* const outBspVersion);


/******************************************************************************
 *Name    : F_VCP_GetDevCfgDetail
 *Function: ��ȡ��ϸ�豸����
 *Para    : @inListLen           : �����豸�б���
 *          @outDevCfgDetailList : ����豸�б�
 *          @outDevNum           : ����豸�б����豸��
 *Return  : �����豸������
 *Note    : ����豸�б��Ȳ���, ֻ�����inListLen���豸����
 *          ����ֵ���ܴ���outDevNum
 ******************************************************************************/
UINT32 F_VCP_GetDevCfgDetail(const UINT32   inListLen,
                             T_DevCfgDetail outDevCfgDetailList[],
                             UINT32*        outDevNum);

/******************************************************************************
 *Name      : F_VCP_IsReleaseVersion
 *Function  : ��ȡ����Ƿ�Ϊ������
 *Para      : void
 *Return    : e_TRUE  : ������
 *            e_FALSE : �Ƿ�����(���԰�)
 *Note      :
 ******************************************************************************/
E_SysBool F_VCP_IsReleaseVersion(void);

#endif



