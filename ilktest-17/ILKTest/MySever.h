#pragma once
#include "afxsock.h"
#include <vector>
using namespace std;

class CILKTestDoc;
class CTCPSocket;

class CMySever :
	public CAsyncSocket
{
public:
	CMySever(CILKTestDoc* doc);
	virtual ~CMySever();

	void InitSock(CString myIp, int myPort);
	virtual void OnAccept(int nErrorCode);
	CILKTestDoc *pDoc;
	vector<CTCPSocket*> vConnectClient;

	virtual void OnConnect(int nErrorCode);
	bool m_bConnected;
	virtual void OnClose(int nErrorCode);
	void OnTimer();
	void deletClient(CTCPSocket* client);
	void showAllTestDlg();
};

