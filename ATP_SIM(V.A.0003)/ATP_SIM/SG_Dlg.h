#pragma once

#include "afxcmn.h"
#include "MainFrm.h"
#include "Device.h"
//#include "ATP_SIMDoc.h"
// SG_Dlg 对话框
#define MAX_SIGN_COUNT        (33) //
#define MAX_ROW            (5)  // 一排最大放4个CheckBox


class SG_Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(SG_Dlg)

public:
	SG_Dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SG_Dlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void DrawSignalCheckButton();
	void DrawSignalStatusCheckButton();
	bool IsBtnSelected(UINT uID);
public:
	CButton* pSignalButton[MAX_SIGN_COUNT];
	CButton* pSignalStatusButton[5] ;
	afx_msg void OnBnClickedSgSend();
	void Computecobebit(CString name);
	UINT8 m_codebit;
	afx_msg void OnBnClickedSgClear();
};

