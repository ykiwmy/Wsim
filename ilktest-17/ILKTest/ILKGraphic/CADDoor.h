#pragma once
#include "CADDevice.h"


struct DoorData : public DeviceData
{
	CPoint closePt_[2];
	CPoint openPt_[2];
};


class CADDoor :
	public CADDevice
{
public:
	CADDoor();
	virtual ~CADDoor();
	DECLARE_SERIAL(CADDoor);
	virtual void Serialize(CArchive& ar);

public:
	
	DoorData data_;
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();

	BOOL bCutOff;
	BOOL bClosed;
	void GetStatus();

	virtual void setInputRelay(void);

public:
	bool isPsdOpen();
	bool m_blastSendAtpState;
	bool checkChange();
	bool isPsdCutOff();
	bool isPsdClosed();
	BOOL GetRelayIndexByRName(CString rightName, int* numb);
	bool isOutClose();
	bool isOutOpen(); //KMJ;
};

