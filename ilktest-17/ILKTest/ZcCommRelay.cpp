#include "stdafx.h"
#include "ZcCommRelay.h"
#include "IOItem.h"
#include "IOCaptionWnd.h"
#include "ILKGraphic\CADDevice.h"
#include "MainFrm.h"
#include "ILKTestDoc.h"
#include "ILKGraphic\RailSwitch.h"
#include "ILKGraphic\CADSignal.h"
#include "ILKGraphic\CADSection.h"


CZcCommRelay::CZcCommRelay(CString deviceName, int deviceType, CString relayName, BOOL bInput, BOOL bOutput, CADDevice* pDevice)
: deviceName_(deviceName)
, devType(deviceType)
, name_(relayName)
, bInput_(bInput)
, bOutput_(bOutput)
, pDevice_(pDevice)
, pFDevice_(NULL)
{
	for (size_t i = 0; i < 2; i++)
	{
		codeSeqIndex[i] = 0xFFFF;
		boardID_[i] = UNUSERELAYINADDR;	// 板号
		portID_[i] = UNUSERELAYINADDR;	// 位号
		pIoItem_[i] = NULL;		// IOItem 的指针
	}
	stat_ = lastStat_ = COMM_LOW;
}


CZcCommRelay::~CZcCommRelay()
{
}

BOOL CZcCommRelay::isSetOppSiteState(BOOL bCommHigh)
{
	BOOL bVal = FALSE;
	if ((name_.Find(L"AT占用") >=0) || (name_.Find(L"UT占用") >=0))
	{
		bVal = !bCommHigh;
	}
	else
	{
		bVal = bCommHigh;
	}
	return bVal;
}
BOOL CZcCommRelay::isSetSameState(BOOL bCommHigh)
{
	BOOL bVal = FALSE;
	if (true)
	{
		bVal = bCommHigh;
	}
	else
	{
		bVal = !bCommHigh;
	}
	return bVal;
}

BOOL CZcCommRelay::isHighStateByCommVal(BOOL bCommHigh)
{
	BOOL bVal = TRUE;
	if (bInput_)
	{
		bVal = isSetOppSiteState(bCommHigh);
		//bVal = isSetSameState(bCommHigh);
	}
	else
	{
		bVal = bCommHigh;
	}
	return bVal;
}

bool CZcCommRelay::isValidSignalCross(int iAuto)
{
	bool bVal = false;
	int color = 0;
	CILKTestDoc* pDoc = (CILKTestDoc*)dynamic_cast<CMainFrame*>(AfxGetMainWnd())->GetActiveDocument();
	if (iAuto == 1) //自动更新
	{
		if (devType == T_ZC_SIGN)
		{
			if (name_.Compare(L"列车跨压") == 0)
			{
				CADSignal* pSignal = (CADSignal*)pDoc->findDevcieByAddr(pDevice_->io_.addr_, RUNTIME_CLASS(CADSignal));
				color = pSignal->getSignalState();
				if ((color == 1) || (color == 2)) //绿灯 黄灯
				{
					bVal = true;
				}
			}
			else
			{
				bVal = true;
			}
		}
		else
		{
			bVal = true;
		}
	}
	else
	{
		bVal = true;
	}

	return bVal;
}

int CZcCommRelay::getRtSignStopAssureByAsk(int iAuto, int srcState, int curDstState)
{
	int state = COMM_LOW;
	CILKTestDoc* pDoc = (CILKTestDoc*)dynamic_cast<CMainFrame*>(AfxGetMainWnd())->GetActiveDocument();

	if (iAuto == 1) //自动更新
	{
		if (devType == T_ZC_SIGN)
		{
			state = srcState;
		}
		else
		{
			state = curDstState;
		}
	}
	else
	{
		state = curDstState;
	}

	return state;
}

void CZcCommRelay::setStat(BOOL bCommHigh, bool bInvalidateDevice)
{
	CILKTestDoc* pDoc = (CILKTestDoc*)dynamic_cast<CMainFrame*>(AfxGetMainWnd())->GetActiveDocument();

	stat_ = isSetOppSiteState(bCommHigh) ? COMM_HIGH : COMM_LOW;

	if (stat_ != lastStat_)
	{
		if (bInput_)
		{
			if (pIoItem_[0] != NULL)
			{
				pIoItem_[0]->pCaption_->Invalidate(FALSE);
			}
			else
			{
				TRACE(L"setStat zccommRelay:" + CString(deviceName_) + CString(name_) + L"\n");
				//AfxMessageBox(deviceName_ + "端口号配置错误:" + name_);
			}
		}

		if (bOutput_)
		{
			if (pIoItem_[1] != NULL)
			{
				pIoItem_[1]->pCaption_->Invalidate(FALSE);
			}
			else
			{
				TRACE(L"setStat zccommRelay:" + CString(deviceName_) + CString(name_) + L"\n");
				//AfxMessageBox(deviceName_ + "端口号配置错误:" + name_);
			}
		}
		lastStat_ = stat_;
	}
	if (bInvalidateDevice)
	{
		InvalidateDevice();
	}
}

void CZcCommRelay::setAutoLowStat(BOOL bCommHigh, int iAuto, int srcState, bool bInvalidateDevice)
{
	CILKTestDoc* pDoc = (CILKTestDoc*)dynamic_cast<CMainFrame*>(AfxGetMainWnd())->GetActiveDocument();

	stat_ = isSetOppSiteState(bCommHigh) ? COMM_HIGH : COMM_LOW;

	if (bInput_)
	{
		switch (devType)
		{
		case T_ZC_SIGN:
			if (name_.Compare(L"列车跨压") == 0)
			{
				stat_ = isValidSignalCross(iAuto) ? stat_ : COMM_LOW;
			}
			else if ((name_.Find(L"_J_停车保证") > 0) || (name_.Find(L"_Z_停车保证") > 0))
			{
				stat_ = (getRtSignStopAssureByAsk(iAuto, srcState, stat_) == 1 ? COMM_HIGH : COMM_LOW);
			}
			else
			{
			}
			break;
		case T_ZC_PHY:
			if ((name_.Find(L"列车停稳") >= 0) && (iAuto == 1))
			{
				stat_ = ((srcState == COMM_LOW) ? COMM_LOW : stat_);
			}
			break;
		default:
			break;
		}
	}

	if (stat_ != lastStat_)
	{
		if (bInput_)
		{
			if (pIoItem_[0] != NULL)
			{
				pIoItem_[0]->pCaption_->Invalidate(FALSE);
			}
			else
			{
				CString str = L"setAutoLowStat zccommRelay:" + CString(deviceName_) + CString(name_);
				TRACE(str);
				//AfxMessageBox(deviceName_ + "端口号配置错误:" + name_);
			}
		}

		if (bOutput_)
		{
			if (pIoItem_[1] != NULL)
			{
				pIoItem_[1]->pCaption_->Invalidate(FALSE);
			}
			else
			{
				CString str = L"setAutoLowStat zccommRelay:" + CString(deviceName_) + CString(name_);
				TRACE(str);
				//AfxMessageBox(deviceName_ + "端口号配置错误:" + name_);
			}
		}
		lastStat_ = stat_;
	}
}

void CZcCommRelay::setStateByClicked(BOOL bCommHigh, bool bInvalidateDevice)
{
	stat_ = isSetOppSiteState(bCommHigh) ? COMM_HIGH : COMM_LOW;

	if (stat_ != lastStat_)
	{
		if (bInput_)
		{
			if (pIoItem_[0] != NULL)
			{
				pIoItem_[0]->pCaption_->Invalidate(FALSE);
			}
			else
			{
				TRACE(L"setStateByClicked zccommRelay:" + CString(deviceName_) + CString(name_) + L"\n");
				//AfxMessageBox(deviceName_ + "端口号配置错误:" + name_);
			}
		}

		if (bOutput_)
		{
			if (pIoItem_[1] != NULL)
			{
				pIoItem_[1]->pCaption_->Invalidate(FALSE);
			}
			else
			{
				TRACE(L"setStateByClicked zccommRelay:" + CString(deviceName_) + CString(name_) + L"\n");
				//AfxMessageBox(deviceName_ + "端口号配置错误:" + name_);
			}
		}
		lastStat_ = stat_;
	}

	if (bInvalidateDevice)
	{
		InvalidateDevice();
	}
}

void CZcCommRelay::InvalidateDevice()
{
	CILKTestDoc* pDoc = (CILKTestDoc*)dynamic_cast<CMainFrame*>(AfxGetMainWnd())->GetActiveDocument();
	if (pDevice_->IsKindOf(RUNTIME_CLASS(CADSection)))
	{
		for each (CADDevice* pDev in pDoc->vDevice_)
		{
			if (pDevice_->io_.getName().Left(pDevice_->io_.getName().GetLength() - 1) == pDev->io_.getName().Left(pDev->io_.getName().GetLength() - 1))
			{
				pDev->Invalidate();
			}
		}
	}
	else
	{
		pDevice_->Invalidate();
	}
	if (pFDevice_ != NULL)
	{
		pFDevice_->Invalidate();
	}

}


//void CZcCommRelay::ChangeRelayStat()
//{
//	if (this->stat_ == COMM_HIGH){
//		this->stat_ = COMM_LOW;
//		this->lastStat_ = COMM_HIGH;
//		if (this->bInput_)
//		{
//			pIoItem_[0]->pCaption_->Invalidate();
//		}
//		if (this->bOutput_)
//		{
//			pIoItem_[1]->pCaption_->Invalidate();
//		}
//	}
//	else if (this->stat_ == COMM_LOW){
//		this->stat_ = COMM_HIGH;
//		this->lastStat_ = COMM_LOW;
//		if (this->bInput_)
//		{
//			pIoItem_[0]->pCaption_->Invalidate();
//		}
//		if (this->bOutput_)
//		{
//			pIoItem_[1]->pCaption_->Invalidate();
//		}
//
//	}
//
//}


void CZcCommRelay::SetFDevice(CADDevice* pDev) 
{
	pFDevice_ = pDev;
}
