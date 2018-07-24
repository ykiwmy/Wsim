#include "stdafx.h"
#include "CADDoor.h"
#include "ColorDef.h"
#include "../Relay.h"

CADDoor::CADDoor()
:bCutOff(FALSE)
, bClosed(FALSE)
, m_blastSendAtpState(true)
{
}


CADDoor::~CADDoor()
{
}

IMPLEMENT_SERIAL(CADDoor, CADDevice, 1)

void CADDoor::Serialize(CArchive& ar)
{
	CADDevice::Serialize(ar);

	if (!ar.IsStoring())
	{	// loading code
		ar.Read(&data_, sizeof(DoorData));
	}
}




BEGIN_MESSAGE_MAP(CADDoor, CADDevice)
	ON_WM_PAINT()
END_MESSAGE_MAP()


void CADDoor::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CADDevice::OnPaint()

	GetStatus();
	/*dc.SelectObject(SectionPen());
	dc.MoveTo(data_.closePt_[0]);
	dc.LineTo(data_.openPt_[0]);
	dc.MoveTo(data_.closePt_[1]);
	dc.LineTo(data_.openPt_[1]);*/


	CPen * pOldPen = dc.SelectObject(SectionBlackPen());
	dc.MoveTo(data_.closePt_[0]);
	dc.LineTo(data_.closePt_[1]);

	dc.SelectObject(bCutOff ? SectionRedPen() : SectionPen());
	if (bClosed)
	{
		dc.MoveTo(data_.closePt_[0]);
		dc.LineTo(data_.closePt_[1]);
	}
	else
	{
		dc.MoveTo(data_.closePt_[0]);
		dc.LineTo(data_.openPt_[0]);
		dc.MoveTo(data_.closePt_[1]);
		dc.LineTo(data_.openPt_[1]);
	}
	dc.SelectObject(pOldPen);
}
void CADDoor::GetStatus()
{
	if (vRelay_.size()>=2)
	{
		/*bClosed = isRelayUp(0);
		bCutOff = isRelayUp(1);*/

		bClosed = isPsdClosed();
		bCutOff = isPsdCutOff();
	}
	else
	{
		//虚拟站台
		//AfxMessageBox(L"屏蔽门继电器配置错误");
	}
	
}
void CADDoor::setInputRelay(void)
{
	////MGJ QCJ KMJ GMJ
	if (vRelay_.size() >= 3)
	{
		int bKMJ = isOutOpen() ? 1 : 0;
		int bGMJ = isOutClose() ? 2 : 0;
		int psdDoorPos = bKMJ | bGMJ;
		switch (psdDoorPos)
		{
		case 1:
			vRelay_[0]->setStat(FALSE, true);//开
			break;
		case 2:
			vRelay_[0]->setStat(TRUE, true); //关
			break;
		case 3:
			//vRelay_[0]->setStat(TRUE, true);
			break;
		default:
			break;
		}
	}

	
}

bool CADDoor::isPsdOpen()
{

	for (int i = 0; i < vRelay_.size(); i++)
	{
		if (vRelay_[i]->name_.Right(3).Compare(L"MGJ")==0)
		{
			return isRelayDown(i);
		}
	}
	return false;
}

bool CADDoor::isPsdClosed()
{
	int relayIndex = 0xff;
	if (GetRelayIndexByRName(L"MGJ", &relayIndex))
	{
		return isRelayUp(relayIndex);
	}
	return false;
}

bool CADDoor::isOutOpen() //KMJ
{
	for (int i = 0; i < vRelay_.size(); i++)
	{
		if (vRelay_[i]->name_.Right(3).Compare(L"KMJ") == 0)
		{
			return isRelayUp(i);
		}
	}
	return false;
}

bool CADDoor::isOutClose()
{
	int relayIndex = 0xff;
	if (GetRelayIndexByRName(L"GMJ", &relayIndex))
	{
		return isRelayUp(relayIndex);
	}
	return false;
}
bool CADDoor::isPsdCutOff()
{
	int relayIndex = 0xff;
	if (GetRelayIndexByRName(L"QCJ",&relayIndex))
	{
		return isRelayUp(relayIndex);//常态落下，吸起为切除
	}
	else if (GetRelayIndexByRName(L"MPLJ", &relayIndex))
	{
		return isRelayUp(relayIndex);
	}
	return false;
}
bool CADDoor::checkChange()
{
	if (m_blastSendAtpState != isPsdOpen())
	{
		m_blastSendAtpState = isPsdOpen();
		return true;
	}
	return false;
}

BOOL CADDoor::GetRelayIndexByRName(CString rightName,int* numb)
{
	int namelen = rightName.GetLength();
	for (int i = 0; i < vRelay_.size(); i++)
	{
		if (vRelay_[i]->name_.GetLength()>=namelen)
		{
			if (vRelay_[i]->name_.Right(namelen).Compare(rightName)==0)
			{
				*numb= i;
				return TRUE;
			}
		}
	}

	return FALSE;
}