#pragma once
#include "MainFrm.h"
#include "Device.h"
// BL_Dlg 对话框
#define MAX_BL_COUNT  50
class BL_Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(BL_Dlg)

public:
	BL_Dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~BL_Dlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif
public:
	CButton* pBlButton[MAX_BL_COUNT];
	CButton* pBlStatusButton[3] ;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	void DrawBlCheckButton();

	void DrawBlStatusCheckButton();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBlSend();
	bool IsBtnSelected(UINT uID);
	void Computecobebit(CString name);
	UINT8 m_Blcodebit;
	afx_msg void OnBnClickedBlClear();
};
