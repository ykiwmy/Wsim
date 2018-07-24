#include "stdafx.h"
#include "NavigableTrack.h"
#include "ColorDef.h"


NavigableTrack::NavigableTrack(void)
{
}



NavigableTrack::~NavigableTrack(void)
{
}


IMPLEMENT_SERIAL(NavigableTrack, CADDevice, 1)


void NavigableTrack::Serialize(CArchive& ar)
{
	CADDevice::Serialize(ar);

	if (!ar.IsStoring())
	{	// loading code
		ar.Read(&data_, sizeof(NavigableTrackData));
	}
}


BEGIN_MESSAGE_MAP(NavigableTrack, CADDevice)
	ON_WM_PAINT()
END_MESSAGE_MAP()


void NavigableTrack::OnPaint()
{
	CPaintDC dc(this); 

	dc.SelectObject(SectionPen());
	dc.SelectStockObject(NULL_BRUSH);
	dc.Rectangle(data_.lineRect_[0]);
	dc.Rectangle(data_.lineRect_[1]);
}
