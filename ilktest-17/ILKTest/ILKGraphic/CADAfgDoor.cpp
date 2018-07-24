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
	CADDevice::Serialize(ar);//序列化名字和IO数据

	if (!ar.IsStoring())
	{	// loading code
		ar.Read(&data_, sizeof(data_));
		ar.Read(&deviceIO2_, sizeof(deviceIO2_));
	}
}

//刷新面积按照最大计算
void CADAfgDoor::compressWindow()
{
	CRect rect;
	CRgn rgnRect;
	rect.left = data_.Point[0].x;
	rect.right = data_.Point[2].x;
	rect.top = data_.Point[0].y;
	rect.bottom = data_.Point[7].y;
	rgnRect.CreateRectRgnIndirect(rect);//_H 创建一个尾部多边形区域
	SetWindowRgn(rgnRect, TRUE);//_H 设置窗口区域，TRUE表示操作系统在设置区域之后重画窗口
}
BEGIN_MESSAGE_MAP(CADAfgDoor, CADDevice)
	ON_WM_PAINT()
END_MESSAGE_MAP()


void CADAfgDoor::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CADDevice::OnPaint()
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
