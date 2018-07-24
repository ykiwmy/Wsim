#pragma once


class Relay;
class CZcCommRelay;

// CIOCaptionWnd

class CIOCaptionWnd : public CWnd
{
	DECLARE_DYNAMIC(CIOCaptionWnd)

public:
	CIOCaptionWnd(CString caption, Relay* pRelay);
	CIOCaptionWnd(CString caption, CZcCommRelay* pZcRelay, bool bZc);
	virtual ~CIOCaptionWnd();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
public:
	CString caption_;
	Relay*	pRelay_;
	CZcCommRelay* pZcRelay_;
	bool bZc_;
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


