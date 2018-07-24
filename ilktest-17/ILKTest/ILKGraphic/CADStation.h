#pragma once
#include "CADGraphic.h"
#include "CADButton.h"
#include "CADDoor.h"

class CADStation :
	public CADGraphic
{
public:
	CADStation(void);
	virtual ~CADStation(void);
	DECLARE_SERIAL(CADStation)
	virtual void Serialize(CArchive& ar);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	enum STATION_GRAPHIC_SIZE
	{
		N_BUTTON = 4,
		STATION_DEFAULT_WIDTH = 120, STATION_DEFAULT_HEIGHT = 90
	};
	CString		stationName_;
	CADButton	btn_[N_BUTTON];
	CRect		btnRect_[N_BUTTON];
	CADDoor		door_[2];
	CRect		doorRect_[2];
	int stationNo_;
	int stationID_;
	int	nBtn_;
public:
	
	BOOL upDoor;
	BOOL downDoor;
	int Width;
	int Height;
};

