#pragma once

class IOItem; // ���� IOItem ����ʾ״̬
class CADDevice; // ����ù���豸���� CIOView �еĵ����Ϣ

// ��ù�ļ̵���
class Relay
{
public:
	Relay(CString deviceName, CString relayName, BOOL bInput, BOOL bOutput, CADDevice* pDevice);
	~Relay();
public:
	enum RELAY_STAT
	{
		STAT_DOWN/*����*/, STAT_UP/*����*/, STAT_NEIGHER, STAT_BOTH,
	};
public:
	CString deviceName_;	// �豸����
	CString	name_;			// ����
	BOOL	bInput_;		// �Ƿ���������
	BOOL	bOutput_;		// �Ƿ��������
	int		boardID_[2];	// ��ţ������룬�������
	int		portID_[2];		// λ�ţ������룬�������
	RELAY_STAT	stat_;		// ״̬
	RELAY_STAT	lastStat_;	// ��һ��״̬
	IOItem* pIoItem_[2];	// IOItem ��ָ�룬��ȡIO�˿��Ƿ���Ч��״̬������IO�˿ڵ���ʾ��ɫ����ʶ�̵�����ǰ״̬��
	BOOL bDriveUp_;
	int  driveSecond_;
	CADDevice* pDevice_;	// ��ù���豸
	CADDevice* pFDevice_;	//��ʾ�豸
public:
	BOOL isUp(void)   { return stat_ == STAT_UP;	}  //����
	BOOL isDown(void) { return stat_ == STAT_DOWN;	}//����
	void setStat(BOOL bRelayUp, bool bInvalidateDevice);
	BOOL setStat(BOOL bRelayUp, bool bInvalidateDevice,BOOL recvFromCI);
	void ChangeRelayStat();
	void driveRelay(BOOL bDriveUp, int driveSecond); 
	void InvalidateDevice();
	void SetFDevice(CADDevice* pDev);
	void setStateByClicked(BOOL bRelayUp, bool bInvalidateDevice);
	BOOL UpdatapIoItem(BOOL isRelayUp);
};

