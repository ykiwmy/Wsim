#pragma once

class IOItem;
class CADDevice;

class WcuComm
{
public:
	WcuComm::WcuComm(CString deviceName,CString relayName, CADDevice* pDevice);
	~WcuComm();

	CString deviceName_;	// Éè±¸Ãû³Æ
	CString name_;
	CADDevice* pDevice_;
};

