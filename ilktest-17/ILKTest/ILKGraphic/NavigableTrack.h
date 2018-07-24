#pragma once
#include "caddevice.h"
#include "CADButton.h"
#include "CADDeLight.h"


struct NavigableTrackData
{
	CRect lineRect_[2];
};


class NavigableTrack :
	public CADDevice
{
public:
	NavigableTrack(void);
	virtual ~NavigableTrack(void);
	DECLARE_SERIAL(NavigableTrack)
	virtual void Serialize(CArchive& ar);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
public:
	NavigableTrackData data_; // Êý¾Ý
};

