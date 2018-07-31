
// ATP_SIMDoc.cpp : CATP_SIMDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "ATP_SIM.h"
#endif

#include "ATP_SIMDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CATP_SIMDoc

IMPLEMENT_DYNCREATE(CATP_SIMDoc, CDocument)

BEGIN_MESSAGE_MAP(CATP_SIMDoc, CDocument)
END_MESSAGE_MAP()


// CATP_SIMDoc ����/����

CATP_SIMDoc::CATP_SIMDoc()
	: m_CheckID(0)
{
	// TODO: �ڴ����һ���Թ������
	netAddr.localIP = htonl(inet_addr("127.0.0.1"));
	netAddr.remoteIP = htonl(inet_addr("127.0.0.1"));
	netAddr.localPort = 44205;
	netAddr.remotePort = 44005;
}

CATP_SIMDoc::~CATP_SIMDoc()
{
}

BOOL CATP_SIMDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)
	if (-1 == ReadElementCfg())
	{
		AfxMessageBox(L"��ȡ�����ļ�ʧ�ܣ�");
	}
	ReadSignalData();
	ReadSignalStatus();
	ReadTSData();
	ReadTSStatus();
	ReadPtlData();
	ReadPtStatus();
	ReadCrlData();
	ReadCrStatus();
	ReadBlData();
	ReadBlStatus();
	return TRUE;
}




// CATP_SIMDoc ���л�

void CATP_SIMDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
		
	}
	
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CATP_SIMDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// ������������֧��
void CATP_SIMDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:     strSearchContent = _T("point;rectangle;circle;ole object;")��
	SetSearchContent(strSearchContent);
}

void CATP_SIMDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CATP_SIMDoc ���

#ifdef _DEBUG
void CATP_SIMDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CATP_SIMDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CATP_SIMDoc ����

INT32 CATP_SIMDoc::ReadElementCfg()
{
	INT32 result = 0;
	UINT8 readBuffer[MAX_READ_LEN] = { 0 };
	INT32 *pInt;
	ElementCode* pEleCode;
	INT32 tempI = 0;
	INT32 inputIndex = 0;
	INT32 outputIndex = 0;

	FILE *pFile = fopen("..\\Cfg\\ElementCode.le", "r");
	if (pFile != NULL)
	{
		fread(readBuffer, 1, MAX_READ_LEN, pFile);
		pInt = (INT32*)readBuffer;
		s_nRealEle = *pInt;
		pInt++;
		pEleCode = (ElementCode*)pInt;

		for (tempI = 0; tempI < s_nRealEle; tempI++)
		{
			s_ElementDefT[tempI].element_index = pEleCode->ixl_element_index;
			memcpy(s_ElementDefT[tempI].ixl_elementname, pEleCode->ixl_elementname, 64);
			s_ElementDefT[tempI].element_type = pEleCode->wcu_instance_kind_id < 200 ? pEleCode->wcu_instance_kind_id : pEleCode->ixl_elementkind_id;
			s_ElementDefT[tempI].io_type = pEleCode->io_type;
			pEleCode++;
		}
		fclose(pFile);
	}
	else
	{
		result = -1;
	}
	return result;
}

// ANSI To UNCODEת��
CString CATP_SIMDoc::AnsiToUnicode(char * szAnsi, int len)
{
	CString str;
	// ansi to unicode
	//Ԥת�����õ�����ռ�Ĵ�С
	int wcsLen;
	if (len>0)
		wcsLen = len;
	else
		wcsLen = ::MultiByteToWideChar(CP_ACP, NULL, szAnsi, strlen(szAnsi), NULL, 0);
	//����ռ�Ҫ��'\0'�����ռ䣬MultiByteToWideChar�����'\0'�ռ�
	wchar_t* wszString = new wchar_t[wcsLen + 1];
	//ת��
	::MultiByteToWideChar(CP_ACP, NULL, szAnsi, strlen(szAnsi), wszString, wcsLen);
	//������'\0'
	wszString[wcsLen] = '\0';            // UNICODE�ִ�
	str = wszString;
	delete wszString;
	return str;
}



void CATP_SIMDoc::ReadSignalData()
{
	UINT8 signNum = 0;
	for (int i = 0; i < s_nRealEle; i++)
	{
		if (s_ElementDefT[i].element_type == 28)
		{
			sign_check[signNum].name = AnsiToUnicode(s_ElementDefT[i].ixl_elementname, strlen(s_ElementDefT[i].ixl_elementname));
			sign_check[signNum].checkID = IDC_BTN+m_CheckID;
			sign_check[signNum].type = E_SIGN;
			sign_check[signNum].elementID = s_ElementDefT[i].element_index;
			vec_device.push_back(sign_check[signNum]);
			signNum++;
			m_CheckID++;
		}
	}
}

void CATP_SIMDoc::ReadSignalStatus()
{
	const CString status[5] = { L"AT_Appro",L"UT_Appro",
		L"Train Stop",L"Stop Assure",L"WCU_ApproLock" };
	for (int i = 0; i < 5; i++)
	{
		sign_status_check[i].name = status[i];
		sign_status_check[i].checkID = IDC_BTN + m_CheckID;
		sign_status_check[i].type = E_SIGN_STATUS;
		vec_device.push_back(sign_status_check[i]);
		m_CheckID++;
	}
}


CButton* CATP_SIMDoc::NewOneCheckButton(CWnd* pWnd, CString Caption, int nID, CRect rect)
{
	CButton *p_Button = new CButton();
	ASSERT_VALID(p_Button);
	p_Button->Create(Caption, WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX, rect, pWnd, nID); //������ť
	return p_Button;
}


void CATP_SIMDoc::ReadTSStatus()
{
	const CString status[5] = { L"Undefine",L"Clear",
	L"OAT",L"ONAT",L"OD" };
	for (int i = 0; i < 5; i++)
	{
		ts_status_check[i].name = status[i];
		ts_status_check[i].checkID = IDC_BTN + m_CheckID;
		ts_status_check[i].type = E_TS_STATUS;
		vec_device.push_back(ts_status_check[i]);
		m_CheckID++;
	}
}

void CATP_SIMDoc::ReadTSData()
{
	UINT8 num = 0;
	for (int i = 0; i <s_nRealEle; i++)
	{
		if (s_ElementDefT[i].element_type == 20)
		{
			ts_check[num].name = AnsiToUnicode(s_ElementDefT[i].ixl_elementname, strlen(s_ElementDefT[i].ixl_elementname));
			ts_check[num].checkID = IDC_BTN+m_CheckID;
			ts_check[num].type = E_TS;
			ts_check[num].elementID = s_ElementDefT[i].element_index;
			vec_device.push_back(ts_check[num]);
			num++;
			m_CheckID++;
		}
	}
}


void CATP_SIMDoc::ReadPtlData()
{
	UINT8 Num = 0;
	for (int i = 0; i < s_nRealEle; i++)
	{
		if (s_ElementDefT[i].element_type == 18)
		{
			pt_check[Num].name = AnsiToUnicode(s_ElementDefT[i].ixl_elementname, strlen(s_ElementDefT[i].ixl_elementname));
			pt_check[Num].checkID = IDC_BTN + m_CheckID;
			pt_check[Num].type = E_POINT;
			pt_check[Num].elementID = s_ElementDefT[i].element_index;
			vec_device.push_back(pt_check[Num]);
			Num++;
			m_CheckID++;
		}
	}
}

void CATP_SIMDoc::ReadPtStatus()
{
	const CString status[5] = { L"Undefine",L"Clear",
		L"OAT",L"ONAT",L"OD" };
	for (int i = 0; i < 5; i++)
	{
		pt_status_check[i].name = status[i];
		pt_status_check[i].checkID = IDC_BTN + m_CheckID;
		pt_status_check[i].type = E_POINT_STATUS;
		vec_device.push_back(pt_status_check[i]);
		m_CheckID++;
	}
}


void CATP_SIMDoc::ReadCrlData()
{
	UINT8 Num = 0;
	for (int i = 0; i < s_nRealEle; i++)
	{
		if (s_ElementDefT[i].element_type == 19)
		{
			cr_check[Num].name = AnsiToUnicode(s_ElementDefT[i].ixl_elementname, strlen(s_ElementDefT[i].ixl_elementname));
			cr_check[Num].checkID = IDC_BTN + m_CheckID;
			cr_check[Num].type = E_CR;
			cr_check[Num].elementID = s_ElementDefT[i].element_index;
			vec_device.push_back(cr_check[Num]);
			Num++;
			m_CheckID++;
		}
	}
}

void CATP_SIMDoc::ReadCrStatus()
{
	const CString status[5] = { L"Undefine",L"Clear",
		L"OAT",L"ONAT",L"OD" };
	for (int i = 0; i < 5; i++)
	{
		cr_status_check[i].name = status[i];
		cr_status_check[i].checkID = IDC_BTN + m_CheckID;
		cr_status_check[i].type = E_CR_STATUS;
		vec_device.push_back(cr_status_check[i]);
		m_CheckID++;
	}
}


void CATP_SIMDoc::ReadBlData()
{
	UINT8 Num = 0;
	for (int i = 0; i < s_nRealEle; i++)
	{
		if (s_ElementDefT[i].element_type == 31&& s_ElementDefT[i].io_type == 2)
		{
			Bl_check[Num].name = AnsiToUnicode(s_ElementDefT[i].ixl_elementname, strlen(s_ElementDefT[i].ixl_elementname));
			Bl_check[Num].checkID = IDC_BTN + m_CheckID;
			Bl_check[Num].type = E_BL;
			Bl_check[Num].elementID = s_ElementDefT[i].element_index;
			vec_device.push_back(Bl_check[Num]);
			Num++;
			m_CheckID++;
		}
	}
}

void CATP_SIMDoc::ReadBlStatus()
{
	const CString status[3] = { L"Undefine",L"Low",
		L"High" };
	for (int i = 0; i < 3; i++)
	{
		Bl_status_check[i].name = status[i];
		Bl_status_check[i].checkID = IDC_BTN + m_CheckID;
		Bl_status_check[i].type = E_BL_STATUS;
		vec_device.push_back(Bl_status_check[i]);
		m_CheckID++;
	}
}