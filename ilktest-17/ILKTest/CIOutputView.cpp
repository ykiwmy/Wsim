// CIOutputView.cpp : ʵ���ļ�
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


// CCIOutputView ���

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


// CCIOutputView ��Ϣ�������


BOOL CCIOutputView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO:  �ڴ����ר�ô����/����û���

	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
