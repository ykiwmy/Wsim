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
#ifndef _SAHARA_H_
#define _SAHARA_H_

#ifndef NULL
#define NULL 0
#endif

#ifndef VCP
#define F_GL_MemSet memset
#define F_GL_MemCpy memcpy
#else
#include "GL_Mem_Operation.h"
#include "Vcp_Service_Interface.h"
#endif


#define VERSION   0x32303230u
#define BUFF_SIZE 10240u
#define USERDATA_SIZE 38u*7u    //一个WCU元素大小为38个字节

#define MSG_CRC_MASK 0xA001u//crc掩码


#define DEFERQUEUENUM 10u //延期队列数 （西门子建议该队列数设置为1）
#define RealLinkNum 1u  //实际链路数
#define SelfBuffSize 3u   //接收缓存大小
#define MAX_SEND_NUM  3u//最大发送数量

#define InitResp  10u     //初始化响应监控超时    2200ms
#define SEQ 2u         //延期队列监控超时    500ms

#define TH 1u        //心跳周期为一个联锁周期
#define Tmax 19u    //信道最长容许延时3000ms
#define s_t 0u      


/*********************************************************/
/*MD4相关参数*/
#define F(x,y,z) (((x)&(y))|((~x)&(z)))
#define G(x,y,z) (((x)&(y))|((x)&(z))|((y)&(z)))
#define H(x,y,z) ((x)^(y)^(z))

#define FF(a,b,c,d,k,s) a=RL(a+F(b,c,d)+X[k],s)
#define GG(a,b,c,d,k,s) a=RL(a+G(b,c,d)+X[k]+0x5a827999u,s)
#define HH(a,b,c,d,k,s) a=RL(a+H(b,c,d)+X[k]+0x6ed9eba1u,s)


#define RL(x,y) (((x)<<(y))|((x)>>(32-(y))))
#define PP(x) (x<<24)|((x<<8)&0xff0000)|((x>>8)&0xff00)|(x>>24)
/***********************************************************/
typedef unsigned char  MyUINT8;
typedef unsigned short MyUINT16;
typedef unsigned int   MyUINT32;
typedef signed int	   MyINT32;
typedef signed char    MyINT8;

union n
{
	int a;
	char b;
};


typedef enum RedunNextState
{
	r_Init = 0u,
	r_Open = 1u,
	r_Close = 2u
}RedunNextState;

typedef enum SafeNextState
{
	s_Close = 0u,
	s_Down = 1u,
	s_Start = 2u,
	s_Up = 3u,
	s_ReReq = 4u,
	s_ReRun = 5u
}SafeNextState;


typedef enum MyBOOL
{
	e_UsrFalse = 0, e_UsrTrue = 1
}MyBOOL;

typedef enum MsgType
{
	ConnReq = 6200u,
	ConnResp = 6201u,
	ReReq = 6212u,       //重传请求
	ReResp = 6213u,      //重传响应
	DisconnReq = 6216u,
	Heartbeat = 6220u,
	Data = 6240u,
	ReData = 6241u
}MsgType;

typedef struct
{
	MyUINT32 High4Bytes;
	MyUINT32 Low4Bytes;
}Hashcode;

typedef struct
{
	MyUINT32 SeqTX;          //下一个发送的序列号
	MyUINT32 SeqRX;          //下一个接收的预期序列号
	MyUINT32 TSEQ[DEFERQUEUENUM];       //延期队列监控超时
	MyUINT32  TInitResq;   //新的通信序列监控超时
}S_RedunSeq;

typedef struct
{
	MyUINT32 SNT;          //下一个发送的序列号
	MyUINT32 SNR;          //下一个接收的预期序列
}S_SafeSeq;

typedef struct
{
	MyUINT32 Tlocal;//本地当前时间
	MyUINT32 TLastRecv;//上次接收到正确数据的时间
	MyUINT32 Tlastrtd; //消息往返时间
	MyUINT32 TI;   //连接监控计时器
	MyUINT32 Th;  //心跳计时器
	MyUINT32 Tstart;  //连接开始时间
	MyUINT32 Difftime;
}S_Time;

// message type info structure
typedef struct
{
	MyBOOL hashcode;
	MyUINT16 length;
	MyUINT16 flags;
	MyUINT32  seqNo;
	MyUINT16 dataSize;
}S_MsgRedun;

typedef struct
{
	MyUINT16 slength;
	MyUINT16 stype;
	MyUINT32 recvaddress;
	MyUINT32 sendaddress;
	MyUINT32 s;
	MyUINT32 cs ,cs_old;
	MyUINT32 ts;
	MyUINT32 cts;
	MyUINT32 version;
	MyUINT16 peerbuffsize;
	MyUINT16 IsReopen;
	MyUINT16 Cause;
}S_MsgSafe;


typedef struct
{
	MyUINT16 msgLength;
	MyUINT8 msg[BUFF_SIZE];
}S_Buf;

typedef struct
{
	MyBOOL IsInitTActive;
	MyUINT16 UnConfirmedNum; //未经确认发送消息的计数器
	MyUINT16 RecvMsgNum;     //接收消息计数器
	MyUINT16 peerbuffsize;   //对方的缓存大小
	MyUINT16 DeferNum;
	MyUINT8 ErrorCount;
	S_Buf RedunRecvMsg, RedunSendMsg;
	S_Buf SafeRecv, SafeSend;
	S_Buf DeferMsg[DEFERQUEUENUM];  //延期队列
	S_Buf ReTx,ReTx1; //重传缓存
	S_Buf SendUsrData, SendUsrDataMem/*用户数据暂存区*/, RecvUsrData, ReUsrData;
	S_RedunSeq R_Seq;
	S_SafeSeq S_Seq;
	S_Time Time;
	S_MsgRedun RecvMsg, SendMsg;
	S_MsgSafe SafeRecvMsg, SafeSendMsg, RightSafeRecvMsg, Redata;
	MyUINT8 RedunNextState;
	MyUINT8 SafeNextState;
}S_SaharaPCtrl;

typedef struct SAHARA_InputSyncMsg
{
	MyUINT8 RedunNextState;
	MyUINT32 SeqTX; 
	MyUINT32 SeqRX;
	MyBOOL IsInitTActive;
	MyUINT8 SafeNextState;
	MyUINT32 TLastRecv;
	MyUINT32 TI;
    MyUINT32 Tlastrtd;
	MyUINT32 Tstart;
	MyUINT32 SaharaClock;
	MyUINT32 SNT;
	MyUINT32 SNR;
	MyUINT32 recv_s;
	MyUINT32 recv_cs;
	MyUINT32 recv_ts;
	MyUINT32 recv_cts;
	MyUINT32 send_cs_old;
	MyUINT32 send_s;
}SAHARA_InputSyncMsg;

////////////////////////
void F_Sahara_UpdateClock();
MyBOOL F_Sahara_GetStatus(const MyUINT16 inDstDevId);
void F_Sahara_Init(const MyUINT16 inDstDevId);
void F_Sahara_CheckData(const MyUINT16 inDstDevId, const MyUINT16 inlen, MyUINT8 *inbuf,
	MyUINT16 *outlen, MyUINT8 *outbuf);
MyUINT16 F_Sahara_SendData(const MyUINT16 inDstDevId, const MyUINT16 inlen, MyUINT8 *inbuf, MyUINT16* outlen, MyUINT8* outbuff);
#endif


