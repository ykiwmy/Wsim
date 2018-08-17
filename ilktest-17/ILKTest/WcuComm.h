#pragma once

class IOItem;
class CADDevice;

class WcuComm
{
	

public:
	WcuComm::WcuComm(CString deviceName,CString relayName, CADDevice* pDevice);
	~WcuComm();

public:
	enum WCU_STAT
	{
		COMM_LOW, COMM_HIGH, COMM_NEIGHER, COMM_BOTH,
	};
	CString deviceName_;	// Éè±¸Ãû³Æ
	CString name_;
	CADDevice* pDevice_;
	WCU_STAT	stat_;		// ×´Ì¬

public:
	BOOL isHigh(void) { return stat_ == COMM_HIGH; }  //1
	BOOL isLow(void) { return stat_ == COMM_LOW; } //0
};

