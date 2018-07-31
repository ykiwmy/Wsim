#include"sahara.h"
//#include "Vcp_Device_Definition_Inf.h"
//#include "Vcp_Service_Interface.h"
//#include "TGMT_Interface.h"
#include <stdlib.h>
//#include "Log.h"
//#include "SYNC_Process.h"

static void OpenSafe(S_ProtocolControl* pCtrl); 
static void OpenRedun(S_ProtocolControl* pCtrl);
static void CloseRedun(S_ProtocolControl* pCtrl);
static void Pack_SaharaMsg(S_ProtocolControl* pCtrl, MyUINT16 type);
static void FormConnSeq(S_ProtocolControl* pCtrl);
static void FormConnResp(S_ProtocolControl * pCtrl);
static void FormReReq(S_ProtocolControl * pCtrl);
static void FormReResp(S_ProtocolControl * pCtrl);
static void FormHeartbeat(S_ProtocolControl * pCtrl);
static void FormData(S_ProtocolControl * pCtrl);
static void FormDisconnReq(S_ProtocolControl * pCtrl);
static void FormReData(S_ProtocolControl * pCtrl);
static MyUINT8 Cleanup(S_ProtocolControl* pCtrl);
static void SetSafeState(S_ProtocolControl* pCtrl, MyUINT8 state);
static void SetRedunState(S_ProtocolControl* pCtrl, MyUINT8 state);
static void MyUINT322Buffer(MyUINT8 * pbuf, MyUINT32 dword);
static void MyUINT162Buffer(MyUINT8* pbuf, MyUINT16 word);
static MyUINT16 Buffer2MyUINT16(const MyUINT8 * bufferPtr);
static MyUINT32 Buffer2MyUINT32(const MyUINT8 * bufferPtr);
static void md4_function();
static Hashcode md4(MyUINT8*key, MyUINT32 length);
static MyBOOL IsServer();
static void InitTmo(S_ProtocolControl* pCtrl);
static void ClearAllDeferQueue(S_ProtocolControl* pCtrl);
static void CheckRedun(S_ProtocolControl* pCtrl, const MyUINT16 inlen, MyUINT8 *inbuf);
static MyUINT8 CheckInitFlag(S_ProtocolControl* pCtrl, const MyUINT16 inlen, MyUINT8 *inbuf);
static MyUINT8 ReceiveData(S_ProtocolControl* pCtrl, const  MyUINT16 inLen, MyUINT8 *inBuf);
static void DeliverDeferQueue(S_ProtocolControl* pCtrl);
static void DeferTmo(S_ProtocolControl* pCtrl);
static MyUINT32 DiffTime(MyUINT32 timer);
static void CheckSafe(S_ProtocolControl* pCtrl, MyUINT16 *inlen, MyUINT8 *inbuf);
static MyBOOL CheckSafeData(S_ProtocolControl* pCtrl);
static void GetPeerBuffSize(S_ProtocolControl* pCtrl);
static void ProcessSafeMsg(S_ProtocolControl* pCtrl);
static void UpdateTI(S_ProtocolControl * pCtrl);
static void ClearReTx(S_ProtocolControl* pCtrl);
static MyUINT8 SendSafe(S_ProtocolControl* pCtrl, const MyUINT16 inlen, MyUINT8 *inbuf);
static void ProcessDisReq(S_ProtocolControl* pCtrl);
static void ProcessReData(S_ProtocolControl* pCtrl);
static void ProcessReResp(S_ProtocolControl* pCtrl);
static void ProcessReReq(S_ProtocolControl* pCtrl);
static void ProcessConnResp(S_ProtocolControl* pCtrl);
static void ProcessHeartbeat(S_ProtocolControl* pCtrl);
static void ProcessData(S_ProtocolControl* pCtrl);
static void ProcessConnSeq(S_ProtocolControl* pCtrl);
static void FormRedunData(S_ProtocolControl* pCtrl);
static MyUINT16 PartitionSafeData(S_ProtocolControl* pCtrl);
static MyBOOL CheckSafeType(S_ProtocolControl* pCtrl);
static MyBOOL CheckTime(S_ProtocolControl* pCtrl);
static void DeliverToSafe(S_ProtocolControl* pCtrl, MyUINT8 *inBuf);
static void DeliverToUser(S_ProtocolControl* pCtrl);
static void PartitionRedunData(S_ProtocolControl* pCtrl,  MyUINT8 *inBuf);
static MyUINT8 CheckReTx(S_ProtocolControl* pCtrl);
static MyUINT8 CheckRedunSeq(S_ProtocolControl* pCtrl, MyUINT16 inLen, MyUINT8 *inBuf);
static MyBOOL CheckSafeSeq(S_ProtocolControl* pCtrl);
static MyBOOL CheckVersion(S_ProtocolControl* pCtrl);
static MyUINT8 StoreInDeferQueue(S_ProtocolControl* pCtrl, MyUINT16 inLen, MyUINT8 *inBuf);
static void CheckDeferQueue(S_ProtocolControl* pCtrl);
static MyUINT8 SendUsrData(S_ProtocolControl* pCtrl);
static MyBOOL CheckMesgMD4(MyUINT8* dataBuf, MyUINT16 dataSize);
static MyBOOL IsBigEndian();
static S_ProtocolControl* GetpCtrl(const MyUINT16 inDstDevId);
static void Sahara_Start(S_ProtocolControl* pCtrl);
static void Sahara_End(S_ProtocolControl *pCtrl);
static void DisconnProcess(S_ProtocolControl* pCtrl, MyUINT16 Isreopen, MyUINT16 cause);
static void InitRedun(S_ProtocolControl* pCtrl);
static void  Sahara_PackSyncData(S_ProtocolControl *pCtrl);
static void  Sahara_UnpackSyncData(S_ProtocolControl *pCtrl);

//extern S_SyncInputMsg SyncInputMsg;
extern MyBOOL IsRecvAfterSwitchHost;

static MyUINT32 SaharaClock; // ��������
static S_ProtocolControl linkList[RealLinkNum];
/*md4��ر���*/
static MyUINT32 X[64], length_pad[2];
static MyUINT32 AA, BB, CC, DD, A, B, C, D;
static MyUINT32 round;


/** *************************************************************
��ȡЭ��ջָ��
************************************************************* */
static S_ProtocolControl* GetpCtrl(const MyUINT16 inDstDevId)
{
	S_ProtocolControl* ret = NULL;
	if (inDstDevId > RealLinkNum || inDstDevId < 0)
	{
		F_GL_MemSet(linkList, 0, sizeof(S_ProtocolControl)*RealLinkNum);
	}
	else
	{
		ret = &linkList[inDstDevId];
	}
	return ret;
}

/** *************************************************************
��ȡ��·״̬
************************************************************* */
MyBOOL F_Sahara_GetStatus(const MyUINT16 inDstDevId)
{
	MyBOOL ret = e_UsrFalse;
	S_ProtocolControl *pCtrl = GetpCtrl(inDstDevId);
	if(s_Up == pCtrl->SafeNextState ||s_ReReq == pCtrl->SafeNextState||s_ReRun == pCtrl->SafeNextState)
	{
		ret = e_UsrTrue;
	}
	else
	{
		ret = e_UsrFalse;
	}
	return ret;
}


/** *************************************************************
����ʱ��
************************************************************* */
void F_Sahara_UpdateClock()
{
	if (SaharaClock == 0xFFFFFFFFU)
	{
		SaharaClock = 0;
	}
	else
	{
		SaharaClock++;
	}
}

/** *************************************************************
���sahara ͬ����������
************************************************************* */
static void  Sahara_UnpackSyncData(S_ProtocolControl *pCtrl)
{
	/*if(E_STANDBY == F_SYNC_HostOrStandby()||e_UsrTrue == IsRecvAfterSwitchHost)
	{
		pCtrl->RedunNextState =SyncInputMsg.SaharaInputMsg.RedunNextState;
		pCtrl->R_Seq.SeqTX = SyncInputMsg.SaharaInputMsg.SeqTX;
		pCtrl->R_Seq.SeqRX = SyncInputMsg.SaharaInputMsg.SeqRX;
		pCtrl->IsInitTActive = SyncInputMsg.SaharaInputMsg.IsInitTActive;
		pCtrl->SafeNextState = SyncInputMsg.SaharaInputMsg.SafeNextState;
		pCtrl->Time.TLastRecv = SyncInputMsg.SaharaInputMsg.TLastRecv ;
		pCtrl->Time.TI = SyncInputMsg.SaharaInputMsg.TI;
		pCtrl->Time.Tlastrtd= SyncInputMsg.SaharaInputMsg.Tlastrtd;
		pCtrl->Time.Tstart= SyncInputMsg.SaharaInputMsg.Tstart;
		SaharaClock = SyncInputMsg.SaharaInputMsg.SaharaClock;
		pCtrl->S_Seq.SNT = SyncInputMsg.SaharaInputMsg.SNT ;
		pCtrl->S_Seq.SNR = SyncInputMsg.SaharaInputMsg.SNR ;
		pCtrl->RightSafeRecvMsg.s= SyncInputMsg.SaharaInputMsg.recv_s;
		pCtrl->RightSafeRecvMsg.cs= SyncInputMsg.SaharaInputMsg.recv_cs ;
		pCtrl->RightSafeRecvMsg.ts = SyncInputMsg.SaharaInputMsg.recv_ts  ;
		pCtrl->RightSafeRecvMsg.cts = SyncInputMsg.SaharaInputMsg.recv_cts;
		pCtrl->SafeSendMsg.cs_old =SyncInputMsg.SaharaInputMsg.send_cs_old ;
		pCtrl->SafeSendMsg.s = SyncInputMsg.SaharaInputMsg.send_s;
		//logprintf("R_state:%d,S_state:%d,SeqTX:%d,SeqRx:%d\n",pCtrl->RedunNextState,pCtrl->SafeNextState,
			pCtrl->R_Seq.SeqTX,pCtrl->R_Seq.SeqRX);
		if(e_UsrTrue == IsRecvAfterSwitchHost)
		{
			//logprintf("Switch Recv last message!\n");
			IsRecvAfterSwitchHost = e_UsrFalse;
		}
	}
	*/
}


/** *************************************************************
���saharaͬ���������
************************************************************* */
static void  Sahara_PackSyncData(S_ProtocolControl *pCtrl)
{
	/*if(E_HOST == F_SYNC_HostOrStandby())
	{
		SyncInputMsg.SaharaInputMsg.RedunNextState = pCtrl->RedunNextState;
		SyncInputMsg.SaharaInputMsg.SeqTX = pCtrl->R_Seq.SeqTX;
		SyncInputMsg.SaharaInputMsg.SeqRX = pCtrl->R_Seq.SeqRX;
		SyncInputMsg.SaharaInputMsg.IsInitTActive = pCtrl->IsInitTActive;
		SyncInputMsg.SaharaInputMsg.SafeNextState= pCtrl->SafeNextState;
		SyncInputMsg.SaharaInputMsg.TLastRecv = pCtrl->Time.TLastRecv;
		SyncInputMsg.SaharaInputMsg.TI =  pCtrl->Time.TI;
		SyncInputMsg.SaharaInputMsg.Tlastrtd = pCtrl->Time.Tlastrtd;
		SyncInputMsg.SaharaInputMsg.Tstart = pCtrl->Time.Tstart;
		SyncInputMsg.SaharaInputMsg.SaharaClock = SaharaClock;
		SyncInputMsg.SaharaInputMsg.SNT =  pCtrl->S_Seq.SNT;
		SyncInputMsg.SaharaInputMsg.SNR = pCtrl->S_Seq.SNR;
		SyncInputMsg.SaharaInputMsg.recv_s = pCtrl->RightSafeRecvMsg.s;
		SyncInputMsg.SaharaInputMsg.recv_cs = pCtrl->RightSafeRecvMsg.cs;
		SyncInputMsg.SaharaInputMsg.recv_ts = pCtrl->RightSafeRecvMsg.ts;
		SyncInputMsg.SaharaInputMsg.recv_cts =  pCtrl->RightSafeRecvMsg.cts;
		SyncInputMsg.SaharaInputMsg.send_cs_old = pCtrl->SafeSendMsg.cs_old;
		SyncInputMsg.SaharaInputMsg.send_s = pCtrl->SafeSendMsg.s;
	}*/
}





/** *************************************************************
Э���ʼ��
************************************************************* */
void F_Sahara_Init(const MyUINT16 inDstDevId)
{
	S_ProtocolControl *pCtrl = GetpCtrl(inDstDevId);
	F_GL_MemSet(pCtrl,0,sizeof(S_ProtocolControl));
	SetRedunState(pCtrl, r_Close);
	SetSafeState(pCtrl,s_Close);
	pCtrl->IsInitTActive = e_UsrFalse;
	SaharaClock = 0;
}

/** *************************************************************
�ϲ�����ر������
************************************************************* */
static void Sahara_End(S_ProtocolControl *pCtrl)
{
	if ((pCtrl->SafeNextState == s_Close) && (pCtrl->RedunNextState != r_Close))
	{
		CloseRedun(pCtrl);
	}
}


/** *************************************************************
У���������
************************************************************* */
MyUINT16 F_Sahara_CheckData(const MyUINT16 inDstDevId, const MyUINT16 inlen, MyUINT8 *inbuf,
	MyUINT16 *outlen, MyUINT8 *outbuf)
{
	S_ProtocolControl *pCtrl = GetpCtrl(inDstDevId);
	/*if(E_HOST == F_SYNC_HostOrStandby())
	{*/
		pCtrl->RedunRecvMsg.msgLength = inlen;
		F_GL_MemCpy(pCtrl->RedunRecvMsg.msg,inbuf, pCtrl->RedunRecvMsg.msgLength);
		Sahara_Start(pCtrl);
		while (pCtrl->RedunRecvMsg.msgLength > 0)
		{
			PartitionRedunData(pCtrl,  pCtrl->RedunRecvMsg.msg);
			if (pCtrl->RedunRecvMsg.msgLength >= pCtrl->RecvMsg.length 
				&& pCtrl->RecvMsg.length > 0)
			{
				/*У�������*/
				CheckRedun(pCtrl, pCtrl->RecvMsg.length, pCtrl->RedunRecvMsg.msg);
			}
			else
			{ /*��ֹ��ѭ��*/
				F_GL_MemSet(&pCtrl->SafeRecv, 0, sizeof(S_Msg));
				F_GL_MemSet(&pCtrl->RedunRecvMsg, 0, sizeof(S_Msg));
				break;
			}
			pCtrl->RedunRecvMsg.msgLength -= pCtrl->RecvMsg.length;
			F_GL_MemCpy(&pCtrl->RedunRecvMsg.msg[0], &pCtrl->RedunRecvMsg.msg[pCtrl->RecvMsg.length], 
						pCtrl->RedunRecvMsg.msgLength);
			F_GL_MemSet(&pCtrl->RecvMsg, 0, sizeof(S_MsgRedun));
		}
		F_GL_MemSet(&pCtrl->RedunRecvMsg, 0, sizeof(S_Msg));
		/*���ڶ��г�ʱ���*/
		DeferTmo(pCtrl);
		/*У�鰲ȫ��*/
		CheckSafe(pCtrl, outlen, outbuf);
		
		F_GL_MemSet(&pCtrl->SafeRecv,0,sizeof(S_Msg));
		F_GL_MemSet(&pCtrl->RecvUsrData,0,sizeof(S_Msg));
	//}
	return 0;
}

/** *************************************************************
��������
����ֵ
	0 �����ͻ�����������ܷ����û�����
         1������
*** ********************************************************** */
MyUINT16 F_Sahara_SendData(const MyUINT16 inDstDevId, const MyUINT16 inlen, MyUINT8 *inbuf,MyUINT16* outlen ,MyUINT8* outbuff)
{
	S_ProtocolControl *pCtrl = GetpCtrl(inDstDevId);
	S_Msg outBuffer[10] = {0};
	MyUINT8 result = 0;
	MyUINT16 length = 0;
	MyUINT8 num = 0; 
	MyUINT8 PipeId1 = 0 ;
	//E_UsrTxStatus sendstatus = 0;
	MyUINT8 msgnum = 0;
	S_Msg sendbuff = {  0  };
	MyUINT8 index = 0;
	if ((pCtrl->SendUsrDataMem.msgLength + inlen) < BUFF_SIZE)
	{
		F_GL_MemCpy(&pCtrl->SendUsrDataMem.msg[pCtrl->SendUsrDataMem.msgLength], inbuf, inlen);
		pCtrl->SendUsrDataMem.msgLength += inlen;
		result = 1;
	}
	else
	{
		F_GL_MemSet(&pCtrl->SendUsrDataMem, 0, sizeof(S_Msg));
		result = 0;
	}
	SendSafe(pCtrl, inlen, inbuf);   
	InitTmo(pCtrl);
	//logprintf("Sahara send len:%d bytes\n",pCtrl->RedunSendMsg.msgLength);
	if (r_Init == pCtrl->RedunNextState || r_Open == pCtrl->RedunNextState)
	{
		if (pCtrl->RedunSendMsg.msgLength > 0)
		{
			F_GL_MemCpy(outbuff, &pCtrl->RedunSendMsg.msg, pCtrl->RedunSendMsg.msgLength);
			*outlen = pCtrl->RedunSendMsg.msgLength;
		}
		F_GL_MemSet(&pCtrl->RedunSendMsg, 0, sizeof(S_Msg));
	}
	Sahara_End(pCtrl);
	return result;
}

/** *************************************************************
��ʼ����
************************************************************* */
static void Sahara_Start(S_ProtocolControl* pCtrl)
{
	OpenRedun(pCtrl);
	OpenSafe(pCtrl);
}
/** *************************************************************
���Ͱ�ȫ������
************************************************************* */
static MyUINT8 SendSafe(S_ProtocolControl* pCtrl, const MyUINT16 inlen, MyUINT8 *inbuf)
{
	MyUINT8 state = pCtrl->SafeNextState;
	MyUINT32 th = 0;
	pCtrl->Time.Tlocal = SaharaClock;
	th = DiffTime(pCtrl->Time.Th);
	if (s_Up == state || s_ReReq == state || s_ReRun == state)
	{
		if (pCtrl->RecvMsgNum >= SelfBuffSize)    //��Ϣ���շ��������յ�����Ϣ���ﵽ�ֲ������С֮ǰ����ȷ��
		{
			if (pCtrl->SendUsrDataMem.msgLength > 0)
			{
				SendUsrData(pCtrl);
			}
			else
			{
				Pack_SaharaMsg(pCtrl, Heartbeat);
				//logprintf("RecvMsgNum bigger than SelfBuffSize ,Send Heartbeat!\n");
			}
		}
		else
		{
			/*���ͷ�δ�ﵽδ��ȷ����Ϣ�����*/
			if (pCtrl->UnConfirmedNum < pCtrl->peerbuffsize)             
			{
				/*���û�����*/
				if (pCtrl->SendUsrDataMem.msgLength > 0)
				{
					SendUsrData(pCtrl);
				}
				else if(th >= TH)
				{
					Pack_SaharaMsg(pCtrl, Heartbeat);
				}
			}
			else if(th >= TH )
			{
				Pack_SaharaMsg(pCtrl, Heartbeat);
			}
		}
	}

	return 1;
}

/** *************************************************************
���û�����    
����ֵ�� 0 �û����������ݷ������ 
		 1 �û����������ݴ�С�ﵽ�Է����ջ�������
************************************************************* */
static MyUINT8 SendUsrData(S_ProtocolControl* pCtrl)
{
	MyUINT8 ret = 0;
	while(pCtrl->SendUsrDataMem.msgLength>0)
	{
		if (pCtrl->SendUsrDataMem.msgLength>USERDATA_SIZE)
		{ /*����USERDATA_SIZE�ְ�*/
			pCtrl->SendUsrData.msgLength = USERDATA_SIZE;
		}
		else
		{
			pCtrl->SendUsrData.msgLength = pCtrl->SendUsrDataMem.msgLength;
		}
		if (pCtrl->SendUsrDataMem.msgLength < pCtrl->SendUsrData.msgLength)
		{/*��ֹ��ѭ��*/
			F_GL_MemSet(&pCtrl->SendUsrData,0,sizeof(S_Msg));
			F_GL_MemSet(&pCtrl->SendUsrDataMem, 0, sizeof(S_Msg));
			break;
		}
		F_GL_MemCpy(pCtrl->SendUsrData.msg, pCtrl->SendUsrDataMem.msg, pCtrl->SendUsrData.msgLength);
		pCtrl->SendUsrDataMem.msgLength -= pCtrl->SendUsrData.msgLength;
		F_GL_MemCpy(pCtrl->SendUsrDataMem.msg, &pCtrl->SendUsrDataMem.msg[pCtrl->SendUsrData.msgLength], pCtrl->SendUsrDataMem.msgLength);
		Pack_SaharaMsg(pCtrl,Data);
		F_GL_MemSet(&pCtrl->SendUsrData,0,sizeof(S_Msg));
		if(pCtrl->UnConfirmedNum >= pCtrl->peerbuffsize)
		{
			ret = 1;
			break;
		}
	}
	return ret;
}



/** *************************************************************
��Ϣ���䵽��ȫ��
************************************************************* */
static void DeliverToSafe(S_ProtocolControl* pCtrl, MyUINT8 *inBuf)
{
	if( pCtrl->RecvMsg.dataSize > 0 &&  pCtrl->RecvMsg.dataSize<BUFF_SIZE)
	{
		F_GL_MemCpy(&pCtrl->SafeRecv.msg[pCtrl->SafeRecv.msgLength], inBuf + 8, pCtrl->RecvMsg.dataSize);
		pCtrl->SafeRecv.msgLength += pCtrl->RecvMsg.dataSize;
	}
}

/** *************************************************************
У��汾
************************************************************* */
static MyBOOL CheckVersion(S_ProtocolControl* pCtrl)
{
	if ((ConnReq == pCtrl->SafeRecvMsg.stype) || (ConnResp == pCtrl->SafeRecvMsg.stype))
	{
		if (VERSION != pCtrl->SafeRecvMsg.version)
		{
			return e_UsrFalse;
		}
	}
	return e_UsrTrue;
}


/** *************************************************************
У�鰲ȫ����Ϣ����
************************************************************* */
static MyBOOL CheckSafeType(S_ProtocolControl* pCtrl)
{
	MyBOOL ret = e_UsrFalse;
	switch (pCtrl->SafeRecvMsg.stype)
	{
	case ConnReq:
	case ConnResp:
	case DisconnReq:
	case ReReq:
	case ReData:
	case ReResp:
	case Data:
	case Heartbeat:
		ret = e_UsrTrue;
		break;
	default:
		break;
	}
	return ret;
}

/** *************************************************************
У�鰲ȫ�㼰ʱ��
************************************************************* */
static MyBOOL CheckTime(S_ProtocolControl* pCtrl)
{
	MyUINT32 Diff = 0;  //��ǰ���ձ������ϴ���ȷ���ձ��ĵ�ʱ���
	MyBOOL ret = e_UsrTrue;
	if ((s_Down != pCtrl->SafeNextState) && (s_Close != pCtrl->SafeNextState))
	{
		if (pCtrl->Time.Tlocal < pCtrl->Time.TLastRecv)
		{ /*����ʱ���������*/
			Diff =  (0xFFFFFFFF - pCtrl->Time.TLastRecv) + pCtrl->Time.Tlocal;
		}
		else
		{
			Diff = pCtrl->Time.Tlocal - pCtrl->Time.TLastRecv;
		}
	}
	//logprintf("diff:%d,TI:%d\n",Diff, pCtrl->Time.TI);
	if (Diff > pCtrl->Time.TI)
	{
		ret = e_UsrFalse;
	}
	return ret;
}


/** *************************************************************
У�鰲ȫ��
************************************************************* */
static void CheckSafe(S_ProtocolControl* pCtrl, MyUINT16 *inlen, MyUINT8 *inbuf)
{
	pCtrl->Time.Tlocal = SaharaClock;
	if (e_UsrTrue == CheckTime(pCtrl))  // У�鼰ʱ��
	{
		while (pCtrl->SafeRecv.msgLength > 0)
		{
			PartitionSafeData(pCtrl);
			if (pCtrl->SafeRecv.msgLength < pCtrl->SafeRecvMsg.slength 
				|| pCtrl->SafeRecvMsg.slength == 0)
			{ /*��ֹ��ѭ��*/
				F_GL_MemSet(&pCtrl->SafeRecv,0,sizeof(S_Msg));
				F_GL_MemSet(&pCtrl->RecvUsrData, 0, sizeof(S_Msg));
				break;
			}
			if (e_UsrTrue == CheckSafeData(pCtrl))
			{
				pCtrl->RightSafeRecvMsg = pCtrl->SafeRecvMsg;
				//logprintf("Receive right seq :0x%08x\n",pCtrl->RightSafeRecvMsg.s);
				/*���յ���ȷ���ݣ������TLastRecv*/
				pCtrl->Time.TLastRecv = pCtrl->Time.Tlocal;
				/*�յ��������͵����ݣ�������Ϣ��������1*/
				pCtrl->RecvMsgNum++; 
				/*δȷ�Ϸ�����Ϣ������ = ���ط�����Ϣ���к� - ������ȷ���յ���Ϣ��ȷ�����*/
				pCtrl->UnConfirmedNum = pCtrl->SafeSendMsg.s - pCtrl->RightSafeRecvMsg.cs;
				GetPeerBuffSize(pCtrl);
				ProcessSafeMsg(pCtrl);
			}
			else
			{
				pCtrl->ErrorCount++;
				if (pCtrl->ErrorCount > 10)
				{
					pCtrl->ErrorCount = 0;
					DisconnProcess(pCtrl, 1, 0);
					//logprintf("Disconnect for over ErrorCount \n");
				}
			}

			pCtrl->SafeRecv.msgLength -= pCtrl->SafeRecvMsg.slength;
			F_GL_MemCpy(&pCtrl->SafeRecv.msg[0],
				&pCtrl->SafeRecv.msg[pCtrl->SafeRecvMsg.slength],
				pCtrl->SafeRecv.msgLength);
			F_GL_MemSet(&pCtrl->SafeRecvMsg, 0, sizeof(S_MsgSafe));
		}
	}
	else
	{
		DisconnProcess(pCtrl, 1, 4);
		//logprintf("Disconnect for over time \n");
	}
	*inlen = pCtrl->RecvUsrData.msgLength;
	F_GL_MemCpy(inbuf, pCtrl->RecvUsrData.msg, pCtrl->RecvUsrData.msgLength);
}

/** ************************************************************
�����û���
************************************************************* */
static void DeliverToUser(S_ProtocolControl* pCtrl)
{
	F_GL_MemCpy(&pCtrl->RecvUsrData.msg[pCtrl->RecvUsrData.msgLength],
		&pCtrl->SafeRecv.msg[28], pCtrl->SafeRecvMsg.slength - 36);
	pCtrl->RecvUsrData.msgLength += pCtrl->SafeRecvMsg.slength - 36;
}

/** ************************************************************
�����������ݰ�
************************************************************* */
static void PartitionRedunData(S_ProtocolControl* pCtrl,  MyUINT8 *inBuf)
{
	pCtrl->RecvMsg.length = Buffer2MyUINT16(&inBuf[0]);
	pCtrl->RecvMsg.flags = Buffer2MyUINT16(&inBuf[2]);
	pCtrl->RecvMsg.seqNo = Buffer2MyUINT32(&inBuf[4]);
	pCtrl->RecvMsg.dataSize = pCtrl->RecvMsg.length - 8; 
	//logprintf("Redun  receive seq :%d , Redun SNR:%d\n",pCtrl->RecvMsg.seqNo,pCtrl->R_Seq.SeqRX);
}
/** ************************************************************
��ְ�ȫ�����ݰ�
************************************************************* */
static MyUINT16 PartitionSafeData(S_ProtocolControl* pCtrl)
{
	pCtrl->SafeRecvMsg.slength = Buffer2MyUINT16(&pCtrl->SafeRecv.msg[0]);
	pCtrl->SafeRecvMsg.stype = Buffer2MyUINT16(&pCtrl->SafeRecv.msg[2]);
	pCtrl->SafeRecvMsg.recvaddress = Buffer2MyUINT32(&pCtrl->SafeRecv.msg[4]);
	pCtrl->SafeRecvMsg.sendaddress = Buffer2MyUINT32(&pCtrl->SafeRecv.msg[8]);
	pCtrl->SafeRecvMsg.s = Buffer2MyUINT32(&pCtrl->SafeRecv.msg[12]);
	pCtrl->SafeRecvMsg.cs = Buffer2MyUINT32(&pCtrl->SafeRecv.msg[16]);
	pCtrl->SafeRecvMsg.ts = Buffer2MyUINT32(&pCtrl->SafeRecv.msg[20]);
	pCtrl->SafeRecvMsg.cts = Buffer2MyUINT32(&pCtrl->SafeRecv.msg[24]);
	pCtrl->SafeRecvMsg.version = Buffer2MyUINT32(&pCtrl->SafeRecv.msg[28]);
	pCtrl->SafeRecvMsg.peerbuffsize = Buffer2MyUINT16(&pCtrl->SafeRecv.msg[32]);
	pCtrl->SafeRecvMsg.Cause = Buffer2MyUINT16(&pCtrl->SafeRecv.msg[30]);
	pCtrl->SafeRecvMsg.IsReopen = Buffer2MyUINT16(&pCtrl->SafeRecv.msg[28]);
	//logprintf("Safe receive seq:0x%08x,Safe receive confirmed seq:%d,Safe SNR:0x%08x\n",
	//	pCtrl->SafeRecvMsg.s,pCtrl->SafeRecvMsg.cs,pCtrl->S_Seq.SNR);
    return 1;
}

/** *************************************************************
��ʼ����ʱ�����³�ʼ��
************************************************************* */
static void InitTmo(S_ProtocolControl* pCtrl)
{
	MyUINT32 difftime = DiffTime(pCtrl->R_Seq.TInitResq);
	if ( difftime > InitResp && (e_UsrTrue == pCtrl->IsInitTActive)&& (r_Init == pCtrl->RedunNextState))  //��ʼ����Ӧ��ʱ����ʱ
	{
		InitRedun(pCtrl);
	}
}


/** ************************************************************
������������
************************************************************* */
static void FormRedunData(S_ProtocolControl* pCtrl)
{
	MyUINT8* pbuf;
	MyUINT16 safelength = pCtrl->SafeSend.msgLength;
	pCtrl->SendMsg.seqNo = pCtrl->R_Seq.SeqTX;
	pbuf = &pCtrl->RedunSendMsg.msg[pCtrl->RedunSendMsg.msgLength];
	pCtrl->SendMsg.length = safelength + 8;
	MyUINT162Buffer(pbuf, pCtrl->SendMsg.length);
	MyUINT162Buffer(&pbuf[2], pCtrl->SendMsg.flags);
	MyUINT322Buffer(&pbuf[4], pCtrl->SendMsg.seqNo);
	F_GL_MemCpy(&pbuf[8], pCtrl->SafeSend.msg, safelength);
	pCtrl->RedunSendMsg.msgLength += pCtrl->SendMsg.length;
	F_GL_MemSet(&pCtrl->SendMsg, 0, sizeof(S_MsgRedun));
}

/** *************************************************************
У�鰲ȫ�����к�
************************************************************* */
static MyBOOL CheckSafeSeq(S_ProtocolControl* pCtrl)
{
	switch (pCtrl->SafeRecvMsg.stype)
	{
	case ConnReq:
	case ConnResp:
	case ReResp:
		pCtrl->S_Seq.SNR = pCtrl->SafeRecvMsg.s;
		break;
	case ReReq:
		if (pCtrl->S_Seq.SNR != pCtrl->SafeRecvMsg.s)
		{
			pCtrl->S_Seq.SNR = pCtrl->SafeRecvMsg.s;
		}
		break;
	case Heartbeat:
	case Data:
	case ReData:
		if (pCtrl->S_Seq.SNR != pCtrl->SafeRecvMsg.s)
		{
			//logprintf("SNR:0x%8x,recvseq:0x%8x\n",pCtrl->S_Seq.SNR,pCtrl->SafeRecvMsg.s);
			return e_UsrFalse;
		}
		break;
	default:
		break;
	}
	/*��������������*/
	if (pCtrl->S_Seq.SNR == 0xFFFFFFFFU)
	{
		pCtrl->S_Seq.SNR = 0;
	}
	else
	{
		pCtrl->S_Seq.SNR++;
	}
	return e_UsrTrue;
}




/** *************************************************************
������ȷ���ݲ�����
************************************************************* */
static void ProcessSafeMsg(S_ProtocolControl* pCtrl)
{
	MyUINT16 type = pCtrl->SafeRecvMsg.stype;
	switch (type)
	{
	case ConnReq:
		//logprintf("Receive ConnReq\n");
		ProcessConnSeq(pCtrl);
		break;
	case ConnResp:
		//logprintf("Receive ConnResp\n");
		ProcessConnResp(pCtrl);
		break;
	case ReReq:
		//logprintf("Receive ReReq!\n");
		pCtrl->UnConfirmedNum = 0;  
		ProcessReReq(pCtrl);
		break;
	case ReResp:
		//logprintf("Receive ReResp\n");
		pCtrl->UnConfirmedNum = 0;
		ProcessReResp(pCtrl);
		break;
	case DisconnReq:
		//logprintf("Receive Disconnect\n");
		ProcessDisReq(pCtrl);
		break;
	case Heartbeat:
		ProcessHeartbeat(pCtrl);
		ClearReTx(pCtrl);   //�յ�ȷ����Ϣ������ش�����
		break;
	case Data:
		ProcessData(pCtrl);
		ClearReTx(pCtrl);
		break;
	case ReData:
		//logprintf("Receive ReData\n");
		ProcessReData(pCtrl);
		break;
	default:
		break;
	}
	UpdateTI(pCtrl);
}

static void UpdateTI(S_ProtocolControl* pCtrl)
{
	MyUINT16 type = pCtrl->SafeRecvMsg.stype;
	if (ConnReq == type)
	{
		pCtrl->Time.Tlastrtd = 0;
	}
	else if(Data == type || Heartbeat == type || ReData == type)
	{
		if (pCtrl->Time.Tlocal < pCtrl->RightSafeRecvMsg.cts)
		{ /*����ʱ���������*/
			pCtrl->Time.Tlastrtd = (0xFFFFFFFF - pCtrl->RightSafeRecvMsg.cts) + pCtrl->Time.Tlocal +s_t;
		}
		else
		{
			pCtrl->Time.Tlastrtd = pCtrl->Time.Tlocal - pCtrl->RightSafeRecvMsg.cts + s_t;
		}
	}
	pCtrl->Time.TI = Tmax - pCtrl->Time.Tlastrtd;
}

/** *************************************************************
У�鰲ȫ����
************************************************************* */
static MyBOOL CheckSafeData(S_ProtocolControl* pCtrl)
{
	if (!CheckMesgMD4(pCtrl->SafeRecv.msg, pCtrl->SafeRecvMsg.slength))
	{
		//logprintf("Message md4 error!\n");
		return e_UsrFalse;              //��ȫ�����
	}
	if ((peeraddress != pCtrl->SafeRecvMsg.sendaddress)
		|| (selfaddress != pCtrl->SafeRecvMsg.recvaddress))
	{
		//logprintf("Message address error !\n");
		return e_UsrFalse;
	}
	if (!CheckSafeType(pCtrl))
	{
		//logprintf("Message Type  error !\n");
		return e_UsrFalse;
	}
	if (!CheckSafeSeq(pCtrl))
	{
		if(pCtrl->S_Seq.SNR <  pCtrl->SafeRecvMsg.s)
		{
			if (s_Up == pCtrl->SafeNextState || s_ReRun == pCtrl->SafeNextState )
			{
				Pack_SaharaMsg(pCtrl, ReReq);	
				SetSafeState(pCtrl, s_ReReq);		
			}
		}
		if (s_Start == pCtrl->SafeNextState)
		{
			DisconnProcess( pCtrl,1,3);
			//logprintf("Disconnect for check seq error at s_Start status !\n");
		}
		//logprintf("Message seq  error !\n");
		return e_UsrFalse;
	}
	if (!CheckVersion(pCtrl))
	{
		//logprintf("Message version  error!\n");
		DisconnProcess(pCtrl, 1, 6);
		return e_UsrFalse;
	}
	return e_UsrTrue;
}


/** *************************************************************
��ȡ�Է���������С 
************************************************************* */
static void GetPeerBuffSize(S_ProtocolControl* pCtrl)
{
	if ((!IsServer() && (ConnResp == pCtrl->SafeRecvMsg.stype)) ||
		(IsServer() && (ConnReq == pCtrl->SafeRecvMsg.stype)))
	{
		pCtrl->peerbuffsize = pCtrl->SafeRecvMsg.peerbuffsize ;
		if (pCtrl->peerbuffsize > MAX_SEND_NUM)
		{
			pCtrl->peerbuffsize = MAX_SEND_NUM;
		}
	}
}

/** *************************************************************
У�������
************************************************************* */
static void CheckRedun(S_ProtocolControl* pCtrl, const MyUINT16 inlen, MyUINT8 *inbuf)
{
	MyUINT8 Status = 0;
	MyUINT16 length = inlen;
	switch (pCtrl->RedunNextState)
	{
	case r_Init:    //��ʼ��״̬
		/*У���ʼ�����*/
		Status = CheckInitFlag(pCtrl, inlen, inbuf);
		SetRedunState(pCtrl, Status);
		break;
	case r_Open:    //�߼��ŵ�����״̬
		ReceiveData(pCtrl, inlen, inbuf); //��������
		break;
	default:    //�����ŵ��ر�״̬
		break;
	}	
}


/*������ڶ��������Ϣ�Ƿ�ʱ*/
static void DeferTmo(S_ProtocolControl* pCtrl)
{
	MyUINT8 Redunstate = pCtrl->RedunNextState;
	if (r_Open == Redunstate)
	{
		DeliverDeferQueue(pCtrl);
	}
}

/** *************************************************************
�����ڶ��з�����������Ϣ���䵽�ϲ�
************************************************************* */
static void DeliverDeferQueue(S_ProtocolControl* pCtrl)
{
	MyUINT8 i = 0;
	MyUINT32 DeferSeqNo = 0;
	for (i = 0; i < DEFERQUEUENUM; i++)
	{
		if (DiffTime(pCtrl->R_Seq.TSEQ[i]) >= SEQ && pCtrl->R_Seq.TSEQ[i] != 0)
		{
			DeferSeqNo = Buffer2MyUINT32(&pCtrl->DeferMsg[i].msg[4]);
			pCtrl->RecvMsg.dataSize = pCtrl->DeferMsg[i].msgLength - 8;
			DeliverToSafe(pCtrl, pCtrl->DeferMsg[i].msg);
			F_GL_MemSet(&pCtrl->DeferMsg[i], 0, sizeof(S_Msg));
			pCtrl->R_Seq.TSEQ[i] = 0;
			pCtrl->R_Seq.SeqRX = DeferSeqNo;
			pCtrl->R_Seq.SeqRX++;
			/*����Ƿ�ɴ������ڶ��е���Ϣ*/
			CheckDeferQueue(pCtrl);
			//logprintf("Defer Time!SeqRx:%d\n",pCtrl->R_Seq.SeqRX);
		}
	}
}

/** ************************************************************
����ʱ���
************************************************************* */
static MyUINT32 DiffTime(MyUINT32 timer)
{
	MyUINT32 ret = 0;
	if (SaharaClock < timer)
	{
		ret = 0xFFFFFFFF - timer + SaharaClock;
	}
	else
	{
		ret = SaharaClock - timer;
	}
	//logprintf("difftime:%d\n",ret);
	return ret;
}


/** *************************************************************
�жϷ����or�ͻ���
************************************************************* */
static MyBOOL IsServer()
{
	MyBOOL bRet;
	if (peeraddress > selfaddress)
	{
		bRet =  e_UsrFalse;
	}
	else
	{
		bRet = e_UsrTrue;
	}
	return bRet;
}

/** *************************************************************
�����������ݰ�
����ֵ��    ������Ϣ                        0 
            ���䵽��ȫ��                    1 
            ���䵽���ڶ���                  2   
            ���յ������û����ݵĳ�ʼ������  3
************************************************************* */
static MyUINT8 ReceiveData(S_ProtocolControl* pCtrl, const  MyUINT16 inLen, MyUINT8 *inBuf)
{
	MyUINT8 result = 0;
	MyUINT16 leftlength = 0;
	/*���յ���Ϣ��Ϊ��ʼ����Ϣ����У����������к�*/
	if (0x0001 != (pCtrl->RecvMsg.flags & 0x0001))
	{
		result = CheckRedunSeq(pCtrl, pCtrl->RecvMsg.length, inBuf);
	}

	/*���յ���ϢΪ��ʼ����Ϣ����Ϣ���Ϊ0*/
	else if(0 == pCtrl->RecvMsg.seqNo)
	{
		/*������ڶ���*/
		ClearAllDeferQueue(pCtrl);

		/*��������������źͷ������*/
		pCtrl->R_Seq.SeqRX = 0;
		pCtrl->R_Seq.SeqTX = 0;

		/*����������ż�1*/
		pCtrl->R_Seq.SeqRX++;

		/*���������ĳ�ʼ����Ϣ���Ϊ1*/
		pCtrl->SendMsg.flags = 0x0001;

		/*�رճ�ʼ����ʱ��*/
		pCtrl->IsInitTActive = e_UsrFalse;
		result = 3;

		/*�������������ڰ�ȫ�����ݣ����䵽��ȫ��*/
		if (pCtrl->RecvMsg.dataSize > 0)
		{
			DeliverToSafe(pCtrl, inBuf);
			result = 1;
		}
	}
	return result;
}

/** *************************************************************
У���ʼ�����
����ֵ�� 0 ������Ϣ
1 ���ϲ㴫����Ϣ
************************************************************* */
static MyUINT8 CheckInitFlag(S_ProtocolControl* pCtrl, const MyUINT16 inlen, MyUINT8 *inbuf)
{
	MyUINT8 ret = 0;
	if (/*(e_UsrFalse == pCtrl->RecvMsg.hashcode)
		|| */(0x0001 != (pCtrl->RecvMsg.flags & 0x0001))
		|| (pCtrl->RecvMsg.seqNo != pCtrl->R_Seq.SeqRX))
	{
		/*δ�յ���ʼ�����Ϊ1����Ϣ*/
		ret = r_Init;
	}
	else
	{
		//logprintf("Redun receive  init message at r_Init status \n");
		/*����Ϣ���䵽��ȫ��*/
		DeliverToSafe(pCtrl, inbuf);
		/*����������ż�1*/
		pCtrl->R_Seq.SeqRX++;
		/*�رճ�ʼ����ʱ��*/
		pCtrl->IsInitTActive = e_UsrFalse;
		ret = r_Open;
	}
	return ret;
}


/** *************************************************************
�򿪰�ȫ��
************************************************************* */
static void OpenSafe(S_ProtocolControl* pCtrl)
{
	if (s_Close == pCtrl->SafeNextState)
	{
		F_GL_MemSet(&pCtrl->ReTx, 0, sizeof(S_Msg));
		if (e_UsrTrue == IsServer())
		{
			SetSafeState(pCtrl, s_Down);
		}
		else
		{
			pCtrl->Time.Tlastrtd = 0;
			pCtrl->Time.TI = Tmax - pCtrl->Time.Tlastrtd;
			Pack_SaharaMsg(pCtrl, ConnReq);
			SetSafeState(pCtrl, s_Start);
		}
	}
}


/** *************************************************************
�������
************************************************************* */
static void OpenRedun(S_ProtocolControl* pCtrl)
{
	if (r_Close == pCtrl->RedunNextState)
	{
		InitRedun(pCtrl);
		SetRedunState(pCtrl, r_Init);
	}
}

/** *************************************************************
��ʼ�������
************************************************************* */
static void InitRedun(S_ProtocolControl* pCtrl)
{
	pCtrl->SendMsg.flags = 0x0001;
	pCtrl->IsInitTActive = e_UsrTrue;
	pCtrl->R_Seq.TInitResq = SaharaClock;
	pCtrl->R_Seq.SeqRX = 0;
	pCtrl->R_Seq.SeqTX = 0;
	ClearAllDeferQueue(pCtrl);
}
/** ************************************************************
������������ŵ������ڶ���
************************************************************* */
static void ClearAllDeferQueue(S_ProtocolControl* pCtrl)
{
	MyUINT8 i = 0;
	MyUINT8 j = 0;
	for (j = 0; j < DEFERQUEUENUM; j++)
	{
		F_GL_MemSet(pCtrl->DeferMsg[j].msg, 0, BUFF_SIZE);
		pCtrl->DeferMsg[j].msgLength = 0;
		pCtrl->R_Seq.TSEQ[j] = 0;        // ���ڼ�ؼ�ʱ����
	}
}



/** *************************************************************
�ر������
************************************************************* */
static void CloseRedun(S_ProtocolControl* pCtrl)
{
	MyUINT8 Redunstate = pCtrl->RedunNextState;
	if (r_Init == Redunstate || r_Open == Redunstate)
	{
		Cleanup(pCtrl);
		SetRedunState(pCtrl, r_Close);
	}
	else
	{
		SetRedunState(pCtrl, r_Close);
	}
}

/** *************************************************************
���������״̬
************************************************************* */
static void SetRedunState(S_ProtocolControl* pCtrl, MyUINT8 state)
{
	pCtrl->RedunNextState = state;
}

/** *************************************************************
���ð�ȫ��״̬
************************************************************* */
static void SetSafeState(S_ProtocolControl* pCtrl, MyUINT8 state)
{
	pCtrl->SafeNextState = state;
}



/** *************************************************************
����ض�����
************************************************************* */
static MyUINT8 Cleanup(S_ProtocolControl* pCtrl)
{
	//����ض�����
	F_GL_MemSet(&pCtrl->RecvUsrData, 0, sizeof(S_Msg));
	F_GL_MemSet(&pCtrl->SendUsrData, 0, sizeof(S_Msg));
	F_GL_MemSet(&pCtrl->SendUsrDataMem, 0, sizeof(S_Msg));
	F_GL_MemSet(&pCtrl->RedunSendMsg, 0, sizeof(S_Msg));
	return 0;
}

/** *************************************************************
���������͵����ݴ���������
************************************************************* */
static void Pack_SaharaMsg(S_ProtocolControl* pCtrl, MyUINT16 type)
{
	pCtrl->Time.Tlocal = SaharaClock;
	
	/*�����ݴ������ȫ��*/
	switch (type)
	{
	case ConnReq:
		pCtrl->Time.Tlastrtd = 0;
		pCtrl->Time.TI = Tmax - pCtrl->Time.Tlastrtd;
		FormConnSeq(pCtrl);
		pCtrl->Time.TLastRecv = pCtrl->Time.Tlocal; //���������������ʼ��TLastRecv
		//logprintf("Send ConnReq\n");
		break;
	case ConnResp:
		FormConnResp(pCtrl);
		//logprintf("Send ConnResp\n");
		break;
	case DisconnReq:
		FormDisconnReq(pCtrl);
		//logprintf("Send DisconnReq\n");
		break;
	case ReReq:
		FormReReq(pCtrl);
		//logprintf("Send ReReq\n");
		break;
	case ReResp:
		FormReResp(pCtrl);
		//logprintf("Send ReResp\n");
		break;
	case Data:
		FormData(pCtrl);
		break;
	case ReData:
		FormReData(pCtrl);
		break;
	case Heartbeat:
		FormHeartbeat(pCtrl);
		break;
	default:
		break;
	}
	pCtrl->Time.Th = SaharaClock;
	if (pCtrl->S_Seq.SNT == 0xFFFFFFFFU)
	{/*��ֹ��ȫ�㷢��������*/
		pCtrl->S_Seq.SNT = 0;
	}
	else
	{
		pCtrl->S_Seq.SNT++;
	}
	/*ÿ����һ���������͵����ݣ�δȷ�Ϸ�����Ϣ��������1*/
	pCtrl->UnConfirmedNum++;
	/*���½�����Ϣ������  = �ɵĽ�����Ϣ������  -���µķ���ȷ����� - �ɵķ���ȷ����ţ�*/
	pCtrl->RecvMsgNum -=(pCtrl->SafeSendMsg.cs - pCtrl->SafeSendMsg.cs_old);
	/*����ɵķ�����Ϣ��ȷ�����*/
	pCtrl->SafeSendMsg.cs_old = pCtrl->SafeSendMsg.cs;
	//logprintf("RecvMsgNum:%d\n",pCtrl->RecvMsgNum );
	/*����ȫ�����ݴ���������*/
	FormRedunData(pCtrl);
	if (pCtrl->R_Seq.SeqTX == 0xFFFFFFFFU)
	{ /*��ֹ����㷢��������*/
		pCtrl->R_Seq.SeqTX = 0;
	}
	else
	{
		pCtrl->R_Seq.SeqTX++;
	}
	F_GL_MemSet(&pCtrl->SafeSend, 0, sizeof(S_Msg));
}


/** ************************************************************
���ConnSeq
************************************************************* */
static void FormConnSeq(S_ProtocolControl* pCtrl)
{
	Hashcode hashcode;
	MyUINT8 *pbuf = NULL;
	pCtrl->S_Seq.SNT = 0;//GenerateSafeSeq(pCtrl);
	pCtrl->SafeSendMsg.s = pCtrl->S_Seq.SNT;
	pbuf = &pCtrl->SafeSend.msg[0];
	pCtrl->SafeSendMsg.slength = 42;
	pCtrl->SafeSendMsg.stype = ConnReq;
	pCtrl->SafeSendMsg.recvaddress = peeraddress;
	pCtrl->SafeSendMsg.sendaddress = selfaddress;
	pCtrl->SafeSendMsg.cs = 0;
	pCtrl->SafeSendMsg.ts = pCtrl->Time.Tlocal;
	pCtrl->SafeSendMsg.cts = 0;
	pCtrl->SafeSendMsg.version = VERSION;
	MyUINT162Buffer(pbuf, pCtrl->SafeSendMsg.slength);
	pbuf += 2;
	MyUINT162Buffer(pbuf, pCtrl->SafeSendMsg.stype);
	pbuf += 2;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.recvaddress);
	pbuf += 4;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.sendaddress);
	pbuf += 4;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.s);
	pbuf += 4;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.cs);
	pbuf += 4;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.ts);
	pbuf += 4;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.cts);
	pbuf += 4;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.version);
	pbuf += 4;
	MyUINT162Buffer(pbuf, SelfBuffSize);
	pbuf += 2;
	hashcode =  md4(&pCtrl->SafeSend.msg[0], pCtrl->SafeSendMsg.slength - 8);
	MyUINT322Buffer(pbuf,hashcode.High4Bytes);
	pbuf += 4;
	MyUINT322Buffer(pbuf,hashcode.Low4Bytes);
	pCtrl->SafeSend.msgLength = 42;
}

/** ************************************************************
���ConnResp
************************************************************* */
static void FormConnResp(S_ProtocolControl* pCtrl)
{
    Hashcode hashcode;
	MyUINT8* pbuf;
	pCtrl->S_Seq.SNT = 0;
	pCtrl->SafeSendMsg.s = pCtrl->S_Seq.SNT;
	pbuf = &pCtrl->SafeSend.msg[0];
	pCtrl->SafeSendMsg.slength = 42;
	pCtrl->SafeSendMsg.stype = ConnResp;
	pCtrl->SafeSendMsg.recvaddress = peeraddress;
	pCtrl->SafeSendMsg.sendaddress = selfaddress;
	pCtrl->SafeSendMsg.cs = pCtrl->RightSafeRecvMsg.s;
	pCtrl->SafeSendMsg.ts = pCtrl->Time.Tlocal;
	pCtrl->SafeSendMsg.cts = pCtrl->RightSafeRecvMsg.ts;
	pCtrl->SafeSendMsg.version = VERSION;
	MyUINT162Buffer(pbuf, pCtrl->SafeSendMsg.slength);
	pbuf += 2;
	MyUINT162Buffer(pbuf, pCtrl->SafeSendMsg.stype);
	pbuf += 2;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.recvaddress);
	pbuf += 4;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.sendaddress);
	pbuf += 4;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.s);
	pbuf += 4;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.cs);
	pbuf += 4;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.ts);
	pbuf += 4;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.cts);
	pbuf += 4;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.version);
	pbuf += 4;
	MyUINT162Buffer(pbuf, SelfBuffSize);
	pbuf += 2;
	hashcode =  md4(&pCtrl->SafeSend.msg[0], pCtrl->SafeSendMsg.slength - 8);
	MyUINT322Buffer(pbuf,hashcode.High4Bytes);
	pbuf += 4;
	MyUINT322Buffer(pbuf,hashcode.Low4Bytes);
	pCtrl->SafeSend.msgLength = 42;
}


/** ************************************************************
���ReReq
************************************************************* */
static void FormReReq(S_ProtocolControl* pCtrl)
{
	Hashcode hashcode;
	MyUINT8* pbuf;
	pCtrl->SafeSendMsg.s = pCtrl->S_Seq.SNT;
	pbuf = &pCtrl->SafeSend.msg[0];
	pCtrl->SafeSendMsg.slength = 36;
	pCtrl->SafeSendMsg.stype = ReReq;
	pCtrl->SafeSendMsg.recvaddress = peeraddress;
	pCtrl->SafeSendMsg.sendaddress = selfaddress;
	pCtrl->SafeSendMsg.cs = pCtrl->RightSafeRecvMsg.s;
	pCtrl->SafeSendMsg.ts = pCtrl->Time.Tlocal;
	pCtrl->SafeSendMsg.cts = pCtrl->RightSafeRecvMsg.ts;
	pCtrl->SafeSendMsg.version = VERSION;
	MyUINT162Buffer(pbuf, pCtrl->SafeSendMsg.slength);
	pbuf += 2;
	MyUINT162Buffer(pbuf, pCtrl->SafeSendMsg.stype);
	pbuf += 2;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.recvaddress);
	pbuf += 4;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.sendaddress);
	pbuf += 4;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.s);
	pbuf += 4;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.cs);
	pbuf += 4;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.ts);
	pbuf += 4;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.cts);
	pbuf += 4;
	hashcode =  md4(&pCtrl->SafeSend.msg[0], pCtrl->SafeSendMsg.slength - 8);
	MyUINT322Buffer(pbuf,hashcode.High4Bytes);
	pbuf += 4;
	MyUINT322Buffer(pbuf,hashcode.Low4Bytes);
	pCtrl->SafeSend.msgLength = 36;
}

/** ************************************************************
���ReResp
************************************************************* */
static void FormReResp(S_ProtocolControl* pCtrl)
{
	Hashcode hashcode;
	MyUINT8* pbuf;
	pCtrl->SafeSendMsg.s = pCtrl->S_Seq.SNT;
	pbuf = &pCtrl->SafeSend.msg[0];
	pCtrl->SafeSendMsg.slength = 36;
	pCtrl->SafeSendMsg.stype = ReResp;
	pCtrl->SafeSendMsg.recvaddress = peeraddress;
	pCtrl->SafeSendMsg.sendaddress = selfaddress;
	pCtrl->SafeSendMsg.cs = pCtrl->RightSafeRecvMsg.s;
	pCtrl->SafeSendMsg.ts = pCtrl->Time.Tlocal;
	pCtrl->SafeSendMsg.cts = pCtrl->RightSafeRecvMsg.ts;
	pCtrl->SafeSendMsg.version = VERSION;
	MyUINT162Buffer(pbuf, pCtrl->SafeSendMsg.slength);
	pbuf += 2;
	MyUINT162Buffer(pbuf, pCtrl->SafeSendMsg.stype);
	pbuf += 2;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.recvaddress);
	pbuf += 4;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.sendaddress);
	pbuf += 4;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.s);
	pbuf += 4;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.cs);
	pbuf += 4;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.ts);
	pbuf += 4;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.cts);
	pbuf += 4;
	hashcode =  md4(&pCtrl->SafeSend.msg[0], pCtrl->SafeSendMsg.slength - 8);
	MyUINT322Buffer(pbuf,hashcode.High4Bytes);
	pbuf += 4;
	MyUINT322Buffer(pbuf,hashcode.Low4Bytes);
	pCtrl->SafeSend.msgLength = 36;
}

/** ************************************************************
���Heartbeat
************************************************************* */
static void FormHeartbeat(S_ProtocolControl* pCtrl)
{
	Hashcode hashcode;
	MyUINT8* pbuf;
	pCtrl->SafeSendMsg.s = pCtrl->S_Seq.SNT;
	pbuf = &pCtrl->SafeSend.msg[0];
	pCtrl->SafeSendMsg.slength = 36;
	pCtrl->SafeSendMsg.stype = Heartbeat;
	pCtrl->SafeSendMsg.recvaddress = peeraddress;
	pCtrl->SafeSendMsg.sendaddress = selfaddress;
	pCtrl->SafeSendMsg.cs = pCtrl->RightSafeRecvMsg.s;
	pCtrl->SafeSendMsg.ts = pCtrl->Time.Tlocal;
	pCtrl->SafeSendMsg.cts = pCtrl->RightSafeRecvMsg.ts;
	MyUINT162Buffer(pbuf, pCtrl->SafeSendMsg.slength);
	pbuf += 2;
	MyUINT162Buffer(pbuf, pCtrl->SafeSendMsg.stype);
	pbuf += 2;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.recvaddress);
	pbuf += 4;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.sendaddress);
	pbuf += 4;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.s);
	pbuf += 4;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.cs);
	pbuf += 4;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.ts);
	pbuf += 4;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.cts);
	pbuf += 4;
	hashcode =  md4(&pCtrl->SafeSend.msg[0], pCtrl->SafeSendMsg.slength - 8);
	MyUINT322Buffer(pbuf,hashcode.High4Bytes);
	pbuf += 4;
	MyUINT322Buffer(pbuf,hashcode.Low4Bytes);
	pCtrl->SafeSend.msgLength = 36;
}


/** ************************************************************
���DisconnReq
************************************************************* */
static void FormDisconnReq(S_ProtocolControl* pCtrl)
{
	Hashcode hashcode;
	MyUINT8* pbuf;
	pCtrl->SafeSendMsg.s = pCtrl->S_Seq.SNT;
	pbuf = &pCtrl->SafeSend.msg[0];
	pCtrl->SafeSendMsg.slength = 40;
	pCtrl->SafeSendMsg.stype = DisconnReq;
	pCtrl->SafeSendMsg.recvaddress = peeraddress;
	pCtrl->SafeSendMsg.sendaddress = selfaddress;
	pCtrl->SafeSendMsg.cs = pCtrl->RightSafeRecvMsg.s;
	pCtrl->SafeSendMsg.ts = pCtrl->Time.Tlocal;
	pCtrl->SafeSendMsg.cts = pCtrl->RightSafeRecvMsg.ts;
	MyUINT162Buffer(pbuf, pCtrl->SafeSendMsg.slength);
	pbuf += 2;
	MyUINT162Buffer(pbuf, pCtrl->SafeSendMsg.stype);
	pbuf += 2;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.recvaddress);
	pbuf += 4;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.sendaddress);
	pbuf += 4;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.s);
	pbuf += 4;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.cs);
	pbuf += 4;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.ts);
	pbuf += 4;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.cts);
	pbuf += 4;
	MyUINT162Buffer(pbuf, pCtrl->SafeSendMsg.IsReopen);
	pbuf += 2;
	MyUINT162Buffer(pbuf, pCtrl->SafeSendMsg.Cause);
	pbuf += 2;
	hashcode =  md4(&pCtrl->SafeSend.msg[0], pCtrl->SafeSendMsg.slength - 8);
	MyUINT322Buffer(pbuf,hashcode.High4Bytes);
	pbuf += 4;
	MyUINT322Buffer(pbuf,hashcode.Low4Bytes);
	pCtrl->SafeSend.msgLength = 40;
}


/** ************************************************************
���Data
************************************************************* */
static void FormData(S_ProtocolControl* pCtrl)
{
	Hashcode hashcode;
	MyUINT8* pbuf;
	pCtrl->SafeSendMsg.s = pCtrl->S_Seq.SNT;
	pbuf = &pCtrl->SafeSend.msg[0];
	pCtrl->SafeSendMsg.slength = pCtrl->SendUsrData.msgLength + 36;
	pCtrl->SafeSendMsg.stype = Data;
	pCtrl->SafeSendMsg.recvaddress = peeraddress;
	pCtrl->SafeSendMsg.sendaddress = selfaddress;
	pCtrl->SafeSendMsg.cs = pCtrl->RightSafeRecvMsg.s;
	pCtrl->SafeSendMsg.ts = pCtrl->Time.Tlocal;
	pCtrl->SafeSendMsg.cts = pCtrl->RightSafeRecvMsg.ts;
	MyUINT162Buffer(pbuf, pCtrl->SafeSendMsg.slength);
	pbuf += 2;
	MyUINT162Buffer(pbuf, pCtrl->SafeSendMsg.stype);
	pbuf += 2;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.recvaddress);
	pbuf += 4;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.sendaddress);
	pbuf += 4;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.s);
	pbuf += 4;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.cs);
	pbuf += 4;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.ts);
	pbuf += 4;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.cts);
	pbuf += 4;
	F_GL_MemCpy(pbuf, pCtrl->SendUsrData.msg, pCtrl->SendUsrData.msgLength);
	pbuf += pCtrl->SendUsrData.msgLength;
	hashcode =  md4(&pCtrl->SafeSend.msg[0], pCtrl->SafeSendMsg.slength - 8);
	MyUINT322Buffer(pbuf,hashcode.High4Bytes);
	pbuf += 4;
	MyUINT322Buffer(pbuf,hashcode.Low4Bytes);
	if ((pCtrl->ReTx.msgLength + pCtrl->SafeSendMsg.slength) < BUFF_SIZE)
	{
		F_GL_MemCpy(&pCtrl->ReTx.msg[pCtrl->ReTx.msgLength],
			&pCtrl->SafeSend.msg[0],
			pCtrl->SafeSendMsg.slength);//���ݷ��뵽�ش�����
		pCtrl->ReTx.msgLength += pCtrl->SafeSendMsg.slength;
	}
	pCtrl->SafeSend.msgLength = pCtrl->SafeSendMsg.slength;
}


/** ************************************************************
���ReData
************************************************************* */
static void FormReData(S_ProtocolControl* pCtrl)
{
	Hashcode hashcode;
	MyUINT8* pbuf;
	pCtrl->SafeSendMsg.s = pCtrl->S_Seq.SNT;
	pbuf = &pCtrl->SafeSend.msg[0];
	pCtrl->SafeSendMsg.slength = pCtrl->ReUsrData.msgLength + 36;
	pCtrl->SafeSendMsg.stype = ReData;
	pCtrl->SafeSendMsg.recvaddress = peeraddress;
	pCtrl->SafeSendMsg.sendaddress = selfaddress;
	pCtrl->SafeSendMsg.cs = pCtrl->RightSafeRecvMsg.s;
	pCtrl->SafeSendMsg.ts = pCtrl->Time.Tlocal;
	pCtrl->SafeSendMsg.cts = pCtrl->Redata.cts;
	MyUINT162Buffer(pbuf, pCtrl->SafeSendMsg.slength);
	pbuf += 2;
	MyUINT162Buffer(pbuf, pCtrl->SafeSendMsg.stype);
	pbuf += 2;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.recvaddress);
	pbuf += 4;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.sendaddress);
	pbuf += 4;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.s);
	pbuf += 4;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.cs);
	pbuf += 4;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.ts);
	pbuf += 4;
	MyUINT322Buffer(pbuf, pCtrl->SafeSendMsg.cts);
	pbuf += 4;
	F_GL_MemCpy(pbuf, pCtrl->ReUsrData.msg, pCtrl->ReUsrData.msgLength);
	pbuf += pCtrl->ReUsrData.msgLength;
	hashcode =  md4(&pCtrl->SafeSend.msg[0], pCtrl->SafeSendMsg.slength - 8);
	MyUINT322Buffer(pbuf,hashcode.High4Bytes);
	pbuf += 4;
	MyUINT322Buffer(pbuf,hashcode.Low4Bytes);
	if ((pCtrl->ReTx.msgLength + pCtrl->SafeSendMsg.slength) < BUFF_SIZE)
	{
		F_GL_MemCpy(&pCtrl->ReTx.msg[pCtrl->ReTx.msgLength],
			&pCtrl->SafeSend.msg[0],
			pCtrl->SafeSendMsg.slength);//�ش����ݷ��뵽�ش�����
		/*���ش������е��ش���Ϣʱ�����Ϊ֮ǰ��������Ϣ��ʱ���*/
		MyUINT322Buffer(&pCtrl->ReTx.msg[pCtrl->ReTx.msgLength+20],pCtrl->Redata.ts); 
		pCtrl->ReTx.msgLength += pCtrl->SafeSendMsg.slength;
	}
	pCtrl->SafeSend.msgLength = pCtrl->SafeSendMsg.slength;
}


/** ************************************************************
����ش������жԷ��Ѿ�ȷ�Ϲ�����Ϣ
************************************************************* */
static void ClearReTx(S_ProtocolControl* pCtrl)
{
	MyUINT16 length = 0;
	MyUINT32 seq = 0;
	while (pCtrl->ReTx.msgLength > 0)
	{
		length = Buffer2MyUINT16(pCtrl->ReTx.msg);
		if (pCtrl->ReTx.msgLength < length)
		{/*��ֹ��ѭ��*/
			F_GL_MemSet(&pCtrl->ReTx, 0, sizeof(S_Msg));
			break;
		}
		seq = Buffer2MyUINT32(&pCtrl->ReTx.msg[12]);
		if (seq <= pCtrl->RightSafeRecvMsg.cs)
		{
			pCtrl->ReTx.msgLength -= length;
			F_GL_MemCpy(&pCtrl->ReTx.msg[0], &pCtrl->ReTx.msg[length],
				pCtrl->ReTx.msgLength);
		}
		else
		{
			break;
		}
	}
}

///** ************************************************************
//��ȡ����ʱ��
//************************************************************* */
//static MyUINT32 GetlocalTime()
//{
//	MyUINT32 Sec = 0;
//	MyUINT32 uSec = 0;
//	T_SysClk sysclk = {0} ;
//	T_SysClk* pSysclk = &sysclk;
//	MyUINT32 LocalTime = 0;
//	F_VCP_SystemClkGet(pSysclk);
//	Sec = pSysclk->Sec;
//	uSec = pSysclk->uSec;
//	//printf("Sec: %d,uSec:%d\n",Sec,uSec);
//	LocalTime = Sec*1000 + uSec/1000;
//	return LocalTime;
//}

//static MyUINT32 GetlocalTime()
//{
//	SYSTEMTIME currentTime;
//	MyUINT32 LocalTime = 0, Day = 0, Hour = 0, Minute = 0, Second = 0, Milliseconds = 0;
//	GetSystemTime(&currentTime);
//	Day = currentTime.wDay;
//	Hour = currentTime.wHour;
//	Minute = currentTime.wMinute;
//	Second = currentTime.wSecond;
//	Milliseconds = currentTime.wMilliseconds;
//	LocalTime = Day * 24 * 60 * 60 * 1000 + Hour * 60 * 60 * 1000 + Minute * 60 * 1000 + Second * 1000 + Milliseconds;
//	return LocalTime;
//}

/** ************************************************************
64λMD4���
************************************************************* */
static MyBOOL CheckMesgMD4(MyUINT8* dataBuf, MyUINT16 dataSize)
{
	MyBOOL ret = e_UsrFalse;
	S_Msg buff = { 0 };
	Hashcode hashcode1,hashcode2;
	MyUINT16 len = 0;
	len = dataSize - 8;
	F_GL_MemCpy(buff.msg, dataBuf, len);
	hashcode1.High4Bytes = Buffer2MyUINT32(&dataBuf[len]);
	hashcode1.Low4Bytes= Buffer2MyUINT32(&dataBuf[len+4]);
	hashcode2 = md4(buff.msg, len);
	if((hashcode2.High4Bytes == hashcode1.High4Bytes)&&(hashcode2.Low4Bytes == hashcode1.Low4Bytes))
	{
		ret = e_UsrTrue;
	}
	else
	{
		ret = e_UsrFalse;
	}
	return ret;
}


/** ************************************************************
BUFF����MyUINT32
************************************************************* */
static MyUINT32 Buffer2MyUINT32(const MyUINT8 * bufferPtr)
{
	MyUINT32 result = 0;
	result = bufferPtr[3] * 256 * 256 * 256 + bufferPtr[2] * 256 * 256 + bufferPtr[1] * 256 + bufferPtr[0];
	return result;
}
/** ************************************************************
BUFF����MyUINT16
************************************************************* */
static MyUINT16 Buffer2MyUINT16(const MyUINT8 * bufferPtr)
{
	MyUINT16 result = 0;
	result = bufferPtr[1] * 256 + bufferPtr[0];
	return result;
}




/** ************************************************************
�޷��ų���������BUFFER
************************************************************* */
static void MyUINT322Buffer(MyUINT8* pbuf, MyUINT32 dword)
{
	pbuf[0] = (dword)& 0XFF;
	pbuf[1] = (dword >> 8) & 0XFF;
	pbuf[2] = (dword >> 16) & 0XFF;
	pbuf[3] = (dword >> 24) & 0XFF;
}

/** ************************************************************
�޷��Ŷ���������BUFFER
************************************************************* */
static void MyUINT162Buffer(MyUINT8* pbuf, MyUINT16 word)
{
	pbuf[0] = (word)& 0XFF;
	pbuf[1] = (word >> 8) & 0XFF;
}

/** ************************************************************
16λCRC����
************************************************************* */
//static MyUINT16 ComputeMesgCRC(MyUINT8* Addr, MyUINT16 Len)
//{
//	MyUINT8 BitCount = 0;
//	MyUINT16 ActByte = 0;
//	MyUINT16 CompCRC = 0;
//	while (Len > 0) 
//	{
//		ActByte = (MyUINT16)(*Addr);
//		for (BitCount = 8; BitCount > 0; BitCount--)
//		{
//			if ((ActByte ^ CompCRC) & 1)
//			{
//				CompCRC = (CompCRC >> 1) ^ MSG_CRC_MASK;
//			}
//			else
//			{
//				CompCRC >>= 1;
//			}
//			ActByte >>= 1;
//		}
//		Addr++;
//		Len--;
//	}
//	return(CompCRC);
//}

/** *************************************************************
����������к�
************************************************************* */
//static MyUINT32 GenerateSafeSeq(S_ProtocolControl* pCtrl)
//{
//	MyUINT32 i;
//	srand((unsigned)time(NULL));
//	i = rand() % 100;
//	return i;
//}

/** ************************************************************
md4
************************************************************* */
static Hashcode md4(MyUINT8*key, MyUINT32 length)
{
	MyUINT8 buf[1024] = {0};
	MyUINT8 *pbuf = &buf[0];
	MyUINT16 i,j;
	MyUINT16 z= 0;
	Hashcode hashcode = {0};
	MyUINT32 len = length;
	MyUINT32 len1 ,len2;
	F_GL_MemCpy(buf,key,length);
	AA = 0X4C1987D3, BB = 0X216912FD, CC = 0XA7556F07, DD = 0X34C2716E;
	round = len / 64;
	length_pad[1] = len / 0x20000000;
	length_pad[0] = (len * 8) % 0x10000000;
	for (i = 0; i < round; i++)
	{
		for (j = 0; j < 16; j++)
		{
			X[j] = Buffer2MyUINT32(&pbuf[z]);
			z += 4;
		}
		z = 0;
		pbuf += 64;
		md4_function();
		F_GL_MemSet(X, 0, 64);
	}
	for (j = 0; j < 16; j++)
	{
		X[j] = Buffer2MyUINT32(&pbuf[z]);
		z += 4;
	}
	if(IsBigEndian())
	{
		len1 = len % 64;
		len2 = (len1/4)*4*2+3;
		((char*)X)[len2 - len1] = 128;
	}
	else
	{
		((char*)X)[len % 64] = 128;
	}
	if (len % 64>55)
	{
		md4_function();
		F_GL_MemSet(X, 0, 64);
	}
	F_GL_MemCpy(X + 14, length_pad, 8);
	md4_function();
	hashcode.High4Bytes= AA;
	hashcode.Low4Bytes= BB;
	return hashcode;
}

/** ************************************************************
md4
************************************************************* */
static void md4_function()
{
	A = AA, B = BB, C = CC, D = DD;

	FF(A, B, C, D, 0, 3);
	FF(D, A, B, C, 1, 7);
	FF(C, D, A, B, 2, 11);
	FF(B, C, D, A, 3, 19);
	FF(A, B, C, D, 4, 3);
	FF(D, A, B, C, 5, 7);
	FF(C, D, A, B, 6, 11);
	FF(B, C, D, A, 7, 19);
	FF(A, B, C, D, 8, 3);
	FF(D, A, B, C, 9, 7);
	FF(C, D, A, B, 10, 11);
	FF(B, C, D, A, 11, 19);
	FF(A, B, C, D, 12, 3);
	FF(D, A, B, C, 13, 7);
	FF(C, D, A, B, 14, 11);
	FF(B, C, D, A, 15, 19);

	GG(A, B, C, D, 0, 3);
	GG(D, A, B, C, 4, 5);
	GG(C, D, A, B, 8, 9);
	GG(B, C, D, A, 12, 13);
	GG(A, B, C, D, 1, 3);
	GG(D, A, B, C, 5, 5);
	GG(C, D, A, B, 9, 9);
	GG(B, C, D, A, 13, 13);
	GG(A, B, C, D, 2, 3);
	GG(D, A, B, C, 6, 5);
	GG(C, D, A, B, 10, 9);
	GG(B, C, D, A, 14, 13);
	GG(A, B, C, D, 3, 3);
	GG(D, A, B, C, 7, 5);
	GG(C, D, A, B, 11, 9);
	GG(B, C, D, A, 15, 13);

	HH(A, B, C, D, 0, 3);
	HH(D, A, B, C, 8, 9);
	HH(C, D, A, B, 4, 11);
	HH(B, C, D, A, 12, 15);
	HH(A, B, C, D, 2, 3);
	HH(D, A, B, C, 10, 9);
	HH(C, D, A, B, 6, 11);
	HH(B, C, D, A, 14, 15);
	HH(A, B, C, D, 1, 3);
	HH(D, A, B, C, 9, 9);
	HH(C, D, A, B, 5, 11);
	HH(B, C, D, A, 13, 15);
	HH(A, B, C, D, 3, 3);
	HH(D, A, B, C, 11, 9);
	HH(C, D, A, B, 7, 11);
	HH(B, C, D, A, 15, 15);

	AA += A, BB += B, CC += C, DD += D;
}

/** ************************************************************
У����������
************************************************************* */
static void ProcessConnSeq(S_ProtocolControl* pCtrl)
{
	MyUINT8 safestate = pCtrl->SafeNextState;
	if (s_Down == safestate)
	{
		Pack_SaharaMsg(pCtrl, ConnResp);
		SetSafeState(pCtrl, s_Start);
		pCtrl->RecvMsgNum = 0;
		pCtrl->SafeSendMsg.cs_old = pCtrl->RightSafeRecvMsg.s;
	}
	if (s_Close != safestate && s_Down != safestate && s_Start != safestate)
	{
		DisconnProcess(pCtrl, 1, 2);
	}
}


/** ************************************************************
У��������Ӧ
************************************************************* */
static void ProcessConnResp(S_ProtocolControl* pCtrl)
{
	MyUINT8 safestate = pCtrl->SafeNextState;
	if (s_Start == safestate)
	{
		if (IsServer())
		{
			DisconnProcess(pCtrl, 1, 1);
			//printf(" ConnResp err! \n");
		}
		else
		{
			Pack_SaharaMsg(pCtrl, Heartbeat);
			SetSafeState(pCtrl, s_Up);
		}
	}
	if (s_Up == safestate || s_ReReq == safestate || s_ReRun == safestate)
	{
		DisconnProcess(pCtrl, 1, 22);
	//	printf("66\n");
	}
}


/** ************************************************************
У���ش���Ӧ
************************************************************* */
static void ProcessReResp(S_ProtocolControl* pCtrl)
{
	MyUINT8 safestate = pCtrl->SafeNextState;
	if (s_Start == safestate || s_Up == safestate || s_ReRun == safestate)
	{
		DisconnProcess(pCtrl, 1, 23);
	//	printf("77\n");
	}
	if (s_ReReq == safestate)
	{
		SetSafeState(pCtrl, s_ReRun);
		//logprintf("recv ReResp!\n");
	}
}




/** ************************************************************
У���ش�����
************************************************************* */
static void ProcessReReq(S_ProtocolControl* pCtrl)
{
	MyUINT8 safestate = pCtrl->SafeNextState;
	MyUINT16 ReTxlength = 0;	
	if (s_Start == safestate || s_ReRun == safestate)
	{
		DisconnProcess(pCtrl, 1, 24);
	//	printf("88\n");
	}
	if (s_Up == safestate || s_ReReq == safestate)
	{
		if (1 == CheckReTx(pCtrl))/*���ش�����Ϣ��ʱ*/
		{
			DisconnProcess(pCtrl, 1, 8);
			//printf("99\n");
		}
		else
		{
			if (pCtrl->ReTx1.msgLength > 0)   //�ش����������������Ϣ
			{
				Pack_SaharaMsg(pCtrl, ReResp);
				while(pCtrl->ReTx1.msgLength >0)
				{
					ReTxlength = Buffer2MyUINT16(pCtrl->ReTx1.msg);
					if (pCtrl->ReTx1.msgLength < ReTxlength)
					{/*��ֹ��ѭ��*/
						F_GL_MemSet(&pCtrl->ReTx1, 0, sizeof(S_Msg));
						break;
					}
					/*�����ش���Ϣ��ȷ��ʱ���*/
					pCtrl->Redata.ts = Buffer2MyUINT32(&pCtrl->ReTx1.msg[20]);
					pCtrl->Redata.cts = Buffer2MyUINT32(&pCtrl->ReTx1.msg[24]);
					pCtrl->ReUsrData.msgLength = ReTxlength - 36;
					F_GL_MemCpy(pCtrl->ReUsrData.msg,&pCtrl->ReTx1.msg[28],pCtrl->ReUsrData.msgLength);
					Pack_SaharaMsg(pCtrl, ReData);
					pCtrl->ReTx1.msgLength -= ReTxlength;
					F_GL_MemCpy(pCtrl->ReTx1.msg,&pCtrl->ReTx1.msg[ReTxlength],pCtrl->ReTx1.msgLength);
				}
				Pack_SaharaMsg(pCtrl, Heartbeat);
				F_GL_MemSet(&pCtrl->ReTx1, 0, sizeof(S_Msg));
			}
			else
			{
				 //�ش�ʧ�ܣ���Ϣ�����ش�������
				/* printf("send all elements!\n");
				F_GL_MemSet(&pCtrl->ReTx, 0, sizeof(S_Msg));
				F_GL_MemSet(&pCtrl->ReTx1, 0, sizeof(S_Msg));
				Pack_SaharaMsg(pCtrl, ReResp);
				SendReData_AllElments( pCtrl);
				Pack_SaharaMsg(pCtrl, Heartbeat);*/
				DisconnProcess(pCtrl,1,7);
				//printf("not find redata\n");
			}
		}
	}
}



/** ************************************************************
�Ͽ����Ӵ���
************************************************************* */
static void DisconnProcess(S_ProtocolControl* pCtrl,MyUINT16 Isreopen,MyUINT16 cause)
{
	pCtrl->SafeSendMsg.IsReopen = Isreopen;
	pCtrl->SafeSendMsg.Cause = cause;     
	SetSafeState(pCtrl, s_Close);
	CloseRedun(pCtrl);
	Pack_SaharaMsg(pCtrl, DisconnReq);
	if (Isreopen == 1)
	{
		OpenRedun(pCtrl);
		OpenSafe(pCtrl);
	}
}

/** ************************************************************
У������
************************************************************* */
static void ProcessHeartbeat(S_ProtocolControl* pCtrl)
{
	MyUINT8 safestate = pCtrl->SafeNextState;
	if (s_Start == safestate)
	{
		if (IsServer())
		{
			SetSafeState(pCtrl, s_Up);
		}
		else
		{
			DisconnProcess(pCtrl, 1, 1);
		//	printf("BB\n");
		}
	}
	if (s_ReRun == safestate)
	{
		SetSafeState(pCtrl, s_Up);
	}
}

/** ************************************************************
����Ͽ�����
************************************************************* */
static void ProcessDisReq(S_ProtocolControl* pCtrl)
{
	MyUINT8 safestate = pCtrl->SafeNextState;
	if (s_Down != safestate)
	{
		SetSafeState(pCtrl, s_Close);
		//printf("CC\n");
		F_GL_MemSet(&pCtrl->RedunSendMsg,0,sizeof(S_Msg));
		if (pCtrl->SafeRecvMsg.IsReopen == 1)
		{
			OpenSafe(pCtrl);
		}
		else
		{
		//	printf("peer do not let reopen!\n");
		}
	}
}

/** ************************************************************
�����������к�
************************************************************* */
static MyUINT8 CheckRedunSeq(S_ProtocolControl* pCtrl, MyUINT16 inLen, MyUINT8 *inBuf)
{
	MyUINT8 ret = 0;
	/*��Ϣ���С��Ԥ�ڽ������,�����ø���Ϣ*/
	if (pCtrl->RecvMsg.seqNo < pCtrl->R_Seq.SeqRX)
	{
		ret = 0;
	}
	/*��Ϣ��ŵ���Ԥ�ڽ������,�򽫸���Ϣ������ȫ��*/
	else if (pCtrl->RecvMsg.seqNo == pCtrl->R_Seq.SeqRX)
	{
		DeliverToSafe(pCtrl, inBuf);
		if (pCtrl->R_Seq.SeqRX == 0xFFFFFFFF)
		{
			pCtrl->R_Seq.SeqRX = 0;
		}
		else
		{
			pCtrl->R_Seq.SeqRX++;
		}
		/*����Ƿ�ɴ������ڶ��е���Ϣ*/
		CheckDeferQueue(pCtrl);
		ret = 1;
	}
	/*��Ϣ��Ŵ���Ԥ�ڽ�����ţ�����Ϣ�������ڶ���*/
	else 
	{
		StoreInDeferQueue(pCtrl, inLen, inBuf);
		ret = 2;
	}
	return ret;
}


/** ************************************************************
����ش������Ƿ����ش���Ϣ
����ֵ��0 ���ش���Ϣ
        1 �ش���Ϣ��ʱ
        2 ���ش���Ϣ
************************************************************* */
static MyUINT8 CheckReTx(S_ProtocolControl* pCtrl)
{
	MyUINT16 ReTxlength = 0;
	MyUINT32 ReTxSeq = 0;
	MyUINT32 ts = 0;
	MyUINT32 Seq = 0;
	MyUINT8 ret = 0;
	
	Seq = pCtrl->SafeRecvMsg.cs ;
	while (pCtrl->ReTx.msgLength > 0)
	{
		ReTxlength = Buffer2MyUINT16(pCtrl->ReTx.msg);
		if (pCtrl->ReTx.msgLength < ReTxlength)
		{/*��ֹ��ѭ��*/
			F_GL_MemSet(&pCtrl->ReTx,0,sizeof(S_Msg));
			break;
		}
		ReTxSeq = Buffer2MyUINT32(&pCtrl->ReTx.msg[12]);
		ts = Buffer2MyUINT32(&pCtrl->ReTx.msg[20]);
		/*�ش������ȷ�����С���ش���������Ϣ����ţ����ҵ��ش���Ϣ*/
		if (Seq < ReTxSeq)
		{
			/*�ж��ش���Ϣ�Ƿ�ʱ*/
			if ((pCtrl->Time.Tlocal - ts) > Tmax)
			{
				ret = 1;
				F_GL_MemSet(&pCtrl->ReTx1,0,sizeof(S_Msg));
				break; 
			}
			F_GL_MemCpy(&pCtrl->ReTx1.msg[pCtrl->ReTx1.msgLength], pCtrl->ReTx.msg,ReTxlength);
			pCtrl->ReTx1.msgLength += ReTxlength;
			ret = 2; //  �ش����������������Ϣ
		}
		pCtrl->ReTx.msgLength-= ReTxlength;
		F_GL_MemCpy(pCtrl->ReTx.msg, &pCtrl->ReTx.msg[ReTxlength], pCtrl->ReTx.msgLength);
	}
	return ret; // �ش����������������Ϣ
}



/** ************************************************************
ȷ���Ƿ�ɴ������ڶ��з�������Ϣ��
�������һ���ı�����ô˺���
************************************************************* */
static void CheckDeferQueue(S_ProtocolControl* pCtrl)
{
	MyBOOL IsSeqRxChanged = e_UsrTrue;  // �˱�־���ж�������������Ƿ�ı�
	MyUINT8 i = 0;
	MyUINT32 DeferSeqNo = 0;
	/*�˴����whileѭ��Ϊ��ֹ���ڶ����е���Ϣδ��˳�����У�
	   ��ѭ��������������������Ϣȡ��û��Ϊֹ*/
	while (e_UsrTrue == IsSeqRxChanged)
	{
		IsSeqRxChanged = e_UsrFalse;
		for (i = 0; i<DEFERQUEUENUM; i++)
		{
			DeferSeqNo = Buffer2MyUINT32(&pCtrl->DeferMsg[i].msg[4]);
			/*���ڶ����д��ڷ�����������Ϣ*/
			if ((DeferSeqNo == pCtrl->R_Seq.SeqRX) && (DeferSeqNo != 0))
			{
				/*���䵽��ȫ��*/
				pCtrl->RecvMsg.dataSize = pCtrl->DeferMsg[i].msgLength - 8;
				DeliverToSafe(pCtrl, pCtrl->DeferMsg[i].msg);
				/*���������Ԥ�ڽ������*/
				pCtrl->R_Seq.SeqRX++;
				/*��մ����ڶ���*/
				F_GL_MemSet(&pCtrl->DeferMsg[i], 0, sizeof(S_Msg));
				/*���ô����ڶ��еļ�ʱ��*/
				pCtrl->R_Seq.TSEQ[i] = 0;
				IsSeqRxChanged = e_UsrTrue;
			}
		}
	}
}


/** *************************************************************
ʧ����Ϣ���浽���ڶ���
************************************************************* */
static MyUINT8 StoreInDeferQueue(S_ProtocolControl* pCtrl, MyUINT16 inLen, MyUINT8 *inBuf)
{
	//logprintf("store in deferqueue\n");
	if (pCtrl->DeferNum < DEFERQUEUENUM)
	{
		F_GL_MemCpy(pCtrl->DeferMsg[pCtrl->DeferNum].msg, inBuf, inLen);//����Ϣ�洢�����ڶ�����
		pCtrl->DeferMsg[pCtrl->DeferNum].msgLength = inLen;
		pCtrl->R_Seq.TSEQ[pCtrl->DeferNum] = SaharaClock;
	}
	else
	{
		pCtrl->DeferNum = 0;      //�������򸲸��������Ϣ   
		F_GL_MemCpy(pCtrl->DeferMsg[pCtrl->DeferNum].msg, inBuf, inLen);//����Ϣ�洢�����ڶ�����
		pCtrl->DeferMsg[pCtrl->DeferNum].msgLength = inLen;
		//pCtrl->R_Seq.TSEQ[pCtrl->DeferNum] = SaharaClock;
	}
	pCtrl->DeferNum++;
	return 1;
}


/** ************************************************************
У���ش�����
************************************************************* */
static void ProcessReData(S_ProtocolControl* pCtrl)
{
	MyUINT8 safestate = pCtrl->SafeNextState;
	if (s_Start == safestate || s_Up == safestate || s_ReReq == safestate)
	{
		DisconnProcess(pCtrl, 1, 1);
	//	printf("DD\n");
	}
	if (s_ReRun == safestate)
	{
		DeliverToUser(pCtrl);
	}
}

/** ************************************************************
У������
************************************************************* */
static void ProcessData(S_ProtocolControl* pCtrl)
{
	MyUINT8 safestate = pCtrl->SafeNextState;
	if (s_Start == safestate)
	{
		DisconnProcess(pCtrl, 1, 1);
	//	printf("EE\n");
	}
	else
	{
		if (s_Up == safestate || s_ReRun == safestate)
		{
			DeliverToUser(pCtrl);
			SetSafeState(pCtrl, s_Up);
		}
	}
}

/** ************************************************************
�ж��Ƿ���
************************************************************* */
static MyBOOL IsBigEndian()
{
	union n ss;
	ss.a= 1;
	if(ss.b != 0)
	{
		return e_UsrFalse;
	}
	else
	{
		return e_UsrTrue;
	}
}




