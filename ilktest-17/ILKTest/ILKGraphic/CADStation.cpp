#include "stdafx.h"
#include "CADStation.h"
#include "ColorDef.h"

CADStation::CADStation()
{
}


CADStation::~CADStation()
{
}

IMPLEMENT_SERIAL(CADStation, CADGraphic, 1)

void CADStation::Serialize(CArchive& ar)
{
	if (!ar.IsStoring())
	{	// loading code
		ar >> stationName_ >> stationNo_ >> stationID_ >> nBtn_ >> upDoor >> downDoor;
	}

	for (int i = 0; i < 4/*nBtn_*/; i++)
	{
		btn_[i].Serialize(ar);
		if (!ar.IsStoring())
		{
			ar >> btnRect_[i];
		}
	}
	if (upDoor)
	{
		door_[0].Serialize(ar);
		if (!ar.IsStoring())
		{
			ar >> doorRect_[0];
		}
	}
	if (downDoor)
	{
		door_[1].Serialize(ar);
		if (!ar.IsStoring())
		{
			ar >> doorRect_[1];
		}
	}
}


BEGIN_MESSAGE_MAP(CADStation, CADGraphic)
	ON_WM_PAINT()
	ON_WM_CREATE()
END_MESSAGE_MAP()


void CADStation::OnPaint()
{
	CPaintDC dc(this);

	dc.SelectStockObject(WHITE_PEN);
	dc.SelectStockObject(BLACK_BRUSH);

	CRect rect;
	GetClientRect(&rect);
	dc.Rectangle(rect);
	dc.SetTextColor(COLOR_WHITE);
	dc.SetBkColor(COLOR_BLACK);
	dc.SelectStockObject(DEFAULT_GUI_FONT);
	dc.DrawText(stationName_, -1, rect, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
}


int CADStation::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CADGraphic::OnCreate(lpCreateStruct) == -1)
		return -1;

	for (int i = 0; i < 4/*nBtn_*/; i++)
	{
		btn_[i].createEmptyWnd(this);
		btn_[i].SetParent(this);
		btn_[i].MoveWindow(btnRect_[i]);
	}

	if (upDoor)
	{
		door_[0].createEmptyWnd(this);
		door_[0].SetParent(this);
		door_[0].MoveWindow(doorRect_[0]);
	}
	if (downDoor)
	{
		door_[1].createEmptyWnd(this);
		door_[1].SetParent(this);
		door_[1].MoveWindow(doorRect_[1]);
	}

	return 0;
}

