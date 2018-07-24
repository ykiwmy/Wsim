// CIOutputView.cpp : 实现文件
//

#include "stdafx.h"
#include "ILKTest.h"
#include "CIOutputView.h"


// CCIOutputView

IMPLEMENT_DYNCREATE(CCIOutputView, CFormView)

CCIOutputView::CCIOutputView()
	: CFormView(CCIOutputView::IDD)
{

}

CCIOutputView::~CCIOutputView()
{
}

void CCIOutputView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCIOutputView, CFormView)
END_MESSAGE_MAP()


// CCIOutputView 诊断

#ifdef _DEBUG
void CCIOutputView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CCIOutputView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CCIOutputView 消息处理程序


BOOL CCIOutputView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO:  在此添加专用代码和/或调用基类

	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
