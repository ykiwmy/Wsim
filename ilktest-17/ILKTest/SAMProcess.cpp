#include "stdafx.h"
#include "SAMProcess.h"
#include "ILKGraphic\CADDevice.h"
#include "ILKGraphic\CADSection.h"
#include "ILKGraphic\RailSwitch.h"
#include "ILKGraphic\CADSignal.h"
#include "ILKGraphic\CADDoor.h"
#include "ILKGraphic\CADButton.h"
#include "ILKGraphic\CADDelight.h"
#include "ILKTestDoc.h"
#include "Relay.h"
CSAMProcess::CSAMProcess(CILKTestDoc* pDoc, int ciId)
: m_myId(ciId)
, pDoc_(pDoc)
{

}


CSAMProcess::~CSAMProcess()
{

}


void CSAMProcess::SetRcvData(const char* rcvBuff, int buffLen)
{
	memset(&m_rcvData, 0, sizeof(m_rcvData));

	int len = 0;
	memcpy(&m_rcvData, rcvBuff,4);
	len += 4;
	//memcpy(&m_rcvData.Phy_Numb,&rcvBuff[len],2);
	m_rcvData.Phy_Numb = pDoc_->GetUINT16ByBigData(&rcvBuff[len]);
	len += 2;

	int devDatalen= 3 /*sizeof(Dev_Data_3M)*/;
	for (int i = 0; i < m_rcvData.Phy_Numb;i++)
	{
		//memcpy(&m_rcvData.phy_data[i], &rcvBuff[len], devDatalen);
		m_rcvData.phy_data[i] = GetDevData(&rcvBuff[len]);
		len += devDatalen;
	}
	
	//memcpy(&m_rcvData.Signal_Numb, &rcvBuff[len], 2);
	m_rcvData.Signal_Numb = pDoc_->GetUINT16ByBigData(&rcvBuff[len]);
	len += 2;
	for (int i = 0; i < m_rcvData.Signal_Numb; i++)
	{
		//memcpy(&m_rcvData.signal_data[i], &rcvBuff[len], devDatalen);
		m_rcvData.signal_data[i] = GetDevData(&rcvBuff[len]);
		len += devDatalen;
	}

	//memcpy(&m_rcvData.Switch_Numb, &rcvBuff[len], 2);
	m_rcvData.Switch_Numb = pDoc_->GetUINT16ByBigData(&rcvBuff[len]);
	len += 2;
	for (int i = 0; i < m_rcvData.Switch_Numb; i++)
	{
		//memcpy(&m_rcvData.switch_data[i], &rcvBuff[len], devDatalen);
		m_rcvData.switch_data[i] = GetDevData(&rcvBuff[len]);
		len += devDatalen;
	}

	//memcpy(&m_rcvData.Psd_Numb, &rcvBuff[len], 2);
	m_rcvData.Psd_Numb = pDoc_->GetUINT16ByBigData(&rcvBuff[len]);
	len += 2;
	for (int i = 0; i < m_rcvData.Psd_Numb; i++)
	{
		//memcpy(&m_rcvData.psd_data[i], &rcvBuff[len], 4/*sizeof(Dev_Data2_3M)*/); //两种状态
		//len ++/*sizeof(Dev_Data2_3M)*/
		m_rcvData.psd_data[i].Dev_ID = pDoc_->GetUINT16ByBigData(&rcvBuff[len]);
		len += 2;
		m_rcvData.psd_data[i].Dev_Status1 = rcvBuff[len];
		len++;
		m_rcvData.psd_data[i].Dev_Status2 = rcvBuff[len];
		len ++;
	}

	//memcpy(&m_rcvData.Emp_Numb, &rcvBuff[len], 2);
	m_rcvData.Emp_Numb = pDoc_->GetUINT16ByBigData(&rcvBuff[len]);
	len += 2;
	for (int i = 0; i < m_rcvData.Emp_Numb; i++)
	{
		//memcpy(&m_rcvData.emp_data[i], &rcvBuff[len], devDatalen);
		m_rcvData.emp_data[i] = GetDevData(&rcvBuff[len]);
		len += devDatalen;
	}

	//memcpy(&m_rcvData.Drb_Numb, &rcvBuff[len], 2);
	m_rcvData.Drb_Numb = pDoc_->GetUINT16ByBigData(&rcvBuff[len]);
	len += 2;
	for (int i = 0; i < m_rcvData.Drb_Numb; i++)
	{
		//memcpy(&m_rcvData.drb_data[i], &rcvBuff[len], devDatalen);
		m_rcvData.drb_data[i] = GetDevData(&rcvBuff[len]);
		len += devDatalen;
	}
	ProcessRcvData();
}


void CSAMProcess::ProcessRcvData()
{
	for (int i = 0; i < m_rcvData.Phy_Numb;i++)
	{
		SetSectionState(m_rcvData.phy_data[i]);
	}
	/*for (int i = 0; i < m_rcvData.Switch_Numb; i++)
	{
	SetSwitchState(m_rcvData.switch_data[i]);
	}*/

	/*for (int i = 0; i < m_rcvData.Psd_Numb; i++)
	{
	SetPsdState(m_rcvData.psd_data[i]);
	}*/

	for (int i = 0; i < m_rcvData.Emp_Numb; i++)
	{
		SetEmpState(m_rcvData.emp_data[i]);
	}

	for (int i = 0; i < m_rcvData.Drb_Numb; i++)
	{
		SetDrbState(m_rcvData.drb_data[i]);
	}
}

void  CSAMProcess::SetSectionState(Dev_Data_3M data)
{
	CADDevice* pDev=NULL;
	pDev = pDoc_->findDevcieByDevID_3M(data.Dev_ID, RUNTIME_CLASS(CADSection));
	if (pDev == NULL)
	{
		pDev = pDoc_->findSwitchBySectID_3M(data.Dev_ID);
		if (RailSwitch* pSwit = dynamic_cast<RailSwitch*> (pDev))
		{
			if (data.Dev_Status == CLEAR_3M)//空闲
			{
				pSwit->setSectionState(false);
				//pDev->vRelay_[0]->setStat(TRUE, true);
			}
			else if (data.Dev_Status == OCCUPY_3M)//占用
			{
				//pDev->vRelay_[0]->setStat(FALSE, true);
				pSwit->setSectionState(true);
			}
			else
			{
				TRACE(L"解析道岔区段状态失败" + pDev->io_.getName());
			}

		}
	}
	else if (CADSection* pSect = dynamic_cast<CADSection*> (pDev))
	{
		if (data.Dev_Status == CLEAR_3M)//空闲
		{
			pSect->setSectionState(false);
			//pDev->vRelay_[0]->setStat(TRUE, true);
		}
		else if (data.Dev_Status == OCCUPY_3M)//占用
		{
			pSect->setSectionState(true);
			//pDev->vRelay_[0]->setStat(FALSE, true);
		}
		else
		{
			TRACE(L"解析区段状态失败" + pDev->io_.getName());
		}
		
	}
	else
	{
		TRACE(L"查找区段失败");
	}
}
void  CSAMProcess::SetSwitchState(Dev_Data_3M data)
{
	CADDevice* pDev = NULL;
	pDev = pDoc_->findDevcieByDevID_3M(data.Dev_ID, RUNTIME_CLASS(RailSwitch));
	RailSwitch* pSwitch = dynamic_cast<RailSwitch*> (pDev);
	if (pSwitch != NULL)
	{
		if (data.Dev_Status == NORMAL_3M)//空闲
		{
			pSwitch->setDPos();
		}
		else if (data.Dev_Status == REVERSE_3M)//占用
		{
			pSwitch->setFPos();
		}
		else if (data.Dev_Status == NOSTATE_3M)
		{
			pSwitch->setUnPos();
		}
		else
		{
			TRACE(L"解析区段状态失败" + pDev->io_.getName());
		}
	}
	else
	{
		TRACE(L"查找区段失败");
	}
}

void  CSAMProcess::SetPsdState(Dev_Data2_3M data)
{
	CADDevice* pDev = pDoc_->findDevcieByDevID_3M(data.Dev_ID, RUNTIME_CLASS(CADDoor));
	if (pDev != NULL)
	{
		if (data.Dev_Status1 == PSDOPEN_3M)//开
		{
			pDev->vRelay_[0]->setStat(FALSE, true);
		}
		else if (data.Dev_Status1 == PSDCLOSED_3M)//关
		{
			pDev->vRelay_[0]->setStat(TRUE, true);
		}
		else
		{
			TRACE(L"解析PSD状态1失败" + pDev->io_.getName());
		}

		if (data.Dev_Status2 == RPSDNLOCK_3M)//未解除
		{
			pDev->vRelay_[1]->setStat(FALSE, true);
		}
		else if (data.Dev_Status2 == RPSDLOCK_3M)//已互锁解除
		{
			pDev->vRelay_[1]->setStat(TRUE, true);
		}
		else
		{
			TRACE(L"解析PSD状态2失败" + pDev->io_.getName());
		}
	}
	else
	{
		TRACE(L"查找PSD失败");
	}
}

void  CSAMProcess::SetEmpState(Dev_Data_3M data)
{
	CADDevice* pDev = pDoc_->findDevcieByDevID_3M(data.Dev_ID,RUNTIME_CLASS(CADButton));
	if (pDev != NULL)
	{
		if (data.Dev_Status == EMPDOWN_3M)//未按下
		{
			pDev->vRelay_[0]->setStat(FALSE, true);
		}
		else if (data.Dev_Status == EMPUP_3M)//按下
		{
			pDev->vRelay_[0]->setStat(TRUE, true);
		}
		else
		{
			TRACE(L"解析EMP状态失败" + pDev->io_.getName());
		}
	}
	else
	{
		TRACE(L"查找EMP设备失败");
	}
}

void CSAMProcess::SetDrbState(Dev_Data_3M data)
{
	for each (CADDevice* pDevice in pDoc_->vLingSanDev)
	{
		if (pDevice->devType_3M == DEV_TYPE_ATB_STATE)
		{
			if (pDevice->IsKindOf(RUNTIME_CLASS(CADDeLight)))
			{
				CADDeLight* pLight = dynamic_cast<CADDeLight*>(pDevice);
				if (pLight->isDrb())
				{
					if (data.Dev_Status == DRBDOWN_3M)//按下
					{
						pLight->vRelay_[0]->setStat(TRUE, true);
					}
					else if (data.Dev_Status == DRBUP_3M)//未按下
					{
						pLight->vRelay_[0]->setStat(FALSE, true);
					}
					else
					{
						TRACE(L"解析DRB状态失败" + pLight->io_.getName());
					}
				}

			}
		}
	}
}


void CSAMProcess::SetSwitchStatus(RailSwitch* pSwitch)
{
	m_sndData.switch_data[m_sndData.Switch_Numb].Dev_ID=pSwitch->devId_3M;

	int stat = pSwitch->GetSwitchILPos();
	UINT8 status = NOSTATE_3M; 
	switch (stat)
	{
	case 0:
		// 四开
		status = NOSTATE_3M;
		break;
	case 1:
		// 定位
		status = NORMAL_3M;
		break;
	case 2:
		// 反位
		status = REVERSE_3M;
		break;
	case 3:
		// 故障
		status = NOSTATE_3M;
		break;
	default:
		//
		status = NOSTATE_3M;
		break;
	}

	m_sndData.switch_data[m_sndData.Switch_Numb].Dev_Status = status;
	m_sndData.Switch_Numb++;
}

void CSAMProcess::SetSwitchSectionStatus(RailSwitch* pSwitch)
{
	m_sndData.phy_data[m_sndData.Phy_Numb].Dev_ID = pSwitch->switchSectionID_3M;

	int stat = pSwitch->getOccupyStatus();
	UINT8 status = CLEAR_3M;
	if (pSwitch->getOccupyStatus())
	{
		status = OCCUPY_3M;
	}

	m_sndData.phy_data[m_sndData.Phy_Numb].Dev_Status = status;
	m_sndData.Phy_Numb++;
}

void CSAMProcess::SetSectionStatus(CADSection* pSect)
{
	m_sndData.phy_data[m_sndData.Phy_Numb].Dev_ID = pSect->devId_3M;

	int stat = pSect->getOccupyStatus();
	UINT8 status = CLEAR_3M;
	if (pSect->getOccupyStatus())
	{
		status = OCCUPY_3M;
	}

	m_sndData.phy_data[m_sndData.Phy_Numb].Dev_Status = status;
	m_sndData.Phy_Numb++;
}

void CSAMProcess::SetSignalStatus(CADSignal* pSignal)
{
	m_sndData.signal_data[m_sndData.Signal_Numb].Dev_ID = pSignal->devId_3M;

	int color = pSignal->getSignalState();
	bool isCBTC=pSignal->getDDJstate();
	UINT8 status = CLEAR_3M;
	switch (color)
	{
	case CADSignal::SIGNAL_DS:
		status = RED_D_3M;
		break;
	case CADSignal::SIGNAL_RED:
		if (isCBTC)
		{
			status = RED_C_3M;
		}
		else
		{
			status = RED_3M;
		}
		break;
	case CADSignal::SIGNAL_YELLOW:
		if (isCBTC)
		{
			status = YELLOW_C_3M;
		}
		else
		{
			status = YELLOW_3M;
		}
		break;
	case CADSignal::SIGNAL_GREEN:
		if (isCBTC)
		{
			status = GREEN_C_3M;
		}
		else
		{
			status = GREEN_3M;
		}
		break;
	case CADSignal::SIGNAL_WHITE:
		status = WHITE_3M;
		break;
	case CADSignal::SIGNAL_RED_YELLOW:
		status = REDYELLOW_3M;
		break;
	default:
		status = RED_3M;
		TRACE(L"获取信号机状态失败");
		break;
	}

	m_sndData.signal_data[m_sndData.Signal_Numb].Dev_Status = status;
	m_sndData.Signal_Numb++;
}

void CSAMProcess::SetPsdStatus(CADDoor* pDoor)
{
	m_sndData.psd_data[m_sndData.Psd_Numb].Dev_ID = pDoor->devId_3M;

	UINT8 status = PSDCLOSED_3M;
	if (pDoor->isPsdOpen())
	{
		status = PSDOPEN_3M;
	}

	m_sndData.psd_data[m_sndData.Psd_Numb].Dev_Status = status;
	m_sndData.Psd_Numb++;
}

void CSAMProcess::SetDRIStatus(CADDeLight* pLight)
{
	m_sndData.drl_data[m_sndData.Drl_Numb].Dev_ID = pLight->devId_3M;

	UINT8 status = DRLNS_3M;
	if (pLight->isSDState())
	{
		status = DRLS_3M;
	}
	m_sndData.drl_data[m_sndData.Drl_Numb].Dev_Status1 = status;

	status = DRLNW_3M;
	if (pLight->isWDState())
	{
		status = DRLW_3M;
	}
	m_sndData.drl_data[m_sndData.Drl_Numb].Dev_Status2 = status;

	m_sndData.Drl_Numb++;
}
void CSAMProcess::ProcessSendData()
{
	memset(&m_sndData, 0, sizeof(CI2TSData));

	m_sndData.nCI_Type = 0x3C;
	m_sndData.nCI_ID = m_myId;
	m_sndData.nTs_Type = 0x7a;
	m_sndData.nTs_ID = 0x01;


	for each (CADDevice* pDevice in pDoc_->vDevice_)
	{
		if (pDevice->IsKindOf(RUNTIME_CLASS(CADSection)))
		{
			CADSection *pSection = dynamic_cast<CADSection*>(pDevice);
			if (pSection->devType_3M == DEV_TYPE_PHY_STATE)
			{
				SetSectionStatus(pSection);
			}
		}
		else if (pDevice->IsKindOf(RUNTIME_CLASS(RailSwitch)))
		{
			RailSwitch* pSwitch = dynamic_cast<RailSwitch*>(pDevice);
			//道岔
			if (pSwitch->devType_3M == DEV_TYPE_SWITCH_STATE)
			{
				SetSwitchStatus(pSwitch);

			}
			//道岔区段
			if (pSwitch->switchSectonType_3M == DEV_TYPE_PHY_STATE)
			{
				SetSwitchSectionStatus(pSwitch);
			}
		}
		else if (pDevice->IsKindOf(RUNTIME_CLASS(CADSignal)))
		{
			CADSignal* pSignal = dynamic_cast<CADSignal*>(pDevice);
			if (pSignal->devType_3M == DEV_TYPE_SIGN_STATE)
			{
				SetSignalStatus(pSignal);
			}
		}
	}

	//零散
	for each(CADDevice *pDevice in pDoc_->vLingSanDev)
	{
		if (pDevice->devType_3M == DEV_TYPE_PSD_STATE)
		{
			if (pDevice->IsKindOf(RUNTIME_CLASS(CADDoor)))
			{
				CADDoor* pDoor = dynamic_cast<CADDoor*>(pDevice);
				SetPsdStatus(pDoor);
			}
		}
		else if (pDevice->devType_3M == DEV_TYPE_ATB_STATE)
		{
			if (pDevice->IsKindOf(RUNTIME_CLASS(CADDeLight)))
			{
				CADDeLight* pLight = dynamic_cast<CADDeLight*>(pDevice);
				if (pLight->isDri())
				{
					SetDRIStatus(pLight);
				}
				
			}
		}
		else
		{

		}
	}
}

void CSAMProcess::PackSendData(char* sendBuff, int* sendBuffLen)
{
	char buff[2048];
	memset(buff, 0, 2048);
	int bufflen = 0;

	//准备数据
	ProcessSendData();

	//打包
	memcpy(buff, &m_sndData, 4);
	bufflen += 4;

	int devDatalen = 3 /*sizeof(Dev_Data_3M)*/;

	/*memcpy(&buff[bufflen], &m_sndData.Phy_Numb, 2);
	bufflen += 2;
	for (int i = 0; i < m_sndData.Phy_Numb; i++)
	{
	memcpy(&buff[bufflen], &m_sndData.phy_data[i], devDatalen);
	bufflen += devDatalen;
	}*/  //区段只收不发

	UINT16 tempPhy_Numb = 0;
	//memcpy(&buff[bufflen], &tempPhy_Numb, 2);
	pDoc_->GetBigDataByUINT16(tempPhy_Numb, &buff[bufflen]);
	bufflen += 2;

	//memcpy(&buff[bufflen], &m_sndData.Signal_Numb, 2);
	pDoc_->GetBigDataByUINT16(m_sndData.Signal_Numb, &buff[bufflen]);
	bufflen += 2;

	for (int i = 0; i < m_sndData.Signal_Numb; i++)
	{
		//memcpy(&buff[bufflen], &m_sndData.signal_data[i], devDatalen);
		SetDevData(m_sndData.signal_data[i], &buff[bufflen]);
		bufflen += devDatalen;
	}

	//memcpy(&buff[bufflen],&m_sndData.Switch_Numb, 2);
	pDoc_->GetBigDataByUINT16(m_sndData.Switch_Numb, &buff[bufflen]);
	bufflen += 2;
	for (int i = 0; i < m_sndData.Switch_Numb; i++)
	{
		//memcpy(&buff[bufflen],&m_sndData.switch_data[i], devDatalen);
		SetDevData(m_sndData.switch_data[i], &buff[bufflen]);
		bufflen += devDatalen;
	}

	//memcpy(&buff[bufflen], &m_sndData.Psd_Numb, 2);
	pDoc_->GetBigDataByUINT16(m_sndData.Psd_Numb, &buff[bufflen]);
	bufflen += 2;
	for (int i = 0; i < m_sndData.Psd_Numb; i++)
	{
		//memcpy(&buff[bufflen], &m_sndData.psd_data[i], devDatalen); //
		SetDevData(m_sndData.psd_data[i], &buff[bufflen]);
		bufflen += devDatalen;
	}

	//memcpy(&buff[bufflen], &m_sndData.Drl_Numb, 2);
	pDoc_->GetBigDataByUINT16(m_sndData.Drl_Numb, &buff[bufflen]);
	bufflen += 2;
	for (int i = 0; i < m_sndData.Drl_Numb; i++)
	{
		//memcpy(&buff[bufflen], &m_sndData.drl_data[i],4 /*sizeof(Dev_Data2_3M)*/);
		//bufflen += 4 /*sizeof(Dev_Data2_3M)*/;

		pDoc_->GetBigDataByUINT16(m_sndData.drl_data[i].Dev_ID, &buff[bufflen]);
		bufflen += 2;
		memcpy(&buff[bufflen], &m_sndData.drl_data[i].Dev_Status1, 1);
		bufflen++;
		memcpy(&buff[bufflen], &m_sndData.drl_data[i].Dev_Status2, 1);
		bufflen++;
		
	}

	memcpy(sendBuff,buff,bufflen);
	*sendBuffLen = bufflen;
}


Dev_Data_3M CSAMProcess::GetDevData(const char* inBuff)
{
	Dev_Data_3M  data;
	memset(&data, 0, 3);
	data.Dev_ID = pDoc_->GetUINT16ByBigData(inBuff);
	data.Dev_Status = *(inBuff + 2);
	return data;
}

void CSAMProcess::SetDevData(Dev_Data_3M data,char* inBuff)
{
	pDoc_->GetBigDataByUINT16(data.Dev_ID, inBuff);
	*(inBuff + 2) = data.Dev_Status;
}