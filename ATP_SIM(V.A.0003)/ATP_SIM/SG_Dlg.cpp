// SG_Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ATP_SIM.h"
#include "SG_Dlg.h"
#include "afxdialogex.h"
#include "ATP_SIMView.h"


// SG_Dlg 对话框

IMPLEMENT_DYNAMIC(SG_Dlg, CDialogEx)

SG_Dlg::SG_Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SG, pParent)
	, m_codebit(0x12)
{
	F_GL_MemSet(pSignalButton, NULL, sizeof(CButton*)*MAX_SIGN_COUNT);
	F_GL_MemSet(pSignalStatusButton, NULL, sizeof(CButton*)* 5);
}

SG_Dlg::~SG_Dlg()
{
	for (int i = 0; i < MAX_SIGN_COUNT; i++)
	{
		if (pSignalButton[i] != NULL)
		{
			delete pSignalButton[i];
		}
	}
	for (int i = 0; i < 5; i++)
	{
		delete pSignalStatusButton[i];
	}
}

void SG_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

}



BEGIN_MESSAGE_MAP(SG_Dlg, CDialogEx)
	/*ON_COMMAND_RANGE(0, MAX_SIGN_COUNT - 1, OnCheckButtonClick)*/
	ON_BN_CLICKED(IDC_SG_SEND, &SG_Dlg::OnBnClickedSgSend)
	ON_BN_CLICKED(IDC_SG_Clear, &SG_Dlg::OnBnClickedSgClear)
END_MESSAGE_MAP()


// SG_Dlg 消息处理程序


BOOL SG_Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化
	DrawSignalCheckButton();
	DrawSignalStatusCheckButton();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}




void SG_Dlg::DrawSignalCheckButton()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CATP_SIMView* pView = (CATP_SIMView*)pMain->GetActiveView();
	CATP_SIMDoc* pDoc = pView->GetDocument();
	UINT16 signalnum = 0;
	CONST UINT MAX_CHECKBOX_WIDTH = 130; // 定义CheckBox的宽度
	CONST UINT MAX_CHECKBOX_HEIGHT = 20; // 定义CheckBox的高度
	CONST UINT MAX_CHECKBOX_X_GAP = 15; // 定义CheckBox之间的水平间隔距离
	CONST UINT MAX_CHECKBOX_Y_GAP = 20; // 定义CheckBox之间的垂直间隔距离
	CRect rect;
	rect.SetRectEmpty();
	for (vector<DEVICE_CHECK>::iterator it = pDoc->vec_device.begin();
		it != pDoc->vec_device.end(); it++)
	{
		if (it->type == E_SIGN)
		{
			if (0 == (signalnum % MAX_ROW))
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
			pSignalButton[signalnum] = pDoc->NewOneCheckButton(this, it->name, it->checkID, rect);
			signalnum++;
		}
	}
}



void SG_Dlg::DrawSignalStatusCheckButton()
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
		if (it->type == E_SIGN_STATUS)
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
			pSignalStatusButton[num] = pDoc->NewOneCheckButton(this, it->name, it->checkID, rect);
			num++;
		}
	}
}


bool SG_Dlg::IsBtnSelected(UINT uID)
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

void SG_Dlg::OnBnClickedSgClear()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int ID = IDC_BTN; ID < IDC_BTN + MAX_SIGN_COUNT; ID++)
	{
		CButton *pCheckbox = (CButton*)GetDlgItem(ID);
		if (pCheckbox != NULL)
		{
			pCheckbox->SetCheck(false);
		}
	}
}

void SG_Dlg::OnBnClickedSgSend()
{
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
		if (IsBtnSelected(it->checkID) &&it->type == E_SIGN_STATUS)
		{
			Computecobebit(it->name);
		}
	}
	codebit = m_codebit;
	m_codebit = 0x12;
	for (vector<DEVICE_CHECK>::iterator it = pDoc->vec_device.begin();
		it != pDoc->vec_device.end(); it++)
	{
		if (IsBtnSelected(it->checkID) && it->type == E_SIGN)
		{
			type = 0x1c;
			index =it->elementID;
			pView->PackUsrdata(type, index, codebit);

		}
	}
}

void SG_Dlg::Computecobebit(CString name)
{
	const CString status[5] = { L"AT_Appro",L"UT_Appro",
		L"Train Stop",L"Stop Assure",L"WCU_ApproLock" };
	if (name == status[0])
	{
		m_codebit |= 0x01;
	}
	if (name == status[1])
	{
		m_codebit &= 0xfd; //1111 1101
	}
	if (name == status[2])
	{
		m_codebit |= 0x04;
	}
	if (name == status[3])
	{
		m_codebit |= 0x08;
	}
	if (name == status[4])
	{
		m_codebit &= 0xef;//11101111
	}
}


