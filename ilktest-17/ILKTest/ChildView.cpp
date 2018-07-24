
// ChildView.cpp : CChildView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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

// CChildView 构造/析构

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

// CChildView 绘制

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
	SetTimer(5, 400, NULL); //每400ms发送一次zc数据
}


// CChildView 诊断

#ifdef _DEBUG
void CChildView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CChildView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CILKTestDoc* CChildView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CILKTestDoc)));
	return (CILKTestDoc*)m_pDocument;
}
#endif //_DEBUG


// CChildView 消息处理程序


void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

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
		// 打包采集板数据
		GetDocument()->SendIOData();
		break;
	case 3:
		// 打包采集板数据
		GetDocument()->CheckCnctStatus();
		break;
	case 4:
		// 打包采集板数据
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
