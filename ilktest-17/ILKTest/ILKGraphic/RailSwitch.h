#pragma once
#include "CADDevice.h"
#include "DeviceData.h"


struct SwitchData : public DeviceData
{
#define SWITCH_TOTAL_PT_COUNT 9 + 6 + 14
	CPoint secPt_[3];	// 区段点
	CPoint norPt_[3];   // 直股点
	CPoint revPt_[3];   // 斜股点
	CPoint insuPt_[6];  // 绝缘结 顺序: section, normal, reverse
	CPoint sealPt_[14]; // 封锁框
	int insuType_[3];	// 绝缘结类型
	int	type_;
	int	frontPos_;	// 前方岔位置
	DeviceIO	front_;		// 前方岔数据
	DeviceIO	section_;	// 区段数据
	CRect SectionNameRect_;
	CRect DelayTimeRect_;
	bool bShowSectionName_;
	bool bShowDelayTime_;
	BYTE byReserve[2];
	int	backup_[N_BTYES_BACKUP - 9];
};


class RailSwitch :
	public CADDevice
{
public:
	RailSwitch(void);
	virtual ~RailSwitch(void);
	DECLARE_SERIAL(RailSwitch)
	virtual void Serialize(CArchive& ar);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
public:
	virtual void compressWindow();
	virtual void setSealFrame(CPoint pt[], int height, bool bWnd = false);

	void drawSwitLogicSection(CPaintDC& dc);
public:
	SwitchData	data_;		// 道岔数据
	DeviceIO logicDeviceIO_[MAX_LOGICSWITCH_NUM];//逻辑地址 add by lwm 2016-12-13

	enum FRONT_SWITCH_POS
	{
		POS_NORMAL = 0, POS_REVERSE = 1, NO_FRONT_SWITCH = -1
	};

	enum SWITCH_POS
	{
		NEITHER=0,NORMAL,REVERSE,ERROR_NR
	};
public:

	enum {
		SEC_LEN = 30, NORMAL_LEN = 20, NAIL_LEN = 10 
	};
	enum {
		SEC_FLAG, NOR_FLAG, REV_FLAG
	};
	int totalLen_;  //整体长度
//	int revHeight_;  
	int nailHeight_; //岔尖高度
	int nailWidth_;  //岔尖长度

	static int Angle_;		// 角度
	static int SecLen_;		// 道岔区段长度
	static int NormalLen_;	// 直股长度
	static int NailLen_;	// 岔尖长度
	static int RevHeight_;	// 斜股高度



	enum 
	{
		MAX_POS_NEITHER_COUNT = 26 // 13 秒，500ms定时器周期
	};
public:
	BOOL isUp(void) { return (data_.type_ % 2) == 0;}
	virtual void OnPopMenuEx(int flag);

	virtual BOOL setPreCondition(CString& operation);
	virtual BOOL checkResult(CString& result);
	virtual void setInputRelay(void);
	int lastSwitchStatus;  //1为定位 2为反位 3为四开 用于给列车仿真时判断是否状态变化用
	int GetSwitchWCUPos();
	int GetSwitchILPos();
	int m_nlastSendAtpPos;
	bool checkChange();

	CString switchSectionEleType;
	CString switchSectionEleIndex;
	UINT32 switchSectionID_3M;
	UINT32 switchSectonType_3M;
	bool getOccupyStatus();
	bool m_blastSendAtpSectionState;
	bool checkSwitchSectionChange();
	void setDPos(void);
	void setFPos(void);
	void setUnPos(void);
	void drawRealSectionName();
	void setSectionState(bool isOcuppy);
	void setPreResetState(bool isReset);
	Label realSecName_;

	void setZcCommRelayAllLTEState(BOOL bAT, BOOL bUT);
};

