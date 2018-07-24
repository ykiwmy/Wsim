
// MainFrm.h : CMainFrame 类的接口
//

#pragma once
#include "OutputWnd.h"
#include "ILKTestDoc.h"

class CCIOutputView;
class CSplitterFrame;



class CMainFrame : public CFrameWndEx
{
	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 特性
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CStatusBar m_wndStatusBar;

// 生成的消息映射函数
protected:
	bool SwitchActiveView(int nID);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnStationView();
	afx_msg void OnIOView();
	afx_msg void OnUpdateStationView(CCmdUI *pCmdUI);
	afx_msg void OnUpdateIOView(CCmdUI *pCmdUI);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:


	CCIOutputView*			pCIOutputView_;			// 联锁输出
	CSplitterFrame*			pSplitterFrame_;

	COutputWnd m_wndOutput;
	int						m_currentID;
protected:
	afx_msg void OnNetConfig();
	afx_msg LRESULT OnOutMessage(WPARAM wParam, LPARAM lParam);
public:
	BOOL CreateDockingWindows();
	BOOL	 bShowErrView_;			// 是否显示错误输出
public:
	CILKTestDoc* doc;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnAppExit();
	afx_msg void OnClose();
	afx_msg void OnErrShow();
};


