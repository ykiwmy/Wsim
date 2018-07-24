#pragma once
#include "CADDevice.h"
#include "DeviceData.h"
#include "CADButton.h"


struct SignalData : public DeviceData
{

#define SIGNAL_TOTAL_N_PT (1 + 2 + 2 + 4 + 4 + 7 + 4)
	CPoint	basePt_;		// ����
	CPoint	vLine_[2];		// ���ߵ�������
	CPoint	hLine_[2];		// ���ߵ�������
	CPoint	sealPt_[4];		// ������
	CPoint	crossPt_[4];	// �źŻ����� 5 7 6 8
	CPoint  arrowPt_[7];	// ��ͷ-------------------------------------
	CRect	lightRect_[2];	// �����źŵ�����
	int		nLight_;		// �������ٸ���(һ��������)

	COLORREF defaultColor_; // Ĭ�ϵ�ɫ
	int		 btnID2_;		// �г�������źŻ��ĵ�����ť��
	int		 type_;			// �źŻ�����
	BOOL	 bSingleSet_;	// �Ƿ���
	int		 shuntType_;		// �źŻ�����(�������г����г�������������ť)
	BOOL	 bGuid;			// �Ƿ������
	float	 angle_;         //_H ��ת�Ƕ�(2015-12-08����)
	UINT16   proBtnAddr_[4]; //_H ����������صİ�ť��ַ��ֻ��Ҫ3��������Ϊ�˱������ݸ�ʽ���䣬����Ϊ4��
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
	SignalData		data_;			// �źŻ�����
	CADButton		trainButton_;	// ��ù���г���ť
	CRect			btnRect_;		// �г���ť��λ��
	BOOL 			bSwapPos_;

	int				ioType_;		// �źŻ����������ԣ�һ��ʮ���֣�

	enum
	{
		V_HEIGHT = 16, H_LEN = 8, r = 8, DIS_2_SEC = 5,//_H �������źŻ���ľ���Ϊ5
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

