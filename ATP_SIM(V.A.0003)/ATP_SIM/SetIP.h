#pragma once


// SetIP �Ի���

class SetIP : public CDialogEx
{
	DECLARE_DYNAMIC(SetIP)

public:
	SetIP(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SetIP();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SetIP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
