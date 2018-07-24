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
	CString deviceName_;	// �豸����
	CString name_;
	int devType; //
	int codeSeqIndex[2];
	BOOL	bInput_;		// �Ƿ���������
	BOOL	bOutput_;		// �Ƿ��������
	int		boardID_[2];	// ��ţ������룬�������
	int		portID_[2];		// λ�ţ������룬�������
	ZC_RELAY_STAT	stat_;		// ״̬
	ZC_RELAY_STAT	lastStat_;	// ��һ��״̬
	IOItem* pIoItem_[2];
	CADDevice* pDevice_;	// ��ù���豸
	CADDevice* pFDevice_;	//��ʾ�豸
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

