#pragma once

#include "resource.h"
#include "afxwin.h"
#include "afxcmn.h"
// CTCPShowDlg �Ի���

class CTCPShowDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTCPShowDlg)

public:
	CTCPShowDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTCPShowDlg();

// �Ի�������
	enum { IDD = IDD_DLGTCPSHOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CEdit m_showEdit;
	CString m_strShowEdit;
	void showMsg(CString msg);
	bool m_isShow;
	void showMyDlg();
	afx_msg void OnClose();
	DWORD m_nClientIp;
	void SetClientIPAddr(DWORD ipaddr);
	CIPAddressCtrl clientIpCtrl;
	void setClientPort(UINT port);
	CEdit m_portCtrl;
	CString m_strPort;
	afx_msg void OnBnClickedBtnclear();
	afx_msg void OnBnClickedBtnpause();
	CButton m_BtnPause;
	BOOL m_PauseState;
	CButton m_BtnOk;
	BOOL m_isWriteLog;
};
