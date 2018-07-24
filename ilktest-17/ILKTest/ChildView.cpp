
// ChildView.cpp : CChildView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "ILKTest.h"
#endif

#include "ILKTestDoc.h"
#include "ChildView.h"
#include "SAMProcess.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define  MAXDRAW_X 5000
#define  MAXDRAW_Y 3000
// CChildView

IMPLEMENT_DYNCREATE(CChildView, CScrollView)

BEGIN_MESSAGE_MAP(CChildView, CScrollView)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CChildView ����/����

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CScrollView::PreCreateWindow(cs);
}

// CChildView ����

void CChildView::OnDraw(CDC* pDC)
{
	CILKTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rect(0, 0, MAXDRAW_X, MAXDRAW_Y);
	pDC->SelectStockObject(BLACK_BRUSH);
	pDC->Rectangle(rect);
}

void CChildView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	theApp.thePView = this;
	CSize sizeTotal(MAXDRAW_X, MAXDRAW_Y);
	SetScrollSizes(MM_TEXT, sizeTotal);

	SetTimer(0,1000,NULL);
	SetTimer(1, 500, NULL);
	SetTimer(2, 160, NULL);
	SetTimer(3, 3000, NULL);
	SetTimer(4, 400, NULL);
	SetTimer(5, 400, NULL); //ÿ400ms����һ��zc����
}


// CChildView ���

#ifdef _DEBUG
void CChildView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CChildView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CILKTestDoc* CChildView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CILKTestDoc)));
	return (CILKTestDoc*)m_pDocument;
}
#endif //_DEBUG


// CChildView ��Ϣ�������


void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

	switch (nIDEvent)
	{
	case 0:
		if (GetDocument()->m_isCnctToGalaxy>0)
		{
			GetDocument()->m_tcpSevSock_.OnTimer();
		}
		break;
	case 1:
		GetDocument()->OnDeviceTimer();
		break;
	case 2:
		// ����ɼ�������
		GetDocument()->SendIOData();
		break;
	case 3:
		// ����ɼ�������
		GetDocument()->CheckCnctStatus();
		break;
	case 4:
		// ����ɼ�������
		if (GetDocument()->m_isCnctToSAM > 0)
		{
			GetDocument()->SendAllShowTo3M();
		}
		break;
	case 5:
		GetDocument()->sendZcData();
		break;
	default:
		break;
	}
	
	CScrollView::OnTimer(nIDEvent);
}
