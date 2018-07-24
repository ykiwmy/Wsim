#pragma once
#include "CADDevice.h"
#include "DeviceData.h"

struct ThreeSwitchData : public DeviceData
{
#define THREESWITCH_TOTAL_PT_COUNT 12 + 8 + 20
	CPoint secPt_[3];	// ���ε�
	CPoint norPt_[3];   // ֱ�ɵ�
	CPoint UprevPt_[3];  // ��б�ɵ�
	CPoint DownrevPt_[3];  // ��б�ɵ�
	CPoint insuPt_[8];  // ��Ե�� ˳��: section, normal, upreverse,downreverse
	CPoint sealPt_[20]; // ������
	int insuType_[4];	// ��Ե������
	int	type_;
	int	frontPos_;	 // ǰ����λ��
	DeviceIO front_; // ǰ��������
	DeviceIO section_;	// ��������
	int	backup_[N_BTYES_BACKUP];
};

class CADThreeSwitch :
	public CADDevice
{
public:
	CADThreeSwitch();
	virtual ~CADThreeSwitch();
	
	DECLARE_SERIAL(CADThreeSwitch)
	virtual void Serialize(CArchive& ar);

	virtual void mapWndPt(CRect rect);

public:

	ThreeSwitchData	data_;		// ��������

	enum FRONT_SWITCH_POS
	{
		POS_NORMAL = 0, POS_UPREVERSE = 1, POS_DOWNREVERSE = 2 , NO_FRONT_SWITCH = -1
	};
public:

	enum {
		SEC_FLAG, NOR_FLAG, UPREV_FLAG, DOWNREV_FLAG
	};
	int totalLen_;		// ���峤��
	int nailHeight_;	// ���߶�
	int nailWidth_;		// ��ⳤ��

	static int Angle_;		// �Ƕ�
	static int SecLen_;		// �������γ���
	static int NormalLen_;	// ֱ�ɳ���
	static int NailLen_;	// ��ⳤ��
	static int RevHeight_;	// б�ɸ߶�
};

