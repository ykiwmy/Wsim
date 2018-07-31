
// MainFrm.cpp : CMainFrame ���ʵ��
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
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_COM_STATUS,
};

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
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
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	m_wndStatusBar.GetStatusBarCtrl().SetMinHeight(30);
	//����״̬������
	LOGFONT logfont;
	GetObject(::GetStockObject(DEFAULT_GUI_FONT), sizeof(logfont), &logfont);
	logfont.lfCharSet = GB2312_CHARSET;
	logfont.lfWeight = FW_BOLD;
	logfont.lfHeight = 12;
	lstrcpy(logfont.lfFaceName, _T("����"));//����
	static CFont sfontStatusBar;
	sfontStatusBar.CreateFontIndirect(&logfont);
	m_wndStatusBar.SetFont(&sfontStatusBar);
	m_wndStatusBar.SetPaneInfo(1, ID_COM_STATUS, SBPS_NORMAL, 120);

	// TODO: �������Ҫ��ͣ������������ɾ��������
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//EnableDocking(CBRS_ALIGN_ANY);
	//DockControlBar(&m_wndToolBar);
	//SetWindowLong(this->m_hWnd, GWL_STYLE, 0);// ȥ������������ȫ����
	//SetMenu(NULL);                            //ȥ���˵���

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ
	//cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
	//	|/* WS_THICKFRAME |*/ WS_MAXIMIZE;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	cs.style |= WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	cs.cx = 800;//���ڿ��
	cs.cy = 1000;//���ڸ߶�
	return TRUE;
}

// CMainFrame ���

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


// CMainFrame ��Ϣ�������




//void CMainFrame::OnConnection()
//{
//	// TODO: �ڴ���������������
//
//}


void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	// TODO: �ڴ���������������
	CATP_SIMDoc* pDoc = (CATP_SIMDoc*)GetActiveDocument();
	NetConfigDialog dlg(this, &pDoc->netAddr);
	dlg.DoModal();
}



