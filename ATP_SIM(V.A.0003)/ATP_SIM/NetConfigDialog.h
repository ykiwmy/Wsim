#pragma once


// NetConfigDialog �Ի���
#include "ATP_SIMDoc.h"
#include "afxcmn.h"
class NetConfigDialog : public CDialogEx
{
	DECLARE_DYNAMIC(NetConfigDialog)

public:
	NetConfigDialog(CWnd* pParent ,NetAddr* pAddr);   // ��׼���캯��
	virtual ~NetConfigDialog();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SetIP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
public:
	NetAddr* pAddr;
	CIPAddressCtrl m_selfIP;
	DECLARE_MESSAGE_MAP()
	

	afx_msg void OnBnClickedOk();
};
