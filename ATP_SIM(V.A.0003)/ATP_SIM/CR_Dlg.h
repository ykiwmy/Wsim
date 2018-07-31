#pragma once
#include "MainFrm.h"
#include "Device.h"
// CCR_Dlg 对话框
#define MAX_Cr_COUNT  10
class CCR_Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCR_Dlg)

public:
	CCR_Dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCR_Dlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CButton* pCrButton[MAX_Cr_COUNT];
	CButton* pCrStatusButton[5] ;
	virtual BOOL OnInitDialog();
	void DrawCRCheckButton();
	void DrawCrStatusCheckButton();
	afx_msg void OnBnClickedCrSend();
	bool IsBtnSelected(UINT uID);
	void Computecobebit(CString name);
	UINT8 m_CRcodebit;
	afx_msg void OnBnClickedCrClear();
};
