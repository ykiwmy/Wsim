#include "stdafx.h"
#include "CADButton.h"
#include "ColorDef.h"
#include "CADSignal.h"
#include "../Relay.h"

CADButton::CADButton(void)
: pSignal_(NULL)
, m_blastSendAtpState(true)
{
	
}


CADButton::~CADButton(void)
{
}


IMPLEMENT_SERIAL(CADButton, CADDevice, 1)


void CADButton::Serialize(CArchive& ar)
{
	CADDevice::Serialize(ar);

	if (!ar.IsStoring())
	{	// loading code
		ar.Read(&data_, sizeof(ButtonData));
	}
}


BEGIN_MESSAGE_MAP(CADButton, CADDevice)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


void CADButton::OnPaint()
{
	CPaintDC dc(this);
	COLORREF BrushColor;
	if (vRelay_.size()>0)
	{
		BrushColor = isRelayDown(0) ? data_.bgColor_[1] : data_.bgColor_[0];
	}
	else
	{
		BrushColor=data_.bgColor_[0];
	}
	CBrush btnBrush(BrushColor);
	CBrush *pOldBrushDc=dc.SelectObject(&btnBrush);
	if (data_.btnType_ != ROUND_BUTTON)
	{
		dc.Rectangle(data_.btnRect_);
		CRect rect = data_.btnRect_;
		rect.InflateRect(-1, -1);
		dc.Draw3dRect(rect, GetSysColor(COLOR_BTNHIGHLIGHT), GetSysColor(COLOR_BTNSHADOW));
	}
	else
	{
		dc.Ellipse(data_.btnRect_);
	}

	dc.SelectObject(pOldBrushDc);
	btnBrush.DeleteObject();
}


void CADButton::setBtnSize(const CPoint& topLeft, const CSize& sz)
{
	data_.btnRect_.right = sz.cx;
	data_.btnRect_.bottom = sz.cy;
	CRect rect(topLeft, topLeft + sz);
	MoveWindow(rect);
}


int CADButton::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CADDevice::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (data_.btnType_ == PUSH_BUTTON)
	{
		name_.SetParent(this);
	}

	return 0;
}

CString CADButton::getDeviceName(void)
{
	return pSignal_ ? pSignal_->name_.text_ : name_.text_; 
}

bool CADButton::isPressed()
{
	if (vRelay_.size()>0)
	{
		return isRelayDown(0);
	}
	return false;
}

bool CADButton::chackChange()
{
	if (m_blastSendAtpState!=isPressed())
	{
		m_blastSendAtpState = isPressed();
		return true;
	}
	return false;
}