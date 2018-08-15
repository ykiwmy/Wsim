#pragma once

#include <vector>
using namespace std;

class Relay;
class CZcCommRelay;
class WcuComm;
// CADGraphic
#define RELAY_MENU_0 (WM_USER + 100)
#define ZC_COMM_MENU_0 (RELAY_MENU_0 + 1000)

class CADGraphic : public CWnd
{
public:
	CADGraphic();
	CADGraphic(CWnd* pParent);
	virtual ~CADGraphic();

public:
	DECLARE_SERIAL(CADGraphic)

public:
	// ���ö���δ���
	virtual void compressWindow() {}
public:
	static int ToInt(double d) { return (int)(d > 0 ? d + 0.5 : d - 0.5); }

	void createEmptyWnd(CWnd* pParent) { Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(), pParent, 0); }

	// ���ö���δ���
	void setCRgnWindow(CPoint wndPt[], int nPt);

	vector<Relay*> vRelay_;
	vector<CZcCommRelay*> vZcCommRelay_;
	vector<WcuComm*> vWcuComm_;
	CMenu* pMenu_;	// �����ļ̵����˵�
	CMenu* pRMenu_;

	void loadMenu();	// װ�ز˵�

	void OnRPopMenu(UINT nenuID);
	virtual void OnRPopMenuEx(int flag) {}

	void OnPopMenu(UINT menuID);	// ��Ӧ�̵����˵���Ϣ
	virtual void OnPopMenuEx(int flag) {}	// �������ļ̵���ʱ���ᴥ���в����κ�˫������Ĺ涨����
	void OnClickRelay(Relay* pRelay);	// ��Ӧ CIOView �еĵ���̵����¼�
	void OnClickZcCommRelay(CZcCommRelay* pZcRelay);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	BOOL isRelayUp(int relayNumb);
	BOOL isRelayDown(int relayNumb);

	BOOL isATRelay(int flag);
	BOOL isUTRelay(int flag);
	BOOL isHighRelay(int flag);
	
};