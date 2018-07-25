#include "stdafx.h"
#include "CADDevice.h"
#include "ColorDef.h"
#include "../resource.h"
#include "../Relay.h"
#include "ZcCommRelay.h"
#include <math.h>
#include "ILKTestDoc.h"
#include "MainFrm.h"

int CADDevice::SectionWidth_ = 2;
int CADDevice::InsuHeight_ = 6;

CADDevice::CADDevice(void)\
: bStatChanged_(FALSE)
, eleType(_T(""))
, eleIndex(_T(""))
, m_bFlash(FALSE)
, m_nflashCount(0)
, devId_3M(0)
, devType_3M(0)
{
	statusChanged = false;
}


CADDevice::~CADDevice(void)
{
	
}


IMPLEMENT_SERIAL(CADDevice, CADGraphic, 1)


void CADDevice::Serialize(CArchive& ar)
{
	name_.Serialize(ar);

	if (!ar.IsStoring())
	{
		ar >> nameRect_;
		ar.Read(&io_, sizeof(DeviceInput));
	}
	
}


void CADDevice::drawSection(CPaintDC& dc, CPoint pt[], int nPt, BOOL bOccupied)
{
	CPen* poldPen=dc.SelectObject(bOccupied ? SectionRedPen() : SectionPen());

	dc.MoveTo(pt[0]);
	for (int i = 1; i < nPt; i++)
		dc.LineTo(pt[i]);
	//dc.SelectObject(poldPen);
}

void CADDevice::drawLogicSection(CPaintDC& dc, CPoint pt[], int nPt, BOOL bAT, BOOL bUT)
{
	if (bAT && !bUT)
	{
		dc.SelectObject(LogicATPen());
		dc.MoveTo(pt[0]);
		for (int i = 1; i < nPt; i++)
			dc.LineTo(pt[i]);
	}
	else if (!bAT && bUT)
	{
		dc.SelectObject(LogicUTPen());
		dc.MoveTo(pt[0]);
		for (int i = 1; i < nPt; i++)
			dc.LineTo(pt[i]);
	}
	else
	{
	}
}



CPoint CADDevice::getCrossPoint(CPoint& line1Pt1, CPoint& line1Pt2, CPoint& line2Pt1, CPoint& line2Pt2)
{
	if (line1Pt1 == line1Pt2)
		return TwoPtDistance(line2Pt1, line1Pt1) < TwoPtDistance(line2Pt2, line1Pt1) ? line2Pt1 : line2Pt2;
	if (line2Pt1 == line2Pt2)
		return TwoPtDistance(line1Pt1, line2Pt1) < TwoPtDistance(line1Pt2, line2Pt1) ? line1Pt1 : line1Pt2;

	double a1 = (double)(line1Pt2.y - line1Pt1.y) / (double)(line1Pt2.x - line1Pt1.x);
	double b1 = line1Pt1.y - a1 * (double)line1Pt1.x;

	double a2 = (double)(line2Pt2.y - line2Pt1.y) / (double)(line2Pt2.x - line2Pt1.x);
	double b2 = line2Pt1.y - a2 * (double)line2Pt1.x;

#define MIN_DOUBLE_DIFF 0.000001
	if (abs(a1 - a2) < MIN_DOUBLE_DIFF)
	{
		if (abs(b1 - b2) < MIN_DOUBLE_DIFF)
		{
			int ptDiff[4] = { 
				TwoPtDistance(line1Pt1, line2Pt1),
				TwoPtDistance(line1Pt2, line2Pt1),
				TwoPtDistance(line1Pt1, line2Pt2),
				TwoPtDistance(line1Pt2, line2Pt2),
			};
			int minDiffFlag = 0;
			for (int i = 0; i < 3; i++)
				minDiffFlag = ptDiff[i] < ptDiff[i + 1] ? i : i + 1;
			switch (minDiffFlag)
			{
			case 0:
			case 2:
				return line1Pt1;
			case 1:
			case 3:
				return line1Pt2;
			default:
				return CPoint();
			}
		}
		else 
			return CPoint();
	}
	else
	{
		double x = (b2 - b1) / (a1 - a2);
		double y = a1 * x + b1;
		return CPoint((int)x, (int)y);
	}
}


void CADDevice::drawInsu(CPaintDC& dc, CPoint pt[], int type[], int nInsu)
{

	static int a = 0;
	a++;
	if (a == 3 || a == 4)
	{
		a = 3;
	}

	for (int i = 0; i < nInsu; i++)
	{
		CPoint* pPt[2] = { pt + 2 * i, pt + 2 * i + 1 };
		if (type[i] == NORMAL_INSU)
		{
			dc.MoveTo(*pPt[0]);
			dc.LineTo(*pPt[1]);
		}
		else if (type[i] == LIMIT_INSU)
		{
			CPoint center((pPt[0]->x + pPt[1]->x) / 2, (pPt[0]->y + pPt[1]->y) / 2);
			int r = InsuHeight_ / 2;
			CRect circleRect(center, center);
			circleRect.InflateRect(r, r);
			dc.Ellipse(circleRect);
		}
	}
}

BEGIN_MESSAGE_MAP(CADDevice, CADGraphic)
	ON_WM_CREATE()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()


int CADDevice::TwoPtDistance(CPoint& pt1, CPoint& pt2)
{
	double diffX = pt1.x - pt2.x;
	double diffY = pt1.y - pt2.y;

	return (int)sqrt(diffX * diffX + diffY * diffY);
}


int CADDevice::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CADGraphic::OnCreate(lpCreateStruct) == -1)
		return -1;

	name_.Create(NULL, NULL, WS_CHILD | WS_VISIBLE, nameRect_, GetParent(), 0);

	return 0;
}


void CADDevice::OnLButtonUp(UINT nFlags, CPoint point)
{
	for (size_t i = 0; i < vRelay_.size(); i++)
	{
		pMenu_->CheckMenuItem(RELAY_MENU_0 + i, vRelay_[i]->isUp() ? MF_CHECKED : MF_UNCHECKED);
	}

	ClientToScreen(&point);
	pMenu_->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
}

void CADDevice::OnRButtonUp(UINT nFlags, CPoint point)
{
	CILKTestDoc* pDoc = (CILKTestDoc*)dynamic_cast<CMainFrame*>(AfxGetMainWnd())->GetActiveDocument();
	if (pDoc->m_iIsCnctToSamZC == 1)
	{
		for (size_t i = 0; i < vZcCommRelay_.size(); i++)
		{
			pRMenu_->CheckMenuItem(ZC_COMM_MENU_0 + i, vZcCommRelay_[i]->isHigh() ? MF_CHECKED : MF_UNCHECKED);
		}

		ClientToScreen(&point);
		pRMenu_->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}
}


int CADDevice::getBits(UINT16 src, int bitFlag, int nBits)
{
	src = src << (16 - bitFlag - nBits);
	return src >> (16 - nBits);
}


BOOL CADDevice::checkStatus(void* lastStatus, void* status, int statusSize)
{
	UINT8* pLast = static_cast<UINT8*>(lastStatus);
	UINT8* pCur = static_cast<UINT8*>(status);

	BOOL bCompare = TRUE;
	int i = 0;

	// 比较是否相等，如果不相等就覆盖。
	while (i < statusSize)
	{
		if (*pLast != *pCur)
		{
			bCompare = FALSE;
			*pLast = *pCur;
		}
		i++;
	}

	return bCompare;
}


BOOL CADDevice::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		afxGlobalData.GetHandCursor(), (HBRUSH)GetStockObject(BLACK_BRUSH), 0);

	return CWnd::PreCreateWindow(cs);
}

void  CADDevice::SetRelayState(CString relayName,bool bStatus)
{
	for (int relaynumb = 0; relaynumb < vRelay_.size(); relaynumb++)
	{
		if (vRelay_[relaynumb]->name_.Compare(relayName)==0)
		{
			if (bStatus)
			{
				vRelay_[relaynumb]->setStat(true, true);
			}
			else
			{
				vRelay_[relaynumb]->setStat(false, true);
			}
			break;
		}
	}
}

void  CADDevice::DriveRelayState(CString relayName, bool bStatus)
{
	CILKTestDoc* pDoc = (CILKTestDoc*)dynamic_cast<CMainFrame*>(AfxGetMainWnd())->GetActiveDocument();
	for (int relaynumb = 0; relaynumb < vRelay_.size(); relaynumb++)
	{
		if (vRelay_[relaynumb]->name_.Compare(relayName) == 0)
		{
			if (bStatus)
			{
				vRelay_[relaynumb]->driveRelay(true, pDoc->m_nUpTime);
			}
			else
			{
				vRelay_[relaynumb]->driveRelay(false, pDoc->m_nDownTime);
			}
			break;
		}
	}
}
bool CADDevice::getRelayState(CString relayName)
{
	bool bStatus = false;
	for (int relaynumb = 0; relaynumb < vRelay_.size(); relaynumb++)
	{
		if (vRelay_[relaynumb]->name_.Compare(relayName)==0)
		{
			if (vRelay_[relaynumb]->isUp())
			{
				bStatus = true;
			}
			else
			{
				bStatus = false;
			}
			break;
		}
	}

	return bStatus;

}
