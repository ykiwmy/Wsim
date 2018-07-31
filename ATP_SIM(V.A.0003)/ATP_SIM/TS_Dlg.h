#pragma once

// TS_Dlg 对话框
#include "MainFrm.h"
#include "Device.h"
#define MAX_TS_COUNT        (200) // CheckBox的总个数
class TS_Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(TS_Dlg)

public:
	TS_Dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~TS_Dlg();
	void DrawTSCheckButton();
	void DrawTsStatusCheckButton();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	

	DECLARE_MESSAGE_MAP()
public:
	CButton* pTsButton[MAX_TS_COUNT] ;
	CButton* pTsStatusButton[5] ;
	virtual BOOL OnInitDialog();
	bool IsBtnSelected(UINT uID);
	void Computecobebit(CString name);
	afx_msg void OnBnClickedTsSend();
	UINT8 m_TScodebit;
	afx_msg void OnBnClickedTsClear();
};
