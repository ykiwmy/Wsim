#pragma once
#include "CADDevice.h"
#include "DeviceData.h"

struct ThreeSwitchData : public DeviceData
{
#define THREESWITCH_TOTAL_PT_COUNT 12 + 8 + 20
	CPoint secPt_[3];	// 区段点
	CPoint norPt_[3];   // 直股点
	CPoint UprevPt_[3];  // 上斜股点
	CPoint DownrevPt_[3];  // 下斜股点
	CPoint insuPt_[8];  // 绝缘结 顺序: section, normal, upreverse,downreverse
	CPoint sealPt_[20]; // 封锁框
	int insuType_[4];	// 绝缘结类型
	int	type_;
	int	frontPos_;	 // 前方岔位置
	DeviceIO front_; // 前方岔数据
	DeviceIO section_;	// 区段数据
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

	ThreeSwitchData	data_;		// 道岔数据

	enum FRONT_SWITCH_POS
	{
		POS_NORMAL = 0, POS_UPREVERSE = 1, POS_DOWNREVERSE = 2 , NO_FRONT_SWITCH = -1
	};
public:

	enum {
		SEC_FLAG, NOR_FLAG, UPREV_FLAG, DOWNREV_FLAG
	};
	int totalLen_;		// 整体长度
	int nailHeight_;	// 岔尖高度
	int nailWidth_;		// 岔尖长度

	static int Angle_;		// 角度
	static int SecLen_;		// 道岔区段长度
	static int NormalLen_;	// 直股长度
	static int NailLen_;	// 岔尖长度
	static int RevHeight_;	// 斜股高度
};

