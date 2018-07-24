#pragma once
#include "afxcmn.h"


struct NetAddress;

// CNetConfigDialog 对话框

class CNetConfigDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CNetConfigDialog)

public:
	CNetConfigDialog(CWnd* pParent, NetAddress* pAddr);   // 标准构造函数
	virtual ~CNetConfigDialog();

// 对话框数据
	enum { IDD = IDD_IP_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

public:
	NetAddress* pAddr_;
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOk();
	CIPAddressCtrl clientIpAddrCtrl;
};
