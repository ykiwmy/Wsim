#include "stdafx.h"
#include "DevBorder.h"
#include <math.h>


DevBorder::DevBorder(const CPoint& pt1, const CPoint& pt2, int recWidth)
{
	int halfWidth = recWidth / 2;
	int	dx = pt2.x - pt1.x;
	int	dy = pt2.y - pt1.y;

	if (dy == 0)
	{
		// 水平
		recPt_[0] = CPoint(pt1.x, pt1.y + halfWidth - 1);
		recPt_[1] = CPoint(pt1.x, pt1.y - halfWidth);
		recPt_[2] = CPoint(pt2.x, pt2.y - halfWidth);
		recPt_[3] = CPoint(pt2.x, pt2.y + halfWidth - 1);
	}
	else if (dx == 0)
	{
		// 竖直
		recPt_[0] = CPoint(pt1.x - halfWidth, pt1.y);
		recPt_[1] = CPoint(pt1.x + halfWidth, pt1.y);
		recPt_[2] = CPoint(pt2.x + halfWidth, pt2.y);
		recPt_[3] = CPoint(pt2.x - halfWidth, pt2.y);
	}
	else
	{
		// 斜向
		double recLength = sqrt((double)dx * dx + (double)dy * dy);

		if (recLength != 0)
		{
			int diffX = (ToInt(dy / recLength * halfWidth));
			int diffY = (ToInt(dx / recLength * halfWidth));

			recPt_[0] = CPoint(pt1.x - diffX, pt1.y + diffY);
			recPt_[1] = CPoint(pt1.x + diffX, pt1.y - diffY);
			recPt_[2] = CPoint(pt2.x + diffX, pt2.y - diffY);
			recPt_[3] = CPoint(pt2.x - diffX, pt2.y + diffY);

			if (dx <= 0)
			{
				swapPosition();
			}
		}
	}
}


DevBorder::~DevBorder(void)
{
}


void DevBorder::swapPosition(void)
{
	CPoint temp = recPt_[0];
	recPt_[0] = recPt_[1];
	recPt_[1] = temp;

	temp = recPt_[2];
	recPt_[2] = recPt_[3];
	recPt_[3] = temp;
}
