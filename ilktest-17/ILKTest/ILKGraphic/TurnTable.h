#pragma once
#include "caddevice.h"
#include "CADButton.h"
#include "CADDeLight.h"


struct TurnTableData
{
	CPoint connectPt_[2];
};


class TurnTable :
	public CADDevice
{
public:
	TurnTable(void);
	virtual ~TurnTable(void);
	DECLARE_SERIAL(TurnTable)
	virtual void Serialize(CArchive& ar);
	virtual void compressWindow();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
public:
	TurnTableData data_;
};

