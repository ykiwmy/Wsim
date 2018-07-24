#pragma once
#include "CADGraphic.h"
#include "Label.h"
#include "DeviceData.h"


#define MAX_LOGICSWITCH_NUM	4  //add by lwm 2016-12-13


class CADDevice :
	public CADGraphic
{
public:
	CADDevice(void);
	virtual ~CADDevice(void);
	DECLARE_SERIAL(CADDevice)
	virtual void Serialize(CArchive& ar);
public:
	// ������
	void drawSection(CPaintDC& dc, CPoint pt[], int nPt, BOOL bOccupied);

	//�߼�����
	void drawLogicSection(CPaintDC& dc, CPoint pt[], int nPt, BOOL bAT, BOOL bUT);

	// ����Ե��
	void drawInsu(CPaintDC& dc, CPoint pt[], int type[], int nInsu);

	CPoint getCrossPoint(CPoint& line1Pt1, CPoint& line1Pt2, CPoint& line2Pt1, CPoint& line2Pt2);
public:
	DeviceInput io_;
	Label name_;
	CRect nameRect_;

	// �������
	BOOL bStatChanged_;
public:
	static int SectionWidth_;
	static int InsuHeight_;

	enum INSU_TYPE {
		NORMAL_INSU,	// ��ͨ��Ե
		LIMIT_INSU,		// ���޾�Ե
		NON_INSU,		// �޾�Ե
	};

	enum 
	{
		OCCUPIED, LOCKED, IDLE
	};

	DECLARE_MESSAGE_MAP()
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
public:
	int TwoPtDistance(CPoint& pt1, CPoint& pt2);
public:
	// ��λ����ӵĺ���

	int getBits(UINT16 src, int bitFlag, int nBits); // ����������

	BOOL checkStatus(void* lastStatus, void* status, int statusSize); // ����豸״̬�Ƿ����

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	virtual BOOL setPreCondition(CString& operation) { return TRUE; } // ������������е�Ԥ�趨���豸״̬

	virtual BOOL checkResult(CString& result) { return TRUE; } // �����������Ƿ�����Ԥ��

	virtual void setInputRelay(void) {} // ���������ļ̵��������²ɼ��ļ̵���
	void SetRelayState(CString relayName, bool bStatus);
	bool getRelayState(CString relayName);
	void DriveRelayState(CString relayName, bool bStatus);
	CString eleType;
	CString eleIndex;

	UINT16 devType_3M;
	UINT32 devId_3M;

	BOOL statusChanged;
	BOOL m_bFlash;
	int m_nflashCount;
};

