#pragma once
#include "CADDevice.h"
#include "Label.h"


class CADDeLight :
	public CADDevice
{
public:
	CADDeLight(void);
	~CADDeLight(void);
	DECLARE_SERIAL(CADDeLight)
	virtual void Serialize(CArchive& ar);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
public:
	enum 
	{
		N_COLOR = 3
	};
	CRect lightRect_;
	COLORREF color_[N_COLOR];
	int type_;

	enum DE_LIGHT_ATTR
	{
		DEFAULT_R = 12 //20
	};
	bool isPressed();
	bool checkChange();
	BOOL isSDState();
	BOOL isWDState();
	BOOL isDri();
	BOOL isDrb();
	bool m_blastSendAtpState;
};

