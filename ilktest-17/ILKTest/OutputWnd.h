
#pragma once


class COutputWnd : public CDockablePane
{
// ππ‘Ï
public:
	COutputWnd();
	virtual ~COutputWnd();
protected:
	CMFCTabCtrl	m_wndTabs;
	CListBox outList_;
	CListBox errList_;
public:
	void UpdateFonts();
	void AdjustHorzScroll(CListBox& wndListBox);
public:
	CListBox& outList(void) { return outList_; }
	CListBox& errList(void) { return errList_; }
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
};

