/******************************************************************************
 * File Name		:	parameter.h
 * Copyright		:	Zhuzhou CSR Times Electric Co.,Ltd. All Rights Reserved.
 * Create Data	:	2014��1��21�����ڶ�
  *Description   : �������������Ҫ��ͨ�ò���
 * 
 * 
 * Rev1.0.0	yzb	2014��1��21�����ڶ�	File Create
 *
******************************************************************************/
  
/******************************************************************************
*    Multi-Include-Prevent Start Section
******************************************************************************/
#ifndef _PARAMETER_H_
#define _PARAMETER_H_
/******************************************************************************
*    Debug switch Section
******************************************************************************/
#define PCSIM				1
#define MIDRETRLS			1
/******************************************************************************
*    Include File Section
******************************************************************************/
#include <stdio.h>


/******************************************************************************
 *     Global Macro Define Section*
 ******************************************************************************/

#define MAX_SWIT_SPLITCOUNT 81u/*������������*/


#define MAX_FIRFLKPELE_NUM 5u /*���һ�����Ԫ������*/
#define MAX_FIRFLKCELE_NUM 5u /*���һ���������Ԫ������*/
#define MAX_SECFLKPELE_NUM 3u /*���������Ԫ������*/
#define MAX_SECFLKCELE_NUM 3u /*�������������Ԫ������*/


#define MAX_DWELL_NUM		24u	/*ͣ������Ϣ*/

#define MAX_INBOOL_NUM	40u
#define MAX_OUTBOOL_NUM	30u


#define ROUTE_INTEVAL	0xFFFFu	/*��·�����*/

#define YX_ROUTE			0x0002u	/*��·������*/
#define FYX_ROUTE			0x0000u	/*��·��������*/
#define CLINE				0x0004u	/*����*/
#define ZLINE				0x0000u	/*����*/
#define DZ					0x0008u	/*����*/
#define LZ					0x0000u	/*�г�*/
#define X_DIR				0x0010u	/*���з���*/
#define S_DIR				0x0000u	/*���з���*/
#define ARRIVE_ROUTE		0x0020u	/*�ӳ���·*/
#define DEPART_ROUTE		0x0040u	/*������·*/
#define OTHER_ROUTE			0x0000u	/*��ͨ��·(�����ֽӷ���)*/
#define ONE_RLS				0x0100u/*һ�ν���*/
#define THREE_RLS			0x0000u/*�ֶν���*/
#define ABLE_CALLON			0x0200u/*��·�߱���������*/
#define DISABLE_CALLON		0x0000u/*��·���߱���������*/
#define ZDTG_ROUTE			0x0080u/*�Զ�ͨ����·*/
#define ZRJL_ROUTE      	0x4000u/*�����·*/
#define LZGD_ROUTE			0x1000u/*�������г���ʽ�ص�*/
#define TSDC_ROUTE			0x2000u/*���������·*/

#define YX_TO_BLOCK         0x1000u/*����������*/
#define YX_TO_SAFETY        0x2000u/*��������ȫ��*/
#define YX_NO				0xFF07u

#define CBTC_SECT			0x2000u/*CBTC ���α�־�����������*/
#define DELAY_SECT			0x4000u/*������ʱ��־*/
#define NOOCCUPIED_SECT		0x8000u/*����ռ�ò�����־*/
#define ILK_APPRSECT		0x1000u	/*�������нӽ����α�־*/
#define ILK_LEAVESECT		0x0800u	/*����������ȥ���α�־*/
#define ILK_SECT_SWITCH     0x0400u	/*��������*/

#define ILK_OTH_AUTOREOPEN		0x0400u 	/*�������������㣬���Զ��ؿ���־*/
#define ILK_OTH_NOCHACK			0x0800u 	/*�������������㣬���ű�־*/
#define ILK_OTH_CBTCNOCHECK       0x0200  //cbtc�²�������������


#define UNLOCK				0xAAu/*����*/
#define MPLOCK				0x55u/*�����뱣����*/
#define MLOCK				0x5Au/*����*/
#define PLOCK				0xA5u/*������*/
#define CALOCK              0x53u/*��������*/
#define CALOCK2             0x77u/*��������*/

#define RLOCK				0x55u
#define DZLOCK				0x5Fu/*��������*/
#define DZ2LOCK				0x66u/*����2������*/
#define LZSLOCK				0xF5u/*�г���������*/
#define LZXLOCK				0x75u/*�г���������*/

#define YSLOCK				0xB5u/*����������*/
#define YXLOCK				0x5Bu/*����������*/

#define REQUEST				0x55u/*����*/
#define LZSREQ				0xF5u/*��������*/
#define LZXREQ				0x75u/*��������*/
#define UNREQUEST			0xAAu/*δ����*/

#define BLOCK				0x55u	/*����*/
#define UNBLOCK				0xAAu	/*δ����*/

#define PROHIBIT			0x55u	/*��ֹ����*/
#define UNPROHIBIT			0xAAu	/*δ��ֹ����*/

#define OCCUPIED			0x55u	/*ռ�� �̵�������*/
#define CLEAR				0xAAu	/*���� �̵�������*/

#define RELAY_DOWN			0x55u
#define RELAY_UP			0xAAu

#define HOLD				0x55u	/*�۳�*/
#define UNHOLD				0xAAu	/*δ�۳�*/

#define CLOSE				0xAAu	/*�ر�*/
#define OPEN				0x55u	/*��*/

#define RSET				0x5555u	/*��·����*/
#define	RCLEAR				0xAAAAu	/*��·δ����*/

#define MUTLOCK				0xAAu	/*����*/
#define UNMUTLOCK			0x55u	/*�������*/	

#define NORMAL				0x55u	/*����λ*/
#define REVERSE				0xAAu	/*����λ*/
#define SIKAI				0x5Au	/*�����Ŀ�*/
#define SWITFAULT           0xA5u   /*�������*/

#define ILK_REVERSE			0x1000u		/*�������е���λ(��λ)*/
#define	ILK_NORMAL			0x0000u		/*�������е���λ(��λ)*/
#define ILK_RIGHT			0x2000u		/*��λ*/

#define CBTCSIGN			0x55u	/*CBTC �ź�*/
#define IATPSIGN			0xAAu	/*IATP �ź�*/

#define YES					0x55u	/**/
#define NO					0xAAu	/**/

#define TSR_I				0x55u //��߼�
#define TSR_II				0x75u
#define TSR_III				0x95u
#define TSR_NO				0xAAu

#define BTN_UP				0x55u	/*��ť����*/
#define BTN_DOWN			0xAAu	/*��ť����*/

#define DRVING				0x55u	/*������*/
#define NODRV				0xAAu	/*δ����*/

#define SIGNON				0x55u	/*�źſ���*/
#define SIGNOFF				0xAAu	/*�źŹر�*/

#define FLANKPROT			0x55u	/*�������*/
#define UNFLANKPROT			0xAAu	/*���������*/

#define YDY_FLAG			0x01u	/*������ʱ��־*/
#define ZRJ_FLAG			0x02u	/*���˽��־*/
#define ZQX_FLAG			0x03u	/*��ȡ����־*/
#define QGJ_FLAG			0x04u	/*���ʽ��־*/

#define UDELAY				0x00u	/*����ʱ*/
#define THREE_M				1125u	/*3 ������ʱ*/
#define THIRTY_S			188u	/*30����ʱ*/
#define FOURTEEN_S			88u		/*14��*/
#define THREE_S				18u		/*3 ����ʱ*/
#define EIGHT_M				3000u	/*8 ���Ӽ�ʱ*/
#define FIVEH_MS			6u		/*500MS��ʱ*/

#define ROUTE_CALL			0x01	/*��·ʱ����*/
#define ALOCK_CALL			0x02	/*����������*/

#define FLT_OCCUPIED		0x55u	/*��ռ��.������·���պ�����ռ��*/
#define FLT_BOCCUPIED		0xA5u	/*Ԥռ��.������·����ǰ����ռ��*/
#define FLT_CLEAR			0xAAu	/*����*/

#define SWIT_TIME_INIT       	81 /*��������ʼֵ*/
#define OLP_TRIGGER_TIME		375u/*1���ӣ��������δ���ʱ��*/

#define RED                  		0x55u
#define GREEN                		0xAAu
#define YELLOW               		0xA5u
#define BLUE                 		0x5Au
#define WHITE                		0x3Cu
#define DOUBLEGREEN					0xC3u
#define DOUBLEYELLOW				0x69u
#define GREENYELLOW					0x96u
#define REDYELLOW            		0x66u
#define REDWHITE					0x99u
#define DZDS1F						0x88u	/*ͨ�еƵ�˿��˿*/
#define DS1F		           		0x33u
#define DS2F			    		0xCCu
#define YINDAO						0x77u
#define NOINDICATION				0xFFu
#define CTRCLR                      0XBBu//�Զ�������ɫ�ɱ��־
#define BSD_RED						0X88u//ת�̻�غ���ʾ�Ƶ�����ɫ
#define BSD_GRAY					0XEEu//ת�̻�غ���ʾ�������ɫ

#define FOLLOW_SWITCH				0x01u	
#define PROT_SWITCH					0x02u
#define COM_SWITCH					0x03u
#define OVERLAP_SWITCH				0x04u

#define APPRO_SWITCH				0x06u

#define COMMON_ROUTE				0x00u
#define FAST_ROUTE					0x01u
#define AUTO_ROUTE					0x02u
#define ATB_ROUTE                   0x03u

#define CROSS						0x55u
#define UNCROSS						0xAAu

#define MASTER_COMPUTER				0x55u
#define SLAVE_COMPUTER				0xAAu

#define SG_INIT				1u/*��̬*/
#define SG_FAULT			2u/*���Ϲر�*/
#define SG_NORM				3u/*�����ر�*/

/*�źŻ���ʾ��Ϣ����*/
#define I_SG_XHFS		1u/*�źŷ���*/
#define I_SG_XHJF		2u/*�źŽ��*/
#define I_SG_XHCK		3u/*�ź��ؿ�*/
#define I_SG_SZZDCF		4u/*�����Զ�����*/
#define I_SG_QXZDCF		5u/*ȡ���Զ�����*/
#define I_SG_SZZDTG		6u/*�����Զ�ͨ��*/
#define I_SG_QXZDTG		7u/*ȡ���Զ�ͨ��*/
#define I_SG_TJBMZ		8u/*����������*/
#define I_SG_CZCW		9u/*��������*/
#define I_SG_LCSB		10u/*�Ǳ����豸*/
#define I_SG_IOCW		11u/*�źŻ�IO���ô���*/
#define I_SG_JFQQ      12u/*�źŻ��������*/
#define I_SG_YDXHTJBMZ		13u/*�����źſ�������������*/
#define I_SG_YDXHBNBC		14u/*�����źŲ��ܱ���*/
#define I_SG_SZBHQD1    15u/*���ñ�������1*/
#define I_SG_SZBHQD2    16u/*���ñ�������2*/
#define I_SG_SZBHQD3    17u/*���ñ�������3*/
#define I_SG_QXBHQD     18u/*ȡ����������*/
#define I_SG_PDJ        19u/*�µ���*/

/*������ʾ��Ϣ����*/
#define I_SW_DCDS		1u/*������*/
#define I_SW_DCDJ		2u/*������*/
#define I_SW_DCDF		3u/*�������*/
#define I_SW_DCJF		4u/*������*/
#define I_SW_DCDC		5u/*������*/
#define I_SW_DCFC		6u/*������*/
#define I_SW_DCBSB		7u/*��������*/
#define I_SW_DCQDBZY	8u/*�������α�ռ��*/
#define I_SW_TJBMZ		9u/*����������*/
#define I_SW_CZCW		10u/*��������*/
#define I_SW_LCSB		11u/*�Ǳ����豸*/
#define I_SW_SZLSXSCG   12u/*������ʱ���ٳɹ�*/
#define I_SW_QXLSXSCG   13u/*ȡ����ʱ���ٳɹ�*/
#define I_SW_TYDC       14u/*ͬ�⶯��*/
#define I_SW_QXTYDC     15u/*ȡ��ͬ�⶯��*/

/*������ʾ��Ϣ����*/
#define I_SE_QDFS		1u/*���η���*/
#define I_SE_QDJF		2u/*���ν��*/
#define I_SE_QDYFW	3u/*����Ԥ��λ*/
#define I_SE_LSXS		4u/*��ʱ����*/
#define I_SE_QDKX		5u/*���ο��в���Ԥ��λ*/
#define I_SE_QGJ		6u/*���ʽ�*/
#define I_SE_TJBMZ		7u/*����������*/
#define I_SE_CZCW		8u/*��������*/
#define I_SE_LCSB		9u/*�Ǳ����豸*/

/*�߼�������ʾ��Ϣ*/
#define I_TS_SZLSXSCG   1u/*������ʱ���ٳɹ�*/
#define I_TS_QXLSXSCG   2u/*ȡ����ʱ���ٳɹ�*/
#define I_TS_TJBMZ		3u/*����������*/

/*Cross��ʾ��Ϣ*/
#define I_CR_SZLSXSCG   1u/*������ʱ���ٳɹ�*/
#define I_CR_QXLSXSCG   2u/*ȡ����ʱ���ٳɹ�*/
#define I_CR_TJBMZ       3u/*����������*/

/*��·��ʾ��Ϣ����*/
#define I_RT_JLXP			1u/*��·ѡ��*/
#define I_RT_JLYD			2u/*��·����*/
#define I_RT_ZSBYD			3u/*����������*/
#define I_RT_ZQX			4u/*��·��ȡ��*/
#define I_RT_ZRJ			5u/*��·���˽�*/
#define I_RT_WCTJL			6u/*�޴�����·*/
#define I_RT_QXTJBMZ		7u/*��������������*/
#define I_RT_ESBTJBMZ		8u/*��ͣ����������*/
#define I_RT_PSDTJBMZ		9u/*����������������*/
#define I_RT_AFGTJBMZ		10u/*����������������*/
#define I_RT_ZCTJBMZ		11u/*�ղ�����������*/
#define I_RT_JLBWZ			12u/*��·������*/
#define I_RT_QDBZY			13u/*���α�ռ��*/
#define I_RT_JLDDSB			14u/*��·�ж�����*/
#define I_RT_QDBFS			15u/*���α�����*/
#define I_RT_XHBFS			16u/*�źű�����*/
#define I_RT_DCBDS			17u/*��������*/
#define I_RT_DCBFS			18u/*��������*/
#define I_RT_CFTJBMZ		19u/*�������������*/
#define I_RT_BHQDTJBMZ		20u/*������������������*/
#define I_RT_XHBNKF			21u/*�źŲ��ܿ���*/
#define I_RT_XHBNBC			22u/*�źŲ��ܱ���*/
#define I_RT_ATPJZJS		23u/*ATP��ֹ����*/
#define I_RT_TJBMZ			24u/*����������*/
#define I_RT_XHKF			25u/*�źſ���*/
#define I_RT_XHGZGB			26u/*�źŹ��Ϲر�*/
#define I_RT_XHGB			27u/*�źŹر�*/

/*վ̨��ʾ��Ϣ����*/
#define I_ST_SZKC			1u/*���ÿ۳�*/
#define I_ST_QXKC			2u/*ȡ���۳�*/
#define I_ST_LCSB		3u/*�Ǳ����豸*/

/*���ܰ�ť��ʾ��Ϣ����*/
#define I_FB_SDJS			1u/*�ϵ����*/
#define I_FB_SDJSCS			2u/*�ϵ������ʱ*/
#define I_FB_QCJF			3u/*ȫ�����*/
#define I_FB_QZLSXSJC		4u/*ȫվ��ʱ���ٽ��*/
#define I_FB_SZWXDD			5u/*����ά�޵��*/
#define I_FB_QXWXDD			6u/*ȡ��ά�޵��*/
#define I_FB_SZATB			7u/*�����Զ��۷�*/
#define I_FB_QXATB			8u/*ȡ���Զ��۷�*/
#define I_FB_MLBNZX			9u/*�����ִ��*/

#define I_FB_QQSC			10u/*�����Գ�*/
#define I_FB_KSSC			11u/*��ʼ�Գ�*/
#define I_FB_JSSC			12u/*�����Գ�*/
#define I_FB_TYSC			13u/*ͬ���Գ�*/
#define I_FB_ZDSC			14u/*�ж��Գ�*/
#define I_FB_SZFJLDC       15u/*���÷ǽ�·����*/
#define I_FB_FJLGZJS       16u/*�ǽ�·���Ͻ���*/

/*������ʾ��Ϣ*/
#define I_OT_CZCW			1u/*��������*/

/*��������ID��ʶ*/
#define A_SWIT_DCJC		1u/*������*/
#define A_SWIT_JCHF		2u/*����ػָ�*/
#define A_SIGN_DSDS		1u/*��˿��˿*/
#define A_SIGN_DSHF		2u/*��˿�ָ�*/

#define I_FUNC_ID			0xFFu/*���ܰ�ť������ʾ��ID��ʶĬ��Ϊ0xFFu*/

#define QCTYPE2					0x77u	/**/
#define ZC_FLAG					0x01u	/*����*/
#define HT_FLAG					0x02u	/*����*/
#define GZ_FLAG					0x03u	/*����*/

#ifndef NULL
#define NULL ((void*)0)
#endif

#define c_SignalTotalNum 100u
#define c_SwitchTotalNum 100u
#define c_SectionTotalNum 100u

#define FIRST_SYS_ID	0x01ul	/*I ϵ*/
#define SECOND_SYS_ID	0x02ul	/*II ϵ*/

#define TAG_MYMODE_AUTO  0x00ul //�Զ�
#define TAG_MYMODE_MAN_I  0x01ul //�˹�Iϵ
#define TAG_MYMODE_MAN_II  0x02ul //�˹�IIϵ

#define MAXSYSHALTNUMB 3u   /*崻���������*/

#define FUN_NO_USRDATAWATCH 1u
#define FUN_NO_USRDATARECV 2u
#define FUN_NO_USRDATARECVEX 3u
#define FUN_NO_USRDATARECVMUTUAL 4u
#define FUN_NO_USRDATASEND 5u
#define FUN_NO_USRDATASENDEX 6u
#define FUN_NO_USRDATASENDMUTUAL 7u
#define FUN_NO_INQUIRYDEVSTATUS 8u
#define FUN_NO_INQUIRYSWVERSION 9u
#define FUN_NO_GETSYSTEMID 10u
#define FUN_NO_GETSWID 11u
#define FUN_NO_GETMASTERID 12u
/******************************************************************************
 *    Global Struct    Define Section*
 ******************************************************************************/

typedef unsigned char  MyUINT8;
typedef unsigned short MyUINT16;
typedef unsigned int   MyUINT32;
typedef signed int	   MyINT32;
typedef signed char    MyINT8;

typedef enum YLinkPropty
{
	YL_IsCurNot   = 0, /*�����ǳ����豸*/
	YL_IsCurLink  = 1, /*���������豸*/
	YL_IsNotCur   = 2,  /*�Ǳ����豸*/
	YL_IsNearNot  = 3,/*�ٳ��ǳ����豸*/
	YL_IsNearLink = 4, /*�ٳ������豸*/
	YL_IsNotNear  = 5  /*���ٳ��豸*/

}YLinkPropty;

typedef enum MyBOOL
{
	e_UsrFalse = 0,
	e_UsrTrue = 1
}MyBOOL;

typedef enum E_DevType
{
	E_SECTION, E_SWITCH, 	E_SIGNAL, E_TRACKSECTION,	E_CROSSING,	E_FUNCBTN
}E_DevType;

typedef enum E_DrvState
{
	e_Fail = -1,
	e_Driving = 0,
	e_Success = 1,
	E_XHFS = 2,	/*�źŷ���*/
	E_DSDS = 3,	/*��˿��˿*/
	E_QDZY = 4,	/*����ռ��*/
	E_QDFS = 5,	/*���η���*/
	E_CWSB = 6,	/*��������*/
	E_DCSB = 7,	/*����ʧ��*/
	E_QXTJ = 8,	/*��������*/
	E_CLTJ = 9, /*��������*/
	E_TYTJ = 10, /*ͬ������*/
	E_ESBTJ = 11, /*ESB����*/
	E_PSDTJ = 12, /*PSD����*/
	E_HOLDTJ = 13, /*�۳�����*/
	E_AFDTJ = 14,/*����������*/
	E_DDXH = 15,/*�ж��ź�*/
	E_CFTJ = 16,/*�������*/
	E_CZCW = 17,/*��������*/
	E_KYTJ = 18,/*��ѹ����*/
	E_ATJJ = 19,/*AT�ӽ�*/
	E_BHQDTJ = 20, /*������������*/
	E_DYQDZY = 21,		/*��һ����ռ��*/
	e_JLDDSB = 22,		/*��·�ж�����*/
	e_JLDDZY = 23,		/*��·�ж�����*/
	e_JLBHDDZY = 24,	/*��·�����ж�����*/
	e_JLBHDDSB = 25,	/*��·�����ж�����*/
	e_JLJKQDZY = 26,	/*��·�������ռ��*/
	e_JLJKQDSB = 27,	/*��·�����������*/
	e_JLFJKDDSB = 28,/*��·�Ǽ�����εж�����*/
	e_QDTSR = 29,/*������ʱ����*/
	e_ZDXH = 30/*�赲�ź�*/
}E_DrvState;

typedef struct LockAddr
{
	MyUINT8 table;
	MyUINT8 offset;
}LockAddr;

typedef struct MutiSection
{
	MyUINT16 nSection_;
	char sectionName_[3][16];
	MyUINT16 sectionAddr_[3];
	MyUINT16 relayAddr_[3];
}MultiSect;

typedef struct RtAddrData
{
   	MyUINT16 StartBtn;/*��·ʼ�˰�ť*/
	MyUINT16 alterBtn[2];/*�����ť��*/
   	MyUINT16 EndBtn;/*��·�ն˰�ť*/
   	MyUINT16 BlkSign;/*�赲�źŻ�����*/
   	MyUINT16 RtProp; /*��·����*/
   	MyUINT16 BlkAddr;/*�赲�źŻ���·�ڽ�·��ַ���еĵ�ַ*/
   	LockAddr RtAddr;/*�����+��ƫ����*/
}RtAddrData;

typedef struct ApprOneDataEX
{
	MyUINT16 sectIndex;
	MyUINT16 devType; //����豸����
	MyUINT16 devIndex; //����豸
	MyUINT16 devPos; //λ��
}ApprOneDataEX;

typedef struct PSDCfg
{
	MyUINT16 psdRelayIndex[6];

}PSD_Cfg;

typedef struct AUTO_ZONE_Dev_CFG_DATA
{
	MyUINT16 devIndex;
	MyUINT16 isAutoZone;

}AutoZoneCfg;

typedef struct AUTO_ZONE_Dev_ST_DATA
{
	MyUINT16 devIndex;
	MyUINT8  isAutoZone;

}AutoZoneDevice;

typedef struct ApprSectionDataEX
{
	MyUINT16 signIndex;
	MyUINT16 time;
	MyUINT16 numApprSection;
	ApprOneDataEX apprDev[10];

}ApprSectionDataEX; //������ӽ����α�

typedef struct TurnTableAddr
{
	MyUINT16 WZJAddr;	    /*in��������ɢ��ʾ����index*/
	MyUINT16 XHBSJOutAddr;/*Out���������ϵĵ�ַ*/
	MyUINT16 ZBQC1JAddr;	/*Out���������ϵĵ�ַ*/
}TurnTableAddr;

#define COM_RSSPI_PEER_CI 1u
#define COM_RSSPI_PEER_ZC 2u
#define COM_RSSPI_PEER_DMS 3u
#define COM_RSSPI_PEER_VOBC 4u
typedef struct RSSP_COM_CfgData
{
	MyUINT16    peerType; //ͨ�ŷ�����
	MyUINT16	VcpDevId;	//����������ɵ�����ID
	MyUINT16	Peer_RsspID;//��վ��Ӧ��Э��ID
}RsspComCfgData;

typedef struct RSSP_COM_CFG
{
	MyUINT16	VcpDevId;	//����������ɵ�����ID
	MyUINT16	Peer_RsspID;//��վ��Ӧ��Э��ID
}RSSP_COM_CFG;

typedef struct COMM_RSP_CFG
{
	MyUINT16    peerType; //ͨ�ŷ�����
	MyUINT16	Peer_RsspID; //ͨ�ŷ�ID
	MyUINT16	VcpId_A;//A��Vcp
	MyUINT16	VcpId_B;//B��Vcp
}CommRspCfg;
/******************************************************************************
 *    Global Variable Declare Section
 ******************************************************************************/

/******************************************************************************
*    Global Prototype Declare Section
 ******************************************************************************/

/******************************************************************************
 *    Multi-Include-Prevent End Section
 ******************************************************************************/
#endif

