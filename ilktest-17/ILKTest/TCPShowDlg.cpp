// TCPShowDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ILKTest.h"
#include "TCPShowDlg.h"
#include "afxdialogex.h"
#include "ILKTest.h"
#include "ChildView.h"
#include "ILKTestDoc.h"
// CTCPShowDlg �Ի���

IMPLEMENT_DYNAMIC(CTCPShowDlg, CDialogEx)

CTCPShowDlg::CTCPShowDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTCPShowDlg::IDD, pParent)
	, m_strShowEdit(_T(""))
	, m_isShow(false)
	, m_nClientIp(0)
	, m_strPort(_T("0"))
	, m_PauseState(FALSE)
	, m_isWriteLog(TRUE)
{
	
}

CTCPShowDlg::~CTCPShowDlg()
{
	DestroyWindow();
}

void CTCPShowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_showEdit);
	DDX_Text(pDX, IDC_EDIT1, m_strShowEdit);
	DDX_IPAddress(pDX, IDC_IPADDRESS1, m_nClientIp);
	DDX_Control(pDX, IDC_IPADDRESS1, clientIpCtrl);
	DDX_Control(pDX, IDC_EDIT2, m_portCtrl);
	DDX_Text(pDX, IDC_EDIT2, m_strPort);
	DDX_Control(pDX, IDC_BTNPAUSE, m_BtnPause);
	DDX_Control(pDX, IDOK, m_BtnOk);
	DDX_Check(pDX, IDC_CHECK1, m_isWriteLog);
}


BEGIN_MESSAGE_MAP(CTCPShowDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTCPShowDlg::OnBnClickedOk)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTNCLEAR, &CTCPShowDlg::OnBnClickedBtnclear)
	ON_BN_CLICKED(IDC_BTNPAUSE, &CTCPShowDlg::OnBnClickedBtnpause)
END_MESSAGE_MAP()


// CTCPShowDlg ��Ϣ�������


void CTCPShowDlg::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	showMyDlg();
	CDialogEx::OnOK();
}

void CTCPShowDlg::showMsg(CString msg)
{
	if (m_strShowEdit.GetLength()>=5000)
	{
		m_strShowEdit = L"";
	}
	if (/*m_isShow &&*/ !m_PauseState)
	{
		msg += "\r\n";
		m_strShowEdit = m_strShowEdit + msg;
		/*UpdateData(FALSE);
		UpdateWindow();*/
		m_showEdit.SetWindowTextW(m_strShowEdit);
	}
	
	if (m_isWriteLog)
	{
		theApp.thePView->GetDocument()->OutToFile(msg);
	}
}

void CTCPShowDlg::showMyDlg()
{
	m_isShow = !m_isShow;
	if (m_isShow)
	{
		ShowWindow(SW_SHOW);
	}
	else
	{
		ShowWindow(SW_HIDE);
	}
}

void CTCPShowDlg::OnClose()
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_isShow = false;
	ShowWindow(SW_HIDE);
	CDialogEx::OnClose();
}


void CTCPShowDlg::SetClientIPAddr(DWORD ipaddr)
{
	m_nClientIp = ipaddr;
	UpdateData(FALSE);
}


void CTCPShowDlg::setClientPort(UINT port)
{
	m_strPort.Format(L"%d",port);
	m_portCtrl.SetWindowTextW(m_strPort);
}


void CTCPShowDlg::OnBnClickedBtnclear()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_strShowEdit = L"";
	UpdateData(false);
}


void CTCPShowDlg::OnBnClickedBtnpause()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_PauseState = !m_PauseState;
	if (m_PauseState==TRUE)
	{
		m_BtnPause.SetWindowTextW(L"������ʾ");
	}
	else
	{
		m_BtnPause.SetWindowTextW(L"��ͣ��ʾ");
	}


}
