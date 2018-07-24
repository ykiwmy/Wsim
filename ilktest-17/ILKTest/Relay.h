#pragma once

class IOItem; // 更新 IOItem 的显示状态
class CADDevice; // 给倒霉的设备发送 CIOView 中的点击消息

// 倒霉的继电器
class Relay
{
public:
	Relay(CString deviceName, CString relayName, BOOL bInput, BOOL bOutput, CADDevice* pDevice);
	~Relay();
public:
	enum RELAY_STAT
	{
		STAT_DOWN/*落下*/, STAT_UP/*吸起*/, STAT_NEIGHER, STAT_BOTH,
	};
public:
	CString deviceName_;	// 设备名称
	CString	name_;			// 名称
	BOOL	bInput_;		// 是否输入类型
	BOOL	bOutput_;		// 是否输出类型
	int		boardID_[2];	// 板号（先输入，后输出）
	int		portID_[2];		// 位号（先输入，后输出）
	RELAY_STAT	stat_;		// 状态
	RELAY_STAT	lastStat_;	// 上一贞状态
	IOItem* pIoItem_[2];	// IOItem 的指针，获取IO端口是否有效的状态，设置IO端口的显示颜色（标识继电器当前状态）
	BOOL bDriveUp_;
	int  driveSecond_;
	CADDevice* pDevice_;	// 倒霉的设备
	CADDevice* pFDevice_;	//复示设备
public:
	BOOL isUp(void)   { return stat_ == STAT_UP;	}  //吸起，
	BOOL isDown(void) { return stat_ == STAT_DOWN;	}//落下
	void setStat(BOOL bRelayUp, bool bInvalidateDevice);
	BOOL setStat(BOOL bRelayUp, bool bInvalidateDevice,BOOL recvFromCI);
	void ChangeRelayStat();
	void driveRelay(BOOL bDriveUp, int driveSecond); 
	void InvalidateDevice();
	void SetFDevice(CADDevice* pDev);
	void setStateByClicked(BOOL bRelayUp, bool bInvalidateDevice);
	BOOL UpdatapIoItem(BOOL isRelayUp);
};

