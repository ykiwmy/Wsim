#include "stdafx.h"
#include "Relay.h"
#include "IOItem.h"
#include "IOCaptionWnd.h"
#include "ILKGraphic\CADDevice.h"
#include "MainFrm.h"
#include "ILKTestDoc.h"
#include "ILKGraphic\RailSwitch.h"
#include "ILKGraphic\CADSignal.h"
#include "ILKGraphic\CADSection.h"

Relay::Relay(CString deviceName, CString relayName, BOOL bInput, BOOL bOutput, CADDevice* pDevice)
: deviceName_(deviceName)
, name_(relayName)
, bInput_(bInput)
, bOutput_(bOutput)
, pDevice_(pDevice)
, bDriveUp_(FALSE)
, driveSecond_(-1)
, pFDevice_(NULL)
{
	for (size_t i = 0; i < 2; i++)
	{
		boardID_[i] = UNUSERELAYINADDR;	// ���
		portID_[i] = UNUSERELAYINADDR;	// λ��
		//pIoItem_[i] = NULL;		// IOItem ��ָ��
	}
	stat_ = lastStat_ = STAT_DOWN;
}

Relay::~Relay()
{
}


void Relay::setStat(BOOL bRelayUp, bool bInvalidateDevice)
{ 
	//�ɼ�ר�ã����������Ͳɼ���ƥ���������ֻ�вɼ��̵���
	if (bInput_)
	{
		if (pIoItem_[0] != NULL)
		{
			if (!pIoItem_[0]->isPortValid())
			{
				CString strErr;
				strErr.Format(L"״̬���ð�ť���£�������Ч" + deviceName_);
				AfxGetMainWnd()->SendMessage(WM_OUT_MESSAGE, 1, (LPARAM)&strErr);
				return;
			}
		}
		else
		{
			CString strErr;
			strErr.Format(L"����̵���Ϊ�գ�������Ч" + deviceName_);
			AfxGetMainWnd()->SendMessage(WM_OUT_MESSAGE, 1, (LPARAM)&strErr);
			return;
		}
	}

	UpdatapIoItem(bRelayUp);

	if (bInvalidateDevice)
	{
		InvalidateDevice();
	}
}

void Relay::setStateByClicked(BOOL bRelayUp, bool bInvalidateDevice)
{
	
	UpdatapIoItem(bRelayUp);

	if (bInvalidateDevice)
	{
		InvalidateDevice();
	}
}

BOOL Relay::setStat(BOOL bRelayUp, bool bInvalidateDevice, BOOL recvFromCI)
{
	//ֻ������λ������

	BOOL statechange = FALSE;
	CILKTestDoc* pDoc = (CILKTestDoc*)dynamic_cast<CMainFrame*>(AfxGetMainWnd())->GetActiveDocument();
	if (bOutput_)
	{
		// ���������̵����ұ���ѡ���������أ����Դ�״̬
		if (pIoItem_[1] != NULL)
		{
			if (!pIoItem_[1]->isPortValid())
			{
				CString strErr;
				strErr.Format(L"����̵����ұ���ѡ��������Ч" + deviceName_);
				AfxGetMainWnd()->SendMessage(WM_OUT_MESSAGE, 1, (LPARAM)&strErr);

				return FALSE;
			}
		}
		else
		{
			CString strErr;
			strErr.Format(L"����̵���Ϊ�գ�������Ч" + deviceName_);
			AfxGetMainWnd()->SendMessage(WM_OUT_MESSAGE, 1, (LPARAM)&strErr);
			return FALSE;
		}
	}

	BOOL stat = bRelayUp ? STAT_UP : STAT_DOWN;

	if (stat != lastStat_ && driveSecond_<=0)
	{
		driveRelay(bRelayUp, bRelayUp ? pDoc->m_nUpTime : pDoc->m_nDownTime);
	}


	//if (UpdatapIoItem(bRelayUp))
	//{
	//	statechange = TRUE;
	//}
	//

	//if (bInvalidateDevice)//�����д��λtrue���շ���false
	//{
	//	InvalidateDevice();
	//}

	return statechange;
}

void Relay::ChangeRelayStat()  //��������ɢ�̵����¼�
{
	if (this->stat_ == STAT_UP){
		this->stat_ = STAT_DOWN;
		this->lastStat_ = STAT_UP;
		if (this->bInput_)
		{
			pIoItem_[0]->pCaption_->Invalidate();
		}
		if (this->bOutput_)
		{
			pIoItem_[1]->pCaption_->Invalidate();
		}
	}
	else if (this->stat_ == STAT_DOWN){
		this->stat_ = STAT_UP;
		this->lastStat_ = STAT_DOWN;
		if (this->bInput_)
		{
			pIoItem_[0]->pCaption_->Invalidate();
		}
		if (this->bOutput_)
		{
			pIoItem_[1]->pCaption_->Invalidate();
		}
		
	}
	
}


void Relay::InvalidateDevice()
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
	else if (pDevice_->IsKindOf(RUNTIME_CLASS(RailSwitch)))
	{
		CILKTestDoc* pDoc = (CILKTestDoc*)dynamic_cast<CMainFrame*>(AfxGetMainWnd())->GetActiveDocument();
		RailSwitch* pSwitch = static_cast<RailSwitch*>(pDevice_);
		if (pSwitch->data_.front_.addr_ != UNUSERELAYINADDR)
		{
			RailSwitch* pFrontSwitch = (RailSwitch*)pDoc->findDevcieByAddr(pSwitch->data_.front_.addr_, RUNTIME_CLASS(RailSwitch));
			pFrontSwitch->Invalidate();
		}
	}
	else
	{
		
	}

	if (pDevice_!=NULL)
	{
		pDevice_->Invalidate();
	}
	
	//��ʾ�豸
	if (pFDevice_!=NULL)
	{
		pFDevice_->Invalidate();
	}
	
}


void Relay::SetFDevice(CADDevice* pDev) //һ���̵��������豸����ʾ
{
	pFDevice_ = pDev;
}

void Relay::driveRelay(BOOL bDriveUp, int driveSecond)
{
	bDriveUp_ = bDriveUp;

	if (driveSecond_<=0)
	{
		driveSecond_ = driveSecond;
	}
}

BOOL Relay::UpdatapIoItem(BOOL isRelayUp)
{
	BOOL statechange = FALSE;

	stat_ = isRelayUp ? STAT_UP : STAT_DOWN;

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
				AfxMessageBox(deviceName_ + "�˿ں����ô���:" + name_);
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
				AfxMessageBox(deviceName_ + "�˿ں����ô���:" + name_);
			}
		}
		lastStat_ = stat_;

		statechange = TRUE;
	}

	return statechange;
}
