
// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "ATP_SIM.h"

#include "MainFrm.h"
#include "ATP_SIMView.h"
#include "ATP_SIMDoc.h"
#include "NetConfigDialog.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
//	ON_COMMAND(ID_SEND_ALLELE, &CMainFrame::OnSendAllele)
//	ON_COMMAND(ID_Connection, &CMainFrame::OnConnection)
ON_WM_GETMINMAXINFO()
ON_COMMAND(ID_SetIP, &CMainFrame::OnSetip)

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_COM_STATUS,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	m_wndStatusBar.GetStatusBarCtrl().SetMinHeight(30);
	//设置状态栏字体
	LOGFONT logfont;
	GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(logfont), &logfont);
	logfont.lfCharSet = GB2312_CHARSET;
	logfont.lfWeight = FW_BOLD;
	logfont.lfHeight = 12;
	lstrcpy(logfont.lfFaceName, _T("宋体"));//宋体
	static CFont sfontStatusBar;
	sfontStatusBar.CreateFontIndirect(&logfont);
	m_wndStatusBar.SetFont(&sfontStatusBar);
	m_wndStatusBar.SetPaneInfo(1, ID_COM_STATUS, SBPS_NORMAL, 120);

	// TODO: 如果不需要可停靠工具栏，则删除这三行
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndToolBar);
	//SetWindowLong(this->m_hWnd, GWL_STYLE, 0);// 去掉标题栏（即全屏）
	//SetMenu(NULL);                            //去掉菜单栏

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	//cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
	//	|/* WS_THICKFRAME |*/ WS_MAXIMIZE;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	cs.style |= WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	cs.cx = 800;//窗口宽度
	cs.cy = 1000;//窗口高度
	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序




//void CMainFrame::OnConnection()
//{
//	// TODO: 在此添加命令处理程序代码
//
//}


void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int x = 780;
	int y = 780;
	lpMMI->ptMinTrackSize.x = x;
	lpMMI->ptMinTrackSize.y = y;
	lpMMI->ptMaxTrackSize.x = x;
	lpMMI->ptMaxTrackSize.y = y;

	CFrameWnd::OnGetMinMaxInfo(lpMMI);
}


void CMainFrame::OnSetip()
{
	// TODO: 在此添加命令处理程序代码
	CATP_SIMDoc* pDoc = (CATP_SIMDoc*)GetActiveDocument();
	NetConfigDialog dlg(this, &pDoc->netAddr);
	dlg.DoModal();
}



