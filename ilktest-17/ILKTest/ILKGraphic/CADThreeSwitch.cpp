#include "stdafx.h"
#include "CADThreeSwitch.h"
// #include "联锁设计工具Doc.h"
// #include "ThreeSwitchDialog.h"
// #include "DevBorder.h"
#include "Define.h"
#include "Colordefs.h"


extern ThrSWIT exThrSwit[MAX_SWITNUM];
extern int  nexThrSwitch_;


int CADThreeSwitch::Angle_ = 60;
int CADThreeSwitch::SecLen_ = 30;
int CADThreeSwitch::NormalLen_ = 20;
int CADThreeSwitch::NailLen_ = 10;
int CADThreeSwitch::RevHeight_ = 52;

CADThreeSwitch::CADThreeSwitch()
{
}

CADThreeSwitch::~CADThreeSwitch()
{
}

IMPLEMENT_SERIAL(CADThreeSwitch, CADDevice, 1)


void CADThreeSwitch::Serialize(CArchive& ar)
{
	CADDevice::Serialize(ar);

	if (ar.IsStoring())
	{	// storing code
		ar.Write(&data_, sizeof(data_));
	}
	else
	{	// loading code
		ar.Read(&data_, sizeof(data_));
	}
}

void CADThreeSwitch::mapWndPt(CRect rect)
{
	
}
