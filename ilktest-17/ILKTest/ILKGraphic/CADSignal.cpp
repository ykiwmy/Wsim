#include "stdafx.h"
#include "CADSignal.h"
#include "ColorDef.h"
#include "../Relay.h"
#include "ZcCommRelay.h"
#include "../Resource.h"
#include <algorithm>
#include <array>
using namespace std;


IMPLEMENT_SERIAL(CADSignal, CADDevice, 1)


CADSignal::CADSignal(void)
: ioType_(0)
, m_lastSendAtpColor(SIGNAL_RED)
, m_blastSendAtpDDState(false)
{
}


CADSignal::~CADSignal(void)
{
}


void CADSignal::Serialize(CArchive& ar)
{
	CADDevice::Serialize(ar);
//	trainButton_.Serialize(ar);

	if (!ar.IsStoring())
	{	// loading code
		ar.Read(&data_, sizeof(data_));
//		ar >> btnRect_;
		if (data_.shuntType_ == SignalData::TRAIN_SHUNT_BOTH || data_.shuntType_ == SignalData::SHUNT_TRAIN_BOTH)//_H 2016-06-13
		{
			trainButton_.Serialize(ar);
			ar >> btnRect_;
		}
	}
}


BEGIN_MESSAGE_MAP(CADSignal, CADDevice)
	ON_WM_PAINT()
	ON_WM_CREATE()
END_MESSAGE_MAP()


CADSignal::SignalColor CADSignal::getSignalColor_1(CADSignal* pSignal)
{
	//return pSignal->isRelayUp(0) ? SIGNAL_RED : SIGNAL_DS;
	return SIGNAL_RED;
}

CADSignal::SignalColor CADSignal::getSignalColor_2(CADSignal* pSignal)
{
	/*return pSignal->isRelayUp(1) ?
		(pSignal->isRelayUp(0) ? SIGNAL_WHITE : SIGNAL_BLUE) : SIGNAL_DS;*/
	return pSignal->isRelayUp(0) ? SIGNAL_WHITE : SIGNAL_BLUE;
}

CADSignal::SignalColor CADSignal::getSignalColor_3(CADSignal* pSignal)
{
	/*return pSignal->isRelayUp(1) ?
		(pSignal->isRelayUp(0) ? SIGNAL_WHITE : SIGNAL_RED) : SIGNAL_DS;*/
	return pSignal->isRelayUp(0) ? SIGNAL_WHITE : SIGNAL_RED;
}

CADSignal::SignalColor CADSignal::getSignalColor_4(CADSignal* pSignal)
{
	/*return pSignal->isRelayUp(1) ?
		(pSignal->isRelayUp(0) ? SIGNAL_YELLOW : SIGNAL_RED) : SIGNAL_DS;*/
	return pSignal->isRelayUp(0) ? SIGNAL_YELLOW : SIGNAL_RED;
}

CADSignal::SignalColor CADSignal::getSignalColor_5(CADSignal* pSignal)
{
	/*return pSignal->isRelayUp(1) ?
		(pSignal->isRelayUp(0) ? SIGNAL_GREEN : SIGNAL_RED) : SIGNAL_DS;*/
	return pSignal->isRelayUp(0) ? SIGNAL_GREEN : SIGNAL_RED;
}


static int getRelayFlag(vector<Relay*>& vRelay, int nRelay) 
{
	int nFlag = 0;

	for (int i = 0; i < nRelay; i++)
	{
		nFlag |= vRelay[i]->isUp() << (nRelay - 1 - i);
	}

	return nFlag;
}


CADSignal::SignalColor CADSignal::getSignalColor_6(CADSignal* pSignal)
{
	static SignalColor defColor[] = {
		SIGNAL_RED, SIGNAL_WHITE, /*SIGNAL_GREEN*/SIGNAL_YELLOW,
	};

	/*return pSignal->isRelayUp(2) ?
		defColor[getRelayFlag(pSignal->vRelay_, 2)] : SIGNAL_DS;*/

	return defColor[getRelayFlag(pSignal->vRelay_, 2)];
}


static CADSignal::SignalColor check2DS(CADSignal::SignalColor color, Relay* p2DSRelay)
{
	return (color >= CADSignal::SIGNAL_RED_YELLOW && p2DSRelay->isDown()) ? CADSignal::SIGNAL_2DS : color;
}

CADSignal::SignalColor CADSignal::getSignalColor_7(CADSignal* pSignal)
{
	static SignalColor defColor[] = {
		SIGNAL_RED, SIGNAL_RED_YELLOW, SIGNAL_GREEN,
	};
	/*return pSignal->isRelayUp(2) ?
		check2DS(defColor[getRelayFlag(pSignal->vRelay_, 2)], pSignal->vRelay_[3]) : SIGNAL_DS;*/
	return defColor[getRelayFlag(pSignal->vRelay_, 2)];
}

CADSignal::SignalColor CADSignal::getSignalColor_8(CADSignal* pSignal)
{
	static SignalColor defColor[] = {
		SIGNAL_RED, SIGNAL_RED_YELLOW, SIGNAL_YELLOW,
	};

	/*return pSignal->isRelayUp(2) ?
		check2DS(defColor[getRelayFlag(pSignal->vRelay_, 2)], pSignal->vRelay_[3]) : SIGNAL_DS;*/
	return defColor[getRelayFlag(pSignal->vRelay_, 2)];
}

CADSignal::SignalColor CADSignal::getSignalColor_9(CADSignal* pSignal)
{
	static SignalColor defColor[] = {
		SIGNAL_RED, SIGNAL_RED_WHITE, SIGNAL_YELLOW,
	};

	/*return pSignal->isRelayUp(2) ?
		check2DS(defColor[getRelayFlag(pSignal->vRelay_, 2)], pSignal->vRelay_[3]) : SIGNAL_DS;*/
	return defColor[getRelayFlag(pSignal->vRelay_, 2)];
}

CADSignal::SignalColor CADSignal::getSignalColor_10(CADSignal* pSignal)
{
	static SignalColor defColor[] = {
		SIGNAL_RED, SIGNAL_WHITE, SIGNAL_RED, SIGNAL_WHITE, 
		SIGNAL_DOUBLE_GREEN, SIGNAL_DOUBLE_GREEN, SIGNAL_GREEN, SIGNAL_GREEN
	};

	/*return pSignal->isRelayUp(3) ?
		check2DS(defColor[getRelayFlag(pSignal->vRelay_, 3)], pSignal->vRelay_[4]) : SIGNAL_DS;*/
	return defColor[getRelayFlag(pSignal->vRelay_, 3)];
}

CADSignal::SignalColor CADSignal::getSignalColor_11(CADSignal* pSignal)
{
	static SignalColor defColor[] = {
		SIGNAL_RED, SIGNAL_RED_YELLOW, SIGNAL_RED, SIGNAL_RED_YELLOW,
		SIGNAL_YELLOW, SIGNAL_RED_YELLOW, SIGNAL_GREEN, SIGNAL_RED_YELLOW
	};

	/*return pSignal->isRelayUp(3) ?
		check2DS(defColor[getRelayFlag(pSignal->vRelay_, 3)], pSignal->vRelay_[4]) : SIGNAL_DS;*/
	return defColor[getRelayFlag(pSignal->vRelay_, 3)];
}

CADSignal::SignalColor CADSignal::getSignalColor_12(CADSignal* pSignal)
{
	SignalColor color;

	if (pSignal->isRelayDown(0))
	{
		color = pSignal->isRelayUp(3) ? SIGNAL_RED_WHITE : SIGNAL_RED;
	}
	else
	{
		if (pSignal->isRelayDown(1))
		{
			color = SIGNAL_DOUBLE_YELLOW;
		}
		else
		{
			color = pSignal->isRelayUp(2) ? SIGNAL_GREEN : SIGNAL_YELLOW;
		}
	}

	//return pSignal->isRelayUp(4) ? check2DS(color, pSignal->vRelay_[5]) : SIGNAL_DS;
	return color;
}

CADSignal::SignalColor CADSignal::getSignalColor_13(CADSignal* pSignal)
{
	SignalColor color;

	if (pSignal->isRelayDown(0))
	{
		color = pSignal->isRelayUp(4) ? SIGNAL_RED_WHITE : SIGNAL_RED;
	}
	else
	{
		if (pSignal->isRelayDown(1))
		{
			color = SIGNAL_DOUBLE_YELLOW;
		}
		else
		{
			if (pSignal->isRelayUp(2))
			{
				color = SIGNAL_GREEN;
			}
			else
			{
				color = pSignal->isRelayUp(3) ? SIGNAL_GREEN_YELLOW : SIGNAL_YELLOW;
			}
		}
	}

	//return pSignal->isRelayUp(5) ? check2DS(color, pSignal->vRelay_[6]) : SIGNAL_DS;
	return color;
}

CADSignal::SignalColor CADSignal::getSignalColor_14(CADSignal* pSignal)
{
	return SIGNAL_RED;
}

CADSignal::SignalColor CADSignal::getSignalColor_15(CADSignal* pSignal)
{
	return SIGNAL_RED;
}

CADSignal::SignalColor CADSignal::getSignalColor_16(CADSignal* pSignal)
{
	SignalColor color;

	if (pSignal->isRelayUp(0))
	{
		color = pSignal->isRelayUp(1) ?
			(pSignal->isRelayUp(3) ? SIGNAL_GREEN_YELLOW : SIGNAL_GREEN) : SIGNAL_YELLOW;
	}
	else
	{
		color = pSignal->isRelayUp(2) ? SIGNAL_WHITE : SIGNAL_RED;
	}

	//return pSignal->isRelayUp(4) ? check2DS(color, pSignal->vRelay_[5]) : SIGNAL_DS;
	return color;
}

CADSignal::SignalColor CADSignal::getSignalColor_17(CADSignal* pSignal)
{
	return SIGNAL_CYAN;
}

CADSignal::SignalColor CADSignal::getSignalColor_18(CADSignal* pSignal)
{
	return SIGNAL_RED;
}

CADSignal::SignalColor CADSignal::getSignalState(void)
{
	
	SignalColor color = SIGNAL_RED;
	if (name_.text_.Right(1) != L'F')
	{
		//获取灯色
		color = getColor();

		//检查灯丝
		if (color >= SIGNAL_RED && color <= SIGNAL_WHITE)
		{
			if (getRelayState(L"DJ")==false)
			{
				color = SIGNAL_DS;
			}
		}
		else if (color >= SIGNAL_RED_YELLOW && color <= SIGNAL_DOUBLE_GREEN)
		{
			if (getRelayState(L"2DJ") == false)
			{
				color = SIGNAL_2DS;
			}
		}
		//闪烁
		if (color == SIGNAL_DS || color == SIGNAL_2DS)
		{
			m_bFlash = TRUE;
		}
		else
		{
			m_bFlash = FALSE;
		}
	}

	return color;
}

CADSignal::SignalColor CADSignal::getColor(void)
{

	typedef SignalColor(*GetSignalColor)(CADSignal* pSignal);
	static GetSignalColor getSignalColorFunc[18] = {
		getSignalColor_1, getSignalColor_2, getSignalColor_3, getSignalColor_4, getSignalColor_5,
		getSignalColor_6, getSignalColor_7, getSignalColor_8, getSignalColor_9, getSignalColor_10, getSignalColor_11,
		getSignalColor_12, getSignalColor_13, getSignalColor_14, getSignalColor_15, getSignalColor_16,
		getSignalColor_17, getSignalColor_18
	};

	SignalColor color = SIGNAL_RED;
	if (name_.text_.Right(1) != L'F')
	{
		//获取灯色
		color = getSignalColorFunc[ioType_](this);
	}

	return color;
}

void CADSignal::OnPaint()
{
	CPaintDC dc(this); 

	// 绘制横竖线
	dc.SelectObject(LamppostPen());
	dc.MoveTo(data_.hLine_[0]);
	dc.LineTo(data_.hLine_[1]);
	dc.MoveTo(data_.vLine_[0]);
	dc.LineTo(data_.vLine_[1]);
	
	static CBrush* brush[13][2] = {
		{ RedBrush(),		BlackBrush() },	//SIGNAL_RED, 
		{ GreenBrush(),		BlackBrush() },	//SIGNAL_GREEN, 
		{ YellowBrush(),	BlackBrush() },	//SIGNAL_YELLOW, 
		{ BlueBrush(),		BlackBrush() },	//SIGNAL_BLUE, 
		{ WhiteBrush(),		BlackBrush() },	//SIGNAL_WHITE,

		{ RedBrush(),		YellowBrush()},		//SIGNAL_RED_YELLOW, 
		{ RedBrush(),		WhiteBrush() },		//SIGNAL_RED_WHITE, 
		{ GreenBrush(),		YellowBrush() },	//SIGNAL_GREEN_YELLOW, 
		{ YellowBrush(),	YellowBrush() },	//SIGNAL_DOUBLE_YELLOW, 
		{ GreenBrush(),		GreenBrush() },		//SIGNAL_DOUBLE_GREEN,

		{ RedBrush()/*CyanBrush()*/, BlackBrush() },	//SIGNAL_DS, 
		{ RedBrush(),		CyanBrush() },	//SIGNAL_2DS
		{ CyanBrush(),		BlackBrush()}
	};

	dc.SelectStockObject(WHITE_PEN);

	SignalColor color = SIGNAL_RED;
	if (name_.text_.Right(1) != L'F')
		 color = getSignalState();
	
	BOOL bFlashFlag = m_nflashCount % 2 != 0;
	if (color == SIGNAL_DS && getDDJstate() == false)
	{
		if (bFlashFlag)
		{
			dc.SelectObject(BlackBrush());
		}
		else
		{
			dc.SelectObject(brush[color][0]);
		}
	}
	else
	{
		dc.SelectObject(brush[color][0]);
	}
	dc.Ellipse(data_.lightRect_[0]);

	if (data_.nLight_ > 1)
	{
		if (color == SIGNAL_2DS && bFlashFlag)
		{
			dc.SelectObject(BlackBrush());
		}
		else
		{
			dc.SelectObject(brush[color][1]);
		}
		
		dc.Ellipse(data_.lightRect_[1]);
	}
	if (getDDJstate())
	{
		dc.SelectObject(LamppostPen());
		CRect tempRect = data_.lightRect_[0];
		tempRect.InflateRect(-3, -3);
		dc.MoveTo(tempRect.left, tempRect.top);
		dc.LineTo(tempRect.right, tempRect.bottom);
		dc.MoveTo(tempRect.left, tempRect.bottom);
		dc.LineTo(tempRect.right, tempRect.top);
	}

}


void CADSignal::compressWindow()
{
	CPoint wndPt[4];
	setCloseup(wndPt, data_.nLight_ == 1 ? 2 * r + 2 : 4 * r + 2, true);
	wndPt[0].Offset((int)(2 * sin(data_.angle_)), -(int)(2 * cos(data_.angle_)));
	wndPt[1].Offset(-(int)(2 * sin(data_.angle_)), (int)(2 * cos(data_.angle_)));
	wndPt[2].Offset(-(int)(2 * sin(data_.angle_)), (int)(2 * cos(data_.angle_)));
	wndPt[3].Offset((int)(2 * sin(data_.angle_)), -(int)(2 * cos(data_.angle_)));//要扩大范围，否则封锁框显示不出来
	setCRgnWindow(wndPt, 4);
}

int CADSignal::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CADDevice::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (data_.shuntType_ == SignalData::TRAIN_SHUNT_BOTH || data_.shuntType_ == SignalData::CHANGE_ROUTE_SIGNAL)
	{
		trainButton_.createEmptyWnd(GetParent());
		trainButton_.MoveWindow(btnRect_);
		trainButton_.pSignal_ = this;
	}

	return 0;
}


BOOL CADSignal::setPreCondition(CString& operation)
{
	// 设置灯丝断丝和2灯丝断丝
	if (operation.Compare(L"灯丝") == 0)
	{
		if (ioType_ < 6)
		{
			// 最后一个继电器是灯丝
			vRelay_[vRelay_.size() - 2]->setStat(FALSE, TRUE);
		}
		else
		{
			// 倒数第二个继电器是灯丝
			vRelay_[vRelay_.size() - 3]->setStat(FALSE, TRUE);
		}
	}
	else if (operation.Compare(L"2灯丝") == 0)
	{
		vRelay_[vRelay_.size() - 2]->setStat(FALSE, TRUE);
	}
	else
	{
		// 没有找到对应的灯色
		CString strErr(L"没有预设的设备状态：");
		strErr.Append(name_.text_);
		strErr.Append(operation);
		AfxGetMainWnd()->SendMessage(WM_OUT_MESSAGE, 1, (LPARAM)&strErr);
		return FALSE;
	}

	return TRUE;
}


BOOL CADSignal::checkResult(CString& result)
{
	static array<CString, 13> defResult = {
		L"红", L"绿", L"黄", L"蓝", L"白", 
		L"红绿", L"红黄", L"红白", L"绿黄", L"双黄", L"双绿",
		L"灯丝", L"2灯丝",
	};

	for (size_t i = 0; i < 13; i++)
	{
		if (defResult[i].Compare(result) == 0)
		{
			return (i == getSignalState());
		}
	}

	// 没有找到对应的灯色
	CString strErr(L"没有预期的设备结果：");
	strErr.Append(name_.text_);
	strErr.Append(result);
	AfxGetMainWnd()->SendMessage(WM_OUT_MESSAGE, 1, (LPARAM)&strErr);
	return FALSE;
}


void CADSignal::setInputRelay(void)
{
	if (isNeed2DJUp())
	{
		DriveRelayState(L"2DJ", true);
	}
	else
	{
		DriveRelayState(L"2DJ", false);
	}


	if (isNeedDJDown())
	{
		DriveRelayState(L"DJ", false);
	}
	else
	{
		DriveRelayState(L"DJ", true);
	}
}

void CADSignal::setCloseup(CPoint pt[], int height, bool bWnd)
{
	if ((data_.type_ % 4) < 2)
	{
		height += H_LEN;
	}
	bool isUp = data_.type_ < 4;
	pt[0] = data_.vLine_[0];
	pt[1] = data_.vLine_[1];
	pt[2] = data_.vLine_[1];
	pt[3] = data_.vLine_[0];

	pt[2].Offset((int)(isUp ? (-height - 1)*cos(data_.angle_) : height*cos(data_.angle_)), (int)(isUp ? (-height - 1)*sin(data_.angle_) : height*sin(data_.angle_)));
	pt[3].Offset((int)(isUp ? (-height - 1)*cos(data_.angle_) : height*cos(data_.angle_)), (int)(isUp ? (-height - 1)*sin(data_.angle_) : height*sin(data_.angle_)));

	if (!(data_.type_ < 4))
	{
		pt[0].x -= 1;
		pt[1].x -= 1;
	}
}

bool CADSignal::getDDJstate()
{
	bool bDDJStatus = getRelayState(L"DDJ");

	return bDDJStatus;
	
}

void  CADSignal::SetDDJstate(bool bDDJStatus)
{
	SetRelayState(L"DDJ", bDDJStatus);
}

void  CADSignal::SetDsState(bool bDsStatus)
{
	SetRelayState(L"DJ", bDsStatus);
}

void CADSignal::setZcCommRelayAllSignalLightMode(BOOL bLightOff)
{
	for (int i = 0; i < vZcCommRelay_.size(); i++)
	{
		if (vZcCommRelay_[i]->name_ == L"信号灭灯")
		{
			vZcCommRelay_[i]->setStat(bLightOff, true);
			break;
		}
	}
}

bool CADSignal::checkChange()
{
	if (name_.text_.Right(1) != L'F')
	{
		if (m_blastSendAtpDDState != getDDJstate())
		{
			m_blastSendAtpDDState = getDDJstate();
			return true;
		}

		if (m_lastSendAtpColor != getSignalState())
		{
			m_lastSendAtpColor = getSignalState();
			return true;
		}
	}
	

	return false;
}

bool CADSignal::isNeed2DJUp()
{
	if (getColor() >= SIGNAL_RED_YELLOW && getColor() <= SIGNAL_DOUBLE_GREEN)
	{
		return true;
	}

	return false;
}

//一灯丝继电器是否需要落下
bool CADSignal::isNeedDJDown()
{
	if (getDDJstate())
	{
		return true;
	}

	return false;
}