#pragma once
#include "afxwin.h"
#include <vector>
using namespace std;

struct DEVICE_RT
{
	UINT16 addr;
	char name[20];
};

struct ROUTE_UNIT_RT
{
	DEVICE_RT rtBtn[3];
	UINT16 rtType;
	UINT16 outputColor;
	UINT16 inputIndex;
	UINT16 outputIndex;
	UINT16 cbtcIndex[3];//default
	UINT16 rtSet;
	UINT16 delay; //default
	UINT16 alterBtn[2];
	UINT16 defIndex[4];
	UINT16 delayTime;
	UINT16 proSectDelay;
	DEVICE_RT rtSection;
	UINT16 seq1;
	DEVICE_RT rtOtherCond;
	UINT16 seq2;
	DEVICE_RT rtSwitch;
	UINT16 endSeq;
};


class CBaseRoute :
	public CWnd
{
public:
	CBaseRoute();
	~CBaseRoute();

	vector<ROUTE_UNIT_RT> rtUnit_;
	vector<DEVICE_RT> rtOne_;
};

