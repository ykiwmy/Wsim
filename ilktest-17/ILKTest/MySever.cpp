#include "stdafx.h"
#include "MySever.h"
#include "TCPSocket.h"
#include "ILKTestDoc.h"

CMySever::CMySever(CILKTestDoc* doc)
:pDoc(doc)
, m_bConnected(false)
{
}


CMySever::~CMySever()
{
	for each(CTCPSocket* pClient in vConnectClient)
	{
		pClient->Close();
		delete pClient;
	}
}

void CMySever::InitSock(CString myIp, int myPort)
{
	if (Create(myPort, SOCK_STREAM, FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE, myIp))
	{
		//int m = Bind(myPort, myIp);

		if (!Listen())
		{
			AfxMessageBox(L"Listen Error");
			return;
		}
	}
	else
	{
		AfxMessageBox(L"TCP Creat err!!!");
	}
}


void CMySever::OnAccept(int nErrorCode)
{
	// TODO:  在此添加专用代码和/或调用基类
	if (nErrorCode == 0)
	{
		CTCPSocket* pNewConn = new CTCPSocket(pDoc, this);
		vConnectClient.push_back(pNewConn);
		Accept(*pNewConn);

		CString strAddr("");
		CString strip("");
		UINT nPort;
		pNewConn->GetPeerName(strip, nPort);
		strAddr.Format(L"%s:%d", strip, nPort);
		pNewConn->ShowTipMsg(L"OnAccept，" + strAddr);


		SOCKADDR lpSockAddr;
		memset(&lpSockAddr, 0, sizeof(SOCKADDR));
		int socklen;

		pNewConn->GetPeerName(&lpSockAddr, &socklen);
		DWORD ipAddr = htonl(inet_addr(lpSockAddr.sa_data));
		pNewConn->showDlg->SetClientIPAddr(ipAddr);
		pNewConn->showDlg->setClientPort(nPort);

		pNewConn->setIXLId(pDoc->ixlIDForGalaxy);
	}
	CAsyncSocket::OnAccept(nErrorCode);
}


void CMySever::OnConnect(int nErrorCode)
{
	// TODO:  在此添加专用代码和/或调用基类
	CAsyncSocket::OnConnect(nErrorCode);
}


void CMySever::OnClose(int nErrorCode)
{
	// TODO:  在此添加专用代码和/或调用基类

	CAsyncSocket::OnClose(nErrorCode);
}


void CMySever::OnTimer()
{
	for each(CTCPSocket* pClient in vConnectClient)
	{
		pClient->OnTimer();
	}
}


void CMySever::deletClient(CTCPSocket* client)
{
	vector<CTCPSocket*> ::iterator pos;
	pos = find(vConnectClient.begin(), vConnectClient.end(), client);

	if (pos != vConnectClient.end())
	{
		vConnectClient.erase(pos);
	}
}


void CMySever::showAllTestDlg()
{
	for each(CTCPSocket* pClient in vConnectClient)
	{
		pClient->showDlg->showMyDlg();
	}
}
