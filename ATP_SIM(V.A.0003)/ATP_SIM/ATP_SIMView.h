
// ATP_SIMView.h : CATP_SIMView ��Ľӿ�
//

#pragma once
#include "afxcmn.h"
#include "ATP_SIMDoc.h"

#include "Device.h"
extern "C"
{
#include "sahara.h"
}

class CATP_SIMView : public CFormView
{
protected: // �������л�����
	CATP_SIMView();
	DECLARE_DYNCREATE(CATP_SIMView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_ATP_SIM_FORM };
#endif

// ����
public:
	CATP_SIMDoc* GetDocument() const;

// ����
public:
	TS_Dlg m_TS;
	SG_Dlg m_SG;
	PT_Dlg m_PT;
	CCR_Dlg m_CR;
	BL_Dlg m_BL;
	vector<DEVICE_CHECK> vec_select;
// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnInitialUpdate(); // ������һ�ε���

	void CreateTab();

// ʵ��
public:
	virtual ~CATP_SIMView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:

	CTabCtrl m_tab;
	
	S_Buf MsgFromCI;
	S_Buf SendUsrData;
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	MyUINT16 Buffer2MyUINT16(const MyUINT8 * bufferPtr);

	void PackUsrdata(UINT8 type, UINT32 index, UINT8 codebit);

	void ComStatus();

	void SendAllele();

	afx_msg void OnSendAllele();
	afx_msg void OnConnection();
	afx_msg void OnDisconn();
};

#ifndef _DEBUG  // ATP_SIMView.cpp �еĵ��԰汾
inline CATP_SIMDoc* CATP_SIMView::GetDocument() const
   { return reinterpret_cast<CATP_SIMDoc*>(m_pDocument); }
#endif

