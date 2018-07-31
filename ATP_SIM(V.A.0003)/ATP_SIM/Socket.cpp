// ---------------------------------------------------------------------------

#pragma hdrstop

#include "StdAfx.h"
#include "Socket.h"

//#pragma package(smart_init)

// ---------------------------------------------------------------------------
/** ****************************************************************************
 * Name  :     F_InitSock
 * Function :  创建通信类CIOSock并初始化

 * Return :    BOOL是否创建通信类成功
 *
 * REV1.0.0 : 20142950    2015/07/15    CreateMethod
 ***************************************************************************** */
BOOL SocketInterface::F_InitSock() 
{
	m_addrSelf.sin_addr.S_un.S_addr = inet_addr(IP_SELF);
	m_addrSelf.sin_family = AF_INET;
	m_addrOppo.sin_addr.S_un.S_addr = inet_addr(IP1_OPPO);
	m_addrOppo.sin_family = AF_INET;

	m_addrSelf.sin_port = htons(m_CIA1_PORT_SELF);
	m_addrOppo.sin_port = htons(m_CIA1_PORT_OPPO);
	m_CIA1Sock = new CIOSock(m_addrSelf, m_addrOppo);
	//m_addrSelf.sin_port = htons(m_ZcA1_PORT_SELF);
	//m_addrOppo.sin_port = htons(m_ZcA1_PORT_OPPO);
	//m_ZcA1Sock = new CIOSock(m_addrSelf, m_addrOppo);

	//m_addrSelf.sin_addr.S_un.S_addr = inet_addr(IP_SELF);
	//m_addrSelf.sin_family = AF_INET;
	//m_addrOppo.sin_addr.S_un.S_addr = inet_addr(IP2_OPPO);
	//m_addrOppo.sin_family = AF_INET;
	//
	//m_addrSelf.sin_port = htons(m_CIA2_PORT_SELF);
	//m_addrOppo.sin_port = htons(m_CIA2_PORT_OPPO);
	//m_CIA2Sock = new CIOSock(m_addrSelf, m_addrOppo);
	//m_addrSelf.sin_port = htons(m_ZcA2_PORT_SELF);
	//m_addrOppo.sin_port = htons(m_ZcA2_PORT_OPPO);
	//m_ZcA2Sock = new CIOSock(m_addrSelf, m_addrOppo);


	m_CIA1Sock->F_InitSock();
	//m_CIA2Sock->F_InitSock();
	//m_ZcA1Sock->F_InitSock();
	//m_ZcA2Sock->F_InitSock();

	return true;
}
BOOL SocketInterface::F_InitSock(DWORD SelfIP, DWORD PeerIP, int SelfPort, int PeerPort)
{
	m_addrSelf.sin_addr.S_un.S_addr = htonl(SelfIP);
	m_addrSelf.sin_family = AF_INET;
	m_addrOppo.sin_addr.S_un.S_addr = htonl(PeerIP);
	m_addrOppo.sin_family = AF_INET;

	m_addrSelf.sin_port = htons(SelfPort);
	m_addrOppo.sin_port = htons(PeerPort);
	m_CIA1Sock = new CIOSock(m_addrSelf, m_addrOppo);
	m_CIA1Sock->F_InitSock();
	return true;
}
SocketInterface::SocketInterface()
{
	m_CIA1Sock = NULL;
}
SocketInterface::~SocketInterface()
{
	if (m_CIA1Sock != NULL)
	{
		delete m_CIA1Sock;
	}
	
}

BOOL SocketInterface::F_Send(UINT32 devId, BYTE *pbyData, MyUINT16 pusLen) {
	switch(devId) {
	case DevCI1:
		m_CIA1Sock->F_SendData(pbyData, pusLen);
		break;
	default:
		break;
	}
	return TRUE;
}

BOOL SocketInterface::F_Recv(UINT32 devId, BYTE *pbyData, MyUINT16 *pusLen) {
	switch(devId) {
	case DevCI1:
		m_CIA1Sock->F_GetData(pbyData, pusLen);
		break;
	//case DevCI2:
	//	m_CIA2Sock->F_GetData(pbyData, pusLen);
	//	break;
	//case DevZc1:
	//	m_ZcA1Sock->F_GetData(pbyData, pusLen);
	//	break;
	//case DevZc2:
	//	m_ZcA2Sock->F_GetData(pbyData, pusLen);
	//	break;
	default:
		break;
	}
	return TRUE;
}


BOOL SocketInterface::F_CloseConnect()
{
	m_CIA1Sock->CloseConnect();
	return TRUE;
}