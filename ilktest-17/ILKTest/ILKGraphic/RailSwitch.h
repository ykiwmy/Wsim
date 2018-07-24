#pragma once
#include "CADDevice.h"
#include "DeviceData.h"


struct SwitchData : public DeviceData
{
#define SWITCH_TOTAL_PT_COUNT 9 + 6 + 14
	CPoint secPt_[3];	// ���ε�
	CPoint norPt_[3];   // ֱ�ɵ�
	CPoint revPt_[3];   // б�ɵ�
	CPoint insuPt_[6];  // ��Ե�� ˳��: section, normal, reverse
	CPoint sealPt_[14]; // ������
	int insuType_[3];	// ��Ե������
	int	type_;
	int	frontPos_;	// ǰ����λ��
	DeviceIO	front_;		// ǰ��������
	DeviceIO	section_;	// ��������
	CRect SectionNameRect_;
	CRect DelayTimeRect_;
	bool bShowSectionName_;
	bool bShowDelayTime_;
	BYTE byReserve[2];
	int	backup_[N_BTYES_BACKUP - 9];
};


class RailSwitch :
	public CADDevice
{
public:
	RailSwitch(void);
	virtual ~RailSwitch(void);
	DECLARE_SERIAL(RailSwitch)
	virtual void Serialize(CArchive& ar);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
public:
	virtual void compressWindow();
	virtual void setSealFrame(CPoint pt[], int height, bool bWnd = false);

	void drawSwitLogicSection(CPaintDC& dc);
public:
	SwitchData	data_;		// ��������
	DeviceIO logicDeviceIO_[MAX_LOGICSWITCH_NUM];//�߼���ַ add by lwm 2016-12-13

	enum FRONT_SWITCH_POS
	{
		POS_NORMAL = 0, POS_REVERSE = 1, NO_FRONT_SWITCH = -1
	};

	enum SWITCH_POS
	{
		NEITHER=0,NORMAL,REVERSE,ERROR_NR
	};
public:

	enum {
		SEC_LEN = 30, NORMAL_LEN = 20, NAIL_LEN = 10 
	};
	enum {
		SEC_FLAG, NOR_FLAG, REV_FLAG
	};
	int totalLen_;  //���峤��
//	int revHeight_;  
	int nailHeight_; //���߶�
	int nailWidth_;  //��ⳤ��

	static int Angle_;		// �Ƕ�
	static int SecLen_;		// �������γ���
	static int NormalLen_;	// ֱ�ɳ���
	static int NailLen_;	// ��ⳤ��
	static int RevHeight_;	// б�ɸ߶�



	enum 
	{
		MAX_POS_NEITHER_COUNT = 26 // 13 �룬500ms��ʱ������
	};
public:
	BOOL isUp(void) { return (data_.type_ % 2) == 0;}
	virtual void OnPopMenuEx(int flag);

	virtual BOOL setPreCondition(CString& operation);
	virtual BOOL checkResult(CString& result);
	virtual void setInputRelay(void);
	int lastSwitchStatus;  //1Ϊ��λ 2Ϊ��λ 3Ϊ�Ŀ� ���ڸ��г�����ʱ�ж��Ƿ�״̬�仯��
	int GetSwitchWCUPos();
	int GetSwitchILPos();
	int m_nlastSendAtpPos;
	bool checkChange();

	CString switchSectionEleType;
	CString switchSectionEleIndex;
	UINT32 switchSectionID_3M;
	UINT32 switchSectonType_3M;
	bool getOccupyStatus();
	bool m_blastSendAtpSectionState;
	bool checkSwitchSectionChange();
	void setDPos(void);
	void setFPos(void);
	void setUnPos(void);
	void drawRealSectionName();
	void setSectionState(bool isOcuppy);
	void setPreResetState(bool isReset);
	Label realSecName_;

	void setZcCommRelayAllLTEState(BOOL bAT, BOOL bUT);
};

