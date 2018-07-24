// CADGraphic.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CADGraphic.h"
#include "ColorDef.h"
#include <math.h>
#include "../Relay.h"
#include "ZcCommRelay.h"
#include "CADDevice.h"
#include "MainFrm.h"
#include "ILKTestDoc.h"

// CADGraphic
IMPLEMENT_SERIAL(CADGraphic, CWnd, 1)

CADGraphic::CADGraphic()
: pMenu_(new CMenu())
, pRMenu_(new CMenu())
{
	pMenu_->CreatePopupMenu();
	pRMenu_->CreatePopupMenu();
}


CADGraphic::CADGraphic(CWnd* pParent)
{
	createEmptyWnd(pParent);
}


CADGraphic::~CADGraphic()
{
	delete pMenu_;
	delete pRMenu_;
}


// CADGraphic ��Ϣ�������



void CADGraphic::setCRgnWindow(CPoint wndPt[], int nPt)
{
	CRgn wndRgn;
	VERIFY(wndRgn.CreatePolygonRgn(wndPt, nPt, ALTERNATE));
	VERIFY(SetWindowRgn(HRGN(wndRgn), FALSE));
}


void CADGraphic::loadMenu(void)
{
	for (size_t i = 0; i < vRelay_.size(); i++)
	{
		pMenu_->AppendMenuW(MF_STRING, RELAY_MENU_0 + i, vRelay_[i]->name_);
	}

	for (size_t i = 0; i < vZcCommRelay_.size(); i++)
	{
		pRMenu_->AppendMenuW(MF_STRING, ZC_COMM_MENU_0 + i, vZcCommRelay_[i]->name_);
	}
}


void CADGraphic::OnPopMenu(UINT menuID)
{
	int flag = menuID - RELAY_MENU_0;

	vRelay_[flag]->setStateByClicked(!isRelayUp(flag), TRUE);

	if (vRelay_[flag]->pDevice_ != NULL)
	{
		vRelay_.at(flag)->pDevice_->setInputRelay();
	}
	OnPopMenuEx(flag); // �����в������е���������״̬��˫�������е���һ������
}

void CADGraphic::OnRPopMenu(UINT menuID)
{
	CILKTestDoc* pDoc = (CILKTestDoc*)dynamic_cast<CMainFrame*>(AfxGetMainWnd())->GetActiveDocument();

	int flag = menuID - ZC_COMM_MENU_0;
	if (flag<vZcCommRelay_.size())
	{
		vZcCommRelay_[flag]->setStateByClicked(isHighRelay(flag), TRUE);
		pDoc->setSwitSectOtherSwitARBState(vZcCommRelay_[flag]);
		pDoc->setSwitSectOtherSwitStopState(vZcCommRelay_[flag]);
		pDoc->setSwitSectAllSwitOcpState(vZcCommRelay_[flag]);
		pDoc->setSwitLogicState(vZcCommRelay_[flag]);

		if (vZcCommRelay_[flag]->isHigh())
		{
			vZcCommRelay_[flag]->setStat(FALSE, true);
		}
		else
		{
			vZcCommRelay_[flag]->setStat(TRUE, true);
		}
	}
	OnRPopMenuEx(flag);
}

void CADGraphic::OnClickRelay(Relay* pRelay)
{
	size_t i = 0;
	while (i < vRelay_.size())
	{
		if (vRelay_[i] == pRelay)
		{
			OnPopMenu(RELAY_MENU_0 + i);
			break;
		}
		i++;
	}
}

void CADGraphic::OnClickZcCommRelay(CZcCommRelay* pZcRelay)
{
	int i = 0;
	while (i < vZcCommRelay_.size())
	{
		if (vZcCommRelay_[i] == pZcRelay)
		{
			OnRPopMenu(ZC_COMM_MENU_0 + i);
			break;
		}
		i++;
	}
}


BEGIN_MESSAGE_MAP(CADGraphic, CWnd)
ON_WM_LBUTTONUP()
ON_COMMAND_RANGE(RELAY_MENU_0, RELAY_MENU_0 + 10, &CADGraphic::OnPopMenu)
ON_COMMAND_RANGE(ZC_COMM_MENU_0, ZC_COMM_MENU_0 + 100, &CADGraphic::OnRPopMenu)

ON_WM_RBUTTONUP()
END_MESSAGE_MAP()


void CADGraphic::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	for (size_t i = 0; i < vRelay_.size(); i++)
	{
		pMenu_->CheckMenuItem(RELAY_MENU_0 + i, isRelayUp(i) ? MF_CHECKED : MF_UNCHECKED);
	}

	ClientToScreen(&point);
	pMenu_->TrackPopupMenu(TPM_LEFTALIGN /*| TPM_RIGHTBUTTON*/, point.x, point.y, this);
	//CWnd::OnLButtonUp(nFlags, point);
}

void CADGraphic::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (vZcCommRelay_.size() > 0)
	{
		for (size_t i = 0; i < vZcCommRelay_.size(); i++)
		{
			pRMenu_->CheckMenuItem(RELAY_MENU_0 + i, isRelayUp(i) ? MF_CHECKED : MF_UNCHECKED);
		}

		ClientToScreen(&point);
		pRMenu_->TrackPopupMenu(TPM_LEFTALIGN /*| TPM_RIGHTBUTTON*/, point.x, point.y, this);
	}
}



BOOL CADGraphic::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  �ڴ����ר�ô����/����û���
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		afxGlobalData.GetHandCursor(), (HBRUSH)GetStockObject(BLACK_BRUSH), 0);

	return CWnd::PreCreateWindow(cs);
}

BOOL CADGraphic::isRelayUp(int relayNumb)
{
	if (vRelay_.size()>relayNumb)
	{
		return vRelay_[relayNumb]->isUp();
	}
	CString strErr(L"�̵����������");
	strErr.Format(strErr + L"vRelay_.size()=%d,���Ϊ%d", vRelay_.size(), relayNumb);
	AfxGetMainWnd()->SendMessage(WM_OUT_MESSAGE, 1, (LPARAM)&strErr);
	return FALSE;
	
}

BOOL CADGraphic::isRelayDown(int relayNumb)
{
	if (vRelay_.size() > relayNumb)
	{
		return vRelay_[relayNumb]->isDown();
	}
	CString strErr;
	strErr.Format(L"�̵���������� vRelay_.size()=%d,���=%d", vRelay_.size(), relayNumb);
	AfxGetMainWnd()->SendMessage(WM_OUT_MESSAGE, 1, (LPARAM)&strErr);
	return FALSE;

}

BOOL CADGraphic::isHighRelay(int flag)
{
	if (vZcCommRelay_.size() > flag)
	{
		return vZcCommRelay_[flag]->isHigh();
	}
	return FALSE;
}

BOOL CADGraphic::isATRelay(int flag)
{
	int tempI = 0, iTimes = 0;
	BOOL bHigh = FALSE, bLow = FALSE;
	for (tempI = 0; tempI < vZcCommRelay_.size() && (iTimes != 2); tempI++)
	{
		if (vZcCommRelay_[tempI]->name_.Find(L"ATռ��") >= 0)
		{
			iTimes++;
		}
		
		if ((iTimes == flag) && (flag <=2) && (tempI+1<vZcCommRelay_.size()))
		{
			bHigh = vZcCommRelay_[tempI]->isHigh() ? TRUE : FALSE;
			bLow = vZcCommRelay_[tempI + 1]->isHigh() ? TRUE : FALSE;
			break;
		}
	}
	if (bHigh && !bLow)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
BOOL CADGraphic::isUTRelay(int flag)
{
	int tempI = 0, iTimes = 0;
	BOOL bHigh = FALSE, bLow = FALSE;
	for (tempI = 0; tempI < vZcCommRelay_.size() && (iTimes != 2); tempI++)
	{
		if (vZcCommRelay_[tempI]->name_.Find(L"ATռ��") >= 0)
		{
			iTimes++;
		}

		if ((iTimes == flag) && (flag <=2) && (tempI + 1<vZcCommRelay_.size()))
		{
			bHigh = vZcCommRelay_[tempI]->isHigh() ? TRUE : FALSE;
			bLow = vZcCommRelay_[tempI+1]->isHigh() ? TRUE : FALSE;
			break;
		}
	}
	if (!bHigh && bLow)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

