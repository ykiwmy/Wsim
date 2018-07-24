#pragma once
#include "CADDevice.h"

struct SealCountData : public DeviceData
{
	CRect	 tailRect_;
	CPoint   textPoint;
	LOGFONT  CountNumbFont_;// ×ÖÌå
	COLORREF bgColor_;
	COLORREF textColor_;	// ÑÕÉ«
};

class CADSealCount :
	public CADDevice
{
public:
	CADSealCount();
	virtual ~CADSealCount();
	DECLARE_SERIAL(CADSealCount)

public:
	SealCountData data_;
	CFont font_;
	CString strSealCount;

	enum RECT_SIZE
	{
		//DEFAULT_RECT_WIDTH = 70, DEFAULT_RECT_HEIGHT = 30,
		DEFAULT_RECT_WIDTH = 34, DEFAULT_RECT_HEIGHT = 14,   //³¤¡¢¿í¡£
	};
	virtual void Serialize(CArchive& ar);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
public:
	bool isShowName;  // add by yu 2016-1-20
};

