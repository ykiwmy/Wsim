#pragma once
#include "CADGraphic.h"


class CADDevice;


class Label :
	public CADGraphic
{
public:
	Label(void);
	virtual ~Label(void);
	DECLARE_SERIAL(Label);
	virtual void Serialize(CArchive& ar);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
public:
	struct {
		COLORREF textColor_; // ǰ��ɫ
		COLORREF bgColor_;   // ����ɫ
		LOGFONT  logFont_;   // ����
		CRect	 rect_;      // ��ʾλ��
	}data_;
	CString text_; // ����

	// ���洢
	CFont font_;
	CADDevice* pDev_;

	// ��λ����Ӳ���
	void setTextColor(COLORREF color);
	void setBackgroudColor(COLORREF color);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};



