// IOSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "ILKTest.h"
#include "IOSocket.h"
#include "ILKTestDoc.h"

// CIOSocket

CIOSocket::CIOSocket(CILKTestDoc* pDoc)
: pDoc_(pDoc)
, m_iChanel(0)
, m_nRcvCount(0)
, m_nLastRcvCount(0)
, m_isConnected(FALSE)
{
	memset(&sendData_, 0, sizeof(IOData));
	memset(&remoteAddr_, 0, sizeof(sockaddr));

	// TODO：设置 sendData_ 的 head_
}

CIOSocket::CIOSocket(CILKTestDoc* pDoc,int iChanel)
: pDoc_(pDoc)
, m_iChanel(iChanel)
, m_nRcvCount(0)
, m_nLastRcvCount(0)
, m_isConnected(FALSE)
{
	memset(&sendData_, 0, sizeof(IOData));
	memset(&remoteAddr_, 0, sizeof(sockaddr));

	// TODO：设置 sendData_ 的 head_
}

CIOSocket::~CIOSocket()
{
}


// CIOSocket 成员函数


void CIOSocket::OnReceive(int nErrorCode)
{
	int addrLen = sizeof(sockaddr);
	char recvBuff[1024] = {0};
	int nRecv = ReceiveFrom(&recvBuff, 1024, &remoteAddr_, &addrLen);
	int ret = 0;
	if (nRecv > 0)
	{
		// TODO: 检查包头
		//TRACE(L"nRecv is %d ,m_isConnected is %d \r\n", nRecv, m_isConnected);
		// 刷新站场和驱动板显示

		addRcvCount();
		pDoc_->updateIO(recvBuff, m_iChanel,nRecv);
	}

	CAsyncSocket::OnReceive(nErrorCode);
}


void CIOSocket::InitSock(CString myIp, int myPort)
{
	if (Create(myPort, SOCK_DGRAM, FD_READ))
	{
		int m = Bind(myPort, myIp);
	}
	else
	{
		AfxMessageBox(L"UDP Creat err!!!");
	}
}


void CIOSocket::InitSock(CString myIp, int myPort, CString remoteIp, int remotePort)
{
	if (Create(myPort, SOCK_DGRAM, FD_READ, myIp))
	{
		//int m = Bind(myPort, myIp);
	}
	else
	{
		CString str = L"UDP Creat err,IP=";
		str.Format(str + myIp + L",port=%d",myPort);
		AfxMessageBox(str);
	}

	//赋值发送地址
	char dataIPBuff[24] = {0};
	CStringA dataA(remoteIp);
	strcpy_s(dataIPBuff, dataA);

	m_remoteCIAddr.sin_addr.s_addr = inet_addr(dataIPBuff);
	m_remoteCIAddr.sin_family = AF_INET;
	m_remoteCIAddr.sin_port = htons((UINT16)remotePort);
}


void CIOSocket::sendDataToRemote(char* data, int datalen)
{
	SendTo(data, datalen, (SOCKADDR*)&m_remoteCIAddr, sizeof(sockaddr));

	//if (datalen == sizeof(IOData))
	//{
	//	SendTo(data, datalen, (SOCKADDR*)&m_remoteCIAddr, sizeof(sockaddr));
	//}
	//else
	//{
	//	//发送数据长度错误
	//}
}


BOOL CIOSocket::isRcvNewData()
{
	if (m_nLastRcvCount==m_nRcvCount)
	{
		return FALSE;
	}
	return TRUE;
}


void CIOSocket::updataCnctState()
{
	if (isRcvNewData())
	{
		m_nLastRcvCount = m_nRcvCount;
		m_isConnected = TRUE;
	}
	else
	{
		m_isConnected = FALSE;
	}
}


void CIOSocket::addRcvCount()
{
	m_nRcvCount++;
	if (m_nRcvCount == 0xFFFFFFFF)
	{
		m_nRcvCount = 0;
		m_nLastRcvCount = 0;
	}
}
