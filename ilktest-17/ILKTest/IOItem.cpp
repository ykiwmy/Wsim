#include "stdafx.h"
#include "IOItem.h"


IOItem::IOItem()
: pCaption_(NULL)
, pBtn_(NULL)
{
}


IOItem::~IOItem()
{
}


BOOL IOItem::isPortValid()
{
	if (pBtn_!=NULL)
	{
		return pBtn_->GetCheck() == BST_UNCHECKED;
	}
	return TRUE;
}
