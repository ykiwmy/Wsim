#pragma once
class DevBorder
{
public:
	DevBorder(const CPoint& pt1, const CPoint& pt2, int recWidth);
	~DevBorder(void);
protected:
	CPoint recPt_[4];
public:
	CPoint pt(int flag) { return recPt_[flag]; }
	void swapPosition(void);
protected:
	static int	ToInt(double d) { return (int)(d > 0 ? d + 0.5 : d - 0.5); }
};

