/******************************************************************************
** Copyright (c)2017�����г�ʱ��ͨ���ź����޹�˾
** �ļ���:  sahara.c  
** �� ��:   sahara��ȫͨ��Э��
**
**------------------------------������汾�޸ļ�¼-------------------------------
**  V1.0.0     ����: 2017.7.10   �޸���: ����
**  �ļ����ݣ���SAHARA Version 2_2 en_PM2_A6Z00032113503_-_000.pdf��
**  �޸�����:   ����
*******************************************************************************/

/**------------------------------�����޸ļ�¼-----------------------------------
**  V1.0.1  ����:                 �޸���:
**  �޸�����:
*******************************************************************************/
#include"sahara.h"

#include <stdlib.h>

static void OpenSafe(S_SaharaPCtrl* pCtrl); 
static void OpenRedun(S_SaharaPCtrl* pCtrl);
static void CloseRedun(S_SaharaPCtrl* pCtrl);
static void Pack_SaharaMsg(S_SaharaPCtrl* pCtrl, MyUINT16 type);
static void FormConnSeq(S_SaharaPCtrl* pCtrl);
static void FormConnResp(S_SaharaPCtrl * pCtrl);
static void FormReReq(S_SaharaPCtrl * pCtrl);
static void FormReResp(S_SaharaPCtrl * pCtrl);
static void FormHeartbeat(S_SaharaPCtrl * pCtrl);
static void FormData(S_SaharaPCtrl * pCtrl);
static void FormDisconnReq(S_SaharaPCtrl * pCtrl);
static void FormReData(S_SaharaPCtrl * pCtrl);
static void Cleanup(S_SaharaPCtrl* pCtrl);
static void SetSafeState(S_SaharaPCtrl* pCtrl, MyUINT8 state);
static void SetRedunState(S_SaharaPCtrl* pCtrl, MyUINT8 state);
static void MyUINT322Buffer(MyUINT8 * pbuf, MyUINT32 dword);
static void MyUINT162Buffer(MyUINT8* pbuf, MyUINT16 word);
static MyUINT16 Buffer2MyUINT16(const MyUINT8 * bufferPtr);
static MyUINT32 Buffer2MyUINT32(const MyUINT8 * bufferPtr);
static void md4_function();
static Hashcode md4(MyUINT8*key, MyUINT32 length);
static MyBOOL IsServer();
static void InitTmo(S_SaharaPCtrl* pCtrl);
static void ClearAllDeferQueue(S_SaharaPCtrl* pCtrl);
static void CheckRedun(S_SaharaPCtrl* pCtrl, const MyUINT8 *inbuf);
static MyUINT8 CheckInitFlag(S_SaharaPCtrl* pCtrl, const MyUINT8 *inbuf);
static MyUINT8 ReceiveData(S_SaharaPCtrl* pCtrl,  const MyUINT8 *inBuf);
static void DeliverDeferQueue(S_SaharaPCtrl* pCtrl);
static void DeferTmo(S_SaharaPCtrl* pCtrl);
static MyUINT32 DiffTime(MyUINT32 timer);
static void CheckSafe(S_SaharaPCtrl* pCtrl, MyUINT16 *inlen, MyUINT8 *inbuf);
static MyBOOL CheckSafeData(S_SaharaPCtrl* pCtrl);
static void GetPeerBuffSize(S_SaharaPCtrl* pCtrl);
static void ProcessSafeMsg(S_SaharaPCtrl* pCtrl);
static void UpdateTI(S_SaharaPCtrl * pCtrl);
static void ClearReTx(S_SaharaPCtrl* pCtrl);
static void PackSendData(S_SaharaPCtrl* pCtrl);
static void ProcessDisReq(S_SaharaPCtrl* pCtrl);
static void ProcessReData(S_SaharaPCtrl* pCtrl);
static void ProcessReResp(S_SaharaPCtrl* pCtrl);
static void ProcessReReq(S_SaharaPCtrl* pCtrl);
static void ProcessConnResp(S_SaharaPCtrl* pCtrl);
static void ProcessHeartbeat(S_SaharaPCtrl* pCtrl);
static void ProcessData(S_SaharaPCtrl* pCtrl);
static void ProcessConnReq(S_SaharaPCtrl* pCtrl);
static void FormRedunData(S_SaharaPCtrl* pCtrl);
static MyUINT16 PartitionSafeData(S_SaharaPCtrl* pCtrl);
static MyBOOL CheckSafeType(const S_SaharaPCtrl* pCtrl);
static MyBOOL CheckTime(const S_SaharaPCtrl* pCtrl);
static void DeliverToSafe(S_SaharaPCtrl* pCtrl, const MyUINT8 *inBuf);
static void DeliverToUser(S_SaharaPCtrl* pCtrl);
static void PartitionRedunData(S_SaharaPCtrl* pCtrl, const MyUINT8 *inBuf);
static MyUINT8 CheckReTx(S_SaharaPCtrl* pCtrl);
static MyUINT8 CheckRedunSeq(S_SaharaPCtrl* pCtrl, MyUINT16 inLen, const MyUINT8 *inBuf);
static MyBOOL CheckSafeSeq(S_SaharaPCtrl* pCtrl);
static MyBOOL CheckVersion(const S_SaharaPCtrl* pCtrl);
static void StoreInDeferQueue(S_SaharaPCtrl* pCtrl, MyUINT16 inLen, const MyUINT8 *inBuf);
static void CheckDeferQueue(S_SaharaPCtrl* pCtrl);
static MyUINT8 PackUsrData(S_SaharaPCtrl* pCtrl);
static MyBOOL CheckMesgMD4(MyUINT8* dataBuf, MyUINT16 dataSize);
static MyBOOL IsBigEndian();
static S_SaharaPCtrl* GetpCtrl(const MyUINT16 inDstDevId);
static void Sahara_Start(S_SaharaPCtrl* pCtrl);
static void Sahara_End(S_SaharaPCtrl *pCtrl);
static void DisconnProcess(S_SaharaPCtrl* pCtrl, MyUINT16 Isreopen, MyUINT16 cause);
static void InitRedun(S_SaharaPCtrl* pCtrl);
static void  Sahara_PackSyncData(const S_SaharaPCtrl *pCtrl);
static void  Sahara_UnpackSyncData(S_SaharaPCtrl *pCtrl);
static void ComputeUnConfirmedNum(S_SaharaPCtrl* pCtrl);
static void ComputeRecvNum(S_SaharaPCtrl* pCtrl);
static void AddUnConfirmedNum(S_SaharaPCtrl* pCtrl);
static void AddRecvNum(S_SaharaPCtrl* pCtrl);

static MyUINT32 SaharaClock; // ��������
static MyUINT32  selfaddress = 0x05e10000u;
static MyUINT32  peeraddress = 0x05ff0000u;
static S_SaharaPCtrl linkList[RealLinkNum];
/*md4��ر���*/
static MyUINT32 X[64], length_pad[2];
static MyUINT32 AA, BB, CC, DD, A, B, C, D;
static MyUINT32 round;


/*****************************************************************************
** ������:  F_Sahara_GetStatus
** ����:    ��ȡ��·״̬
** ����:
**    MyUINT16 inDstDevId   ȡֵ��Χ��[0��RealLinkNum]
** ����ֵ:
**   e_UsrTrue  ��·����
**   e_UsrFalse ��·�Ͽ�
** ����������:��
** ����:    ��
*****************************************************************************/
MyBOOL F_Sahara_GetStatus(const MyUINT16 inDstDevId)
{
	MyBOOL ret = e_UsrFalse;
	S_SaharaPCtrl *pCtrl = GetpCtrl(inDstDevId);
	if (pCtrl != NULL)
	{
		if ((MyUINT8)s_Up == pCtrl->SafeNextState || (MyUINT8)s_ReReq == pCtrl->SafeNextState || (MyUINT8)s_ReRun == pCtrl->SafeNextState)
		{
			ret = e_UsrTrue;
		}
		else
		{
			ret = e_UsrFalse;
		}
	}
	return ret;
}

/*****************************************************************************
** ������:  F_Sahara_Init
** ����:    Э���ʼ��
** ����:
**    MyUINT16 inDstDevId[in]   ȡֵ��Χ��[0��RealLinkNum]
** ����ֵ:
**   ��
** ����������: ��
** ����:    ��
*****************************************************************************/
void F_Sahara_Init(const MyUINT16 inDstDevId)
{
	S_SaharaPCtrl *pCtrl = GetpCtrl(inDstDevId);
	if (pCtrl != NULL)
	{
		F_GL_MemSet(pCtrl, 0, sizeof(S_SaharaPCtrl));
		SetRedunState(pCtrl, r_Close);
		SetSafeState(pCtrl, s_Close);
		pCtrl->IsInitTActive = e_UsrFalse;
		SaharaClock = 0u;
	}
}

/*****************************************************************************
** ������:  F_Sahara_UpdateClock
** ����:    ����Э��ʱ��
** ����:
**   ��
** ����ֵ:
**   ��
** ����������:SaharaClock  ȡֵ��Χ��[0,0xFFFFFFFFU]
** ����:    ��
*****************************************************************************/
void F_Sahara_UpdateClock()
{
	if (SaharaClock == 0xFFFFFFFFU)
	{
		SaharaClock = 0u;
	}
	else
	{
		SaharaClock++;
	}
}
/*****************************************************************************
** ������:  F_Sahara_CheckData
** ����:    У���������
** ����:
**  MyUINT16 inDstDevId ��·ID  ȡֵ��Χ��[0,ReadlinkNum]
**  MyUINT16 inlen �������ݳ��� ȡֵ��Χ��[0,BUFF_SIZE]
**  MyUINT8 *inbuf �������ݻ���
**	MyUINT16 *outlen У��������ݳ��� [0,BUFF_SIZE]
**  MyUINT8 *outbuf  У��������ݻ���
** ����ֵ:
**   ��
** ����������: ��
** ����:    ��
*****************************************************************************/
void F_Sahara_CheckData(const MyUINT16 inDstDevId, const MyUINT16 inlen, MyUINT8 *inbuf,
	MyUINT16 *outlen, MyUINT8 *outbuf)
{
	S_SaharaPCtrl *pCtrl = GetpCtrl(inDstDevId);
	if (pCtrl != NULL)
	{
		Sahara_UnpackSyncData(pCtrl);
		/*if (E_HOST == F_SYNC_HostOrStandby() && inlen <= BUFF_SIZE)
		{*/
			pCtrl->RedunRecvMsg.msgLength = inlen;
			F_GL_MemCpy(pCtrl->RedunRecvMsg.msg, inbuf, pCtrl->RedunRecvMsg.msgLength);
			Sahara_Start(pCtrl);
			/*���ڶ��г�ʱ���*/
			DeferTmo(pCtrl);
			printf("recv len:%d\n", pCtrl->RedunRecvMsg.msgLength);
			while (pCtrl->RedunRecvMsg.msgLength > 0u)
			{
				PartitionRedunData(pCtrl, pCtrl->RedunRecvMsg.msg);
				if (pCtrl->RedunRecvMsg.msgLength >= pCtrl->RecvMsg.length
					&& pCtrl->RecvMsg.length > 0u)
				{
					/*У�������*/
					CheckRedun(pCtrl, pCtrl->RedunRecvMsg.msg);
				}
				else
				{   /*���ݳ��ȴ����򲻴���ʣ������*/
					F_GL_MemSet(&pCtrl->RedunRecvMsg, 0, sizeof(S_Buf));
				}
				pCtrl->RedunRecvMsg.msgLength -= pCtrl->RecvMsg.length;
				F_GL_MemCpy(&pCtrl->RedunRecvMsg.msg[0], &pCtrl->RedunRecvMsg.msg[pCtrl->RecvMsg.length],
					pCtrl->RedunRecvMsg.msgLength);
				F_GL_MemSet(&pCtrl->RecvMsg, 0, sizeof(S_MsgRedun));
			}
			F_GL_MemSet(&pCtrl->RedunRecvMsg, 0, sizeof(S_Buf));

			/*У�鰲ȫ��*/
			CheckSafe(pCtrl, outlen, outbuf);

			F_GL_MemSet(&pCtrl->SafeRecv, 0, sizeof(S_Buf));
			F_GL_MemSet(&pCtrl->RecvUsrData, 0, sizeof(S_Buf));
		}/*��������*/
	/*}*/
}
/*****************************************************************************
** ������:  F_Sahara_SendData
** ����:    ����sahara���ݰ�
** ����:
**  MyUINT16 inDstDevId ��·ID  ȡֵ��Χ��[0,ReadlinkNum]
**  MyUINT16 inlen �û����ݳ��� ȡֵ��Χ��[0,BUFF_SIZE]
**  MyUINT8 *inbuf �û����ݻ���
** ����ֵ:
**   0 �����ͻ�����������ܷ����û�����
**   1 ����������
**   2 : ��ȡЭ��ջָ��ʧ��
** ����������: ��
** ����:    ��
*****************************************************************************/
MyUINT16 F_Sahara_SendData(const MyUINT16 inDstDevId, const MyUINT16 inlen, MyUINT8 *inbuf, MyUINT16* outlen, MyUINT8* outbuff)
{
	S_SaharaPCtrl *pCtrl = GetpCtrl(inDstDevId);
	S_Buf outBuffer[10] = { 0 };
	MyUINT8 result = 0u;
	MyUINT8 PipeId1 = 0u;
	MyUINT8 msgnum = 0u;
	S_Buf sendbuff = { 0 };
	MyUINT8 index = 0u;
	MyUINT16 length;
	length = 0u;
	if (pCtrl != NULL)
	{
		if ((pCtrl->SendUsrDataMem.msgLength + inlen) < (MyUINT16)BUFF_SIZE)
		{
			F_GL_MemCpy(&pCtrl->SendUsrDataMem.msg[pCtrl->SendUsrDataMem.msgLength], inbuf, inlen);
			pCtrl->SendUsrDataMem.msgLength += inlen;
			result = 1u;
		}
		else
		{
			F_GL_MemSet(&pCtrl->SendUsrDataMem, 0, sizeof(S_Buf));
			result = 0u;
		}
		/*�����������*/
		PackSendData(pCtrl);
		/*��ʼ����ʱ���*/
		InitTmo(pCtrl);
		//printf("Sahara send len:%d bytes\n",pCtrl->RedunSendMsg.msgLength);
		if ((MyUINT8)r_Init == pCtrl->RedunNextState || (MyUINT8)r_Open == pCtrl->RedunNextState)
		{
			if (pCtrl->RedunSendMsg.msgLength > 0)
			{
				F_GL_MemCpy(outbuff, &pCtrl->RedunSendMsg.msg, pCtrl->RedunSendMsg.msgLength);
				*outlen = pCtrl->RedunSendMsg.msgLength;
			}
			F_GL_MemSet(&pCtrl->RedunSendMsg, 0, sizeof(S_Buf));
		}
	}
		Sahara_End(pCtrl);
		return result;
}

/*****************************************************************************
** ������:  GetpCtrl
** ����:    ��ȡЭ��ջָ��
** ����:
**    MyUINT16 inDstDevId   ȡֵ��Χ��[0��RealLinkNum]
** ����ֵ:
**     NULL  ��ȡָ��ʧ��
**     &linkList[inDstDevId] �ɹ���ȡ��ָ��
** ����������:��
** ����:    ��
*****************************************************************************/
static S_SaharaPCtrl* GetpCtrl(const MyUINT16 inDstDevId)
{
	S_SaharaPCtrl* pCtrl = NULL;
	if (inDstDevId >= RealLinkNum )
	{
		F_GL_MemSet(linkList, 0, sizeof(S_SaharaPCtrl)*RealLinkNum);
	}
	else
	{
		pCtrl = &linkList[inDstDevId];
	}
	return pCtrl;
}
/*****************************************************************************
** ������:  Sahara_UnpackSyncData
** ����:    ���sahara ͬ����������
** ����:
**   S_SaharaPCtrl *pCtrl[out]
** ����ֵ:
**   ��
** ����������: SyncInputMsg.SaharaInputMsg[in]:Э��ͬ��������
**				 S_SaharaPCtrl *pCtrl[out]��Э��ջָ��
** ����:    ��
*****************************************************************************/
static void  Sahara_UnpackSyncData(S_SaharaPCtrl *pCtrl)
{
	/*S_SyncInputMsg* SyncInputMsg = NULL;
	MyBOOL IsRecvAfterSwitchHost = F_SYNC_GetIsRecvAfterSwitchHost();
	SyncInputMsg = F_SYNC_GetInputMsg();
	if(E_STANDBY == F_SYNC_HostOrStandby()||e_UsrTrue == IsRecvAfterSwitchHost)
	{
		pCtrl->RedunNextState =SyncInputMsg->SaharaInputMsg.RedunNextState;
		pCtrl->R_Seq.SeqTX = SyncInputMsg->SaharaInputMsg.SeqTX;
		pCtrl->R_Seq.SeqRX = SyncInputMsg->SaharaInputMsg.SeqRX;
		pCtrl->IsInitTActive = SyncInputMsg->SaharaInputMsg.IsInitTActive;
		pCtrl->SafeNextState = SyncInputMsg->SaharaInputMsg.SafeNextState;
		pCtrl->Time.TLastRecv = SyncInputMsg->SaharaInputMsg.TLastRecv ;
		pCtrl->Time.TI = SyncInputMsg->SaharaInputMsg.TI;
		pCtrl->Time.Tlastrtd= SyncInputMsg->SaharaInputMsg.Tlastrtd;
		pCtrl->Time.Tstart= SyncInputMsg->SaharaInputMsg.Tstart;
		SaharaClock = SyncInputMsg->SaharaInputMsg.SaharaClock;
		pCtrl->S_Seq.SNT = SyncInputMsg->SaharaInputMsg.SNT ;
		pCtrl->S_Seq.SNR = SyncInputMsg->SaharaInputMsg.SNR ;
		pCtrl->RightSafeRecvMsg.s= SyncInputMsg->SaharaInputMsg.recv_s;
		pCtrl->RightSafeRecvMsg.cs= SyncInputMsg->SaharaInputMsg.recv_cs ;
		pCtrl->RightSafeRecvMsg.ts = SyncInputMsg->SaharaInputMsg.recv_ts  ;
		pCtrl->RightSafeRecvMsg.cts = SyncInputMsg->SaharaInputMsg.recv_cts;
		pCtrl->SafeSendMsg.cs_old =SyncInputMsg->SaharaInputMsg.send_cs_old ;
		pCtrl->SafeSendMsg.s = SyncInputMsg->SaharaInputMsg.send_s;
		printf("R_state:%d,S_state:%d,SeqTX:%d,SeqRx:%d\n",pCtrl->RedunNextState,pCtrl->SafeNextState,pCtrl->R_Seq.SeqTX,pCtrl->R_Seq.SeqRX);
		if(e_UsrTrue == IsRecvAfterSwitchHost)
		{
			printf("Switch Recv last message!\n");
			F_SYNC_SetIsRecvAfterSwitchHost(e_UsrFalse);
		}
	}*/
	
}

/*****************************************************************************
** ������:  Sahara_PackSyncData
** ����:    ���saharaͬ���������
** ����:
**   S_SaharaPCtrl *pCtrl[in]
** ����ֵ:
**   ��
** ����������: SyncInputMsg.SaharaInputMsg[out]:Э��ͬ��������
**				 S_SaharaPCtrl *pCtrl[in]��Э��ջָ��
** ����:    ��
*****************************************************************************/
static void  Sahara_PackSyncData(const S_SaharaPCtrl *pCtrl)
{
	/*S_SyncInputMsg* SyncInputMsg = NULL;
	SyncInputMsg = F_SYNC_GetInputMsg();
	if(E_HOST == F_SYNC_HostOrStandby())
	{
		SyncInputMsg->SaharaInputMsg.RedunNextState = pCtrl->RedunNextState;
		SyncInputMsg->SaharaInputMsg.SeqTX = pCtrl->R_Seq.SeqTX;
		SyncInputMsg->SaharaInputMsg.SeqRX = pCtrl->R_Seq.SeqRX;
		SyncInputMsg->SaharaInputMsg.IsInitTActive = pCtrl->IsInitTActive;
		SyncInputMsg->SaharaInputMsg.SafeNextState= pCtrl->SafeNextState;
		SyncInputMsg->SaharaInputMsg.TLastRecv = pCtrl->Time.TLastRecv;
		SyncInputMsg->SaharaInputMsg.TI =  pCtrl->Time.TI;
		SyncInputMsg->SaharaInputMsg.Tlastrtd = pCtrl->Time.Tlastrtd;
		SyncInputMsg->SaharaInputMsg.Tstart = pCtrl->Time.Tstart;
		SyncInputMsg->SaharaInputMsg.SaharaClock = SaharaClock;
		SyncInputMsg->SaharaInputMsg.SNT =  pCtrl->S_Seq.SNT;
		SyncInputMsg->SaharaInputMsg.SNR = pCtrl->S_Seq.SNR;
		SyncInputMsg->SaharaInputMsg.recv_s = pCtrl->RightSafeRecvMsg.s;
		SyncInputMsg->SaharaInputMsg.recv_cs = pCtrl->RightSafeRecvMsg.cs;
		SyncInputMsg->SaharaInputMsg.recv_ts = pCtrl->RightSafeRecvMsg.ts;
		SyncInputMsg->SaharaInputMsg.recv_cts =  pCtrl->RightSafeRecvMsg.cts;
		SyncInputMsg->SaharaInputMsg.send_cs_old = pCtrl->SafeSendMsg.cs_old;
		SyncInputMsg->SaharaInputMsg.send_s = pCtrl->SafeSendMsg.s;
	}*/
}

/*****************************************************************************
** ������:  Sahara_End
** ����:    ���ݰ�ȫ��״̬�����Ƿ�ر������
** ����:
**   S_SaharaPCtrl *pCtrl[in]
** ����ֵ:
**   ��
** ����������: ��
** ����:    ��
*****************************************************************************/
static void Sahara_End(S_SaharaPCtrl *pCtrl)
{
	if ((pCtrl->SafeNextState == (MyUINT8)s_Close) && (pCtrl->RedunNextState != (MyUINT8)r_Close))
	{
		CloseRedun(pCtrl);
	}
}


/*****************************************************************************
** ������:  Sahara_Start
** ����:    ��ʼ����
** ����:
**   S_SaharaPCtrl *pCtrl[in]
** ����ֵ:
**   ��
** ����������: ��
** ����:    ��
*****************************************************************************/
static void Sahara_Start(S_SaharaPCtrl* pCtrl)
{
	OpenRedun(pCtrl);
	OpenSafe(pCtrl);
}

/*****************************************************************************
** ������:  PackSendData
** ����:    ����û�����
** ����:
**   S_SaharaPCtrl *pCtrl[in]
**	 MyUINT16 inlen  �û����ݳ���  ȡֵ��Χ[0��BUFF_SIZE]
**   MyUINT8 *inbuf �û����ݻ���
** ����ֵ:
**   ��
** ����������: ��
** ����:    ��
*****************************************************************************/
static void PackSendData(S_SaharaPCtrl* pCtrl)
{
	MyUINT8 state = pCtrl->SafeNextState;
	MyUINT32 th = 0u;
	pCtrl->Time.Tlocal = SaharaClock;
	th = DiffTime(pCtrl->Time.Th);
	if ((MyUINT8)s_Up == state || (MyUINT8)s_ReReq == state || (MyUINT8)s_ReRun == state)
	{
		if (pCtrl->RecvMsgNum >= SelfBuffSize)    
		{
			if (pCtrl->SendUsrDataMem.msgLength > 0u)
			{
				PackUsrData(pCtrl);
			}/*���û�����*/
			else
			{
				Pack_SaharaMsg(pCtrl, Heartbeat);
				printf("RecvMsgNum bigger than SelfBuffSize ,Send Heartbeat!\n");
			}/*û���û����ݷ�������*/
		}/*��Ϣ���շ��յ�����Ϣ���ﵽ�ֲ������С*/
		else
		{
			if (pCtrl->UnConfirmedNum < pCtrl->peerbuffsize)             
			{
				if (pCtrl->SendUsrDataMem.msgLength > 0u)
				{
					PackUsrData(pCtrl);
				}/*���û�����*/
				else if(th >= TH)
				{
					Pack_SaharaMsg(pCtrl, Heartbeat);
				}/*������ʱ*/
				else
				{ }
			}/*���ͷ�δ�ﵽδ��ȷ����Ϣ�����*/
			else if(th >= TH )
			{
				Pack_SaharaMsg(pCtrl, Heartbeat);
			}/*������ʱ*/
			else
			{ }
		}/*��Ϣ���շ��յ�����Ϣ��δ�ﵽ�ֲ������С*/
	}
}
/*****************************************************************************
** ������:  PackUsrData
** ����:    �ְ������û�����
** ����:
**   S_SaharaPCtrl *pCtrl[in]
** ����ֵ:
**   0 �û����������ݷ������ 
**   1 �û����������ݴ�С�ﵽ�Է����ջ�������
** ����������: ��
** ����:    ��
*****************************************************************************/
static MyUINT8 PackUsrData(S_SaharaPCtrl* pCtrl)
{
	MyUINT8 ret = 0u;
	MyBOOL IsOverPeerBuf = e_UsrFalse;  //δȷ����Ϣ�������ﵽ�Է����ջ������ޱ�־
	while (pCtrl->SendUsrDataMem.msgLength > 0U && IsOverPeerBuf == e_UsrFalse)
	{
		if (pCtrl->SendUsrDataMem.msgLength>USERDATA_SIZE)
		{ 
			pCtrl->SendUsrData.msgLength = USERDATA_SIZE;
		}/*����USERDATA_SIZE�ְ�*/
		else
		{
			pCtrl->SendUsrData.msgLength = pCtrl->SendUsrDataMem.msgLength;
		}
		F_GL_MemCpy(pCtrl->SendUsrData.msg, pCtrl->SendUsrDataMem.msg, pCtrl->SendUsrData.msgLength);
		pCtrl->SendUsrDataMem.msgLength -= pCtrl->SendUsrData.msgLength;
		F_GL_MemCpy(pCtrl->SendUsrDataMem.msg, &pCtrl->SendUsrDataMem.msg[pCtrl->SendUsrData.msgLength], pCtrl->SendUsrDataMem.msgLength);	
		
		
		/*�������*/
		Pack_SaharaMsg(pCtrl,Data);
		F_GL_MemSet(&pCtrl->SendUsrData,0,sizeof(S_Buf));
		if(pCtrl->UnConfirmedNum >= pCtrl->peerbuffsize)
		{
			IsOverPeerBuf = e_UsrTrue;
			ret = 1u;
		}/*δȷ����Ϣ�������ﵽ�Է����ջ������ޣ����ٴ���û�����*/
	}
	return ret;
}


/*****************************************************************************
** ������:  DeliverToSafe
** ����:    ����Ϣ���䵽��ȫ��
** ����:
**   S_SaharaPCtrl *pCtrl[in]
**	 MyUINT8 *inBuf  �û����ݻ���
** ����ֵ:
**   ��
** ����������: ��
** ����:    ��
*****************************************************************************/
static void DeliverToSafe(S_SaharaPCtrl* pCtrl, const MyUINT8 *inBuf)
{
	if ((pCtrl->RecvMsg.dataSize > 0u) && 
		(pCtrl->RecvMsg.dataSize + pCtrl->SafeRecv.msgLength<(MyUINT16)BUFF_SIZE))
	{
		F_GL_MemCpy(&pCtrl->SafeRecv.msg[pCtrl->SafeRecv.msgLength], inBuf + 8, pCtrl->RecvMsg.dataSize);
		pCtrl->SafeRecv.msgLength += pCtrl->RecvMsg.dataSize;
	}
}
/*****************************************************************************
** ������:  CheckVersion
** ����:    У��汾
** ����:
**   S_SaharaPCtrl *pCtrl[in]
** ����ֵ:
**   e_UsrTrue  У����ȷ
**   e_UsrFalse У�����
** ����������: ��
** ����:    ��
*****************************************************************************/
static MyBOOL CheckVersion(const S_SaharaPCtrl* pCtrl)
{
	MyBOOL ret = e_UsrFalse;
	if (((MyUINT16)ConnReq == pCtrl->SafeRecvMsg.stype) || ((MyUINT16)ConnResp == pCtrl->SafeRecvMsg.stype))
	{
		if (VERSION != pCtrl->SafeRecvMsg.version)
		{
			ret =  e_UsrFalse;
		}
		else
		{
			ret = e_UsrTrue;
		}
	}
	else
	{
		ret = e_UsrTrue;
	}
	return ret;
}

/*****************************************************************************
** ������:  CheckSafeType
** ����:    У����������
** ����:
**   S_SaharaPCtrl *pCtrl[in]
** ����ֵ:
**   e_UsrTrue  У����ȷ
**   e_UsrFalse У�����
** ����������: ��
** ����:    ��
*****************************************************************************/
static MyBOOL CheckSafeType(const S_SaharaPCtrl* pCtrl)
{
	MyBOOL ret = e_UsrFalse;
	switch (pCtrl->SafeRecvMsg.stype)
	{
	case (MyUINT16)ConnReq:
	case (MyUINT16)ConnResp:
	case (MyUINT16)DisconnReq:
	case (MyUINT16)ReReq:
	case (MyUINT16)ReData:
	case (MyUINT16)ReResp:
	case (MyUINT16)Data:
	case (MyUINT16)Heartbeat:
		ret = e_UsrTrue;
		break;
	default:
		break;
	}
	return ret;
}

/*****************************************************************************
** ������:  CheckTime
** ����:    У����Ϣ��ʱ��
** ����:
**   S_SaharaPCtrl *pCtrl[in]
** ����ֵ:
**   e_UsrTrue  У����ȷ
**   e_UsrFalse У�����
** ����������: ��
** ����:    ��
*****************************************************************************/
static MyBOOL CheckTime(const S_SaharaPCtrl* pCtrl)
{
	MyUINT32 Diff = 0u;  //��ǰ���ձ������ϴ���ȷ���ձ��ĵ�ʱ���
	MyBOOL ret = e_UsrTrue;
	if (((MyUINT8)s_Down != pCtrl->SafeNextState) && ((MyUINT8)s_Close != pCtrl->SafeNextState))
	{
		if (pCtrl->Time.Tlocal < pCtrl->Time.TLastRecv)
		{
			Diff =  (0xFFFFFFFF - pCtrl->Time.TLastRecv) + pCtrl->Time.Tlocal;
		} /*����ʱ���������*/
		else
		{
			Diff = pCtrl->Time.Tlocal - pCtrl->Time.TLastRecv;
		}
	}
//	printf("diff:%d,TI:%d\n",Diff, pCtrl->Time.TI);
	if (Diff > pCtrl->Time.TI)
	{
		ret = e_UsrFalse;
	}
	return ret;
}
/*****************************************************************************
** ������:  CheckSafeSeq
** ����:    У�鰲ȫ�����к�
** ����:
**   S_SaharaPCtrl *pCtrl[in]
** ����ֵ:
**   e_UsrTrue  У����ȷ
**   e_UsrFalse У�����
** ����������: ��
** ����:    ��
*****************************************************************************/
static MyBOOL CheckSafeSeq(S_SaharaPCtrl* pCtrl)
{
	MyBOOL ret = e_UsrTrue;
	switch (pCtrl->SafeRecvMsg.stype)
	{
	case (MyUINT16)ConnReq:
	case (MyUINT16)ConnResp:
	case (MyUINT16)ReResp:
		pCtrl->S_Seq.SNR = pCtrl->SafeRecvMsg.s;
		break;
	case (MyUINT16)ReReq:
		if (pCtrl->S_Seq.SNR != pCtrl->SafeRecvMsg.s)
		{
			pCtrl->S_Seq.SNR = pCtrl->SafeRecvMsg.s;
		}
		break;
	case (MyUINT16)Heartbeat:
	case (MyUINT16)Data:
	case (MyUINT16)ReData:
		if (pCtrl->S_Seq.SNR != pCtrl->SafeRecvMsg.s)
		{
			printf("SNR:0x%8x,recvseq:0x%8x\n", pCtrl->S_Seq.SNR, pCtrl->SafeRecvMsg.s);
			ret = e_UsrFalse;
		}
		break;
	default:
		break;
	}
	/*��������������*/
	if(ret == e_UsrTrue)
	{
		if (pCtrl->S_Seq.SNR == 0xFFFFFFFFU)
		{
			pCtrl->S_Seq.SNR = 0u;
		}
		else
		{
			pCtrl->S_Seq.SNR++;
		}
	}

	return ret;
}


/*****************************************************************************
** ������:  CheckSafe
** ����:    У�鰲ȫ��
** ����:
**   S_SaharaPCtrl *pCtrl[in]
**	  MyUINT16 *inlen  ��ȫ�����ݳ���
**   MyUINT8 *inbuf  ��ȫ�����ݻ���
** ����ֵ:
** ��
** ����������: ��
** ����:    ��
*****************************************************************************/
static void CheckSafe(S_SaharaPCtrl* pCtrl, MyUINT16 *inlen, MyUINT8 *inbuf)
{
	MyBOOL beCheck = e_UsrFalse;
	pCtrl->Time.Tlocal = SaharaClock;
	if (e_UsrTrue == CheckTime(pCtrl))  // У�鼰ʱ��
	{
		while (pCtrl->SafeRecv.msgLength > 0u)
		{
			beCheck = e_UsrFalse;
			PartitionSafeData(pCtrl);
			if (pCtrl->SafeRecv.msgLength >= pCtrl->SafeRecvMsg.slength
				&& pCtrl->SafeRecvMsg.slength > 0u)/*��ֹ��ѭ��*/
			{ 
				beCheck = CheckSafeData(pCtrl);
			}
			else
			{
				F_GL_MemSet(&pCtrl->SafeRecv,0,sizeof(S_Buf));
			}
			if (e_UsrTrue == beCheck)
			{
				pCtrl->RightSafeRecvMsg = pCtrl->SafeRecvMsg;
			//	printf("Receive right seq :0x%08x\n",pCtrl->RightSafeRecvMsg.s);
				/*���յ���ȷ���ݣ������TLastRecv*/
				pCtrl->Time.TLastRecv = pCtrl->Time.Tlocal;
				/*�յ��������͵����ݣ�������Ϣ��������1*/
				AddRecvNum(pCtrl);
				ComputeUnConfirmedNum(pCtrl);
				/*��ȡ�Է����ջ����С*/
				GetPeerBuffSize(pCtrl);
				ProcessSafeMsg(pCtrl);
			}/*����У����ȷ*/
			else
			{
				/*pCtrl->ErrorCount++;
				printf("Check error:%d !!!!\n",pCtrl->ErrorCount);
				if (pCtrl->ErrorCount > 10u)
				{
					pCtrl->ErrorCount = 0u;
					DisconnProcess(pCtrl, 1u, 0u);
					printf("Disconnect for over ErrorCount \n");
				}*/
			}
			pCtrl->SafeRecv.msgLength -= pCtrl->SafeRecvMsg.slength;
			if (pCtrl->SafeRecv.msgLength <= BUFF_SIZE)
			{
				F_GL_MemCpy(&pCtrl->SafeRecv.msg[0],
					&pCtrl->SafeRecv.msg[pCtrl->SafeRecvMsg.slength],
					pCtrl->SafeRecv.msgLength);
			}
			F_GL_MemSet(&pCtrl->SafeRecvMsg, 0, sizeof(S_MsgSafe));
		}
	}
	else
	{
		DisconnProcess(pCtrl, 1u, 4u);
		printf("Disconnect for over time \n");
	}
	*inlen = pCtrl->RecvUsrData.msgLength;
	F_GL_MemCpy(inbuf, pCtrl->RecvUsrData.msg, pCtrl->RecvUsrData.msgLength);
}
/*****************************************************************************
** ������:  DeliverToUser
** ����:    ����ȫ�����ݴ����û���
** ����:
**   S_SaharaPCtrl *pCtrl[in]
** ����ֵ:
** ��
** ����������: ��
** ����:    ��
*****************************************************************************/
static void DeliverToUser(S_SaharaPCtrl* pCtrl)
{
	if ((pCtrl->SafeRecvMsg.slength >= 36U) && (pCtrl->SafeRecvMsg.slength < BUFF_SIZE))
	{
		F_GL_MemCpy(&pCtrl->RecvUsrData.msg[pCtrl->RecvUsrData.msgLength],
			&pCtrl->SafeRecv.msg[28], pCtrl->SafeRecvMsg.slength - 36u);
		pCtrl->RecvUsrData.msgLength += pCtrl->SafeRecvMsg.slength - 36u;
	}
}
/*****************************************************************************
** ������:  PartitionRedunData
** ����:    �����������ݰ�
** ����:
**   S_SaharaPCtrl *pCtrl[in]
**    MyUINT8 *inBuf  ��������ݻ���
** ����ֵ:
** ��
** ����������: ��
** ����:    ��
*****************************************************************************/
static void PartitionRedunData(S_SaharaPCtrl* pCtrl,  const MyUINT8 *inBuf)
{
	pCtrl->RecvMsg.length = Buffer2MyUINT16(&inBuf[0]);
	pCtrl->RecvMsg.flags = Buffer2MyUINT16(&inBuf[2]);
	pCtrl->RecvMsg.seqNo = Buffer2MyUINT32(&inBuf[4]);
	pCtrl->RecvMsg.dataSize = pCtrl->RecvMsg.length - 8u;
	printf("Redun  receive seq :%d , Redun SNR:%d\n",pCtrl->RecvMsg.seqNo,pCtrl->R_Seq.SeqRX);
}
/*****************************************************************************
** ������:  PartitionSafeData
** ����:    ��ְ�ȫ�����ݰ�
** ����:
**   S_SaharaPCtrl *pCtrl[in]
**    MyUINT8 *inBuf  ��ȫ�����ݻ���
** ����ֵ:
** ��
** ����������: ��
** ����:    ��
*****************************************************************************/
static MyUINT16 PartitionSafeData(S_SaharaPCtrl* pCtrl)
{
	pCtrl->SafeRecvMsg.slength = Buffer2MyUINT16(&pCtrl->SafeRecv.msg[0]);
	pCtrl->SafeRecvMsg.stype = Buffer2MyUINT16(&pCtrl->SafeRecv.msg[2]);
	pCtrl->SafeRecvMsg.recvaddress = Buffer2MyUINT32(&pCtrl->SafeRecv.msg[4]);
	pCtrl->SafeRecvMsg.sendaddress = Buffer2MyUINT32(&pCtrl->SafeRecv.msg[8]);
	pCtrl->SafeRecvMsg.s = Buffer2MyUINT32(&pCtrl->SafeRecv.msg[12]);
	pCtrl->SafeRecvMsg.cs = Buffer2MyUINT32(&pCtrl->SafeRecv.msg[16]);
	pCtrl->SafeRecvMsg.ts = Buffer2MyUINT32(&pCtrl->SafeRecv.msg[20]);
	pCtrl->SafeRecvMsg.cts = Buffer2MyUINT32(&pCtrl->SafeRecv.msg[24]);
	if (pCtrl->SafeRecvMsg.stype == ConnReq || pCtrl->SafeRecvMsg.stype == ConnResp)
	{
		pCtrl->SafeRecvMsg.version = Buffer2MyUINT32(&pCtrl->SafeRecv.msg[28]);
		pCtrl->SafeRecvMsg.peerbuffsize = Buffer2MyUINT16(&pCtrl->SafeRecv.msg[32]);
	}
	if (pCtrl->SafeRecvMsg.stype == DisconnReq)
	{
		pCtrl->SafeRecvMsg.Cause = Buffer2MyUINT16(&pCtrl->SafeRecv.msg[30]);
		pCtrl->SafeRecvMsg.IsReopen = Buffer2MyUINT16(&pCtrl->SafeRecv.msg[28]);
	}
	printf("Safe len:%d,Safe receive seq:0x%08x,Safe receive confirmed seq:%d,Safe SNR:0x%08x\n",
		pCtrl->SafeRecvMsg.slength,pCtrl->SafeRecvMsg.s,pCtrl->SafeRecvMsg.cs,pCtrl->S_Seq.SNR);
    return 1u;
}
/*****************************************************************************
** ������:  InitTmo
** ����:    ��ʼ����ʱ����
** ����:
**   S_SaharaPCtrl *pCtrl[in]
** ����ֵ:
** ��
** ����������: ��
** ����:    ��
*****************************************************************************/
static void InitTmo(S_SaharaPCtrl* pCtrl)
{
	MyUINT32 difftime = DiffTime(pCtrl->R_Seq.TInitResq);
	if ( difftime > InitResp && (e_UsrTrue == pCtrl->IsInitTActive)&& ((MyUINT8)r_Init == pCtrl->RedunNextState))  //��ʼ����Ӧ��ʱ����ʱ
	{
		InitRedun(pCtrl);
	}
}

/*****************************************************************************
** ������:  FormRedunData
** ����:    ������������
** ����:
**   S_SaharaPCtrl *pCtrl[in]
** ����ֵ:
** ��
** ����������: ��
** ����:    ��
*****************************************************************************/
static void FormRedunData(S_SaharaPCtrl* pCtrl)
{
	MyUINT8* pbuf = NULL;
	MyUINT16 safelength = pCtrl->SafeSend.msgLength;
	pCtrl->SendMsg.seqNo = pCtrl->R_Seq.SeqTX;
	pbuf = &pCtrl->RedunSendMsg.msg[pCtrl->RedunSendMsg.msgLength];
	pCtrl->SendMsg.length = safelength + 8u;
	MyUINT162Buffer(pbuf, pCtrl->SendMsg.length);
	MyUINT162Buffer(&pbuf[2], pCtrl->SendMsg.flags);
	MyUINT322Buffer(&pbuf[4], pCtrl->SendMsg.seqNo);
	F_GL_MemCpy(&pbuf[8], pCtrl->SafeSend.msg, safelength);
	pCtrl->RedunSendMsg.msgLength += pCtrl->SendMsg.length;
	F_GL_MemSet(&pCtrl->SendMsg, 0, sizeof(S_MsgRedun));
}


/*****************************************************************************
** ������:  ProcessSafeMsg
** ����:    ������ȷ�İ�ȫ������
** ����:
**   S_SaharaPCtrl *pCtrl[in]
** ����ֵ:
** ��
** ����������: ��
** ����:    ��
*****************************************************************************/
static void ProcessSafeMsg(S_SaharaPCtrl* pCtrl)
{
	MyUINT16 type = pCtrl->SafeRecvMsg.stype;
	switch (type)
	{
	case (MyUINT16)ConnReq:
		printf("Receive ConnReq\n");
		ProcessConnReq(pCtrl);
		break;
	case (MyUINT16)ConnResp:
		printf("Receive ConnResp\n");
		ProcessConnResp(pCtrl);
		break;
	case (MyUINT16)ReReq:
		printf("Receive ReReq!\n");
		pCtrl->UnConfirmedNum = 0u;
		ProcessReReq(pCtrl);
		break;
	case (MyUINT16)ReResp:
		printf("Receive ReResp\n");
		ProcessReResp(pCtrl);
		break;
	case (MyUINT16)DisconnReq:
		printf("Receive Disconnect\n");
		ProcessDisReq(pCtrl);
		break;
	case (MyUINT16)Heartbeat:
		ProcessHeartbeat(pCtrl);
		ClearReTx(pCtrl);   //�յ�ȷ����Ϣ������ش�����
		break;
	case (MyUINT16)Data:
		ProcessData(pCtrl);
		ClearReTx(pCtrl);
		break;
	case (MyUINT16)ReData:
		printf("Receive ReData\n");
		ProcessReData(pCtrl);
		break;
	default:
		break;
	}
	UpdateTI(pCtrl);
}

/*****************************************************************************
** ������:  UpdateTI
** ����:    ����TI
** ����:
**   S_SaharaPCtrl *pCtrl[in]
** ����ֵ:
** ��
** ����������: ��
** ����:    ��
*****************************************************************************/
static void UpdateTI(S_SaharaPCtrl* pCtrl)
{
	MyUINT16 type = pCtrl->SafeRecvMsg.stype;
	if ((MyUINT16)ConnReq == type)
	{
		pCtrl->Time.Tlastrtd = 0u;
	}
	else if((MyUINT16)Data == type || (MyUINT16)Heartbeat == type || (MyUINT16)ReData == type)
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
	if (pCtrl->Time.Tlastrtd <= Tmax)
	{
		pCtrl->Time.TI = Tmax - pCtrl->Time.Tlastrtd;
	}
}
/*****************************************************************************
** ������:  CheckSafeData
** ����:    У�鰲ȫ������
** ����:
**   S_SaharaPCtrl *pCtrl[in]
** ����ֵ:
** ��
** ����������: ��
** ����:    ��
*****************************************************************************/
static MyBOOL CheckSafeData(S_SaharaPCtrl* pCtrl)
{
	MyBOOL ret = e_UsrFalse;
	if (e_UsrTrue == CheckMesgMD4(pCtrl->SafeRecv.msg, pCtrl->SafeRecvMsg.slength)) /*У��MD4*/
	{
		if ((peeraddress == pCtrl->SafeRecvMsg.sendaddress)
			&& (selfaddress == pCtrl->SafeRecvMsg.recvaddress)) /*У���ַ*/
		{
			if (e_UsrTrue == CheckSafeType(pCtrl)) /*У������*/
			{
				if (e_UsrTrue == CheckSafeSeq(pCtrl)) /*У�����*/
				{
					if (e_UsrTrue == CheckVersion(pCtrl)) /*У��汾*/
					{
						ret = e_UsrTrue;
					}
					else
					{
						DisconnProcess(pCtrl, 1u, 6u);
					}
				}
				else
				{
					if (pCtrl->S_Seq.SNR <  pCtrl->SafeRecvMsg.s)
					{
						if ((MyUINT8)s_Up == pCtrl->SafeNextState || (MyUINT8)s_ReRun == pCtrl->SafeNextState)
						{
							Pack_SaharaMsg(pCtrl, ReReq);
							SetSafeState(pCtrl, s_ReReq);
						}
					}
					if ((MyUINT8)s_Start == pCtrl->SafeNextState)
					{
						DisconnProcess(pCtrl, 1u, 3u);
						printf("Disconnect for check seq error at s_Start status !\n");
					}
				}
			}
		}
		else
		{
		}
	}
	return ret;
}

/*****************************************************************************
** ������:  GetPeerBuffSize
** ����:    ��ȡ�Է���������С 
** ����:
**   S_SaharaPCtrl *pCtrl[in]
** ����ֵ:
** ��
** ����������: ��
** ����:    ��
*****************************************************************************/
static void GetPeerBuffSize(S_SaharaPCtrl* pCtrl)
{
	if (((MyUINT16)ConnResp == pCtrl->SafeRecvMsg.stype) && (e_UsrFalse == IsServer()) ||
		((MyUINT16)ConnReq == pCtrl->SafeRecvMsg.stype) && (e_UsrTrue == IsServer()))
	{
		pCtrl->peerbuffsize = pCtrl->SafeRecvMsg.peerbuffsize ;
		if (pCtrl->peerbuffsize > MAX_SEND_NUM)
		{
			pCtrl->peerbuffsize = MAX_SEND_NUM;
		}
	}
}
/*****************************************************************************
** ������:  CheckRedun
** ����:    У�������
** ����:
**   S_SaharaPCtrl *pCtrl[in]  Э��ջָ��
**    MyUINT8 *inbuf[in]  ��������ݻ���  
** ����ֵ:
** ��
** ����������: ��
** ����:    ��
*****************************************************************************/
static void CheckRedun(S_SaharaPCtrl* pCtrl,const MyUINT8 *inbuf)
{
	MyUINT8 Status ;
	Status = 0u;
	switch (pCtrl->RedunNextState)
	{
	case (MyUINT8)r_Init:    //��ʼ��״̬
		/*У���ʼ�����*/
		Status = CheckInitFlag(pCtrl, inbuf);
		SetRedunState(pCtrl, Status);
		break;
	case (MyUINT8)r_Open:    //�߼��ŵ�����״̬
		ReceiveData(pCtrl,  inbuf); //��������
		break;
	default:    //�����ŵ��ر�״̬
		break;
	}	
}

/*****************************************************************************
** ������:  DeferTmo
** ����:    ������ڶ��������Ϣ�Ƿ�ʱ
** ����:
**   S_SaharaPCtrl *pCtrl[in]
** ����ֵ:
** ��
** ����������: ��
** ����:    ��
*****************************************************************************/
static void DeferTmo(S_SaharaPCtrl* pCtrl)
{
	MyUINT8 Redunstate = pCtrl->RedunNextState;
	if ((MyUINT8)r_Open == Redunstate)
	{
		DeliverDeferQueue(pCtrl);
	}
}
/*****************************************************************************
** ������:  DeliverDeferQueue
** ����:    �����ڶ��з�����������Ϣ���䵽��ȫ��
** ����:
**   S_SaharaPCtrl *pCtrl[in]
** ����ֵ:
** ��
** ����������: ��
** ����:    ��
*****************************************************************************/
static void DeliverDeferQueue(S_SaharaPCtrl* pCtrl)
{
	MyUINT8 i = 0u;
	MyUINT32 DeferSeqNo = 0u;
	for (i = 0u; i < DEFERQUEUENUM; i++)
	{
		if (DiffTime(pCtrl->R_Seq.TSEQ[i]) >= SEQ && pCtrl->R_Seq.TSEQ[i] != 0u)
		{
			DeferSeqNo = Buffer2MyUINT32(&pCtrl->DeferMsg[i].msg[4]);
			pCtrl->RecvMsg.dataSize = pCtrl->DeferMsg[i].msgLength - 8u;
			DeliverToSafe(pCtrl, pCtrl->DeferMsg[i].msg);
			F_GL_MemSet(&pCtrl->DeferMsg[i], 0, sizeof(S_Buf));
			pCtrl->R_Seq.TSEQ[i] = 0u;    //�ͷŸö���
			pCtrl->R_Seq.SeqRX = DeferSeqNo;
			pCtrl->R_Seq.SeqRX++;
			/*����Ƿ�ɴ������ڶ��е���Ϣ*/
			CheckDeferQueue(pCtrl);
			printf("Defer Time!SeqRx:%d\n",pCtrl->R_Seq.SeqRX);
		}
	}
}
/*****************************************************************************
** ������:  DiffTime
** ����:    ����ʱ���
** ����:
**   MyUINT32 timer[in] ȡֵ��Χ[0,0xffffffff]
** ����ֵ:
** ��
** ����������: ��
** ����:    ��
*****************************************************************************/
static MyUINT32 DiffTime(MyUINT32 timer)
{
	MyUINT32 ret = 0u;
	if (SaharaClock < timer)
	{
		ret = 0xFFFFFFFF - timer + SaharaClock;
	}
	else
	{
		ret = SaharaClock - timer;
	}
	//printf("difftime:%d\n",ret);
	return ret;
}

/*****************************************************************************
** ������:  IsServer
** ����:    �ж��Ƿ�Ϊ�����
** ����:
**   ��
** ����ֵ:
** e_UsrFalse  �ͻ���
** e_UsrTrue   �����
** ����������: ��
** ����:    ��
*****************************************************************************/
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
/*****************************************************************************
** ������:  ReceiveData
** ����:    �����������ݰ�
** ����:S_SaharaPCtrl* pCtrl, 
**      MyUINT16 inLen ��������ݳ��� [0,0xffff]
**		MyUINT8 *inBuf ��������ݻ���
** ����ֵ:
** 0  ������Ϣ
** 1  ���䵽��ȫ��
** 2  ���䵽���ڶ���
** 3  ���յ������û����ݵĳ�ʼ����Ϣ
** ����������: ��
** ����:    ��
*****************************************************************************/
static MyUINT8 ReceiveData(S_SaharaPCtrl* pCtrl,  const MyUINT8 *inBuf)
{
	MyUINT8 result = 0u;
	/*���յ���Ϣ��Ϊ��ʼ����Ϣ����У����������к�*/
	if (0x0001u != (pCtrl->RecvMsg.flags & 0x0001u))
	{
		result = CheckRedunSeq(pCtrl, pCtrl->RecvMsg.length, inBuf);
	}

	/*���յ���ϢΪ��ʼ����Ϣ����Ϣ���Ϊ0*/
	else if(0u == pCtrl->RecvMsg.seqNo)
	{
		/*������ڶ���*/
		ClearAllDeferQueue(pCtrl);

		/*��������������źͷ������*/
		pCtrl->R_Seq.SeqRX = 0u;
		pCtrl->R_Seq.SeqTX = 0u;

		/*����������ż�1*/
		pCtrl->R_Seq.SeqRX++;

		/*���������ĳ�ʼ����Ϣ���Ϊ1*/
		pCtrl->SendMsg.flags = 0x0001u;

		/*�رճ�ʼ����ʱ��*/
		pCtrl->IsInitTActive = e_UsrFalse;

		/*�������������ڰ�ȫ�����ݣ����䵽��ȫ��*/
		if (pCtrl->RecvMsg.dataSize > 0u)
		{
			DeliverToSafe(pCtrl, inBuf);
			result = 1u;
		}
		else
		{
			result = 3u;
		}
	}
	return result;
}
/*****************************************************************************
** ������:  CheckInitFlag
** ����:    У���ʼ����Ϣflag
** ����:S_SaharaPCtrl* pCtrl,
**      MyUINT16 inLen ��������ݳ��� [0,0xffff]
**		MyUINT8 *inBuf ��������ݻ���
** ����ֵ:
** 0  ������Ϣ
** 1  ���䵽��ȫ��
** ����������: ��
** ����:    ��
*****************************************************************************/
static MyUINT8 CheckInitFlag(S_SaharaPCtrl* pCtrl, const MyUINT8 *inbuf)
{
	MyUINT8 ret = 0u;
	if (/*(e_UsrFalse == pCtrl->RecvMsg.hashcode)
		|| */(0x0001u != (pCtrl->RecvMsg.flags & 0x0001u))
		|| (pCtrl->RecvMsg.seqNo != pCtrl->R_Seq.SeqRX))
	{
		/*δ�յ���ʼ�����Ϊ1����Ϣ*/
		ret = (MyUINT8)r_Init;
	}
	else
	{
		//printf("Redun receive  init message at r_Init status \n");
		/*����Ϣ���䵽��ȫ��*/
		DeliverToSafe(pCtrl, inbuf);
		/*����������ż�1*/
		pCtrl->R_Seq.SeqRX++;
		/*�رճ�ʼ����ʱ��*/
		pCtrl->IsInitTActive = e_UsrFalse;
		ret = (MyUINT8)r_Open;
	}
	return ret;
}

/*****************************************************************************
** ������:  OpenSafe
** ����:    �򿪰�ȫ��
** ����:S_SaharaPCtrl* pCtrl,
** ����ֵ:��
** ����������: ��
** ����:    ��
*****************************************************************************/
static void OpenSafe(S_SaharaPCtrl* pCtrl)
{
	if ((MyUINT8)s_Close == pCtrl->SafeNextState)
	{
		F_GL_MemSet(&pCtrl->ReTx, 0, sizeof(S_Buf));
		if (e_UsrTrue == IsServer())
		{
			SetSafeState(pCtrl, s_Down);
		}/*�������ȫ��״̬��ΪDown*/
		else
		{
			pCtrl->Time.Tlastrtd = 0u;
			pCtrl->Time.TI = Tmax - pCtrl->Time.Tlastrtd;
			Pack_SaharaMsg(pCtrl, ConnReq);
			SetSafeState(pCtrl, s_Start);
		}/*�ͻ���������������*/
	}
}

/*****************************************************************************
** ������:  OpenRedun
** ����:    �������
** ����:S_SaharaPCtrl* pCtrl,
** ����ֵ:��
** ����������: ��
** ����:    ��
*****************************************************************************/
static void OpenRedun(S_SaharaPCtrl* pCtrl)
{
	if ((MyUINT8)r_Close == pCtrl->RedunNextState)
	{
		InitRedun(pCtrl);
		SetRedunState(pCtrl, r_Init);
	}
}
/*****************************************************************************
** ������:  InitRedun
** ����:    ��ʼ�������
** ����:S_SaharaPCtrl* pCtrl,
** ����ֵ:��
** ����������: ��
** ����:    ��
*****************************************************************************/
static void InitRedun(S_SaharaPCtrl* pCtrl)
{
	pCtrl->SendMsg.flags = 0x0001u;
	pCtrl->IsInitTActive = e_UsrTrue;
	pCtrl->R_Seq.TInitResq = SaharaClock;
	pCtrl->R_Seq.SeqRX = 0u;
	pCtrl->R_Seq.SeqTX = 0u;
	ClearAllDeferQueue(pCtrl);
}
/*****************************************************************************
** ������:  ClearAllDeferQueue
** ����:    ������������ŵ������ڶ���
** ����:S_SaharaPCtrl* pCtrl,
** ����ֵ:��
** ����������: ��
** ����:    ��
*****************************************************************************/
static void ClearAllDeferQueue(S_SaharaPCtrl* pCtrl)
{
	MyUINT8 j = 0u;
	for (j = 0u; j < DEFERQUEUENUM; j++)
	{
		F_GL_MemSet(pCtrl->DeferMsg[j].msg, 0, (MyUINT16)BUFF_SIZE);
		pCtrl->DeferMsg[j].msgLength = 0u;
		pCtrl->R_Seq.TSEQ[j] = 0u;        // ���ڼ�ؼ�ʱ����
	}
}


/*****************************************************************************
** ������:  CloseRedun
** ����:    �ر������
** ����:S_SaharaPCtrl* pCtrl,
** ����ֵ:��
** ����������: ��
** ����:    ��
*****************************************************************************/
static void CloseRedun(S_SaharaPCtrl* pCtrl)
{
	MyUINT8 Redunstate = pCtrl->RedunNextState;
	if ((MyUINT8)r_Init == Redunstate || (MyUINT8)r_Open == Redunstate)
	{
		Cleanup(pCtrl);
		SetRedunState(pCtrl, (MyUINT8)r_Close);
	}
	else
	{
		SetRedunState(pCtrl, (MyUINT8)r_Close);
	}
}
/*****************************************************************************
** ������:  SetRedunState
** ����:    ���������״̬
** ����:S_SaharaPCtrl* pCtrl[out],
**		MyUINT8 state[in]
** ����ֵ:��
** ����������: ��
** ����:    ��
*****************************************************************************/
static void SetRedunState(S_SaharaPCtrl* pCtrl, MyUINT8 state)
{
	pCtrl->RedunNextState = state;
}
/*****************************************************************************
** ������:  SetSafeState
** ����:    ���ð�ȫ��״̬
** ����:S_SaharaPCtrl* pCtrl[out],
**		MyUINT8 state[in]
** ����ֵ:��
** ����������: ��
** ����:    ��
*****************************************************************************/
static void SetSafeState(S_SaharaPCtrl* pCtrl, MyUINT8 state)
{
	pCtrl->SafeNextState = state;
}
/*****************************************************************************
** ������:  Cleanup
** ����:    ����ض�����
** ����:S_SaharaPCtrl* pCtrl[out],
** ����ֵ:��
** ����������: ��
** ����:    ��
*****************************************************************************/

static void Cleanup(S_SaharaPCtrl* pCtrl)
{
	//����ض�����
	F_GL_MemSet(&pCtrl->RecvUsrData, 0, sizeof(S_Buf));
	F_GL_MemSet(&pCtrl->SendUsrData, 0, sizeof(S_Buf));
	F_GL_MemSet(&pCtrl->SendUsrDataMem, 0, sizeof(S_Buf));
	F_GL_MemSet(&pCtrl->RedunSendMsg, 0, sizeof(S_Buf));
}
/*****************************************************************************
** ������:  Pack_SaharaMsg
** ����:    ���Sahara���ݰ�
** ����:S_SaharaPCtrl* pCtrl[out],
**		MyUINT16 type  ��Ϣ���� 
** ����ֵ:��
** ����������: ��
** ����:    ��
*****************************************************************************/
static void Pack_SaharaMsg(S_SaharaPCtrl* pCtrl, MyUINT16 type)
{
	pCtrl->Time.Tlocal = SaharaClock;
	/*�����ݴ������ȫ��*/
	switch (type)
	{
	case (MyUINT16)ConnReq:
		pCtrl->Time.Tlastrtd = 0u;
		pCtrl->Time.TI = Tmax - pCtrl->Time.Tlastrtd;
		FormConnSeq(pCtrl);
		pCtrl->Time.TLastRecv = pCtrl->Time.Tlocal; //���������������ʼ��TLastRecv
		printf("Send ConnReq\n");
		break;
	case (MyUINT16)ConnResp:
		FormConnResp(pCtrl);
		printf("Send ConnResp\n");
		break;
	case (MyUINT16)DisconnReq:
		FormDisconnReq(pCtrl);
		printf("Send DisconnReq\n");
		break;
	case (MyUINT16)ReReq:
		FormReReq(pCtrl);
		printf("Send ReReq\n");
		break;
	case (MyUINT16)ReResp:
		FormReResp(pCtrl);
		printf("Send ReResp\n");
		break;
	case (MyUINT16)Data:
		FormData(pCtrl);
		break;
	case (MyUINT16)ReData:
		FormReData(pCtrl);
		break;
	case (MyUINT16)Heartbeat:
		FormHeartbeat(pCtrl);
		break;
	default:
		break;
	}
	pCtrl->Time.Th = SaharaClock;
	if (pCtrl->S_Seq.SNT == 0xFFFFFFFFU)
	{/*��ֹ��ȫ�㷢��������*/
		pCtrl->S_Seq.SNT = 0u;
	}
	else
	{
		pCtrl->S_Seq.SNT++;
	}
	/*ÿ����һ���������͵����ݣ�δȷ�Ϸ�����Ϣ��������1*/
	AddUnConfirmedNum(pCtrl);
	ComputeRecvNum(pCtrl);
	/*����ɵķ�����Ϣ��ȷ�����*/
	pCtrl->SafeSendMsg.cs_old = pCtrl->SafeSendMsg.cs;
	//printf("RecvMsgNum:%d\n",pCtrl->RecvMsgNum );
	/*����ȫ�����ݴ���������*/
	FormRedunData(pCtrl);
	if (pCtrl->R_Seq.SeqTX == 0xFFFFFFFFU)
	{ /*��ֹ����㷢��������*/
		pCtrl->R_Seq.SeqTX = 0u;
	}
	else
	{
		pCtrl->R_Seq.SeqTX++;
	}
	F_GL_MemSet(&pCtrl->SafeSend, 0, sizeof(S_Buf));
}

/*****************************************************************************
** ������:  AddRecvNum
** ����:    ���ռ�������1
** ����:S_SaharaPCtrl* pCtrl[out],
** ����ֵ:��
** ����������: ��
** ����:    ��
*****************************************************************************/
static void AddRecvNum(S_SaharaPCtrl* pCtrl)
{
	MyUINT8 state = pCtrl->SafeNextState;
	if (state == (MyUINT8)s_Up || state == (MyUINT8)s_ReReq || state == (MyUINT8)s_ReRun)
	{
		pCtrl->RecvMsgNum ++;
	}
}


/*****************************************************************************
** ������:  AddUnConfirmedNum
** ����:    δȷ����Ϣ��������1
** ����:S_SaharaPCtrl* pCtrl[out],
** ����ֵ:��
** ����������: ��
** ����:    ��
*****************************************************************************/
static void AddUnConfirmedNum(S_SaharaPCtrl* pCtrl)
{
	MyUINT8 state = pCtrl->SafeNextState;
	if (state == (MyUINT8)s_Up || state == (MyUINT8)s_ReReq || state == (MyUINT8)s_ReRun)
	{
		/*���½�����Ϣ������  = �ɵĽ�����Ϣ������  -���µķ���ȷ����� - �ɵķ���ȷ����ţ�*/
		pCtrl->UnConfirmedNum++;
	}
}
/*****************************************************************************
** ������:  ComputeRecvNum
** ����:    ������ռ�����
** ����:S_SaharaPCtrl* pCtrl[out],
** ����ֵ:��
** ����������: ��
** ����:    ��
*****************************************************************************/
static void ComputeRecvNum(S_SaharaPCtrl* pCtrl)
{
	MyUINT8 state = pCtrl->SafeNextState;
	if (state == (MyUINT8)s_Up || state == (MyUINT8)s_ReReq || state == (MyUINT8)s_ReRun)
	{
		/*���½�����Ϣ������  = �ɵĽ�����Ϣ������  -���µķ���ȷ����� - �ɵķ���ȷ����ţ�*/
		if (pCtrl->RecvMsgNum >= (pCtrl->SafeSendMsg.cs - pCtrl->SafeSendMsg.cs_old))
		{
			pCtrl->RecvMsgNum -= (MyUINT16)(pCtrl->SafeSendMsg.cs - pCtrl->SafeSendMsg.cs_old);
		}
	}
}
/*****************************************************************************
** ������:  ComputeUnConfirmedNum
** ����:    ����δȷ����Ϣ������
** ����:S_SaharaPCtrl* pCtrl[out],
** ����ֵ:��
** ����������: ��
** ����:    ��
*****************************************************************************/
static void ComputeUnConfirmedNum(S_SaharaPCtrl* pCtrl)
{
	MyUINT8 state = pCtrl->SafeNextState;
	if (state == (MyUINT8)s_Up || state == (MyUINT8)s_ReReq || state == (MyUINT8)s_ReRun)
	{
		/*δȷ�Ϸ�����Ϣ������ = ���ط�����Ϣ���к� - ������ȷ���յ���Ϣ��ȷ�����*/
		if (pCtrl->SafeSendMsg.s >= pCtrl->RightSafeRecvMsg.cs)
		{
			pCtrl->UnConfirmedNum = (MyUINT16)(pCtrl->SafeSendMsg.s - pCtrl->RightSafeRecvMsg.cs);
		}
		printf("UnConfirmedNum:%d\n", pCtrl->UnConfirmedNum);
	}
}
/*****************************************************************************
** ������:  FormConnSeq
** ����:    �����������
** ����:S_SaharaPCtrl* pCtrl[out],
** ����ֵ:��
** ����������: ��
** ����:    ��
*****************************************************************************/
static void FormConnSeq(S_SaharaPCtrl* pCtrl)
{
	Hashcode hashcode;
	MyUINT8 *pbuf = NULL;
	pCtrl->S_Seq.SNT = 0u;//GenerateSafeSeq(pCtrl);
	pCtrl->SafeSendMsg.s = pCtrl->S_Seq.SNT;
	pbuf = &pCtrl->SafeSend.msg[0];
	pCtrl->SafeSendMsg.slength = 42u;
	pCtrl->SafeSendMsg.stype = (MyUINT16)ConnReq;
	pCtrl->SafeSendMsg.recvaddress = peeraddress;
	pCtrl->SafeSendMsg.sendaddress = selfaddress;
	pCtrl->SafeSendMsg.cs = 0u;
	pCtrl->SafeSendMsg.ts = pCtrl->Time.Tlocal;
	pCtrl->SafeSendMsg.cts = 0u;
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
	hashcode =  md4(&pCtrl->SafeSend.msg[0], pCtrl->SafeSendMsg.slength - 8u);
	MyUINT322Buffer(pbuf,hashcode.High4Bytes);
	pbuf += 4;
	MyUINT322Buffer(pbuf,hashcode.Low4Bytes);
	pCtrl->SafeSend.msgLength = 42u;
}

/*****************************************************************************
** ������:  FormConnResp
** ����:    ���������Ӧ
** ����:S_SaharaPCtrl* pCtrl[out],
** ����ֵ:��
** ����������: ��
** ����:    ��
*****************************************************************************/
static void FormConnResp(S_SaharaPCtrl* pCtrl)
{
    Hashcode hashcode;
	MyUINT8* pbuf = NULL;
	pCtrl->S_Seq.SNT = 0u;
	pCtrl->SafeSendMsg.s = pCtrl->S_Seq.SNT;
	pbuf = &pCtrl->SafeSend.msg[0];
	pCtrl->SafeSendMsg.slength = 42u;
	pCtrl->SafeSendMsg.stype = (MyUINT16)ConnResp;
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
	hashcode =  md4(&pCtrl->SafeSend.msg[0], pCtrl->SafeSendMsg.slength - 8u);
	MyUINT322Buffer(pbuf,hashcode.High4Bytes);
	pbuf += 4;
	MyUINT322Buffer(pbuf,hashcode.Low4Bytes);
	pCtrl->SafeSend.msgLength = 42u;
}

/*****************************************************************************
** ������:  FormReReq
** ����:    ����ش�����
** ����:S_SaharaPCtrl* pCtrl[out],
** ����ֵ:��
** ����������: ��
** ����:    ��
*****************************************************************************/
static void FormReReq(S_SaharaPCtrl* pCtrl)
{
	Hashcode hashcode;
	MyUINT8* pbuf = NULL;
	pCtrl->SafeSendMsg.s = pCtrl->S_Seq.SNT;
	pbuf = &pCtrl->SafeSend.msg[0];
	pCtrl->SafeSendMsg.slength = 36u;
	pCtrl->SafeSendMsg.stype = (MyUINT16)ReReq;
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
	hashcode =  md4(&pCtrl->SafeSend.msg[0], pCtrl->SafeSendMsg.slength - 8u);
	MyUINT322Buffer(pbuf,hashcode.High4Bytes);
	pbuf += 4;
	MyUINT322Buffer(pbuf,hashcode.Low4Bytes);
	pCtrl->SafeSend.msgLength = 36u;
}

/*****************************************************************************
** ������:  FormReResp
** ����:    ����ش���Ӧ
** ����:S_SaharaPCtrl* pCtrl[out],
** ����ֵ:��
** ����������: ��
** ����:    ��
*****************************************************************************/
static void FormReResp(S_SaharaPCtrl* pCtrl)
{
	Hashcode hashcode;
	MyUINT8* pbuf = NULL;
	pCtrl->SafeSendMsg.s = pCtrl->S_Seq.SNT;
	pbuf = &pCtrl->SafeSend.msg[0];
	pCtrl->SafeSendMsg.slength = 36u;
	pCtrl->SafeSendMsg.stype = (MyUINT16)ReResp;
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
	hashcode =  md4(&pCtrl->SafeSend.msg[0], pCtrl->SafeSendMsg.slength - 8u);
	MyUINT322Buffer(pbuf,hashcode.High4Bytes);
	pbuf += 4;
	MyUINT322Buffer(pbuf,hashcode.Low4Bytes);
	pCtrl->SafeSend.msgLength = 36u;
}

/*****************************************************************************
** ������:  FormHeartbeat
** ����:    �������
** ����:S_SaharaPCtrl* pCtrl[out],
** ����ֵ:��
** ����������: ��
** ����:    ��
*****************************************************************************/
static void FormHeartbeat(S_SaharaPCtrl* pCtrl)
{
	Hashcode hashcode;
	MyUINT8* pbuf = NULL;
	pCtrl->SafeSendMsg.s = pCtrl->S_Seq.SNT;
	pbuf = &pCtrl->SafeSend.msg[0];
	pCtrl->SafeSendMsg.slength = 36u;
	pCtrl->SafeSendMsg.stype =(MyUINT16)Heartbeat;
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
	hashcode =  md4(&pCtrl->SafeSend.msg[0], pCtrl->SafeSendMsg.slength - 8u);
	MyUINT322Buffer(pbuf,hashcode.High4Bytes);
	pbuf += 4;
	MyUINT322Buffer(pbuf,hashcode.Low4Bytes);
	pCtrl->SafeSend.msgLength = 36u;
}


/*****************************************************************************
** ������:  FormDisconnReq
** ����:    ����Ͽ�����
** ����:S_SaharaPCtrl* pCtrl[out],
** ����ֵ:��
** ����������: ��
** ����:    ��
*****************************************************************************/
static void FormDisconnReq(S_SaharaPCtrl* pCtrl)
{
	Hashcode hashcode;
	MyUINT8* pbuf = NULL;
	pCtrl->SafeSendMsg.s = pCtrl->S_Seq.SNT;
	pbuf = &pCtrl->SafeSend.msg[0];
	pCtrl->SafeSendMsg.slength = 40u;
	pCtrl->SafeSendMsg.stype = (MyUINT16)DisconnReq;
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
	hashcode =  md4(&pCtrl->SafeSend.msg[0], pCtrl->SafeSendMsg.slength - 8u);
	MyUINT322Buffer(pbuf,hashcode.High4Bytes);
	pbuf += 4;
	MyUINT322Buffer(pbuf,hashcode.Low4Bytes);
	pCtrl->SafeSend.msgLength = 40u;
}



/*****************************************************************************
** ������:  FormData
** ����:    ����û�����
** ����:S_SaharaPCtrl* pCtrl[out],
** ����ֵ:��
** ����������: ��
** ����:    ��
*****************************************************************************/
static void FormData(S_SaharaPCtrl* pCtrl)
{
	Hashcode hashcode;
	MyUINT8* pbuf = NULL;
	pCtrl->SafeSendMsg.s = pCtrl->S_Seq.SNT;
	pbuf = &pCtrl->SafeSend.msg[0];
	pCtrl->SafeSendMsg.slength = pCtrl->SendUsrData.msgLength + 36u;
	pCtrl->SafeSendMsg.stype = (MyUINT16)Data;
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
	hashcode =  md4(&pCtrl->SafeSend.msg[0], pCtrl->SafeSendMsg.slength - 8u);
	MyUINT322Buffer(pbuf,hashcode.High4Bytes);
	pbuf += 4;
	MyUINT322Buffer(pbuf,hashcode.Low4Bytes);
	if ((pCtrl->ReTx.msgLength + pCtrl->SafeSendMsg.slength) < (MyUINT16)BUFF_SIZE)
	{
		F_GL_MemCpy(&pCtrl->ReTx.msg[pCtrl->ReTx.msgLength],
			&pCtrl->SafeSend.msg[0],
			pCtrl->SafeSendMsg.slength);//���ݷ��뵽�ش�����
		pCtrl->ReTx.msgLength += pCtrl->SafeSendMsg.slength;
	}
	pCtrl->SafeSend.msgLength = pCtrl->SafeSendMsg.slength;
}


/*****************************************************************************
** ������:  FormReData
** ����:    ����ش�����
** ����:S_SaharaPCtrl* pCtrl[out],
** ����ֵ:��
** ����������: ��
** ����:    ��
*****************************************************************************/
static void FormReData(S_SaharaPCtrl* pCtrl)
{
	Hashcode hashcode;
	MyUINT8* pbuf = NULL;
	pCtrl->SafeSendMsg.s = pCtrl->S_Seq.SNT;
	pbuf = &pCtrl->SafeSend.msg[0];
	pCtrl->SafeSendMsg.slength = pCtrl->ReUsrData.msgLength + 36u;
	pCtrl->SafeSendMsg.stype = (MyUINT16)ReData;
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
	hashcode =  md4(&pCtrl->SafeSend.msg[0], pCtrl->SafeSendMsg.slength - 8u);
	MyUINT322Buffer(pbuf,hashcode.High4Bytes);
	pbuf += 4;
	MyUINT322Buffer(pbuf,hashcode.Low4Bytes);
	if ((pCtrl->ReTx.msgLength + pCtrl->SafeSendMsg.slength) <(MyUINT16)BUFF_SIZE)
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

/*****************************************************************************
** ������:  ClearReTx
** ����:    ����ش������жԷ��Ѿ�ȷ�Ϲ�����Ϣ
** ����:S_SaharaPCtrl* pCtrl[out],
** ����ֵ:��
** ����������: ��
** ����:    ��
*****************************************************************************/
static void ClearReTx(S_SaharaPCtrl* pCtrl)
{
	MyUINT16 length = 0u;
	MyUINT32 seq = 0u;
	MyBOOL beClear = e_UsrFalse;   //�Ƿ������ɱ�־
	while (pCtrl->ReTx.msgLength > 0U && beClear == e_UsrFalse)
	{
		beClear = e_UsrFalse;
		length = Buffer2MyUINT16(pCtrl->ReTx.msg);
		if (pCtrl->ReTx.msgLength >= length && length>0)/*��ֹ��ѭ��*/
		{
			seq = Buffer2MyUINT32(&pCtrl->ReTx.msg[12]);
			if (seq <= pCtrl->RightSafeRecvMsg.cs  && pCtrl->ReTx.msgLength <= BUFF_SIZE)
			{
				pCtrl->ReTx.msgLength -= length;
				F_GL_MemCpy(&pCtrl->ReTx.msg[0], &pCtrl->ReTx.msg[length],
					pCtrl->ReTx.msgLength);
			}
			else
			{
				beClear = e_UsrTrue;
			}
		}
		else
		{
			F_GL_MemSet(&pCtrl->ReTx, 0, sizeof(S_Buf));
		}
		
	}
}
/*****************************************************************************
** ������:  CheckMesgMD4
** ����:    64λMD4���
** ����:  MyUINT8* dataBuf У�����ݻ���
**		  MyUINT16 dataSize У�����ݴ�С  ȡֵ��Χ[0,0xffff]
** ����ֵ:��
** ����������: ��
** ����:    ��
*****************************************************************************/
static MyBOOL CheckMesgMD4(MyUINT8* dataBuf, MyUINT16 dataSize)
{
	MyBOOL ret = e_UsrFalse;
	S_Buf buff = { 0 };
	Hashcode hashcode1,hashcode2;
	MyUINT16 len = 0u;
	len = dataSize - 8u;
	if (len <= BUFF_SIZE)
	{
		F_GL_MemCpy(buff.msg, dataBuf, len);
	}
	else
	{
		F_GL_MemSet(&buff, 0, sizeof(S_Buf));
	}
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

/*****************************************************************************
** ������:  Buffer2MyUINT32
** ����:    BUFF����MyUINT32
** ����:  MyUINT8* dataBuf ���ݻ���
** ����ֵ:MyUINT32 result  ȡֵ��Χ[0,0xffffffff]
** ����������: ��
** ����:    ��
*****************************************************************************/
static MyUINT32 Buffer2MyUINT32(const MyUINT8 * bufferPtr)
{
	MyUINT32 result = 0u;
	result = bufferPtr[3] * 256u * 256u * 256u + bufferPtr[2] * 256u * 256u + bufferPtr[1] * 256u + bufferPtr[0];
	return result;
}
/*****************************************************************************
** ������:  Buffer2MyUINT16
** ����:    BUFF����MyUINT16
** ����:  MyUINT8* dataBuf ���ݻ���
** ����ֵ:MyUINT32 result  ȡֵ��Χ[0,0xffff]
** ����������: ��
** ����:    ��
*****************************************************************************/
static MyUINT16 Buffer2MyUINT16(const MyUINT8 * bufferPtr)
{
	MyUINT16 result = 0u;
	result = bufferPtr[1] * 256u + bufferPtr[0];
	return result;
}
/*****************************************************************************
** ������:  MyUINT322Buffer
** ����:   �޷��ų���������BUFFER
** ����:  MyUINT8* pbuf  ���ݻ���
**		MyUINT32 dword   ȡֵ��Χ[0,0xffffffff]
** ����ֵ:��
** ����������: ��
** ����:    ��
*****************************************************************************/
static void MyUINT322Buffer(MyUINT8* pbuf, MyUINT32 dword)
{
	pbuf[0] = (dword)& 0XFFu;
	pbuf[1] = (dword >> 8u) & 0XFFu;
	pbuf[2] = (dword >> 16u) & 0XFFu;
	pbuf[3] = (dword >> 24u) & 0XFFu;
}
/*****************************************************************************
** ������:  MyUINT162Buffer
** ����:   �޷��Ŷ���������BUFFER
** ����:  MyUINT8* pbuf  ���ݻ���
**		MyUINT16 word   ȡֵ��Χ[0,0xffff]
** ����ֵ:��
** ����������: ��
** ����:    ��
*****************************************************************************/
static void MyUINT162Buffer(MyUINT8* pbuf, MyUINT16 word)
{
	pbuf[0] = (word)& 0XFFu;
	pbuf[1] = (word >> 8u) & 0XFFu;
}

/*****************************************************************************
** ������:  md4
** ����:   ���㰲ȫУ����md4
** ����:  MyUINT8*key  ���ݻ���
**		MyUINT32 length  ȡֵ��Χ[0,0xffffffff]
** ����ֵ:Hashcode hashcode  ��ȫУ����
** ����������: ��
** ����:    ��
*****************************************************************************/
static Hashcode md4(MyUINT8*key, MyUINT32 length)
{
	MyUINT8 buf[1024] = {0};
	MyUINT8 *pbuf = &buf[0];
	MyUINT16 i,j;
	MyUINT16 z= 0u;
	Hashcode hashcode = {0};
	MyUINT32 len = length;
	MyUINT32 len1 ;
	MyUINT32 len2 ;
	len1 = 0u;
	len2 = 0u;
	F_GL_MemCpy(buf,key,length);
	AA = 0X4C1987D3u, BB = 0X216912FDu, CC = 0XA7556F07u, DD = 0X34C2716Eu;
	round = len / 64u;
	length_pad[1] = len / 0x20000000u;
	length_pad[0] = (len * 8u) % 0x10000000u;
	for (i = 0u; i < round; i++)
	{
		for (j = 0u; j < 16u; j++)
		{
			X[j] = Buffer2MyUINT32(&pbuf[z]);
			z += 4u;
		}
		z = 0u;
		pbuf += 64u;
		md4_function();
		F_GL_MemSet(X, 0, 64);
	}
	for (j = 0u; j < 16u; j++)
	{
		X[j] = Buffer2MyUINT32(&pbuf[z]);
		z += 4u;
	}
	if(IsBigEndian())
	{
		len1 = len % 64u;
		len2 = (len1/4u)*4u*2u+3u;
		((char*)X)[len2 - len1] = 128;
	}
	else
	{
		((char*)X)[len % 64u] = 128;
	}
	if (len % 64u>55u)
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

/*****************************************************************************
** ������:  md4_function
** ����:   md4����
** ����: ��
** ����ֵ:��
** ����������: ��
** ����:    ��
*****************************************************************************/
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


/*****************************************************************************
** ������:  ProcessConnReq
** ����:   ������������
** ����: S_SaharaPCtrl* pCtrl
** ����ֵ:��
** ����������: ��
** ����:    ��
*****************************************************************************/
static void ProcessConnReq(S_SaharaPCtrl* pCtrl)
{
	MyUINT8 safestate = pCtrl->SafeNextState;
	if ((MyUINT8)s_Down == safestate)
	{
		Pack_SaharaMsg(pCtrl, ConnResp);
		SetSafeState(pCtrl, s_Start);
	}
	if ((MyUINT8)s_Close != safestate && (MyUINT8)s_Down != safestate && (MyUINT8)s_Start != safestate)
	{
		DisconnProcess(pCtrl, 1u, 2u);
	}
}

/*****************************************************************************
** ������:  ProcessConnResp
** ����:   ����������Ӧ
** ����: S_SaharaPCtrl* pCtrl
** ����ֵ:��
** ����������: ��
** ����:    ��
*****************************************************************************/
static void ProcessConnResp(S_SaharaPCtrl* pCtrl)
{
	MyUINT8 safestate = pCtrl->SafeNextState;
	if ((MyUINT8)s_Start == safestate)
	{
		if (IsServer())
		{
			DisconnProcess(pCtrl, 1u, 1u);
		}
		else
		{
			Pack_SaharaMsg(pCtrl, Heartbeat);
			SetSafeState(pCtrl, s_Up);
			pCtrl->RecvMsgNum = 0u;
			pCtrl->UnConfirmedNum = 0u;
		}
	}
	if ((MyUINT8)s_Up == safestate || (MyUINT8)s_ReReq == safestate || (MyUINT8)s_ReRun == safestate)
	{
		DisconnProcess(pCtrl, 1u, 22u);
	}
}

/*****************************************************************************
** ������:  ProcessReResp
** ����:   �����ش���Ӧ
** ����: S_SaharaPCtrl* pCtrl
** ����ֵ:��
** ����������: ��
** ����:    ��
*****************************************************************************/
static void ProcessReResp(S_SaharaPCtrl* pCtrl)
{
	MyUINT8 safestate = pCtrl->SafeNextState;
	if ((MyUINT8)s_Start == safestate || (MyUINT8)s_Up == safestate || (MyUINT8)s_ReRun == safestate)
	{
		DisconnProcess(pCtrl, 1u, 23u);
	}
	if ((MyUINT8)s_ReReq == safestate)
	{
	//	pCtrl->UnConfirmedNum = 0u;
		pCtrl->RecvMsgNum = 0u;
		pCtrl->SafeSendMsg.cs_old = pCtrl->SafeRecvMsg.s;
		SetSafeState(pCtrl, (MyUINT8)s_ReRun);
		printf("recv ReResp!\n");
	}
}
/*****************************************************************************
** ������:  ProcessReReq
** ����:   �����ش�����
** ����: S_SaharaPCtrl* pCtrl
** ����ֵ:��
** ����������: ��
** ����:    ��
*****************************************************************************/
static void ProcessReReq(S_SaharaPCtrl* pCtrl)
{
	MyUINT8 safestate = pCtrl->SafeNextState;
	MyUINT16 ReTxlength ;
	MyUINT8 tempRet = 0;
	ReTxlength = 0u;
	if ((MyUINT8)s_Start == safestate || (MyUINT8)s_ReRun == safestate)
	{
		DisconnProcess(pCtrl, 1u, 24u);
	}
	if ((MyUINT8)s_Up == safestate || (MyUINT8)s_ReReq == safestate)
	{
		tempRet = CheckReTx(pCtrl);
		if (1u == tempRet)/*���ش�����Ϣ��ʱ*/
		{
			DisconnProcess(pCtrl, 1u, 8u);
		}
		else
		{
			if ((2u == tempRet)&&(pCtrl->ReTx1.msgLength > 0u))
			{
				Pack_SaharaMsg(pCtrl, ReResp);
				while(pCtrl->ReTx1.msgLength >0u)
				{
					ReTxlength = Buffer2MyUINT16(pCtrl->ReTx1.msg);
					if (pCtrl->ReTx1.msgLength >= ReTxlength && ReTxlength > 36u)
					{
						/*�����ش���Ϣ��ȷ��ʱ���*/
						pCtrl->Redata.ts = Buffer2MyUINT32(&pCtrl->ReTx1.msg[20]);
						pCtrl->Redata.cts = Buffer2MyUINT32(&pCtrl->ReTx1.msg[24]);
						pCtrl->ReUsrData.msgLength = ReTxlength - 36u;
						F_GL_MemCpy(pCtrl->ReUsrData.msg, &pCtrl->ReTx1.msg[28], pCtrl->ReUsrData.msgLength);
						Pack_SaharaMsg(pCtrl, ReData);
						pCtrl->ReTx1.msgLength -= ReTxlength;
						F_GL_MemCpy(pCtrl->ReTx1.msg, &pCtrl->ReTx1.msg[ReTxlength], pCtrl->ReTx1.msgLength);
					}
					else
					{
						F_GL_MemSet(&pCtrl->ReTx1, 0, sizeof(S_Buf));
					}
				}
				Pack_SaharaMsg(pCtrl, Heartbeat);
				F_GL_MemSet(&pCtrl->ReTx1, 0, sizeof(S_Buf));
			}/*�ش����������������Ϣ*/
			else
			{
				DisconnProcess(pCtrl,1u,7u);
			}/*�ش�ʧ�ܣ���Ϣ�����ش�������*/
		}
	}
}
/*****************************************************************************
** ������:  ProcessReData
** ����:   �����ش�����
** ����: S_SaharaPCtrl* pCtrl
** ����ֵ:��
** ����������: ��
** ����:    ��
*****************************************************************************/
static void ProcessReData(S_SaharaPCtrl* pCtrl)
{
	MyUINT8 safestate = pCtrl->SafeNextState;
	if ((MyUINT8)s_Start == safestate || (MyUINT8)s_Up == safestate ||(MyUINT8)s_ReReq == safestate)
	{
		DisconnProcess(pCtrl, 1u, 1u);
	}
	if ((MyUINT8)s_ReRun == safestate)
	{
		DeliverToUser(pCtrl);
	}
}


/*****************************************************************************
** ������:  DisconnProcess
** ����:   �Ͽ����Ӵ���
** ����: S_SaharaPCtrl* pCtrl  Э��ջָ��
			MyUINT16 Isreopen  �Ƿ��ܹ����´�����
			MyUINT16 cause   �Ͽ�ԭ��
** ����ֵ:��
** ����������: ��
** ����:    ��
*****************************************************************************/
static void DisconnProcess(S_SaharaPCtrl* pCtrl,MyUINT16 Isreopen,MyUINT16 cause)
{
	pCtrl->SafeSendMsg.IsReopen = Isreopen;
	pCtrl->SafeSendMsg.Cause = cause;     
	SetSafeState(pCtrl, (MyUINT8)s_Close);
	CloseRedun(pCtrl);
	Pack_SaharaMsg(pCtrl, DisconnReq);
	if (Isreopen == 1u)
	{
		OpenRedun(pCtrl);
		OpenSafe(pCtrl);
	}
}
/*****************************************************************************
** ������:  ProcessHeartbeat
** ����:   ��������
** ����: S_SaharaPCtrl* pCtrl
** ����ֵ:��
** ����������: ��
** ����:    ��
*****************************************************************************/
static void ProcessHeartbeat(S_SaharaPCtrl* pCtrl)
{
	MyUINT8 safestate = pCtrl->SafeNextState;
	if ((MyUINT8)s_Start == safestate)
	{
		if (IsServer())
		{
			SetSafeState(pCtrl,s_Up);

			/*�Ͽ�������ʼ��������*/
			pCtrl->UnConfirmedNum = 0u;
			pCtrl->RecvMsgNum = 0u;
			pCtrl->SafeSendMsg.cs_old = pCtrl->SafeRecvMsg.s;
		}
		else
		{
			DisconnProcess(pCtrl, 1u, 1u);
		}
	}
	if ((MyUINT8)s_ReRun == safestate)
	{
		SetSafeState(pCtrl, s_Up);
	}
}
/*****************************************************************************
** ������:  ProcessData
** ����:   ��������
** ����: S_SaharaPCtrl* pCtrl
** ����ֵ:��
** ����������: ��
** ����:    ��
*****************************************************************************/
static void ProcessData(S_SaharaPCtrl* pCtrl)
{
	MyUINT8 safestate = pCtrl->SafeNextState;
	if ((MyUINT8)s_Start == safestate)
	{
		DisconnProcess(pCtrl, 1u, 1u);
	}
	else
	{
		if ((MyUINT8)s_Up == safestate || (MyUINT8)s_ReRun == safestate)
		{
			DeliverToUser(pCtrl);
			SetSafeState(pCtrl, s_Up);
		}
	}
}
/*****************************************************************************
** ������:  ProcessHeartbeat
** ����:   ����Ͽ�����
** ����: S_SaharaPCtrl* pCtrl
** ����ֵ:��
** ����������: ��
** ����:    ��
*****************************************************************************/
static void ProcessDisReq(S_SaharaPCtrl* pCtrl)
{
	MyUINT8 safestate = pCtrl->SafeNextState;
	if ((MyUINT8)s_Down != safestate)
	{
		SetSafeState(pCtrl, s_Close);
		//printf("CC\n");
		F_GL_MemSet(&pCtrl->RedunSendMsg,0,sizeof(S_Buf));
		if (pCtrl->SafeRecvMsg.IsReopen == 1u)
		{
			OpenSafe(pCtrl);
		}
		else
		{
		//	printf("peer do not let reopen!\n");
		}
	}
}
/*****************************************************************************
** ������:  CheckRedunSeq
** ����:   �����������к�
** ����: S_SaharaPCtrl* pCtrl
**		 MyUINIT16 inLen  ��������ݳ���  ȡֵ��Χ[0,0xffff]
**		 MyUINT8 *inBuf   ��������ݻ���
** ����ֵ: 0   �������С��Ԥ�ڽ������
		   1   ������ŵ���Ԥ�ڽ������
		   2   ������Ŵ���Ԥ�ڽ������
** ����������: ��
** ����:    ��
*****************************************************************************/
static MyUINT8 CheckRedunSeq(S_SaharaPCtrl* pCtrl, MyUINT16 inLen, const MyUINT8 *inBuf)
{
	MyUINT8 ret = 0u;
	/*��Ϣ���С��Ԥ�ڽ������,�����ø���Ϣ*/
	if (pCtrl->RecvMsg.seqNo < pCtrl->R_Seq.SeqRX)
	{
		ret = 0u;
	}
	/*��Ϣ��ŵ���Ԥ�ڽ������,�򽫸���Ϣ������ȫ��*/
	else if (pCtrl->RecvMsg.seqNo == pCtrl->R_Seq.SeqRX)
	{
		DeliverToSafe(pCtrl, inBuf);
		if (pCtrl->R_Seq.SeqRX == 0xFFFFFFFF)
		{
			pCtrl->R_Seq.SeqRX = 0u;
		}
		else
		{
			pCtrl->R_Seq.SeqRX ++;
		}
		/*����Ƿ�ɴ������ڶ��е���Ϣ*/
		CheckDeferQueue(pCtrl);
		ret = 1u;
	}
	/*��Ϣ��Ŵ���Ԥ�ڽ�����ţ�����Ϣ�������ڶ���*/
	else 
	{
		StoreInDeferQueue(pCtrl, inLen, inBuf);
		ret = 2u;
	}
	return ret;
}

/*****************************************************************************
** ������:  CheckReTx
** ����:   ����ش������Ƿ����ش���Ϣ
** ����: S_SaharaPCtrl* pCtrl
** ����ֵ: 0 ���ش���Ϣ
**         1 �ش���Ϣ��ʱ
**         2 ���ش���Ϣ
** ����������: ��
** ����:    ��
*****************************************************************************/
static MyUINT8 CheckReTx(S_SaharaPCtrl* pCtrl)
{
	MyUINT16 ReTxlength = 0u;
	MyUINT32 ReTxSeq = 0u;
	MyUINT32 ts = 0u;
	MyUINT32 Seq = 0u;
	MyUINT8 ret = 0u;
	
	Seq = pCtrl->SafeRecvMsg.cs ;
	while (pCtrl->ReTx.msgLength > 0u)
	{
		ReTxlength = Buffer2MyUINT16(pCtrl->ReTx.msg);
		if (pCtrl->ReTx.msgLength >= ReTxlength && ReTxlength > 0)
		{
			ReTxSeq = Buffer2MyUINT32(&pCtrl->ReTx.msg[12]);
			ts = Buffer2MyUINT32(&pCtrl->ReTx.msg[20]);
			/*�ش������ȷ�����С���ش���������Ϣ����ţ����ҵ��ش���Ϣ*/
			if (Seq < ReTxSeq)
			{
				/*�ж��ش���Ϣ�Ƿ�ʱ*/
				if ((pCtrl->Time.Tlocal - ts) <= Tmax)
				{
					if (BUFF_SIZE - pCtrl->ReTx1.msgLength >= ReTxlength)
					{
						F_GL_MemCpy(&pCtrl->ReTx1.msg[pCtrl->ReTx1.msgLength], pCtrl->ReTx.msg, ReTxlength);
						pCtrl->ReTx1.msgLength += ReTxlength;
						ret = 2u; //  �ش����������������Ϣ
					}
				}
				else
				{
					ret = 1u;
					F_GL_MemSet(&pCtrl->ReTx1, 0, sizeof(S_Buf));
				}
			
			}
			pCtrl->ReTx.msgLength -= ReTxlength;
			F_GL_MemCpy(pCtrl->ReTx.msg, &pCtrl->ReTx.msg[ReTxlength], pCtrl->ReTx.msgLength);
		}
		else
		{
			F_GL_MemSet(&pCtrl->ReTx, 0, sizeof(S_Buf));
		}
		
	}
	return ret; // �ش����������������Ϣ
}


/*****************************************************************************
** ������:  CheckDeferQueue
** ����:   ȷ���Ƿ�ɴ������ڶ��з�������Ϣ��
** ����: S_SaharaPCtrl* pCtrl
** ����ֵ:��
** ����������: ��
** ����:   �������һ���ı�����ô˺���
*****************************************************************************/
static void CheckDeferQueue(S_SaharaPCtrl* pCtrl)
{
	MyBOOL IsSeqRxChanged = e_UsrTrue;  // �˱�־���ж�������������Ƿ�ı�
	MyUINT8 i = 0u;
	MyUINT32 DeferSeqNo = 0u;
	/*�˴����whileѭ��Ϊ��ֹ���ڶ����е���Ϣδ��˳�����У�
	   ��ѭ��������������������Ϣȡ��û��Ϊֹ*/
	while (e_UsrTrue == IsSeqRxChanged)
	{
		IsSeqRxChanged = e_UsrFalse;
		for (i = 0u; i<DEFERQUEUENUM; i++)
		{
			DeferSeqNo = Buffer2MyUINT32(&pCtrl->DeferMsg[i].msg[4]);
			/*���ڶ����д��ڷ�����������Ϣ*/
			if ((DeferSeqNo == pCtrl->R_Seq.SeqRX) && (DeferSeqNo != 0u))
			{
				/*���䵽��ȫ��*/
				pCtrl->RecvMsg.dataSize = pCtrl->DeferMsg[i].msgLength - 8u;
				DeliverToSafe(pCtrl, pCtrl->DeferMsg[i].msg);
				/*���������Ԥ�ڽ������*/
				pCtrl->R_Seq.SeqRX++;
				/*��մ����ڶ���*/
				F_GL_MemSet(&pCtrl->DeferMsg[i], 0, sizeof(S_Buf));
				/*���ô����ڶ��еļ�ʱ��*/
				pCtrl->R_Seq.TSEQ[i] = 0u;
				IsSeqRxChanged = e_UsrTrue;
			}
		}
	}
}
/*****************************************************************************
** ������:  StoreInDeferQueue
** ����:   �����ʧ����Ϣ���浽���ڶ��У�
** ����: S_SaharaPCtrl* pCtrl
**		 MyUINIT16 inLen  ��������ݳ���  ȡֵ��Χ[0,0xffff]
**		 MyUINT8 *inBuf   ��������ݻ���
** ����ֵ:��
** ����������: ��
** ����:   �������һ���ı�����ô˺���
*****************************************************************************/
static void StoreInDeferQueue(S_SaharaPCtrl* pCtrl, MyUINT16 inLen, const MyUINT8 *inBuf)
{
	printf("store in deferqueue :%d\n",pCtrl->RecvMsg.seqNo);
	if (pCtrl->DeferNum < DEFERQUEUENUM)
	{
		F_GL_MemCpy(pCtrl->DeferMsg[pCtrl->DeferNum].msg, inBuf, inLen);//����Ϣ�洢�����ڶ�����
		pCtrl->DeferMsg[pCtrl->DeferNum].msgLength = inLen;
		pCtrl->R_Seq.TSEQ[pCtrl->DeferNum] = SaharaClock;
	}
	else
	{
		pCtrl->DeferNum = 0u;      //�������򸲸��������Ϣ
		F_GL_MemCpy(pCtrl->DeferMsg[pCtrl->DeferNum].msg, inBuf, inLen);//����Ϣ�洢�����ڶ�����
		pCtrl->DeferMsg[pCtrl->DeferNum].msgLength = inLen;
		pCtrl->R_Seq.TSEQ[pCtrl->DeferNum] = SaharaClock;
	}
	pCtrl->DeferNum++;
}
/*****************************************************************************
** ������:  IsBigEndian
** ����:   �ж��Ƿ��ˣ�
** ����: ��
** ����ֵ:e_UsrFalse С��ģʽ
**		  e_UsrTrue  ���ģʽ
** ����������: ��
** ����:   
*****************************************************************************/
static MyBOOL IsBigEndian()
{
	MyBOOL ret = e_UsrFalse;
	union n ss;
	ss.a= 1;
	if(ss.b != 0)
	{
		ret = e_UsrFalse;
	}
	else
	{
		ret = e_UsrTrue;
	}
	return ret;
}




