#pragma once
#include "CADDevice.h"
#include "DeviceData.h"
#include "CADButton.h"


struct SignalData : public DeviceData
{

#define SIGNAL_TOTAL_N_PT (1 + 2 + 2 + 4 + 4 + 7 + 4)
	CPoint	basePt_;		// 基点
	CPoint	vLine_[2];		// 竖线的两个点
	CPoint	hLine_[2];		// 横线的两个点
	CPoint	sealPt_[4];		// 封锁框
	CPoint	crossPt_[4];	// 信号机打叉点 5 7 6 8
	CPoint  arrowPt_[7];	// 箭头-------------------------------------
	CRect	lightRect_[2];	// 两个信号灯坐标
	int		nLight_;		// 包含多少个灯(一个或两个)

	COLORREF defaultColor_; // 默认灯色
	int		 btnID2_;		// 列车兼调车信号机的调车按钮号
	int		 type_;			// 信号机类型
	BOOL	 bSingleSet_;	// 是否单置
	int		 shuntType_;		// 信号机类型(调车、列车、列车兼调车、变更按钮)
	BOOL	 bGuid;			// 是否可引导
	float	 angle_;         //_H 旋转角度(2015-12-08新增)
	UINT16   proBtnAddr_[4]; //_H 防护道岔相关的按钮地址，只需要3个，但是为了保持数据格式不变，暂设为4个
	int		 backup_[N_BTYES_BACKUP - 2 - 1 - 2];

	enum shunType
	{
		TRAIN_SIGNAL, SHUNT_SIGNAL, TRAIN_SHUNT_BOTH, CHANGE_ROUTE_SIGNAL, END_LINE_SIGNAL, SHUNT_TRAIN_BOTH
	};
};


class CADSignal :
	public CADDevice
{
public:
	CADSignal(void);
	virtual ~CADSignal(void);
	DECLARE_SERIAL(CADSignal)  
	virtual void Serialize(CArchive& ar);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	SignalData		data_;			// 信号机数据
	CADButton		trainButton_;	// 倒霉的列车按钮
	CRect			btnRect_;		// 列车按钮的位置
	BOOL 			bSwapPos_;

	int				ioType_;		// 信号机的驱采属性（一共十五种）

	enum
	{
		V_HEIGHT = 16, H_LEN = 8, r = 8, DIS_2_SEC = 5,//_H 基点与信号机间的距离为5
	};

	enum SignalColor
	{
		SIGNAL_RED, SIGNAL_GREEN, SIGNAL_YELLOW, SIGNAL_BLUE, SIGNAL_WHITE,
		SIGNAL_RED_YELLOW, SIGNAL_RED_WHITE, SIGNAL_GREEN_YELLOW,SIGNAL_DOUBLE_YELLOW, SIGNAL_DOUBLE_GREEN,
		SIGNAL_DS, SIGNAL_2DS, SIGNAL_CYAN,SIGNAL_MD
	};
	static SignalColor getSignalColor_1(CADSignal* pSignal);
	static SignalColor getSignalColor_2(CADSignal* pSignal);
	static SignalColor getSignalColor_3(CADSignal* pSignal);
	static SignalColor getSignalColor_4(CADSignal* pSignal);
	static SignalColor getSignalColor_5(CADSignal* pSignal);
	static SignalColor getSignalColor_6(CADSignal* pSignal);
	static SignalColor getSignalColor_7(CADSignal* pSignal);
	static SignalColor getSignalColor_8(CADSignal* pSignal);
	static SignalColor getSignalColor_9(CADSignal* pSignal);
	static SignalColor getSignalColor_10(CADSignal* pSignal);
	static SignalColor getSignalColor_11(CADSignal* pSignal);
	static SignalColor getSignalColor_12(CADSignal* pSignal);
	static SignalColor getSignalColor_13(CADSignal* pSignal);
	static SignalColor getSignalColor_14(CADSignal* pSignal);
	static SignalColor getSignalColor_15(CADSignal* pSignal);
	static SignalColor getSignalColor_16(CADSignal* pSignal);
	static SignalColor getSignalColor_17(CADSignal* pSignal);
	static SignalColor getSignalColor_18(CADSignal* pSignal);
	SignalColor getSignalState(void);
	void compressWindow();
	virtual BOOL setPreCondition(CString& operation);
	virtual BOOL checkResult(CString& result);
	virtual void setInputRelay(void);

	void setCloseup(CPoint pt[], int height, bool bWnd);
	bool getDDJstate();
	SignalColor m_lastSendAtpColor;
	bool m_blastSendAtpDDState;
	bool checkChange();
	void SetDDJstate(bool bDDJStatus);
	void SetDsState(bool bDsStatus);

	void setZcCommRelayAllSignalLightMode(BOOL bLightOff);
	bool isNeed2DJUp();
	bool isNeedDJDown();
	CADSignal::SignalColor getColor(void);
};

