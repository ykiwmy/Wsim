// NetConfigDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "ATP_SIM.h"
#include "NetConfigDialog.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "ATP_SIMDoc.h"

// NetConfigDialog 对话框

IMPLEMENT_DYNAMIC(NetConfigDialog, CDialogEx)

NetConfigDialog::NetConfigDialog(CWnd* pParent ,NetAddr* pAddr)
	: CDialogEx(IDD_SetIP, pParent)
	,pAddr(pAddr)
{

}

NetConfigDialog::~NetConfigDialog()
{
}

void NetConfigDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_IPAddress(pDX, IDC_SelfIP, pAddr->localIP);
	DDX_IPAddress(pDX, IDC_PeerIP, pAddr->remoteIP);
	DDX_Text(pDX, IDC_SelfPort, pAddr->localPort);
	DDX_Text(pDX, IDC_PeerPort, pAddr->remotePort);
}


BEGIN_MESSAGE_MAP(NetConfigDialog, CDialogEx)
ON_BN_CLICKED(IDOK, &NetConfigDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// NetConfigDialog 消息处理程序





void NetConfigDialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CATP_SIMDoc* pDoc = (CATP_SIMDoc*)pMain->GetActiveDocument();
	CIPAddressCtrl* m_pSelfIP = (CIPAddressCtrl*)GetDlgItem(IDC_SelfIP);
	CIPAddressCtrl* m_pPeerIP = (CIPAddressCtrl*)GetDlgItem(IDC_PeerIP);
	m_pSelfIP->GetAddress(pDoc->netAddr.localIP);
	m_pPeerIP->GetAddress(pDoc->netAddr.remoteIP);
	pDoc->netAddr.localPort = GetDlgItemInt(IDC_SelfPort);
	pDoc->netAddr.remotePort = GetDlgItemInt(IDC_PeerPort);
	CDialogEx::OnOK();
}
