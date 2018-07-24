#include "stdafx.h"
#include "CADDeLine.h"
#include "ColorDef.h"
#include "DevBorder.h"


CADDeLine::CADDeLine(void)
{
}

   
CADDeLine::~CADDeLine(void)
{
}


IMPLEMENT_SERIAL(CADDeLine, CADGraphic, 1)


void CADDeLine::Serialize(CArchive& ar)
{
	if (!ar.IsStoring())
	{	// loading code
		ar.Read(&data_, sizeof(data_));
	}
}


BEGIN_MESSAGE_MAP(CADDeLine, CADGraphic)
	ON_WM_PAINT()
END_MESSAGE_MAP()


void CADDeLine::OnPaint()
{
	CPaintDC dc(this); 
	CPen pen(PS_SOLID, data_.width_, data_.color_);
	dc.SelectObject(&pen);

	dc.MoveTo(data_.pt_[0]);
	dc.LineTo(data_.pt_[1]);
}


void CADDeLine::compressWindow() 
{
	CPoint wndPt[4];
	DevBorder border(data_.pt_[0], data_.pt_[1],  data_.width_ + 2);
	for (int i = 0; i < 4; i++)
		wndPt[i] = border.pt(i);
	setCRgnWindow(wndPt, 4);
}
