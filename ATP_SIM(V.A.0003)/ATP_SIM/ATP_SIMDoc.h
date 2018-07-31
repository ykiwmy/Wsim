
// ATP_SIMDoc.h : CATP_SIMDoc 类的接口
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
#define MAX_READ_LEN 204800UL /*读取最大字节数(200KB)*/
#define MAX_ELE_NUM		400			/*一个设备集中站中元素最大数*/
#define IDC_BTN 10000
typedef struct ElementCode
{
	UINT16 DevID;//设备编号
	char   ixl_elementname[64];//元素名称
	char   ixl_elementkind[16];//元素类别
	UINT16 ixl_elementkind_id;//元素类别ID
	UINT16 ixl_element_index; //元素索引
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
	char   ixl_elementname[64];//元素名称
	UINT16 element_index; //元素索引
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
protected: // 仅从序列化创建
	CATP_SIMDoc();
	DECLARE_DYNCREATE(CATP_SIMDoc)

// 特性
public:

// 操作
public:
	char *pBuf;  //定义文件指针
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
// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
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

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
