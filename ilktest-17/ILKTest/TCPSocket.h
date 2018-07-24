#pragma once
#include "afxsock.h"
#include "TCPShowDlg.h"
#include <vector>
using namespace std;

class CILKTestDoc;
class CMySever;
class RailSwitch;
class CADSignal;
class CADSection;
class CADDoor;
class CADButton;
class CADDeLight;
class CADAfgDoor;
class CTCPSocket :
	public CAsyncSocket
{
public:
	CTCPSocket(CILKTestDoc* doc,CMySever* pSev);
	virtual ~CTCPSocket();
	virtual void OnReceive(int nErrorCode);
	virtual void OnConnect(int nErrorCode);

	CILKTestDoc *pDoc;
	CMySever* pParentSev;

	sockaddr_in myAddr_;
	sockaddr_in remoteAddr_;
	
	CTCPShowDlg* showDlg;

	void SendPackDate();
	int m_ReciveCount;
	int m_lastReciveCount;
	int GetReciveCount();
	int GetLastReciveCount();
	void SetLastReciveCount(int count);
	virtual void OnAccept(int nErrorCode);
	void ShowTipMsg(CString msg);
	virtual void OnClose(int nErrorCode);
	void CreatATPView();
	void  processCommand(CString recvdata);
	struct RecvATPData
	{
		CString head;
		CString type;
		CString ixlNo;
		CString eleType;
		CString eleIndex;
		CString state;
	};

	struct RecvATPCommandData
	{
		CString ixlNo;
		CString eleType;
		CString eleIndex;
		CString state;
	};
	RecvATPData myRecvData;
	CString getStrLeftByCh(CString &recvdata, char ch);
	void processAllStateCommand();
	bool checkRecvData(CString data);
	void sendACK(char* data, int len);
	void sendData(char* buff, int bufflen);
	void setAtpSectionState(CString eleIndex, CString state);
	void setAtpPsdState(CString eleIndex, CString state);
	void setAtpEmpState(CString eleIndex, CString state);
	void setAtpDrbState(CString eleIndex, CString state);
	CString GetSignalState(int color , bool isDDstate);
	CString getSwitchPos(int pos);
	CString IXL_id;
	int packA2Map(CString eleType, CString eleldx, CString state, char* buff);
	int turnCStrToStr(CString str, char* buff);
	void processAllStateCommandCycle();
	void OnTimer();
	
	CString getSectionState(bool isOccupy);
	void sendLingSanState();
	CString getPsdState(bool isOpen);
	CString getEMPState(bool isEMPDown);
	CString getDTRBState(bool isDTRBDown);
	void sendSwitchPos(RailSwitch* pSwitch);
	void sendSignalState(CADSignal* pSignal);
	void sendSwitchSectionState(RailSwitch* pSwitch);
	void sendSectionState(CADSection* pSection);
	void sendPsdState(CADDoor* pDoor);
	void sendEMPState(CADButton* pBtn);
	void sendDTRBState(CADDeLight* pLight);
	void setIXLId(int numb);
	void sendAFGState(CADAfgDoor* pAfg);
};

