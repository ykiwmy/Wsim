// IOView.cpp : 实现文件
//

#include "stdafx.h"
#include "ILKTest.h"
#include "IOView.h"
#include "ILKTestDoc.h"

// CIOView

IMPLEMENT_DYNCREATE(CIOView, CFormView)

CIOView::CIOView()
	: CFormView(CIOView::IDD)
{
	font_.CreateStockObject(DEFAULT_GUI_FONT);
}

CIOView::~CIOView()
{
}

void CIOView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CIOView, CFormView)
END_MESSAGE_MAP()


// CIOView 诊断

#ifdef _DEBUG
void CIOView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CIOView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CIOView 消息处理程序

