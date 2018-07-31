#ifndef _SAHARA_H_
#define _SAHARA_H_

//#define VCP//��ƽ̨������Ҫע�͸þ�

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

//#include "parameter.h"

#define VERSION   0x32303230
#define BUFF_SIZE 10240
#define USERDATA_SIZE 38*7    //һ��WCUԪ�ش�СΪ38���ֽ�

#define MSG_CRC_MASK 0xA001//crc����


#define DEFERQUEUENUM 1 //���ڶ����� �������ӽ���ö���������Ϊ1��
#define RealLinkNum 1  //ʵ����·��
#define SelfBuffSize 3   //���ջ����С
#define MAX_SEND_NUM  3//���������

#define InitResp  10     //��ʼ����Ӧ��س�ʱ    2200ms
#define SEQ 1         //���ڶ��м�س�ʱ    500ms

#define TH 1        //��������Ϊһ����������
#define Tmax 19    //�ŵ��������ʱ3000ms
#define s_t 0      
#define  selfaddress   0x05e10000
#define  peeraddress   0x05ff0000

/*********************************************************/
/*MD4��ز���*/
#define F(x,y,z) (((x)&(y))|((~x)&(z)))
#define G(x,y,z) (((x)&(y))|((x)&(z))|((y)&(z)))
#define H(x,y,z) ((x)^(y)^(z))

#define FF(a,b,c,d,k,s) a=RL(a+F(b,c,d)+X[k],s)
#define GG(a,b,c,d,k,s) a=RL(a+G(b,c,d)+X[k]+0x5a827999,s)
#define HH(a,b,c,d,k,s) a=RL(a+H(b,c,d)+X[k]+0x6ed9eba1,s)


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
	r_Init = 0,
	r_Open = 1,
	r_Close = 2
}RedunNextState;

typedef enum SafeNextState
{
	s_Close = 0,
	s_Down = 1,
	s_Start = 2,
	s_Up = 3,
	s_ReReq = 4,
	s_ReRun = 5
}SafeNextState;


typedef enum MyBOOL
{
	e_UsrFalse = 0, e_UsrTrue = 1
}MyBOOL;

typedef enum MsgType
{
	ConnReq = 6200,
	ConnResp = 6201,
	ReReq = 6212,       //�ش�����
	ReResp = 6213,      //�ش���Ӧ
	DisconnReq = 6216,
	Heartbeat = 6220,
	Data = 6240,
	ReData = 6241
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
	MyUINT16  TSEQ[DEFERQUEUENUM];       //���ڶ��м�س�ʱ
	MyUINT16  TInitResq;   //�µ�ͨ�����м�س�ʱ
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
}S_Msg;

typedef struct
{
	MyBOOL IsInitTActive;
	MyUINT16 UnConfirmedNum; //δ��ȷ�Ϸ�����Ϣ�ļ�����
	MyUINT16 RecvMsgNum;     //������Ϣ������
	MyUINT16 peerbuffsize;   //�Է��Ļ����С
	MyUINT16 DeferNum;
	MyUINT32 ErrorCount;
	S_Msg RedunRecvMsg, RedunSendMsg;
	S_Msg SafeRecv, SafeSend;
	S_Msg DeferMsg[DEFERQUEUENUM];  //���ڶ���
	S_Msg ReTx,ReTx1; //�ش�����
	S_Msg SendUsrData, SendUsrDataMem/*�û������ݴ���*/, RecvUsrData, ReUsrData;
	S_RedunSeq R_Seq;
	S_SafeSeq S_Seq;
	S_Time Time;
	S_MsgRedun RecvMsg, SendMsg;
	S_MsgSafe SafeRecvMsg, SafeSendMsg, RightSafeRecvMsg, Redata;
	MyUINT8 RedunNextState;
	MyUINT8 SafeNextState;
}S_ProtocolControl;

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
MyUINT16 F_Sahara_CheckData(const MyUINT16 inDstDevId, const MyUINT16 inlen, MyUINT8 *inbuf,
	MyUINT16 *outlen, MyUINT8 *outbuf);
MyUINT16 F_Sahara_SendData(const MyUINT16 inDstDevId, const MyUINT16 inlen, MyUINT8 *inbuf, MyUINT16* outlen, MyUINT8* outbuff);
#endif


