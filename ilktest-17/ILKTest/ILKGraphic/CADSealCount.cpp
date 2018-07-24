#include "stdafx.h"
#include "CADSealCount.h"
#include "ColorDef.h"
#include "ChildView.h"

CADSealCount::CADSealCount()
: isShowName(TRUE)
{
	data_.bgColor_ = COLOR_GRAY_100;//GetSysColor(COLOR_BTNFACE);
	data_.textColor_ = COLOR_WHITE;
	data_.textPoint.x = data_.textPoint.y =1;
	strSealCount = L"00000";

}

CADSealCount::~CADSealCount()
{
}

void CADSealCount::Serialize(CArchive& ar)
{
	CADDevice::Serialize(ar);//���л����ֺ�IO����

	if (!ar.IsStoring())
	{		// loading code
		ar.Read(&data_, sizeof(data_));

		font_.DeleteObject();
		font_.CreateFontIndirectW(&data_.CountNumbFont_);
	}
}
BEGIN_MESSAGE_MAP(CADSealCount, CADDevice)
	ON_WM_PAINT()
END_MESSAGE_MAP()

IMPLEMENT_SERIAL(CADSealCount, CADDevice/*CADGraphic*/, 1);//_H ʲô��CADGraphic������CADDevice

void CADSealCount::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CBrush btnBrush(data_.bgColor_);
	dc.SelectObject(&btnBrush);

	dc.Rectangle(data_.tailRect_);//_���õ�ǰ���ʻ��ƾ��Σ��ڲ��ɵ�ǰ��ˢ���
	dc.SetBkColor(data_.bgColor_); // ����
	dc.SelectObject(&font_); // ����
	dc.SetTextColor(data_.textColor_); // ǰ��
	dc.TextOutW(data_.textPoint.x, data_.textPoint.y, strSealCount);

	//add by yu 
	if (data_.bValid_)
	{
		name_.ShowWindow(SW_SHOW);
	}
	else
	{
		name_.ShowWindow(SW_HIDE);
	}
	name_.Invalidate();
}
