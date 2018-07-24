/******************************************************************************
** Copyright (c)2017湖南中车时代通信信号有限公司
** 文件名:  Rsp1_Interface.h
** 描 述:   Rssp1接口
**
**------------------------------基础版版本修改记录-------------------------------
**  V1.0.0      日期: 2017.12.27 修改人: 尹琨
**  修改内容:   创建
*******************************************************************************/

/**------------------------------特殊修改记录-----------------------------------
**  V1.0.1  日期:                 修改人:
**  修改内容:
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
#define TYPE_ZC  0x49u   //待定

 
#define RSSP_RSD     0        //发送rssp1 Rsd包类型
#define RSSP_LEU_RSD 1			//LEU发送rssp1 Rsd包类型
#define RSSP_LEU_BTM 2		 //LEU发送rssp1 Btm包类型


#define LEU_PIPE_NUM 5U        //LEU收发多管道
//typedef struct RSSP_COM_CFG
//{
//	MyUINT16    peerType; //通信方类型
//	MyUINT16	VcpDevId;	//配置软件生成的配置ID
//	MyUINT16	Peer_RsspID;//临站对应的协议ID
//}RSSP_COM_CFG;


void F_RSSP1_SetComCfg(MyUINT16 index, RSSP_COM_CFG ncicfg);
void F_RSSP1_SetSelfRsspID(MyUINT16 rsspId);
void F_RSSP1_PackDataInOneFrame(MyUINT16 LinkIndex, MyUINT8 type, MyUINT8* inbuff, MyUINT16 inlen, MyUINT8* outbuff, MyUINT16* outlen);
void F_RSSP1_PackLeuData(MyUINT16 LinkIndex, MyUINT16 VcpID, MyUINT8* inbuff, MyUINT16 inlen);
void F_RSSP1_CheckLeuData(MyUINT16 LinkIndex, MyUINT16 VcpID, MyUINT8* outbuff, MyUINT16* outlen);
void F_RSSP1_CheckData(MyUINT16 LinkIndex, MyUINT16 VcpID, E_UsrRxStatus* bRecv, MyUINT8* outbuff, MyUINT16* outlen);
#endif