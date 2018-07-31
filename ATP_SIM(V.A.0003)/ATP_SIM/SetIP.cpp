// SetIP.cpp : 实现文件
//

#include "stdafx.h"
#include "ATP_SIM.h"
#include "SetIP.h"
#include "afxdialogex.h"


// SetIP 对话框

IMPLEMENT_DYNAMIC(SetIP, CDialogEx)

SetIP::SetIP(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SetIP, pParent)
{

}

SetIP::~SetIP()
{
}

void SetIP::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SetIP, CDialogEx)
END_MESSAGE_MAP()


// SetIP 消息处理程序
