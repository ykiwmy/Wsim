
// ChildView.h : CChildView 类的接口
//

#pragma once
class CILKTestDoc;

class CChildView : public CScrollView
{
public: // 仅从序列化创建
	CChildView();
	DECLARE_DYNCREATE(CChildView)
public:
	CILKTestDoc* GetDocument() const;
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate(); // 构造后第一次调用
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

#ifndef _DEBUG  // ChildView.cpp 中的调试版本
inline CILKTestDoc* CChildView::GetDocument() const
   { return reinterpret_cast<CILKTestDoc*>(m_pDocument); }
#endif

