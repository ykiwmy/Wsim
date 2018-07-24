/******************************************************************************
** Copyright (c)2017�����г�ʱ��ͨ���ź����޹�˾
** �ļ���:  Rsp1_Interface.h
** �� ��:   Rssp1�ӿ�
**
**------------------------------������汾�޸ļ�¼-------------------------------
**  V1.0.0      ����: 2017.12.27 �޸���: ����
**  �޸�����:   ����
*******************************************************************************/

/**------------------------------�����޸ļ�¼-----------------------------------
**  V1.0.1  ����:                 �޸���:
**  �޸�����:
*******************************************************************************/
#ifndef _RSP1_INTERFACE_H_
#define _RSP1_INTERFACE_H_

#include "parameter.h"
#include "VCP_Service_Interface.h"
#include "VCP_File_Interface.h"
//#include "Vcp_Device_Definition_Inf.h"
//#include "COM_VCPInterface.h"
#include "Rsp1.h"
#define MAX_LINK_NUM     20u

#ifdef VCP
#define RSSP1CFG_PATH  "RspCfg.bin"
#else
#define RSSP1CFG_PATH  "..\\Cfg\\RspCfg.bin"
#endif
#define TYPE_CI  0x3Cu
#define TYPE_ZC  0x49u   //����

 
#define RSSP_RSD     0        //����rssp1 Rsd������
#define RSSP_LEU_RSD 1			//LEU����rssp1 Rsd������
#define RSSP_LEU_BTM 2		 //LEU����rssp1 Btm������


#define LEU_PIPE_NUM 5U        //LEU�շ���ܵ�
//typedef struct RSSP_COM_CFG
//{
//	MyUINT16    peerType; //ͨ�ŷ�����
//	MyUINT16	VcpDevId;	//����������ɵ�����ID
//	MyUINT16	Peer_RsspID;//��վ��Ӧ��Э��ID
//}RSSP_COM_CFG;


void F_RSSP1_SetComCfg(MyUINT16 index, RSSP_COM_CFG ncicfg);
void F_RSSP1_SetSelfRsspID(MyUINT16 rsspId);
void F_RSSP1_PackDataInOneFrame(MyUINT16 LinkIndex, MyUINT8 type, MyUINT8* inbuff, MyUINT16 inlen, MyUINT8* outbuff, MyUINT16* outlen);
void F_RSSP1_PackLeuData(MyUINT16 LinkIndex, MyUINT16 VcpID, MyUINT8* inbuff, MyUINT16 inlen);
void F_RSSP1_CheckLeuData(MyUINT16 LinkIndex, MyUINT16 VcpID, MyUINT8* outbuff, MyUINT16* outlen);
void F_RSSP1_CheckData(MyUINT16 LinkIndex, MyUINT16 VcpID, E_UsrRxStatus* bRecv, MyUINT8* outbuff, MyUINT16* outlen);
#endif