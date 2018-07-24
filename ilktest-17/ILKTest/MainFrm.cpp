
// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "ILKTest.h"
#include "SplitterFrame.h"
#include "MainFrm.h"
#include "ChildView.h"
#include "IOView.h"
#include "NetConfigDialog.h"
#include "ILKTestDoc.h"
#include "Relay.h"
#include "ILKGraphic\CADDevice.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_32771, &CMainFrame::OnStationView)
	ON_UPDATE_COMMAND_UI(ID_32771, &CMainFrame::OnUpdateStationView)
	ON_COMMAND(ID_MENU_IO, &CMainFrame::OnIOView)
	ON_UPDATE_COMMAND_UI(ID_MENU_IO, &CMainFrame::OnUpdateIOView)
	ON_COMMAND(ID_32777, &CMainFrame::OnNetConfig)
	ON_WM_TIMER()
	ON_MESSAGE(WM_OUT_MESSAGE, &CMainFrame::OnOutMessage)
	ON_COMMAND(ID_APP_EXIT, &CMainFrame::OnAppExit)
	ON_WM_CLOSE()
	ON_COMMAND(ID_32782, &CMainFrame::OnErrShow)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
: pCIOutputView_(NULL)
, pSplitterFrame_(NULL)
, bShowErrView_(TRUE)
{
	m_currentID = AFX_IDW_PANE_FIRST;
	
}

CMainFrame::~CMainFrame()
{
	/*if (pCIOutputView_!=NULL)
	{
		delete pCIOutputView_;
	}*/
	if (pSplitterFrame_ != NULL)
	{
		delete pSplitterFrame_;
	}
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{	
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// 启用 Visual Studio 2005 样式停靠窗口行为
	CDockingManager::SetDockingMode(DT_SMART);
	// 启用 Visual Studio 2005 样式停靠窗口自动隐藏行为
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// 创建停靠窗口
	if (!CreateDockingWindows())
	{
		TRACE0("未能创建停靠窗口\n");
		return -1;
	}

	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOutput);

	// 开启 定时器
	SetTimer(0, 100, NULL);

	return 0;
}


BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CFrameWndEx::PreCreateWindow(cs))
		return FALSE;

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		| WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE | WS_SYSMENU;

	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


bool CMainFrame::SwitchActiveView(int nID)
{
	if (nID == m_currentID)
		return FALSE;
	switch (nID)
	{
	case IDD_IOVIEW:
		pSplitterFrame_->ShowWindow(SW_SHOW);
		pSplitterFrame_->SetDlgCtrlID(AFX_IDW_PANE_FIRST);
		m_currentID = AFX_IDW_PANE_FIRST;
		break;
	default:
		break;
	}
	RecalcLayout();
	return true;
}


// CMainFrame 消息处理程序



void CMainFrame::OnStationView()
{
	SwitchActiveView(IDD_IOVIEW);
	pSplitterFrame_->On32771();
}


void CMainFrame::OnIOView()
{
	SwitchActiveView(IDD_IOVIEW);
	pSplitterFrame_->On32772();
}


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// 创建其他的 View

	CRect rect;
	GetClientRect(&rect);

	pSplitterFrame_ = new CSplitterFrame;
	pSplitterFrame_->Create(NULL, NULL, WS_CHILD | WS_BORDER, rect, this, NULL,0, pContext);
	pSplitterFrame_->ShowWindow(SW_SHOW);
	pSplitterFrame_->SetDlgCtrlID(AFX_IDW_PANE_FIRST);

	return TRUE;
}


void CMainFrame::OnUpdateStationView(CCmdUI *pCmdUI)
{
//	pCmdUI->SetCheck(GetActiveView()->IsKindOf(RUNTIME_CLASS(CChildView)));
	pCmdUI->SetCheck(m_currentID == IDD_IOVIEW);
}


void CMainFrame::OnUpdateIOView(CCmdUI *pCmdUI)
{
//	pCmdUI->SetCheck(bShowIOView_);
//	pCmdUI->SetCheck((m_currentID == IDD_IOVIEW));
	pCmdUI->SetCheck(m_currentID == IDD_IOVIEW);
}

void CMainFrame::OnNetConfig()
{
	CILKTestDoc* pDoc = (CILKTestDoc*)GetActiveDocument();
	
	CNetConfigDialog dlg(this, &pDoc->netAddr_);
	if (dlg.DoModal() == IDOK)
	{
		pDoc->InitSockets();
	}
}


BOOL CMainFrame::CreateDockingWindows()
{
	// 创建输出窗口
	if (!m_wndOutput.Create(L"输出", this, CRect(0, 0, 50, 50)/*CRect(0, 0, 100, 100)暂时不显示输出串口*/, TRUE, 149, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("未能创建输出窗口\n");
		return FALSE; // 未能创建
	}

	return TRUE;
}


void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	CILKTestDoc* pDoc = (CILKTestDoc*)GetActiveDocument();
	if (pDoc!=NULL)
	{
		pDoc->OnTimer();
	}

	CFrameWndEx::OnTimer(nIDEvent);
}


LRESULT CMainFrame::OnOutMessage(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case 0:
		m_wndOutput.outList().AddString(*(CString*)lParam);
		break;
	case 1:
		m_wndOutput.errList().AddString(*(CString*)lParam);
		break;
	default:
		break;
	}

	return 0;
}

void CMainFrame::OnAppExit()
{
	// TODO:  在此添加命令处理程序代码
	DestroyWindow();
}


void CMainFrame::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	DestroyWindow();
	CFrameWndEx::OnClose();
}


void CMainFrame::OnErrShow()
{
	// TODO:  在此添加命令处理程序代码
	bShowErrView_ =!bShowErrView_;

	if (bShowErrView_)
	{
		m_wndOutput.ShowWindow(SW_SHOW);
	}
	else
	{
		m_wndOutput.ShowWindow(SW_HIDE);
	}

	RecalcLayout();
}
