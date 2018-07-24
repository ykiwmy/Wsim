#pragma once
#include "CADGraphic.h"

class CADDeLine :
	public CADGraphic
{
public:
	CADDeLine(void);
	virtual ~CADDeLine(void);
	DECLARE_SERIAL(CADDeLine);
	virtual void Serialize(CArchive& ar);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
public:
	virtual void compressWindow();
public:
	struct {
		CPoint   pt_[2]; // 装饰线两个点
		COLORREF color_; // 装饰线颜色
		int      width_; // 装饰线宽度
	}data_;
};

