#pragma once
#include "afxwin.h"
#include <vector>
typedef enum DEVICE_TYPE
{
	E_SIGN = 10,
	E_SIGN_STATUS = 11,
	E_POINT = 12,
	E_POINT_STATUS = 13,
	E_TS = 14,
	E_TS_STATUS = 15,
	E_BL = 16,
	E_BL_STATUS = 17,
	E_CR = 18,
	E_CR_STATUS = 19,
}DEVICE_TYPE;
typedef struct
{
	CString name;
	UINT16 checkID;
	UINT8 type;
	UINT32 elementID;
}DEVICE_CHECK;

class CDevice :
	public CWnd
{
public:
	CDevice();
	virtual ~CDevice();
};

