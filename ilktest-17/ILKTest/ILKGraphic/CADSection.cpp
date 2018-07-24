#include "stdafx.h"
#include "CADSection.h"
#include "ColorDef.h"
#include "DevBorder.h"
#include "../Relay.h"
#include "ZcCommRelay.h"
#include "ILKTestDoc.h"
#include "MainFrm.h"
#include "ChildView.h"
#include "SplitterFrame.h"
IMPLEMENT_SERIAL(CADSection, CADDevice, 1)


void CADSection::Serialize(CArchive& ar)
{
	CADDevice::Serialize(ar);

	if (!ar.IsStoring())
	{	// loading code
		ar.Read(&data_, sizeof(data_));
		ar.Read(&logicDeviceIO_, sizeof(DeviceIO));//add by lwm 2016-12-13  

		drawRealSectionName();
	}
}


BEGIN_MESSAGE_MAP(CADSection, CADDevice)
	ON_WM_PAINT()
END_MESSAGE_MAP()


void CADSection::setTvsState(CPaintDC& pDC)
{
	CILKTestDoc* pDoc = (CILKTestDoc*)dynamic_cast<CMainFrame*>(AfxGetMainWnd())->GetActiveDocument();
	if (pDoc->m_isLogicSection == 1 /*&& name_.text_.Right(1) != L"A"*/)
	{
		//drawSection(dc, data_.pt_, 4, pDoc->findDeviceByName(io_.getName().Left(io_.getName().GetLength() -1) + L"A")->isRelayDown(0));//逻辑区段不为A的不查找继电器

		CADDevice *pDev = pDoc->findDeviceByName(logicDeviceIO_.getName().Left(logicDeviceIO_.getName().GetLength() - 1) + L"A");
		if (pDev != NULL)
		{
			drawSection(pDC, data_.pt_, 4, pDev->isRelayDown(0));//逻辑区段不为A的不查找继电器
		}
		else
		{
			drawSection(pDC, data_.pt_, 4, 0);
		}
	}
	else
	{
		drawSection(pDC, data_.pt_, 4, isRelayDown(0));
	}
}
void CADSection::setLogicState(CPaintDC& pDC)
{
	CILKTestDoc* pDoc = (CILKTestDoc*)dynamic_cast<CMainFrame*>(AfxGetMainWnd())->GetActiveDocument();
	if (pDoc->m_isLogicSection == 1)
	{
		CADDevice *pDev = pDoc->findLogicDeviceByName(logicDeviceIO_.addr_, logicDeviceIO_.getName(), RUNTIME_CLASS(CADSection));
		if (pDev != NULL)
		{
			drawLogicSection(pDC, data_.pt_, 4, pDev->isATRelay(1), pDev->isUTRelay(1));
		}
		else
		{
			drawSection(pDC, data_.pt_, 4, 0);
		}
	}
	else
	{
	}
}

void CADSection::OnPaint()
{
	CPaintDC dc(this);
	
	if (isFSSection())//复示区段不查找继电器
	{
		drawSection(dc, data_.pt_, 4, 0);
	}
	else 
	{
		setTvsState(dc);
		setLogicState(dc);
	}
	
	drawInsu(dc, data_.insuPt_, data_.insuType_, 2);
}


void CADSection::compressWindow()
{
	CPoint wndPt[8];
	setSealFrame(wndPt, InsuHeight_ + 3, true);
	setCRgnWindow(wndPt, 8);
}


void CADSection::setSealFrame(CPoint pt[], int height, bool bWnd)
{
	DevBorder border[3] = { 
		DevBorder(data_.pt_[0], data_.pt_[1], height),
		DevBorder(data_.pt_[1], data_.pt_[2], height),
		DevBorder(data_.pt_[2], data_.pt_[3], height),
	};

	pt[0] = border[0].pt(0);
	pt[1] = getCrossPoint(border[0].pt(0), border[0].pt(3), border[1].pt(0), border[1].pt(3));
	pt[2] = getCrossPoint(border[1].pt(0), border[1].pt(3), border[2].pt(0), border[2].pt(3));
	pt[3] = border[2].pt(3);
	pt[4] = border[2].pt(2);
	pt[5] = getCrossPoint(border[2].pt(1), border[2].pt(2), border[1].pt(1), border[1].pt(2));
	pt[6] = getCrossPoint(border[1].pt(1), border[1].pt(2), border[0].pt(1), border[0].pt(2));
	pt[7] = border[0].pt(1);
}


BOOL CADSection::setPreCondition(CString& operation)
{
	if (operation.Compare(L"占用") == 0)
	{
		vRelay_[0]->setStat(FALSE, TRUE);
	}
	else if (operation.Compare(L"空闲") == 0)
	{
		vRelay_[0]->setStat(TRUE, TRUE);
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}


BOOL CADSection::checkResult(CString& result)
{
	return isRelayUp(0) ? result.Compare(L"空闲") == 0 : result.Compare(L"占用") == 0;
}

void CADSection::setSectionState(bool isOcuppy)
{
	SetRelayState(L"WGJ", !isOcuppy);
}
void CADSection::setPreResetState(bool isReset)
{
	SetRelayState(L"FWJ", isReset);
}

bool CADSection::getOccupyStatus()
{
	bool state=getRelayState(L"WGJ");
	return !state;
}

bool CADSection::checkChange()
{
	if (m_blastSendAtpState != getOccupyStatus())
	{
		m_blastSendAtpState = getOccupyStatus();
		return true;
	}
	return false;
}

void CADSection::drawRealSectionName()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CView* pView = (CView*)(pMainFrame->pSplitterFrame_->pChildView_);

	if (data_.bShowSectionName_)
	{
		CString strSectionName = io_.getName();//删除最后一个字母
		CString strRight = strSectionName.Right(1);
		if (strRight == L"A" || strRight == L"B" || strRight == L"C" || strRight == L"D" || strRight == L"E")
			realSecName_.text_ = strSectionName.Left(strSectionName.GetLength() - 1);
		else
			realSecName_.text_ = strSectionName;

		realSecName_.Create(NULL, NULL, WS_CHILD | WS_VISIBLE, data_.SectionNameRect_, pView, 0);

		if (realSecName_.font_.GetSafeHandle() != 0)
		{
			realSecName_.font_.DeleteObject();
		}
		
		realSecName_.data_.logFont_ = name_.data_.logFont_;
		realSecName_.font_.CreateFontIndirect(&realSecName_.data_.logFont_);
		realSecName_.compressWindow();
		realSecName_.ShowWindow(SW_SHOW);
	}
}
BOOL CADSection::isFSSection()
{
	
	//if (vRelay_.size()==0)
	if (name_.text_.Right(2).Left(1) == L"F" || name_.text_.Right(1) == L"F")
	{
		return TRUE;
	}
	return FALSE;
}

void CADSection::OnRPopMenuEx(int flag)
{
	CILKTestDoc* pDoc = (CILKTestDoc*)dynamic_cast<CMainFrame*>(AfxGetMainWnd())->GetActiveDocument();

	switch (flag)
	{
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	default:
		break;
	}
}

void CADSection::setZcCommRelayAllLTEState(BOOL bAT, BOOL bUT)
{
	for (int i = 0; i < vZcCommRelay_.size(); i++)
	{
		if (vZcCommRelay_[i]->name_ == L"AT占用")
		{
			vZcCommRelay_[i]->setStat(bAT, true);
			vZcCommRelay_[i]->setStateByClicked(isHighRelay(i), TRUE);
		}
		if (vZcCommRelay_[i]->name_ == L"UT占用")
		{
			vZcCommRelay_[i]->setStat(bUT, true);
			vZcCommRelay_[i]->setStateByClicked(isHighRelay(i), TRUE);
		}
	}
}