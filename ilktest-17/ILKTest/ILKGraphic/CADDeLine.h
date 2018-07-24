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
		CPoint   pt_[2]; // װ����������
		COLORREF color_; // װ������ɫ
		int      width_; // װ���߿��
	}data_;
};

