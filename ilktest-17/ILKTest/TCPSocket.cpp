#include "stdafx.h"
#include "TCPSocket.h"
#include "resource.h"
#include "ILKGraphic\CADDevice.h"
#include "ILKGraphic\CADSection.h"
#include "ILKGraphic\RailSwitch.h"
#include "ILKGraphic\CADSignal.h"
#include "ILKGraphic\CADDoor.h"
#include "ILKGraphic\CADButton.h"
#include "ILKGraphic\CADDelight.h"
#include "ILKGraphic\CADAfgDoor.h"
#include "ILKTestDoc.h"
#include "Relay.h"
#include "MySever.h"

CTCPSocket::CTCPSocket(CILKTestDoc* doc, CMySever* pSev)
: pParentSev(pSev)
, pDoc(doc)
, m_ReciveCount(0)
, m_lastReciveCount(0)
, showDlg(NULL)
, IXL_id(_T("5"))//225
{
	memset(&remoteAddr_, 0, sizeof(sockaddr));
	CreatATPView();
}


CTCPSocket::~CTCPSocket()
{
	if (m_hSocket != INVALID_SOCKET)
	{
		Close();
	}
	if (showDlg!=NULL)
	{
		delete showDlg;
	}
}


void CTCPSocket::OnReceive(int nErrorCode)
{
	// TODO:  在此添加专用代码和/或调用基类
	char ReciveBuff[1024];
	memset(ReciveBuff, 0, sizeof(ReciveBuff));

	int ReciveLen = Receive(ReciveBuff,1024,0);

	if (ReciveLen >0)
	{
		CString recv(ReciveBuff);
		ShowTipMsg(L"接收数据：" + recv);

		if (checkRecvData(recv))
		{
			sendACK(ReciveBuff, ReciveLen);

			if (myRecvData.type.Compare(L"command") == 0)  //设置单个设备状态
			{
				processCommand(recv);
			}
			else if (myRecvData.type.Compare(L"sendAllStates") == 0)//请求全站状态
			{
				processAllStateCommandCycle();
			}
		}
	}
	CAsyncSocket::OnReceive(nErrorCode);
}


void CTCPSocket::OnConnect(int nErrorCode)
{
	// TODO:  在此添加专用代码和/或调用基类
	CAsyncSocket::OnConnect(nErrorCode);
}

void CTCPSocket::SendPackDate()
{
	//char  senddate_[1024];
	//int senddatelen=pDoc->PackTcpDate(senddate_);
	//int m= Send(senddate_,senddatelen,0);
}

int CTCPSocket::GetReciveCount()
{
	return m_ReciveCount;
}

int CTCPSocket::GetLastReciveCount()
{
	return m_lastReciveCount;
}

void CTCPSocket::SetLastReciveCount(int count)
{
	m_lastReciveCount = count;
}

void CTCPSocket::OnAccept(int nErrorCode)
{
	// TODO:  在此添加专用代码和/或调用基类

	CAsyncSocket::OnAccept(nErrorCode);
}


void CTCPSocket::ShowTipMsg(CString msg)
{
	showDlg->showMsg(msg);
}


void CTCPSocket::OnClose(int nErrorCode)
{
	// TODO:  在此添加专用代码和/或调用基类
	if (nErrorCode==0)
	{

		showDlg->SetClientIPAddr(0);
		showDlg->setClientPort(0);
		ShowTipMsg(L"断开连接！！！");

		pParentSev->deletClient(this);
	}
	CAsyncSocket::OnClose(nErrorCode);
}


void CTCPSocket::CreatATPView()
{
	showDlg = new CTCPShowDlg();
	showDlg->Create(IDD_DLGTCPSHOW);
	showDlg->UpdateData(FALSE);
	showDlg->ShowWindow(SW_HIDE);
}

void CTCPSocket::processCommand(CString recvdata)
{
	RecvATPCommandData commadndData;
	CString str1 = recvdata;
	CString  str = getStrLeftByCh(str1, 0x0a);
	
	while (str.GetLength() > 0)
	{
		commadndData.ixlNo = getStrLeftByCh(str, '_');
		commadndData.eleType = getStrLeftByCh(str, '_');
		commadndData.eleIndex = getStrLeftByCh(str, ',');
		commadndData.state = getStrLeftByCh(str, ')');;
		if (commadndData.eleType == L"213")
		{
			setAtpSectionState(commadndData.eleIndex, commadndData.state);
		}
		else if (commadndData.eleType == L"25501")
		{
			setAtpPsdState(commadndData.eleIndex, commadndData.state);
		}
		else if (commadndData.eleType == L"25502")
		{
			setAtpEmpState(commadndData.eleIndex, commadndData.state);
		}
		else if (commadndData.eleType == L"25503")
		{
			setAtpDrbState(commadndData.eleIndex, commadndData.state);
		}
		else
		{
			ShowTipMsg(L"命令类型错误：" + commadndData.eleType);
		}
		if (str1.GetLength()>0)
		{
			str = getStrLeftByCh(str1, 0x0a);
		}
	}
	
}


CString CTCPSocket::getStrLeftByCh(CString &recvdata, char ch)
{
	CString head(L"");
	int mm = recvdata.Find(ch);
	if (mm>0)
	{
		head = recvdata.Left(mm);
		recvdata = recvdata.Mid(mm+1);
	}
	else
	{
		ShowTipMsg(L"查找头分隔符失败：" + recvdata);
	}
	
	return head;
}


void CTCPSocket::processAllStateCommand()
{
	char sendBuff[1024];
	int sendBufflen = 0;
	memset(sendBuff,0,sizeof(sendBuff));
	for each (CADDevice* pDevice in pDoc->vDevice_)
	{
		if (pDevice->IsKindOf(RUNTIME_CLASS(RailSwitch)))
		{
			RailSwitch* pSwitch = dynamic_cast<RailSwitch*>(pDevice);

			CString strpos = getSwitchPos(pSwitch->GetSwitchWCUPos());
			if (pSwitch->eleIndex!=L"")
			{
				int len = packA2Map(pSwitch->eleType,pSwitch->eleIndex,strpos,&sendBuff[sendBufflen]);
				sendBufflen += len;
			}
		}
		else if (pDevice->IsKindOf(RUNTIME_CLASS(CADSignal)))
		{
			CADSignal* pSignal = dynamic_cast<CADSignal*>(pDevice);
			CString strcolor = GetSignalState(pSignal->getSignalState(),pSignal->getDDJstate());
			if (pSignal->eleIndex != L"")
			{
				int len = packA2Map(pSignal->eleType, pSignal->eleIndex, strcolor, &sendBuff[sendBufflen]);
				sendBufflen += len;
			}
		}
		else
		{
		}
	}

	sendData(sendBuff, sendBufflen);
}

void CTCPSocket::processAllStateCommandCycle()
{
	for each (CADDevice* pDevice in pDoc->vDevice_)
	{
		if (pDevice->IsKindOf(RUNTIME_CLASS(RailSwitch)))
		{
			//道岔
			RailSwitch* pSwitch = dynamic_cast<RailSwitch*>(pDevice);
			sendSwitchPos(pSwitch);
			
			//道岔区段
			sendSwitchSectionState(pSwitch);
		}
		else if (pDevice->IsKindOf(RUNTIME_CLASS(CADSignal)))
		{
			CADSignal* pSignal = dynamic_cast<CADSignal*>(pDevice);
			sendSignalState(pSignal);
		}
		else if (pDevice->IsKindOf(RUNTIME_CLASS(CADSection)))
		{
			CADSection* pSection = dynamic_cast<CADSection*>(pDevice);
			sendSectionState(pSection);
		}		
	}

	sendLingSanState();
}

bool CTCPSocket::checkRecvData(CString data)
{
	bool checkResult = false;

	if (pDoc->m_isCnctToGalaxy<=0)
	{
		ShowTipMsg(L"收大galaxy数据，配置文件<是否外接Galaxy>属性为0，不处理" + data);
		return checkResult;
	}
	CString str = data;
	myRecvData.head = getStrLeftByCh(str, ':');
	myRecvData.type = getStrLeftByCh(str, '(');
	if (myRecvData.head.Compare(L"A2Map") != 0)
	{
		ShowTipMsg(L"校验头部错误" + data);
		return false;
	}

	if (myRecvData.type.Compare(L"command") == 0)  //设置单个设备状态
	{
		checkResult = true;
	}
	else if (myRecvData.type.Compare(L"sendAllStates") == 0)//请求全站状态
	{
		checkResult = true;
	}
	else
	{
		ShowTipMsg(L"接收数据类型错误" + data);
	}
	return checkResult;
}


void CTCPSocket::sendACK(char* data,int len)
{
	CString sendstr(L"ACK::");
	
	char mybuff[1024];
	memset(mybuff,0,sizeof(mybuff));

	int bufflen = turnCStrToStr(sendstr, mybuff);

	memcpy(&mybuff[bufflen], data,len);
	bufflen += len;
	sendData(mybuff,bufflen);
}


void CTCPSocket::setAtpSectionState(CString eleIndex, CString state)
{
	CADDevice* pDev;
	pDev = pDoc->findDevcieByEleIndex(eleIndex);
	if (pDev==NULL )
	{
		pDev = pDoc->findSwitchBySectEleIndex(eleIndex);
	}

	if (pDev!=NULL)
	{
		if (state ==L"86837")//空闲
		{
			pDev->vRelay_[0]->setStat(TRUE,true);
		}
		else if (state == L"86838")//占用
		{
			pDev->vRelay_[0]->setStat(FALSE, true);
		}
		else
		{
			ShowTipMsg(L"解析区段状态失败" + state);
		}
	}
	else
	{
		ShowTipMsg(L"查找区段失败" + eleIndex);
	}
}
void CTCPSocket::setAtpPsdState(CString eleIndex, CString state)
{
	CADDevice* pDev = pDoc->findDevcieByEleIndex(eleIndex);
	if (pDev != NULL)
	{
		if (state == L"91136")//开
		{
			pDev->vRelay_[0]->setStat(FALSE, true);
		}
		else if (state == L"91137")//关
		{
			pDev->vRelay_[0]->setStat(TRUE, true);
		}
		else if (state == L"0")//未定义状态--设置为关
		{
			pDev->vRelay_[0]->setStat(TRUE, true);
		}
		else
		{
			ShowTipMsg(L"解析PSD状态失败" + state);
		}
	}
	else
	{

	}
}
void CTCPSocket::setAtpEmpState(CString eleIndex, CString state)
{
	CADDevice* pDev = pDoc->findDevcieByEleIndex(eleIndex);
	if (pDev != NULL)
	{
		if (state == L"91236")//按下
		{
			pDev->vRelay_[0]->setStat(FALSE, true);
		}
		else if (state == L"91237")//未按下
		{
			pDev->vRelay_[0]->setStat(TRUE, true);
		}
		else if (state == L"0")//未定义状态--设置为关
		{
			pDev->vRelay_[0]->setStat(FALSE, true);
		}
		else
		{
			ShowTipMsg(L"解析EMP状态失败" + state);
		}
	}
	else
	{
		ShowTipMsg(L"查找EMP设备失败" + eleIndex);
	}
}
void CTCPSocket::setAtpDrbState(CString eleIndex, CString state)
{
	CADDevice* pDev = pDoc->findDevcieByEleIndex(eleIndex);
	if (pDev != NULL)
	{
		if (state == L"91336")//按下
		{
			pDev->vRelay_[0]->setStat(TRUE, true);
		}
		else if (state == L"91337")//未按下
		{
			pDev->vRelay_[0]->setStat(FALSE, true);
		}
		else if (state == L"0")//未定义状态--设置为关
		{
			pDev->vRelay_[0]->setStat(FALSE, true);
		}
		else
		{
			ShowTipMsg(L"解析DRB状态失败" + state);
		}
	}
	else
	{
		ShowTipMsg(L"查找DRB设备失败" + eleIndex);
	}
}

CString CTCPSocket::GetSignalState(int color, bool isDDstate)
{
	CString State = L"85637";
	if (isDDstate)
	{
		 State = L"85641";
		 return State;
	}
	
	switch (color)
	{
	case CADSignal::SIGNAL_RED:
	case CADSignal::SIGNAL_BLUE:
	case CADSignal::SIGNAL_DS:
	case CADSignal::SIGNAL_2DS:
		State = L"85637";
		break;
	case CADSignal::SIGNAL_YELLOW:
		State = L"85639";
		break;
	case CADSignal::SIGNAL_GREEN:
		
	case CADSignal::SIGNAL_WHITE:
				
	case CADSignal::SIGNAL_GREEN_YELLOW:
		
	case CADSignal::SIGNAL_DOUBLE_YELLOW:
		
	case CADSignal::SIGNAL_DOUBLE_GREEN:

	case CADSignal::SIGNAL_CYAN:
		State = L"85636";
		break;
	case CADSignal::SIGNAL_RED_YELLOW:
	case CADSignal::SIGNAL_RED_WHITE:
		State = L"85640";
		break;
	default:
		State = L"85637";
		ShowTipMsg(L"获取信号机状态失败");
		break;
	}
	return State;
}


CString CTCPSocket::getSwitchPos(int pos)
{
	CString strPos;
	switch (pos)
	{
	case Switch_Trailed:
		strPos = L"0";
		break;
	case Switch_Left:
		strPos = L"85549";
		break;
	case Switch_Right:
		strPos = L"85550";
		break;
	default:
		strPos = L"0";
		ShowTipMsg(L"打包道岔状态失败");
		break;
	}

	return strPos ;
}


int CTCPSocket::packA2Map(CString eleType, CString eleldx, CString state,char* buff)
{
	CString sendstr(L"A2Map:ZST:");
	sendstr = sendstr + IXL_id + L"_" + eleType + L"_" + eleldx + L"," + state;

	char mybuff[1024];
	int bufflen = turnCStrToStr(sendstr, mybuff);

	mybuff[bufflen] = 0x0a;//结束符
	bufflen++;

	memcpy(buff,mybuff,bufflen);

	return bufflen;
}


int CTCPSocket::turnCStrToStr(CString str, char* buff)
{
	char sendData[1024];
	CStringA dataA(str);
	strcpy_s(sendData, dataA);

	memcpy(buff,sendData,str.GetLength());

	return str.GetLength();
}

void CTCPSocket::OnTimer()
{
	//设备
	for each (CADDevice* pDevice in pDoc->vDevice_)
	{
		if (pDevice->IsKindOf(RUNTIME_CLASS(RailSwitch)))
		{
			RailSwitch* pSwitch = dynamic_cast<RailSwitch*>(pDevice);
			//道岔
			if (pSwitch->checkChange())
			{
				sendSwitchPos(pSwitch);
			}
			//道岔区段
			if (pSwitch->checkSwitchSectionChange())
			{
				sendSwitchSectionState(pSwitch);
			}
		}
		else if (pDevice->IsKindOf(RUNTIME_CLASS(CADSignal)))
		{
			CADSignal* pSignal = dynamic_cast<CADSignal*>(pDevice);
			if (pSignal->checkChange())
			{
				sendSignalState(pSignal);
			}
		}
		else if (pDevice->IsKindOf(RUNTIME_CLASS(CADSection)))
		{
			CADSection *pSection = dynamic_cast<CADSection*>(pDevice);
			if (pSection->checkChange())
			{
				sendSectionState(pSection);
			}
		}
	}

	//零散
	for each(CADDevice *pDevice in pDoc->vLingSanDev)
	{
		if (pDevice->eleType == L"25501")
		{
			if (pDevice->IsKindOf(RUNTIME_CLASS(CADDoor)))
			{
				CADDoor* pDoor = dynamic_cast<CADDoor*>(pDevice);
				if (pDoor->checkChange())
				{
					sendPsdState(pDoor);
				}
				
			}
		}
		else if (pDevice->eleType == L"25502")
		{
			if (pDevice->IsKindOf(RUNTIME_CLASS(CADButton)))
			{
				CADButton* pBtn = dynamic_cast<CADButton*>(pDevice);
				if (pBtn->chackChange())
				{
					sendEMPState(pBtn);
				}
				
			}
			else if (pDevice->IsKindOf(RUNTIME_CLASS(CADAfgDoor)))
			{
				CADAfgDoor* pAfg = dynamic_cast<CADAfgDoor*>(pDevice);
				sendAFGState(pAfg);
			}
			else
			{
			}
		}
		else if (pDevice->eleType == L"25503")
		{
			if (pDevice->IsKindOf(RUNTIME_CLASS(CADDeLight)))
			{
				CADDeLight* pLight = dynamic_cast<CADDeLight*>(pDevice);
				if (pLight->checkChange())
				{
					sendDTRBState(pLight);
				}
				
			}
		}

	}
}


void CTCPSocket::sendData(char* buff, int bufflen)
{
	int nSendState=Send(buff,bufflen);
	
	CString dataA(buff);
	dataA.Format(L"发送数据："+dataA+L"\r\n发送返回值 %d",buff, nSendState);

	ShowTipMsg(dataA);

}


CString CTCPSocket::getSectionState(bool isOccupy)
{
	CString strOccupy(L"");
	if (isOccupy)
	{
		strOccupy = L"86838";
	}
	else
	{
		strOccupy = L"86837";
	}
	return strOccupy;
}


void CTCPSocket::sendLingSanState()
{
	for each(CADDevice* pDevice in pDoc->vLingSanDev)
	{
		if (pDevice->eleType == L"25501")
		{
			if (pDevice->IsKindOf(RUNTIME_CLASS(CADDoor)))
			{
				CADDoor* pDoor = dynamic_cast<CADDoor*>(pDevice);
				sendPsdState(pDoor);
			}
		}
		else if (pDevice->eleType == L"25502")
		{
			if (pDevice->IsKindOf(RUNTIME_CLASS(CADButton)))
			{
				CADButton* pBtn = dynamic_cast<CADButton*>(pDevice);
				sendEMPState(pBtn);
			}
			else if (pDevice->IsKindOf(RUNTIME_CLASS(CADAfgDoor)))
			{
				CADAfgDoor* pAfg = dynamic_cast<CADAfgDoor*>(pDevice);
				sendAFGState(pAfg);
			}
			else
			{
			}
		}
		else if (pDevice->eleType == L"25503")
		{
			if (pDevice->IsKindOf(RUNTIME_CLASS(CADDeLight)))
			{
				CADDeLight* pBtn = dynamic_cast<CADDeLight*>(pDevice);
				sendDTRBState(pBtn);
			}
		}
		else
		{

		}
	}

}


CString CTCPSocket::getPsdState(bool isOpen)
{
	CString strState(L"");
	if (isOpen)
	{
		strState = L"91136";
	}
	else
	{
		strState = L"91137";
	}
	return strState;
}


CString CTCPSocket::getEMPState(bool isEMPDown)
{
	CString strState(L"");
	if (isEMPDown)
	{
		strState = L"91236";
	}
	else
	{
		strState = L"91237";
	}
	return strState;
}


CString CTCPSocket::getDTRBState(bool isDTRBDown)
{
	CString strState(L"");
	if (isDTRBDown)
	{
		strState = L"91336";
	}
	else
	{
		strState = L"91337";
	}
	return strState;
}


void CTCPSocket::sendSwitchPos(RailSwitch* pSwitch)
{
	CString strpos = getSwitchPos(pSwitch->GetSwitchWCUPos());
	if (pSwitch->eleIndex.GetLength()>0)
	{
		char sendBuff[1024];
		memset(sendBuff, 0, sizeof(sendBuff));
		int sendBufflen = packA2Map(pSwitch->eleType, pSwitch->eleIndex, strpos, sendBuff);
		sendData(sendBuff, sendBufflen);
	}
}


void CTCPSocket::sendSignalState(CADSignal* pSignal)
{
	CString strcolor = GetSignalState(pSignal->getSignalState(), pSignal->getDDJstate());
	if (pSignal->eleIndex.GetLength()>0)
	{
		char sendBuff[1024];
		memset(sendBuff, 0, sizeof(sendBuff));
		int sendBufflen = packA2Map(pSignal->eleType, pSignal->eleIndex, strcolor, sendBuff);
		sendData(sendBuff, sendBufflen);
	}
}


void CTCPSocket::sendSwitchSectionState(RailSwitch* pSwitch)
{
	if (pSwitch->switchSectionEleIndex.GetLength()>0)
	{
		char sendBuff[1024];
		CString strSectionState = getSectionState(pSwitch->getOccupyStatus());
		memset(sendBuff, 0, sizeof(sendBuff));
		int sendBufflen = packA2Map(pSwitch->switchSectionEleType, pSwitch->switchSectionEleIndex, strSectionState, sendBuff);
		sendData(sendBuff, sendBufflen);
	}
}


void CTCPSocket::sendSectionState(CADSection* pSection)
{
	CString strSectionState = getSectionState(pSection->getOccupyStatus());
	if (pSection->eleIndex != L"")
	{
		char sendBuff[1024];
		memset(sendBuff, 0, sizeof(sendBuff));
		int sendBufflen = packA2Map(pSection->eleType, pSection->eleIndex, strSectionState, sendBuff);
		sendData(sendBuff, sendBufflen);
	}
}


void CTCPSocket::sendPsdState(CADDoor* pDoor)
{
	CString strState = getPsdState(pDoor->isPsdOpen());
	char sendBuff[1024];
	memset(sendBuff, 0, sizeof(sendBuff));
	int sendBufflen = packA2Map(pDoor->eleType, pDoor->eleIndex, strState, sendBuff);
	sendData(sendBuff, sendBufflen);
}


void CTCPSocket::sendEMPState(CADButton* pBtn)
{
	CString strState = getEMPState(pBtn->isPressed());
	char sendBuff[1024];
	memset(sendBuff, 0, sizeof(sendBuff));
	int sendBufflen = packA2Map(pBtn->eleType, pBtn->eleIndex, strState, sendBuff);
	sendData(sendBuff, sendBufflen);
}
void CTCPSocket::sendAFGState(CADAfgDoor* pAfg)
{
	if (pAfg->vRelay_.size() > 0)
	{
		bool isPress= pAfg->isRelayDown(0);
		CString strState = getEMPState(isPress);
		char sendBuff[1024];
		memset(sendBuff, 0, sizeof(sendBuff));
		int sendBufflen = packA2Map(pAfg->eleType, pAfg->eleIndex, strState, sendBuff);
		sendData(sendBuff, sendBufflen);
	}
	
}


void CTCPSocket::sendDTRBState(CADDeLight* pLight)
{
	CString strState = getDTRBState(pLight->isPressed());
	char sendBuff[1024];
	memset(sendBuff, 0, sizeof(sendBuff));
	int sendBufflen = packA2Map(pLight->eleType, pLight->eleIndex, strState, sendBuff);
	sendData(sendBuff, sendBufflen);
}


void CTCPSocket::setIXLId(int numb)
{
	CString strID;
	strID.Format(L"%d", numb);

	IXL_id = strID;
}
