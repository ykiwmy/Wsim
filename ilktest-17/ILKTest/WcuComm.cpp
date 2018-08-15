#include "stdafx.h"
#include "WcuComm.h"



WcuComm::WcuComm(CString deviceName, CString relayName,CADDevice* pDevice)
: deviceName_(deviceName)
, name_(relayName)
, pDevice_(pDevice)
{
	
}

WcuComm::~WcuComm()
{
}
