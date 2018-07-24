#include "stdafx.h"
#include "RailSwitch.h"
#include "DevBorder.h"
#include "ColorDef.h"
#include "../Relay.h"
#include "ZcCommRelay.h"
#include "../MainFrm.h"
#include "../ILKTestDoc.h"
#include "SplitterFrame.h"
#include "ChildView.h"

RailSwitch::RailSwitch(void)
: m_nlastSendAtpPos(Switch_Left)
, m_blastSendAtpSectionState(false)
, switchSectionID_3M(0)
, switchSectonType_3M(0)
{
}


RailSwitch::~RailSwitch(void)
{
}


IMPLEMENT_SERIAL(RailSwitch, CADDevice, 1)


void RailSwitch::Serialize(CArchive& ar)
{
	CADDevice::Serialize(ar);

	if (!ar.IsStoring())
	{	// loading code
		ar.Read(&data_, sizeof(data_));
		ar.Read(&logicDeviceIO_, MAX_LOGICSWITCH_NUM * sizeof(DeviceIO));//add by lwm 2016-12-13

		drawRealSectionName();
	}
}


BEGIN_MESSAGE_MAP(RailSwitch, CADDevice)
	ON_WM_PAINT()
END_MESSAGE_MAP()


void RailSwitch::OnPaint()
{
	CPaintDC dc(this); 
	CILKTestDoc* pDoc = (CILKTestDoc*)dynamic_cast<CMainFrame*>(AfxGetMainWnd())->GetActiveDocument();
	bool bOccupyStatus;
	bOccupyStatus = getOccupyStatus();
	if (data_.front_.addr_ != UNUSERELAYINADDR)
	{
		RailSwitch* pSwitch = (RailSwitch*)pDoc->findDevcieByAddr(data_.front_.addr_,RUNTIME_CLASS(RailSwitch));
		int frontstat = pSwitch->GetSwitchILPos();
		
		if (frontstat == 2 && GetSwitchILPos() == 2)//同时反位
		{
			if (pSwitch->data_.front_.addr_ != data_.front_.addr_)//不互为前方叉
			{
				bOccupyStatus = false;
			}
		}
		else //考虑前方岔
		{
			if (data_.frontPos_ == (frontstat - 1) && (bOccupyStatus == true))//不是前方岔的位置，清掉本道岔显示
			{
				bOccupyStatus = false;
			}
		}
		
		drawSection(dc, data_.secPt_, 3, bOccupyStatus);
	}
	else
	{
		drawSection(dc, data_.secPt_, 3, bOccupyStatus);
	}
//	drawSection(dc, data_.secPt_, 3, isRelayDown(0) ? TRUE : FALSE);

	int stat = GetSwitchILPos();

	BOOL bNormalUse = bOccupyStatus;
	BOOL bReverseUse = bOccupyStatus;

	if (data_.type_>3)
	{
		if (stat==1)
		{
			stat = 2;
		}
		else if (stat==2)
		{
			stat = 1;
		}
		else
		{

		}
	}

	switch (stat)
	{
	case 0:
		// 四开

		break;
	case 1:
		// 定位
		bReverseUse = FALSE;
		dc.LineTo(data_.norPt_[2]);
		break;
	case 2:
		// 反位
		bNormalUse = FALSE;
		dc.LineTo(data_.revPt_[2]);
		break;
	case 3:
		// 不应该有的现象，也属四开
	/*	dc.LineTo(data_.norPt_[2]);
		dc.MoveTo(data_.secPt_[2]);
		dc.LineTo(data_.revPt_[2]);*/
		break;
	default:
		// 卧槽
		break;
	}

	drawSection(dc, data_.norPt_, 3, bNormalUse);
	drawSection(dc, data_.revPt_, 3, bReverseUse);
	drawInsu(dc, data_.insuPt_, data_.insuType_, 3);
	drawSwitLogicSection(dc);
}

void RailSwitch::drawSwitLogicSection(CPaintDC& dc)
{
	int stat = 0;

	CILKTestDoc* pDoc = (CILKTestDoc*)dynamic_cast<CMainFrame*>(AfxGetMainWnd())->GetActiveDocument();
	
	stat = GetSwitchILPos();
	if (pDoc->m_isLogicSection == 1)
	{
		switch (stat)
		{
		case 0:
		case 3:
			drawLogicSection(dc, data_.secPt_, 3, this->isATRelay(1), this->isUTRelay(1));
			drawLogicSection(dc, data_.norPt_, 3, this->isATRelay(1), this->isUTRelay(1));
			drawLogicSection(dc, data_.secPt_, 3, this->isATRelay(2), this->isUTRelay(2));
			drawLogicSection(dc, data_.revPt_, 3, this->isATRelay(2), this->isUTRelay(2));
			break;
		case 1: //定位
			if (data_.type_ < 4) //直股定位
			{
				drawLogicSection(dc, data_.secPt_, 3, this->isATRelay(1), this->isUTRelay(1));
				drawLogicSection(dc, data_.norPt_, 3, this->isATRelay(1), this->isUTRelay(1));
			}
			else
			{
				drawLogicSection(dc, data_.secPt_, 3, this->isATRelay(1), this->isUTRelay(1));
				drawLogicSection(dc, data_.revPt_, 3, this->isATRelay(1), this->isUTRelay(1));
			}
			break;
		case 2: //反位
			if (data_.type_ < 4) //直股定位
			{
				drawLogicSection(dc, data_.secPt_, 3, this->isATRelay(2), this->isUTRelay(2));
				drawLogicSection(dc, data_.revPt_, 3, this->isATRelay(2), this->isUTRelay(2));
			}
			else
			{
				drawLogicSection(dc, data_.secPt_, 3, this->isATRelay(2), this->isUTRelay(2));
				drawLogicSection(dc, data_.norPt_, 3, this->isATRelay(2), this->isUTRelay(2));
			}
			break;
		default:
			break;
		}
	}
}

void RailSwitch::setZcCommRelayAllLTEState(BOOL bAT, BOOL bUT)
{
	int stat = 0;
	int iTimes[2] = { 0, 0 };
	CILKTestDoc* pDoc = (CILKTestDoc*)dynamic_cast<CMainFrame*>(AfxGetMainWnd())->GetActiveDocument();

	stat = GetSwitchILPos();

	switch (stat)
	{
	case 0:
	case 3:
		break;
	default:
		break;
	}

	for (int i = 0; i < vZcCommRelay_.size(); i++)
	{
		if (vZcCommRelay_[i]->name_.Find(L"AT占用") >=0)
		{
			iTimes[0]++;
			if (bAT) //占用，必一个占用；故障也一个故障即可
			{
				if ((iTimes[0] == 1) && ((stat == 0) || (stat == 1) || (stat == 3)))
				{
					vZcCommRelay_[i]->setStat(bAT, true);
					vZcCommRelay_[i]->setStateByClicked(isHighRelay(i), TRUE);
					pDoc->setSwitLogicState(vZcCommRelay_[i]);
				}
				if ((iTimes[0] == 2) && ((stat == 0) || (stat == 2) || (stat == 3)))
				{
					vZcCommRelay_[i]->setStat(bAT, true);
					vZcCommRelay_[i]->setStateByClicked(isHighRelay(i), TRUE);
					pDoc->setSwitLogicState(vZcCommRelay_[i]);
				}
			}
			else //空闲，则都空闲
			{
				vZcCommRelay_[i]->setStat(bAT, true);
				vZcCommRelay_[i]->setStateByClicked(isHighRelay(i), TRUE);
				pDoc->setSwitLogicState(vZcCommRelay_[i]);
			}
		}
		if (vZcCommRelay_[i]->name_.Find(L"UT占用") >= 0)
		{
			iTimes[1]++;
			if (bUT)
			{
				if ((iTimes[1] == 1) && ((stat == 0) || (stat == 1) || (stat == 3)))
				{
					vZcCommRelay_[i]->setStat(bUT, true);
					vZcCommRelay_[i]->setStateByClicked(isHighRelay(i), TRUE);
					pDoc->setSwitLogicState(vZcCommRelay_[i]);
				}
				if ((iTimes[1] == 2) && ((stat == 0) || (stat == 2) || (stat == 3)))
				{
					vZcCommRelay_[i]->setStat(bUT, true);
					vZcCommRelay_[i]->setStateByClicked(isHighRelay(i), TRUE);
					pDoc->setSwitLogicState(vZcCommRelay_[i]);
				}
			}
			else
			{
				vZcCommRelay_[i]->setStat(bUT, true);
				vZcCommRelay_[i]->setStateByClicked(isHighRelay(i), TRUE);
				pDoc->setSwitLogicState(vZcCommRelay_[i]);
			}
		}
	}
}



void RailSwitch::compressWindow()
{
	CPoint wndPt[14];
	setSealFrame(wndPt, InsuHeight_ + 3, true);
	setCRgnWindow(wndPt, 14);
}


void RailSwitch::setSealFrame(CPoint pt[], int height, bool bWnd)
{
	DevBorder bd[5] = {
		DevBorder(data_.secPt_[0], data_.secPt_[1], height),
		DevBorder(data_.secPt_[1], data_.norPt_[2], height),
		DevBorder(data_.norPt_[0], data_.norPt_[1], height),
		DevBorder(data_.revPt_[0], data_.revPt_[1], height),
		DevBorder(data_.revPt_[1], data_.revPt_[2], height),
	};
	if (!isUp())
		for (int i = 0; i < 5; i++)
			bd[i].swapPosition();

	pt[0]	= bd[0].pt(0);
	pt[1]	= bd[0].pt(1);
	pt[2]	= getCrossPoint(bd[0].pt(1), bd[0].pt(2), bd[1].pt(1), bd[1].pt(2));
	pt[3]	= getCrossPoint(bd[1].pt(1), bd[1].pt(2), bd[4].pt(1), bd[4].pt(2));
	pt[4]	= getCrossPoint(bd[3].pt(1), bd[3].pt(2), bd[4].pt(1), bd[4].pt(2));
	pt[5]	= bd[3].pt(1);
	pt[6]	= bd[3].pt(0);
	pt[7]	= getCrossPoint(bd[3].pt(0), bd[3].pt(3), bd[4].pt(0), bd[4].pt(3));
	pt[8]	= getCrossPoint(bd[1].pt(1), bd[1].pt(2), bd[4].pt(0), bd[4].pt(3));
	pt[9]	= getCrossPoint(bd[1].pt(1), bd[1].pt(2), bd[2].pt(1), bd[2].pt(2));	
	pt[10]	= bd[2].pt(1);
	pt[11]	= bd[2].pt(0);
	pt[12]	= getCrossPoint(bd[1].pt(0), bd[1].pt(3), bd[2].pt(0), bd[2].pt(3));	
	pt[13]	= getCrossPoint(bd[0].pt(0), bd[0].pt(3), bd[1].pt(0), bd[1].pt(3));
}


void RailSwitch::OnPopMenuEx(int flag)
{
	if (flag == 0)
	{
		// 有岔区段占用状态变更
		CILKTestDoc* pDoc = (CILKTestDoc*)dynamic_cast<CMainFrame*>(AfxGetMainWnd())->GetActiveDocument();
		pDoc->OnSetSwitchSection(data_.section_.addr_, this);
	}
	else
	{

	}
}

BOOL RailSwitch::setPreCondition(CString& operation)
{
	if (operation.Compare(L"定位") == 0)
	{
		vRelay_[2]->setStat(TRUE, FALSE);
		vRelay_[3]->setStat(FALSE, TRUE);
	}
	else if (operation.Compare(L"反位") == 0)
	{
		vRelay_[2]->setStat(FALSE, FALSE);
		vRelay_[3]->setStat(TRUE, TRUE);
	}
	else if (operation.Compare(L"四开") == 0)
	{
		vRelay_[2]->setStat(FALSE, FALSE);
		vRelay_[3]->setStat(FALSE, TRUE);
	}
	else
	{
		return FALSE;
	}

	Invalidate();

	return TRUE;
}

BOOL RailSwitch::checkResult(CString& result)
{
	static CString defResult[] = {
		L"四开", L"反位", L"定位", L"四开",
	};

	return defResult[(isRelayUp(2) << 1) | isRelayUp(3)].Compare(result) == 0;
}

void RailSwitch::setInputRelay(void)
{

	CILKTestDoc* pDoc = (CILKTestDoc*)dynamic_cast<CMainFrame*>(AfxGetMainWnd())->GetActiveDocument();
	if (isRelayUp(5) || isRelayUp(6))  //DCJ,FCJ吸起，DBJ,FBJ延时吸起，设置延时时间
	{
		for (size_t i = 2; i < 4; i++)
		{
			vRelay_[i]->setStat(FALSE, FALSE);
		}
		vRelay_[isRelayUp(5) ? 2 : 3]->driveRelay(TRUE, pDoc->driveTimeCount);

	}
}

int RailSwitch::GetSwitchWCUPos()
{
	int switchPos = GetSwitchILPos();
	int c_t2wPtPos[4][4] =
	{		
		/*左位          右位          四开*/
		///*左上*/{ Switch_Trailed, Switch_ForceLeft, Switch_ForceRight },
		///*左下*/{ Switch_Trailed, Switch_ForceRight, Switch_ForceLeft },
		///*右上*/{ Switch_Trailed, Switch_ForceRight, Switch_ForceLeft },
		///*右下*/{ Switch_Trailed, Switch_ForceLeft, Switch_ForceRight }
		/*左上*/{ Switch_Trailed, Switch_Left, Switch_Right, Switch_Trailed },
		/*左下*/{ Switch_Trailed, Switch_Right, Switch_Left, Switch_Trailed },
		/*右上*/{ Switch_Trailed, Switch_Right, Switch_Left, Switch_Trailed },
		/*右下*/{ Switch_Trailed, Switch_Left, Switch_Right, Switch_Trailed }
	};
	return c_t2wPtPos[this->data_.type_][switchPos];
}

int RailSwitch::GetSwitchILPos()
{
	int bDBJ = isRelayUp(2) ? 1 : 0;
	int bFBJ = isRelayUp(3) ? 2 : 0;
	int switchPos = bDBJ | bFBJ;
	return switchPos;
}

bool RailSwitch::checkChange()
{
	if (m_nlastSendAtpPos != GetSwitchWCUPos())
	{
		m_nlastSendAtpPos = GetSwitchWCUPos();
		return true;
	}
	return false;
}

bool RailSwitch::getOccupyStatus()
{
	if (vRelay_.size()>0)
	{
		return isRelayDown(0);
	}
	return false;
}

bool RailSwitch::checkSwitchSectionChange()
{
	if (m_blastSendAtpSectionState != getOccupyStatus())
	{
		m_blastSendAtpSectionState = getOccupyStatus();
		return true;
	}
	return false;
}

void RailSwitch::setDPos(void)
{
	CILKTestDoc* pDoc = (CILKTestDoc*)dynamic_cast<CMainFrame*>(AfxGetMainWnd())->GetActiveDocument();
	if (vRelay_.size()>=4)
	{
		for (size_t i = 2; i < 4; i++)
		{
			vRelay_[i]->setStat(FALSE, FALSE);
		}
		vRelay_[2]->driveRelay(TRUE, pDoc->driveTimeCount);
	}
	
}

void RailSwitch::setFPos(void)
{
	CILKTestDoc* pDoc = (CILKTestDoc*)dynamic_cast<CMainFrame*>(AfxGetMainWnd())->GetActiveDocument();

	if (vRelay_.size() >= 4)
	{
		for (size_t i = 2; i < 4; i++)
		{
			vRelay_[i]->setStat(FALSE, FALSE);
		}
		vRelay_[3]->driveRelay(TRUE, pDoc->driveTimeCount);
	}
	
}  

void RailSwitch::setUnPos(void)
{
	if (vRelay_.size()>=4)
	{
		for (size_t i = 2; i < 4; i++)
		{
			vRelay_[i]->setStat(FALSE, true);
		}
	}
}

void RailSwitch::drawRealSectionName()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CView* pView = (CView*)(pMainFrame->pSplitterFrame_->pChildView_);

	if (data_.bShowSectionName_)
	{
		CString strSectionName = data_.section_.getName();//删除最后一个字母
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

void RailSwitch::setSectionState(bool isOcuppy)
{
	SetRelayState(L"GJ", !isOcuppy);
}
void RailSwitch::setPreResetState(bool isReset)
{
	SetRelayState(L"FWJ", !isReset);
}