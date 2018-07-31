
// ATP_SIMView.cpp : CATP_SIMView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "ATP_SIM.h"
#endif


#include "ATP_SIMView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CATP_SIMView

IMPLEMENT_DYNCREATE(CATP_SIMView, CFormView)

BEGIN_MESSAGE_MAP(CATP_SIMView, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CATP_SIMView::OnTcnSelchangeTab1)
	ON_WM_TIMER()
	ON_COMMAND(ID_SEND_ALLELE, &CATP_SIMView::OnSendAllele)
	ON_COMMAND(ID_Connection, &CATP_SIMView::OnConnection)
	ON_COMMAND(ID_DisConn, &CATP_SIMView::OnDisconn)
END_MESSAGE_MAP()

// CATP_SIMView 构造/析构

CATP_SIMView::CATP_SIMView()
	: CFormView(IDD_ATP_SIM_FORM)
	
{
	// TODO: 在此处添加构造代码
	MsgFromCI = { 0 };
	SendUsrData = { 0 };
}

CATP_SIMView::~CATP_SIMView()
{
}

void CATP_SIMView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab);
}

BOOL CATP_SIMView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CFormView::PreCreateWindow(cs);
}

void CATP_SIMView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit(); 
	// TODO: Add extra initialization here  
	F_Sahara_Init(0);
	CreateTab();
	
}

void CATP_SIMView::CreateTab() 
{
	CRect tabRect;   // 标签控件客户区的位置和大小   
	m_tab.InsertItem(0, _T("Signal"));         // 插入第一个标签“SIGNAL”   
	m_tab.InsertItem(1, _T("TS"));				// 插入第二个标签“TS”   
	m_tab.InsertItem(2, _T("Point"));
	m_tab.InsertItem(3, _T("Cross"));
	m_tab.InsertItem(4, _T("Bool"));
	m_SG.Create(IDD_SG, &m_tab); // 创建第二个标签页
	m_TS.Create(IDD_TS, &m_tab);    // 创建第一个标签页   
	m_PT.Create(IDD_PT, &m_tab);
	m_CR.Create(IDD_CR, &m_tab);
	m_BL.Create(IDD_BL, &m_tab);
	m_tab.GetClientRect(&tabRect);    // 获取标签控件客户区Rect   
									  // 调整tabRect，使其覆盖范围适合放置标签页   
	tabRect.left = 1;
	tabRect.right = 750;
	tabRect.top = 30;
	tabRect.bottom = 1000;
	m_tab.MoveWindow(CRect(10,10,750,950));
	// 根据调整好的tabRect放置m_TS子对话框，并设置为隐藏   
	// 根据调整好的tabRect放置m_SG子对话框，并设置为显示  
	m_SG.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
	m_TS.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
	m_PT.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
	m_CR.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
	m_BL.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
}

// CATP_SIMView 诊断

#ifdef _DEBUG
void CATP_SIMView::AssertValid() const
{
	CFormView::AssertValid();
}

void CATP_SIMView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CATP_SIMDoc* CATP_SIMView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CATP_SIMDoc)));
	return (CATP_SIMDoc*)m_pDocument;
}
#endif //_DEBUG



void CATP_SIMView::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here   
	*pResult = 0;
	CRect tabRect;    // 标签控件客户区的Rect   

	// 获取标签控件客户区Rect，并对其调整，以适合放置标签页   
	m_tab.GetClientRect(&tabRect);
	tabRect.left += 1;
	tabRect.right -= 1;
	tabRect.top += 25;
	tabRect.bottom -= 1;

	switch (m_tab.GetCurSel())
	{
	case 0:
		m_SG.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
		m_TS.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_PT.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_CR.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_BL.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		break;
	case 1:
		m_SG.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_TS.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
		m_PT.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_CR.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_BL.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		break;
	case 2:
		m_SG.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_TS.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_PT.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
		m_CR.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_BL.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		break;
	case 3:
		m_SG.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_TS.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_PT.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_CR.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
		m_BL.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		break;
	case 4:
		m_SG.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_TS.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_PT.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_CR.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
		m_BL.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
		break;
	default:
		break;
	}
}




void CATP_SIMView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CATP_SIMDoc* pDoc = this->GetDocument();
	S_Buf usrdata = { 0 };
	S_Buf outdata = { 0 };
	S_Buf sendbuff = { 0 };
	MyUINT16 length = 0;
	S_Buf temp = { 0 };

	switch (nIDEvent)
	{
	case 1:
		ComStatus();
		F_Sahara_UpdateClock();
		for (int i = 0; i<10; i++)
		{
			temp.msgLength = 0;
			pDoc->MySocket.F_Recv(DevCI1, (BYTE*)temp.msg, (MyUINT16*)&temp.msgLength);
			if (temp.msgLength > 0U)
			{
				F_GL_MemCpy(&MsgFromCI.msg[MsgFromCI.msgLength], temp.msg,temp.msgLength);
				MsgFromCI.msgLength += temp.msgLength;
			}
		}
	
		F_Sahara_CheckData(0, MsgFromCI.msgLength, (MyUINT8*)MsgFromCI.msg, (MyUINT16*)&usrdata.msgLength, (MyUINT8*)usrdata.msg);
		F_GL_MemSet(&MsgFromCI,0,sizeof(S_Buf));
		F_Sahara_SendData(0, SendUsrData.msgLength, (MyUINT8*)SendUsrData.msg, (MyUINT16*)&outdata.msgLength,(MyUINT8*)outdata.msg);
		if (SendUsrData.msgLength > 0)
		{
			SendUsrData = { 0 };
		}
		while (outdata.msgLength > 0)
		{
			length = Buffer2MyUINT16(outdata.msg);
			F_GL_MemCpy(sendbuff.msg, outdata.msg, length);
			sendbuff.msgLength = length;
			outdata.msgLength -= length;
			pDoc->MySocket.F_Send(DevCI1, sendbuff.msg, sendbuff.msgLength);
			F_GL_MemCpy(outdata.msg, &outdata.msg[length], outdata.msgLength);
			length = 0;
			sendbuff = { 0 };
		}
		break;
	default:
		break;
	}
	CFormView::OnTimer(nIDEvent);
}

/** ************************************************************
BUFF填入MyUINT16
************************************************************* */
MyUINT16 CATP_SIMView::Buffer2MyUINT16(const MyUINT8 * bufferPtr) {
	MyUINT16 result = 0;
	result = bufferPtr[1] * 256 + bufferPtr[0];
	return result;
}



void CATP_SIMView::PackUsrdata(UINT8 type,UINT32 index, UINT8 codebit)
{
	UINT8 telMsg[38] = { 0 };
	telMsg[0] = 37u;	/*vMsgLen*/
	telMsg[1] = 4u;		/*vFktNr*/
	telMsg[2] = 91u;	/*mbx-low*/
	telMsg[3] = 0u;		/*mbx-high*/
	telMsg[4] = 0u;		/*mode*/
	telMsg[5] = 0u;		/*mode*/
						/*Communication Header*/
	telMsg[6] = 0u;		/*remoteinsel -1*/
	telMsg[7] = 0x05u;		/*remoteaddr  -2*/
	telMsg[8] = 200u;	/*telType	  -3*/
						/*WCU telegram header*/
	telMsg[9] = 0u;		/*Time-Informaiton	-4*/
	telMsg[10] = 0u;	/*Time-Informaiton	-5*/
	telMsg[11] = 0u;	/*Time-Informaiton	-6*/
	telMsg[12] = 0u;	/*Time-Informaiton	-7*/
	
	telMsg[13] = type;		/*Sender element type		225-8*/
	telMsg[14] = index % 256;		/*Sender element index-low	-9*/
	telMsg[15] = index / 256;		/*Sender element index-high	-10*/
	telMsg[16] = 0u;	/*Sender flag -low			-11*/
	telMsg[17] = 0u;	/*Sender flag -high			-12*/

	telMsg[18] = 219;		/*Receiver element type			-13*/
	telMsg[19] = index % 256;		/*Receiver element index-low	-14*/
	telMsg[20] = index / 256;		/*Receiver element index-high	-15*/
	telMsg[21] = 0u;	/*Usr telegram type				-16*/
	telMsg[22] = 0xd3;
	telMsg[23] = 0x28;
	telMsg[24] = 0x81;
	telMsg[25] = codebit;
	memcpy(&SendUsrData.msg[SendUsrData.msgLength], telMsg, 38);
	SendUsrData.msgLength += 38;
}


void CATP_SIMView::ComStatus()
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	CString str;
	MyUINT8 state = F_Sahara_GetStatus(0);
	if (state == e_UsrTrue)
	{
		str.Format(L"正常");
	}
	else
	{
		str.Format(L"断开");
	}
	
	pMain->m_wndStatusBar.SetPaneText(pMain->m_wndStatusBar.CommandToIndex(ID_COM_STATUS), L"链路状态："+str);
}


void CATP_SIMView::SendAllele()
{
	// TODO: 在此添加命令处理程序代码
	CATP_SIMDoc* pDoc = this->GetDocument();
	UINT8 codebit = 0xff;
	UINT8 type;
	for (vector<DEVICE_CHECK>::iterator it = pDoc->vec_device.begin();
	it != pDoc->vec_device.end(); it++)
	{
		switch (it->type)
		{
		case E_SIGN:
			type = 0x1c;
			codebit = 0x12;
			PackUsrdata(type, it->elementID, codebit);
			break;
		case E_POINT:
			type = 0x12;
			codebit = 0x02;
			PackUsrdata(type, it->elementID, codebit);
			break;
		case E_CR:
			type = 0x13;
			codebit = 0x02;
			PackUsrdata(type, it->elementID, codebit);
			break;
		case E_TS:
			type = 0x14;
			codebit = 0x02;
			PackUsrdata(type, it->elementID, codebit);
			break;
		case E_BL:
			type = 0x1f;
			codebit = 0x00;
			PackUsrdata(type, it->elementID, codebit);
			break;
		default:
			break;
		}

	}
}


void CATP_SIMView::OnSendAllele()
{
	// TODO: 在此添加命令处理程序代码
	SendAllele();
}


void CATP_SIMView::OnConnection()
{
	// TODO: 在此添加命令处理程序代码
	CATP_SIMDoc* pDoc = this->GetDocument();
	SetTimer(1, 200, NULL);
	pDoc->MySocket.F_InitSock(pDoc->netAddr.localIP, pDoc->netAddr.remoteIP, pDoc->netAddr.localPort, pDoc->netAddr.remotePort);
	F_Sahara_Init(0);
}


void CATP_SIMView::OnDisconn()
{
	// TODO: 在此添加命令处理程序代码
	CATP_SIMDoc* pDoc = this->GetDocument();
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	KillTimer(1);
	pDoc->MySocket.F_CloseConnect();
	pMain->m_wndStatusBar.SetPaneText(pMain->m_wndStatusBar.CommandToIndex(ID_COM_STATUS), L"链路状态：断开");
}
