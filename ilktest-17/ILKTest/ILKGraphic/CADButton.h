#pragma once
#include "CADDevice.h"


struct ButtonData : public DeviceData
{
	CRect btnRect_;
	COLORREF bgColor_[2];
	COLORREF captionColor_[2];
	int	btnType_;
	int passwdType_;
	BOOL bSelfReset_;
};


class CADSignal;


class CADButton :
	public CADDevice
{
public:
	CADButton(void);
	virtual ~CADButton(void);
	DECLARE_SERIAL(CADButton)
	virtual void Serialize(CArchive& ar);
public:
	ButtonData data_;
	CADSignal* pSignal_; // 倒霉的列车按钮！

	enum BUTTON_TYPE
	{
		PUSH_BUTTON, SMALL_SHIT_BUTTON, ROUND_BUTTON,
	};
	enum BUTTON_SIZE
	{
		DEFAULT_BTN_WIDTH = 60, DEFAULT_BTN_HEIGHT = 24,
		DEFAULT_ROUND_BTN_R = 24,
		DEFAULT_SMALL_BTN_WIDTH = 16,
	};

	enum PASSWORD_TYPE
	{
		NO_PASSWD, SINGLE_PASSWD, DOUBLE_PASSWD,
	};
	void setBtnSize(const CPoint& topLeft, const CSize& sz);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	virtual CString getDeviceName();
public:
	bool isPressed();
	bool m_blastSendAtpState;
	bool chackChange();
};

