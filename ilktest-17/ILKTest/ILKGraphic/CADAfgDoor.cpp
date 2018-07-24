#include "stdafx.h"
#include "CADAfgDoor.h"
#include "ColorDef.h"
#include "CADDevice.h"
#include "Relay.h"


CADAfgDoor::CADAfgDoor()
:m_blastSendAtpState(false)
{
}

CADAfgDoor::CADAfgDoor(CWnd* pParent)
:m_blastSendAtpState(false)
{
	for (int i = 0; i < AFG_POINT_NUM; i++)
	{
		data_.Point[i] = COLOR_RED;
	}
	for (int i = 0; i < 3; i++)
	{
		data_.color_[i] = COLOR_RED;
		data_.TextColor_[i] = COLOR_RED;
	}
}

CADAfgDoor::~CADAfgDoor()
{
}

IMPLEMENT_SERIAL(CADAfgDoor, CADDevice, 1);

void CADAfgDoor::Serialize(CArchive& ar)
{
	CADDevice::Serialize(ar);//���л����ֺ�IO����

	if (!ar.IsStoring())
	{	// loading code
		ar.Read(&data_, sizeof(data_));
		ar.Read(&deviceIO2_, sizeof(deviceIO2_));
	}
}

//ˢ���������������
void CADAfgDoor::compressWindow()
{
	CRect rect;
	CRgn rgnRect;
	rect.left = data_.Point[0].x;
	rect.right = data_.Point[2].x;
	rect.top = data_.Point[0].y;
	rect.bottom = data_.Point[7].y;
	rgnRect.CreateRectRgnIndirect(rect);//_H ����һ��β�����������
	SetWindowRgn(rgnRect, TRUE);//_H ���ô�������TRUE��ʾ����ϵͳ����������֮���ػ�����
}
BEGIN_MESSAGE_MAP(CADAfgDoor, CADDevice)
	ON_WM_PAINT()
END_MESSAGE_MAP()


void CADAfgDoor::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CADDevice::OnPaint()
	drawSection(dc, data_.Point, AFG_POINT_NUM, FALSE);

	CPen * pPen;
	if (isOpened())
		pPen = AfgOpenPen();
	else
		pPen = AfgClosePen();

	CPen * pOldPen = dc.SelectObject(pPen);
	dc.MoveTo(data_.Point[0]);
	for (int i = 1; i < AFG_POINT_NUM; i++)
		dc.LineTo(data_.Point[i]);
//	dc.SelectObject(pOldPen);

}

bool CADAfgDoor::isOpened()
{
	if (vRelay_.size() > 0)
	{
		return isRelayDown(0);
	}
	return false;
}

bool CADAfgDoor::chackChange()
{
	if (m_blastSendAtpState != isOpened())
	{
		m_blastSendAtpState = isOpened();
		return true;
	}
	return false;
}
