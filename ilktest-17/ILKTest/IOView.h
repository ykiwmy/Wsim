#pragma once
#include "IOItem.h"
#include <vector>
#include <array>
using namespace std;

// CIOView 窗体视图

class CILKTestDoc;


class CIOView : public CFormView
{
	DECLARE_DYNCREATE(CIOView)

protected:
	CIOView();           // 动态创建所使用的受保护的构造函数
	virtual ~CIOView();

public:
	enum { IDD = IDD_IOVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	CStatic boardIDLabel_[2][128];
	CStatic boardIDLabelZC_[2][128];
	CFont font_;
	afx_msg void OnStnClickedStaticRelayName();
};

