
// ATP_SIMDoc.h : CATP_SIMDoc ��Ľӿ�
//


#pragma once
#include <iostream> 
#include <string>
#include "afxcmn.h"
#include "TS_Dlg.h"
#include "SG_Dlg.h"
#include "PT_Dlg.h"
#include "CR_Dlg.h"
#include "BL_Dlg.h"
#include "Socket.h"
extern SG_Dlg* pSgdlg;
using namespace std;
#define MAX_READ_LEN 204800UL /*��ȡ����ֽ���(200KB)*/
#define MAX_ELE_NUM		400			/*һ���豸����վ��Ԫ�������*/
#define IDC_BTN 10000
typedef struct ElementCode
{
	UINT16 DevID;//�豸���
	char   ixl_elementname[64];//Ԫ������
	char   ixl_elementkind[16];//Ԫ�����
	UINT16 ixl_elementkind_id;//Ԫ�����ID
	UINT16 ixl_element_index; //Ԫ������
	UINT16 instance_kind_id_to_wcu;
	UINT16 instance_kind_id_from_wcu;
	UINT16 ixl_instance_index;
	UINT16 wcu_instance_kind_id;
	UINT16 wcu_instance_index;
	UINT16 ixl_Devaddr;
	UINT16 io_type;
	UINT16 inputaddr;
	UINT16 outputaddr;
}ElementCode;
typedef struct Element
{
	char   ixl_elementname[64];//Ԫ������
	UINT16 element_index; //Ԫ������
	UINT16 element_type;
	UINT16 io_type;
}Element;

typedef struct NetAddr
{
	DWORD localIP;
	DWORD remoteIP;
	int localPort;
	int remotePort;
}NetAddr;

class CATP_SIMDoc : public CDocument
{
protected: // �������л�����
	CATP_SIMDoc();
	DECLARE_DYNCREATE(CATP_SIMDoc)

// ����
public:

// ����
public:
	char *pBuf;  //�����ļ�ָ��
	INT32 s_nRealEle;
	Element s_ElementDefT[MAX_ELE_NUM];
	string strFileName;
	SG_Dlg sgdlg;
	TS_Dlg tsdlg;
	int m_CheckID;
	NetAddr netAddr;
	SocketInterface MySocket;
	vector<DEVICE_CHECK> vec_device;
	DEVICE_CHECK sign_check[MAX_SIGN_COUNT];
	DEVICE_CHECK sign_status_check[5];
	DEVICE_CHECK ts_check[MAX_TS_COUNT];
	DEVICE_CHECK ts_status_check[5];
	DEVICE_CHECK pt_check[MAX_Pt_COUNT];
	DEVICE_CHECK pt_status_check[5];
	DEVICE_CHECK cr_check[MAX_Cr_COUNT];
	DEVICE_CHECK cr_status_check[5];
	DEVICE_CHECK Bl_check[MAX_BL_COUNT];
	DEVICE_CHECK Bl_status_check[3];
// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~CATP_SIMDoc();
	
	INT32 ReadElementCfg();
	CString AnsiToUnicode(char * szAnsi, int len);
	void ReadSignalData();
	void ReadPtlData();
	void ReadCrlData();
	void ReadBlData();
	void ReadBlStatus();
	void ReadCrStatus();
	void ReadPtStatus();
	void ReadSignalStatus();
	CButton * NewOneCheckButton(CWnd * pWnd, CString Caption, int nID, CRect rect);
	void ReadTSStatus();
	void ReadTSData();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
	
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
