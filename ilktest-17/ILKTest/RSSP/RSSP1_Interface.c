/******************************************************************************
** Copyright (c)2017湖南中车时代通信信号有限公司
** 文件名:  Rsp1_Interface.c
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


#include "RSSP1_Interface.h"

typedef struct TAG_DeviceInf
{
	UINT16 Type; /**/
	UINT8  AppId; /**/
	UINT8  SysId; /**/
}T_DeviceInf;

#define c_DeviceItemTotal (34UL)
#define      SME_A1  (0UL)
#define      MIC_A1  (1UL)
#define      MIC_A2  (2UL)
#define      MIC_A3  (3UL)
#define      MIC_A4  (4UL)
#define      SDI_A1  (5UL)
#define      SDI_A2  (6UL)
#define     SDOV_A1  (7UL)
#define      SME_B1  (8UL)
#define      MIC_B1  (9UL)
#define      MIC_B2  (10UL)
#define      MIC_B3  (11UL)
#define      MIC_B4  (12UL)
#define      SDI_B1  (13UL)
#define      SDI_B2  (14UL)
#define     SDOV_B1  (15UL)
#define     TGMT_A1  (16UL)
#define    OD_GB_A1  (17UL)
#define      OD1_A1  (18UL)
#define      OD2_A1  (19UL)
#define      OD3_A1  (20UL)
#define      OD4_A1  (21UL)
#define     WSIM_A1  (22UL)
#define    NCI_1_A1  (23UL)
#define    NCI_2_A1  (24UL)
#define     TGMT_B1  (25UL)
#define    OD_GB_B1  (26UL)
#define      OD1_B1  (27UL)
#define      OD2_B1  (28UL)
#define      OD3_B1  (29UL)
#define      OD4_B1  (30UL)
#define     WSIM_B1  (31UL)
#define    NCI_1_B1  (32UL)
#define    NCI_2_B1  (33UL)

extern const T_DeviceInf c_DeviceInfTable[c_DeviceItemTotal] =
{
	{ 0x0301, 0x01, 0x01 },//平台内设备名称[SME_A1],插件位置[1],机箱号[1]
	{ 0x0302, 0x01, 0x01 },//平台内设备名称[MIC_A1],插件位置[2],机箱号[1]
	{ 0x0302, 0x02, 0x01 },//平台内设备名称[MIC_A2],插件位置[3],机箱号[1]
	{ 0x0302, 0x03, 0x01 },//平台内设备名称[MIC_A3],插件位置[4],机箱号[1]
	{ 0x0302, 0x04, 0x01 },//平台内设备名称[MIC_A4],插件位置[10],机箱号[1]
	{ 0x0307, 0x01, 0x01 },//平台内设备名称[SDI_A1],插件位置[11],机箱号[1]
	{ 0x0307, 0x02, 0x01 },//平台内设备名称[SDI_A2],插件位置[12],机箱号[1]
	{ 0x0306, 0x01, 0x01 },//平台内设备名称[SDOV_A1],插件位置[13],机箱号[1]
	{ 0x0301, 0x01, 0x02 },//平台内设备名称[SME_B1],插件位置[1],机箱号[1]
	{ 0x0302, 0x01, 0x02 },//平台内设备名称[MIC_B1],插件位置[2],机箱号[1]
	{ 0x0302, 0x02, 0x02 },//平台内设备名称[MIC_B2],插件位置[3],机箱号[1]
	{ 0x0302, 0x03, 0x02 },//平台内设备名称[MIC_B3],插件位置[4],机箱号[1]
	{ 0x0302, 0x04, 0x02 },//平台内设备名称[MIC_B4],插件位置[10],机箱号[1]
	{ 0x0307, 0x01, 0x02 },//平台内设备名称[SDI_B1],插件位置[11],机箱号[1]
	{ 0x0307, 0x02, 0x02 },//平台内设备名称[SDI_B2],插件位置[12],机箱号[1]
	{ 0x0306, 0x01, 0x02 },//平台内设备名称[SDOV_B1],插件位置[13],机箱号[1]
	{ 0x0001, 0x01, 0x01 },//平台外设备名称[TGMT_A1]
	{ 0x0002, 0x01, 0x01 },//平台外设备名称[OD_GB_A1]
	{ 0x0003, 0x01, 0x01 },//平台外设备名称[OD1_A1]
	{ 0x0004, 0x01, 0x01 },//平台外设备名称[OD2_A1]
	{ 0x0005, 0x01, 0x01 },//平台外设备名称[OD3_A1]
	{ 0x0006, 0x01, 0x01 },//平台外设备名称[OD4_A1]
	{ 0x0007, 0x01, 0x01 },//平台外设备名称[WSIM_A1]
	{ 0x0008, 0x01, 0x01 },//平台外设备名称[NCI_1_A1]
	{ 0x0009, 0x01, 0x01 },//平台外设备名称[NCI_2_A1]
	{ 0x0001, 0x01, 0x02 },//平台外设备名称[TGMT_B1]
	{ 0x0002, 0x01, 0x02 },//平台外设备名称[OD_GB_B1]
	{ 0x0003, 0x01, 0x02 },//平台外设备名称[OD1_B1]
	{ 0x0004, 0x01, 0x02 },//平台外设备名称[OD2_B1]
	{ 0x0005, 0x01, 0x02 },//平台外设备名称[OD3_B1]
	{ 0x0006, 0x01, 0x02 },//平台外设备名称[OD4_B1]
	{ 0x0007, 0x01, 0x02 },//平台外设备名称[WSIM_B1]
	{ 0x0008, 0x01, 0x02 },//平台外设备名称[NCI_1_B1]
	{ 0x0009, 0x01, 0x02 }//平台外设备名称[NCI_2_B1]
};

static RSSP_COM_CFG s_ComCfg[MAX_LINK_NUM];
static MyUINT16 s_SelfRsspID;
/*****************************************************************************
** 函数名:  F_RSSP1_SetSelfRsspID
** 功能:    更新本站Rssp协议ID
** 参数:	MyUINT16 rsspId	协议ID [0,0xffff]
**
** 返回值:
**     		无
** 相关输入输出:
**			无

** 其他:    		无
*****************************************************************************/
void F_RSSP1_SetSelfRsspID(MyUINT16 rsspId)
{
	s_SelfRsspID = rsspId;
}




/*****************************************************************************
** 函数名:	ReadRssp1Cfg
** 功能:	 读取Rssp1配置文件
** 参数:	无
** 返回值:	无
** 相关输入输出:
**
** 其他: 无
*****************************************************************************/
//MyBOOL F_RSSP1_ReadRssp1Cfg()
//{
//	MyBOOL bRet = e_UsrTrue;
//	MyINT32 fileHandle, fileResult, fReadResult;
//	MyUINT8 readBuffer[MAX_READ_LEN] = { 0 };
//	MyUINT16 readNum = 0x1B0u;
//	fileResult = F_VCP_FileOpen(RSSP1CFG_PATH, e_VCP_FOpenRead, &fileHandle);
//	if (fileResult == c_VCP_FOpenOk)
//	{
//		fReadResult = F_VCP_FileRead(fileHandle, MAX_READ_LEN, readBuffer);
//		if (c_VCP_FReadDone == fReadResult)
//		{
//			if (r_TRUE == Rsp1_Init(s_SelfRsspID, readNum, readBuffer))
//			{
//				bRet = e_UsrTrue;
//			}
//			else
//			{
//				bRet = e_UsrFalse;
//			}
//
//		}
//		else
//		{
//			bRet = e_UsrFalse;
//		}
//		F_VCP_FileClose(fileHandle, e_TRUE);
//	}
//	else
//	{
//		bRet = e_UsrFalse;
//	}
//	return bRet;
//}


/*****************************************************************************
** 函数名:	F_RSSP1_PackDataInOneFrame
** 功能:	 打包rssp1数据
** 参数:	无
** 返回值:	无
** 相关输入输出:
**
** 其他: 无
*****************************************************************************/
void F_RSSP1_PackDataInOneFrame(MyUINT16 LinkIndex, MyUINT8 type, MyUINT8* inbuff, MyUINT16 inlen, MyUINT8* outbuff, MyUINT16* outlen)
{
	S_SsMsg sseMsg, ssrMsg;
	S_Msg inMsg, outMsg;
	F_GL_MemSet(&sseMsg, 0, sizeof(S_SsMsg));
	F_GL_MemSet(&ssrMsg, 0, sizeof(S_SsMsg));
	F_GL_MemSet(&outMsg, 0, sizeof(S_Msg));
	F_GL_MemCpy(&inMsg.buff, inbuff, inlen);
	inMsg.length = inlen;
	switch (type)
	{
	case RSSP_RSD:
		Rsp1_PackageData(LinkIndex, &inMsg, &outMsg, &sseMsg, &ssrMsg);
		break;
	case RSSP_LEU_BTM:
		LEU_PackageBtmData(LinkIndex, &outMsg, &sseMsg, &ssrMsg);
		break;
	case RSSP_LEU_RSD:
		LEU_PackageRsdData(LinkIndex, &inMsg, &outMsg, &sseMsg, &ssrMsg);
		break;
	default:
		break;
	}
	if (sseMsg.length == 20)
	{
		F_GL_MemCpy(outbuff, sseMsg.buff, sseMsg.length);
		*outlen += sseMsg.length;
		F_GL_MemCpy(&outbuff[*outlen], outMsg.buff, outMsg.length);
		*outlen += outMsg.length;
	}
	else if (ssrMsg.length == 25)
	{
		F_GL_MemCpy(outbuff, ssrMsg.buff, ssrMsg.length);
		*outlen += ssrMsg.length;
		F_GL_MemCpy(&outbuff[*outlen], outMsg.buff, outMsg.length);
		*outlen += outMsg.length;
	}
	else
	{
		F_GL_MemCpy(outbuff, outMsg.buff, outMsg.length);
		*outlen = outMsg.length;
	}
}

/*****************************************************************************
** 函数名:	F_RSSP1_PackLeuData
** 功能:	 打包发送给LEU的rssp1数据
** 参数:	无
** 返回值:	无
** 相关输入输出:
**
** 其他: 无
*****************************************************************************/
//void F_RSSP1_PackLeuData(MyUINT16 LinkIndex, MyUINT16 VcpID, MyUINT8* inbuff, MyUINT16 inlen)
//{
//	S_Msg SendMsg = { 0 };
//	F_RSSP1_PackDataInOneFrame(LinkIndex, RSSP_LEU_RSD, inbuff, inlen, SendMsg.buff, &SendMsg.length);
//
//	if (SendMsg.length > 0)
//	{
//		F_VCP_UsrDataSendEx(c_DeviceInfTable[VcpID].Type, c_DeviceInfTable[VcpID].AppId, c_DeviceInfTable[VcpID].SysId, 0, SendMsg.buff, SendMsg.length);
//		F_GL_MemSet(&SendMsg, 0, sizeof(S_Msg));
//	}
//
//	LEU_BtmCycle(LinkIndex);
//	F_RSSP1_PackDataInOneFrame(LinkIndex, RSSP_LEU_BTM, inbuff, inlen, SendMsg.buff, &SendMsg.length);
//	if (SendMsg.length > 0)
//	{
//		F_VCP_UsrDataSendEx(c_DeviceInfTable[VcpID].Type, c_DeviceInfTable[VcpID].AppId, c_DeviceInfTable[VcpID].SysId, 1, SendMsg.buff, SendMsg.length);
//		F_GL_MemSet(&SendMsg, 0, sizeof(S_Msg));
//	}
//}
/*****************************************************************************
** 函数名:	F_RSSP1_CheckLeuData
** 功能:	接收并校验来自LEU的rssp数据
** 参数:	无
** 返回值:	无
** 相关输入输出:
**
** 其他: 无
*****************************************************************************/
//void F_RSSP1_CheckLeuData(MyUINT16 LinkIndex, MyUINT16 VcpID, MyUINT8* outbuff, MyUINT16* outlen)
//{
//	MyUINT8 Pipe = 0;
//	T_UsrBuff recvmsg;
//	MyUINT8 recvbuff[1024];
//	S_Msg tempBuf = { 0 };
//	recvmsg.ptrMsg = recvbuff;
//	recvmsg.dataLen = 0U;
//	recvmsg.allocLen = 1024U;
//	F_GL_MemSet(&tempBuf, 0, sizeof(S_Msg));
//	for (Pipe = 0; Pipe < LEU_PIPE_NUM; Pipe++)
//	{
//		if (e_UsrRxLcAppNew == F_VCP_UsrDataRecvEx(c_DeviceInfTable[VcpID].Type, c_DeviceInfTable[VcpID].AppId, c_DeviceInfTable[VcpID].SysId, Pipe, &recvmsg))
//		{
//			F_GL_MemCpy(&tempBuf.buff[tempBuf.length], recvmsg.ptrMsg, recvmsg.dataLen);
//			tempBuf.length += recvmsg.dataLen;
//			F_GL_MemSet(recvmsg.ptrMsg, 0U, 1024U);
//			recvmsg.dataLen = 0U;
//			recvmsg.allocLen = 1024U;
//		}
//	}
//	if (tempBuf.length >0U)
//	{
//		LEU_CheckRsdPackage(LinkIndex, tempBuf.length, tempBuf.buff);
//		LEU_CheckBtmPackage(LinkIndex, tempBuf.length, tempBuf.buff, outlen, outbuff);
//	}
//}

/*****************************************************************************
** 函数名:	F_RSSP1_CheckData
** 功能:	 校验rssp1数据
** 参数:	无
** 返回值:	无
** 相关输入输出:
**
** 其他: 无
*****************************************************************************/
//void F_RSSP1_CheckData(MyUINT16 LinkIndex, MyUINT16 VcpID, E_UsrRxStatus* bRecv, MyUINT8* outbuff, MyUINT16* outlen)
//{
//	MyUINT32 devType;
//	MyUINT32 appId;
//	MyUINT32 sysId;
//	MyUINT8 recvbuff1[1024], recvbuff2[1024];
//	T_CellMsg recvMsg[2];
//	F_GL_MemSet(recvbuff1, 0x00u, sizeof(recvbuff1));
//	F_GL_MemSet(recvbuff2, 0x00u, sizeof(recvbuff2));
//	recvMsg[0].ptrMsg = recvbuff1;
//	recvMsg[0].dataLen = sizeof(recvbuff1);
//	recvMsg[1].ptrMsg = recvbuff2;
//	recvMsg[1].dataLen = sizeof(recvbuff2);
//	bRecv[0] = e_UsrRxLcEmpty;
//	bRecv[1] = e_UsrRxLcEmpty;
//	devType = (UINT32)c_DeviceInfTable[VcpID].Type;
//	appId = (UINT32)c_DeviceInfTable[VcpID].AppId;
//	sysId = (UINT32)c_DeviceInfTable[VcpID].SysId;
//
//	bRecv[0] = F_VCP_UsrDataRecv(devType, appId, 1, &recvMsg[0]);
//	bRecv[1] = F_VCP_UsrDataRecv(devType, appId, 2, &recvMsg[1]);
//	Rsp1_Open(LinkIndex);
//	if (bRecv[0] == e_UsrRxLcAppNew)
//	{
//		Rsp1_CheckPackage(LinkIndex, recvMsg[0].dataLen, recvMsg[0].ptrMsg, outlen, outbuff);
//	}
//	else if (bRecv[1] == e_UsrRxLcAppNew)
//	{
//		Rsp1_CheckPackage(LinkIndex, recvMsg[1].dataLen, recvMsg[1].ptrMsg, outlen, outbuff);
//	}
//	else{}
//}
