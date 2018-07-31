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
#define USERDATA_SIZE 38u*7u    //һ��WCUԪ�ش�СΪ38���ֽ�

#define MSG_CRC_MASK 0xA001u//crc����


#define DEFERQUEUENUM 10u //���ڶ����� �������ӽ���ö���������Ϊ1��
#define RealLinkNum 1u  //ʵ����·��
#define SelfBuffSize 3u   //���ջ����С
#define MAX_SEND_NUM  3u//���������

#define InitResp  10u     //��ʼ����Ӧ��س�ʱ    2200ms
#define SEQ 2u         //���ڶ��м�س�ʱ    500ms

#define TH 1u        //��������Ϊһ����������
#define Tmax 19u    //�ŵ��������ʱ3000ms
#define s_t 0u      


/*********************************************************/
/*MD4��ز���*/
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
	ReReq = 6212u,       //�ش�����
	ReResp = 6213u,      //�ش���Ӧ
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
	MyUINT32 SeqTX;          //��һ�����͵����к�
	MyUINT32 SeqRX;          //��һ�����յ�Ԥ�����к�
	MyUINT32 TSEQ[DEFERQUEUENUM];       //���ڶ��м�س�ʱ
	MyUINT32  TInitResq;   //�µ�ͨ�����м�س�ʱ
}S_RedunSeq;

typedef struct
{
	MyUINT32 SNT;          //��һ�����͵����к�
	MyUINT32 SNR;          //��һ�����յ�Ԥ������
}S_SafeSeq;

typedef struct
{
	MyUINT32 Tlocal;//���ص�ǰʱ��
	MyUINT32 TLastRecv;//�ϴν��յ���ȷ���ݵ�ʱ��
	MyUINT32 Tlastrtd; //��Ϣ����ʱ��
	MyUINT32 TI;   //���Ӽ�ؼ�ʱ��
	MyUINT32 Th;  //������ʱ��
	MyUINT32 Tstart;  //���ӿ�ʼʱ��
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
	MyUINT16 UnConfirmedNum; //δ��ȷ�Ϸ�����Ϣ�ļ�����
	MyUINT16 RecvMsgNum;     //������Ϣ������
	MyUINT16 peerbuffsize;   //�Է��Ļ����С
	MyUINT16 DeferNum;
	MyUINT8 ErrorCount;
	S_Buf RedunRecvMsg, RedunSendMsg;
	S_Buf SafeRecv, SafeSend;
	S_Buf DeferMsg[DEFERQUEUENUM];  //���ڶ���
	S_Buf ReTx,ReTx1; //�ش�����
	S_Buf SendUsrData, SendUsrDataMem/*�û������ݴ���*/, RecvUsrData, ReUsrData;
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


