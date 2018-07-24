
// ChildView.h : CChildView ��Ľӿ�
//

#pragma once
class CILKTestDoc;

class CChildView : public CScrollView
{
public: // �������л�����
	CChildView();
	DECLARE_DYNCREATE(CChildView)
public:
	CILKTestDoc* GetDocument() const;
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate(); // ������һ�ε���
	virtual ~CChildView();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#ifndef _DEBUG  // ChildView.cpp �еĵ��԰汾
inline CILKTestDoc* CChildView::GetDocument() const
   { return reinterpret_cast<CILKTestDoc*>(m_pDocument); }
#endif

