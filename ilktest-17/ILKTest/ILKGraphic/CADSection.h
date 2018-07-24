#pragma once
#include "CADDevice.h"
#include "DeviceData.h"


struct SectionData : public DeviceData
{
#define SECTION_TOTAL_N_PT  4 + 8 + 4
	CPoint pt_[4];		// 区段的四个点
	CPoint sealPt_[8];	// 封锁框
	CPoint insuPt_[4];	// 绝缘结
	CRect trainTrack_;	// 列车信息显示
	int insuType_[2];	// 绝缘结类型
	int sectionType_;
	BYTE byAfgID_;		//防淹门ID
	BYTE byAfgID2_;
	WORD wUpStationID_; //上侧站台ID
	WORD wDownStationID_;//下侧站台ID
	bool bShowSectionName_;
	bool bShowDelayTime_;
	CRect SectionNameRect_;
	CRect DelayTimeRect_;
	int backup_[N_BTYES_BACKUP - 8 - 4 - 2];
};


class CADSection :
	public CADDevice
{
public:
	CADSection(void) : m_blastSendAtpState(false)
	{}
	virtual ~CADSection(void) {}
	DECLARE_SERIAL(CADSection)
	virtual void Serialize(CArchive& ar);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();

	void setTvsState(CPaintDC& pDC);
	void setLogicState(CPaintDC& pDC);
public:
	virtual void compressWindow();
	virtual void setSealFrame(CPoint pt[], int height, bool bWnd = false);
	virtual void OnRPopMenuEx(int flag);

public:
	SectionData data_;
	DeviceIO logicDeviceIO_;
	enum SECTION_TYPE
	{
		TRACK_SECTION,		// 股道
		NO_SWITCH_SECTION,	// 无岔区段
	};

	virtual BOOL setPreCondition(CString& operation);
	virtual BOOL checkResult(CString& result);

	void setSectionState(bool isOcuppy);

	bool getOccupyStatus();
	bool m_blastSendAtpState;
	bool checkChange();
	void drawRealSectionName();
	Label realSecName_;

	BOOL isFSSection();
	void setPreResetState(bool isReset);

	void setZcCommRelayAllLTEState(BOOL bAT, BOOL bUT);
};

