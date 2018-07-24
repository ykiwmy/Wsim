#include "stdafx.h"
#include "CADSemiAutoBlock.h"


CADSemiAutoBlock::CADSemiAutoBlock(void)
{
}


CADSemiAutoBlock::~CADSemiAutoBlock(void)
{
}


IMPLEMENT_SERIAL(CADSemiAutoBlock, CADGraphic, 1)


void CADSemiAutoBlock::Serialize(CArchive& ar)
{
	int i;
	if (ar.IsStoring())
	{	// storing code
	}
	else
	{	// loading code
		ar >> type_ >> nArrow_ >> nBtn_ >> caption_;
		for (i = 0; i < nArrow_; i++)
		{
			ar >> arrowRect_[i];
		}
		for (int i = 0; i < nBtn_; i++)
		{
			ar >> btnRect_[i];
		}
	}

	for (i = 0; i < nArrow_; i++)
		arrow_[i].Serialize(ar);
	
	for (i = 0; i < nBtn_; i++)
		btn_[i].Serialize(ar);
}

void CADSemiAutoBlock::mapWndPt(CRect rect)
{
	
}
