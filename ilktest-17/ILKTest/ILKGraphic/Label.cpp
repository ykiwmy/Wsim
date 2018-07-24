#include "stdafx.h"
#include "Label.h"
#include "ColorDef.h"
#include "CADDevice.h"
#include "CADDeLight.h"

Label::Label(void)
	: text_(L"NO")
	, pDev_(NULL)
{
	data_.textColor_ = COLOR_WHITE;
	data_.bgColor_ = COLOR_BLACK;
	font_.CreateStockObject(DEFAULT_GUI_FONT);
	font_.GetLogFont(&data_.logFont_);
}


Label::~Label(void)
{
}


IMPLEMENT_SERIAL(Label, CADGraphic, 1)


void Label::Serialize(CArchive& ar)
{
	if (!ar.IsStoring())
	{	
		ar.Read(&data_, sizeof(data_));
		ar >> text_;
		if (font_.GetSafeHandle() != 0)
		{
			font_.DeleteObject();
		}
		font_.CreateFontIndirect(&data_.logFont_);
	}
}


BEGIN_MESSAGE_MAP(Label, CADGraphic)
	ON_WM_PAINT()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


void Label::OnPaint()
{
	CPaintDC dc(this); 
	if (pDev_!=NULL)
	{
		if (pDev_->IsKindOf(RUNTIME_CLASS(CADDeLight)))
		{
			if (pDev_->vRelay_.size()>0)
			{
				dc.SetBkColor(data_.bgColor_); // ±³¾°
				dc.SelectObject(&font_); // ×ÖÌå
				dc.SetTextColor(data_.textColor_); // Ç°¾°
				dc.TextOutW(0, 0, text_);
			}
		}
		else
		{
			dc.SetBkColor(data_.bgColor_); // ±³¾°
			dc.SelectObject(&font_); // ×ÖÌå
			dc.SetTextColor(data_.textColor_); // Ç°¾°
			dc.TextOutW(0, 0, text_);
		}
	}
	else
	{
		dc.SetBkColor(data_.bgColor_); // ±³¾°
		dc.SelectObject(&font_); // ×ÖÌå
		dc.SetTextColor(data_.textColor_); // Ç°¾°
		dc.TextOutW(0, 0, text_);
	}
	
}




void Label::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (pDev_)
	{
		pDev_->PostMessageW(WM_LBUTTONUP);
	}

	CADGraphic::OnLButtonUp(nFlags, point);
}


void Label::setTextColor(COLORREF color)
{
	data_.textColor_ = color;
	Invalidate(FALSE);
}


void Label::setBackgroudColor(COLORREF color)
{
	data_.bgColor_ = color;
	Invalidate(FALSE);
}


BOOL Label::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		afxGlobalData.GetHandCursor(), (HBRUSH)GetStockObject(BLACK_BRUSH), 0);

	return CWnd::PreCreateWindow(cs);
}
