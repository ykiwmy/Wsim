#include "stdafx.h"
#include "CADDeLight.h"
#include "../Relay.h"
#include "ColorDef.h"
CADDeLight::CADDeLight(void)
: m_blastSendAtpState(false)
{
}


CADDeLight::~CADDeLight(void)
{
}


IMPLEMENT_SERIAL(CADDeLight, CADDevice, 1)


void CADDeLight::Serialize(CArchive& ar)
{
	CADDevice::Serialize(ar);

	if (!ar.IsStoring())
	{	// loading code
		for (int i = 0; i < 3; i++)
		{
			ar >> color_[i];
		}
		ar >> lightRect_ ;
	}
}


BEGIN_MESSAGE_MAP(CADDeLight, CADDevice)
	ON_WM_PAINT()
END_MESSAGE_MAP()


void CADDeLight::OnPaint()
{
	CPaintDC dc(this); 
	COLORREF BrushColor;
	if (vRelay_.size()>0)
	{
		BrushColor = isRelayDown(0) ? color_[0] : color_[1];

		if (vRelay_.size()==2)//SDJ 目前只有DRI有两个继电器
		{
			BrushColor = isWDState() ? color_[1] : color_[0];  //稳灯

			if (isSDState()) // 闪灯
			{
				m_bFlash = TRUE;
				BOOL bFlashFlag = m_nflashCount % 2 != 0;
				BrushColor = bFlashFlag ? COLOR_BLACK : color_[1];
			}
			else
			{
				m_bFlash = FALSE;
			}
		}

		CBrush lightBrush(BrushColor);
		CPen lightPen(PS_SOLID, 1, COLOR_WHITE);
		CPen *pOldPen=dc.SelectObject(&lightPen);
		CBrush *pOldBrushDc = dc.SelectObject(&lightBrush);
		dc.Ellipse(lightRect_);

		dc.SelectObject(pOldPen);
		dc.SelectObject(pOldBrushDc);
		lightPen.DeleteObject();
		pOldBrushDc->DeleteObject();
	}
	else
	{
		BrushColor = color_[0];
	}
	
}
bool CADDeLight::isPressed()
{
	if (vRelay_.size()>0)
	{
		return isRelayDown(0);
	}
	return false;
}


bool CADDeLight::checkChange()
{
	if (m_blastSendAtpState!=isPressed())
	{
		m_blastSendAtpState = isPressed();
		return true;
	}
	return false;
}

BOOL CADDeLight::isSDState()
{
	for (int i = 0; i < vRelay_.size(); i++)
	{
		Relay *pRely = vRelay_.at(i);
		CString str = pRely->name_;
		str.MakeLower();
		if (str.Find(L"sd")>0)
		{
			return isRelayUp(i);
		}
	}
	return FALSE;
}

BOOL CADDeLight::isWDState()
{
	for (int i = 0; i < vRelay_.size(); i++)
	{
		Relay *pRely = vRelay_.at(i);
		CString str = pRely->name_;
		str.MakeLower();
		if (str.Find(L"wd")>0)
		{
			int ni = isRelayUp(i);
			return isRelayUp(i);
		}
	}
	return FALSE;
}

//自动折返灯
BOOL CADDeLight::isDri()
{
	if (vRelay_.size() == 2 && name_.text_.Left(3) == L"DRI")
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CADDeLight::isDrb()
{
	if (name_.text_.Left(3) == L"DRB")
	{
		return TRUE;
	}
	return FALSE;
}
