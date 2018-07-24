#include "stdafx.h"
#include "TurnTable.h"
#include "ColorDef.h"
#include <math.h>

TurnTable::TurnTable(void)
{
}


TurnTable::~TurnTable(void)
{
}


BEGIN_MESSAGE_MAP(TurnTable, CADDevice)
	ON_WM_PAINT()
	ON_WM_CREATE()
END_MESSAGE_MAP()


void TurnTable::OnPaint()
{
	CPaintDC dc(this);
	CRect rect;
	GetClientRect(&rect);

	dc.SelectObject(SectionPen());
	dc.SelectStockObject(NULL_BRUSH);
	dc.Ellipse(rect);

	dc.MoveTo(data_.connectPt_[0]);
	dc.LineTo(data_.connectPt_[1]);
}


IMPLEMENT_SERIAL(TurnTable, CADDevice, 1)
void TurnTable::Serialize(CArchive& ar)
{
	CADDevice::Serialize(ar);

	if (!ar.IsStoring())
	{	// loading code
		ar.Read(&data_, sizeof(TurnTableData));
	}
}


void TurnTable::compressWindow()
{
	CRect rect;
	GetClientRect(&rect);
	rect.InflateRect(2, 2);

	CRgn rgn;
	rgn.CreateEllipticRgnIndirect(&rect);
	SetWindowRgn((HRGN)rgn, TRUE);
}

