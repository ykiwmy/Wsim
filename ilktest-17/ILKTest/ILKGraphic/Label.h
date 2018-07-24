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
		COLORREF textColor_; // 前景色
		COLORREF bgColor_;   // 背景色
		LOGFONT  logFont_;   // 字体
		CRect	 rect_;      // 显示位置
	}data_;
	CString text_; // 文字

	// 不存储
	CFont font_;
	CADDevice* pDev_;

	// 上位机添加操作
	void setTextColor(COLORREF color);
	void setBackgroudColor(COLORREF color);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};



