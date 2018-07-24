#pragma once


class CILKTestDoc;

#define MAX_SDI_NUM  96
struct IOData
{
	UINT16 head_;
	UINT16 data_[MAX_SDI_NUM];
	UINT16 crc16_;
};

// CIOSocket ÃüÁîÄ¿±ê

class CIOSocket : public CAsyncSocket
{
public:
	CIOSocket(CILKTestDoc* pDoc);
	CIOSocket(CILKTestDoc* pDoc,int iChanel);
	virtual ~CIOSocket();
	virtual void OnReceive(int nErrorCode);

	IOData sendData_;
	sockaddr remoteAddr_;
	sockaddr_in m_remoteCIAddr;
	CILKTestDoc* pDoc_;
	void InitSock(CString myIp, int myPort);
	void InitSock(CString myIp, int myPort, CString remoteIp, int remotePort);
	void sendDataToRemote(char* data, int datalen);
	int m_iChanel;
	int m_nRcvCount;
	int m_nLastRcvCount;
	BOOL isRcvNewData();
	void updataCnctState();
	BOOL m_isConnected;
	void addRcvCount();
};


