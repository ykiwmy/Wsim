#pragma once


// NetConfigDialog 对话框
#include "ATP_SIMDoc.h"
#include "afxcmn.h"
class NetConfigDialog : public CDialogEx
{
	DECLARE_DYNAMIC(NetConfigDialog)

public:
	NetConfigDialog(CWnd* pParent ,NetAddr* pAddr);   // 标准构造函数
	virtual ~NetConfigDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SetIP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
public:
	NetAddr* pAddr;
	CIPAddressCtrl m_selfIP;
	DECLARE_MESSAGE_MAP()
	

	afx_msg void OnBnClickedOk();
};
