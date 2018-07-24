
// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once
#include "OutputWnd.h"
#include "ILKTestDoc.h"

class CCIOutputView;
class CSplitterFrame;



class CMainFrame : public CFrameWndEx
{
	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CStatusBar m_wndStatusBar;

// ���ɵ���Ϣӳ�亯��
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


	CCIOutputView*			pCIOutputView_;			// �������
	CSplitterFrame*			pSplitterFrame_;

	COutputWnd m_wndOutput;
	int						m_currentID;
protected:
	afx_msg void OnNetConfig();
	afx_msg LRESULT OnOutMessage(WPARAM wParam, LPARAM lParam);
public:
	BOOL CreateDockingWindows();
	BOOL	 bShowErrView_;			// �Ƿ���ʾ�������
public:
	CILKTestDoc* doc;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnAppExit();
	afx_msg void OnClose();
	afx_msg void OnErrShow();
};


