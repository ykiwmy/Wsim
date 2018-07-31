//---------------------------------------------------------------------------


#pragma hdrstop
#include "StdAfx.h"
#include "IOSock.h"


//---------------------------------------------------------------------------

//#pragma package(smart_init)

CIOSock::CIOSock(SOCKADDR_IN addrSelf, SOCKADDR_IN addrOppo)
{
	m_addrSelf = addrSelf;
	m_addrOppo = addrOppo;

}


/******************************************************************************
* Name  :     F_InitSock
* Function :  构建UDP类型socket通信套接字

* Return :    BOOL是否构建成功
*
* REV1.0.0 : 20142950    2015/07/15    CreateMethod
******************************************************************************/
BOOL CIOSock::F_InitSock()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	INT32 err;
	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup( wVersionRequested, &wsaData);
	if (err != 0)
	{
		return FALSE;
	}

	if ((LOBYTE(wsaData.wVersion) != 2) || (HIBYTE(wsaData.wVersion) != 2))
	{
		WSACleanup();
		return FALSE;
	}

	//创建套接字
	m_Socket = socket(AF_INET, SOCK_DGRAM, 0);
	unsigned long ul = 1;
	INT32 r = ioctlsocket(m_Socket, FIONBIO, &ul);
	if(r == SOCKET_ERROR)
	{
		return FALSE;
	}

	r = bind(m_Socket, (SOCKADDR*)&m_addrSelf, sizeof(SOCKADDR));
	if (r == SOCKET_ERROR)
	{
		return FALSE;
	}

	BOOL bNewBehavior = FALSE;
	DWORD dwBytesReturned;
	DWORD status = WSAIoctl(m_Socket, SIO_UDP_CONNRESET, &bNewBehavior, sizeof(bNewBehavior), NULL, 0, &dwBytesReturned, NULL, NULL);
	if (status == SOCKET_ERROR)
	{
		DWORD dwErr = WSAGetLastError();
		if (dwErr != WSAEWOULDBLOCK)
		{
			return FALSE;
		}
	}

	return TRUE;
}


/******************************************************************************
* Name  :     F_GetData
* Function :  从以太网获取数据，须外部手动调用
* Para  :     BYTE *pbyData			获取到的数据存放地址
* Para  :     MyUINT16 *pusLen		获取到的数据的长度
* Return :    BOOL 					是否获取数据成功
*
* REV1.0.0 : 20142950    2015/07/15    CreateMethod
******************************************************************************/
BOOL CIOSock::F_GetData(BYTE *pbyData, MyUINT16 *pusLen)
{
	BYTE byRecvData[1024];	//四种插件输入/输出最大12字节
	INT32 nRet = 0;
	sockaddr_in addr;
	INT32 nLen = sizeof(SOCKADDR);
	nRet = recvfrom(m_Socket, (char *)byRecvData, 1024, 0, (SOCKADDR *)&addr, &nLen);

	if(nRet <= 0)
	{
		INT32 r = WSAGetLastError();
		return FALSE;
	}

/*	if(nRet != usLen)
	{
		return FALSE;
	}   */

	memcpy(pbyData, byRecvData, nRet);
    *pusLen = nRet;
	return TRUE;
}


/******************************************************************************
* Name  :     F_SendData
* Function :  向以太网发送数据
* Para  :     BYTE *pbyData			发送的数据buf
* Para  :     MyUINT16 usLen			发送的数据的长度

* Return :    BOOL 					是否发送数据成功
*
* REV1.0.0 : 20142950    2015/07/15    CreateMethod
******************************************************************************/
BOOL CIOSock::F_SendData(BYTE *pbyData, MyUINT16 usLen)
{
	INT32 ret = 0;
	ret = sendto(m_Socket, (const char *)pbyData, usLen, 0, (SOCKADDR *)&m_addrOppo, sizeof(SOCKADDR));
	if(ret == SOCKET_ERROR)
	{
		return FALSE;
	}
	else
	{
        return TRUE;
    }
}


void CIOSock::CloseConnect()
{
	shutdown(m_Socket, SD_BOTH);
	closesocket(m_Socket);
}