#pragma once


// SetIP 对话框

class SetIP : public CDialogEx
{
	DECLARE_DYNAMIC(SetIP)

public:
	SetIP(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SetIP();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SetIP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
