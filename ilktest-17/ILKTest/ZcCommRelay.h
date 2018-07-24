#pragma once

class IOItem;
class CADDevice;

class CZcCommRelay
{
public:
	CZcCommRelay(CString deviceName, int deviceType, CString relayName, BOOL bInput, BOOL bOutput, CADDevice* pDevice);
	~CZcCommRelay();

public:
	enum ZC_RELAY_STAT
	{
		COMM_LOW, COMM_HIGH, COMM_NEIGHER, COMM_BOTH,
	};
	
public:
	CString deviceName_;	// 设备名称
	CString name_;
	int devType; //
	int codeSeqIndex[2];
	BOOL	bInput_;		// 是否输入类型
	BOOL	bOutput_;		// 是否输出类型
	int		boardID_[2];	// 板号（先输入，后输出）
	int		portID_[2];		// 位号（先输入，后输出）
	ZC_RELAY_STAT	stat_;		// 状态
	ZC_RELAY_STAT	lastStat_;	// 上一贞状态
	IOItem* pIoItem_[2];
	CADDevice* pDevice_;	// 倒霉的设备
	CADDevice* pFDevice_;	//复示设备
public:
	BOOL isHigh(void)   { return stat_ == COMM_HIGH; }  //1
	BOOL isLow(void) { return stat_ == COMM_LOW; } //0
	void setStat(BOOL bCommHigh, bool bInvalidateDevice);
	BOOL isHighStateByCommVal(BOOL bCommHigh);
	BOOL isSetOppSiteState(BOOL bCommHigh);
	BOOL isSetSameState(BOOL bCommHigh);
	void setAutoLowStat(BOOL bCommHigh, int iAuto, int srcState, bool bInvalidateDevice);
	
	//void ChangeRelayStat();
	void InvalidateDevice();
	void SetFDevice(CADDevice* pDev);
	void setStateByClicked(BOOL bRelayUp, bool bInvalidateDevice);

	bool isValidSignalCross(int iAuto);
	int getRtSignStopAssureByAsk(int iAuto, int srcState, int curDstState);

	int getSrcStatByCurCommRelay(int addr, CString strCommRelay);
};

