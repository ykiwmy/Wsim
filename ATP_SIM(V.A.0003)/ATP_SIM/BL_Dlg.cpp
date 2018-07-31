// BL_Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ATP_SIM.h"
#include "BL_Dlg.h"
#include "afxdialogex.h"
#include "ATP_SIMView.h"

// BL_Dlg 对话框

IMPLEMENT_DYNAMIC(BL_Dlg, CDialogEx)

BL_Dlg::BL_Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_BL, pParent)
	, m_Blcodebit(0)
{
	F_GL_MemSet(pBlButton, NULL, sizeof(CButton*)*MAX_BL_COUNT);
	F_GL_MemSet(pBlStatusButton, NULL, sizeof(CButton*)* 3);
}

BL_Dlg::~BL_Dlg()
{
	for (int i = 0; i < MAX_BL_COUNT; i++)
	{
		if (pBlButton[i] != NULL)
		{
			delete pBlButton[i];
		}
	}
	for (int i = 0; i < 3; i++)
	{
		delete pBlStatusButton[i];
	}
}

void BL_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(BL_Dlg, CDialogEx)
	ON_BN_CLICKED(IDC_BL_SEND, &BL_Dlg::OnBnClickedBlSend)
	ON_BN_CLICKED(IDC_BL_Clear, &BL_Dlg::OnBnClickedBlClear)
END_MESSAGE_MAP()


// BL_Dlg 消息处理程序

void BL_Dlg::DrawBlCheckButton()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CATP_SIMView* pView = (CATP_SIMView*)pMain->GetActiveView();
	CATP_SIMDoc* pDoc = pView->GetDocument();
	UINT16 num = 0;
	CONST UINT MAX_CHECKBOX_WIDTH = 330; // 定义CheckBox的宽度
	CONST UINT MAX_CHECKBOX_HEIGHT = 20; // 定义CheckBox的高度
	CONST UINT MAX_CHECKBOX_X_GAP = 20; // 定义CheckBox之间的水平间隔距离
	CONST UINT MAX_CHECKBOX_Y_GAP = 20; // 定义CheckBox之间的垂直间隔距离
	CRect rect;
	rect.SetRectEmpty();
	for (vector<DEVICE_CHECK>::iterator it = pDoc->vec_device.begin();
		it != pDoc->vec_device.end(); it++)
	{
		if (it->type == E_BL)
		{
			if (0 == (num % 2))
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
			pBlButton[num] = pDoc->NewOneCheckButton(this, it->name, it->checkID, rect);
			num++;
		}
	}
}



void BL_Dlg::DrawBlStatusCheckButton()
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
		if (it->type == E_BL_STATUS)
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
			pBlStatusButton[num] = pDoc->NewOneCheckButton(this, it->name, it->checkID, rect);
			num++;
		}
	}
}

BOOL BL_Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	DrawBlCheckButton();
	DrawBlStatusCheckButton();
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void BL_Dlg::OnBnClickedBlSend()
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
		if (IsBtnSelected(it->checkID) && it->type == E_BL_STATUS)
		{
			Computecobebit(it->name);
		}
	}
	codebit = m_Blcodebit;
	for (vector<DEVICE_CHECK>::iterator it = pDoc->vec_device.begin();
		it != pDoc->vec_device.end(); it++)
	{
		if (IsBtnSelected(it->checkID) && it->type == E_BL)
		{
			type = 0x1f;
			index = it->elementID;
			pView->PackUsrdata(type, index, codebit);
		}
	}
}


bool BL_Dlg::IsBtnSelected(UINT uID)
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


void BL_Dlg::Computecobebit(CString name)
{
	const CString status[3] = { L"Undefine",L"Low",L"High" };
	if (name == status[0])
	{
		m_Blcodebit = 0x00;
	}
	else if (name == status[1])
	{
		m_Blcodebit = 0x01; //1111 1101
	}
	else if (name == status[2])
	{
		m_Blcodebit = 0x02;
	}
}


void BL_Dlg::OnBnClickedBlClear()
{
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
