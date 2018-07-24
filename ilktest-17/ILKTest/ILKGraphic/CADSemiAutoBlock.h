#pragma once
#include "cadgraphic.h"
#include "Arrow.h"
#include "Label.h"
#include "CADButton.h"
#include "CADDeLight.h"

class CADSemiAutoBlock :
	public CADGraphic
{
public:
	CADSemiAutoBlock(void);
	~CADSemiAutoBlock(void);
	DECLARE_SERIAL(CADSemiAutoBlock)
	virtual void Serialize(CArchive& ar);

	virtual void mapWndPt(CRect rect);

public:
	enum STATION_GRAPHIC_SIZE
	{
		DIR_LEFT = 0, DIR_RIGHT = 1, DIR_BOTH = 2,
		MAX_N_ARROW = 2, MAX_N_BTN = 3, MAX_N_LIGHT = 6,
		DEFAULT_WIDTH = 160 /*140*/, DEFAULT_HEIGHT = 140 /*90*/
	};
	CString		caption_;
	Arrow		arrow_[MAX_N_ARROW];
	CADButton	btn_[MAX_N_BTN];
	CADDeLight  light_[MAX_N_LIGHT];
	CRect		arrowRect_[MAX_N_ARROW];
	CRect		btnRect_[MAX_N_BTN];
	CRect       lightRect_[MAX_N_LIGHT];
	int type_;
	int nArrow_;
	int nBtn_;
	Label accidentCounter;
};

