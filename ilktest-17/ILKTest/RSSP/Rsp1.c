/******************************************************************************
*File Name     : Rsp1.c
*Copyright     : Zhuzhou CSR Times Electric Co.,Ltd. All Rights Reserved.
*Create Date    :      2016/01/01
*Description    :������������кš�ʱ�������ʱ��Դ��ʶ���������ġ�˫��У����ֶ�
*��֤��������ͨ�ź�ϵͳ��ȫ�豸����ϵͳ����а�ȫ���ݽ�����
*V.1.0.0  chenxin  2015/12/21  File Create
*V.1.0.1  chenxin  2016/1/2  ���ݱ���淶�޸�
*V.1.0.2  chenxin  2016/3/21 ͬ���ӿ�������CRC����·��У��
*V.1.0.3  chenxin  2016/7/21 �޸�Rsp1_Init(����CRCУ��),����Rsp1_Open���޸�Rsp1_PackageData�ӿ�
*V.1.0.4  chenxin  2016/8/16 ���Rsp1_Init��ʼ�����⣬
���Rsp1_CheckMesgCRC����ֵ����
���Rsp1_PackageData�û����ݴ�С����
���Rsp1_PartitionRecvDataPackageѭ������
ɾ��Rsp1_AnalyseRecvFrames�����ж�����
�޸����ļ���󳤶�c_MAX_SIZEΪ10000U
*
******************************************************************************/

/******************************************************************************
*    Debug switch Section
******************************************************************************/

/******************************************************************************
*    Include File Section
******************************************************************************/
#include "Rsp1.h"
/******************************************************************************
*    Local Macro Define Section
******************************************************************************/

#define rNULL ((void*)0)

/*�ļ���󳤶�*/
#define c_MAX_SIZE (10000U)

/*��ȫ����Ӧ��*/
#define c_APPLCAT_MAINCTL  (0x01U)

/*��·����*/
#define c_APPLCAT_STANDBY  (0x00U)

/*ϵͳ���鳣�� 1ͨ��*/
#define c_SYSCKW_1 (0xAE390B5AU)

/*ϵͳ���鳣�� 2ͨ��*/
#define c_SYSCKW_2 (0xC103589CU)

/*crc����*/
#define c_MSG_CRC_MASK (0x8810U)

/*A������*/
#define c_MSG_A (0X80U)

/*B������*/
#define c_MSG_B (0X81U)

/*SSE ����*/
#define c_MSG_SSE (0X90U)

/*SSR ����*/
#define c_MSG_SSR (0X91U)

/* SSE����������*/
#define c_TIMEOUT_SSE_SEND (8U)

/*BTMģʽ�� SSE����������*/
#define c_TIMEOUT_BTM_SSE_SEND (0x40U)   //��ӦЭ�����Maxlifetime
/* �����·��*/
#define c_MAX_LINK_NUMBER (253U)

/* ���������*/
#define c_MAX_DEVTYPE_NUMBER (8U)

/* ���ͨ����*/
#define c_CHANEL_NUMBER (2U)

/*ϵͳʶ���*/
#define c_SYSID (0x72U)

/* LEU����ʶ���*/
#define c_LEUID (0x27U)

/* ��������*/
#define c_MULT_NUM (256UL)

/* �����ļ���ͷ����*/
#define c_CFG_FILE_HEADER (64U)

/* ���������ݳ��������� */
#define c_ERR_PACKAGE_LENGTH_OVER (0x01U)

/* ʱ�������ʱ�䳬��ʱ�����̷�Χ */
#define c_ERR_SSR_OVER  (0x02U)

/* ʱ����Ƕ�Ӧ���� */
#define c_ERR_NOT_COMPARED (0x03U)

/* ʱ��������� */
#define c_ERR_NOT_TIMERSTAMP (0x04U)

/* ��ʼ�����ô��� */
#define c_ERR_CONFIG (0x05U)

/* ϵ��ͬ������ */
#define c_ERR_SYN_ERR (0x06U)

/* �޶�Ӧ�����豸 */
#define c_ERR_NO_DEV (0x07U)

/* CRC���� */
#define c_ERR_CRC (0x08U)

/*���Կ��Ƶ�LEU�� v.1.0.5*/
#define c_LEU_NUMBER (0x06U)

/* BTMģʽ����*/
#define c_CLOCK_BTM (93U)

/*SBT ���洫�俪ʼ v.1.0.5*/
#define c_MSG_SBT (0xA0U)

/*EBT ���洫����� v.1.0.5*/
#define c_MSG_EBT (0xA1U)

/*ABT ȷ�ϻ��洫�� v.1.0.5*/
#define c_MSG_ABT (0xA2U)

/*BTC ���洫����� v.1.0.5*/
#define c_MSG_BTC (0xA3U)

/*BTD ���洫������ v.1.0.5*/
#define c_MSG_BTD (0xA4U)

/*SBT������ v.1.0.5*/
#define c_SBT_SIZE (138U)

/*EBT������ v.1.0.5*/
#define c_EBT_SIZE (22U)

/*ABT������ v.1.0.5*/
#define c_ABT_SIZE (22U)

/*BTC������ v.1.0.5*/
#define c_BTC_SIZE (30U)

/*BTD������ v.1.0.5*/
#define c_BTD_SIZE (52U)

/*BTM����ͷ v.1.0.5*/
#define c_APPLCAT_BTM (0x10U)

/*BTMģʽԴ��ַ v.1.0.5*/
#define c_BTM_SA (0x00FFU)

/*BTMģʽĿ���ַ v.1.0.5*/
#define c_BTM_DA (0x0006U)

/*SBT֡��ȫ���ݳ��� v.1.0.5*/
#define c_SBT_SDS_LENGTH (0x007C)

/*EBT֡��ȫ���ݳ��� v.1.0.5*/
#define c_EBT_SDS_LENGTH (0x0008)

/*ABT֡��ȫ���ݳ��� v.1.0.5*/
#define c_ABT_SDS_LENGTH (0x0008)

/*BTC֡��ȫ���ݳ��� v.1.0.5*/
#define c_BTC_SDS_LENGTH (0x0010)

/*SBT֡��ȫ���ݳ��� v.1.0.5*/
#define c_BTD_SDS_LENGTH (0x0026)
/******************************************************************************
*    Local Struct Define Section
******************************************************************************/
/* ʱ�������� */
typedef enum TAG_TimeStampCheckResult
{
	e_TsCheckErr = 0U,/* ʱ��������� */
	e_TsCheckOk = 1U,/* ʱ��������ȷ */
	e_TsCheckRepeat = 2U/* �ظ�����֡ */
}E_TimeStampCheckResult;

/* ͨ�������б�Ԫ */
typedef struct TAG_CommTypeIndexTable
{
	UTiny commType;/* ͨ���������� */
	UTiny startIndex;/* ��ʼ���к� */
	UTiny allNumber;/* �������豸������ */
}S_CommTypeIndexTable;

/* ͨ�����̱�Ԫ */
typedef struct TAG_ComToleration
{
	UTiny commType;/*ͨ������*/
	UTiny missToleration;/*��������ֵ*/
	UTiny timeoutToleration;/*��ʱ����ֵ*/
}S_ComToleration;

/* ͨ�Žڵ㶨�� */
typedef struct TAG_NodeParam
{
	UShort sourceAddress;/*Դ��ַ*/
	ULong sid[c_CHANEL_NUMBER];/*˫ͨ��SID*/
	ULong sinit[c_CHANEL_NUMBER];/*˫ͨ��SINIT*/
	ULong dataver[c_CHANEL_NUMBER];/*˫ͨ�����ݰ汾����*/
	UTiny numDataVer;/*����ͨ����*/
}S_NodeParam;

/* ͨ������֡ͷ���� */
typedef struct TAG_MsgTypeInfo
{
	UTiny applCat;/*ϵ������*/
	UTiny msgType;/*����֡����*/
	UShort srcAddress;/*Դ��ַ*/
	UShort dstAddress;/*Ŀ�ĵ�ַ*/
	UShort dataSize;/*���ݳ���*/
}S_MsgTypeInfo;

/* SSE��Ϣ */
typedef struct TAG_SseInfo
{
	ULong tc;       /*ʱ���*/
	ULong seq[c_CHANEL_NUMBER]; /* ���к�*/
}S_SseInfo;

/* SSR��Ϣ */
typedef struct TAG_SsrInfo
{
	ULong seqInit[c_CHANEL_NUMBER]; /*��ʼ���к�*/
	ULong tce;/*����ʱ���*/
	ULong tcr;/*Ӧ��ʱ���*/
	UTiny dataVer;/*���ݰ汾����*/
}S_SsrInfo;

/* ʱ�������ṹ�� */
typedef struct TAG_TimeStampMgr
{
	ULong tc; /* ���ؼ�ʱ*/
	ULong tNow[c_CHANEL_NUMBER]; /* ������ʱ��ֵ*/
	ULong tLast[c_CHANEL_NUMBER]; /* ��һ����Ч����ʱ��ֵ*/
}S_TimeStampMgr;

/* ֡���ͷ��Ϳ��� */
typedef struct TAG_SendControl
{
	E_Bool sendSse; /* SSE�����Ϳ���*/
	E_Bool sendSsr; /* SSR�����Ϳ���*/
	E_Bool sendTsd; /* TSD�����Ϳ���*/
}S_SendControl;

/* ͨ�����ƽṹ�� */
typedef struct TAG_ChnControl
{
	UShort rsdSendTimer;/* ���ݰ���������ֵ*/
	S_TimeStampMgr timeStampMgr;/*ʱ�����¼*/
	S_SendControl sendCtrl;/*���ݰ����Ϳ���*/
	UTiny sseCnt;/*SSE���ݰ����Ϳ���*/
	UShort sseSendTimer;/*SSE��������ֵ*/
	S_SseInfo sseInfoTx;/*SSE����״̬*/
	S_SseInfo sseInfoRx;/*SSE����״̬*/
	S_SsrInfo ssrInfoTx;/*SSR����״̬*/
	S_SsrInfo ssrInfoRx;/*SSR����״̬*/
	ULong tcLast;/*��һ�ν��յ���Ч���ݰ�������ֵ*/
	ULong onSendSSE[c_CHANEL_NUMBER];/*SSEʱ���ֵ*/
	ULong tcrLast;/*��һ�ν��յ���Ч���ݰ�������ֵ*/
	ULong tcrNow;/*���ν��յ���Ч���ݰ�������ֵ*/
	ULong nowSidXorTn[c_CHANEL_NUMBER];/*����˫ͨ��SID��T(n)������*/
	ULong lastSidXorTn[c_CHANEL_NUMBER];/*��һ�δ�˫ͨ��SID��T(n)������*/
	ULong precFirstSinit[c_CHANEL_NUMBER];/*��ʼSINITֵ*/
	UTiny ssrBuf[c_SSR_SIZE];/*SSR��������*/
	UTiny sseBuff[c_SSE_SIZE];/*SSE��������*/
	UTiny recvInfoBuf[c_BUFF_SIZE];/*���ݽ�����������*/
	UTiny sendInfoBuf[c_BUFF_SIZE];/*���ݷ�����������*/
	UTiny pRecvBuf[c_BUFF_SIZE];/*Ӧ�����ݽ��ջ�����*/
	UTiny pSendBuf[c_BUFF_SIZE];/*Ӧ�����ݷ��ͻ�����*/
	UShort byteInRecvBuf;/*���ջ�������ʣ��δ�����ֽ���*/
	UShort byteInSendBuf;/*���ͻ�������ʣ��δ�����ֽ���*/
	UShort btRecvFrameSize;/*�������ֽ���*/
	UShort btSendInfoSize;/*�������ֽ���*/
	UShort linkoffTimeCntr;/*ͨ����·�Ͽ���־λ*/
	E_Bool bTimeStampCorrect;/* ʱ�����ȷ��ʾ */
}S_ChnControl;

/* ��·���ƽṹ�� */
typedef struct TAG_ProtocolControl
{
	E_Bool isOpen;/*ͨ����·�Ƿ��*/
	UShort devId; /* �豸��*/
	UTiny missToleration; /* ���̶�����*/
	UTiny timeoutToleration; /* ���̳�ʱ������*/
	S_NodeParam nodeParamPeer;/*�Է�ͨ�����ò���*/
	S_NodeParam nodeParamSelf;/*��ͨ�����ò���*/
	S_MsgTypeInfo msgInfoA;/*����֡Ԥ�����Aͨ��*/
	S_MsgTypeInfo msgInfoB;/*����֡Ԥ�����Bͨ��*/
	S_MsgTypeInfo msgInfoSse;/*SSEԤ�����*/
	S_MsgTypeInfo msgInfoSsr;/*SSRԤ�����*/
	S_MsgTypeInfo msgInfo_BTM_SSE;
	S_MsgTypeInfo MsgInfo_BTM_SSR;
	S_MsgTypeInfo msgInfo_SBT;
	S_MsgTypeInfo msgInfo_EBT;
	S_MsgTypeInfo msgInfo_ABT;
	S_MsgTypeInfo msgInfo_BTC;
	S_MsgTypeInfo msgInfo_BTD;
	ULong pdreg[c_CHANEL_NUMBER]; /* ��ͨ��ʱ����Ĵ���*/
	ULong pTimeStampChk[c_CHANEL_NUMBER][128];/*ʱ�������ô�ֵ*/
	ULong pPrecSinit[c_CHANEL_NUMBER][128];/*SINITԤ��ֵ*/
	S_ChnControl safeCtrl[c_CHANEL_NUMBER];/*˫ͨ��*/


}S_ProtocolControl;

/* �������ݽṹ�� */
typedef struct TAG_Config
{
	ULong sinit;/*SINIT����ֵ*/
	ULong sid;/*SID����ֵ*/
	UShort srcAddress;/*Դ��ַ*/
	UShort dstAddress;/*Ŀ�ĵ�ַ*/
}S_Config;

/* �������ݹ���ṹ�� */
typedef struct TAG_CfgFileHeader
{
	ULong fileMarker;/*�ļ��汾��*/
	ULong checkSum;/*У��ֵ*/
	ULong fileLength;/*�ļ�����*/
	ULong softMarker;/*����汾��*/
	ULong softTime;/*�������ʱ��*/
	ULong compilerTime;/*����ʱ��*/
	UShort devTypeNumber;/*�豸��������*/
	ULong devTypeStartAddress;/*�豸������ʼ��ַ*/
	ULong devNumber;/*�豸����*/
}S_CfgFileHeader;/*�����ļ�����*/

/*Э����־*/
typedef struct TAG_Errcode
{
	UShort systemId;/*ϵͳʶ���*/
	UTiny errorId;/*������*/
}S_Errcode;

/* BTMģʽ״̬��ö�� */
typedef enum TAG_BTM_STATUS
{
	e_init = 0,    //����BTMģʽ
	e_start = 1,   //����SBT
	e_initover = 2,
	e_contrl = 3,  //��start״̬���յ�ABT
	e_contrlover = 4,
	e_transfer = 5, //
	e_transferover = 6, //
	e_end = 7
}BTM_STATUS;


/*BTMģʽ����ṹ��*/
typedef struct TAG_LeuProtocolControl
{
	S_ProtocolControl protocolCtrl;/*Э�����*/
	BTM_STATUS btm_status;/*״̬��*/
}S_BtmProtocolControl;


/******************************************************************************
*    Local Prototype Declare Section
******************************************************************************/

/** ************************************************************
Local function
************************************************************* */
static ULong Rsp1_Add(const UTiny inChanel,
	const ULong inVal,
	S_ProtocolControl *inProtocolCtrl);
static void Rsp1_AddTimeStamp(S_ProtocolControl *inProtocolCtrl,
	S_TimeStampMgr* inTsMgr);
static void Rsp1_AnalyseRecvFrames(UTiny inApplCat,
	UTiny inMsgType,
	S_ProtocolControl *inProtocolControl,
	S_ChnControl *inChnCtrl);
static void Rsp1_AnalyzeRSD(S_ProtocolControl *inProtocolCtrl,
	S_ChnControl *inChnCtrl);
static void Rsp1_AnalyzeSSE(S_ChnControl *inChnCtrl);
static void Rsp1_AnalyzeSSR(S_ProtocolControl *inProtocolCtrl,
	S_ChnControl *inChnCtrl);
static ULong Rsp1_Buffer2ULong(const UTiny * inBufferPtr);
static UShort Rsp1_Buffer2UShort(const UTiny * inBufferPtr);
static void Rsp1_CalculateFirstSinit(UTiny inChanel,
	S_ChnControl *inChnCtrl,
	S_ProtocolControl *inProtocolCtrl);
static void Rsp1_CalculateSEQINI(const UTiny inApplCat,
	const S_ProtocolControl *inProtocolCtrl,
	S_ChnControl *inChnCtrl);
static E_Bool Rsp1_CheckMesgCRC(const UTiny* inDataBuf,
	const UShort inDataSize_U16);
static E_TimeStampCheckResult Rsp1_CheckTimeStamp(UTiny* inDataBuf,
	UShort inDatafieldSize_U16,
	S_ProtocolControl *inProtocolCtrl,
	S_ChnControl *inChnCtrl);
static UShort Rsp1_ComputeMesgCRC(const UTiny* inAddr,
	UShort inLen_U16);
static UShort Rsp1_DiffTime(const UShort inTimer_U16);
static UTiny Rsp1_FindLinkIndex(const UShort inDevId_U16);
static void Rsp1_FormSSEMessage(const S_MsgTypeInfo* inMesgInfo,
	S_ChnControl *inChnCtrl);
static void Rsp1_FormSafetyDataFrame(const S_MsgTypeInfo* inMesgInfo,
	const S_NodeParam* inNodeParam,
	UTiny* inDataField,
	UShort inDataFieldSize,
	S_ChnControl *inChnCtrl);
static void Rsp1_FormSSRMessage(const S_MsgTypeInfo* inMesgInfo,
	const S_ProtocolControl *inProtocolCtrl,
	S_ChnControl *inChnCtrl);
static void Rsp1_FreeChnControl(S_ProtocolControl *inProtocolCtrl);
static ULong Rsp1_GenerateSINIT(const UTiny inChanel,
	const S_ChnControl *inChnCtrl,
	S_ProtocolControl *inProtocolCtrl);
static E_Bool Rsp1_GetToleration(const UShort inDesDevId_U16,
	UTiny *outMissToleration,
	UTiny *outTimeoutToleration);
static void Rsp1_Load(const UTiny inChanel,
	const ULong inVal,
	S_ProtocolControl *inProtocolCtrl);
static E_Bool Rsp1_PartitionRecvDataPackage(S_ProtocolControl *inProtocolCtrl,
	S_ChnControl *inChnCtrl);
static void Rsp1_Post(const UTiny inChanel,
	const ULong inVal,
	S_ProtocolControl *inProtocolCtrl);
static void Rsp1_PreComputeOfTimeStampChk(S_ProtocolControl *inProtocolCtrl);
static ULong Rsp1_Read(const UTiny inChanel,
	const S_ProtocolControl *inProtocolCtrl);
static E_Bool Rsp1_RecvData(S_ProtocolControl * inProtocolCtrl,
	S_ChnControl * inChnCtrl,
	UTiny * inDataPtr,
	UShort inLen_U16);
static void Rsp1_ULong2Buffer(UTiny * inPbuf,
	ULong outDword);
static void Rsp1_UShort2Buffer(UTiny * inPbuf,
	UShort outWord_U16);
static E_Bool Rsp1_UpdateTelegram(const S_ProtocolControl *inProtocolCtrl,
	const S_ChnControl *inChnCtrl,
	const UTiny *inBuf,
	const UShort inLen,
	const UShort inCrc_U16);
static void Rsp1_ResetProtocolControl(S_ProtocolControl * inProtocolCtrl);
static void Rsp1_SetErrorCode(UShort inFunId,
	UTiny inErrorId);
static ULong Rsp1_SafetyCRCCompute(UTiny *inPtr,
	const UShort inSize_U16,
	const UTiny inChanel);
static void Rsp1_SendRSD(S_ProtocolControl * inProtocolCtrl,
	S_ChnControl * inChnCtrl,
	UTiny * inBuf,
	UShort inLen_U16,
	UShort inCrc_U16);
static void Rsp1_SendSSE(const S_ProtocolControl *inProtocolCtrl,
	S_ChnControl *inChnCtrl);
static void Rsp1_SendSSR(S_ProtocolControl *inProtocolCtrl,
	S_ChnControl *inChnCtrl);
static void Rsp1_ShiftLeft(UTiny inChanel,
	UTiny inShift,
	S_ProtocolControl * inProtocolCtrl);
static void Rsp1_ShiftRight(UTiny inChanel,
	UTiny inShift,
	S_ProtocolControl * inProtocolCtrl);

static void Rsp1_Calculate_PREC_FIRSTSINIT(UTiny chanel, S_ChnControl *chnCtrl,
	S_ProtocolControl *protocolCtrl);
/*LEU*/
static E_Bool LEU_RecvData(S_BtmProtocolControl *inProtocolCtrl,
	S_ChnControl *inChnCtrl,
	UTiny *inDataPtr,
	UShort inLen_U16);
static E_Bool LEU_PartitionRecvDataPackage(S_BtmProtocolControl *inProtocolCtrl,
	S_ChnControl *inChnCtrl);
static void LEU_SendSSR(S_BtmProtocolControl *inProtocolCtrl,
	S_ChnControl *inChnCtrl);
static void LEU_SendSSE(const S_BtmProtocolControl *inProtocolCtrl,
	S_ChnControl *inChnCtrl);
static void Rsp1_FormSBTMessage();
static void Rsp1_FormEBTMessage();
static void Rsp1_FormABTMessage();
static void Rsp1_FormBTCMessage();
static void Rsp1_FormBTDMessage();
static void LEU_SendBTM(const UShort inLeuIndex);
static void LEU_FormSSEMessage(S_ChnControl *inChnCtrl);
static void LEU_FormSSRMessage(S_ChnControl *inChnCtrl);
static void Rsp1_LEU_FormSafetyDataFrame(const S_MsgTypeInfo* inMesgInfo,
	const S_NodeParam* inNodeParam,
	UTiny* inDataField,
	UShort inDataFieldSize,
	S_ChnControl *inChnCtrl);
static void Rsp1_LEU_SendRSD(S_ProtocolControl *inProtocolCtrl,
	S_ChnControl *inChnCtrl,
	UTiny *inBuf,
	UShort inLen_U16,
	UShort inCrc_U16);
static void Rsp1_BtmInit();
/******************************************************************************
*    Static Variable Define Section
******************************************************************************/
/* ʱ������ɶ���ʽ*/
static const ULong c_MASK_LFSR[c_CHANEL_NUMBER] =
{
	0x0FC22F87UL, 0xC3E887E1UL
};

static UShort s_RsspClock; /* ��������*/
static S_ProtocolControl s_PrtclCtrlList[c_MAX_LINK_NUMBER];/* ͨ�Ŷ����б�*/
static UTiny s_UsedLinkNum;/* ����������*/
static S_ComToleration s_ComTolerationList[c_MAX_DEVTYPE_NUMBER];/*ͨ�ų�ʱ�������̱�*/
static S_CommTypeIndexTable s_CommTypeIndexTable[c_MAX_DEVTYPE_NUMBER];/*ͨ����·������*/
static S_Errcode s_Errcode[c_MAX_LINK_NUMBER];/*������־*/
static UTiny s_ErrNumber;/*������־*/

static S_BtmProtocolControl s_BtmPrtclCtrlList[c_LEU_NUMBER];
static S_ProtocolControl s_RSDPrtclCtrlList[c_LEU_NUMBER];
/****************************************************************
*Name:Rsp1_BtmInit
*Function:btm���ֳ�ʼ��
*BTM���ֳ�ʼ����
*para:�ޣ�
*return:�ޡ�
*Create by YinKun 2017/10/01
****************************************************************/
static void Rsp1_BtmInit()
{
	UTiny m = 0;
	S_BtmProtocolControl* pLEUCtrl = NULL;
	/*��ʼ��LEUͨ��*/
	for (m = 0; m < c_LEU_NUMBER; m++)
	{
		pLEUCtrl = &s_BtmPrtclCtrlList[m];
		pLEUCtrl->protocolCtrl.nodeParamSelf.sourceAddress = 0x00ffu;
		pLEUCtrl->protocolCtrl.nodeParamSelf.sid[0] = 0x0BED3F1C;
		pLEUCtrl->protocolCtrl.nodeParamSelf.sid[1] = 0x130C021E;
		pLEUCtrl->protocolCtrl.nodeParamSelf.sinit[0] = 0x64B11C18;
		pLEUCtrl->protocolCtrl.nodeParamSelf.sinit[1] = 0x3EE92C1A;
		pLEUCtrl->protocolCtrl.nodeParamSelf.dataver[0] = 0x6DB31504;
		pLEUCtrl->protocolCtrl.nodeParamSelf.dataver[1] = 0x37540506;
		pLEUCtrl->protocolCtrl.nodeParamSelf.numDataVer = 1;

		pLEUCtrl->protocolCtrl.nodeParamPeer.sourceAddress = 0x0006u;
		pLEUCtrl->protocolCtrl.nodeParamPeer.sid[0] = 0x5B26603C;
		pLEUCtrl->protocolCtrl.nodeParamPeer.sid[1] = 0x64C54B3E;
		pLEUCtrl->protocolCtrl.nodeParamPeer.sinit[0] = 0x26AA6538;
		pLEUCtrl->protocolCtrl.nodeParamPeer.sinit[1] = 0x5C82653A;
		pLEUCtrl->protocolCtrl.nodeParamPeer.dataver[0] = 0x6DB31504;
		pLEUCtrl->protocolCtrl.nodeParamPeer.dataver[1] = 0x37540506;
		pLEUCtrl->protocolCtrl.nodeParamPeer.numDataVer = 1;


		pLEUCtrl->protocolCtrl.isOpen = r_FALSE;
		pLEUCtrl->protocolCtrl.pdreg[0] = 0U;
		pLEUCtrl->protocolCtrl.pdreg[1] = 0U;
		F_GL_MemSet(&pLEUCtrl->protocolCtrl.pPrecSinit[0], 0, (UShort)sizeof(ULong));
		F_GL_MemSet(&pLEUCtrl->protocolCtrl.pPrecSinit[1], 0, (UShort)sizeof(ULong));
		pLEUCtrl->protocolCtrl.missToleration = 6;
		pLEUCtrl->protocolCtrl.timeoutToleration = 13;

		Rsp1_FreeChnControl(&pLEUCtrl->protocolCtrl);/* ���ͨ�����ƻ���*/
		Rsp1_PreComputeOfTimeStampChk(&pLEUCtrl->protocolCtrl);/* ���ʱ���*/
		Rsp1_ResetProtocolControl(&pLEUCtrl->protocolCtrl);/* ��ʼ��Э������*/

		pLEUCtrl->btm_status = e_end;
	}
}
/****************************************************************
*Name:LEU_Init
*Function:LEU�ӿڳ�ʼ��
*LEU�ӿڳ�ʼ����
*para:�ޣ�
*return:�ޡ�
*Create by YinKun 2017/10/01
****************************************************************/
void LEU_Init()
{
	UTiny m = 0;
	S_ProtocolControl* pLEUCtrl = NULL;
	Rsp1_BtmInit();
	/*��ʼ��LEUͨ��*/
	for (m = 0; m < c_LEU_NUMBER; m++)
	{
		pLEUCtrl = &s_RSDPrtclCtrlList[m];

		pLEUCtrl->nodeParamSelf.sourceAddress = 0x0003u;
		pLEUCtrl->nodeParamSelf.sid[0] = 0x3B9F750C;
		pLEUCtrl->nodeParamSelf.sid[1] = 0x6BEA7F0E;
		pLEUCtrl->nodeParamSelf.sinit[0] = 0x23357E08;
		pLEUCtrl->nodeParamSelf.sinit[1] = 0x61CD7C0A;
		pLEUCtrl->nodeParamSelf.dataver[0] = 0x0EDD2134;
		pLEUCtrl->nodeParamSelf.dataver[1] = 0x1C116136;
		pLEUCtrl->nodeParamSelf.numDataVer = 1;

		pLEUCtrl->nodeParamPeer.sourceAddress = 0x0006u;
		pLEUCtrl->nodeParamPeer.sid[0] = 0x3B9F750C;
		pLEUCtrl->nodeParamPeer.sid[1] = 0x6BEA7F0E;
		pLEUCtrl->nodeParamPeer.sinit[0] = 0x23357E08;
		pLEUCtrl->nodeParamPeer.sinit[1] = 0x61CD7C0A;
		pLEUCtrl->nodeParamPeer.dataver[0] = 0x0EDD2134;
		pLEUCtrl->nodeParamPeer.dataver[1] = 0x1C116136;
		pLEUCtrl->nodeParamPeer.numDataVer = 1;

		pLEUCtrl->isOpen = r_FALSE;
		pLEUCtrl->pdreg[0] = 0U;
		pLEUCtrl->pdreg[1] = 0U;
		F_GL_MemSet(&pLEUCtrl->pPrecSinit[0], 0, (UShort)sizeof(ULong));
		F_GL_MemSet(&pLEUCtrl->pPrecSinit[1], 0, (UShort)sizeof(ULong));
		pLEUCtrl->missToleration = 6;
		pLEUCtrl->timeoutToleration = 13;



		pLEUCtrl->msgInfoA.applCat = c_APPLCAT_MAINCTL;
		pLEUCtrl->msgInfoA.msgType = c_MSG_A;
		pLEUCtrl->msgInfoA.srcAddress = pLEUCtrl->nodeParamSelf.sourceAddress;
		pLEUCtrl->msgInfoA.dstAddress = pLEUCtrl->nodeParamPeer.sourceAddress;

		pLEUCtrl->msgInfoSsr.applCat = c_APPLCAT_MAINCTL;
		pLEUCtrl->msgInfoSsr.msgType = c_MSG_SSR;
		pLEUCtrl->msgInfoSsr.srcAddress = pLEUCtrl->nodeParamSelf.sourceAddress;
		pLEUCtrl->msgInfoSsr.dstAddress = pLEUCtrl->nodeParamPeer.sourceAddress;
		//RSDģʽ�£�CI������SSE,�ʲ��ó�ʼ��
		Rsp1_FreeChnControl(pLEUCtrl);/* ���ͨ�����ƻ���*/
		Rsp1_PreComputeOfTimeStampChk(pLEUCtrl);/* ���ʱ���*/
		Rsp1_ResetProtocolControl(pLEUCtrl);/* ��ʼ��Э������*/

	}
}

/****************************************************************
*Name:Rsp1_Init
*Function:��ȡ�ļ���Э���ʼ��
*��CRCУ�飬У��ͨ�������ζ�ȡ�����ļ��и����ֽڲ�������Ӧ�������ݽṹ�塣
*�������ݷ�Ϊ�������֣�ͨ���豸��������ͨ���豸�������˸�ͨ��ϵͳ������ͨ
*�Ŷ����SID��SINIT��DATAVER�����������ڿ��ٶ�λĳ���豸��ͨ���豸���е�λ�ã�
*��������ٶȡ��������д��е�startIndexΪĳһ���豸����ʼ���кţ�ĳ�豸DevId��
*ͨ���豸���е�λ����ΪstartIndex + (DevId - (DevId & 0xFF00))timeoutToleration
*��missToleration����m*m����������mΪ���������豸��������������ʾ�˲�ͬ�豸�䳬
*ʱ���̵���ֵ�Ͷ������̵���ֵ��
*para:selfDevId_U16�������豸�ţ�
inCfgLen_U16���������ݳ��ȣ�
inCfgBuf���������ݻ��塣
*return:1����ȡ�ɹ���0��ȡʧ�ܡ�
*Create by ChenX 2016/01/01
****************************************************************/
E_Bool Rsp1_Init(const UShort selfDevId_U16,
	const UShort inCfgLen_U16,
	UTiny *const inCfgBuf)
{
	UTiny i = 0U;
	UTiny m = 0U;
	UTiny k = 0U;
	UShort index = 0U;
	S_NodeParam selfParam;
	S_NodeParam peerParam;
	S_MsgTypeInfo aSysMsgType;
	S_MsgTypeInfo sseMsgType;
	S_MsgTypeInfo ssrMsgType;
	UTiny missToleration_U8 = 0U;
	UTiny timeoutToleration_U8 = 0U;
	UShort devId_U16 = 0U;
	UTiny tmpIndex_U8 = 0U;
	E_Bool result = r_FALSE;
	UTiny fileBuf[c_MAX_SIZE];
	ULong tmpCrc = 0UL;
	ULong fileCrc = 0UL;
	S_CfgFileHeader fileHeader;
	UShort length = 0U;

	s_ErrNumber = 0U;
	s_UsedLinkNum = 0U;
	F_GL_MemSet(fileBuf, 0, c_MAX_SIZE);
	F_GL_MemCpy(fileBuf, inCfgBuf, inCfgLen_U16);
	F_GL_MemSet(&fileHeader, 0, (UShort)sizeof(S_CfgFileHeader));/*����ļ�ͷ������*/
	F_GL_MemSet(s_ComTolerationList, 0, (UShort)sizeof(S_ComToleration)*c_MAX_DEVTYPE_NUMBER);/*������̹����*/
	F_GL_MemSet(s_CommTypeIndexTable, 0, (UShort)sizeof(S_CommTypeIndexTable)*c_MAX_DEVTYPE_NUMBER);/*����豸��ű�*/
	F_GL_MemSet(s_PrtclCtrlList, 0, (UShort)sizeof(S_ProtocolControl)*c_MAX_DEVTYPE_NUMBER);/*�����·�����*/

	length = fileBuf[11] * c_MULT_NUM* c_MULT_NUM* c_MULT_NUM
		+ fileBuf[10] * c_MULT_NUM* c_MULT_NUM
		+ fileBuf[9] * c_MULT_NUM
		+ fileBuf[8];
	if (length > c_CFG_FILE_HEADER)
	{
		tmpCrc = Rsp1_SafetyCRCCompute(&fileBuf[12], length - 4U, 0U);
		fileCrc = (fileBuf[7] * c_MULT_NUM * c_MULT_NUM * c_MULT_NUM) +
			(fileBuf[6] * c_MULT_NUM * c_MULT_NUM) +
			(fileBuf[5] * c_MULT_NUM) +
			fileBuf[4];
	    if(inCfgLen_U16 > 0U)// (tmpCrc == fileCrc)
		{
			s_RsspClock = 0U;
			index = 0U;/* ��ȡ�ļ�ͷ*/
			fileHeader.fileMarker =
				(fileBuf[index + 3U] * c_MULT_NUM * c_MULT_NUM * c_MULT_NUM) +
				(fileBuf[index + 2U] * c_MULT_NUM * c_MULT_NUM) +
				(fileBuf[index + 1U] * c_MULT_NUM) +
				fileBuf[index + 0U];
			index += 4U;/* ��ȡУ��ֵ*/
			fileHeader.checkSum =
				(fileBuf[index + 3U] * c_MULT_NUM * c_MULT_NUM * c_MULT_NUM) +
				(fileBuf[index + 2U] * c_MULT_NUM * c_MULT_NUM) +
				(fileBuf[index + 1U] * c_MULT_NUM) +
				fileBuf[index + 0U];
			index += 4U;/* ��ȡ�ļ�����*/
			fileHeader.fileLength =
				(fileBuf[index + 3U] * c_MULT_NUM * c_MULT_NUM * c_MULT_NUM) +
				(fileBuf[index + 2U] * c_MULT_NUM * c_MULT_NUM) +
				(fileBuf[index + 1U] * c_MULT_NUM) +
				fileBuf[index + 0U];
			index += 4U;/* ��ȡ����汾*/
			fileHeader.softMarker =
				(fileBuf[index + 3U] * c_MULT_NUM * c_MULT_NUM * c_MULT_NUM) +
				(fileBuf[index + 2U] * c_MULT_NUM * c_MULT_NUM) +
				(fileBuf[index + 1U] * c_MULT_NUM) +
				fileBuf[index + 0U];
			index += 4U;
			fileHeader.softTime =/* �����������ʱ��*/
				(fileBuf[index + 3U] * c_MULT_NUM * c_MULT_NUM * c_MULT_NUM) +
				(fileBuf[index + 2U] * c_MULT_NUM * c_MULT_NUM) +
				(fileBuf[index + 1U] * c_MULT_NUM) +
				fileBuf[index + 0U];
			index += 4U;
			fileHeader.compilerTime =/* �����������ʱ��*/
				(fileBuf[index + 3U] * c_MULT_NUM * c_MULT_NUM * c_MULT_NUM) +
				(fileBuf[index + 2U] * c_MULT_NUM * c_MULT_NUM) +
				(fileBuf[index + 1U] * c_MULT_NUM) +
				fileBuf[index + 0U];
			index += 12U;
			fileHeader.devTypeNumber =/* �豸��������*/
				(fileBuf[index + 1U] * (UShort)c_MULT_NUM) +
				fileBuf[index + 0U];
			index += 2U;
			fileHeader.devTypeStartAddress =/* �豸������ʼ��ַ*/
				(fileBuf[index + 3U] * c_MULT_NUM * c_MULT_NUM * c_MULT_NUM) +
				(fileBuf[index + 2U] * c_MULT_NUM * c_MULT_NUM) +
				(fileBuf[index + 1U] * c_MULT_NUM) +
				fileBuf[index + 0U];
			index += 4U;
			index = 24U;/* ��ȡ�豸����*/
			fileHeader.devNumber =
				(fileBuf[index + 3U] * c_MULT_NUM * c_MULT_NUM * c_MULT_NUM) +
				(fileBuf[index + 2U] * c_MULT_NUM * c_MULT_NUM) +
				(fileBuf[index + 1U] * c_MULT_NUM) +
				fileBuf[index + 0U];

			s_UsedLinkNum = (UTiny)(fileHeader.devNumber);
			index = 64U;
			for (m = 0U; m < fileHeader.devTypeNumber; m++)
			{
				/* ���̱����*/
				s_ComTolerationList[m].commType = fileBuf[index + i];/* �豸����*/
				s_CommTypeIndexTable[m].commType = fileBuf[index + i];/* �豸����*/
				s_CommTypeIndexTable[m].startIndex = fileBuf[index + i + 1U];/* ��ʼλ��*/
				s_CommTypeIndexTable[m].allNumber = fileBuf[index + i + 2U];/* ����*/
				if (((selfDevId_U16 & 0xFF00U) >> 8U) == s_ComTolerationList[m].commType)
				{
					for (k = 0U; k < fileHeader.devTypeNumber; k++)
					{
						s_ComTolerationList[k].missToleration = fileBuf[index + i + (k * 2U) + 3U];/* ��������*/
						s_ComTolerationList[k].timeoutToleration = fileBuf[index + i + (k * 2U) + 4U];/* ��ʱ����*/
					}
				}
				i = i + 16U;
			}

			index = 64U + (fileHeader.devTypeNumber * 16U);

			for (m = 1U; m <= s_UsedLinkNum; m++)
			{
				/* ͨ�Žڵ��������*/
				devId_U16 = (fileBuf[index + 1U] * 256U) + fileBuf[index];
				index += 2U;
				peerParam.sourceAddress = (fileBuf[index + 1U] * 256U) + fileBuf[index];
				index += 2U;
				peerParam.sid[0U] = (fileBuf[index + 3U] * c_MULT_NUM * c_MULT_NUM * c_MULT_NUM) +
					(fileBuf[index + 2U] * c_MULT_NUM * c_MULT_NUM) +
					(fileBuf[index + 1U] * c_MULT_NUM) +
					fileBuf[index + 0U];/* ���SID */
				index += 4U;
				peerParam.sinit[0U] = (fileBuf[index + 3U] * c_MULT_NUM * c_MULT_NUM * c_MULT_NUM) +
					(fileBuf[index + 2U] * c_MULT_NUM * c_MULT_NUM) +
					(fileBuf[index + 1U] * c_MULT_NUM) +
					fileBuf[index + 0U];/* ���SINIT */
				index += 4U;
				peerParam.dataver[0U] = (fileBuf[index + 3U] * c_MULT_NUM * c_MULT_NUM * c_MULT_NUM) +
					(fileBuf[index + 2U] * c_MULT_NUM * c_MULT_NUM) +
					(fileBuf[index + 1U] * c_MULT_NUM) +
					fileBuf[index + 0U];/* ���DATAVER */
				index += 4U;

				peerParam.sid[1U] = (fileBuf[index + 3U] * c_MULT_NUM * c_MULT_NUM * c_MULT_NUM) +
					(fileBuf[index + 2U] * c_MULT_NUM * c_MULT_NUM) +
					(fileBuf[index + 1U] * c_MULT_NUM) +
					fileBuf[index + 0U];/* ���SID */
				index += 4U;
				peerParam.sinit[1U] = (fileBuf[index + 3U] * c_MULT_NUM * c_MULT_NUM * c_MULT_NUM) +
					(fileBuf[index + 2U] * c_MULT_NUM * c_MULT_NUM) +
					(fileBuf[index + 1U] * c_MULT_NUM) +
					fileBuf[index + 0U];/* ���SINIT */
				index += 4U;
				peerParam.dataver[1U] = (fileBuf[index + 3U] * c_MULT_NUM * c_MULT_NUM * c_MULT_NUM) +
					(fileBuf[index + 2U] * c_MULT_NUM * c_MULT_NUM) +
					(fileBuf[index + 1U] * c_MULT_NUM) +
					fileBuf[index + 0U];/* ���DATAVER */
				index += 4U;
				index += 4U;/*����*/
				peerParam.numDataVer = 1U;/*selfParam.numDataVer;*/

				aSysMsgType.applCat = c_APPLCAT_MAINCTL;
				aSysMsgType.msgType = c_MSG_A;
				aSysMsgType.dstAddress = peerParam.sourceAddress;
				aSysMsgType.dataSize = c_BYTE_SIZE;
				s_PrtclCtrlList[m].msgInfoA = aSysMsgType;/* ���MSG_AЭ������ */

				aSysMsgType.applCat = c_APPLCAT_MAINCTL;
				aSysMsgType.msgType = c_MSG_B;
				aSysMsgType.dstAddress = peerParam.sourceAddress;
				aSysMsgType.dataSize = c_BYTE_SIZE;
				s_PrtclCtrlList[m].msgInfoB = aSysMsgType;/* ���MSG_BЭ������ */

				sseMsgType.applCat = c_APPLCAT_MAINCTL;
				sseMsgType.msgType = c_MSG_SSE;
				sseMsgType.dstAddress = peerParam.sourceAddress;
				sseMsgType.dataSize = c_BYTE_SIZE;
				s_PrtclCtrlList[m].msgInfoSse = sseMsgType;/* ���SSEЭ������ */

				ssrMsgType.applCat = c_APPLCAT_MAINCTL;
				ssrMsgType.msgType = c_MSG_SSR;
				ssrMsgType.dstAddress = peerParam.sourceAddress;
				ssrMsgType.dataSize = c_BYTE_SIZE;
				s_PrtclCtrlList[m].msgInfoSsr = ssrMsgType;/* ���SSRЭ������ */

				s_PrtclCtrlList[m].devId = devId_U16;
				s_PrtclCtrlList[m].nodeParamPeer = peerParam;

				s_PrtclCtrlList[m].isOpen = r_FALSE;
				s_PrtclCtrlList[m].pdreg[0] = 0U;
				s_PrtclCtrlList[m].pdreg[1] = 0U;
				F_GL_MemSet(&s_PrtclCtrlList[m].pPrecSinit[0], 0, (UShort)sizeof(ULong));
				F_GL_MemSet(&s_PrtclCtrlList[m].pPrecSinit[1], 0, (UShort)sizeof(ULong));
			}

			tmpIndex_U8 = Rsp1_FindLinkIndex(selfDevId_U16);
			F_GL_MemCpy(&selfParam, &s_PrtclCtrlList[tmpIndex_U8].nodeParamPeer,
				(UShort)sizeof(S_NodeParam));

			for (m = 1U; m <= s_UsedLinkNum; m++)
			{
				/* ��䱾�ڵ���������*/
				if (r_TRUE == Rsp1_GetToleration(s_PrtclCtrlList[m].devId,
					&missToleration_U8, &timeoutToleration_U8))
				{
					F_GL_MemCpy(&s_PrtclCtrlList[m].nodeParamSelf, &selfParam,
						(UShort)sizeof(S_NodeParam));
					s_PrtclCtrlList[m].missToleration = missToleration_U8;
					s_PrtclCtrlList[m].timeoutToleration = timeoutToleration_U8;
					s_PrtclCtrlList[m].msgInfoA.srcAddress = selfParam.sourceAddress;
					s_PrtclCtrlList[m].msgInfoB.srcAddress = selfParam.sourceAddress;
					s_PrtclCtrlList[m].msgInfoSse.srcAddress = selfParam.sourceAddress;
					s_PrtclCtrlList[m].msgInfoSsr.srcAddress = selfParam.sourceAddress;

					Rsp1_FreeChnControl(&s_PrtclCtrlList[m]);/* ���ͨ�����ƻ���*/
					Rsp1_PreComputeOfTimeStampChk(&s_PrtclCtrlList[m]);
					Rsp1_ResetProtocolControl(&s_PrtclCtrlList[m]);/* ��ʼ��Э������*/
				}
			}
			result = r_TRUE;
		}
		else
		{
			Rsp1_SetErrorCode(c_SYSID, c_ERR_CONFIG); /* ��ʼ�����ô��󣬻㱨 */
			result = r_FALSE;
		}
	}
	else
	{
		Rsp1_SetErrorCode(c_ERR_CRC, c_ERR_CONFIG); /* ��ʼ�����ô��󣬻㱨 */
		result = r_FALSE;
	}
	return result;
}

/** *************************************************************
BTM���ڴ�������
************************************************************* */
void LEU_BtmCycle(const UTiny LEUIndex)
{
	S_BtmProtocolControl* pLEUCtrl = &s_BtmPrtclCtrlList[LEUIndex];
	if (s_RsspClock % c_CLOCK_BTM == 0)
	{
		pLEUCtrl->btm_status = e_init;
	}
}




/****************************************************************
*Name:Rsp1_GetChanelStatus
*Function:��ȡͨ��״̬
*δ���յ����ݰ��������� = ��ǰ����Э��ջʱ��ֵ - ���һ�ν��յ���Ч���ݰ��ı���Э��ջʱ��ֵ��
*���δ���յ����ݰ������������ڻ��ߵ��ڸ���·����ĳ�ʱ����ֵ������Ϊ����·�Ѿ��Ͽ���
*para:inDstDevId_U16�����շ��豸ʶ��ţ�
outCycDiff��������һ����Ч���ݼ��������
*return: 1��������0��HALT
*Create by ChenX 2016/01/01
****************************************************************/
E_Bool Rsp1_GetChanelStatus(const UShort inDstDevId_U16,
	UShort * const outCycDiff)
{
	E_Bool chanelStatus_U8 = r_FALSE;
	UTiny linkIndex_U8 = 0U;
	linkIndex_U8 = Rsp1_FindLinkIndex(inDstDevId_U16);
	/* δ���յ����ݰ��������� = ��ǰ����Э��ջʱ��ֵ - ���һ�ν��յ���Ч���ݰ��ı���Э��ջʱ��ֵ��
	���δ���յ����ݰ������������ڻ��ߵ��ڸ���·����ĳ�ʱ����ֵ������Ϊ����·�Ѿ��Ͽ���*/
	*outCycDiff = Rsp1_DiffTime(s_PrtclCtrlList[linkIndex_U8].safeCtrl[0].linkoffTimeCntr);
	if (*outCycDiff >= s_PrtclCtrlList[linkIndex_U8].timeoutToleration)
	{
		chanelStatus_U8 = r_FALSE;
	}
	else
	{
		chanelStatus_U8 = r_TRUE;
	}
	return chanelStatus_U8;
}

/****************************************************************
*Name: Rsp1_UpdateClock
*Function:����ʱ�ӣ������ۼ�Э��ջʱ��ֵ��
*para:��
*return:��
*Create by ChenX 2016/01/01
****************************************************************/
void Rsp1_UpdateClock(void)
{
	s_RsspClock++;
	s_ErrNumber = 0U;
	memset(&s_Errcode, 0, sizeof(S_Errcode)*c_MAX_LINK_NUMBER);
}

/****************************************************************
*Name: Rsp1_PackageData
*Function:���RSD֡��SSE֡��SSR֡���ݰ���Ӧ�ò��ȡ��
*para:inDstDevId_U16���Է��豸ʶ��ţ�
inMsg��δ��װӦ�����ݣ�
outMsg����װ������ݻ��壻
outSsMsg��Уʱ֡����
*return:
*Create by ChenX 2016/01/01
****************************************************************/
UTiny Rsp1_PackageData(const UShort inDstDevId_U16,
	S_Msg* inMsg,
	S_Msg* outMsg,
	S_SsMsg* outSseMsg,
	S_SsMsg* outSsrMsg)
{
	UTiny status_U8 = 0U;
	UTiny linkIndex_U8 = 0U;
	UTiny tsdBuf[c_BUFF_SIZE];

	(*outMsg).length = 0U;/*init*/
	(*outSseMsg).length = 0U;/*init*/
	(*outSsrMsg).length = 0U;/*init*/

	F_GL_MemSet(tsdBuf, 0, c_BUFF_SIZE);
	linkIndex_U8 = Rsp1_FindLinkIndex(inDstDevId_U16);/*����Ŀ���豸���*/
	if (r_FALSE == s_PrtclCtrlList[linkIndex_U8].isOpen)
	{
		s_PrtclCtrlList[linkIndex_U8].isOpen = r_TRUE;
	}

	if ((*inMsg).length > c_BYTE_SIZE)
	{
		status_U8 = 1U;/*����������*/
		Rsp1_SetErrorCode(inDstDevId_U16, c_ERR_PACKAGE_LENGTH_OVER);
	}
	else
	{
		if ((r_TRUE == s_PrtclCtrlList[linkIndex_U8].safeCtrl[0].sendCtrl.sendSse) ||
			(r_TRUE == s_PrtclCtrlList[linkIndex_U8].safeCtrl[0].sendCtrl.sendSsr))
		{
			/*���ڶ�ʱ֡*/
			(*outSsrMsg).length = 0U;
			(*outSseMsg).length = 0U;
			if (c_SSE_SIZE == s_PrtclCtrlList[linkIndex_U8].safeCtrl[0].byteInSendBuf)
			{
				F_GL_MemCpy((*outSseMsg).buff, s_PrtclCtrlList[linkIndex_U8].safeCtrl[0].pSendBuf, c_SSE_SIZE);
				(*outSseMsg).length = c_SSE_SIZE;
				s_PrtclCtrlList[linkIndex_U8].safeCtrl[0].sendCtrl.sendSse = r_FALSE;
				s_PrtclCtrlList[linkIndex_U8].safeCtrl[0].sendCtrl.sendSsr = r_FALSE;
			}
			else
			{
				if (c_SSR_SIZE == s_PrtclCtrlList[linkIndex_U8].safeCtrl[0].byteInSendBuf)
				{
					F_GL_MemCpy((*outSsrMsg).buff, s_PrtclCtrlList[linkIndex_U8].safeCtrl[0].pSendBuf, c_SSR_SIZE);
					(*outSsrMsg).length = c_SSR_SIZE;
					s_PrtclCtrlList[linkIndex_U8].safeCtrl[0].sendCtrl.sendSse = r_FALSE;
					s_PrtclCtrlList[linkIndex_U8].safeCtrl[0].sendCtrl.sendSsr = r_FALSE;
					s_PrtclCtrlList[linkIndex_U8].safeCtrl[0].sendCtrl.sendTsd = r_TRUE;
				}
			}
			status_U8 = 2U;/*��Уʱ֡*/
		}
		else
		{
			status_U8 = 3U;/*ʵʱ����֡*/
			s_PrtclCtrlList[linkIndex_U8].safeCtrl[0].sendCtrl.sendTsd = r_TRUE;
		}
		Rsp1_SendRSD(&s_PrtclCtrlList[linkIndex_U8],
			&s_PrtclCtrlList[linkIndex_U8].safeCtrl[0],
			(*inMsg).buff,
			(*inMsg).length,
			Rsp1_ComputeMesgCRC((*inMsg).buff, (*inMsg).length));/*RSD֡*/
		(*outMsg).length = s_PrtclCtrlList[linkIndex_U8].safeCtrl[0].byteInSendBuf;
		F_GL_MemCpy((*outMsg).buff, s_PrtclCtrlList[linkIndex_U8].safeCtrl[0].pSendBuf, (*outMsg).length);
		F_GL_MemSet(s_PrtclCtrlList[linkIndex_U8].safeCtrl[0].pSendBuf, 0, c_BUFF_SIZE);
		s_PrtclCtrlList[linkIndex_U8].safeCtrl[0].byteInSendBuf = 0;
	}

	return status_U8;
}


/****************************************************************
*Name: LEU_PackageRsdData
*Function:���RSD֡��SSE֡��SSR֡���ݰ���Ӧ�ò��ȡ��
*para:inDstDevId_U16���Է��豸ʶ��ţ�
inMsg��δ��װӦ�����ݣ�
outMsg����װ������ݻ��壻
outSsMsg��Уʱ֡����
*return:
*Create by ChenX 2016/01/01
****************************************************************/
UTiny LEU_PackageRsdData(const UShort LEUIndex,
	S_Msg* inMsg,
	S_Msg* outMsg,
	S_SsMsg* outSseMsg,
	S_SsMsg* outSsrMsg)
{
	UTiny status_U8 = 0U;
	UTiny tsdBuf[c_BUFF_SIZE];

	(*outMsg).length = 0U;/*init*/
	(*outSseMsg).length = 0U;/*init*/
	(*outSsrMsg).length = 0U;/*init*/

	F_GL_MemSet(tsdBuf, 0, c_BUFF_SIZE);
	if (r_FALSE == s_RSDPrtclCtrlList[LEUIndex].isOpen)
	{
		s_RSDPrtclCtrlList[LEUIndex].isOpen = r_TRUE;
	}

	if ((*inMsg).length > c_BYTE_SIZE)
	{
		status_U8 = 1U;/*����������*/
		Rsp1_SetErrorCode(c_LEUID, c_ERR_PACKAGE_LENGTH_OVER);
	}
	else
	{
		if ((r_TRUE == s_RSDPrtclCtrlList[LEUIndex].safeCtrl[0].sendCtrl.sendSse) ||
			(r_TRUE == s_RSDPrtclCtrlList[LEUIndex].safeCtrl[0].sendCtrl.sendSsr))
		{
			/*���ڶ�ʱ֡*/
			(*outSsrMsg).length = 0U;
			(*outSseMsg).length = 0U;
			if (c_SSE_SIZE == s_RSDPrtclCtrlList[LEUIndex].safeCtrl[0].byteInSendBuf)
			{
				F_GL_MemCpy((*outSseMsg).buff, s_RSDPrtclCtrlList[LEUIndex].safeCtrl[0].pSendBuf, c_SSE_SIZE);
				(*outSseMsg).length = c_SSE_SIZE;
				s_RSDPrtclCtrlList[LEUIndex].safeCtrl[0].sendCtrl.sendSse = r_FALSE;
				s_RSDPrtclCtrlList[LEUIndex].safeCtrl[0].sendCtrl.sendSsr = r_FALSE;
			}
			else
			{
				if (c_SSR_SIZE ==s_RSDPrtclCtrlList[LEUIndex].safeCtrl[0].byteInSendBuf)
				{
					F_GL_MemCpy((*outSsrMsg).buff,s_RSDPrtclCtrlList[LEUIndex].safeCtrl[0].pSendBuf, c_SSR_SIZE);
					(*outSsrMsg).length = c_SSR_SIZE;
					s_RSDPrtclCtrlList[LEUIndex].safeCtrl[0].sendCtrl.sendSse = r_FALSE;
					s_RSDPrtclCtrlList[LEUIndex].safeCtrl[0].sendCtrl.sendSsr = r_FALSE;
					s_RSDPrtclCtrlList[LEUIndex].safeCtrl[0].sendCtrl.sendTsd = r_TRUE;
				}
			}
			status_U8 = 2U;/*��Уʱ֡*/
		}
		else
		{
			s_RSDPrtclCtrlList[LEUIndex].safeCtrl[0].sendCtrl.sendTsd = r_TRUE;
			status_U8 = 3U;/*ʵʱ����֡*/

		}
		Rsp1_LEU_SendRSD(&s_RSDPrtclCtrlList[LEUIndex],
				&s_RSDPrtclCtrlList[LEUIndex].safeCtrl[0],
				(*inMsg).buff,
				(*inMsg).length,
				Rsp1_ComputeMesgCRC((*inMsg).buff, (*inMsg).length));/*RSD֡*/
		(*outMsg).length =s_RSDPrtclCtrlList[LEUIndex].safeCtrl[0].byteInSendBuf;
		F_GL_MemCpy((*outMsg).buff,s_RSDPrtclCtrlList[LEUIndex].safeCtrl[0].pSendBuf, (*outMsg).length);
		F_GL_MemSet(s_RSDPrtclCtrlList[LEUIndex].safeCtrl[0].pSendBuf, 0, c_BUFF_SIZE);
		s_RSDPrtclCtrlList[LEUIndex].safeCtrl[0].byteInSendBuf = 0;
	}

	return status_U8;
}
/****************************************************************
*Name: Rsp1_PackageBtmData
*Function:���RSD֡��SSE֡��SSR֡���ݰ���Ӧ�ò��ȡ��
*para:inDstDevId_U16���Է��豸ʶ��ţ�
inMsg��δ��װӦ�����ݣ�
outMsg����װ������ݻ��壻
outSsMsg��Уʱ֡����
*return:
*Create by ChenX 2016/01/01
****************************************************************/
UTiny LEU_PackageBtmData(const UShort LEUIndex,
	S_Msg* outMsg,
	S_SsMsg* outSseMsg,
	S_SsMsg* outSsrMsg)
{
	UTiny status_U8 = 0U;
	UTiny linkIndex_U8 = 0U;
	UTiny tsdBuf[c_BUFF_SIZE];
	S_BtmProtocolControl*  pLEUCtrl = &s_BtmPrtclCtrlList[LEUIndex];
	UTiny BTMbuf[1024] = { 0 };
	UShort BTMlen = 0;
	UTiny sendtype = 0;
	(*outMsg).length = 0U;/*init*/
	(*outSseMsg).length = 0U;/*init*/
	(*outSsrMsg).length = 0U;/*init*/

	F_GL_MemSet(tsdBuf, 0, c_BUFF_SIZE);
	if (r_FALSE == pLEUCtrl->protocolCtrl.isOpen)
	{
		pLEUCtrl->protocolCtrl.isOpen = r_TRUE;
	}

	if ((r_TRUE == pLEUCtrl->protocolCtrl.safeCtrl[0].sendCtrl.sendSse) ||
		(r_TRUE == pLEUCtrl->protocolCtrl.safeCtrl[0].sendCtrl.sendSsr))
	{
		/*���ڶ�ʱ֡*/
		(*outSsrMsg).length = 0U;
		(*outSseMsg).length = 0U;
		if (c_SSE_SIZE == pLEUCtrl->protocolCtrl.safeCtrl[0].byteInSendBuf)
		{
			F_GL_MemCpy((*outSseMsg).buff, pLEUCtrl->protocolCtrl.safeCtrl[0].pSendBuf, c_SSE_SIZE);
			(*outSseMsg).length = c_SSE_SIZE;
			pLEUCtrl->protocolCtrl.safeCtrl[0].sendCtrl.sendSse = r_FALSE;
			pLEUCtrl->protocolCtrl.safeCtrl[0].sendCtrl.sendSsr = r_FALSE;
			F_GL_MemSet(pLEUCtrl->protocolCtrl.safeCtrl[0].pSendBuf, 0, c_BUFF_SIZE);
			pLEUCtrl->protocolCtrl.safeCtrl[0].byteInSendBuf = 0; //yk���
		}
		else if (c_SSR_SIZE == pLEUCtrl->protocolCtrl.safeCtrl[0].byteInSendBuf)
		{
			F_GL_MemCpy((*outSsrMsg).buff, pLEUCtrl->protocolCtrl.safeCtrl[0].pSendBuf, c_SSR_SIZE);
			(*outSsrMsg).length = c_SSR_SIZE;
			pLEUCtrl->protocolCtrl.safeCtrl[0].sendCtrl.sendSse = r_FALSE;
			pLEUCtrl->protocolCtrl.safeCtrl[0].sendCtrl.sendSsr = r_FALSE;
			F_GL_MemSet(pLEUCtrl->protocolCtrl.safeCtrl[0].pSendBuf, 0, c_BUFF_SIZE);
			pLEUCtrl->protocolCtrl.safeCtrl[0].byteInSendBuf = 0; //yk���
		}
		else
		{

		}
		status_U8 = 2U;/*��Уʱ֡*/
	}
	LEU_SendBTM(LEUIndex);/*BTM֡*/

	F_GL_MemCpy(&(*outMsg).buff[(*outMsg).length],
		pLEUCtrl->protocolCtrl.safeCtrl[0].pSendBuf,
		pLEUCtrl->protocolCtrl.safeCtrl[0].byteInSendBuf);
	(*outMsg).length += pLEUCtrl->protocolCtrl.safeCtrl[0].byteInSendBuf;
	F_GL_MemSet(pLEUCtrl->protocolCtrl.safeCtrl[0].pSendBuf, 0, c_BUFF_SIZE);
	pLEUCtrl->protocolCtrl.safeCtrl[0].byteInSendBuf = 0; //yk���

	return status_U8;
}


/****************************************************************
*Name:   Rsp1_CheckPackage
*Function:����յ������ݰ����������
*para:inDstDevId_U16���豸ʶ��ţ�
inLen_U16�����ݳ��ȣ�
inBuf�����ݻ��壻
outLen����װ��Ӧ�����ݳ��ȣ�
outBuf����װ��Ӧ�����ݻ��壻
*return:0���رյ���·��1����Ч���ݣ�2������֡��3��Уʱ֡
*Create by ChenX 2016/01/01
****************************************************************/
UTiny Rsp1_CheckPackage(const UShort inDstDevId_U16,
	const UShort inLen_U16,
	UTiny *inBuf,
	UShort *outLen,
	UTiny *outBuf)
{
	UTiny result_U8 = 0U;
	UTiny linkIndex_U8 = 0U;
	E_Bool tmpResult = r_FALSE;

	linkIndex_U8 = Rsp1_FindLinkIndex(inDstDevId_U16);
	if (r_FALSE == s_PrtclCtrlList[linkIndex_U8].isOpen)
	{
		result_U8 = 0U;
	}
	else
	{
		if (inLen_U16 > (c_BYTE_SIZE + 22U))
		{
			result_U8 = 1U;
		}
		else
		{
			tmpResult = Rsp1_RecvData(&s_PrtclCtrlList[linkIndex_U8],
				&s_PrtclCtrlList[linkIndex_U8].safeCtrl[0], inBuf, inLen_U16); /* ������ջ��� */
			if ((tmpResult == r_TRUE)
				&& (s_PrtclCtrlList[linkIndex_U8].safeCtrl[0].btRecvFrameSize > 0U))
			{
				*outLen = s_PrtclCtrlList[linkIndex_U8].safeCtrl[0].btRecvFrameSize; /* ������ЧӦ�����ݳ��� */
				F_GL_MemCpy(outBuf, &s_PrtclCtrlList[linkIndex_U8].safeCtrl[0].recvInfoBuf[20], *outLen);
				result_U8 = 3U; /*����֡*/
			}
			else
			{
				result_U8 = 2U; /*��ʱ֡*/
			}
		}
	}
	return result_U8;
}



/****************************************************************
*Name:   LEU_CheckRsdPackage
*Function:����յ������ݰ����������
*para:inDstDevId_U16���豸ʶ��ţ�
inLen_U16�����ݳ��ȣ�
inBuf�����ݻ��壻
outLen����װ��Ӧ�����ݳ��ȣ�
outBuf����װ��Ӧ�����ݻ��壻
*return:0���رյ���·��1����Ч���ݣ�2��Уʱ����3��Уʱ��ȷ
*Create by ChenX 2016/01/01
****************************************************************/
UTiny LEU_CheckRsdPackage(const UShort LinkIndex,
	const UShort inLen_U16,UTiny *inBuf)
{
	UTiny result_U8 = 0U;
	E_Bool tmpResult = r_FALSE;

	if (r_FALSE == s_RSDPrtclCtrlList[LinkIndex].isOpen)
	{
		result_U8 = 0U;
	}
	else
	{
		if (inLen_U16 > (c_BYTE_SIZE + 22U))
		{
			result_U8 = 1U;
		}
		else
		{
			tmpResult = Rsp1_RecvData(&s_RSDPrtclCtrlList[LinkIndex],
				&s_RSDPrtclCtrlList[LinkIndex].safeCtrl[0], inBuf, inLen_U16); /* ������ջ��� */
			if (tmpResult == r_TRUE)
			{
				result_U8 = 3U;
			}
			else
			{
				result_U8 = 2U; /*��ʱ֡*/
			}
		}
	}
	return result_U8;
}
/****************************************************************
*Name:   LEU_CheckBtmPackage
*Function:����յ������ݰ����������
*para:inDstDevId_U16���豸ʶ��ţ�
inLen_U16�����ݳ��ȣ�
inBuf�����ݻ��壻
outLen����װ��Ӧ�����ݳ��ȣ�
outBuf����װ��Ӧ�����ݻ��壻
*return:0���رյ���·��1����Ч���ݣ�2������֡��3��Уʱ֡
*Create by ChenX 2016/01/01
****************************************************************/
UTiny LEU_CheckBtmPackage(const UTiny LEUIndex,
	const UShort inLen_U16,
	UTiny *inBuf,
	UShort *outLen,
	UTiny *outBuf)
{
	UTiny result_U8 = 0U;
	UTiny linkIndex_U8 = 0U;
	E_Bool tmpResult = r_FALSE;

	S_BtmProtocolControl* pLEUCtrl = &s_BtmPrtclCtrlList[LEUIndex];

	if (r_FALSE == pLEUCtrl->protocolCtrl.isOpen)
	{
		result_U8 = 0U;
	}
	else
	{
		if (inLen_U16 > (c_BYTE_SIZE + 22U))
		{
			result_U8 = 1U;
		}
		else
		{
			tmpResult = LEU_RecvData(pLEUCtrl,
				&pLEUCtrl->protocolCtrl.safeCtrl[0], inBuf, inLen_U16); /* ������ջ��� */
			if ((tmpResult == r_TRUE)
				&& (pLEUCtrl->protocolCtrl.safeCtrl[0].btRecvFrameSize > 0U))
			{
				*outLen = pLEUCtrl->protocolCtrl.safeCtrl[0].btRecvFrameSize; /* ������ЧӦ�����ݳ��� */
				F_GL_MemCpy(outBuf, &pLEUCtrl->protocolCtrl.safeCtrl[0].recvInfoBuf[20], *outLen);
				result_U8 = 3U; /*����֡*/
			}
			else
			{
				result_U8 = 2U; /*��ʱ֡*/
			}
		}
	}
	return result_U8;
}



/****************************************************************
*Name:  Rsp1_Close
*Function:�ر���·���رպ�ܾ�������������
*para:inDstDevId_U16���ڵ��豸ʶ���
*return:1,�رճɹ���0���ر�ʧ��
*Create by ChenX 2016/01/01
****************************************************************/
E_Bool Rsp1_Close(const UShort inDstDevId_U16)
{
	E_Bool result_U8 = r_FALSE;
	UTiny linkIndex_U8 = 0U;

	linkIndex_U8 = Rsp1_FindLinkIndex(inDstDevId_U16);
	if (r_FALSE == s_PrtclCtrlList[linkIndex_U8].isOpen)
	{
		result_U8 = r_FALSE;
	}
	else
	{
		Rsp1_FreeChnControl(&s_PrtclCtrlList[linkIndex_U8]);
		s_PrtclCtrlList[linkIndex_U8].isOpen = r_FALSE;
		result_U8 = r_TRUE;
	}
	return result_U8;
}

/****************************************************************
*Name:  Rsp1_Open
*Function:����·
*para:inDstDevId_U16���ڵ��豸ʶ���
*return:1,�رճɹ���0���ر�ʧ��
*Create by ChenX 2016/01/01
****************************************************************/
E_Bool Rsp1_Open(const UShort inDstDevId_U16)
{
	E_Bool result_U8 = r_FALSE;
	UTiny linkIndex_U8 = 0U;
	linkIndex_U8 = Rsp1_FindLinkIndex(inDstDevId_U16);
	if (r_FALSE == s_PrtclCtrlList[linkIndex_U8].isOpen)
	{
		s_PrtclCtrlList[linkIndex_U8].isOpen = r_TRUE;
		result_U8 = r_TRUE;
	}
	else
	{
		result_U8 = r_FALSE;
	}
	return result_U8;
}

/****************************************************************
*Name:  Rsp1_GetSynData
*Function:��ȡͬ������ֵ
*para:outBuf,ͬ�����ݻ���
*return:ͬ�����ݳ���
*Create by ChenX 2016/01/01
****************************************************************/
UShort Rsp1_GetSynData(UTiny *outBuf)
{
	UShort i = 0U;
	UTiny tmpBuff[c_BUFF_SIZE];
	UShort index = 0U;
	UShort tmpCrc = 0U;

	F_GL_MemSet(tmpBuff, 0, (UShort)sizeof(c_BUFF_SIZE));
	if (outBuf != rNULL)
	{
		tmpBuff[index] = s_UsedLinkNum;/*ʹ�õ���·��*/
		index++;
		Rsp1_UShort2Buffer(&tmpBuff[index], s_RsspClock);/*����Э��ջʱ��*/
		index += 2U;
		for (i = 0U; i<s_UsedLinkNum; i++)
		{
			/*ÿ����·ʱ���*/
			Rsp1_ULong2Buffer(&tmpBuff[index + (i * 4U)], s_PrtclCtrlList[i].safeCtrl[0].tcrNow);
			index += 4U;
		}
		tmpCrc = Rsp1_ComputeMesgCRC(tmpBuff, index);
		Rsp1_UShort2Buffer(&tmpBuff[index], tmpCrc);/*CRC*/
		index += 2U;

		F_GL_MemCpy(outBuf, tmpBuff, index);
	}
	return index;
}

/****************************************************************
*Name:  Rsp1_SetSynData
*Function:����ͬ������ֵ
*para:inBuf��ͬ�����ݻ��壻inDataLength_U16��ͬ�����ݳ���
*return:��
*Create by ChenX 2016/01/01
****************************************************************/
void Rsp1_SetSynData(const UTiny *inBuf,
	UShort inDataLength_U16)
{
	UShort linkIndex = 0U;
	UTiny tmpBuff[c_BUFF_SIZE];
	UShort recvCrc = 0U;
	UShort tmpUsedLinkNum = 0U;
	UShort index = 0U;
	UShort tmpCrc = 0U;

	if (inDataLength_U16 >2U)
	{
		F_GL_MemCpy(tmpBuff, inBuf, inDataLength_U16);
		tmpUsedLinkNum = tmpBuff[index];/*ʹ�õ���·��*/
		index++;
		recvCrc = Rsp1_Buffer2UShort(&tmpBuff[inDataLength_U16 - 2U]);
		tmpCrc = Rsp1_ComputeMesgCRC(inBuf, inDataLength_U16 - 2U);
		if ((tmpUsedLinkNum == s_UsedLinkNum) && (tmpCrc == recvCrc))
		{
			s_RsspClock = Rsp1_Buffer2UShort(&tmpBuff[index]);/*����Э��ջʱ��*/
			index += 2U;
			for (linkIndex = 0U; linkIndex < tmpUsedLinkNum; linkIndex++)
			{
				s_PrtclCtrlList[linkIndex].safeCtrl[0].tcrNow = Rsp1_Buffer2ULong(&tmpBuff[index]); /* ��¼����ʱ�� */
				index += 4U;
				s_PrtclCtrlList[linkIndex].safeCtrl[0].linkoffTimeCntr = s_RsspClock; /* ��¼���� */
			}
		}
		else
		{
			Rsp1_SetErrorCode(c_SYSID, c_ERR_SYN_ERR); /* ϵ��ͬ��ʧ�ܣ��㱨 */
		}
	}
}

/****************************************************************
*Name:  Rsp1_FindLinkIndex
*Function:�豸���Ҷ�Ӧ����·
*�������д��е�startIndexΪĳһ���豸����ʼ���кţ�ĳ�豸DevId��ͨ���豸���е�λ��
*��ΪstartIndex + (DevId - (DevId & 0xFF00))��
*para:���ʶ���
*return:�豸ʶ��Ŷ�Ӧ�豸���
*Create by ChenX 2016/01/01
****************************************************************/
static UTiny Rsp1_FindLinkIndex(const UShort inDevId_U16)
{
	UTiny linkIndex = 0U;
	UTiny tmpIndex = 0U;
	UTiny i = 0U;
	UTiny devType = (UTiny)((inDevId_U16 & 0xFF00U) >> 8U);
	UTiny offset = (UTiny)(inDevId_U16 & 0x00FFU);

	for (i = 0U; i < c_MAX_DEVTYPE_NUMBER; i++)
	{
		if ((devType == s_CommTypeIndexTable[i].commType)
			&& (offset <= s_CommTypeIndexTable[i].allNumber))
		{
			tmpIndex = (s_CommTypeIndexTable[i].startIndex + offset) - 1U;
			break;
		}
	}

	if (s_PrtclCtrlList[tmpIndex].devId == inDevId_U16)
	{
		linkIndex = tmpIndex;
	}
	else
	{
		Rsp1_SetErrorCode(inDevId_U16, c_ERR_NO_DEV); /* �޶�Ӧ�����豸���㱨 */
		linkIndex = 0U;
	}
	return linkIndex;
}

/** **************************************************************
*Name:  Rsp1_SafetyCRCCompute
*Function:32λCRC���㣬Ӧ�ò������32λCRC
*para:��Ҫ����CRC�����ݻ��塢���ȡ�ͨ����
*return:������CRC32
*Create by ChenX 2016/01/01
*************************************************************** */
static ULong Rsp1_SafetyCRCCompute(UTiny *inPtr,
	const UShort inSize_U16,
	const UTiny inChanel)
{
	UShort k = 0U;
	ULong result_U32 = 0UL;
	UShort index = 0U;
	UTiny tmpBuff[c_MAX_SIZE];

	const static ULong CRC_32_TABLE[2][256] =
	{
		{
			0x00000000UL, 0x1942B361UL, 0x328566C2UL, 0x2BC7D5A3UL,
			0x650ACD84UL, 0x7C487EE5UL, 0x578FAB46UL, 0x4ECD1827UL,
			0xCA159B08UL, 0xD3572869UL, 0xF890FDCAUL, 0xE1D24EABUL,
			0xAF1F568CUL, 0xB65DE5EDUL, 0x9D9A304EUL, 0x84D8832FUL,
			0x18CE5601UL, 0x018CE560UL, 0x2A4B30C3UL, 0x330983A2UL,
			0x7DC49B85UL, 0x648628E4UL, 0x4F41FD47UL, 0x56034E26UL,
			0xD2DBCD09UL, 0xCB997E68UL, 0xE05EABCBUL, 0xF91C18AAUL,
			0xB7D1008DUL, 0xAE93B3ECUL, 0x8554664FUL, 0x9C16D52EUL,
			0x319CAC02UL, 0x28DE1F63UL, 0x0319CAC0UL, 0x1A5B79A1UL,
			0x54966186UL, 0x4DD4D2E7UL, 0x66130744UL, 0x7F51B425UL,
			0xFB89370AUL, 0xE2CB846BUL, 0xC90C51C8UL, 0xD04EE2A9UL,
			0x9E83FA8EUL, 0x87C149EFUL, 0xAC069C4CUL, 0xB5442F2DUL,
			0x2952FA03UL, 0x30104962UL, 0x1BD79CC1UL, 0x02952FA0UL,
			0x4C583787UL, 0x551A84E6UL, 0x7EDD5145UL, 0x679FE224UL,
			0xE347610BUL, 0xFA05D26AUL, 0xD1C207C9UL, 0xC880B4A8UL,
			0x864DAC8FUL, 0x9F0F1FEEUL, 0xB4C8CA4DUL, 0xAD8A792CUL,
			0x63395804UL, 0x7A7BEB65UL, 0x51BC3EC6UL, 0x48FE8DA7UL,
			0x06339580UL, 0x1F7126E1UL, 0x34B6F342UL, 0x2DF44023UL,
			0xA92CC30CUL, 0xB06E706DUL, 0x9BA9A5CEUL, 0x82EB16AFUL,
			0xCC260E88UL, 0xD564BDE9UL, 0xFEA3684AUL, 0xE7E1DB2BUL,
			0x7BF70E05UL, 0x62B5BD64UL, 0x497268C7UL, 0x5030DBA6UL,
			0x1EFDC381UL, 0x07BF70E0UL, 0x2C78A543UL, 0x353A1622UL,
			0xB1E2950DUL, 0xA8A0266CUL, 0x8367F3CFUL, 0x9A2540AEUL,
			0xD4E85889UL, 0xCDAAEBE8UL, 0xE66D3E4BUL, 0xFF2F8D2AUL,
			0x52A5F406UL, 0x4BE74767UL, 0x602092C4UL, 0x796221A5UL,
			0x37AF3982UL, 0x2EED8AE3UL, 0x052A5F40UL, 0x1C68EC21UL,
			0x98B06F0EUL, 0x81F2DC6FUL, 0xAA3509CCUL, 0xB377BAADUL,
			0xFDBAA28AUL, 0xE4F811EBUL, 0xCF3FC448UL, 0xD67D7729UL,
			0x4A6BA207UL, 0x53291166UL, 0x78EEC4C5UL, 0x61AC77A4UL,
			0x2F616F83UL, 0x3623DCE2UL, 0x1DE40941UL, 0x04A6BA20UL,
			0x807E390FUL, 0x993C8A6EUL, 0xB2FB5FCDUL, 0xABB9ECACUL,
			0xE574F48BUL, 0xFC3647EAUL, 0xD7F19249UL, 0xCEB32128UL,
			0xC672B008UL, 0xDF300369UL, 0xF4F7D6CAUL, 0xEDB565ABUL,
			0xA3787D8CUL, 0xBA3ACEEDUL, 0x91FD1B4EUL, 0x88BFA82FUL,
			0x0C672B00UL, 0x15259861UL, 0x3EE24DC2UL, 0x27A0FEA3UL,
			0x696DE684UL, 0x702F55E5UL, 0x5BE88046UL, 0x42AA3327UL,
			0xDEBCE609UL, 0xC7FE5568UL, 0xEC3980CBUL, 0xF57B33AAUL,
			0xBBB62B8DUL, 0xA2F498ECUL, 0x89334D4FUL, 0x9071FE2EUL,
			0x14A97D01UL, 0x0DEBCE60UL, 0x262C1BC3UL, 0x3F6EA8A2UL,
			0x71A3B085UL, 0x68E103E4UL, 0x4326D647UL, 0x5A646526UL,
			0xF7EE1C0AUL, 0xEEACAF6BUL, 0xC56B7AC8UL, 0xDC29C9A9UL,
			0x92E4D18EUL, 0x8BA662EFUL, 0xA061B74CUL, 0xB923042DUL,
			0x3DFB8702UL, 0x24B93463UL, 0x0F7EE1C0UL, 0x163C52A1UL,
			0x58F14A86UL, 0x41B3F9E7UL, 0x6A742C44UL, 0x73369F25UL,
			0xEF204A0BUL, 0xF662F96AUL, 0xDDA52CC9UL, 0xC4E79FA8UL,
			0x8A2A878FUL, 0x936834EEUL, 0xB8AFE14DUL, 0xA1ED522CUL,
			0x2535D103UL, 0x3C776262UL, 0x17B0B7C1UL, 0x0EF204A0UL,
			0x403F1C87UL, 0x597DAFE6UL, 0x72BA7A45UL, 0x6BF8C924UL,
			0xA54BE80CUL, 0xBC095B6DUL, 0x97CE8ECEUL, 0x8E8C3DAFUL,
			0xC0412588UL, 0xD90396E9UL, 0xF2C4434AUL, 0xEB86F02BUL,
			0x6F5E7304UL, 0x761CC065UL, 0x5DDB15C6UL, 0x4499A6A7UL,
			0x0A54BE80UL, 0x13160DE1UL, 0x38D1D842UL, 0x21936B23UL,
			0xBD85BE0DUL, 0xA4C70D6CUL, 0x8F00D8CFUL, 0x96426BAEUL,
			0xD88F7389UL, 0xC1CDC0E8UL, 0xEA0A154BUL, 0xF348A62AUL,
			0x77902505UL, 0x6ED29664UL, 0x451543C7UL, 0x5C57F0A6UL,
			0x129AE881UL, 0x0BD85BE0UL, 0x201F8E43UL, 0x395D3D22UL,
			0x94D7440EUL, 0x8D95F76FUL, 0xA65222CCUL, 0xBF1091ADUL,
			0xF1DD898AUL, 0xE89F3AEBUL, 0xC358EF48UL, 0xDA1A5C29UL,
			0x5EC2DF06UL, 0x47806C67UL, 0x6C47B9C4UL, 0x75050AA5UL,
			0x3BC81282UL, 0x228AA1E3UL, 0x094D7440UL, 0x100FC721UL,
			0x8C19120FUL, 0x955BA16EUL, 0xBE9C74CDUL, 0xA7DEC7ACUL,
			0xE913DF8BUL, 0xF0516CEAUL, 0xDB96B949UL, 0xC2D40A28UL,
			0x460C8907UL, 0x5F4E3A66UL, 0x7489EFC5UL, 0x6DCB5CA4UL,
			0x23064483UL, 0x3A44F7E2UL, 0x11832241UL, 0x08C19120UL
		}, {
			0x00000000UL, 0x1EF09C7EUL, 0x3DE138FCUL, 0x2311A482UL,
			0x7BC271F8UL, 0x6532ED86UL, 0x46234904UL, 0x58D3D57AUL,
			0xF784E3F0UL, 0xE9747F8EUL, 0xCA65DB0CUL, 0xD4954772UL,
			0x8C469208UL, 0x92B60E76UL, 0xB1A7AAF4UL, 0xAF57368AUL,
			0xFF048983UL, 0xE1F415FDUL, 0xC2E5B17FUL, 0xDC152D01UL,
			0x84C6F87BUL, 0x9A366405UL, 0xB927C087UL, 0xA7D75CF9UL,
			0x08806A73UL, 0x1670F60DUL, 0x3561528FUL, 0x2B91CEF1UL,
			0x73421B8BUL, 0x6DB287F5UL, 0x4EA32377UL, 0x5053BF09UL,
			0xEE045D65UL, 0xF0F4C11BUL, 0xD3E56599UL, 0xCD15F9E7UL,
			0x95C62C9DUL, 0x8B36B0E3UL, 0xA8271461UL, 0xB6D7881FUL,
			0x1980BE95UL, 0x077022EBUL, 0x24618669UL, 0x3A911A17UL,
			0x6242CF6DUL, 0x7CB25313UL, 0x5FA3F791UL, 0x41536BEFUL,
			0x1100D4E6UL, 0x0FF04898UL, 0x2CE1EC1AUL, 0x32117064UL,
			0x6AC2A51EUL, 0x74323960UL, 0x57239DE2UL, 0x49D3019CUL,
			0xE6843716UL, 0xF874AB68UL, 0xDB650FEAUL, 0xC5959394UL,
			0x9D4646EEUL, 0x83B6DA90UL, 0xA0A77E12UL, 0xBE57E26CUL,
			0xCC05F4A9UL, 0xD2F568D7UL, 0xF1E4CC55UL, 0xEF14502BUL,
			0xB7C78551UL, 0xA937192FUL, 0x8A26BDADUL, 0x94D621D3UL,
			0x3B811759UL, 0x25718B27UL, 0x06602FA5UL, 0x1890B3DBUL,
			0x404366A1UL, 0x5EB3FADFUL, 0x7DA25E5DUL, 0x6352C223UL,
			0x33017D2AUL, 0x2DF1E154UL, 0x0EE045D6UL, 0x1010D9A8UL,
			0x48C30CD2UL, 0x563390ACUL, 0x7522342EUL, 0x6BD2A850UL,
			0xC4859EDAUL, 0xDA7502A4UL, 0xF964A626UL, 0xE7943A58UL,
			0xBF47EF22UL, 0xA1B7735CUL, 0x82A6D7DEUL, 0x9C564BA0UL,
			0x2201A9CCUL, 0x3CF135B2UL, 0x1FE09130UL, 0x01100D4EUL,
			0x59C3D834UL, 0x4733444AUL, 0x6422E0C8UL, 0x7AD27CB6UL,
			0xD5854A3CUL, 0xCB75D642UL, 0xE86472C0UL, 0xF694EEBEUL,
			0xAE473BC4UL, 0xB0B7A7BAUL, 0x93A60338UL, 0x8D569F46UL,
			0xDD05204FUL, 0xC3F5BC31UL, 0xE0E418B3UL, 0xFE1484CDUL,
			0xA6C751B7UL, 0xB837CDC9UL, 0x9B26694BUL, 0x85D6F535UL,
			0x2A81C3BFUL, 0x34715FC1UL, 0x1760FB43UL, 0x0990673DUL,
			0x5143B247UL, 0x4FB32E39UL, 0x6CA28ABBUL, 0x725216C5UL,
			0x8806A731UL, 0x96F63B4FUL, 0xB5E79FCDUL, 0xAB1703B3UL,
			0xF3C4D6C9UL, 0xED344AB7UL, 0xCE25EE35UL, 0xD0D5724BUL,
			0x7F8244C1UL, 0x6172D8BFUL, 0x42637C3DUL, 0x5C93E043UL,
			0x04403539UL, 0x1AB0A947UL, 0x39A10DC5UL, 0x275191BBUL,
			0x77022EB2UL, 0x69F2B2CCUL, 0x4AE3164EUL, 0x54138A30UL,
			0x0CC05F4AUL, 0x1230C334UL, 0x312167B6UL, 0x2FD1FBC8UL,
			0x8086CD42UL, 0x9E76513CUL, 0xBD67F5BEUL, 0xA39769C0UL,
			0xFB44BCBAUL, 0xE5B420C4UL, 0xC6A58446UL, 0xD8551838UL,
			0x6602FA54UL, 0x78F2662AUL, 0x5BE3C2A8UL, 0x45135ED6UL,
			0x1DC08BACUL, 0x033017D2UL, 0x2021B350UL, 0x3ED12F2EUL,
			0x918619A4UL, 0x8F7685DAUL, 0xAC672158UL, 0xB297BD26UL,
			0xEA44685CUL, 0xF4B4F422UL, 0xD7A550A0UL, 0xC955CCDEUL,
			0x990673D7UL, 0x87F6EFA9UL, 0xA4E74B2BUL, 0xBA17D755UL,
			0xE2C4022FUL, 0xFC349E51UL, 0xDF253AD3UL, 0xC1D5A6ADUL,
			0x6E829027UL, 0x70720C59UL, 0x5363A8DBUL, 0x4D9334A5UL,
			0x1540E1DFUL, 0x0BB07DA1UL, 0x28A1D923UL, 0x3651455DUL,
			0x44035398UL, 0x5AF3CFE6UL, 0x79E26B64UL, 0x6712F71AUL,
			0x3FC12260UL, 0x2131BE1EUL, 0x02201A9CUL, 0x1CD086E2UL,
			0xB387B068UL, 0xAD772C16UL, 0x8E668894UL, 0x909614EAUL,
			0xC845C190UL, 0xD6B55DEEUL, 0xF5A4F96CUL, 0xEB546512UL,
			0xBB07DA1BUL, 0xA5F74665UL, 0x86E6E2E7UL, 0x98167E99UL,
			0xC0C5ABE3UL, 0xDE35379DUL, 0xFD24931FUL, 0xE3D40F61UL,
			0x4C8339EBUL, 0x5273A595UL, 0x71620117UL, 0x6F929D69UL,
			0x37414813UL, 0x29B1D46DUL, 0x0AA070EFUL, 0x1450EC91UL,
			0xAA070EFDUL, 0xB4F79283UL, 0x97E63601UL, 0x8916AA7FUL,
			0xD1C57F05UL, 0xCF35E37BUL, 0xEC2447F9UL, 0xF2D4DB87UL,
			0x5D83ED0DUL, 0x43737173UL, 0x6062D5F1UL, 0x7E92498FUL,
			0x26419CF5UL, 0x38B1008BUL, 0x1BA0A409UL, 0x05503877UL,
			0x5503877EUL, 0x4BF31B00UL, 0x68E2BF82UL, 0x761223FCUL,
			0x2EC1F686UL, 0x30316AF8UL, 0x1320CE7AUL, 0x0DD05204UL,
			0xA287648EUL, 0xBC77F8F0UL, 0x9F665C72UL, 0x8196C00CUL,
			0xD9451576UL, 0xC7B58908UL, 0xE4A42D8AUL, 0xFA54B1F4UL
		}
	};

	const static UTiny REFLECTED_UINT8[256] =
	{
		0x00U, 0x80U, 0x40U, 0xC0U, 0x20U, 0xA0U, 0x60U, 0xE0U,/**/
		0x10U, 0x90U, 0x50U, 0xD0U, 0x30U, 0xB0U, 0x70U, 0xF0U,/**/
		0x08U, 0x88U, 0x48U, 0xC8U, 0x28U, 0xA8U, 0x68U, 0xE8U,/**/
		0x18U, 0x98U, 0x58U, 0xD8U, 0x38U, 0xB8U, 0x78U, 0xF8U,/**/
		0x04U, 0x84U, 0x44U, 0xC4U, 0x24U, 0xA4U, 0x64U, 0xE4U,/**/
		0x14U, 0x94U, 0x54U, 0xD4U, 0x34U, 0xB4U, 0x74U, 0xF4U,/**/
		0x0CU, 0x8CU, 0x4CU, 0xCCU, 0x2CU, 0xACU, 0x6CU, 0xECU,/**/
		0x1CU, 0x9CU, 0x5CU, 0xDCU, 0x3CU, 0xBCU, 0x7CU, 0xFCU,/**/
		0x02U, 0x82U, 0x42U, 0xC2U, 0x22U, 0xA2U, 0x62U, 0xE2U,/**/
		0x12U, 0x92U, 0x52U, 0xD2U, 0x32U, 0xB2U, 0x72U, 0xF2U,/**/
		0x0AU, 0x8AU, 0x4AU, 0xCAU, 0x2AU, 0xAAU, 0x6AU, 0xEAU,/**/
		0x1AU, 0x9AU, 0x5AU, 0xDAU, 0x3AU, 0xBAU, 0x7AU, 0xFAU,/**/
		0x06U, 0x86U, 0x46U, 0xC6U, 0x26U, 0xA6U, 0x66U, 0xE6U,/**/
		0x16U, 0x96U, 0x56U, 0xD6U, 0x36U, 0xB6U, 0x76U, 0xF6U,/**/
		0x0EU, 0x8EU, 0x4EU, 0xCEU, 0x2EU, 0xAEU, 0x6EU, 0xEEU,/**/
		0x1EU, 0x9EU, 0x5EU, 0xDEU, 0x3EU, 0xBEU, 0x7EU, 0xFEU,/**/
		0x01U, 0x81U, 0x41U, 0xC1U, 0x21U, 0xA1U, 0x61U, 0xE1U,/**/
		0x11U, 0x91U, 0x51U, 0xD1U, 0x31U, 0xB1U, 0x71U, 0xF1U,/**/
		0x09U, 0x89U, 0x49U, 0xC9U, 0x29U, 0xA9U, 0x69U, 0xE9U,/**/
		0x19U, 0x99U, 0x59U, 0xD9U, 0x39U, 0xB9U, 0x79U, 0xF9U,/**/
		0x05U, 0x85U, 0x45U, 0xC5U, 0x25U, 0xA5U, 0x65U, 0xE5U,/**/
		0x15U, 0x95U, 0x55U, 0xD5U, 0x35U, 0xB5U, 0x75U, 0xF5U,/**/
		0x0DU, 0x8DU, 0x4DU, 0xCDU, 0x2DU, 0xADU, 0x6DU, 0xEDU,/**/
		0x1DU, 0x9DU, 0x5DU, 0xDDU, 0x3DU, 0xBDU, 0x7DU, 0xFDU,/**/
		0x03U, 0x83U, 0x43U, 0xC3U, 0x23U, 0xA3U, 0x63U, 0xE3U,/**/
		0x13U, 0x93U, 0x53U, 0xD3U, 0x33U, 0xB3U, 0x73U, 0xF3U,/**/
		0x0BU, 0x8BU, 0x4BU, 0xCBU, 0x2BU, 0xABU, 0x6BU, 0xEBU,/**/
		0x1BU, 0x9BU, 0x5BU, 0xDBU, 0x3BU, 0xBBU, 0x7BU, 0xFBU,/**/
		0x07U, 0x87U, 0x47U, 0xC7U, 0x27U, 0xA7U, 0x67U, 0xE7U,/**/
		0x17U, 0x97U, 0x57U, 0xD7U, 0x37U, 0xB7U, 0x77U, 0xF7U,/**/
		0x0FU, 0x8FU, 0x4FU, 0xCFU, 0x2FU, 0xAFU, 0x6FU, 0xEFU,/**/
		0x1FU, 0x9FU, 0x5FU, 0xDFU, 0x3FU, 0xBFU, 0x7FU, 0xFFU/**/
	};

	F_GL_MemSet(tmpBuff, 0, c_MAX_SIZE);
	if ((inPtr != rNULL) && (inChanel <= 1U))
	{
		F_GL_MemCpy(tmpBuff, inPtr, inSize_U16);
		for (k = 0U; k < inSize_U16; k++)
		{
			result_U32 = ((result_U32 >> 8U) & 0x00FFFFFFUL)
				^ CRC_32_TABLE[inChanel][(UTiny)((result_U32 ^ REFLECTED_UINT8[tmpBuff[index]]) & 0xFFU)];
			index++;
		}
	}
	else
	{
		result_U32 = 0U;
	}
	return result_U32;
}

/****************************************************************
*Name:  Rsp1_ComputeMesgCRC
*Function:16λCRC����
*para:����CRC�����ݻ��塢����
*return:CRC16ֵ
*Create by ChenX 2016/01/01
****************************************************************/
static UShort Rsp1_ComputeMesgCRC(const UTiny* inAddr,
	UShort inLen_U16)
{
	UTiny bitCount = 0U;
	UShort actByte = 0U;
	UShort compCRC = 0U;
	UShort index = 0U;
	UTiny tmpBuff[c_BUFF_SIZE];

	F_GL_MemSet(tmpBuff, 0, c_BUFF_SIZE);
	F_GL_MemCpy(tmpBuff, inAddr, inLen_U16);
	compCRC = 0U;

	while (inLen_U16 > 0U)
	{
		actByte = tmpBuff[index];

		for (bitCount = 8U; bitCount > 0U; bitCount--)
		{
			if (((actByte ^ compCRC) & 1U) > 0U)
			{
				compCRC = (compCRC >> 1U) ^ c_MSG_CRC_MASK;
			}
			else
			{
				compCRC >>= 1U;
			}

			actByte >>= 1U;
		}

		index++;
		inLen_U16--;
	}

	return compCRC;
}

/****************************************************************
*Name: Rsp1_FreeChnControl
*Function:������ݻ���
*��SSE���ͻ��塢SSR���ͻ��塢���ջ��塢���ͻ��塢������Ӧ�����ݻ��塢������Ӧ�����ݻ�����ա�
*para:��Ҫ��������·
*return:��
*Create by ChenX 2016/01/01
****************************************************************/
static void Rsp1_FreeChnControl(S_ProtocolControl *inProtocolCtrl)
{
	UTiny i = 0U;

	for (i = 0U; i < c_CHANEL_NUMBER; i++)
	{
		F_GL_MemSet(inProtocolCtrl->safeCtrl[i].ssrBuf, 0, c_SSR_SIZE);
		F_GL_MemSet(inProtocolCtrl->safeCtrl[i].sseBuff, 0, c_SSE_SIZE);
		F_GL_MemSet(inProtocolCtrl->safeCtrl[i].recvInfoBuf, 0, c_BUFF_SIZE);
		F_GL_MemSet(inProtocolCtrl->safeCtrl[i].sendInfoBuf, 0, c_BUFF_SIZE);
		F_GL_MemSet(inProtocolCtrl->safeCtrl[i].pRecvBuf, 0, c_BUFF_SIZE);
		F_GL_MemSet(inProtocolCtrl->safeCtrl[i].pSendBuf, 0, c_BUFF_SIZE);
		inProtocolCtrl->safeCtrl[i].linkoffTimeCntr = 0U;
	}
}

/****************************************************************
*Name:  Rsp1_ResetProtocolControl
*Function:��ʼ��Э��ջ״̬
*��Э��ջʱ�ӡ�SSE��������ֵ������֡��������ֵ��ʱ�������ṹ�塢���Ϳ��ƽṹ
*�塢ʱ�������Ĵ�����SSE���ͻ��塢SSR���ͻ��塢���ջ��塢���ͻ��塢������Ӧ��
*���ݻ��塢������Ӧ�����ݻ�����ա�
*para:��Ҫ��������·
*return:��
*Create by ChenX 2016/01/01
****************************************************************/
static void Rsp1_ResetProtocolControl(S_ProtocolControl *inProtocolCtrl)
{
	UTiny i = 0U;/*init*/
	S_NodeParam* strNodePara;
	S_ChnControl *pctrl;

	for (i = 0U; i < c_CHANEL_NUMBER; i++)
	{
		/*��ʼ��Э��ջʱ��*/
		if (inProtocolCtrl != rNULL)
		{
			pctrl = &inProtocolCtrl->safeCtrl[i];
			strNodePara = &inProtocolCtrl->nodeParamSelf;
			pctrl->btRecvFrameSize = 0U;/*init*/
			pctrl->btSendInfoSize = 0U;/*init*/

			pctrl->rsdSendTimer = s_RsspClock;/*init*/
			pctrl->sseSendTimer = s_RsspClock;/*init*/
			pctrl->bTimeStampCorrect = r_TRUE;/*init*/

			pctrl->timeStampMgr.tc = 0U;/*init*/
			pctrl->timeStampMgr.tNow[0] = strNodePara->sid[0];
			pctrl->timeStampMgr.tNow[1] = strNodePara->sid[1];

			/* ��ʼ��S_SendControl*/
			F_GL_MemSet(&(pctrl->sendCtrl), 0, (UShort)sizeof(S_SendControl));

			pctrl->sendCtrl.sendSse = r_FALSE;
			pctrl->sseCnt = 0U;

			pctrl->onSendSSE[0] = 0U;
			pctrl->onSendSSE[1] = 0U;
			/* ���tcLast*/
			pctrl->tcLast = 0U;
			/* ���tcrLast*/
			pctrl->tcrLast = 0U;
			/* ���tcrNow*/
			pctrl->tcrNow = 0U;
			/* ���nowSidXorTn*/
			pctrl->nowSidXorTn[0] = 0U;
			/* ���nowSidXorTn*/
			pctrl->nowSidXorTn[1] = 0U;
			/* ���lastSidXorTn*/
			pctrl->lastSidXorTn[0] = 0U;
			/* ���lastSidXorTn*/
			pctrl->lastSidXorTn[1] = 0U;

			/*���SSE*/
			F_GL_MemSet(&(pctrl->sseInfoTx), 0, (UShort)sizeof(S_SseInfo));
			F_GL_MemSet((&pctrl->sseInfoTx), 0, (UShort)sizeof(S_SseInfo));
			/*���SSR*/
			F_GL_MemSet((&pctrl->ssrInfoTx), 0, (UShort)sizeof(S_SsrInfo));
			F_GL_MemSet(&(pctrl->ssrInfoRx), 0, (UShort)sizeof(S_SsrInfo));
			/*���ssrBuf*/
			F_GL_MemSet(pctrl->ssrBuf, 0, c_SSR_SIZE);
			/*���sseBuff*/
			F_GL_MemSet(pctrl->sseBuff, 0, c_SSE_SIZE);
			/*���recvInfoBuf*/
			F_GL_MemSet(pctrl->recvInfoBuf, 0, c_BUFF_SIZE);
			/*���sendInfoBuf*/
			F_GL_MemSet(pctrl->sendInfoBuf, 0, c_BUFF_SIZE);
			/*���pRecvBuf*/
			F_GL_MemSet(pctrl->pRecvBuf, 0, c_BUFF_SIZE);
			/*���pSendBuf*/
			F_GL_MemSet(pctrl->pSendBuf, 0, c_BUFF_SIZE);

			pctrl->byteInRecvBuf = 0U;
			pctrl->byteInSendBuf = 0U;
			/*�����ʼʱ��*/
			Rsp1_CalculateFirstSinit(0U, pctrl, inProtocolCtrl);
			Rsp1_CalculateFirstSinit(1U, pctrl, inProtocolCtrl);
		}
	}
}

/****************************************************************
*Name:  Rsp1_AddTimeStamp
*Function:����ʱ�����ͨ���Ա�����ʱ����Ĵ����ڵ�ֵ��Load��Add���㣬��ʱ����ۼӡ�
*para:��Ҫ��������·��ʱ����Ĵ���
*return:��
*Create by ChenX 2016/01/01
****************************************************************/
static void Rsp1_AddTimeStamp(S_ProtocolControl *inProtocolCtrl,
	S_TimeStampMgr* inTsMgr)
{
	UTiny i = 0U;

	/*����ʱ���*/
	inTsMgr->tc++;
	for (i = 0U; i < c_CHANEL_NUMBER; i++)
	{
		inTsMgr->tLast[i] = inTsMgr->tNow[i];
		Rsp1_Load(i, inTsMgr->tNow[i], inProtocolCtrl);
		Rsp1_Add(i, 0U, inProtocolCtrl); /* ʱ������� */
		inTsMgr->tNow[i] = Rsp1_Read(i, inProtocolCtrl);
	}
}

/****************************************************************
*Name: Rsp1_CheckMesgCRC
*Function:16λCRC��飬����CRC16�������ݰ��е�CRC16���бȽϣ������ֵ����򷵻��棬����Ϊ�١�
*para:��ҪУ������ݻ��壬���ݳ���
*return:�棬У��ͨ�����٣�У��ʧ�ܡ�
*Create by ChenX 2016/01/01
****************************************************************/
static E_Bool Rsp1_CheckMesgCRC(const UTiny* inDataBuf,
	const UShort inDataSize_U16)
{
	UShort mesgCRC_U16 = 0U;
	UShort len_U16 = 0U;
	E_Bool isRight = r_FALSE;
	UTiny tmpBuff[c_BUFF_SIZE];

	F_GL_MemCpy(tmpBuff, inDataBuf, inDataSize_U16);
	len_U16 = inDataSize_U16 - 2U;
	if (len_U16 < c_BUFF_SIZE)
	{
		mesgCRC_U16 = Rsp1_Buffer2UShort(&tmpBuff[len_U16]);
		if (Rsp1_ComputeMesgCRC(inDataBuf, len_U16) == mesgCRC_U16)
		{
			isRight = r_TRUE;
		}
		else
		{
			isRight = r_FALSE;
		}
	}

	return isRight;
}

/****************************************************************
*Name: Rsp1_AnalyzeSSE
*Function:����SSE֡������SSE֡�����õ�������ʱ�������кŴ��뱾��sseInfoRx�У�������SSR֡���Ϳ��Ʒ���
*para:��Ҫ������ͨ��
*return:��
*Create by ChenX 2016/01/01
****************************************************************/
static void Rsp1_AnalyzeSSE(S_ChnControl *inChnCtrl)
{
	UTiny pbuf[c_SSE_SIZE];

	F_GL_MemCpy(pbuf, inChnCtrl->sseBuff, c_SSE_SIZE);
	/*��������ʱ�������к�*/
	inChnCtrl->sseInfoRx.tc = Rsp1_Buffer2ULong(&pbuf[6U]);
	inChnCtrl->sseInfoRx.seq[0] = Rsp1_Buffer2ULong(&pbuf[10U]);
	inChnCtrl->sseInfoRx.seq[1] = Rsp1_Buffer2ULong(&pbuf[14U]);
	/*����SSR�����Ʒ���ֵ*/
	inChnCtrl->sendCtrl.sendSsr = r_TRUE;
}

/****************************************************************
*Name: Rsp1_AnalyzeSSR
*Function:����SSR֡����鱾���Ƿ����SSE֡������SSE�ȴ�ʱ����Ч������Ч��
*������ʱ��ָ�ΪSSRЯ��ʱ�򣬷��򽫶�����SSR֡��
*para:��Ҫ��������·����Ҫ������ͨ��
*return:��
*Create by ChenX 2016/01/01
****************************************************************/
static void Rsp1_AnalyzeSSR(S_ProtocolControl *inProtocolCtrl,
	S_ChnControl *inChnCtrl)
{
	UTiny pbuf[c_SSR_SIZE];
	ULong tempTC_U32 = 0UL;

	F_GL_MemCpy(pbuf, inChnCtrl->ssrBuf, c_SSR_SIZE);

	if (Rsp1_DiffTime(inChnCtrl->sseSendTimer) >= inProtocolCtrl->timeoutToleration) /* �ж��Ƿ�ʱ */
	{
		Rsp1_SetErrorCode(inProtocolCtrl->devId, c_ERR_SSR_OVER); /* ʱ�������ʱ�䳬��ʱ�����̷�Χ */
	}
	else
	{
		tempTC_U32 = Rsp1_Buffer2ULong(&pbuf[10]);
		if (inChnCtrl->tcLast != tempTC_U32)
		{
			Rsp1_SetErrorCode(inProtocolCtrl->devId, c_ERR_NOT_COMPARED);/* ʱ����Ƕ�Ӧ���� */
		}
		else
		{
			inChnCtrl->sendCtrl.sendSse = r_FALSE; /* SSE���Ϳ�������� */
			inChnCtrl->bTimeStampCorrect = r_TRUE;
			inChnCtrl->sseCnt = 0U;

			inChnCtrl->linkoffTimeCntr = s_RsspClock;
			inChnCtrl->ssrInfoRx.tcr = Rsp1_Buffer2ULong(&pbuf[6U]);
			inChnCtrl->tcrNow = inChnCtrl->ssrInfoRx.tcr;
			inChnCtrl->ssrInfoRx.tce = Rsp1_Buffer2ULong(&pbuf[10U]);
			inChnCtrl->ssrInfoRx.seqInit[0] = Rsp1_Buffer2ULong(&pbuf[14U]); /* �������к� */
			inChnCtrl->ssrInfoRx.seqInit[1] = Rsp1_Buffer2ULong(&pbuf[18U]);

			inChnCtrl->ssrInfoRx.dataVer = pbuf[22U];

			inChnCtrl->lastSidXorTn[0] = Rsp1_GenerateSINIT(0U, inChnCtrl,
				inProtocolCtrl);
			inChnCtrl->lastSidXorTn[1] = Rsp1_GenerateSINIT(1U, inChnCtrl,
				inProtocolCtrl);
		}
	}
}

/****************************************************************
*Name: Rsp1_AnalyzeRSD
*Function:����֡����
*��ͨ��CheckTimeStamp����ʱ�����飬���ݼ����������ͬ������
*���ʱ�����ȷ��������ݰ����ݣ�������к�С�ڵ�ǰ���к��򲻴���
*����������ʱ���������ʱ�����ϲ�����SSE��
*para:��Ҫ��������·����Ҫ������ͨ��
*return:0��ʱ�������1��ʱ�����ȷ��2���ظ����ݰ�
*Create by ChenX 2016/01/01
****************************************************************/
static void Rsp1_AnalyzeRSD(S_ProtocolControl *inProtocolCtrl,
	S_ChnControl *inChnCtrl)
{
	UTiny pbuf[c_BUFF_SIZE];
	E_TimeStampCheckResult checkResult = e_TsCheckErr;
	F_GL_MemCpy(pbuf, inChnCtrl->recvInfoBuf, c_BUFF_SIZE);

	checkResult = Rsp1_CheckTimeStamp(pbuf, inChnCtrl->btRecvFrameSize,
		inProtocolCtrl, inChnCtrl);
	switch (checkResult)
	{
	case e_TsCheckOk: /* ʱ��������ȷʱ�Ŵ�������ݰ� */
		inChnCtrl->sendCtrl.sendSse = r_FALSE;
		inChnCtrl->bTimeStampCorrect = r_TRUE;
		inChnCtrl->linkoffTimeCntr = s_RsspClock;
		break;
	case e_TsCheckRepeat: /* �������ظ����ݰ� */
		break;
	default: /* ʱ���������ʱ�����ϲ�����SSE */
		inChnCtrl->sendCtrl.sendSse = r_TRUE;
		inChnCtrl->bTimeStampCorrect = r_FALSE;
		Rsp1_SetErrorCode(inProtocolCtrl->devId, c_ERR_NOT_TIMERSTAMP);/* ʱ��������� */
		break;
	}
}

/****************************************************************
*Name: Rsp1_AnalyseRecvFrames
*Function:��ͬ����֡����
*para:��Ҫ��������·����Ҫ������ͨ��
*return:0��ʱ�������1��ʱ�����ȷ��2���ظ����ݰ�
*Create by ChenX 2016/01/01
****************************************************************/
static void Rsp1_AnalyseRecvFrames(UTiny inApplCat,
	UTiny inMsgType,
	S_ProtocolControl *inProtocolControl,
	S_ChnControl *inChnCtrl)
{
	if ((c_APPLCAT_MAINCTL == inApplCat) || (c_APPLCAT_STANDBY == inApplCat) || (c_APPLCAT_BTM == inApplCat) || (c_APPLCAT_BTM == inApplCat))
	{
		switch (inMsgType)
		{
		case c_MSG_SSE:
			Rsp1_AnalyzeSSE(inChnCtrl);
			Rsp1_SendSSR(inProtocolControl, inChnCtrl);
			inChnCtrl->sendCtrl.sendTsd = r_FALSE;
			break;
		case c_MSG_SSR:
			if (r_FALSE == inChnCtrl->bTimeStampCorrect)
			{
				Rsp1_AnalyzeSSR(inProtocolControl, inChnCtrl);
			}
			break;
		case c_MSG_A:
		case c_MSG_B:
			Rsp1_AnalyzeRSD(inProtocolControl, inChnCtrl);
			if (r_TRUE == inChnCtrl->sendCtrl.sendSse)
			{
				if ((Rsp1_DiffTime(inChnCtrl->sseSendTimer) >= c_TIMEOUT_SSE_SEND || inChnCtrl->sseSendTimer == 0)
					&& (inChnCtrl->sendCtrl.sendSsr == r_FALSE))
				{
					Rsp1_SendSSE(inProtocolControl, inChnCtrl);
					inChnCtrl->sendCtrl.sendTsd = r_FALSE;
				}
			}
			break;
		default:
			break;
		}
	}
}
/****************************************************************
*Name: LEU_AnalyseRecvFrames
*Function:��ͬ����֡����
*para:��Ҫ��������·����Ҫ������ͨ��
*return:0��ʱ�������1��ʱ�����ȷ��2���ظ����ݰ�
*Create by ChenX 2016/01/01
****************************************************************/
static void LEU_AnalyseRecvFrames(UTiny inApplCat,
	UTiny inMsgType,
	S_BtmProtocolControl *inProtocolControl,
	S_ChnControl *inChnCtrl)
{
	if (c_APPLCAT_BTM == inApplCat)
	{
		switch (inMsgType)
		{
		case c_MSG_SSE:
			Rsp1_AnalyzeSSE(inChnCtrl);
			LEU_SendSSR(inProtocolControl, inChnCtrl);
			inChnCtrl->sendCtrl.sendTsd = r_FALSE;
			break;
		case c_MSG_SSR:
			if (r_FALSE == inChnCtrl->bTimeStampCorrect)
			{
				Rsp1_AnalyzeSSR(&inProtocolControl->protocolCtrl, inChnCtrl);
			}
			break;
		case c_MSG_ABT:

			Rsp1_AnalyzeRSD(&inProtocolControl->protocolCtrl, inChnCtrl);
			if (r_TRUE == inChnCtrl->sendCtrl.sendSse)
			{
				if ((Rsp1_DiffTime(inChnCtrl->sseSendTimer) >= c_TIMEOUT_BTM_SSE_SEND || inChnCtrl->sseSendTimer == 0)
					&& (inChnCtrl->sendCtrl.sendSsr == r_FALSE))
				{
					LEU_SendSSE(inProtocolControl, inChnCtrl);
					inChnCtrl->sendCtrl.sendTsd = r_FALSE;
				}
				else
				{
					if (inProtocolControl->btm_status == e_start)
					{
						inProtocolControl->btm_status = e_initover;
					}
					if (inProtocolControl->btm_status == e_contrl)
					{
						inProtocolControl->btm_status = e_contrlover;
					}
				}
			}
			else
			{
				if (inProtocolControl->btm_status == e_start)
				{
					inProtocolControl->btm_status = e_initover;
				}
				if (inProtocolControl->btm_status == e_contrl)
				{
					inProtocolControl->btm_status = e_contrlover;
				}
			}

			break;
		case c_MSG_EBT:
			inProtocolControl->btm_status = e_init;
			break;
		default:
			break;
		}
	}
}
/****************************************************************
*Name:Rsp1_RecvData
*Function:��������
*para:��Ҫ��������·����Ҫ������ͨ�����������ݻ��壬�������ݳ���
*return:r_FALSE�����չ��ϣ�r_TRUE�����ճɹ�
*Create by ChenX 2016/01/01
****************************************************************/
static E_Bool Rsp1_RecvData(S_ProtocolControl *inProtocolCtrl,
	S_ChnControl *inChnCtrl,
	UTiny *inDataPtr,
	UShort inLen_U16)
{
	E_Bool result = r_FALSE;

	if ((inLen_U16 <= c_BUFF_SIZE) && (inDataPtr != rNULL) && (inChnCtrl != rNULL))
	{
		inChnCtrl->sendCtrl.sendSsr = r_FALSE;
		inChnCtrl->sendCtrl.sendSse = r_FALSE;
		inChnCtrl->sendCtrl.sendTsd = r_FALSE;

		F_GL_MemCpy(&inChnCtrl->pRecvBuf[0], inDataPtr, inLen_U16);
		inChnCtrl->byteInRecvBuf = inLen_U16;

		result = Rsp1_PartitionRecvDataPackage(inProtocolCtrl, inChnCtrl);
	}
	return result;
}

/****************************************************************
*Name:Rsp1_LEU_RecvData
*Function:��������
*para:��Ҫ��������·����Ҫ������ͨ�����������ݻ��壬�������ݳ���
*return:r_FALSE�����չ��ϣ�r_TRUE�����ճɹ�
*Create by ChenX 2016/01/01
****************************************************************/
static E_Bool LEU_RecvData(S_BtmProtocolControl *inProtocolCtrl,
	S_ChnControl *inChnCtrl,
	UTiny *inDataPtr,
	UShort inLen_U16)
{
	E_Bool result = r_FALSE;

	if ((inLen_U16 <= c_BUFF_SIZE) && (inDataPtr != rNULL) && (inChnCtrl != rNULL))
	{
		inChnCtrl->sendCtrl.sendSsr = r_FALSE;
		inChnCtrl->sendCtrl.sendSse = r_FALSE;
		inChnCtrl->sendCtrl.sendTsd = r_FALSE;

		F_GL_MemCpy(&inChnCtrl->pRecvBuf[0], inDataPtr, inLen_U16);
		inChnCtrl->byteInRecvBuf = inLen_U16;

		result = LEU_PartitionRecvDataPackage(inProtocolCtrl, inChnCtrl);
	}
	return result;
}

/****************************************************************
*Name: Rsp1_PartitionRecvDataPackage
*Function:��ְ�
*�����յ������ݻ����ֽ�������Դ��ַ��Ŀ�ĵ�ַ��Ӧ�����͡�CRC16��Ӧ��
*���ݳ��Ƚ����ж�����֡�Ƿ���Ч��
*para:��Ҫ��������·����Ҫ������ͨ��
*return:0�����չ��ϣ�1�����ճɹ�
*Create by ChenX 2016/01/01
****************************************************************/
static E_Bool Rsp1_PartitionRecvDataPackage(S_ProtocolControl *inProtocolCtrl,
	S_ChnControl *inChnCtrl)
{
	E_Bool isOk = r_FALSE;
	E_Bool tmpBool = r_FALSE;
	UTiny pbuf[c_BUFF_SIZE];
	UTiny* panabuf = inChnCtrl->pRecvBuf;
	UTiny applCat_U8 = 0U;
	UTiny msgType_U8 = 0U;
	UShort leftlength_U16 = 0U;
	UShort framesize_U16 = 0U;
	UShort templen_U16 = 0U;
	UShort wSrc_U16 = 0U;
	UShort wDest_U16 = 0U;

	templen_U16 = inChnCtrl->byteInRecvBuf;
	F_GL_MemCpy(pbuf, inChnCtrl->pRecvBuf, templen_U16);
	wSrc_U16 = inProtocolCtrl->nodeParamPeer.sourceAddress;
	wDest_U16 = inProtocolCtrl->nodeParamSelf.sourceAddress;

	if ((templen_U16 < 20U) || (0U == wSrc_U16) || (0U == wDest_U16))
	{
		isOk = r_FALSE;
	}
	else
	{
		leftlength_U16 = templen_U16;
		isOk = r_TRUE;
		while ((leftlength_U16 >= c_SSE_SIZE) && (r_TRUE == isOk))
		{
			if ((wSrc_U16 == (pbuf[2] + (pbuf[3] * 256U))) &&
				(wDest_U16 == (pbuf[4] + (pbuf[5] * 256U))))
			{
				applCat_U8 = pbuf[0];
				msgType_U8 = pbuf[1];

				framesize_U16 = 0U;

				if ((applCat_U8 != c_APPLCAT_MAINCTL) && (applCat_U8 != c_APPLCAT_STANDBY))
				{
					isOk = r_FALSE;
				}
				else
				{
					switch (msgType_U8)
					{
					case c_MSG_SSR:
						framesize_U16 = c_SSR_SIZE;
						panabuf = inChnCtrl->ssrBuf;
						break;
					case c_MSG_SSE:
						framesize_U16 = c_SSE_SIZE;
						panabuf = inChnCtrl->sseBuff;
						break;
					case c_MSG_A:
					case c_MSG_B:
						framesize_U16 = pbuf[10] + (pbuf[11] * 256U) + 14U;
						panabuf = inChnCtrl->recvInfoBuf;
						break;
					default:
						framesize_U16 = 0U;
						break;
					}

					tmpBool = Rsp1_CheckMesgCRC(pbuf, framesize_U16);
					if ((framesize_U16 > 0U) &&
						(leftlength_U16 >= framesize_U16) &&
						(r_TRUE == tmpBool))
					{
						F_GL_MemCpy(panabuf, pbuf, framesize_U16);
						if (framesize_U16 > 25U)
						{
							inChnCtrl->btRecvFrameSize = framesize_U16 - 22U;
						}
						else
						{
							inChnCtrl->btRecvFrameSize = 0U;
						}
						Rsp1_AnalyseRecvFrames(applCat_U8, msgType_U8, inProtocolCtrl, inChnCtrl);
						F_GL_MemCpy(&pbuf[0], &pbuf[framesize_U16], leftlength_U16);
						inChnCtrl->byteInRecvBuf -= framesize_U16;
						leftlength_U16 = inChnCtrl->byteInRecvBuf;
						isOk = r_TRUE;
					}
					else
					{
						Rsp1_SetErrorCode(wDest_U16, c_ERR_CRC);
						isOk = r_FALSE;
					}
				}
			}
			else
			{
				inChnCtrl->byteInRecvBuf = 0U;
				leftlength_U16 = inChnCtrl->byteInRecvBuf;
				isOk = r_FALSE;
			}
		}
	}
	return isOk;
}



/****************************************************************
*Name: LEU_PartitionRecvDataPackage
*Function:��ְ�
*�����յ������ݻ����ֽ�������Դ��ַ��Ŀ�ĵ�ַ��Ӧ�����͡�CRC16��Ӧ��
*���ݳ��Ƚ����ж�����֡�Ƿ���Ч��
*para:��Ҫ��������·����Ҫ������ͨ��
*return:0�����չ��ϣ�1�����ճɹ�
*Create by ChenX 2016/01/01
****************************************************************/
static E_Bool LEU_PartitionRecvDataPackage(S_BtmProtocolControl *inProtocolCtrl,
	S_ChnControl *inChnCtrl)
{
	E_Bool isOk = r_FALSE;
	E_Bool tmpBool = r_FALSE;
	UTiny pbuf[c_BUFF_SIZE];
	UTiny* panabuf = inChnCtrl->pRecvBuf;
	UTiny applCat_U8 = 0U;
	UTiny msgType_U8 = 0U;
	UShort leftlength_U16 = 0U;
	UShort framesize_U16 = 0U;
	UShort templen_U16 = 0U;
	UShort wSrc_U16 = 0U;
	UShort wDest_U16 = 0U;

	templen_U16 = inChnCtrl->byteInRecvBuf;
	F_GL_MemCpy(pbuf, inChnCtrl->pRecvBuf, templen_U16);
	wSrc_U16 = inProtocolCtrl->protocolCtrl.nodeParamPeer.sourceAddress;
	wDest_U16 = inProtocolCtrl->protocolCtrl.nodeParamSelf.sourceAddress;

	if ((templen_U16 < 20U) || (0U == wSrc_U16) || (0U == wDest_U16))
	{
		isOk = r_FALSE;
	}
	else
	{
		leftlength_U16 = templen_U16;
		isOk = r_TRUE;
		while ((leftlength_U16 >= c_SSE_SIZE) && (r_TRUE == isOk))
		{
			if ((wSrc_U16 == (pbuf[2] + (pbuf[3] * 256U))) &&
				(wDest_U16 == (pbuf[4] + (pbuf[5] * 256U))))
			{
				applCat_U8 = pbuf[0];
				msgType_U8 = pbuf[1];

				framesize_U16 = 0U;

				if (applCat_U8 != c_APPLCAT_BTM)
				{
					isOk = r_FALSE;
				}
				else
				{
					switch (msgType_U8)
					{
					case c_MSG_SSR:
						framesize_U16 = c_SSR_SIZE;
						panabuf = inChnCtrl->ssrBuf;
						break;
					case c_MSG_SSE:
						framesize_U16 = c_SSE_SIZE;
						panabuf = inChnCtrl->sseBuff;
						break;
					case c_MSG_ABT:
						framesize_U16 = 22;
						panabuf = inChnCtrl->recvInfoBuf;
						break;
					case c_MSG_EBT:
						framesize_U16 = 22;
						panabuf = inChnCtrl->recvInfoBuf;
						break;
					case c_MSG_BTD:
						printf("Recv BTD!\n");
						framesize_U16 = pbuf[10] + pbuf[11] * 256 + 14;
						panabuf = inChnCtrl->recvInfoBuf;
						if (inProtocolCtrl->btm_status == e_contrlover || inProtocolCtrl->btm_status == e_contrl)
						{
							inProtocolCtrl->btm_status = e_transfer;
						}
						break;
					default:
						framesize_U16 = 0U;
						break;
					}

					tmpBool = Rsp1_CheckMesgCRC(pbuf, framesize_U16);
					if ((framesize_U16 > 0U) &&
						(leftlength_U16 >= framesize_U16) &&
						(r_TRUE == tmpBool))
					{
						F_GL_MemCpy(panabuf, pbuf, framesize_U16);
						if (framesize_U16 > 25U)
						{
							inChnCtrl->btRecvFrameSize = framesize_U16 - 22U;
						}
						else
						{
							inChnCtrl->btRecvFrameSize = 0U;
						}
						LEU_AnalyseRecvFrames(applCat_U8, msgType_U8, inProtocolCtrl, inChnCtrl);
						F_GL_MemCpy(&pbuf[0], &pbuf[framesize_U16], leftlength_U16);
						inChnCtrl->byteInRecvBuf -= framesize_U16;
						leftlength_U16 = inChnCtrl->byteInRecvBuf;
						isOk = r_TRUE;
					}
					else
					{
						isOk = r_FALSE;
					}
				}
			}
			else
			{
				inChnCtrl->byteInRecvBuf = 0U;
				leftlength_U16 = inChnCtrl->byteInRecvBuf;
				isOk = r_FALSE;
			}
		}
	}
	return isOk;
}
/****************************************************************
*Name: Rsp1_PreComputeOfTimeStampChk
*Function:��ʼʱ�������
*para:��Ҫ��������·
*return:��
*Create by ChenX 2016/01/01
****************************************************************/
static void Rsp1_PreComputeOfTimeStampChk(S_ProtocolControl *inProtocolCtrl)
{
	UTiny m, n;
	ULong valueTmp, valueTmp1;
	UTiny chanel;
	S_NodeParam* strPeerNodePara = rNULL;

	if (inProtocolCtrl != rNULL)
	{
		strPeerNodePara = &inProtocolCtrl->nodeParamPeer;
		for (chanel = 0U; chanel < c_CHANEL_NUMBER; chanel++)
		{
			Rsp1_Load(chanel, strPeerNodePara->sinit[chanel], inProtocolCtrl);
			Rsp1_Add(chanel, strPeerNodePara->sid[chanel], inProtocolCtrl);
			valueTmp = Rsp1_Read(chanel, inProtocolCtrl);
			valueTmp1 = valueTmp;
			for (m = 1U; m < inProtocolCtrl->missToleration; m++)
			{
				valueTmp = valueTmp1;
				for (n = 1U; n < m; n++)
				{
					Rsp1_Load(chanel, valueTmp, inProtocolCtrl);
					Rsp1_Add(chanel, 0U, inProtocolCtrl);
					valueTmp = Rsp1_Read(chanel, inProtocolCtrl);
				}

				Rsp1_Load(chanel, valueTmp, inProtocolCtrl);
				Rsp1_Add(chanel, strPeerNodePara->sid[chanel], inProtocolCtrl);

				inProtocolCtrl->pTimeStampChk[chanel][m] = Rsp1_Read(chanel,
					inProtocolCtrl);

				Rsp1_Load(chanel, valueTmp, inProtocolCtrl);
				Rsp1_Post(chanel, strPeerNodePara->sinit[chanel], inProtocolCtrl);
				valueTmp = Rsp1_Read(chanel, inProtocolCtrl);

				inProtocolCtrl->pPrecSinit[chanel][m] = valueTmp ^
					strPeerNodePara->sid[chanel];
			}
		}
	}
}
/** ************************************************************
�����ʼSINIT
************************************************************* */
static void Rsp1_Calculate_PREC_FIRSTSINIT(UTiny chanel, S_ChnControl *chnCtrl,
	S_ProtocolControl *protocolCtrl)
{
	ULong tmpValue = 0;
	ULong srcId, SINIT, DATAVER;
	S_NodeParam* strLocalNodePara = NULL;
	S_NodeParam* strPeerNodePara = NULL;

	strLocalNodePara = &protocolCtrl->nodeParamSelf;
	strPeerNodePara = &protocolCtrl->nodeParamPeer;

	if (strLocalNodePara == NULL || strPeerNodePara == NULL) {
		Rsp1_SetErrorCode(protocolCtrl->devId, c_ERR_NO_DEV);
	}
	else {
		srcId = strLocalNodePara->sid[chanel];
		SINIT = strPeerNodePara->sinit[chanel];
		DATAVER = strPeerNodePara->dataver[chanel];

		Rsp1_Load(chanel, SINIT, protocolCtrl);
		Rsp1_Add(chanel, srcId ^ DATAVER, protocolCtrl);
		tmpValue = Rsp1_Read(chanel, protocolCtrl);
		Rsp1_Load(chanel, 0, protocolCtrl);
		Rsp1_Post(chanel, tmpValue, protocolCtrl);
		chnCtrl->precFirstSinit[chanel] = Rsp1_Read(chanel, protocolCtrl);
	}
}
/****************************************************************
*Name: Rsp1_GenerateSINIT
*Function:����SINIT
*ͨ�����¹�ʽ���㵱ǰSINIT��
*��ǰSINIT = ��ʼSINIT^SEQ^TE
*para:ͨ��ʶ��ţ���Ҫ������ͨ������Ҫ��������·
*return:SINITֵ
*Create by ChenX 2016/01/01
****************************************************************/
static ULong Rsp1_GenerateSINIT(const UTiny inChanel,
	const S_ChnControl *inChnCtrl,
	S_ProtocolControl *inProtocolCtrl)
{
	ULong sinit_U32 = 0UL;
	ULong firstInit_U32 = 0UL;
	ULong seq_U32 = 0UL;
	ULong tEmit_U32 = 0UL;

	firstInit_U32 = inChnCtrl->precFirstSinit[inChanel];
	seq_U32 = inChnCtrl->ssrInfoRx.seqInit[inChanel];
	tEmit_U32 = inChnCtrl->onSendSSE[inChanel];

	Rsp1_Load(inChanel, firstInit_U32, inProtocolCtrl);
	Rsp1_Add(inChanel, (seq_U32 ^ tEmit_U32), inProtocolCtrl);
	sinit_U32 = Rsp1_Read(inChanel, inProtocolCtrl);

	return sinit_U32;
}

/****************************************************************
*Name:  Rsp1_FormSSEMessage
*Function:SSE��֡����SSE��ʽ���SSE֡
*para:SSE��Ϣ֡ͷ����Ҫ������ͨ��
*return:��
*Create by ChenX 2016/01/01
****************************************************************/
static void Rsp1_FormSSEMessage(const S_MsgTypeInfo* inMesgInfo,
	S_ChnControl *inChnCtrl)
{
	UTiny pbuf[c_SSE_SIZE];
	UShort index = 0U;

	pbuf[index] = inMesgInfo->applCat;
	index++;
	pbuf[index] = c_MSG_SSE;
	index++;
	Rsp1_UShort2Buffer(&pbuf[index], inMesgInfo->srcAddress);
	index += 2U;
	Rsp1_UShort2Buffer(&pbuf[index], inMesgInfo->dstAddress);
	index += 2U;
	Rsp1_ULong2Buffer(&pbuf[index], inChnCtrl->timeStampMgr.tc);
	index += 4U;
	Rsp1_ULong2Buffer(&pbuf[index], inChnCtrl->sseInfoTx.seq[0]);
	index += 4U;
	Rsp1_ULong2Buffer(&pbuf[index], inChnCtrl->sseInfoTx.seq[1]);
	index += 4U;
	Rsp1_UShort2Buffer(&pbuf[index], Rsp1_ComputeMesgCRC(pbuf, c_SSE_SIZE - 2U));
	F_GL_MemCpy(inChnCtrl->pSendBuf, pbuf, c_SSE_SIZE);
}

/****************************************************************
*Name:  Rsp1_SendSSE
*Function:����SSE���ݰ������SSE֡�����±���ʱ�������¼����SSE֡ʱ�ı�������ʱ��ֵ
*para:��Ҫ��������·����Ҫ������ͨ��
*return:��
*Create by ChenX 2016/01/01
****************************************************************/
static void Rsp1_SendSSE(const S_ProtocolControl *inProtocolCtrl,
	S_ChnControl *inChnCtrl)
{
	inChnCtrl->sseInfoTx.seq[0] = (inProtocolCtrl->nodeParamSelf.sid[0]) ^
		(inChnCtrl->timeStampMgr.tNow[0]);
	inChnCtrl->sseInfoTx.seq[1] = (inProtocolCtrl->nodeParamSelf.sid[1]) ^
		(inChnCtrl->timeStampMgr.tNow[1]);

	Rsp1_FormSSEMessage(&inProtocolCtrl->msgInfoSse, inChnCtrl);

	inChnCtrl->byteInSendBuf = c_SSE_SIZE;
	inChnCtrl->onSendSSE[0] = inChnCtrl->timeStampMgr.tNow[0];
	inChnCtrl->onSendSSE[1] = inChnCtrl->timeStampMgr.tNow[1];
	inChnCtrl->tcLast = inChnCtrl->timeStampMgr.tc;
	inChnCtrl->sseSendTimer = s_RsspClock;
}

/****************************************************************
*Name:  LEU_SendSSE
*Function:����SSE���ݰ������SSE֡�����±���ʱ�������¼����SSE֡ʱ�ı�������ʱ��ֵ
*para:��Ҫ��������·����Ҫ������ͨ��
*return:��
*Create by ChenX 2016/01/01
****************************************************************/
static void LEU_SendSSE(const S_BtmProtocolControl *inProtocolCtrl,
	S_ChnControl *inChnCtrl)
{
	S_MsgTypeInfo* strSSEMsg = NULL;
	inChnCtrl->sseInfoTx.seq[0] = (inProtocolCtrl->protocolCtrl.nodeParamSelf.sid[0]) ^
		(inChnCtrl->timeStampMgr.tNow[0]);
	inChnCtrl->sseInfoTx.seq[1] = (inProtocolCtrl->protocolCtrl.nodeParamSelf.sid[1]) ^
		(inChnCtrl->timeStampMgr.tNow[1]);

	LEU_FormSSEMessage(inChnCtrl);

	inChnCtrl->byteInSendBuf = c_SSE_SIZE;
	inChnCtrl->onSendSSE[0] = inChnCtrl->timeStampMgr.tNow[0];
	inChnCtrl->onSendSSE[1] = inChnCtrl->timeStampMgr.tNow[1];
	inChnCtrl->tcLast = inChnCtrl->timeStampMgr.tc;
	inChnCtrl->sseSendTimer = s_RsspClock;
}
/****************************************************************
*Name:  Rsp1_CalculateSEQINI
*Function:����ʱ���ʼ��ͨ��ֵ
*para:Ӧ�����ͣ���Ҫ��������·����Ҫ������ͨ��
*return:��
*Create by ChenX 2016/01/01
****************************************************************/
static void Rsp1_CalculateSEQINI(const UTiny inApplCat,
	const S_ProtocolControl *inProtocolCtrl,
	S_ChnControl *inChnCtrl)
{
	ULong srcId_U32 = 0UL;
	ULong dataver_U32 = 0UL;
	ULong tRecvNum_U32 = 0UL;
	UTiny chanel = 0U;

	for (chanel = 0U; chanel < c_CHANEL_NUMBER; chanel++)
	{
		if ((c_APPLCAT_MAINCTL == inApplCat) || (c_APPLCAT_STANDBY == inApplCat) || (c_APPLCAT_BTM == inApplCat))
		{
			srcId_U32 = inProtocolCtrl->nodeParamSelf.sid[chanel];
			dataver_U32 = inProtocolCtrl->nodeParamSelf.dataver[chanel];
			tRecvNum_U32 = inChnCtrl->timeStampMgr.tNow[chanel];
			inChnCtrl->ssrInfoTx.seqInit[chanel] =
				(inChnCtrl->sseInfoRx.seq[chanel]) ^ (srcId_U32) ^
				(dataver_U32) ^ (tRecvNum_U32);
		}
	}
}

/****************************************************************
*Name: Rsp1_FormSSRMessage
*Function:��װSSR֡����Э���ʽ��װSSR֡
*para:SSRӦ����Ϣͷ����Ҫ��������·����Ҫ������ͨ��
*return:��
*Create by ChenX 2016/01/01
****************************************************************/
static void Rsp1_FormSSRMessage(const S_MsgTypeInfo* inMesgInfo,
	const S_ProtocolControl *inProtocolCtrl,
	S_ChnControl *inChnCtrl)
{
	UTiny pbuf[c_SSR_SIZE];
	UShort index = 0U;
	UShort crcMsg_U16 = 0U;

	pbuf[index] = inMesgInfo->applCat;
	index++;
	pbuf[index] = c_MSG_SSR;
	index++;
	Rsp1_UShort2Buffer(&pbuf[index], inMesgInfo->srcAddress);
	index += 2U;
	Rsp1_UShort2Buffer(&pbuf[index], inMesgInfo->dstAddress);
	index += 2U;

	if ((c_APPLCAT_MAINCTL == inMesgInfo->applCat) || (c_APPLCAT_STANDBY == inMesgInfo->applCat))
	{
		Rsp1_ULong2Buffer(&pbuf[index], inChnCtrl->timeStampMgr.tc);
		index += 4U;
		Rsp1_ULong2Buffer(&pbuf[index], inChnCtrl->sseInfoRx.tc);
		index += 4U;
		Rsp1_ULong2Buffer(&pbuf[index], inChnCtrl->ssrInfoTx.seqInit[0]);
		index += 4U;
		Rsp1_ULong2Buffer(&pbuf[index], inChnCtrl->ssrInfoTx.seqInit[1]);
		index += 4U;
		pbuf[index] = inProtocolCtrl->nodeParamSelf.numDataVer;
		index++;

		crcMsg_U16 = Rsp1_ComputeMesgCRC(pbuf, c_SSR_SIZE - 2U);
		Rsp1_UShort2Buffer(&pbuf[index], crcMsg_U16);

		F_GL_MemCpy(inChnCtrl->pSendBuf, pbuf, c_SSR_SIZE);
	}
}

/****************************************************************
*Name:  Rsp1_FormSafetyDataFrame
*Function:��װʵʱ����֡
*para:Ӧ����Ϣͷ���ڵ���Ϣ��ʵʱ���ݻ��壬���ݻ����С����Ҫ������ͨ��
*return:��
*Create by ChenX 2016/01/01
****************************************************************/
static void Rsp1_FormSafetyDataFrame(const S_MsgTypeInfo* inMesgInfo,
	const S_NodeParam* inNodeParam,
	UTiny* inDataField,
	UShort inDataFieldSize,
	S_ChnControl *inChnCtrl)
{
	UTiny pbuf[c_BUFF_SIZE];
	UShort index = 0U;/*init*/
	ULong crc1_U32 = 0UL;/*init*/
	ULong crc2_U32 = 0UL;/*init*/
	ULong crcM_U32 = 0UL;/*init*/
	UShort mesgCRC_U16 = 0U;/*init*/
	S_TimeStampMgr* pTSMgr;

	F_GL_MemSet(pbuf, 0, c_BUFF_SIZE);
	pTSMgr = &(inChnCtrl->timeStampMgr);
	if ((inDataFieldSize > 0U) && (inDataFieldSize < (c_BUFF_SIZE - 22U)))
	{
		if ((c_APPLCAT_MAINCTL == inMesgInfo->applCat) || (c_APPLCAT_STANDBY == inMesgInfo->applCat) || (c_APPLCAT_BTM == inMesgInfo->applCat))
		{
			pTSMgr = &(inChnCtrl->timeStampMgr);

			pbuf[index] = inMesgInfo->applCat;
			index++;
			pbuf[index] = inMesgInfo->msgType;
			index++;
			/*���Դ��ַ*/
			Rsp1_UShort2Buffer(&pbuf[index], inMesgInfo->srcAddress);
			index += 2U;
			/*���Ŀ�ĵ�ַ*/
			Rsp1_UShort2Buffer(&pbuf[index], inMesgInfo->dstAddress);
			index += 2U;
			/*�������ֵ*/
			Rsp1_ULong2Buffer(&pbuf[index], pTSMgr->tc);
			index += 4U;
			/*������ݳ���*/
			Rsp1_UShort2Buffer(&pbuf[index], inDataFieldSize + 8U);
			index += 2U;
		}
		/*����CRC32*/
		crc1_U32 = Rsp1_SafetyCRCCompute(inDataField, inDataFieldSize, 0U);
		crc2_U32 = Rsp1_SafetyCRCCompute(inDataField, inDataFieldSize, 1U);
		/*����˫ͨ��У��ֵ*/
		crcM_U32 = crc1_U32 ^ (inNodeParam->sid[0]) ^ (pTSMgr->tNow[0]) ^ c_SYSCKW_1;
		Rsp1_ULong2Buffer(&pbuf[index], crcM_U32);
		index += 4U;
		/*����˫ͨ��У��ֵ*/
		crcM_U32 = crc2_U32 ^ (inNodeParam->sid[1]) ^ (pTSMgr->tNow[1]) ^ c_SYSCKW_2;
		Rsp1_ULong2Buffer(&pbuf[index], crcM_U32);
		index += 4U;

		/*��䴫������*/
		F_GL_MemCpy(&pbuf[index], inDataField, inDataFieldSize);
		index += inDataFieldSize;

		/*����RSSP1���CRC16*/
		mesgCRC_U16 = Rsp1_ComputeMesgCRC(pbuf, inDataFieldSize + 20U);
		Rsp1_UShort2Buffer(&pbuf[index], mesgCRC_U16);
		F_GL_MemCpy(inChnCtrl->pSendBuf, pbuf, c_BUFF_SIZE);
	}
}



/****************************************************************
*Name:  Rsp1_LEU_FormSafetyDataFrame
*Function:��װʵʱ����֡
*para:Ӧ����Ϣͷ���ڵ���Ϣ��ʵʱ���ݻ��壬���ݻ����С����Ҫ������ͨ��
*return:��
*Create by ChenX 2016/01/01
****************************************************************/
static void Rsp1_LEU_FormSafetyDataFrame(const S_MsgTypeInfo* inMesgInfo,
	const S_NodeParam* inNodeParam,
	UTiny* inDataField,
	UShort inDataFieldSize,
	S_ChnControl *inChnCtrl)
{
	UTiny pbuf[c_BUFF_SIZE];
	UShort index = 0U;/*init*/
	ULong crc1_U32 = 0UL;/*init*/
	ULong crc2_U32 = 0UL;/*init*/
	ULong crcM_U32 = 0UL;/*init*/
	UShort mesgCRC_U16 = 0U;/*init*/
	S_TimeStampMgr* pTSMgr;

	F_GL_MemSet(pbuf, 0, c_BUFF_SIZE);
	pTSMgr = &(inChnCtrl->timeStampMgr);
	if ((inDataFieldSize > 0U) && (inDataFieldSize < (c_BUFF_SIZE - 22U)))
	{
		if ((c_APPLCAT_MAINCTL == inMesgInfo->applCat) || (c_APPLCAT_STANDBY == inMesgInfo->applCat) || (c_APPLCAT_BTM == inMesgInfo->applCat))
		{
			pTSMgr = &(inChnCtrl->timeStampMgr);

			pbuf[index] = inMesgInfo->applCat;
			index++;
			pbuf[index] = inMesgInfo->msgType;
			index++;
			/*���Դ��ַ*/
			Rsp1_UShort2Buffer(&pbuf[index], inMesgInfo->srcAddress);
			index += 2U;
			/*���Ŀ�ĵ�ַ*/
			Rsp1_UShort2Buffer(&pbuf[index], 0xffff);
			index += 2U;
			/*�������ֵ*/
			Rsp1_ULong2Buffer(&pbuf[index], pTSMgr->tc);
			index += 4U;
			/*������ݳ���*/
			Rsp1_UShort2Buffer(&pbuf[index], inDataFieldSize + 8U);
			index += 2U;
		}
		/*����CRC32*/
		crc1_U32 = Rsp1_SafetyCRCCompute(inDataField, inDataFieldSize, 0U);
		crc2_U32 = Rsp1_SafetyCRCCompute(inDataField, inDataFieldSize, 1U);
		/*����˫ͨ��У��ֵ*/
		crcM_U32 = crc1_U32 ^ (inNodeParam->sid[0]) ^ (pTSMgr->tNow[0]) ^ c_SYSCKW_1;
		Rsp1_ULong2Buffer(&pbuf[index], crcM_U32);
		index += 4U;
		/*����˫ͨ��У��ֵ*/
		crcM_U32 = crc2_U32 ^ (inNodeParam->sid[1]) ^ (pTSMgr->tNow[1]) ^ c_SYSCKW_2;
		Rsp1_ULong2Buffer(&pbuf[index], crcM_U32);
		index += 4U;

		/*��䴫������*/
		F_GL_MemCpy(&pbuf[index], inDataField, inDataFieldSize);
		index += inDataFieldSize;

		/*����RSSP1���CRC16*/
		mesgCRC_U16 = Rsp1_ComputeMesgCRC(pbuf, inDataFieldSize + 20U);
		Rsp1_UShort2Buffer(&pbuf[index], mesgCRC_U16);
		F_GL_MemCpy(inChnCtrl->pSendBuf, pbuf, c_BUFF_SIZE);
	}
}

/****************************************************************
*Name:  Rsp1_UpdateTelegram
*Function:��������Ƿ�ʱ CRC�Ƿ�һ��
*para:��Ҫ��������·����Ҫ������ͨ�������뻺��������������С��CRCֵ
*return:�棬���³ɹ����٣�����ʧ��
*Create by ChenX 2016/01/01
****************************************************************/
static E_Bool Rsp1_UpdateTelegram(const S_ProtocolControl *inProtocolCtrl,
	const S_ChnControl *inChnCtrl,
	const UTiny *inBuf,
	const UShort inLen,
	const UShort inCrc_U16)
{
	E_Bool bCRCCheck = r_FALSE;
	UShort iCrc_U16 = 0U;


	if ((inProtocolCtrl != rNULL) && (inChnCtrl != rNULL))
	{
		iCrc_U16 = Rsp1_ComputeMesgCRC(inBuf, inLen);

		if (iCrc_U16 == inCrc_U16)
		{
			bCRCCheck = r_TRUE;
		}
		else
		{
			bCRCCheck = r_FALSE;
		}
	}
	return bCRCCheck;
}

/****************************************************************
*Name: Rsp1_SendRSD
*Function:��װ����֡������ʱ���
*para:��Ҫ��������·����Ҫ������ͨ�������뻺��������������С��CRCֵ
*return:��
*Create by ChenX 2016/01/01
****************************************************************/
static void Rsp1_SendRSD(S_ProtocolControl *inProtocolCtrl,
	S_ChnControl *inChnCtrl,
	UTiny *inBuf,
	UShort inLen_U16,
	UShort inCrc_U16)
{
	S_NodeParam* strLocalNodePara = rNULL;/*init*/
	S_MsgTypeInfo* strMsgType = rNULL;/*init*/
	UTiny offset_U8 = 0U;/*init*/
	UTiny sendto_U8 = 0U;/*init*/

	strLocalNodePara = &inProtocolCtrl->nodeParamSelf;
	sendto_U8 = 0U;
	if (r_TRUE == inProtocolCtrl->safeCtrl[0].sendCtrl.sendTsd)
	{
		sendto_U8 |= 0x01U;
	}
	if (r_TRUE == inProtocolCtrl->safeCtrl[1].sendCtrl.sendTsd)
	{
		sendto_U8 |= 0x02U;
	}

	if (0U == sendto_U8)
	{
		Rsp1_AddTimeStamp(inProtocolCtrl, &(inProtocolCtrl->safeCtrl[0].timeStampMgr));/*ʱ�������*/
		Rsp1_AddTimeStamp(inProtocolCtrl, &(inProtocolCtrl->safeCtrl[1].timeStampMgr));/*ʱ�������*/
	}
	else
	{
		strMsgType = &inProtocolCtrl->msgInfoA;

		F_GL_MemCpy(inChnCtrl->sendInfoBuf, inBuf, inLen_U16 - offset_U8);

		if (Rsp1_UpdateTelegram(inProtocolCtrl, inChnCtrl, inChnCtrl->sendInfoBuf,
			inLen_U16 - offset_U8, inCrc_U16) == r_TRUE)
		{
			Rsp1_FormSafetyDataFrame(strMsgType, strLocalNodePara,
				inChnCtrl->sendInfoBuf, inLen_U16 - offset_U8, inChnCtrl);/*������ȫ����֡*/
			inChnCtrl->byteInSendBuf = (inLen_U16 - offset_U8) + 22U;
		}

		Rsp1_AddTimeStamp(inProtocolCtrl, &(inProtocolCtrl->safeCtrl[0].timeStampMgr));/*ʱ����ۼ�*/
		Rsp1_AddTimeStamp(inProtocolCtrl, &(inProtocolCtrl->safeCtrl[1].timeStampMgr));/*ʱ����ۼ�*/
	}
}


/****************************************************************
*Name: Rsp1_LEU_SendRSD
*Function:��װ����֡������ʱ���
*para:��Ҫ��������·����Ҫ������ͨ�������뻺��������������С��CRCֵ
*return:��
*Create by ChenX 2016/01/01
****************************************************************/
static void Rsp1_LEU_SendRSD(S_ProtocolControl *inProtocolCtrl,
	S_ChnControl *inChnCtrl,
	UTiny *inBuf,
	UShort inLen_U16,
	UShort inCrc_U16)
{
	S_NodeParam* strLocalNodePara = rNULL;/*init*/
	S_MsgTypeInfo* strMsgType = rNULL;/*init*/
	UTiny offset_U8 = 0U;/*init*/
	UTiny sendto_U8 = 0U;/*init*/

	strLocalNodePara = &inProtocolCtrl->nodeParamSelf;
	sendto_U8 = 0U;
	if (r_TRUE == inProtocolCtrl->safeCtrl[0].sendCtrl.sendTsd)
	{
		sendto_U8 |= 0x01U;
	}
	if (r_TRUE == inProtocolCtrl->safeCtrl[1].sendCtrl.sendTsd)
	{
		sendto_U8 |= 0x02U;
	}

	if (0U == sendto_U8)
	{
		Rsp1_AddTimeStamp(inProtocolCtrl, &(inProtocolCtrl->safeCtrl[0].timeStampMgr));/*ʱ�������*/
		Rsp1_AddTimeStamp(inProtocolCtrl, &(inProtocolCtrl->safeCtrl[1].timeStampMgr));/*ʱ�������*/
	}
	else
	{
		strMsgType = &inProtocolCtrl->msgInfoA;

		F_GL_MemCpy(inChnCtrl->sendInfoBuf, inBuf, inLen_U16 - offset_U8);

		if (Rsp1_UpdateTelegram(inProtocolCtrl, inChnCtrl, inChnCtrl->sendInfoBuf,
			inLen_U16 - offset_U8, inCrc_U16) == r_TRUE)
		{
			Rsp1_LEU_FormSafetyDataFrame(strMsgType, strLocalNodePara,
				inChnCtrl->sendInfoBuf, inLen_U16 - offset_U8, inChnCtrl);/*������ȫ����֡*/
			inChnCtrl->byteInSendBuf = (inLen_U16 - offset_U8) + 22U;
		}

		Rsp1_AddTimeStamp(inProtocolCtrl, &(inProtocolCtrl->safeCtrl[0].timeStampMgr));/*ʱ����ۼ�*/
		Rsp1_AddTimeStamp(inProtocolCtrl, &(inProtocolCtrl->safeCtrl[1].timeStampMgr));/*ʱ����ۼ�*/
	}
}



/****************************************************************
*Name: Rsp1_SendRSD
*Function:��װ����֡������ʱ���
*para:��Ҫ��������·����Ҫ������ͨ�������뻺��������������С��CRCֵ
*return:��
*Create by ChenX 2016/01/01
****************************************************************/
static void LEU_SendBTM(const UShort inLeuIndex)
{
	printf("status:%d\n",s_BtmPrtclCtrlList[inLeuIndex].btm_status);
	switch (s_BtmPrtclCtrlList[inLeuIndex].btm_status)
	{
	case e_init:
		s_BtmPrtclCtrlList[inLeuIndex].btm_status = e_start;
		Rsp1_FormSBTMessage(&s_BtmPrtclCtrlList[inLeuIndex].protocolCtrl.nodeParamSelf, &s_BtmPrtclCtrlList[inLeuIndex].protocolCtrl.safeCtrl[0]);
		break;
	case e_initover:
		s_BtmPrtclCtrlList[inLeuIndex].btm_status = e_contrl;
		Rsp1_FormBTCMessage(&s_BtmPrtclCtrlList[inLeuIndex].protocolCtrl.nodeParamSelf, &s_BtmPrtclCtrlList[inLeuIndex].protocolCtrl.safeCtrl[0]);
		break;
	case e_contrl:
		Rsp1_FormBTCMessage(&s_BtmPrtclCtrlList[inLeuIndex].protocolCtrl.nodeParamSelf, &s_BtmPrtclCtrlList[inLeuIndex].protocolCtrl.safeCtrl[0]);
		break;
	case e_transfer:
		s_BtmPrtclCtrlList[inLeuIndex].btm_status = e_transferover;
		Rsp1_FormABTMessage(&s_BtmPrtclCtrlList[inLeuIndex].protocolCtrl.nodeParamSelf, &s_BtmPrtclCtrlList[inLeuIndex].protocolCtrl.safeCtrl[0]);
		break;
	case e_transferover:
		s_BtmPrtclCtrlList[inLeuIndex].btm_status = e_end;
		Rsp1_FormEBTMessage(&s_BtmPrtclCtrlList[inLeuIndex].protocolCtrl.nodeParamSelf, &s_BtmPrtclCtrlList[inLeuIndex].protocolCtrl.safeCtrl[0]);
		break;
	default:
		break;
	}
	if (s_BtmPrtclCtrlList[inLeuIndex].protocolCtrl.safeCtrl[0].byteInSendBuf >0)
	{
		Rsp1_AddTimeStamp(&s_BtmPrtclCtrlList[inLeuIndex].protocolCtrl,
			&(s_BtmPrtclCtrlList[inLeuIndex].protocolCtrl.safeCtrl[0].timeStampMgr)); /* ʱ����ۼ� */
		Rsp1_AddTimeStamp(&s_BtmPrtclCtrlList[inLeuIndex].protocolCtrl,
			&(s_BtmPrtclCtrlList[inLeuIndex].protocolCtrl.safeCtrl[1].timeStampMgr)); /* ʱ����ۼ� */
	}

}

/****************************************************************
*Name: Rsp1_CalculateFirstSinit
*Function:�����ʼSINIT
*para:ͨ���ţ���Ҫ������ͨ������Ҫ��������·
*return:��
*Create by ChenX 2016/01/01
****************************************************************/
static void Rsp1_CalculateFirstSinit(UTiny inChanel,
	S_ChnControl *inChnCtrl,
	S_ProtocolControl *inProtocolCtrl)
{
	ULong tmpValue = 0UL;
	ULong srcId = 0UL;
	ULong sinit_U32 = 0UL;
	ULong dataVer_U32 = 0UL;
	S_NodeParam* strLocalNodePara = rNULL;
	S_NodeParam* strPeerNodePara = rNULL;

	if ((inChnCtrl != rNULL) && (inProtocolCtrl != rNULL))
	{
		strLocalNodePara = &inProtocolCtrl->nodeParamSelf;
		strPeerNodePara = &inProtocolCtrl->nodeParamPeer;

		srcId = strLocalNodePara->sid[inChanel];
		sinit_U32 = strPeerNodePara->sinit[inChanel];
		dataVer_U32 = strPeerNodePara->dataver[inChanel];

		Rsp1_Load(inChanel, sinit_U32, inProtocolCtrl);
		Rsp1_Add(inChanel, srcId ^ dataVer_U32, inProtocolCtrl);
		tmpValue = Rsp1_Read(inChanel, inProtocolCtrl);
		Rsp1_Load(inChanel, 0U, inProtocolCtrl);
		Rsp1_Post(inChanel, tmpValue, inProtocolCtrl);
		inChnCtrl->precFirstSinit[inChanel] = Rsp1_Read(inChanel, inProtocolCtrl);

	}
}

/****************************************************************
*Name: Rsp1_SendSSR
*Function:��װSSR֡������ʱ���
*para:��Ҫ��������·����Ҫ������ͨ��
*return:��
*Create by ChenX 2016/01/01
****************************************************************/
static void Rsp1_SendSSR(S_ProtocolControl *inProtocolCtrl,
	S_ChnControl *inChnCtrl)
{
	S_MsgTypeInfo* strSSRMsg = rNULL;

	if ((inProtocolCtrl != rNULL) &&  /* �жϲ�����·�Ƿ���Ч */
		(inChnCtrl != rNULL))
	{
		strSSRMsg = &inProtocolCtrl->msgInfoSsr;

		inChnCtrl->sendCtrl.sendSsr = r_TRUE; /* SSR���Ϳ�������λ */
		Rsp1_CalculateSEQINI(c_APPLCAT_MAINCTL, inProtocolCtrl, inChnCtrl);

		Rsp1_FormSSRMessage(strSSRMsg, inProtocolCtrl, inChnCtrl); /* SSR֡��֡ */
		inChnCtrl->byteInSendBuf = c_SSR_SIZE;
		Rsp1_AddTimeStamp(inProtocolCtrl,
			&(inProtocolCtrl->safeCtrl[0].timeStampMgr)); /* ʱ����ۼ� */
		Rsp1_AddTimeStamp(inProtocolCtrl,
			&(inProtocolCtrl->safeCtrl[1].timeStampMgr)); /* ʱ����ۼ� */
	}
}


/****************************************************************
*Name: LEU_SendSSR
*Function:��װSSR֡������ʱ���
*para:��Ҫ��������·����Ҫ������ͨ��
*return:��
*Create by ChenX 2016/01/01
****************************************************************/
static void LEU_SendSSR(S_BtmProtocolControl *inProtocolCtrl,
	S_ChnControl *inChnCtrl)
{
	S_MsgTypeInfo* strSSRMsg = rNULL;

	if ((inProtocolCtrl != rNULL) &&  /* �жϲ�����·�Ƿ���Ч */
		(inChnCtrl != rNULL))
	{
		inChnCtrl->sendCtrl.sendSsr = r_TRUE; /* SSR���Ϳ�������λ */
		Rsp1_CalculateSEQINI(c_APPLCAT_BTM, &inProtocolCtrl->protocolCtrl, inChnCtrl);

		LEU_FormSSRMessage(inChnCtrl); /* SSR֡��֡ */
		inChnCtrl->byteInSendBuf = c_SSR_SIZE;
		Rsp1_AddTimeStamp(&inProtocolCtrl->protocolCtrl,
			&(inProtocolCtrl->protocolCtrl.safeCtrl[0].timeStampMgr)); /* ʱ����ۼ� */
		Rsp1_AddTimeStamp(&inProtocolCtrl->protocolCtrl,
			&(inProtocolCtrl->protocolCtrl.safeCtrl[1].timeStampMgr)); /* ʱ����ۼ� */
	}
}

/****************************************************************
*Name: Rsp1_DiffTime
*Function:�������ڲ�
*para:ʱ������
*return:����ʱ������ֵ��Э��ջʱ������ֵ��ֵ
*Create by ChenX 2016/01/01
****************************************************************/
static UShort Rsp1_DiffTime(const UShort inTimer_U16)
{
	return s_RsspClock - inTimer_U16;
}

/****************************************************************
*Name: CheckTimeStamp
*Function:���ʱ���
*para:���ݻ��壬���������ȣ���Ҫ��������·����Ҫ������ͨ��
*return:0��ʧ�ܣ�1���ɹ���2���ظ�����֡
*Create by ChenX 2016/01/01
****************************************************************/
static E_TimeStampCheckResult Rsp1_CheckTimeStamp(UTiny* inDataBuf,
	UShort inDatafieldSize_U16,
	S_ProtocolControl *inProtocolCtrl,
	S_ChnControl *inChnCtrl)
{
	E_TimeStampCheckResult result = e_TsCheckErr;
	UTiny pbuf[c_BUFF_SIZE];
	ULong crcM_1_U32 = 0UL;
	ULong crcM_2_U32 = 0UL;
	ULong crc_1_U32 = 0UL;
	ULong crc_2_U32 = 0UL;
	ULong valuetmpL_U32 = 0UL;
	ULong valuetmpR_U32 = 0UL;
	ULong i = 0UL;
	UTiny chanel = 0U;
	ULong tempTC_U32 = 0UL;
	SLong deltaTC_U32 = 0L;
	ULong timeControl_U32 = 0UL;

	F_GL_MemCpy(pbuf, inDataBuf, c_BUFF_SIZE);
	tempTC_U32 = Rsp1_Buffer2ULong(&pbuf[6]); /* ��ȡ�Է�����֡�е�ʱ��� */
	deltaTC_U32 = (SLong)tempTC_U32 - (SLong)inChnCtrl->tcrNow; /* �������ڲ�ֵ  */
	if ((deltaTC_U32 >= (SLong)inProtocolCtrl->missToleration) ||
		(-deltaTC_U32 >= (SLong)(inProtocolCtrl->missToleration)) ||
		(Rsp1_DiffTime(inChnCtrl->linkoffTimeCntr) >= inProtocolCtrl->timeoutToleration))
	{
		result = e_TsCheckErr;   /* ʱ���ƫ�Ƴ���ʱ�����̷�Χ  */
	}
	else
	{
		if (deltaTC_U32 <= (SLong)0) /* �ж��Ƿ����ظ����ݰ� */
		{
			result = e_TsCheckRepeat; /* �ظ����ݰ� */
		}
		else
		{
			timeControl_U32 = (ULong)deltaTC_U32;
			inChnCtrl->tcrLast = inChnCtrl->tcrNow;
			inChnCtrl->tcrNow = tempTC_U32;
			/* ��ȡͨ��1CRCMֵ */
			crcM_1_U32 = pbuf[12] + (pbuf[13] * c_MULT_NUM) + (pbuf[14] * 65536UL) + (pbuf[15] * 16777216UL);
			/* ��ȡͨ��1CRCMֵ */
			crcM_2_U32 = pbuf[16] + (pbuf[17] * c_MULT_NUM) + (pbuf[18] * 65536UL) + (pbuf[19] * 16777216UL);

			crc_1_U32 = Rsp1_SafetyCRCCompute(&pbuf[20U], inDatafieldSize_U16, 0U);/* ����ͨ��1CRCֵ */
			crc_2_U32 = Rsp1_SafetyCRCCompute(&pbuf[20U], inDatafieldSize_U16, 1U);/* ����ͨ��2CRCֵ */

			inChnCtrl->nowSidXorTn[0] = crc_1_U32 ^ crcM_1_U32 ^ c_SYSCKW_1;/* ����У��ֵ */
			inChnCtrl->nowSidXorTn[1] = crc_2_U32 ^ crcM_2_U32 ^ c_SYSCKW_2;/* ����У��ֵ */

			/*ʱ���У��*/
			// check Time Stamp
			for (chanel = 0; chanel < 2; chanel++)
			{
				Rsp1_Load(chanel, inChnCtrl->lastSidXorTn[chanel], inProtocolCtrl);
				valuetmpL_U32 = Rsp1_Read(chanel, inProtocolCtrl);
				for (i = 1; i < deltaTC_U32; i++)
				{
					Rsp1_Load(chanel, valuetmpL_U32, inProtocolCtrl);
					Rsp1_Add(chanel, 0, inProtocolCtrl);
					valuetmpL_U32 = Rsp1_Read(chanel, inProtocolCtrl);
				}
				Rsp1_Load(chanel, valuetmpL_U32, inProtocolCtrl);
				Rsp1_Add(chanel, inChnCtrl->nowSidXorTn[chanel], inProtocolCtrl);
				valuetmpL_U32 = Rsp1_Read(chanel, inProtocolCtrl);


				valuetmpR_U32 = inProtocolCtrl->pTimeStampChk[chanel][deltaTC_U32];

				if (valuetmpR_U32 != valuetmpL_U32)
				{
					result = e_TsCheckErr;/*ʱ���������*/
					/* ����LastSID_XOR_Tn */
				}
				else
				{
					Rsp1_Load(chanel, inChnCtrl->lastSidXorTn[chanel], inProtocolCtrl);
					valuetmpL_U32 = Rsp1_Read(chanel, inProtocolCtrl);
					for (i = 0; i < (deltaTC_U32 + 1); i++)
					{
						Rsp1_Load(chanel, valuetmpL_U32, inProtocolCtrl);
						Rsp1_Add(chanel, 0, inProtocolCtrl);
						valuetmpL_U32 = Rsp1_Read(chanel, inProtocolCtrl);
					}

					Rsp1_Load(chanel, inProtocolCtrl->pPrecSinit[chanel][deltaTC_U32], inProtocolCtrl);
					Rsp1_Add(chanel, inChnCtrl->nowSidXorTn[chanel], inProtocolCtrl);
					Rsp1_Add(chanel, inChnCtrl->nowSidXorTn[chanel], inProtocolCtrl);
					valuetmpR_U32 = Rsp1_Read(chanel, inProtocolCtrl);

					inChnCtrl->lastSidXorTn[chanel] = valuetmpL_U32^valuetmpR_U32;
					result = e_TsCheckOk;
				}
			}
		}
	}
	return result; /* ʱ��������ȷ */
}

/****************************************************************
*Name: Rsp1_SetErrorCode
*Function:���ù��ϴ���
*para:ϵͳ�ţ�ģ��ţ����ܺţ��������
*return:��
*Create by ChenX 2016/01/01
****************************************************************/
static void Rsp1_SetErrorCode(UShort inFunId,
	UTiny inErrorId)
{
	s_Errcode[s_ErrNumber].systemId = inFunId;
	s_Errcode[s_ErrNumber].errorId = inErrorId;
	s_ErrNumber++;
}

/****************************************************************
*Name: Rsp1_ULong2Buffer
*Function:�޷��ų���������BUFFER
*para:���壬�޷��ų�������
*return:��
*Create by ChenX 2016/01/01
****************************************************************/
static void Rsp1_ULong2Buffer(UTiny* inPbuf,
	ULong outDword)
{
	UTiny tmpBuff[4];

	tmpBuff[0] = (UTiny)((outDword)& 0xFFU);
	tmpBuff[1] = (UTiny)((outDword >> 8) & 0xFFU);
	tmpBuff[2] = (UTiny)((outDword >> 16) & 0xFFU);
	tmpBuff[3] = (UTiny)((outDword >> 24) & 0xFFU);

	F_GL_MemCpy(inPbuf, tmpBuff, 4U);
}

/****************************************************************
*Name: Rsp1_UShort2Buffer
*Function:�޷��Ŷ���������BUFFER
*para:���壬�޷��Ŷ�������
*return:��
*Create by ChenX 2016/01/01
****************************************************************/
static void Rsp1_UShort2Buffer(UTiny* inPbuf,
	UShort outWord_U16)
{
	UTiny tmpBuff[2];

	tmpBuff[0] = (UTiny)((outWord_U16)& 0xFFU);
	tmpBuff[1] = (UTiny)((outWord_U16 >> 8) & 0xFFU);

	F_GL_MemCpy(inPbuf, tmpBuff, 2U);
}

/****************************************************************
*Name:  Rsp1_Buffer2ULong
*Function:BUFF����ULong
*para:����
*return:����������
*Create by ChenX 2016/01/01
****************************************************************/
static ULong Rsp1_Buffer2ULong(const UTiny * inBufferPtr)
{
	ULong result = 0UL;
	UTiny tmpBuff[4];

	F_GL_MemCpy(tmpBuff, inBufferPtr, 4U);
	result = (tmpBuff[3] * c_MULT_NUM*c_MULT_NUM*c_MULT_NUM) +
		(tmpBuff[2] * c_MULT_NUM*c_MULT_NUM) +
		(tmpBuff[1] * c_MULT_NUM) +
		tmpBuff[0];
	return result;
}

/****************************************************************
*Name:  Rsp1_Buffer2UShort
*Function:BUFF����UShort
*para:����
*return:����������
*Create by ChenX 2016/01/01
****************************************************************/
static UShort Rsp1_Buffer2UShort(const UTiny * inBufferPtr)
{
	UTiny tmpBuff[2];
	UShort result = 0U;

	F_GL_MemCpy(tmpBuff, inBufferPtr, 2U);
	result = (tmpBuff[1] * 256U) + tmpBuff[0];
	return result;
}

/****************************************************************
*Name: Rsp1_Load
*Function:����ʱ���
*para:ͨ���ţ�ʱ���ֵ����Ҫ��������·
*return:��
*Create by ChenX 2016/01/01
****************************************************************/
static void Rsp1_Load(const UTiny inChanel,
	const ULong inVal,
	S_ProtocolControl *inProtocolCtrl)
{
	inProtocolCtrl->pdreg[inChanel] = inVal;
}

/****************************************************************
*Name: Rsp1_Post
*Function:POST���ܣ�����ʱ�������
*para:ͨ���ţ�ʱ���ֵ����Ҫ��������·
*return:��
*Create by ChenX 2016/01/01
****************************************************************/
static void Rsp1_Post(const UTiny inChanel,
	const ULong inVal,
	S_ProtocolControl *inProtocolCtrl)
{
	ULong tmpValue = 0UL;

	tmpValue = inProtocolCtrl->pdreg[inChanel];
	inProtocolCtrl->pdreg[inChanel] = inVal;
	Rsp1_ShiftRight(inChanel, 32U, inProtocolCtrl);
	inProtocolCtrl->pdreg[inChanel] ^= tmpValue;
}

/****************************************************************
*Name: Rsp1_Read
*Function:��ȡʱ���
*para:ͨ���ţ���Ҫ��������·
*return:��
*Create by ChenX 2016/01/01
****************************************************************/
static ULong Rsp1_Read(const UTiny inChanel,
	const S_ProtocolControl *inProtocolCtrl)
{
	return inProtocolCtrl->pdreg[inChanel];
}

/****************************************************************
*Name: Rsp1_Add
*Function:ʱ�������
*para:ͨ���ţ�ʱ���ֵ����Ҫ��������·
*return:�������ʱ���ֵ
*Create by ChenX 2016/01/01
****************************************************************/
static ULong Rsp1_Add(const UTiny inChanel,
	const ULong inVal,
	S_ProtocolControl *inProtocolCtrl)
{
	inProtocolCtrl->pdreg[inChanel] ^= inVal;
	Rsp1_ShiftLeft(inChanel, 32U, inProtocolCtrl);

	return(inProtocolCtrl->pdreg[inChanel]);
}

/****************************************************************
*Name:  Rsp1_ShiftLeft
*Function:ʱ�������
*para:ͨ���ţ�����λ������Ҫ��������·��
*return:��
*Create by ChenX 2016/01/01
****************************************************************/
static void Rsp1_ShiftLeft(UTiny inChanel,
	UTiny inShift,
	S_ProtocolControl *inProtocolCtrl)
{
	UTiny i = 0U;
	UTiny carry = 0U;
	ULong valTmp = 0UL;

	valTmp = inProtocolCtrl->pdreg[inChanel];

	for (i = 0U; i < inShift; i++) /*nshift*/
	{
		if ((valTmp & 0x80000000UL) != 0UL)
		{
			carry = 1U;/*��BITΪ1*/
		}
		else
		{
			carry = 0U;/*��BITΪ0*/
		}
		valTmp = valTmp << 1;
		if (1U == carry)
		{
			valTmp ^= c_MASK_LFSR[inChanel];
		}
	}
	inProtocolCtrl->pdreg[inChanel] = valTmp;
}

/****************************************************************
*Name: Rsp1_ShiftRight
*Function:ʱ�������
*para:ͨ���ţ�����λ������Ҫ��������·��
*return:��
*Create by ChenX 2016/01/01
****************************************************************/
static void Rsp1_ShiftRight(UTiny inChanel,
	UTiny inShift,
	S_ProtocolControl *inProtocolCtrl)
{
	UTiny i = 0U;
	UTiny carry = 0U;
	ULong valTmp = 0UL;

	valTmp = inProtocolCtrl->pdreg[inChanel];/*ѡ��ͨ����*/

	for (i = 0U; i < inShift; i++)
	{
		carry = (UTiny)(valTmp & 1U);
		if (carry > 0U)
		{
			valTmp ^= c_MASK_LFSR[inChanel];/*���ϵͳ������*/
		}
		valTmp >>= 1;
		if (carry > 0U)
		{
			valTmp |= 0x80000000UL;/*��BITΪ1*/
		}
		else
		{
			valTmp |= 0x00000000UL;/*��BITΪ0*/
		}
	}

	inProtocolCtrl->pdreg[inChanel] = valTmp;
}

/****************************************************************
*Name: Rsp1_GetToleration
*Function:��ȡ��Ӧͨ��˫��������ʱ�������̰�
*para:Դ�豸ʶ��ţ�Ŀ���豸ʶ��ţ��������̣���ʱ����
*return:0������ʧ�ܣ�1�����ҳɹ�
*Create by ChenX 2016/01/01
****************************************************************/
static E_Bool Rsp1_GetToleration(const UShort inDesDevId_U16,
	UTiny *outMissToleration,
	UTiny *outTimeoutToleration)
{
	E_Bool result = r_FALSE;
	UTiny desType = 0U;
	UTiny i = 0U;

	desType = (UTiny)((inDesDevId_U16 & 0xFF00U) >> 8U);/*��ȡ�豸����*/

	for (i = 0U; i < c_MAX_DEVTYPE_NUMBER; i++)
	{
		if (s_ComTolerationList[i].commType == desType)/*���ҵ���Ӧ���豸*/
		{
			*outMissToleration = s_ComTolerationList[i].missToleration;/*���ҵ���Ӧ���豸�Ķ�������*/
			*outTimeoutToleration = s_ComTolerationList[i].timeoutToleration;/*���ҵ���Ӧ���豸�ĳ�ʱ����*/
			result = r_TRUE;
			break;
		}
		else
		{
			/*���Ҳ�����Ӧ���豸*/
			*outMissToleration = 0U;/*�������̸�ֵΪ0*/
			*outTimeoutToleration = 0U;/*��ʱ���̸�ֵΪ0*/
			result = r_FALSE;
		}
	}
	return result;
}
/****************************************************************
*Name:  Rsp1_GetErrCode
*Function:��ȡ������
*para:outBuf,���������ݻ���
*return:���������ݳ���
*Create by ChenX 2016/09/26
****************************************************************/
UShort Rsp1_GetRunningCode(UTiny *outBuf)
{
	UShort i = 0U;
	UTiny tmpBuff[c_BYTE_SIZE];
	UShort index = 0U;

	F_GL_MemSet(tmpBuff, 0, (UShort)sizeof(c_BYTE_SIZE));
	for (i = 0U; i <s_ErrNumber; i++)
	{
		tmpBuff[index] = (UTiny)((s_Errcode[i].systemId & 0xFF00U) >> 8U);
		index++;
		tmpBuff[index] = (UTiny)(s_Errcode[i].systemId & 0x00FFU);
		index++;
		tmpBuff[index] = s_Errcode[i].errorId;
		index++;
	}
	F_GL_MemCpy(outBuf, tmpBuff, index);
	return index;
}


/****************************************************************
*Name: Rsp1_FormSBTMessage
*Function:���SBT֡
*para:��
*return:��
*Create by ChenX 2017/07/05
****************************************************************/
static void Rsp1_FormSBTMessage(const S_NodeParam* inNodeParam, S_ChnControl *inChnCtrl)
{
	UTiny pbuf[c_SBT_SIZE];
	UTiny inDataField[c_SBT_SIZE];
	UShort inDataFieldSize = c_SBT_SIZE - 22U;
	UShort index = 0U;
	ULong crc1_U32 = 0UL;/*init*/
	ULong crc2_U32 = 0UL;/*init*/
	ULong crcM_U32 = 0UL;/*init*/
	UShort mesgCRC_U16 = 0U;/*init*/

	pbuf[index] = c_APPLCAT_BTM;
	index++;
	pbuf[index] = c_MSG_SBT;
	index++;
	Rsp1_UShort2Buffer(&pbuf[index], c_BTM_SA);
	index += 2U;
	Rsp1_UShort2Buffer(&pbuf[index], c_BTM_DA);
	index += 2U;
	Rsp1_ULong2Buffer(&pbuf[index], inChnCtrl->timeStampMgr.tc);
	index += 4U;
	Rsp1_UShort2Buffer(&pbuf[index], c_SBT_SDS_LENGTH);
	index += 2U;
	/*����CRC32*/
	crc1_U32 = Rsp1_SafetyCRCCompute(inDataField, inDataFieldSize, 0U);
	crc2_U32 = Rsp1_SafetyCRCCompute(inDataField, inDataFieldSize, 1U);
	/*����˫ͨ��У��ֵ*/
	crcM_U32 = crc1_U32 ^ (inNodeParam->sid[0]) ^ (inChnCtrl->timeStampMgr.tNow[0]) ^ c_SYSCKW_1;
	Rsp1_ULong2Buffer(&pbuf[index], crcM_U32);
	index += 4U;
	/*����˫ͨ��У��ֵ*/
	crcM_U32 = crc2_U32 ^ (inNodeParam->sid[1]) ^ (inChnCtrl->timeStampMgr.tNow[1]) ^ c_SYSCKW_2;
	Rsp1_ULong2Buffer(&pbuf[index], crcM_U32);
	index += 4U;
	F_GL_MemSet(&pbuf[index], 0, (c_SBT_SDS_LENGTH - 8U));
	index += c_SBT_SDS_LENGTH - 8U;
	Rsp1_UShort2Buffer(&pbuf[index], Rsp1_ComputeMesgCRC(pbuf, c_SBT_SIZE - 2U));
	F_GL_MemCpy(inChnCtrl->pSendBuf, pbuf, c_SBT_SIZE);
	inChnCtrl->byteInSendBuf = c_SBT_SIZE;
}

/****************************************************************
*Name: Rsp1_FormEBTMessage
*Function:���SBT֡
*para:��
*return:��
*Create by ChenX 2017/07/05
****************************************************************/
static void Rsp1_FormEBTMessage(const S_NodeParam* inNodeParam, S_ChnControl *inChnCtrl)
{
	UTiny pbuf[c_EBT_SIZE];
	UTiny inDataField[c_EBT_SIZE];
	UShort inDataFieldSize = c_EBT_SIZE - 22U;
	UShort index = 0U;
	ULong crc1_U32 = 0UL;/*init*/
	ULong crc2_U32 = 0UL;/*init*/
	ULong crcM_U32 = 0UL;/*init*/
	UShort mesgCRC_U16 = 0U;/*init*/

	pbuf[index] = c_APPLCAT_BTM;
	index++;
	pbuf[index] = c_MSG_EBT;
	index++;
	Rsp1_UShort2Buffer(&pbuf[index], c_BTM_SA);
	index += 2U;
	Rsp1_UShort2Buffer(&pbuf[index], c_BTM_DA);
	index += 2U;
	Rsp1_ULong2Buffer(&pbuf[index], inChnCtrl->timeStampMgr.tc);
	index += 4U;
	Rsp1_UShort2Buffer(&pbuf[index], c_EBT_SDS_LENGTH);
	index += 2U;
	/*����CRC32*/
	crc1_U32 = Rsp1_SafetyCRCCompute(inDataField, inDataFieldSize, 0U);
	crc2_U32 = Rsp1_SafetyCRCCompute(inDataField, inDataFieldSize, 1U);
	/*����˫ͨ��У��ֵ*/
	crcM_U32 = crc1_U32 ^ (inNodeParam->sid[0]) ^ (inChnCtrl->timeStampMgr.tNow[0]) ^ c_SYSCKW_1;
	Rsp1_ULong2Buffer(&pbuf[index], crcM_U32);
	index += 4U;
	/*����˫ͨ��У��ֵ*/
	crcM_U32 = crc2_U32 ^ (inNodeParam->sid[1]) ^ (inChnCtrl->timeStampMgr.tNow[1]) ^ c_SYSCKW_2;
	Rsp1_ULong2Buffer(&pbuf[index], crcM_U32);
	index += 4U;
	Rsp1_UShort2Buffer(&pbuf[index], Rsp1_ComputeMesgCRC(pbuf, c_EBT_SIZE - 2U));
	F_GL_MemCpy(inChnCtrl->pSendBuf, pbuf, c_EBT_SIZE);
	inChnCtrl->byteInSendBuf = c_EBT_SIZE;
}

/****************************************************************
*Name: Rsp1_FormABTMessage
*Function:���SBT֡
*para:��
*return:��
*Create by ChenX 2017/07/05
****************************************************************/
static void Rsp1_FormABTMessage(const S_NodeParam* inNodeParam, S_ChnControl *inChnCtrl)
{
	UTiny pbuf[c_ABT_SIZE];
	UTiny inDataField[c_ABT_SIZE];
	UShort inDataFieldSize = c_ABT_SIZE - 22;
	UShort index = 0U;
	ULong crc1_U32 = 0UL;/*init*/
	ULong crc2_U32 = 0UL;/*init*/
	ULong crcM_U32 = 0UL;/*init*/
	UShort mesgCRC_U16 = 0U;/*init*/

	pbuf[index] = c_APPLCAT_BTM;
	index++;
	pbuf[index] = c_MSG_ABT;
	index++;
	Rsp1_UShort2Buffer(&pbuf[index], c_BTM_SA);
	index += 2U;
	Rsp1_UShort2Buffer(&pbuf[index], c_BTM_DA);
	index += 2U;
	Rsp1_ULong2Buffer(&pbuf[index], inChnCtrl->timeStampMgr.tc);
	index += 4U;
	Rsp1_UShort2Buffer(&pbuf[index], c_ABT_SDS_LENGTH);
	index += 2U;
	/*����CRC32*/
	crc1_U32 = Rsp1_SafetyCRCCompute(inDataField, inDataFieldSize, 0U);
	crc2_U32 = Rsp1_SafetyCRCCompute(inDataField, inDataFieldSize, 1U);
	/*����˫ͨ��У��ֵ*/
	crcM_U32 = crc1_U32 ^ (inNodeParam->sid[0]) ^ (inChnCtrl->timeStampMgr.tNow[0]) ^ c_SYSCKW_1;
	Rsp1_ULong2Buffer(&pbuf[index], crcM_U32);
	index += 4U;
	/*����˫ͨ��У��ֵ*/
	crcM_U32 = crc2_U32 ^ (inNodeParam->sid[1]) ^ (inChnCtrl->timeStampMgr.tNow[1]) ^ c_SYSCKW_2;
	Rsp1_ULong2Buffer(&pbuf[index], crcM_U32);
	index += 4U;
	Rsp1_UShort2Buffer(&pbuf[index], Rsp1_ComputeMesgCRC(pbuf, c_ABT_SIZE - 2U));
	F_GL_MemCpy(inChnCtrl->pSendBuf, pbuf, c_ABT_SIZE);
	inChnCtrl->byteInSendBuf = c_ABT_SIZE;
}

/****************************************************************
*Name: Rsp1_FormBTCMessage
*Function:���SBT֡
*para:��
*return:��
*Create by ChenX 2017/07/05
****************************************************************/
static void Rsp1_FormBTCMessage(const S_NodeParam* inNodeParam, S_ChnControl *inChnCtrl)
{
	UTiny pbuf[c_BTC_SIZE];
	UTiny inDataField[c_BTC_SIZE - 22] = { 0 };
	UShort inDataFieldSize = c_BTC_SIZE - 22;
	UShort index = 0U;
	ULong crc1_U32 = 0UL;/*init*/
	ULong crc2_U32 = 0UL;/*init*/
	ULong crcM_U32 = 0UL;/*init*/
	UShort mesgCRC_U16 = 0U;/*init*/

	pbuf[index] = c_APPLCAT_BTM;
	index++;
	pbuf[index] = c_MSG_BTC;
	index++;
	Rsp1_UShort2Buffer(&pbuf[index], c_BTM_SA);
	index += 2U;
	Rsp1_UShort2Buffer(&pbuf[index], c_BTM_DA);
	index += 2U;
	Rsp1_ULong2Buffer(&pbuf[index], inChnCtrl->timeStampMgr.tc);
	index += 4U;
	Rsp1_UShort2Buffer(&pbuf[index], c_BTC_SDS_LENGTH);
	index += 2U;
	/*����CRC32*/
	crc1_U32 = Rsp1_SafetyCRCCompute(inDataField, inDataFieldSize, 0U);
	crc2_U32 = Rsp1_SafetyCRCCompute(inDataField, inDataFieldSize, 1U);
	/*����˫ͨ��У��ֵ*/
	crcM_U32 = crc1_U32 ^ (inNodeParam->sid[0]) ^ (inChnCtrl->timeStampMgr.tNow[0]) ^ c_SYSCKW_1;
	Rsp1_ULong2Buffer(&pbuf[index], crcM_U32);
	index += 4U;
	/*����˫ͨ��У��ֵ*/
	crcM_U32 = crc2_U32 ^ (inNodeParam->sid[1]) ^ (inChnCtrl->timeStampMgr.tNow[1]) ^ c_SYSCKW_2;
	Rsp1_ULong2Buffer(&pbuf[index], crcM_U32);
	index += 4U;
	F_GL_MemSet(&pbuf[index], 0, (c_BTC_SDS_LENGTH - 8U));
	index += c_BTC_SDS_LENGTH - 8U;
	Rsp1_UShort2Buffer(&pbuf[index], Rsp1_ComputeMesgCRC(pbuf, c_BTC_SIZE - 2U));
	F_GL_MemCpy(inChnCtrl->pSendBuf, pbuf, c_BTC_SIZE);
	inChnCtrl->byteInSendBuf = c_BTC_SIZE;
}

/****************************************************************
*Name: Rsp1_FormBTDMessage
*Function:���SBT֡
*para:��
*return:��
*Create by ChenX 2017/07/05
****************************************************************/
static void Rsp1_FormBTDMessage(const S_NodeParam* inNodeParam, S_ChnControl *inChnCtrl)
{
	UTiny pbuf[c_BTD_SIZE];
	UTiny inDataField[c_BTD_SIZE - 22U];
	UShort inDataFieldSize = c_BTD_SIZE - 22U;
	UShort index = 0U;
	ULong crc1_U32 = 0UL;/*init*/
	ULong crc2_U32 = 0UL;/*init*/
	ULong crcM_U32 = 0UL;/*init*/
	UShort mesgCRC_U16 = 0U;/*init*/
	pbuf[index] = c_APPLCAT_BTM;
	index++;
	pbuf[index] = c_MSG_BTD;
	index++;
	Rsp1_UShort2Buffer(&pbuf[index], c_BTM_SA);
	index += 2U;
	Rsp1_UShort2Buffer(&pbuf[index], c_BTM_DA);
	index += 2U;
	Rsp1_ULong2Buffer(&pbuf[index], inChnCtrl->timeStampMgr.tc);
	index += 4U;
	Rsp1_UShort2Buffer(&pbuf[index], c_BTD_SDS_LENGTH);
	index += 2U;
	/*����CRC32*/
	crc1_U32 = Rsp1_SafetyCRCCompute(inDataField, inDataFieldSize, 0U);
	crc2_U32 = Rsp1_SafetyCRCCompute(inDataField, inDataFieldSize, 1U);
	/*����˫ͨ��У��ֵ*/
	crcM_U32 = crc1_U32 ^ (inNodeParam->sid[0]) ^ (inChnCtrl->timeStampMgr.tNow[0]) ^ c_SYSCKW_1;
	Rsp1_ULong2Buffer(&pbuf[index], crcM_U32);
	index += 4U;
	/*����˫ͨ��У��ֵ*/
	crcM_U32 = crc2_U32 ^ (inNodeParam->sid[1]) ^ (inChnCtrl->timeStampMgr.tNow[1]) ^ c_SYSCKW_2;
	Rsp1_ULong2Buffer(&pbuf[index], crcM_U32);
	index += 4U;
	F_GL_MemSet(&pbuf[index], 0, (c_BTD_SDS_LENGTH - 8U));
	index += c_BTD_SDS_LENGTH - 8U;
	Rsp1_UShort2Buffer(&pbuf[index], Rsp1_ComputeMesgCRC(pbuf, c_BTD_SIZE - 2U));
	F_GL_MemCpy(inChnCtrl->pSendBuf, pbuf, c_BTD_SIZE);
	inChnCtrl->byteInSendBuf = c_BTD_SIZE;
}


/****************************************************************
*Name: Rsp1_FormBTDMessage
*Function:���SBT֡
*para:��
*return:��
*Create by ChenX 2017/07/05
****************************************************************/
static void LEU_FormSSEMessage(S_ChnControl *inChnCtrl)
{
	UTiny pbuf[c_SSE_SIZE];
	UShort index = 0U;

	pbuf[index] = c_APPLCAT_BTM;
	index++;
	pbuf[index] = c_MSG_SSE;
	index++;
	Rsp1_UShort2Buffer(&pbuf[index], c_BTM_SA);
	index += 2U;
	Rsp1_UShort2Buffer(&pbuf[index], c_BTM_DA);
	index += 2U;
	Rsp1_ULong2Buffer(&pbuf[index], inChnCtrl->timeStampMgr.tc);
	index += 4U;
	Rsp1_ULong2Buffer(&pbuf[index], inChnCtrl->sseInfoTx.seq[0]);
	index += 4U;
	Rsp1_ULong2Buffer(&pbuf[index], inChnCtrl->sseInfoTx.seq[1]);
	index += 4U;
	Rsp1_UShort2Buffer(&pbuf[index], Rsp1_ComputeMesgCRC(pbuf, c_SSE_SIZE - 2U));
	F_GL_MemCpy(inChnCtrl->pSendBuf, pbuf, c_SSE_SIZE);
	inChnCtrl->byteInSendBuf = c_SSE_SIZE;
}


/****************************************************************
*Name: LEU_FormSSRMessage
*Function:���SBT֡
*para:��
*return:��
*Create by ChenX 2017/07/05
****************************************************************/
static void LEU_FormSSRMessage(S_ChnControl *inChnCtrl)
{
	UTiny pbuf[c_SSR_SIZE];
	UShort index = 0U;

	pbuf[index] = c_APPLCAT_BTM;
	index++;
	pbuf[index] = c_MSG_SSR;
	index++;
	Rsp1_UShort2Buffer(&pbuf[index], c_BTM_SA);
	index += 2U;
	Rsp1_UShort2Buffer(&pbuf[index], c_BTM_DA);
	index += 2U;
	Rsp1_ULong2Buffer(&pbuf[index], inChnCtrl->timeStampMgr.tc);
	index += 4U;
	Rsp1_ULong2Buffer(&pbuf[index], inChnCtrl->sseInfoRx.tc);
	index += 4U;
	Rsp1_ULong2Buffer(&pbuf[index], inChnCtrl->ssrInfoTx.seqInit[0]);
	index += 4U;
	Rsp1_ULong2Buffer(&pbuf[index], inChnCtrl->ssrInfoTx.seqInit[1]);
	index += 4U;
	pbuf[index] = 0x01;
	index++;
	Rsp1_UShort2Buffer(&pbuf[index], Rsp1_ComputeMesgCRC(pbuf, c_SSR_SIZE - 2U));

	F_GL_MemCpy(inChnCtrl->pSendBuf, pbuf, c_SSR_SIZE);
}
/****************************************************************
*Name:  Rsp1_GetLeuSynData
*Function:��ȡLEUͬ������ֵ
*para:outBuf,ͬ�����ݻ���
*return:ͬ�����ݳ���
*Create by ChenX 2017/11/11
****************************************************************/
UShort LEU_GetSynData(UTiny *outBuf)
{
	UShort i = 0U;
	UTiny tmpBuff[c_BUFF_SIZE];
	UShort index = 0U;
	UShort tmpCrc = 0U;

	F_GL_MemSet(tmpBuff, 0, (UShort)sizeof(c_BUFF_SIZE));
	if (outBuf != rNULL)
	{
		Rsp1_UShort2Buffer(&tmpBuff[index], s_RsspClock);/*����Э��ջʱ��*/
		index += 2U;
		for (i = 0U; i<c_LEU_NUMBER; i++)
		{
			/*ÿ����·ʱ���*/
			Rsp1_ULong2Buffer(&tmpBuff[index + (i * 4U)], s_BtmPrtclCtrlList[i].protocolCtrl.safeCtrl[0].tcrNow);
			index += 4U;
			Rsp1_ULong2Buffer(&tmpBuff[index + (i * 4U)], s_RSDPrtclCtrlList[i].safeCtrl[0].tcrNow);
			index += 4U;
			Rsp1_UShort2Buffer(&tmpBuff[index + (i * 2U)], s_BtmPrtclCtrlList[i].btm_status);
			index += 2U;
		}
		tmpCrc = Rsp1_ComputeMesgCRC(tmpBuff, index);
		Rsp1_UShort2Buffer(&tmpBuff[index], tmpCrc);/*CRC*/
		index += 2U;

		F_GL_MemCpy(outBuf, tmpBuff, index);
	}
	return index;
}

/****************************************************************
*Name:  Rsp1_SetLeuSynData
*Function:����leuͬ������ֵ
*para:inBuf��ͬ�����ݻ��壻inDataLength_U16��ͬ�����ݳ���
*return:��
*Create by ChenX 2017/11/11
****************************************************************/
void LEU_SetLeuSynData(const UTiny *inBuf,
	UShort inDataLength_U16)
{
	UShort linkIndex = 0U;
	UTiny tmpBuff[c_BUFF_SIZE];
	UShort recvCrc = 0U;
	UShort tmpUsedLinkNum = 0U;
	UShort index = 0U;
	UShort tmpCrc = 0U;

	F_GL_MemCpy(tmpBuff, inBuf, inDataLength_U16);
	recvCrc = Rsp1_Buffer2UShort(&tmpBuff[inDataLength_U16 - 2U]);
	tmpCrc = Rsp1_ComputeMesgCRC(inBuf, inDataLength_U16 - 2U);
	if (tmpCrc == recvCrc)
	{
		s_RsspClock = Rsp1_Buffer2UShort(&tmpBuff[index]);/*����Э��ջʱ��*/
		index += 2U;
		for (linkIndex = 0; linkIndex < c_LEU_NUMBER; linkIndex++)
		{
			s_BtmPrtclCtrlList[linkIndex].protocolCtrl.safeCtrl[0].tcrNow = Rsp1_Buffer2ULong(&tmpBuff[index]); /* ��¼����ʱ�� */
			index += 4U;
			s_RSDPrtclCtrlList[linkIndex].safeCtrl[0].tcrNow = Rsp1_Buffer2ULong(&tmpBuff[index]);
			index += 4U;
			s_BtmPrtclCtrlList[linkIndex].btm_status = Rsp1_Buffer2UShort(&tmpBuff[index]);
			index += 2U;
			s_PrtclCtrlList[linkIndex].safeCtrl[0].linkoffTimeCntr = s_RsspClock; /* ��¼���� */
		}
	}
	else
	{
		Rsp1_SetErrorCode(c_LEUID, c_ERR_SYN_ERR); /* ϵ��ͬ��ʧ�ܣ��㱨 */
	}
}


