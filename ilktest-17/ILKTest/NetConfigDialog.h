#pragma once
#include "afxcmn.h"


struct NetAddress;

// CNetConfigDialog �Ի���

class CNetConfigDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CNetConfigDialog)

public:
	CNetConfigDialog(CWnd* pParent, NetAddress* pAddr);   // ��׼���캯��
	virtual ~CNetConfigDialog();

// �Ի�������
	enum { IDD = IDD_IP_CONFIG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

public:
	NetAddress* pAddr_;
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOk();
	CIPAddressCtrl clientIpAddrCtrl;
};
