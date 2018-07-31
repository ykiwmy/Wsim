/******************************************************************************
** Copyright (c)2017湖南中车时代通信信号有限公司
** 文件名:  sahara.c  
** 描 述:   sahara安全通信协议
**
**------------------------------基础版版本修改记录-------------------------------
**  V1.0.0     日期: 2017.7.10   修改人: 尹琨
**  文件依据：《SAHARA Version 2_2 en_PM2_A6Z00032113503_-_000.pdf》
**  修改内容:   创建
*******************************************************************************/

/**------------------------------特殊修改记录-----------------------------------
**  V1.0.1  日期:                 修改人:
**  修改内容:
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

static MyUINT32 SaharaClock; // 运行周期
static MyUINT32  selfaddress = 0x05e10000u;
static MyUINT32  peeraddress = 0x05ff0000u;
static S_SaharaPCtrl linkList[RealLinkNum];
/*md4相关变量*/
static MyUINT32 X[64], length_pad[2];
static MyUINT32 AA, BB, CC, DD, A, B, C, D;
static MyUINT32 round;


/*****************************************************************************
** 函数名:  F_Sahara_GetStatus
** 功能:    获取链路状态
** 参数:
**    MyUINT16 inDstDevId   取值范围：[0，RealLinkNum]
** 返回值:
**   e_UsrTrue  链路正常
**   e_UsrFalse 链路断开
** 相关输入输出:无
** 其他:    无
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
** 函数名:  F_Sahara_Init
** 功能:    协议初始化
** 参数:
**    MyUINT16 inDstDevId[in]   取值范围：[0，RealLinkNum]
** 返回值:
**   无
** 相关输入输出: 无
** 其他:    无
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
** 函数名:  F_Sahara_UpdateClock
** 功能:    更新协议时钟
** 参数:
**   无
** 返回值:
**   无
** 相关输入输出:SaharaClock  取值范围：[0,0xFFFFFFFFU]
** 其他:    无
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
** 函数名:  F_Sahara_CheckData
** 功能:    校验接收数据
** 参数:
**  MyUINT16 inDstDevId 链路ID  取值范围：[0,ReadlinkNum]
**  MyUINT16 inlen 接收数据长度 取值范围：[0,BUFF_SIZE]
**  MyUINT8 *inbuf 接收数据缓冲
**	MyUINT16 *outlen 校验完的数据长度 [0,BUFF_SIZE]
**  MyUINT8 *outbuf  校验完的数据缓冲
** 返回值:
**   无
** 相关输入输出: 无
** 其他:    无
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
			/*延期队列超时检查*/
			DeferTmo(pCtrl);
			printf("recv len:%d\n", pCtrl->RedunRecvMsg.msgLength);
			while (pCtrl->RedunRecvMsg.msgLength > 0u)
			{
				PartitionRedunData(pCtrl, pCtrl->RedunRecvMsg.msg);
				if (pCtrl->RedunRecvMsg.msgLength >= pCtrl->RecvMsg.length
					&& pCtrl->RecvMsg.length > 0u)
				{
					/*校验冗余层*/
					CheckRedun(pCtrl, pCtrl->RedunRecvMsg.msg);
				}
				else
				{   /*数据长度错误，则不处理剩余数据*/
					F_GL_MemSet(&pCtrl->RedunRecvMsg, 0, sizeof(S_Buf));
				}
				pCtrl->RedunRecvMsg.msgLength -= pCtrl->RecvMsg.length;
				F_GL_MemCpy(&pCtrl->RedunRecvMsg.msg[0], &pCtrl->RedunRecvMsg.msg[pCtrl->RecvMsg.length],
					pCtrl->RedunRecvMsg.msgLength);
				F_GL_MemSet(&pCtrl->RecvMsg, 0, sizeof(S_MsgRedun));
			}
			F_GL_MemSet(&pCtrl->RedunRecvMsg, 0, sizeof(S_Buf));

			/*校验安全层*/
			CheckSafe(pCtrl, outlen, outbuf);

			F_GL_MemSet(&pCtrl->SafeRecv, 0, sizeof(S_Buf));
			F_GL_MemSet(&pCtrl->RecvUsrData, 0, sizeof(S_Buf));
		}/*主机处理*/
	/*}*/
}
/*****************************************************************************
** 函数名:  F_Sahara_SendData
** 功能:    发送sahara数据包
** 参数:
**  MyUINT16 inDstDevId 链路ID  取值范围：[0,ReadlinkNum]
**  MyUINT16 inlen 用户数据长度 取值范围：[0,BUFF_SIZE]
**  MyUINT8 *inbuf 用户数据缓冲
** 返回值:
**   0 ：发送缓冲溢出，不能发送用户数据
**   1 ：正常发送
**   2 : 获取协议栈指针失败
** 相关输入输出: 无
** 其他:    无
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
		/*打包发送数据*/
		PackSendData(pCtrl);
		/*初始化超时检查*/
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
** 函数名:  GetpCtrl
** 功能:    获取协议栈指针
** 参数:
**    MyUINT16 inDstDevId   取值范围：[0，RealLinkNum]
** 返回值:
**     NULL  获取指针失败
**     &linkList[inDstDevId] 成功获取的指针
** 相关输入输出:无
** 其他:    无
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
** 函数名:  Sahara_UnpackSyncData
** 功能:    解包sahara 同步输入数据
** 参数:
**   S_SaharaPCtrl *pCtrl[out]
** 返回值:
**   无
** 相关输入输出: SyncInputMsg.SaharaInputMsg[in]:协议同步数据区
**				 S_SaharaPCtrl *pCtrl[out]：协议栈指针
** 其他:    无
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
** 函数名:  Sahara_PackSyncData
** 功能:    打包sahara同步输出数据
** 参数:
**   S_SaharaPCtrl *pCtrl[in]
** 返回值:
**   无
** 相关输入输出: SyncInputMsg.SaharaInputMsg[out]:协议同步数据区
**				 S_SaharaPCtrl *pCtrl[in]：协议栈指针
** 其他:    无
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
** 函数名:  Sahara_End
** 功能:    根据安全层状态决定是否关闭冗余层
** 参数:
**   S_SaharaPCtrl *pCtrl[in]
** 返回值:
**   无
** 相关输入输出: 无
** 其他:    无
*****************************************************************************/
static void Sahara_End(S_SaharaPCtrl *pCtrl)
{
	if ((pCtrl->SafeNextState == (MyUINT8)s_Close) && (pCtrl->RedunNextState != (MyUINT8)r_Close))
	{
		CloseRedun(pCtrl);
	}
}


/*****************************************************************************
** 函数名:  Sahara_Start
** 功能:    开始连接
** 参数:
**   S_SaharaPCtrl *pCtrl[in]
** 返回值:
**   无
** 相关输入输出: 无
** 其他:    无
*****************************************************************************/
static void Sahara_Start(S_SaharaPCtrl* pCtrl)
{
	OpenRedun(pCtrl);
	OpenSafe(pCtrl);
}

/*****************************************************************************
** 函数名:  PackSendData
** 功能:    打包用户数据
** 参数:
**   S_SaharaPCtrl *pCtrl[in]
**	 MyUINT16 inlen  用户数据长度  取值范围[0，BUFF_SIZE]
**   MyUINT8 *inbuf 用户数据缓冲
** 返回值:
**   无
** 相关输入输出: 无
** 其他:    无
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
			}/*有用户数据*/
			else
			{
				Pack_SaharaMsg(pCtrl, Heartbeat);
				printf("RecvMsgNum bigger than SelfBuffSize ,Send Heartbeat!\n");
			}/*没有用户数据发送心跳*/
		}/*消息接收方收到的信息数达到局部缓存大小*/
		else
		{
			if (pCtrl->UnConfirmedNum < pCtrl->peerbuffsize)             
			{
				if (pCtrl->SendUsrDataMem.msgLength > 0u)
				{
					PackUsrData(pCtrl);
				}/*有用户数据*/
				else if(th >= TH)
				{
					Pack_SaharaMsg(pCtrl, Heartbeat);
				}/*心跳超时*/
				else
				{ }
			}/*发送方未达到未经确认消息最大数*/
			else if(th >= TH )
			{
				Pack_SaharaMsg(pCtrl, Heartbeat);
			}/*心跳超时*/
			else
			{ }
		}/*消息接收方收到的信息数未达到局部缓存大小*/
	}
}
/*****************************************************************************
** 函数名:  PackUsrData
** 功能:    分包发送用户数据
** 参数:
**   S_SaharaPCtrl *pCtrl[in]
** 返回值:
**   0 用户缓存区数据发送完毕 
**   1 用户缓存区数据大小达到对方接收缓存上限
** 相关输入输出: 无
** 其他:    无
*****************************************************************************/
static MyUINT8 PackUsrData(S_SaharaPCtrl* pCtrl)
{
	MyUINT8 ret = 0u;
	MyBOOL IsOverPeerBuf = e_UsrFalse;  //未确认消息计数器达到对方接收缓存上限标志
	while (pCtrl->SendUsrDataMem.msgLength > 0U && IsOverPeerBuf == e_UsrFalse)
	{
		if (pCtrl->SendUsrDataMem.msgLength>USERDATA_SIZE)
		{ 
			pCtrl->SendUsrData.msgLength = USERDATA_SIZE;
		}/*按照USERDATA_SIZE分包*/
		else
		{
			pCtrl->SendUsrData.msgLength = pCtrl->SendUsrDataMem.msgLength;
		}
		F_GL_MemCpy(pCtrl->SendUsrData.msg, pCtrl->SendUsrDataMem.msg, pCtrl->SendUsrData.msgLength);
		pCtrl->SendUsrDataMem.msgLength -= pCtrl->SendUsrData.msgLength;
		F_GL_MemCpy(pCtrl->SendUsrDataMem.msg, &pCtrl->SendUsrDataMem.msg[pCtrl->SendUsrData.msgLength], pCtrl->SendUsrDataMem.msgLength);	
		
		
		/*打包数据*/
		Pack_SaharaMsg(pCtrl,Data);
		F_GL_MemSet(&pCtrl->SendUsrData,0,sizeof(S_Buf));
		if(pCtrl->UnConfirmedNum >= pCtrl->peerbuffsize)
		{
			IsOverPeerBuf = e_UsrTrue;
			ret = 1u;
		}/*未确认消息计数器达到对方接收缓存上限，则不再打包用户数据*/
	}
	return ret;
}


/*****************************************************************************
** 函数名:  DeliverToSafe
** 功能:    将消息传输到安全层
** 参数:
**   S_SaharaPCtrl *pCtrl[in]
**	 MyUINT8 *inBuf  用户数据缓冲
** 返回值:
**   无
** 相关输入输出: 无
** 其他:    无
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
** 函数名:  CheckVersion
** 功能:    校验版本
** 参数:
**   S_SaharaPCtrl *pCtrl[in]
** 返回值:
**   e_UsrTrue  校验正确
**   e_UsrFalse 校验错误
** 相关输入输出: 无
** 其他:    无
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
** 函数名:  CheckSafeType
** 功能:    校验数据类型
** 参数:
**   S_SaharaPCtrl *pCtrl[in]
** 返回值:
**   e_UsrTrue  校验正确
**   e_UsrFalse 校验错误
** 相关输入输出: 无
** 其他:    无
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
** 函数名:  CheckTime
** 功能:    校验消息及时性
** 参数:
**   S_SaharaPCtrl *pCtrl[in]
** 返回值:
**   e_UsrTrue  校验正确
**   e_UsrFalse 校验错误
** 相关输入输出: 无
** 其他:    无
*****************************************************************************/
static MyBOOL CheckTime(const S_SaharaPCtrl* pCtrl)
{
	MyUINT32 Diff = 0u;  //当前接收报文与上次正确接收报文的时间差
	MyBOOL ret = e_UsrTrue;
	if (((MyUINT8)s_Down != pCtrl->SafeNextState) && ((MyUINT8)s_Close != pCtrl->SafeNextState))
	{
		if (pCtrl->Time.Tlocal < pCtrl->Time.TLastRecv)
		{
			Diff =  (0xFFFFFFFF - pCtrl->Time.TLastRecv) + pCtrl->Time.Tlocal;
		} /*本地时间戳溢出情况*/
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
** 函数名:  CheckSafeSeq
** 功能:    校验安全层序列号
** 参数:
**   S_SaharaPCtrl *pCtrl[in]
** 返回值:
**   e_UsrTrue  校验正确
**   e_UsrFalse 校验错误
** 相关输入输出: 无
** 其他:    无
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
	/*接收序号溢出清零*/
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
** 函数名:  CheckSafe
** 功能:    校验安全层
** 参数:
**   S_SaharaPCtrl *pCtrl[in]
**	  MyUINT16 *inlen  安全层数据长度
**   MyUINT8 *inbuf  安全层数据缓冲
** 返回值:
** 无
** 相关输入输出: 无
** 其他:    无
*****************************************************************************/
static void CheckSafe(S_SaharaPCtrl* pCtrl, MyUINT16 *inlen, MyUINT8 *inbuf)
{
	MyBOOL beCheck = e_UsrFalse;
	pCtrl->Time.Tlocal = SaharaClock;
	if (e_UsrTrue == CheckTime(pCtrl))  // 校验及时性
	{
		while (pCtrl->SafeRecv.msgLength > 0u)
		{
			beCheck = e_UsrFalse;
			PartitionSafeData(pCtrl);
			if (pCtrl->SafeRecv.msgLength >= pCtrl->SafeRecvMsg.slength
				&& pCtrl->SafeRecvMsg.slength > 0u)/*防止死循环*/
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
				/*接收到正确数据，则更新TLastRecv*/
				pCtrl->Time.TLastRecv = pCtrl->Time.Tlocal;
				/*收到任意类型的数据，接收消息计数器加1*/
				AddRecvNum(pCtrl);
				ComputeUnConfirmedNum(pCtrl);
				/*获取对方接收缓存大小*/
				GetPeerBuffSize(pCtrl);
				ProcessSafeMsg(pCtrl);
			}/*数据校验正确*/
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
** 函数名:  DeliverToUser
** 功能:    将安全层数据传到用户层
** 参数:
**   S_SaharaPCtrl *pCtrl[in]
** 返回值:
** 无
** 相关输入输出: 无
** 其他:    无
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
** 函数名:  PartitionRedunData
** 功能:    拆分冗余层数据包
** 参数:
**   S_SaharaPCtrl *pCtrl[in]
**    MyUINT8 *inBuf  冗余层数据缓冲
** 返回值:
** 无
** 相关输入输出: 无
** 其他:    无
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
** 函数名:  PartitionSafeData
** 功能:    拆分安全层数据包
** 参数:
**   S_SaharaPCtrl *pCtrl[in]
**    MyUINT8 *inBuf  安全层数据缓冲
** 返回值:
** 无
** 相关输入输出: 无
** 其他:    无
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
** 函数名:  InitTmo
** 功能:    初始化超时处理
** 参数:
**   S_SaharaPCtrl *pCtrl[in]
** 返回值:
** 无
** 相关输入输出: 无
** 其他:    无
*****************************************************************************/
static void InitTmo(S_SaharaPCtrl* pCtrl)
{
	MyUINT32 difftime = DiffTime(pCtrl->R_Seq.TInitResq);
	if ( difftime > InitResp && (e_UsrTrue == pCtrl->IsInitTActive)&& ((MyUINT8)r_Init == pCtrl->RedunNextState))  //初始化响应计时器超时
	{
		InitRedun(pCtrl);
	}
}

/*****************************************************************************
** 函数名:  FormRedunData
** 功能:    打包冗余层数据
** 参数:
**   S_SaharaPCtrl *pCtrl[in]
** 返回值:
** 无
** 相关输入输出: 无
** 其他:    无
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
** 函数名:  ProcessSafeMsg
** 功能:    处理正确的安全层数据
** 参数:
**   S_SaharaPCtrl *pCtrl[in]
** 返回值:
** 无
** 相关输入输出: 无
** 其他:    无
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
		ClearReTx(pCtrl);   //收到确认消息，清除重传缓存
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
** 函数名:  UpdateTI
** 功能:    更新TI
** 参数:
**   S_SaharaPCtrl *pCtrl[in]
** 返回值:
** 无
** 相关输入输出: 无
** 其他:    无
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
		{ /*本地时间戳溢出情况*/
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
** 函数名:  CheckSafeData
** 功能:    校验安全层数据
** 参数:
**   S_SaharaPCtrl *pCtrl[in]
** 返回值:
** 无
** 相关输入输出: 无
** 其他:    无
*****************************************************************************/
static MyBOOL CheckSafeData(S_SaharaPCtrl* pCtrl)
{
	MyBOOL ret = e_UsrFalse;
	if (e_UsrTrue == CheckMesgMD4(pCtrl->SafeRecv.msg, pCtrl->SafeRecvMsg.slength)) /*校验MD4*/
	{
		if ((peeraddress == pCtrl->SafeRecvMsg.sendaddress)
			&& (selfaddress == pCtrl->SafeRecvMsg.recvaddress)) /*校验地址*/
		{
			if (e_UsrTrue == CheckSafeType(pCtrl)) /*校验类型*/
			{
				if (e_UsrTrue == CheckSafeSeq(pCtrl)) /*校验序号*/
				{
					if (e_UsrTrue == CheckVersion(pCtrl)) /*校验版本*/
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
** 函数名:  GetPeerBuffSize
** 功能:    获取对方缓存区大小 
** 参数:
**   S_SaharaPCtrl *pCtrl[in]
** 返回值:
** 无
** 相关输入输出: 无
** 其他:    无
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
** 函数名:  CheckRedun
** 功能:    校验冗余层
** 参数:
**   S_SaharaPCtrl *pCtrl[in]  协议栈指针
**    MyUINT8 *inbuf[in]  冗余层数据缓冲  
** 返回值:
** 无
** 相关输入输出: 无
** 其他:    无
*****************************************************************************/
static void CheckRedun(S_SaharaPCtrl* pCtrl,const MyUINT8 *inbuf)
{
	MyUINT8 Status ;
	Status = 0u;
	switch (pCtrl->RedunNextState)
	{
	case (MyUINT8)r_Init:    //初始化状态
		/*校验初始化标记*/
		Status = CheckInitFlag(pCtrl, inbuf);
		SetRedunState(pCtrl, Status);
		break;
	case (MyUINT8)r_Open:    //逻辑信道启用状态
		ReceiveData(pCtrl,  inbuf); //接收数据
		break;
	default:    //冗余信道关闭状态
		break;
	}	
}

/*****************************************************************************
** 函数名:  DeferTmo
** 功能:    检查延期队列里的消息是否超时
** 参数:
**   S_SaharaPCtrl *pCtrl[in]
** 返回值:
** 无
** 相关输入输出: 无
** 其他:    无
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
** 函数名:  DeliverDeferQueue
** 功能:    将延期队列发出的所有消息传输到安全层
** 参数:
**   S_SaharaPCtrl *pCtrl[in]
** 返回值:
** 无
** 相关输入输出: 无
** 其他:    无
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
			pCtrl->R_Seq.TSEQ[i] = 0u;    //释放该队列
			pCtrl->R_Seq.SeqRX = DeferSeqNo;
			pCtrl->R_Seq.SeqRX++;
			/*检查是否可传输延期队列的消息*/
			CheckDeferQueue(pCtrl);
			printf("Defer Time!SeqRx:%d\n",pCtrl->R_Seq.SeqRX);
		}
	}
}
/*****************************************************************************
** 函数名:  DiffTime
** 功能:    计算时间差
** 参数:
**   MyUINT32 timer[in] 取值范围[0,0xffffffff]
** 返回值:
** 无
** 相关输入输出: 无
** 其他:    无
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
** 函数名:  IsServer
** 功能:    判断是否为服务端
** 参数:
**   无
** 返回值:
** e_UsrFalse  客户端
** e_UsrTrue   服务端
** 相关输入输出: 无
** 其他:    无
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
** 函数名:  ReceiveData
** 功能:    冗余层接收数据包
** 参数:S_SaharaPCtrl* pCtrl, 
**      MyUINT16 inLen 冗余层数据长度 [0,0xffff]
**		MyUINT8 *inBuf 冗余层数据缓冲
** 返回值:
** 0  弃用消息
** 1  传输到安全层
** 2  传输到延期队列
** 3  接收到不含用户数据的初始化消息
** 相关输入输出: 无
** 其他:    无
*****************************************************************************/
static MyUINT8 ReceiveData(S_SaharaPCtrl* pCtrl,  const MyUINT8 *inBuf)
{
	MyUINT8 result = 0u;
	/*接收的消息不为初始化消息，则校验冗余层序列号*/
	if (0x0001u != (pCtrl->RecvMsg.flags & 0x0001u))
	{
		result = CheckRedunSeq(pCtrl, pCtrl->RecvMsg.length, inBuf);
	}

	/*接收的消息为初始化消息且消息序号为0*/
	else if(0u == pCtrl->RecvMsg.seqNo)
	{
		/*清空延期队列*/
		ClearAllDeferQueue(pCtrl);

		/*重置冗余层接收序号和发送序号*/
		pCtrl->R_Seq.SeqRX = 0u;
		pCtrl->R_Seq.SeqTX = 0u;

		/*冗余层接收序号加1*/
		pCtrl->R_Seq.SeqRX++;

		/*设置冗余层的初始化消息标记为1*/
		pCtrl->SendMsg.flags = 0x0001u;

		/*关闭初始化计时器*/
		pCtrl->IsInitTActive = e_UsrFalse;

		/*冗余层数据域存在安全层数据，则传输到安全层*/
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
** 函数名:  CheckInitFlag
** 功能:    校验初始化消息flag
** 参数:S_SaharaPCtrl* pCtrl,
**      MyUINT16 inLen 冗余层数据长度 [0,0xffff]
**		MyUINT8 *inBuf 冗余层数据缓冲
** 返回值:
** 0  弃用消息
** 1  传输到安全层
** 相关输入输出: 无
** 其他:    无
*****************************************************************************/
static MyUINT8 CheckInitFlag(S_SaharaPCtrl* pCtrl, const MyUINT8 *inbuf)
{
	MyUINT8 ret = 0u;
	if (/*(e_UsrFalse == pCtrl->RecvMsg.hashcode)
		|| */(0x0001u != (pCtrl->RecvMsg.flags & 0x0001u))
		|| (pCtrl->RecvMsg.seqNo != pCtrl->R_Seq.SeqRX))
	{
		/*未收到初始化标记为1的消息*/
		ret = (MyUINT8)r_Init;
	}
	else
	{
		//printf("Redun receive  init message at r_Init status \n");
		/*将消息传输到安全层*/
		DeliverToSafe(pCtrl, inbuf);
		/*冗余层接受序号加1*/
		pCtrl->R_Seq.SeqRX++;
		/*关闭初始化计时器*/
		pCtrl->IsInitTActive = e_UsrFalse;
		ret = (MyUINT8)r_Open;
	}
	return ret;
}

/*****************************************************************************
** 函数名:  OpenSafe
** 功能:    打开安全层
** 参数:S_SaharaPCtrl* pCtrl,
** 返回值:无
** 相关输入输出: 无
** 其他:    无
*****************************************************************************/
static void OpenSafe(S_SaharaPCtrl* pCtrl)
{
	if ((MyUINT8)s_Close == pCtrl->SafeNextState)
	{
		F_GL_MemSet(&pCtrl->ReTx, 0, sizeof(S_Buf));
		if (e_UsrTrue == IsServer())
		{
			SetSafeState(pCtrl, s_Down);
		}/*服务端则安全层状态置为Down*/
		else
		{
			pCtrl->Time.Tlastrtd = 0u;
			pCtrl->Time.TI = Tmax - pCtrl->Time.Tlastrtd;
			Pack_SaharaMsg(pCtrl, ConnReq);
			SetSafeState(pCtrl, s_Start);
		}/*客户端则发送连接请求*/
	}
}

/*****************************************************************************
** 函数名:  OpenRedun
** 功能:    打开冗余层
** 参数:S_SaharaPCtrl* pCtrl,
** 返回值:无
** 相关输入输出: 无
** 其他:    无
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
** 函数名:  InitRedun
** 功能:    初始化冗余层
** 参数:S_SaharaPCtrl* pCtrl,
** 返回值:无
** 相关输入输出: 无
** 其他:    无
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
** 函数名:  ClearAllDeferQueue
** 功能:    清空所有物理信道的延期队列
** 参数:S_SaharaPCtrl* pCtrl,
** 返回值:无
** 相关输入输出: 无
** 其他:    无
*****************************************************************************/
static void ClearAllDeferQueue(S_SaharaPCtrl* pCtrl)
{
	MyUINT8 j = 0u;
	for (j = 0u; j < DEFERQUEUENUM; j++)
	{
		F_GL_MemSet(pCtrl->DeferMsg[j].msg, 0, (MyUINT16)BUFF_SIZE);
		pCtrl->DeferMsg[j].msgLength = 0u;
		pCtrl->R_Seq.TSEQ[j] = 0u;        // 延期监控计时清零
	}
}


/*****************************************************************************
** 函数名:  CloseRedun
** 功能:    关闭冗余层
** 参数:S_SaharaPCtrl* pCtrl,
** 返回值:无
** 相关输入输出: 无
** 其他:    无
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
** 函数名:  SetRedunState
** 功能:    设置冗余层状态
** 参数:S_SaharaPCtrl* pCtrl[out],
**		MyUINT8 state[in]
** 返回值:无
** 相关输入输出: 无
** 其他:    无
*****************************************************************************/
static void SetRedunState(S_SaharaPCtrl* pCtrl, MyUINT8 state)
{
	pCtrl->RedunNextState = state;
}
/*****************************************************************************
** 函数名:  SetSafeState
** 功能:    设置安全层状态
** 参数:S_SaharaPCtrl* pCtrl[out],
**		MyUINT8 state[in]
** 返回值:无
** 相关输入输出: 无
** 其他:    无
*****************************************************************************/
static void SetSafeState(S_SaharaPCtrl* pCtrl, MyUINT8 state)
{
	pCtrl->SafeNextState = state;
}
/*****************************************************************************
** 函数名:  Cleanup
** 功能:    清除特定操作
** 参数:S_SaharaPCtrl* pCtrl[out],
** 返回值:无
** 相关输入输出: 无
** 其他:    无
*****************************************************************************/

static void Cleanup(S_SaharaPCtrl* pCtrl)
{
	//清除特定操作
	F_GL_MemSet(&pCtrl->RecvUsrData, 0, sizeof(S_Buf));
	F_GL_MemSet(&pCtrl->SendUsrData, 0, sizeof(S_Buf));
	F_GL_MemSet(&pCtrl->SendUsrDataMem, 0, sizeof(S_Buf));
	F_GL_MemSet(&pCtrl->RedunSendMsg, 0, sizeof(S_Buf));
}
/*****************************************************************************
** 函数名:  Pack_SaharaMsg
** 功能:    打包Sahara数据包
** 参数:S_SaharaPCtrl* pCtrl[out],
**		MyUINT16 type  消息类型 
** 返回值:无
** 相关输入输出: 无
** 其他:    无
*****************************************************************************/
static void Pack_SaharaMsg(S_SaharaPCtrl* pCtrl, MyUINT16 type)
{
	pCtrl->Time.Tlocal = SaharaClock;
	/*将数据打包到安全层*/
	switch (type)
	{
	case (MyUINT16)ConnReq:
		pCtrl->Time.Tlastrtd = 0u;
		pCtrl->Time.TI = Tmax - pCtrl->Time.Tlastrtd;
		FormConnSeq(pCtrl);
		pCtrl->Time.TLastRecv = pCtrl->Time.Tlocal; //发送连接请求则初始化TLastRecv
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
	{/*防止安全层发送序号溢出*/
		pCtrl->S_Seq.SNT = 0u;
	}
	else
	{
		pCtrl->S_Seq.SNT++;
	}
	/*每发送一包任意类型的数据，未确认发送消息计数器加1*/
	AddUnConfirmedNum(pCtrl);
	ComputeRecvNum(pCtrl);
	/*保存旧的发送消息的确认序号*/
	pCtrl->SafeSendMsg.cs_old = pCtrl->SafeSendMsg.cs;
	//printf("RecvMsgNum:%d\n",pCtrl->RecvMsgNum );
	/*将安全层数据打包到冗余层*/
	FormRedunData(pCtrl);
	if (pCtrl->R_Seq.SeqTX == 0xFFFFFFFFU)
	{ /*防止冗余层发送序号溢出*/
		pCtrl->R_Seq.SeqTX = 0u;
	}
	else
	{
		pCtrl->R_Seq.SeqTX++;
	}
	F_GL_MemSet(&pCtrl->SafeSend, 0, sizeof(S_Buf));
}

/*****************************************************************************
** 函数名:  AddRecvNum
** 功能:    接收计数器加1
** 参数:S_SaharaPCtrl* pCtrl[out],
** 返回值:无
** 相关输入输出: 无
** 其他:    无
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
** 函数名:  AddUnConfirmedNum
** 功能:    未确认消息计数器加1
** 参数:S_SaharaPCtrl* pCtrl[out],
** 返回值:无
** 相关输入输出: 无
** 其他:    无
*****************************************************************************/
static void AddUnConfirmedNum(S_SaharaPCtrl* pCtrl)
{
	MyUINT8 state = pCtrl->SafeNextState;
	if (state == (MyUINT8)s_Up || state == (MyUINT8)s_ReReq || state == (MyUINT8)s_ReRun)
	{
		/*最新接收消息计数器  = 旧的接收消息计数器  -（新的发送确认序号 - 旧的发送确认序号）*/
		pCtrl->UnConfirmedNum++;
	}
}
/*****************************************************************************
** 函数名:  ComputeRecvNum
** 功能:    计算接收计数器
** 参数:S_SaharaPCtrl* pCtrl[out],
** 返回值:无
** 相关输入输出: 无
** 其他:    无
*****************************************************************************/
static void ComputeRecvNum(S_SaharaPCtrl* pCtrl)
{
	MyUINT8 state = pCtrl->SafeNextState;
	if (state == (MyUINT8)s_Up || state == (MyUINT8)s_ReReq || state == (MyUINT8)s_ReRun)
	{
		/*最新接收消息计数器  = 旧的接收消息计数器  -（新的发送确认序号 - 旧的发送确认序号）*/
		if (pCtrl->RecvMsgNum >= (pCtrl->SafeSendMsg.cs - pCtrl->SafeSendMsg.cs_old))
		{
			pCtrl->RecvMsgNum -= (MyUINT16)(pCtrl->SafeSendMsg.cs - pCtrl->SafeSendMsg.cs_old);
		}
	}
}
/*****************************************************************************
** 函数名:  ComputeUnConfirmedNum
** 功能:    计算未确认消息计数器
** 参数:S_SaharaPCtrl* pCtrl[out],
** 返回值:无
** 相关输入输出: 无
** 其他:    无
*****************************************************************************/
static void ComputeUnConfirmedNum(S_SaharaPCtrl* pCtrl)
{
	MyUINT8 state = pCtrl->SafeNextState;
	if (state == (MyUINT8)s_Up || state == (MyUINT8)s_ReReq || state == (MyUINT8)s_ReRun)
	{
		/*未确认发送消息计数器 = 本地发送消息序列号 - 最新正确接收到消息的确认序号*/
		if (pCtrl->SafeSendMsg.s >= pCtrl->RightSafeRecvMsg.cs)
		{
			pCtrl->UnConfirmedNum = (MyUINT16)(pCtrl->SafeSendMsg.s - pCtrl->RightSafeRecvMsg.cs);
		}
		printf("UnConfirmedNum:%d\n", pCtrl->UnConfirmedNum);
	}
}
/*****************************************************************************
** 函数名:  FormConnSeq
** 功能:    打包连接请求
** 参数:S_SaharaPCtrl* pCtrl[out],
** 返回值:无
** 相关输入输出: 无
** 其他:    无
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
** 函数名:  FormConnResp
** 功能:    打包连接响应
** 参数:S_SaharaPCtrl* pCtrl[out],
** 返回值:无
** 相关输入输出: 无
** 其他:    无
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
** 函数名:  FormReReq
** 功能:    打包重传请求
** 参数:S_SaharaPCtrl* pCtrl[out],
** 返回值:无
** 相关输入输出: 无
** 其他:    无
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
** 函数名:  FormReResp
** 功能:    打包重传响应
** 参数:S_SaharaPCtrl* pCtrl[out],
** 返回值:无
** 相关输入输出: 无
** 其他:    无
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
** 函数名:  FormHeartbeat
** 功能:    打包心跳
** 参数:S_SaharaPCtrl* pCtrl[out],
** 返回值:无
** 相关输入输出: 无
** 其他:    无
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
** 函数名:  FormDisconnReq
** 功能:    打包断开请求
** 参数:S_SaharaPCtrl* pCtrl[out],
** 返回值:无
** 相关输入输出: 无
** 其他:    无
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
** 函数名:  FormData
** 功能:    打包用户数据
** 参数:S_SaharaPCtrl* pCtrl[out],
** 返回值:无
** 相关输入输出: 无
** 其他:    无
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
			pCtrl->SafeSendMsg.slength);//数据放入到重传缓存
		pCtrl->ReTx.msgLength += pCtrl->SafeSendMsg.slength;
	}
	pCtrl->SafeSend.msgLength = pCtrl->SafeSendMsg.slength;
}


/*****************************************************************************
** 函数名:  FormReData
** 功能:    打包重传数据
** 参数:S_SaharaPCtrl* pCtrl[out],
** 返回值:无
** 相关输入输出: 无
** 其他:    无
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
			pCtrl->SafeSendMsg.slength);//重传数据放入到重传缓存
		/*将重传缓存中的重传消息时间戳改为之前发出该消息的时间戳*/
		MyUINT322Buffer(&pCtrl->ReTx.msg[pCtrl->ReTx.msgLength+20],pCtrl->Redata.ts); 
		pCtrl->ReTx.msgLength += pCtrl->SafeSendMsg.slength;
	}
	pCtrl->SafeSend.msgLength = pCtrl->SafeSendMsg.slength;
}

/*****************************************************************************
** 函数名:  ClearReTx
** 功能:    清除重传缓存中对方已经确认过的消息
** 参数:S_SaharaPCtrl* pCtrl[out],
** 返回值:无
** 相关输入输出: 无
** 其他:    无
*****************************************************************************/
static void ClearReTx(S_SaharaPCtrl* pCtrl)
{
	MyUINT16 length = 0u;
	MyUINT32 seq = 0u;
	MyBOOL beClear = e_UsrFalse;   //是否清除完成标志
	while (pCtrl->ReTx.msgLength > 0U && beClear == e_UsrFalse)
	{
		beClear = e_UsrFalse;
		length = Buffer2MyUINT16(pCtrl->ReTx.msg);
		if (pCtrl->ReTx.msgLength >= length && length>0)/*防止死循环*/
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
** 函数名:  CheckMesgMD4
** 功能:    64位MD4检查
** 参数:  MyUINT8* dataBuf 校验数据缓冲
**		  MyUINT16 dataSize 校验数据大小  取值范围[0,0xffff]
** 返回值:无
** 相关输入输出: 无
** 其他:    无
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
** 函数名:  Buffer2MyUINT32
** 功能:    BUFF填入MyUINT32
** 参数:  MyUINT8* dataBuf 数据缓冲
** 返回值:MyUINT32 result  取值范围[0,0xffffffff]
** 相关输入输出: 无
** 其他:    无
*****************************************************************************/
static MyUINT32 Buffer2MyUINT32(const MyUINT8 * bufferPtr)
{
	MyUINT32 result = 0u;
	result = bufferPtr[3] * 256u * 256u * 256u + bufferPtr[2] * 256u * 256u + bufferPtr[1] * 256u + bufferPtr[0];
	return result;
}
/*****************************************************************************
** 函数名:  Buffer2MyUINT16
** 功能:    BUFF填入MyUINT16
** 参数:  MyUINT8* dataBuf 数据缓冲
** 返回值:MyUINT32 result  取值范围[0,0xffff]
** 相关输入输出: 无
** 其他:    无
*****************************************************************************/
static MyUINT16 Buffer2MyUINT16(const MyUINT8 * bufferPtr)
{
	MyUINT16 result = 0u;
	result = bufferPtr[1] * 256u + bufferPtr[0];
	return result;
}
/*****************************************************************************
** 函数名:  MyUINT322Buffer
** 功能:   无符号长整型填入BUFFER
** 参数:  MyUINT8* pbuf  数据缓冲
**		MyUINT32 dword   取值范围[0,0xffffffff]
** 返回值:无
** 相关输入输出: 无
** 其他:    无
*****************************************************************************/
static void MyUINT322Buffer(MyUINT8* pbuf, MyUINT32 dword)
{
	pbuf[0] = (dword)& 0XFFu;
	pbuf[1] = (dword >> 8u) & 0XFFu;
	pbuf[2] = (dword >> 16u) & 0XFFu;
	pbuf[3] = (dword >> 24u) & 0XFFu;
}
/*****************************************************************************
** 函数名:  MyUINT162Buffer
** 功能:   无符号短整型填入BUFFER
** 参数:  MyUINT8* pbuf  数据缓冲
**		MyUINT16 word   取值范围[0,0xffff]
** 返回值:无
** 相关输入输出: 无
** 其他:    无
*****************************************************************************/
static void MyUINT162Buffer(MyUINT8* pbuf, MyUINT16 word)
{
	pbuf[0] = (word)& 0XFFu;
	pbuf[1] = (word >> 8u) & 0XFFu;
}

/*****************************************************************************
** 函数名:  md4
** 功能:   计算安全校验码md4
** 参数:  MyUINT8*key  数据缓冲
**		MyUINT32 length  取值范围[0,0xffffffff]
** 返回值:Hashcode hashcode  安全校验码
** 相关输入输出: 无
** 其他:    无
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
** 函数名:  md4_function
** 功能:   md4计算
** 参数: 无
** 返回值:无
** 相关输入输出: 无
** 其他:    无
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
** 函数名:  ProcessConnReq
** 功能:   处理连接请求
** 参数: S_SaharaPCtrl* pCtrl
** 返回值:无
** 相关输入输出: 无
** 其他:    无
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
** 函数名:  ProcessConnResp
** 功能:   处理连接响应
** 参数: S_SaharaPCtrl* pCtrl
** 返回值:无
** 相关输入输出: 无
** 其他:    无
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
** 函数名:  ProcessReResp
** 功能:   处理重传响应
** 参数: S_SaharaPCtrl* pCtrl
** 返回值:无
** 相关输入输出: 无
** 其他:    无
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
** 函数名:  ProcessReReq
** 功能:   处理重传请求
** 参数: S_SaharaPCtrl* pCtrl
** 返回值:无
** 相关输入输出: 无
** 其他:    无
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
		if (1u == tempRet)/*需重传的消息超时*/
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
						/*保存重传消息的确认时间戳*/
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
			}/*重传缓存中有请求的消息*/
			else
			{
				DisconnProcess(pCtrl,1u,7u);
			}/*重传失败，消息不在重传缓存中*/
		}
	}
}
/*****************************************************************************
** 函数名:  ProcessReData
** 功能:   处理重传数据
** 参数: S_SaharaPCtrl* pCtrl
** 返回值:无
** 相关输入输出: 无
** 其他:    无
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
** 函数名:  DisconnProcess
** 功能:   断开连接处理
** 参数: S_SaharaPCtrl* pCtrl  协议栈指针
			MyUINT16 Isreopen  是否能够重新打开连接
			MyUINT16 cause   断开原因
** 返回值:无
** 相关输入输出: 无
** 其他:    无
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
** 函数名:  ProcessHeartbeat
** 功能:   处理心跳
** 参数: S_SaharaPCtrl* pCtrl
** 返回值:无
** 相关输入输出: 无
** 其他:    无
*****************************************************************************/
static void ProcessHeartbeat(S_SaharaPCtrl* pCtrl)
{
	MyUINT8 safestate = pCtrl->SafeNextState;
	if ((MyUINT8)s_Start == safestate)
	{
		if (IsServer())
		{
			SetSafeState(pCtrl,s_Up);

			/*断开重连初始化计数器*/
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
** 函数名:  ProcessData
** 功能:   处理数据
** 参数: S_SaharaPCtrl* pCtrl
** 返回值:无
** 相关输入输出: 无
** 其他:    无
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
** 函数名:  ProcessHeartbeat
** 功能:   处理断开请求
** 参数: S_SaharaPCtrl* pCtrl
** 返回值:无
** 相关输入输出: 无
** 其他:    无
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
** 函数名:  CheckRedunSeq
** 功能:   检查冗余层序列号
** 参数: S_SaharaPCtrl* pCtrl
**		 MyUINIT16 inLen  冗余层数据长度  取值范围[0,0xffff]
**		 MyUINT8 *inBuf   冗余层数据缓冲
** 返回值: 0   接收序号小于预期接收序号
		   1   接收序号等于预期接收序号
		   2   接收序号大于预期接收序号
** 相关输入输出: 无
** 其他:    无
*****************************************************************************/
static MyUINT8 CheckRedunSeq(S_SaharaPCtrl* pCtrl, MyUINT16 inLen, const MyUINT8 *inBuf)
{
	MyUINT8 ret = 0u;
	/*消息序号小于预期接收序号,则弃用该消息*/
	if (pCtrl->RecvMsg.seqNo < pCtrl->R_Seq.SeqRX)
	{
		ret = 0u;
	}
	/*消息序号等于预期接收序号,则将该消息传到安全层*/
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
		/*检查是否可传输延期队列的消息*/
		CheckDeferQueue(pCtrl);
		ret = 1u;
	}
	/*消息序号大于预期接收序号，则将消息放入延期队列*/
	else 
	{
		StoreInDeferQueue(pCtrl, inLen, inBuf);
		ret = 2u;
	}
	return ret;
}

/*****************************************************************************
** 函数名:  CheckReTx
** 功能:   检查重传缓存是否有重传消息
** 参数: S_SaharaPCtrl* pCtrl
** 返回值: 0 无重传消息
**         1 重传消息超时
**         2 有重传消息
** 相关输入输出: 无
** 其他:    无
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
			/*重传请求的确认序号小于重传缓存中消息的序号，则找到重传消息*/
			if (Seq < ReTxSeq)
			{
				/*判断重传消息是否超时*/
				if ((pCtrl->Time.Tlocal - ts) <= Tmax)
				{
					if (BUFF_SIZE - pCtrl->ReTx1.msgLength >= ReTxlength)
					{
						F_GL_MemCpy(&pCtrl->ReTx1.msg[pCtrl->ReTx1.msgLength], pCtrl->ReTx.msg, ReTxlength);
						pCtrl->ReTx1.msgLength += ReTxlength;
						ret = 2u; //  重传缓存中有请求的消息
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
	return ret; // 重传缓存中无请求的消息
}


/*****************************************************************************
** 函数名:  CheckDeferQueue
** 功能:   确认是否可传输延期队列发出的消息，
** 参数: S_SaharaPCtrl* pCtrl
** 返回值:无
** 相关输入输出: 无
** 其他:   接收序号一旦改变需调用此函数
*****************************************************************************/
static void CheckDeferQueue(S_SaharaPCtrl* pCtrl)
{
	MyBOOL IsSeqRxChanged = e_UsrTrue;  // 此标志是判断冗余层接收序号是否改变
	MyUINT8 i = 0u;
	MyUINT32 DeferSeqNo = 0u;
	/*此处添加while循环为防止延期队列中的消息未按顺序排列，
	   需循环将符合条件的延期消息取到没有为止*/
	while (e_UsrTrue == IsSeqRxChanged)
	{
		IsSeqRxChanged = e_UsrFalse;
		for (i = 0u; i<DEFERQUEUENUM; i++)
		{
			DeferSeqNo = Buffer2MyUINT32(&pCtrl->DeferMsg[i].msg[4]);
			/*延期队列中存在符合条件的消息*/
			if ((DeferSeqNo == pCtrl->R_Seq.SeqRX) && (DeferSeqNo != 0u))
			{
				/*传输到安全层*/
				pCtrl->RecvMsg.dataSize = pCtrl->DeferMsg[i].msgLength - 8u;
				DeliverToSafe(pCtrl, pCtrl->DeferMsg[i].msg);
				/*更新冗余层预期接收序号*/
				pCtrl->R_Seq.SeqRX++;
				/*清空此延期队列*/
				F_GL_MemSet(&pCtrl->DeferMsg[i], 0, sizeof(S_Buf));
				/*重置此延期队列的计时器*/
				pCtrl->R_Seq.TSEQ[i] = 0u;
				IsSeqRxChanged = e_UsrTrue;
			}
		}
	}
}
/*****************************************************************************
** 函数名:  StoreInDeferQueue
** 功能:   冗余层失序消息保存到延期队列，
** 参数: S_SaharaPCtrl* pCtrl
**		 MyUINIT16 inLen  冗余层数据长度  取值范围[0,0xffff]
**		 MyUINT8 *inBuf   冗余层数据缓冲
** 返回值:无
** 相关输入输出: 无
** 其他:   接收序号一旦改变需调用此函数
*****************************************************************************/
static void StoreInDeferQueue(S_SaharaPCtrl* pCtrl, MyUINT16 inLen, const MyUINT8 *inBuf)
{
	printf("store in deferqueue :%d\n",pCtrl->RecvMsg.seqNo);
	if (pCtrl->DeferNum < DEFERQUEUENUM)
	{
		F_GL_MemCpy(pCtrl->DeferMsg[pCtrl->DeferNum].msg, inBuf, inLen);//将消息存储在延期队列中
		pCtrl->DeferMsg[pCtrl->DeferNum].msgLength = inLen;
		pCtrl->R_Seq.TSEQ[pCtrl->DeferNum] = SaharaClock;
	}
	else
	{
		pCtrl->DeferNum = 0u;      //队列满则覆盖最早的消息
		F_GL_MemCpy(pCtrl->DeferMsg[pCtrl->DeferNum].msg, inBuf, inLen);//将消息存储在延期队列中
		pCtrl->DeferMsg[pCtrl->DeferNum].msgLength = inLen;
		pCtrl->R_Seq.TSEQ[pCtrl->DeferNum] = SaharaClock;
	}
	pCtrl->DeferNum++;
}
/*****************************************************************************
** 函数名:  IsBigEndian
** 功能:   判断是否大端，
** 参数: 无
** 返回值:e_UsrFalse 小端模式
**		  e_UsrTrue  大端模式
** 相关输入输出: 无
** 其他:   
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




