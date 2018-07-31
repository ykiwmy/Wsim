// CR_Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ATP_SIM.h"
#include "CR_Dlg.h"
#include "afxdialogex.h"
#include "ATP_SIMView.h"

// CCR_Dlg 对话框

IMPLEMENT_DYNAMIC(CCR_Dlg, CDialogEx)

CCR_Dlg::CCR_Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CR, pParent)
	, m_CRcodebit(0)
{
	F_GL_MemSet(pCrButton, NULL, sizeof(CButton*)*MAX_Cr_COUNT);
	F_GL_MemSet(pCrStatusButton, NULL, sizeof(CButton*)* 5);

}

CCR_Dlg::~CCR_Dlg()
{
	for (int i = 0; i < MAX_Cr_COUNT; i++)
	{
		if (pCrButton[i] != NULL)
		{
			delete pCrButton[i];
		}
	}
	for (int i = 0; i < 5; i++)
	{
		delete pCrStatusButton[i];
	}
}

void CCR_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCR_Dlg, CDialogEx)
	ON_BN_CLICKED(IDC_CR_SEND, &CCR_Dlg::OnBnClickedCrSend)
	ON_BN_CLICKED(IDC_CR_Clear, &CCR_Dlg::OnBnClickedCrClear)
END_MESSAGE_MAP()


// CCR_Dlg 消息处理程序


BOOL CCR_Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	DrawCRCheckButton();
	DrawCrStatusCheckButton();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CCR_Dlg::DrawCRCheckButton()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CATP_SIMView* pView = (CATP_SIMView*)pMain->GetActiveView();
	CATP_SIMDoc* pDoc = pView->GetDocument();
	UINT16 num = 0;
	CONST UINT MAX_CHECKBOX_WIDTH = 130; // 定义CheckBox的宽度
	CONST UINT MAX_CHECKBOX_HEIGHT = 20; // 定义CheckBox的高度
	CONST UINT MAX_CHECKBOX_X_GAP = 15; // 定义CheckBox之间的水平间隔距离
	CONST UINT MAX_CHECKBOX_Y_GAP = 20; // 定义CheckBox之间的垂直间隔距离
	CRect rect;
	rect.SetRectEmpty();
	for (vector<DEVICE_CHECK>::iterator it = pDoc->vec_device.begin();
		it != pDoc->vec_device.end(); it++)
	{
		if (it->type == E_CR)
		{
			if (0 == (num % MAX_ROW))
			{
				rect.left = 10;
				rect.top = rect.bottom + MAX_CHECKBOX_Y_GAP;
			}
			else
			{
				rect.left = rect.right + MAX_CHECKBOX_X_GAP;
			}
			rect.right = rect.left + MAX_CHECKBOX_WIDTH;
			rect.bottom = rect.top + MAX_CHECKBOX_HEIGHT;
			pCrButton[num] = pDoc->NewOneCheckButton(this, it->name, it->checkID, rect);
			num++;
		}
	}
}



void CCR_Dlg::DrawCrStatusCheckButton()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CATP_SIMView* pView = (CATP_SIMView*)pMain->GetActiveView();
	CATP_SIMDoc* pDoc = pView->GetDocument();
	CONST UINT MAX_CHECKBOX_WIDTH = 130; // 定义CheckBox的宽度
	CONST UINT MAX_CHECKBOX_HEIGHT = 20; // 定义CheckBox的高度
	CONST UINT MAX_CHECKBOX_X_GAP = 15; // 定义CheckBox之间的水平间隔距离
	CONST UINT MAX_CHECKBOX_Y_GAP = 20; // 定义CheckBox之间的垂直间隔距离
	CRect clientrect;
	CRect rect;
	UINT16 num = 0;
	GetClientRect(&clientrect);
	for (vector<DEVICE_CHECK>::iterator it = pDoc->vec_device.begin();
		it != pDoc->vec_device.end(); it++)
	{
		if (it->type == E_CR_STATUS)
		{
			if (num == 0)
			{
				rect.left = 10;
			}
			else
			{
				rect.left = rect.right + MAX_CHECKBOX_X_GAP;
			}
			rect.right = rect.left + MAX_CHECKBOX_WIDTH;
			rect.bottom = clientrect.bottom - 10;
			rect.top = rect.bottom - MAX_CHECKBOX_HEIGHT;
			pCrStatusButton[num] = pDoc->NewOneCheckButton(this, it->name, it->checkID, rect);
			num++;
		}
	}
}

void CCR_Dlg::OnBnClickedCrSend()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CATP_SIMView* pView = (CATP_SIMView*)pMain->GetActiveView();
	CATP_SIMDoc* pDoc = pView->GetDocument();
	UINT8 codebit = 0xff;
	UINT32 index = 0xffff;
	UINT8 type = 0xff;
	UINT8 elementnum = 0;
	for (vector<DEVICE_CHECK>::iterator it = pDoc->vec_device.begin();
		it != pDoc->vec_device.end(); it++)
	{
		if (IsBtnSelected(it->checkID) && it->type == E_SIGN_STATUS)
		{
			Computecobebit(it->name);
		}
	}
	codebit = m_CRcodebit;
	for (vector<DEVICE_CHECK>::iterator it = pDoc->vec_device.begin();
		it != pDoc->vec_device.end(); it++)
	{
		if (IsBtnSelected(it->checkID) && it->type == E_SIGN)
		{
			type = 0x13;
			index = it->elementID;
			pView->PackUsrdata(type, index, codebit);
		}
	}
}


bool CCR_Dlg::IsBtnSelected(UINT uID)
{
	CButton *pCheckbox = (CButton*)GetDlgItem(uID);
	if (pCheckbox != NULL &&pCheckbox->GetCheck())
	{
		return true;
	}
	else
	{
		return false;
	}
}


void CCR_Dlg::Computecobebit(CString name)
{
	const CString status[5] = { L"Undefine",L"Clear",
		L"OAT",L"ONAT",L"OD" };
	if (name == status[0])
	{
		m_CRcodebit = 0x00;
	}
	else if (name == status[1])
	{
		m_CRcodebit = 0x02; //1111 1101
	}
	else if (name == status[2])
	{
		m_CRcodebit = 0x04;
	}
	else if (name == status[3])
	{
		m_CRcodebit = 0x06;
	}
	else if (name == status[4])
	{
		m_CRcodebit = 0x08;
	}
}


void CCR_Dlg::OnBnClickedCrClear()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CATP_SIMView* pView = (CATP_SIMView*)pMain->GetActiveView();
	CATP_SIMDoc* pDoc = pView->GetDocument();
	for (int ID = IDC_BTN; ID < IDC_BTN + pDoc->s_nRealEle; ID++)
	{
		CButton *pCheckbox = (CButton*)GetDlgItem(ID);
		if (pCheckbox != NULL)
		{
			pCheckbox->SetCheck(false);
		}
	}
}
