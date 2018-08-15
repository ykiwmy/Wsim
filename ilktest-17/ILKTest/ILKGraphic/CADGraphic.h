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
	// 设置多边形窗口
	virtual void compressWindow() {}
public:
	static int ToInt(double d) { return (int)(d > 0 ? d + 0.5 : d - 0.5); }

	void createEmptyWnd(CWnd* pParent) { Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(), pParent, 0); }

	// 设置多边形窗口
	void setCRgnWindow(CPoint wndPt[], int nPt);

	vector<Relay*> vRelay_;
	vector<CZcCommRelay*> vZcCommRelay_;
	vector<WcuComm*> vWcuComm_;
	CMenu* pMenu_;	// 弹出的继电器菜单
	CMenu* pRMenu_;

	void loadMenu();	// 装载菜单

	void OnRPopMenu(UINT nenuID);
	virtual void OnRPopMenuEx(int flag) {}

	void OnPopMenu(UINT menuID);	// 响应继电器菜单消息
	virtual void OnPopMenuEx(int flag) {}	// 点击道岔的继电器时，会触发有岔区段和双动道岔的规定动作
	void OnClickRelay(Relay* pRelay);	// 响应 CIOView 中的点击继电器事件
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