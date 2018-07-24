#pragma once
#include "CADDevice.h"

#define AFG_POINT_NUM  8
struct AfsDoorData : public DeviceData
{
	CPoint   Point[AFG_POINT_NUM];
	COLORREF  color_[3];
	COLORREF  TextColor_[3];
	int backup_[N_BTYES_BACKUP];
};

class CADAfgDoor :
	public CADDevice
{
public:
	CADAfgDoor();
	CADAfgDoor(CWnd* pParent);
	virtual ~CADAfgDoor();
	DECLARE_SERIAL(CADAfgDoor)
public:
	AfsDoorData data_;
	DeviceIO deviceIO2_;//继电器2对应的防淹门
	virtual void compressWindow();

	enum TRIANGLE_SIZE
	{
		DEFAULT_TRIANGLE_BASELENGTH = 16, DEFAULT_TRIANGLE_HEIGHT = 8,   //底边长、高
		DEFAULT_AFG_LENGTH = 30,
	};
	virtual void Serialize(CArchive& ar);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	bool m_blastSendAtpState;
	bool isOpened();
	bool chackChange();
};