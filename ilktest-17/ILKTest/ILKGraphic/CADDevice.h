#pragma once
#include "CADGraphic.h"
#include "Label.h"
#include "DeviceData.h"


#define MAX_LOGICSWITCH_NUM	4  //add by lwm 2016-12-13


class CADDevice :
	public CADGraphic
{
public:
	CADDevice(void);
	virtual ~CADDevice(void);
	DECLARE_SERIAL(CADDevice)
	virtual void Serialize(CArchive& ar);
public:
	// 画区段
	void drawSection(CPaintDC& dc, CPoint pt[], int nPt, BOOL bOccupied);

	//逻辑区段
	void drawLogicSection(CPaintDC& dc, CPoint pt[], int nPt, BOOL bAT, BOOL bUT);

	// 画绝缘结
	void drawInsu(CPaintDC& dc, CPoint pt[], int type[], int nInsu);

	CPoint getCrossPoint(CPoint& line1Pt1, CPoint& line1Pt2, CPoint& line2Pt1, CPoint& line2Pt2);
public:
	DeviceInput io_;
	Label name_;
	CRect nameRect_;

	// 测试添加
	BOOL bStatChanged_;
public:
	static int SectionWidth_;
	static int InsuHeight_;

	enum INSU_TYPE {
		NORMAL_INSU,	// 普通绝缘
		LIMIT_INSU,		// 超限绝缘
		NON_INSU,		// 无绝缘
	};

	enum 
	{
		OCCUPIED, LOCKED, IDLE
	};

	DECLARE_MESSAGE_MAP()
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
public:
	int TwoPtDistance(CPoint& pt1, CPoint& pt2);
public:
	// 上位机添加的函数

	int getBits(UINT16 src, int bitFlag, int nBits); // 解析报文用

	BOOL checkStatus(void* lastStatus, void* status, int statusSize); // 检查设备状态是否更新

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	virtual BOOL setPreCondition(CString& operation) { return TRUE; } // 处理测试用例中的预设定的设备状态

	virtual BOOL checkResult(CString& result) { return TRUE; } // 检查测试用例是否满足预期

	virtual void setInputRelay(void) {} // 根据驱动的继电器，更新采集的继电器
	void SetRelayState(CString relayName, bool bStatus);
	bool getRelayState(CString relayName);
	void DriveRelayState(CString relayName, bool bStatus);
	CString eleType;
	CString eleIndex;

	UINT16 devType_3M;
	UINT32 devId_3M;

	BOOL statusChanged;
	BOOL m_bFlash;
	int m_nflashCount;
};

