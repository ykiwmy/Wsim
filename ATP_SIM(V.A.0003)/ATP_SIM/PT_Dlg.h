#pragma once
#include "MainFrm.h"
#include "Device.h"
// PT_Dlg 对话框
#define MAX_Pt_COUNT  10
class PT_Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(PT_Dlg)

public:
	PT_Dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~PT_Dlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif
public:
	CButton* pPtButton[MAX_Pt_COUNT];
	CButton* pPtStatusButton[5];
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	void DrawPtCheckButton();

	void DrawPtStatusCheckButton();

	
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg
		bool IsBtnSelected(UINT uID);
	void Computecobebit(CString name);
	void OnBnClickedPtSend();
	afx_msg void OnBnClickedPtClear();
	UINT8 m_PTcodebit;
};
