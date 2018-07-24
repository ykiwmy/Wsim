// NetConfigDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "ILKTest.h"
#include "NetConfigDialog.h"
#include "afxdialogex.h"
#include "ILKTestDoc.h"
#include "MainFrm.h"


// CNetConfigDialog 对话框

IMPLEMENT_DYNAMIC(CNetConfigDialog, CDialogEx)

CNetConfigDialog::CNetConfigDialog(CWnd* pParent, NetAddress* pAddr)
	: CDialogEx(CNetConfigDialog::IDD, pParent)
	, pAddr_(pAddr)
{
}

CNetConfigDialog::~CNetConfigDialog()
{
	DestroyWindow();
}

void CNetConfigDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	CILKTestDoc* pDoc = (CILKTestDoc*)dynamic_cast<CMainFrame*>(AfxGetMainWnd())->GetActiveDocument();
	DDX_IPAddress(pDX, IDC_IPADDRESS_MY_A, pAddr_->localIP_);
	DDX_IPAddress(pDX, IDC_IPADDRESS_MY_ATP, pAddr_->LocalATPServerIP_);
	DDX_Text(pDX, IDC_EDIT_MY_A, pAddr_->localIOPort_);
	DDX_Text(pDX, IDC_EDIT_MY_ATP, pAddr_->localATPPort_);
	DDX_Text(pDX, IDC_EDIT_DR_TIME, pDoc->driveTimeCount);
	DDX_Control(pDX, IDC_IPADDRESS_MY_A, clientIpAddrCtrl);
}
BEGIN_MESSAGE_MAP(CNetConfigDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CNetConfigDialog::OnBnClickedOk)
END_MESSAGE_MAP()


void CNetConfigDialog::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}
