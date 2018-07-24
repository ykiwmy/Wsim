// SplitterFrame.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ILKTest.h"
#include "SplitterFrame.h"
#include "ChildView.h"
#include "IOView.h"


// CSplitterFrame

IMPLEMENT_DYNCREATE(CSplitterFrame, CFrameWnd)

CSplitterFrame::CSplitterFrame()
: bShowIOView_(TRUE)
, pChildView_(NULL)
, pIOView_(NULL)
{

}

CSplitterFrame::~CSplitterFrame()
{
}


BEGIN_MESSAGE_MAP(CSplitterFrame, CFrameWnd)
END_MESSAGE_MAP()


// CSplitterFrame ��Ϣ�������


BOOL CSplitterFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO:  �ڴ����ר�ô����/����û���
	m_wndSplitter.CreateStatic(this, 1, 2);

	CRect rect;
	GetClientRect(&rect);

	m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CChildView), CSize(rect.Width(), rect.Height()), pContext);
	m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CIOView), CSize(), pContext);

	pChildView_ = dynamic_cast<CChildView*>(m_wndSplitter.GetPane(0, 0));
	pIOView_ = dynamic_cast<CIOView*>(m_wndSplitter.GetPane(0, 1));

	return TRUE;
}


void CSplitterFrame::On32771()
{
	// TODO:  �ڴ���������������
	bShowIOView_ = FALSE;
	On32772();
}


void CSplitterFrame::On32772()
{
	// TODO:  �ڴ���������������
	bShowIOView_ = !bShowIOView_;

	CRect rect;
	GetClientRect(&rect);
	
	m_wndSplitter.SetColumnInfo(0, bShowIOView_ ? rect.Width() - IO_VIEW_WIDTH : rect.Width(), 100);
	m_wndSplitter.SetColumnInfo(1, bShowIOView_ ? IO_VIEW_WIDTH : 0, 100);
	m_wndSplitter.RecalcLayout();
}
