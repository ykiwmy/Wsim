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

static MyUINT32 SaharaClock; // 运行周期
static S_ProtocolControl linkList[RealLinkNum];
/*md4相关变量*/
static MyUINT32 X[64], length_pad[2];
static MyUINT32 AA, BB, CC, DD, A, B, C, D;
static MyUINT32 round;


/** *************************************************************
获取协议栈指针
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
获取链路状态
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
更新时钟
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
解包sahara 同步输入数据
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
打包sahara同步输出数据
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
协议初始化
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
上层请求关闭冗余层
************************************************************* */
static void Sahara_End(S_ProtocolControl *pCtrl)
{
	if ((pCtrl->SafeNextState == s_Close) && (pCtrl->RedunNextState != r_Close))
	{
		CloseRedun(pCtrl);
	}
}


/** *************************************************************
校验接收数据
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
				/*校验冗余层*/
				CheckRedun(pCtrl, pCtrl->RecvMsg.length, pCtrl->RedunRecvMsg.msg);
			}
			else
			{ /*防止死循环*/
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
		/*延期队列超时检查*/
		DeferTmo(pCtrl);
		/*校验安全层*/
		CheckSafe(pCtrl, outlen, outbuf);
		
		F_GL_MemSet(&pCtrl->SafeRecv,0,sizeof(S_Msg));
		F_GL_MemSet(&pCtrl->RecvUsrData,0,sizeof(S_Msg));
	//}
	return 0;
}

/** *************************************************************
发送数据
返回值
	0 ：发送缓冲溢出，不能发送用户数据
         1：正常
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
开始连接
************************************************************* */
static void Sahara_Start(S_ProtocolControl* pCtrl)
{
	OpenRedun(pCtrl);
	OpenSafe(pCtrl);
}
/** *************************************************************
发送安全层数据
************************************************************* */
static MyUINT8 SendSafe(S_ProtocolControl* pCtrl, const MyUINT16 inlen, MyUINT8 *inbuf)
{
	MyUINT8 state = pCtrl->SafeNextState;
	MyUINT32 th = 0;
	pCtrl->Time.Tlocal = SaharaClock;
	th = DiffTime(pCtrl->Time.Th);
	if (s_Up == state || s_ReReq == state || s_ReRun == state)
	{
		if (pCtrl->RecvMsgNum >= SelfBuffSize)    //消息接收方负责在收到的信息数达到局部缓存大小之前发送确认
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
			/*发送方未达到未经确认消息最大数*/
			if (pCtrl->UnConfirmedNum < pCtrl->peerbuffsize)             
			{
				/*有用户数据*/
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
拆用户数据    
返回值： 0 用户缓存区数据发送完毕 
		 1 用户缓存区数据大小达到对方接收缓存上限
************************************************************* */
static MyUINT8 SendUsrData(S_ProtocolControl* pCtrl)
{
	MyUINT8 ret = 0;
	while(pCtrl->SendUsrDataMem.msgLength>0)
	{
		if (pCtrl->SendUsrDataMem.msgLength>USERDATA_SIZE)
		{ /*按照USERDATA_SIZE分包*/
			pCtrl->SendUsrData.msgLength = USERDATA_SIZE;
		}
		else
		{
			pCtrl->SendUsrData.msgLength = pCtrl->SendUsrDataMem.msgLength;
		}
		if (pCtrl->SendUsrDataMem.msgLength < pCtrl->SendUsrData.msgLength)
		{/*防止死循环*/
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
消息传输到安全层
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
校验版本
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
校验安全层消息类型
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
校验安全层及时性
************************************************************* */
static MyBOOL CheckTime(S_ProtocolControl* pCtrl)
{
	MyUINT32 Diff = 0;  //当前接收报文与上次正确接收报文的时间差
	MyBOOL ret = e_UsrTrue;
	if ((s_Down != pCtrl->SafeNextState) && (s_Close != pCtrl->SafeNextState))
	{
		if (pCtrl->Time.Tlocal < pCtrl->Time.TLastRecv)
		{ /*本地时间戳溢出情况*/
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
校验安全层
************************************************************* */
static void CheckSafe(S_ProtocolControl* pCtrl, MyUINT16 *inlen, MyUINT8 *inbuf)
{
	pCtrl->Time.Tlocal = SaharaClock;
	if (e_UsrTrue == CheckTime(pCtrl))  // 校验及时性
	{
		while (pCtrl->SafeRecv.msgLength > 0)
		{
			PartitionSafeData(pCtrl);
			if (pCtrl->SafeRecv.msgLength < pCtrl->SafeRecvMsg.slength 
				|| pCtrl->SafeRecvMsg.slength == 0)
			{ /*防止死循环*/
				F_GL_MemSet(&pCtrl->SafeRecv,0,sizeof(S_Msg));
				F_GL_MemSet(&pCtrl->RecvUsrData, 0, sizeof(S_Msg));
				break;
			}
			if (e_UsrTrue == CheckSafeData(pCtrl))
			{
				pCtrl->RightSafeRecvMsg = pCtrl->SafeRecvMsg;
				//logprintf("Receive right seq :0x%08x\n",pCtrl->RightSafeRecvMsg.s);
				/*接收到正确数据，则更新TLastRecv*/
				pCtrl->Time.TLastRecv = pCtrl->Time.Tlocal;
				/*收到任意类型的数据，接收消息计数器加1*/
				pCtrl->RecvMsgNum++; 
				/*未确认发送消息计数器 = 本地发送消息序列号 - 最新正确接收到消息的确认序号*/
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
传到用户层
************************************************************* */
static void DeliverToUser(S_ProtocolControl* pCtrl)
{
	F_GL_MemCpy(&pCtrl->RecvUsrData.msg[pCtrl->RecvUsrData.msgLength],
		&pCtrl->SafeRecv.msg[28], pCtrl->SafeRecvMsg.slength - 36);
	pCtrl->RecvUsrData.msgLength += pCtrl->SafeRecvMsg.slength - 36;
}

/** ************************************************************
拆分冗余层数据包
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
拆分安全层数据包
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
初始化超时，重新初始化
************************************************************* */
static void InitTmo(S_ProtocolControl* pCtrl)
{
	MyUINT32 difftime = DiffTime(pCtrl->R_Seq.TInitResq);
	if ( difftime > InitResp && (e_UsrTrue == pCtrl->IsInitTActive)&& (r_Init == pCtrl->RedunNextState))  //初始化响应计时器超时
	{
		InitRedun(pCtrl);
	}
}


/** ************************************************************
打包冗余层数据
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
校验安全层序列号
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
	/*接收序号溢出清零*/
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
接收正确数据并处理
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
		ClearReTx(pCtrl);   //收到确认消息，清除重传缓存
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
		{ /*本地时间戳溢出情况*/
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
校验安全数据
************************************************************* */
static MyBOOL CheckSafeData(S_ProtocolControl* pCtrl)
{
	if (!CheckMesgMD4(pCtrl->SafeRecv.msg, pCtrl->SafeRecvMsg.slength))
	{
		//logprintf("Message md4 error!\n");
		return e_UsrFalse;              //安全码错误
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
获取对方缓存区大小 
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
校验冗余层
************************************************************* */
static void CheckRedun(S_ProtocolControl* pCtrl, const MyUINT16 inlen, MyUINT8 *inbuf)
{
	MyUINT8 Status = 0;
	MyUINT16 length = inlen;
	switch (pCtrl->RedunNextState)
	{
	case r_Init:    //初始化状态
		/*校验初始化标记*/
		Status = CheckInitFlag(pCtrl, inlen, inbuf);
		SetRedunState(pCtrl, Status);
		break;
	case r_Open:    //逻辑信道启用状态
		ReceiveData(pCtrl, inlen, inbuf); //接收数据
		break;
	default:    //冗余信道关闭状态
		break;
	}	
}


/*检查延期队列里的消息是否超时*/
static void DeferTmo(S_ProtocolControl* pCtrl)
{
	MyUINT8 Redunstate = pCtrl->RedunNextState;
	if (r_Open == Redunstate)
	{
		DeliverDeferQueue(pCtrl);
	}
}

/** *************************************************************
将延期队列发出的所有消息传输到上层
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
			/*检查是否可传输延期队列的消息*/
			CheckDeferQueue(pCtrl);
			//logprintf("Defer Time!SeqRx:%d\n",pCtrl->R_Seq.SeqRX);
		}
	}
}

/** ************************************************************
计算时间差
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
判断服务端or客户端
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
冗余层接收数据包
返回值：    弃用消息                        0 
            传输到安全层                    1 
            传输到延期队列                  2   
            接收到不含用户数据的初始化序列  3
************************************************************* */
static MyUINT8 ReceiveData(S_ProtocolControl* pCtrl, const  MyUINT16 inLen, MyUINT8 *inBuf)
{
	MyUINT8 result = 0;
	MyUINT16 leftlength = 0;
	/*接收的消息不为初始化消息，则校验冗余层序列号*/
	if (0x0001 != (pCtrl->RecvMsg.flags & 0x0001))
	{
		result = CheckRedunSeq(pCtrl, pCtrl->RecvMsg.length, inBuf);
	}

	/*接收的消息为初始化消息且消息序号为0*/
	else if(0 == pCtrl->RecvMsg.seqNo)
	{
		/*清空延期队列*/
		ClearAllDeferQueue(pCtrl);

		/*重置冗余层接收序号和发送序号*/
		pCtrl->R_Seq.SeqRX = 0;
		pCtrl->R_Seq.SeqTX = 0;

		/*冗余层接收序号加1*/
		pCtrl->R_Seq.SeqRX++;

		/*设置冗余层的初始化消息标记为1*/
		pCtrl->SendMsg.flags = 0x0001;

		/*关闭初始化计时器*/
		pCtrl->IsInitTActive = e_UsrFalse;
		result = 3;

		/*冗余层数据域存在安全层数据，则传输到安全层*/
		if (pCtrl->RecvMsg.dataSize > 0)
		{
			DeliverToSafe(pCtrl, inBuf);
			result = 1;
		}
	}
	return result;
}

/** *************************************************************
校验初始化标记
返回值： 0 弃用消息
1 向上层传输消息
************************************************************* */
static MyUINT8 CheckInitFlag(S_ProtocolControl* pCtrl, const MyUINT16 inlen, MyUINT8 *inbuf)
{
	MyUINT8 ret = 0;
	if (/*(e_UsrFalse == pCtrl->RecvMsg.hashcode)
		|| */(0x0001 != (pCtrl->RecvMsg.flags & 0x0001))
		|| (pCtrl->RecvMsg.seqNo != pCtrl->R_Seq.SeqRX))
	{
		/*未收到初始化标记为1的消息*/
		ret = r_Init;
	}
	else
	{
		//logprintf("Redun receive  init message at r_Init status \n");
		/*将消息传输到安全层*/
		DeliverToSafe(pCtrl, inbuf);
		/*冗余层接受序号加1*/
		pCtrl->R_Seq.SeqRX++;
		/*关闭初始化计时器*/
		pCtrl->IsInitTActive = e_UsrFalse;
		ret = r_Open;
	}
	return ret;
}


/** *************************************************************
打开安全层
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
打开冗余层
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
初始化冗余层
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
清空所有物理信道的延期队列
************************************************************* */
static void ClearAllDeferQueue(S_ProtocolControl* pCtrl)
{
	MyUINT8 i = 0;
	MyUINT8 j = 0;
	for (j = 0; j < DEFERQUEUENUM; j++)
	{
		F_GL_MemSet(pCtrl->DeferMsg[j].msg, 0, BUFF_SIZE);
		pCtrl->DeferMsg[j].msgLength = 0;
		pCtrl->R_Seq.TSEQ[j] = 0;        // 延期监控计时清零
	}
}



/** *************************************************************
关闭冗余层
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
设置冗余层状态
************************************************************* */
static void SetRedunState(S_ProtocolControl* pCtrl, MyUINT8 state)
{
	pCtrl->RedunNextState = state;
}

/** *************************************************************
设置安全层状态
************************************************************* */
static void SetSafeState(S_ProtocolControl* pCtrl, MyUINT8 state)
{
	pCtrl->SafeNextState = state;
}



/** *************************************************************
清除特定操作
************************************************************* */
static MyUINT8 Cleanup(S_ProtocolControl* pCtrl)
{
	//清除特定操作
	F_GL_MemSet(&pCtrl->RecvUsrData, 0, sizeof(S_Msg));
	F_GL_MemSet(&pCtrl->SendUsrData, 0, sizeof(S_Msg));
	F_GL_MemSet(&pCtrl->SendUsrDataMem, 0, sizeof(S_Msg));
	F_GL_MemSet(&pCtrl->RedunSendMsg, 0, sizeof(S_Msg));
	return 0;
}

/** *************************************************************
将各种类型的数据打包到冗余层
************************************************************* */
static void Pack_SaharaMsg(S_ProtocolControl* pCtrl, MyUINT16 type)
{
	pCtrl->Time.Tlocal = SaharaClock;
	
	/*将数据打包到安全层*/
	switch (type)
	{
	case ConnReq:
		pCtrl->Time.Tlastrtd = 0;
		pCtrl->Time.TI = Tmax - pCtrl->Time.Tlastrtd;
		FormConnSeq(pCtrl);
		pCtrl->Time.TLastRecv = pCtrl->Time.Tlocal; //发送连接请求则初始化TLastRecv
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
	{/*防止安全层发送序号溢出*/
		pCtrl->S_Seq.SNT = 0;
	}
	else
	{
		pCtrl->S_Seq.SNT++;
	}
	/*每发送一包任意类型的数据，未确认发送消息计数器加1*/
	pCtrl->UnConfirmedNum++;
	/*最新接收消息计数器  = 旧的接收消息计数器  -（新的发送确认序号 - 旧的发送确认序号）*/
	pCtrl->RecvMsgNum -=(pCtrl->SafeSendMsg.cs - pCtrl->SafeSendMsg.cs_old);
	/*保存旧的发送消息的确认序号*/
	pCtrl->SafeSendMsg.cs_old = pCtrl->SafeSendMsg.cs;
	//logprintf("RecvMsgNum:%d\n",pCtrl->RecvMsgNum );
	/*将安全层数据打包到冗余层*/
	FormRedunData(pCtrl);
	if (pCtrl->R_Seq.SeqTX == 0xFFFFFFFFU)
	{ /*防止冗余层发送序号溢出*/
		pCtrl->R_Seq.SeqTX = 0;
	}
	else
	{
		pCtrl->R_Seq.SeqTX++;
	}
	F_GL_MemSet(&pCtrl->SafeSend, 0, sizeof(S_Msg));
}


/** ************************************************************
打包ConnSeq
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
打包ConnResp
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
打包ReReq
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
打包ReResp
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
打包Heartbeat
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
打包DisconnReq
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
打包Data
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
			pCtrl->SafeSendMsg.slength);//数据放入到重传缓存
		pCtrl->ReTx.msgLength += pCtrl->SafeSendMsg.slength;
	}
	pCtrl->SafeSend.msgLength = pCtrl->SafeSendMsg.slength;
}


/** ************************************************************
打包ReData
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
			pCtrl->SafeSendMsg.slength);//重传数据放入到重传缓存
		/*将重传缓存中的重传消息时间戳改为之前发出该消息的时间戳*/
		MyUINT322Buffer(&pCtrl->ReTx.msg[pCtrl->ReTx.msgLength+20],pCtrl->Redata.ts); 
		pCtrl->ReTx.msgLength += pCtrl->SafeSendMsg.slength;
	}
	pCtrl->SafeSend.msgLength = pCtrl->SafeSendMsg.slength;
}


/** ************************************************************
清除重传缓存中对方已经确认过的消息
************************************************************* */
static void ClearReTx(S_ProtocolControl* pCtrl)
{
	MyUINT16 length = 0;
	MyUINT32 seq = 0;
	while (pCtrl->ReTx.msgLength > 0)
	{
		length = Buffer2MyUINT16(pCtrl->ReTx.msg);
		if (pCtrl->ReTx.msgLength < length)
		{/*防止死循环*/
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
//获取本地时间
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
64位MD4检查
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
BUFF填入MyUINT32
************************************************************* */
static MyUINT32 Buffer2MyUINT32(const MyUINT8 * bufferPtr)
{
	MyUINT32 result = 0;
	result = bufferPtr[3] * 256 * 256 * 256 + bufferPtr[2] * 256 * 256 + bufferPtr[1] * 256 + bufferPtr[0];
	return result;
}
/** ************************************************************
BUFF填入MyUINT16
************************************************************* */
static MyUINT16 Buffer2MyUINT16(const MyUINT8 * bufferPtr)
{
	MyUINT16 result = 0;
	result = bufferPtr[1] * 256 + bufferPtr[0];
	return result;
}




/** ************************************************************
无符号长整型填入BUFFER
************************************************************* */
static void MyUINT322Buffer(MyUINT8* pbuf, MyUINT32 dword)
{
	pbuf[0] = (dword)& 0XFF;
	pbuf[1] = (dword >> 8) & 0XFF;
	pbuf[2] = (dword >> 16) & 0XFF;
	pbuf[3] = (dword >> 24) & 0XFF;
}

/** ************************************************************
无符号短整型填入BUFFER
************************************************************* */
static void MyUINT162Buffer(MyUINT8* pbuf, MyUINT16 word)
{
	pbuf[0] = (word)& 0XFF;
	pbuf[1] = (word >> 8) & 0XFF;
}

/** ************************************************************
16位CRC计算
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
生成随机序列号
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
校验连接请求
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
校验连接响应
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
校验重传响应
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
校验重传请求
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
		if (1 == CheckReTx(pCtrl))/*需重传的消息超时*/
		{
			DisconnProcess(pCtrl, 1, 8);
			//printf("99\n");
		}
		else
		{
			if (pCtrl->ReTx1.msgLength > 0)   //重传缓存中有请求的消息
			{
				Pack_SaharaMsg(pCtrl, ReResp);
				while(pCtrl->ReTx1.msgLength >0)
				{
					ReTxlength = Buffer2MyUINT16(pCtrl->ReTx1.msg);
					if (pCtrl->ReTx1.msgLength < ReTxlength)
					{/*防止死循环*/
						F_GL_MemSet(&pCtrl->ReTx1, 0, sizeof(S_Msg));
						break;
					}
					/*保存重传消息的确认时间戳*/
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
				 //重传失败，消息不在重传缓存中
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
断开连接处理
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
校验心跳
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
处理断开请求
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
检查冗余层序列号
************************************************************* */
static MyUINT8 CheckRedunSeq(S_ProtocolControl* pCtrl, MyUINT16 inLen, MyUINT8 *inBuf)
{
	MyUINT8 ret = 0;
	/*消息序号小于预期接收序号,则弃用该消息*/
	if (pCtrl->RecvMsg.seqNo < pCtrl->R_Seq.SeqRX)
	{
		ret = 0;
	}
	/*消息序号等于预期接收序号,则将该消息传到安全层*/
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
		/*检查是否可传输延期队列的消息*/
		CheckDeferQueue(pCtrl);
		ret = 1;
	}
	/*消息序号大于预期接收序号，则将消息放入延期队列*/
	else 
	{
		StoreInDeferQueue(pCtrl, inLen, inBuf);
		ret = 2;
	}
	return ret;
}


/** ************************************************************
检查重传缓存是否有重传消息
返回值：0 无重传消息
        1 重传消息超时
        2 有重传消息
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
		{/*防止死循环*/
			F_GL_MemSet(&pCtrl->ReTx,0,sizeof(S_Msg));
			break;
		}
		ReTxSeq = Buffer2MyUINT32(&pCtrl->ReTx.msg[12]);
		ts = Buffer2MyUINT32(&pCtrl->ReTx.msg[20]);
		/*重传请求的确认序号小于重传缓存中消息的序号，则找到重传消息*/
		if (Seq < ReTxSeq)
		{
			/*判断重传消息是否超时*/
			if ((pCtrl->Time.Tlocal - ts) > Tmax)
			{
				ret = 1;
				F_GL_MemSet(&pCtrl->ReTx1,0,sizeof(S_Msg));
				break; 
			}
			F_GL_MemCpy(&pCtrl->ReTx1.msg[pCtrl->ReTx1.msgLength], pCtrl->ReTx.msg,ReTxlength);
			pCtrl->ReTx1.msgLength += ReTxlength;
			ret = 2; //  重传缓存中有请求的消息
		}
		pCtrl->ReTx.msgLength-= ReTxlength;
		F_GL_MemCpy(pCtrl->ReTx.msg, &pCtrl->ReTx.msg[ReTxlength], pCtrl->ReTx.msgLength);
	}
	return ret; // 重传缓存中无请求的消息
}



/** ************************************************************
确认是否可传输延期队列发出的消息，
接收序号一旦改变需调用此函数
************************************************************* */
static void CheckDeferQueue(S_ProtocolControl* pCtrl)
{
	MyBOOL IsSeqRxChanged = e_UsrTrue;  // 此标志是判断冗余层接收序号是否改变
	MyUINT8 i = 0;
	MyUINT32 DeferSeqNo = 0;
	/*此处添加while循环为防止延期队列中的消息未按顺序排列，
	   需循环将符合条件的延期消息取到没有为止*/
	while (e_UsrTrue == IsSeqRxChanged)
	{
		IsSeqRxChanged = e_UsrFalse;
		for (i = 0; i<DEFERQUEUENUM; i++)
		{
			DeferSeqNo = Buffer2MyUINT32(&pCtrl->DeferMsg[i].msg[4]);
			/*延期队列中存在符合条件的消息*/
			if ((DeferSeqNo == pCtrl->R_Seq.SeqRX) && (DeferSeqNo != 0))
			{
				/*传输到安全层*/
				pCtrl->RecvMsg.dataSize = pCtrl->DeferMsg[i].msgLength - 8;
				DeliverToSafe(pCtrl, pCtrl->DeferMsg[i].msg);
				/*更新冗余层预期接收序号*/
				pCtrl->R_Seq.SeqRX++;
				/*清空此延期队列*/
				F_GL_MemSet(&pCtrl->DeferMsg[i], 0, sizeof(S_Msg));
				/*重置此延期队列的计时器*/
				pCtrl->R_Seq.TSEQ[i] = 0;
				IsSeqRxChanged = e_UsrTrue;
			}
		}
	}
}


/** *************************************************************
失序消息保存到延期队列
************************************************************* */
static MyUINT8 StoreInDeferQueue(S_ProtocolControl* pCtrl, MyUINT16 inLen, MyUINT8 *inBuf)
{
	//logprintf("store in deferqueue\n");
	if (pCtrl->DeferNum < DEFERQUEUENUM)
	{
		F_GL_MemCpy(pCtrl->DeferMsg[pCtrl->DeferNum].msg, inBuf, inLen);//将消息存储在延期队列中
		pCtrl->DeferMsg[pCtrl->DeferNum].msgLength = inLen;
		pCtrl->R_Seq.TSEQ[pCtrl->DeferNum] = SaharaClock;
	}
	else
	{
		pCtrl->DeferNum = 0;      //队列满则覆盖最早的消息   
		F_GL_MemCpy(pCtrl->DeferMsg[pCtrl->DeferNum].msg, inBuf, inLen);//将消息存储在延期队列中
		pCtrl->DeferMsg[pCtrl->DeferNum].msgLength = inLen;
		//pCtrl->R_Seq.TSEQ[pCtrl->DeferNum] = SaharaClock;
	}
	pCtrl->DeferNum++;
	return 1;
}


/** ************************************************************
校验重传数据
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
校验数据
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
判断是否大端
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




