#pragma once
#include "CADDevice.h"
#include "DeviceData.h"


struct SectionData : public DeviceData
{
#define SECTION_TOTAL_N_PT  4 + 8 + 4
	CPoint pt_[4];		// ���ε��ĸ���
	CPoint sealPt_[8];	// ������
	CPoint insuPt_[4];	// ��Ե��
	CRect trainTrack_;	// �г���Ϣ��ʾ
	int insuType_[2];	// ��Ե������
	int sectionType_;
	BYTE byAfgID_;		//������ID
	BYTE byAfgID2_;
	WORD wUpStationID_; //�ϲ�վ̨ID
	WORD wDownStationID_;//�²�վ̨ID
	bool bShowSectionName_;
	bool bShowDelayTime_;
	CRect SectionNameRect_;
	CRect DelayTimeRect_;
	int backup_[N_BTYES_BACKUP - 8 - 4 - 2];
};


class CADSection :
	public CADDevice
{
public:
	CADSection(void) : m_blastSendAtpState(false)
	{}
	virtual ~CADSection(void) {}
	DECLARE_SERIAL(CADSection)
	virtual void Serialize(CArchive& ar);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();

	void setTvsState(CPaintDC& pDC);
	void setLogicState(CPaintDC& pDC);
public:
	virtual void compressWindow();
	virtual void setSealFrame(CPoint pt[], int height, bool bWnd = false);
	virtual void OnRPopMenuEx(int flag);

public:
	SectionData data_;
	DeviceIO logicDeviceIO_;
	enum SECTION_TYPE
	{
		TRACK_SECTION,		// �ɵ�
		NO_SWITCH_SECTION,	// �޲�����
	};

	virtual BOOL setPreCondition(CString& operation);
	virtual BOOL checkResult(CString& result);

	void setSectionState(bool isOcuppy);

	bool getOccupyStatus();
	bool m_blastSendAtpState;
	bool checkChange();
	void drawRealSectionName();
	Label realSecName_;

	BOOL isFSSection();
	void setPreResetState(bool isReset);

	void setZcCommRelayAllLTEState(BOOL bAT, BOOL bUT);
};

