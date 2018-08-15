
// ILKTestDoc.cpp : CILKTestDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "ILKTest.h"
#endif

#include "ILKTestDoc.h"
#include "ILKGraphic\CADGraphic.h"
#include "ILKGraphic\CADDevice.h"
#include "ILKGraphic\CADSection.h"
#include "ILKGraphic\RailSwitch.h"
#include "ILKGraphic\CADSignal.h"
#include "ILKGraphic\LingSan.h"
#include "ILKGraphic\CADStation.h"
#include "ILKGraphic\CADDeLight.h"
#include "ILKGraphic\CADAfgDoor.h"
#include "Relay.h"
#include "ZcCommRelay.h"
#include "BaseRoute.h"
#include "SwitchSection.h"
#include "IOView.h"
#include "IOCaptionWnd.h"
#include "MainFrm.h"
#include "SplitterFrame.h"
#include <propkey.h>
#include "SAMProcess.h"

#include <fstream>
using namespace std;

extern "C"{
#include "RSSP\RSSP1_Interface.h"
#include "RSSP\Rsp1.h"
#include "RSSP\GL_Basic_Type_Definition.h"
#include "RSSP\Vcp_File_Interface.h"
#include "RSSP\Vcp_Public_Definition.h"
#include "RSSP\Vcp_Service_Interface.h"
#include "RSSP\Vcp_Service_TestInterface.h"
#include "RSSP\Vcp_Track_Interface.h"
#include "RSSP\Vcp_UsrProcess_Entrance.h"
}


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define  BOARDDISTENCE 21
// CILKTestDoc

IMPLEMENT_DYNCREATE(CILKTestDoc, CDocument)

BEGIN_MESSAGE_MAP(CILKTestDoc, CDocument)

	ON_COMMAND(ID_MENU_ATP, &CILKTestDoc::OnMenuAtpShow)
	ON_COMMAND(ID_ALLNORPOS, &CILKTestDoc::OnAllNorPos)
	ON_COMMAND(ID_ALLREVPOS, &CILKTestDoc::OnAllRevPos)
	ON_COMMAND(ID_ALL4KAIPOS, &CILKTestDoc::OnAll4KaiPos)
	ON_COMMAND(ID_ALLOCPPOS, &CILKTestDoc::OnAllOcpState)
	ON_COMMAND(ID_ALLCLEARPOS, &CILKTestDoc::OnAllClearOcpState)
	ON_COMMAND(ID_ALLPREPOS, &CILKTestDoc::OnAllPrePos)
	ON_COMMAND(ID_ALLDS, &CILKTestDoc::OnAllDS)
	ON_COMMAND(ID_ALLCLEARDS, &CILKTestDoc::OnAllClearDS)
	ON_COMMAND(ID_ALLCLEARPRE, &CILKTestDoc::OnAllClearPreReset)
	ON_COMMAND(ID_ALLLIGHTOFF, &CILKTestDoc::OnAllLightOFF)
	ON_COMMAND(ID_ALLLIGHTON, &CILKTestDoc::OnAllLightON)
	ON_COMMAND(ID_ALLLTE_AT, &CILKTestDoc::OnAllLTEAT)
	ON_COMMAND(ID_ALLLTE_CLEAR, &CILKTestDoc::OnAllLTEClear)
	ON_COMMAND(ID_ALLLTE_EEROR, &CILKTestDoc::OnAllLTEEeror)
	ON_COMMAND(ID_ALLLTE_UT, &CILKTestDoc::OnAllLTEUT)
	ON_COMMAND(ID_DISCONN_TO_ZC, &CILKTestDoc::OnDisConnToZc)
	ON_UPDATE_COMMAND_UI(ID_DISCONN_TO_ZC, &CILKTestDoc::OnUpdateDisConnToZc)
END_MESSAGE_MAP()


// CILKTestDoc ����/����

CILKTestDoc::CILKTestDoc()
: ioSock_(this)
, nElementTotal(0)
, driveTimeCount(30)// 100һ�����ڣ�3��
, m_tcpSevSock_(this)
, strPath(_T(""))
, isFirstOpenFile(TRUE)
, m_isLogicSection(0)
, ixlIDForGalaxy(0)
, ciIdWith3M(0)
, m_3MProcess(NULL)
, m_isCnctToSAM(0)
, m_isCnctToGalaxy(0)
, m_iIsCnctToSamZC(0)
, m_bIsCommToZC(TRUE)
, m_iDisConnCntWithZC(0)
, zcIdWithZCSIM_(1)
, ciIdWithZCSIM_(1)
, zcTypeWithZCSIM_(0)
, ciTypeWithZCSIM_(0)
, m_iAutoUpdateZCStat_(0)
, m_iManUpdateAswStat_(1)
, m_nUpTime(0)
, m_nDownTime(0)
, m_IsCnctToWcuATP(0)
{
	/*netAddr_.localIP_MY_A = htonl(inet_addr("192.168.11.110"));
	netAddr_.LocalATPServerIP_ = htonl(inet_addr("192.168.11.68"));
	
	netAddr_.localIOPort_		= 6001;
	netAddr_.localATPPort_		= 25000;*/

	for (int i = 0; i < 2;i++)
	{
		m_pioSock[i] = new CIOSocket(this,i);
	}

	m_p3MSock = new CIOSocket(this,2);
	m_pZcSock_ = new CIOSocket(this, 3);
	m_pWcuATPSock = new CIOSocket(this,2);
}

CILKTestDoc::~CILKTestDoc()
{

	ioSock_.Close();

	for (int i = 0; i < 2; i++)
	{
		m_pioSock[i]->Close();
		delete m_pioSock[i];
	}
	m_tcpSevSock_.Close();

	m_p3MSock->Close();
	delete m_p3MSock;

	m_pZcSock_->Close();
	delete m_pZcSock_;

	m_pWcuATPSock->Close();
	delete m_pWcuATPSock;
	
	for each (CADDevice* pDevice in vDevice_)
	{
		delete pDevice;
	}

	for each (Relay* pRelay in vRelay_)
	{
		delete pRelay;
	}

	for each (CZcCommRelay* pZcRelay in vZcCommRelay_)
	{
		delete pZcRelay;
	}

	for each (SwitchSection* pSwitchSection in vSwitchSection_)
	{
		delete pSwitchSection;
	}

	for each (array<IOItem, 16> ioArray in vInputBoard_)
	{
		for each (IOItem io in ioArray)
		{
			delete io.pBtn_;
			delete io.pCaption_;
		}
	}

	for each (array<IOItem, 16> ioArray in vOutputBoard_)
	{
		for each (IOItem io in ioArray)
		{
			delete io.pBtn_;
			delete io.pCaption_;
		}
	}

	for each(CADStation* pStation in vStation_)
	{
		delete pStation;
	}
	for each(CADGraphic* pGraphic in vGraphic_)
	{
		delete pGraphic;
	}

}


BOOL CILKTestDoc::OnNewDocument()
{	
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}

// CILKTestDoc ���л�

void CILKTestDoc::Serialize(CArchive& ar)
{
	if (!ar.IsStoring())
	{
		strPath = ar.m_strFileName.Left(ar.m_strFileName.ReverseFind(L'\\') + 1); // �ļ�·��
		loadNetConfig(strPath);

		loadGraphic(ar);			// ��ȡͼ��
		loadInputRelay(strPath);	// ��ȡ�ɼ��̵���
		loadOutputRelay(strPath);	// ��ȡ�����̵���
		loadElementData(strPath);
		load3MData(strPath);
		
		if (m_isCnctToSAM>0)
		{
			m_3MProcess = new CSAMProcess(this, ciIdWith3M);
		}
		/*ZCͨ������*/
		if (m_iIsCnctToSamZC == 1)
		{
			F_RSSP1_ReadRssp1Cfg(strPath);
			loadRouteAddrData(strPath);
			loadRouteTableData(strPath);
			loadLogicTvsCfgData(strPath);
			loadPsdCfgData(strPath);
			loadOlpData(strPath);
			load3MZCData(strPath);
			checkZcConnHead(); 
			setZcCommRelay();
			OnAllLightOFF(); /*��ʼ��Ϊȫ�����*/
		}
		
		loadPourRelay();

		for each (CADDevice* pDevice in vDevice_)
		{
			TRACE(pDevice->name_.text_ + L"\n");
			pDevice->ShowWindow(SW_SHOW); // Ҫ���ݼ̵���״̬�����豸������ֻ���������ݶ�ȡ��Ϻ����ʾ
			pDevice->loadMenu(); // �����Լ��ļ̵������ɲ˵���ÿ�����͵��źŻ������ļ̵�����ͬ������ÿ�� CADDevice Ҫ�洢�Լ��� CMenu��
		}
		for each (CADStation* pStation in vStation_)
		{
			pStation->ShowWindow(SW_SHOW); //
			pStation->loadMenu(); // 
		}
		for each (CADDevice* pDevice in vLingSanDev)
		{
			pDevice->ShowWindow(SW_SHOW); // 
		}

		createIOBoard();
		checkIOValid();
		//createZCSimBoard();

		InitSocketByCfg();
		//InitSockets();

	}
}

// CILKTestDoc ���

#ifdef _DEBUG
void CILKTestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CILKTestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CILKTestDoc ����


int CILKTestDoc::getBoardCount(BOOL bInput)
{
	int maxBoardID = 0;
	for each (Relay* pRelay in vRelay_)
	{
		if (bInput)
		{
			if (pRelay->bInput_ && pRelay->boardID_[0] > maxBoardID && pRelay->boardID_[0] != 0xFF && pRelay->boardID_[0] != UNUSERELAYINADDR)
			{
				maxBoardID = pRelay->boardID_[0];
			}
		}
		else
		{
			if (pRelay->bOutput_ && pRelay->boardID_[1] > maxBoardID && pRelay->boardID_[1] != 0xFF && pRelay->boardID_[1] != UNUSERELAYINADDR)
			{
				if (pRelay->boardID_[1] == UNUSERELAYINADDR)
				{
					int c = 1;
				}
				maxBoardID = pRelay->boardID_[1];
			}
		}
	}

	return maxBoardID + 1;
}


void CILKTestDoc::loadGraphic(CArchive& ar)
{
	// ��ȡͼ��
	int nGraphic = 0;
	BOOL m_bHasLogicSection;//add by lwm�Ƿ�����߼�����
	int m_nMaxDGLogSecNum;//�������������߼����θ���
	BYTE m_Version[4];//�汾�ţ��ĸ��ֽ�0-3����Ϊ�ߡ��С��͡�build��

	ar.Read(&m_Version, sizeof(m_Version));//�м�汾ʱ��ע�͵�
	ar >> m_bHasLogicSection >> m_nMaxDGLogSecNum;//add by lwm �Ƿ��߼��������Զ�ȡ

	ar >> nGraphic;

//	POSITION pz = GetFirstViewPosition();
//	CView* pView = GetNextView(pz);

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CView* pView = (CView*)(pMainFrame->pSplitterFrame_->pChildView_);

	for (int i = 0; i < nGraphic; i++)
	{
		CADGraphic* pGraphic = nullptr;
		CRect wndRect;
		ar >> pGraphic >> wndRect;
		pGraphic->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, wndRect, pView, 0);
		pGraphic->compressWindow();

		if (pGraphic->IsKindOf(RUNTIME_CLASS(CADDevice)))
		{
			pGraphic->ShowWindow(SW_HIDE);
			vDevice_.push_back(static_cast<CADDevice*>(pGraphic));
			static_cast<CADDevice*>(pGraphic)->name_.pDev_ = (CADDevice*)pGraphic;
		}
		else if (pGraphic->IsKindOf(RUNTIME_CLASS(CADStation)))
		{
			pGraphic->ShowWindow(SW_HIDE);//��ֹ��ͼʱ�̵���δ���룬����̵�����ͳһ����
			vStation_.push_back(static_cast<CADStation*>(pGraphic));
			static_cast<CADDevice*>(pGraphic)->name_.pDev_ = (CADDevice*)pGraphic;
		}
		else
		{
			pGraphic->ShowWindow(SW_SHOW);
			vGraphic_.push_back(pGraphic);
		}
	}
}


static DeviceInput* CreateSectionInput(void) { return new SectionInput; }
static DeviceInput* CreateSwitchInput(void)  { return new SwitchInput;  }
static DeviceInput* CreateSingalInput(void)  { return new SignalInput;  }
static DeviceInput* CreateLingSanInput(void)  { return new LingSanInput; }

void CILKTestDoc::loadInputRelay(CString strPath)
{
	CString inputFileName[4] = { 
		strPath + L"x86\\Section.le", 
		strPath + L"x86\\Switch.le", 
		strPath + L"x86\\Signal.le",
		strPath + L"x86\\LingSan.le"
	};

	typedef DeviceInput* (*InputCreator)(void);
	InputCreator createInput[4] = { // ������Ϣ�� Creator
		CreateSectionInput, CreateSwitchInput, CreateSingalInput, CreateLingSanInput, };
	int ioDataSize[4] = { // ÿ�����ʹ�С
		sizeof(SectionInput), sizeof(SwitchInput), sizeof(SignalInput), sizeof(LingSanInput) };

	for (size_t i = 0; i < 4; i++)
	{
		CFile file;
		if (file.Open(inputFileName[i], CFile::modeRead|CFile::typeBinary))
		{
			int nTotal = 0; // ����
			file.Read(&nTotal, sizeof(nTotal));

			for (int j = 0; j < nTotal; j++)
			{
				DeviceInput* pInput = createInput[i]();
				file.Read(pInput, ioDataSize[i]);

				// �ɻ�
				switch (i)
				{
				case 0:
					setSectionRelay((SectionInput*)pInput);
					break;
				case 1:
					setSwitchRelayIn((SwitchInput*)pInput);
					break;
				case 2:
					setSignalRelayIn((SignalInput*)pInput);  
					break;
				case 3:
					setLingSanRelayIn((LingSanInput*)pInput);
					break;
				default:
					break;
				}

				delete pInput;
			}

			file.Close();
		}
		else
		{
			CString strErr(L"���ļ�ʧ�ܣ�");
			strErr.Append(inputFileName[i]);
			AfxGetMainWnd()->SendMessage(WM_OUT_MESSAGE, 1, (LPARAM)&strErr);
		}
	}
}



void CILKTestDoc::loadElementData(CString strPath){
	CString inputFileName = strPath + L"x86\\ATP\\ElementCode.le";
	CFile file;
	int dataSize = sizeof(ElementCode);
	if (file.Open(inputFileName, CFile::modeRead | CFile::typeBinary))
	{ //���ļ�

		if (m_isCnctToGalaxy <= 0)
		{
			CString strErr(L"��˲������ļ�<�Ƿ����Galzxy>�������ã���ɾ����");
			strErr.Append(inputFileName);
			//AfxGetMainWnd()->SendMessage(WM_OUT_MESSAGE, 1, (LPARAM)&strErr);

			AfxMessageBox(strErr);
			file.Close();
			return;
		}

		file.Read(&nElementTotal, 4);
		ElementCode Element;
		int c = 1;
		for (int i = 0; i < nElementTotal; i++)
		{
			file.Read(&Element, dataSize);
			vElements_.push_back(Element);
		}
		TRACE1("\n nElementTotal = %d\n", nElementTotal);
		StartMapTrainsimData(nElementTotal);
		file.Close();
	}else
	{
		if (m_isCnctToGalaxy > 0)
		{
			CString strErr(L"���ļ�ʧ�ܣ���˲飺\n");
			strErr.Append(L"1��" + inputFileName);
			AfxGetMainWnd()->SendMessage(WM_OUT_MESSAGE, 1, (LPARAM)&strErr);

			AfxMessageBox(strErr + L"�ļ��Ƿ���ڡ�\n2�������ļ�<�Ƿ����Galaxy>��������");
			exit(0);
		}
		
	}
	int b = 1;
}


void CILKTestDoc::StartMapTrainsimData(int Count)
{
	//for each(ElementCode ele in vElements_)
	for (int index = 0; index <vElements_.size(); index++)
	{
		if (vElements_.at(index).ixl_elementkind_id == 200){//����ǵ���
			 CADDevice* pDevice = findDevcieByAddr(vElements_.at(index).ixl_Devaddr, RUNTIME_CLASS(RailSwitch));
			if (pDevice != NULL){
				pDevice->eleType = L"200";
				pDevice->eleIndex.Format(L"%d", vElements_.at(index).wcu_instance_index); //= /*ixl_element_index*/;
			}
			else
			{
				AfxMessageBox(L"����Ԫ�ر�ƥ��ʧ�ܣ�����˲�" + CString(vElements_.at(index).ixl_elementname));
			}
		}
		
		if (vElements_.at(index).ixl_elementkind_id == 201){//������źŻ�
			CADDevice* pDevice = NULL;
			pDevice = findDevcieByAddr(vElements_.at(index).ixl_Devaddr, RUNTIME_CLASS(CADSignal));
			if (pDevice != NULL){
				pDevice->eleType = L"201";
				pDevice->eleIndex.Format(L"%d", vElements_.at(index).wcu_instance_index);
			}
			else
			{
				AfxMessageBox(L"�źŻ�Ԫ�ر�ƥ��ʧ�ܣ�����˲�" + CString(vElements_.at(index).ixl_elementname));
			}
		}
		
		if (vElements_.at(index).ixl_elementkind_id == 213)
		{//���������
			CADDevice* pDevice = NULL;
			if (m_isLogicSection==1)
			{
				CString name(vElements_.at(index).ixl_elementname);
				pDevice = findDeviceByName(name + L"A");
			}
			else
			{
				pDevice = findDevcieByAddr(vElements_.at(index).ixl_Devaddr, RUNTIME_CLASS(CADSection));
			}
			
			if (pDevice == NULL)
			{
				pDevice = findSwitchBySectAddr(vElements_.at(index).ixl_Devaddr);
				RailSwitch* pswitch = dynamic_cast<RailSwitch*> (pDevice);
				if (pswitch != NULL)
				{
					pswitch->switchSectionEleType = L"213";
					pswitch->switchSectionEleIndex.Format(L"%d", vElements_.at(index).wcu_instance_index);
				}
				else
				{
					AfxMessageBox(L"����Ԫ�ر�ƥ��ʧ�ܣ�����˲�" + CString(vElements_.at(index).ixl_elementname));
				}
			}
			else
			{
				pDevice->eleType = L"213";
				pDevice->eleIndex.Format(L"%d", vElements_.at(index).wcu_instance_index);
			}
		}
		
		if (vElements_.at(index).ixl_elementkind_id >= 25501 && vElements_.at(index).ixl_elementkind_id <= 25503)
		{//��ɢ
			CADDevice* pDevice = NULL;
			pDevice = findLingsanDevByAddr(vElements_.at(index).ixl_Devaddr);
			if (pDevice != NULL)
			{
				pDevice->eleType.Format(L"%d", vElements_.at(index).ixl_elementkind_id);
				pDevice->eleIndex.Format(L"%d", vElements_.at(index).wcu_instance_index);
			}
			else
			{
				AfxMessageBox(L"������ɢ�豸ʧ��" + CString(vElements_.at(index).ixl_elementname));
			}
		}
	}
}



void CILKTestDoc::setSectionRelay(SectionInput* pInput)
{
	if (pInput->relayAddr_ == UNUSERELAYINADDR)
	{
		return ;//ȥ����ʾ����
	}

	BOOL bLoadSecess = FALSE;
	CString inputName = CString(pInput->name_);
	if (m_isLogicSection == 1 && inputName.Right(1) != L"A")  //io����һ���������������ƣ�
	{
		inputName = inputName + L"A";
	}
	if (CADSection* pSection = (CADSection*)findDeviceByName(inputName))
	{
		// �޲�����
		if (isRelayExist(inputName) == false)
		{
			/*pSection->name_.text_*//*pSection->logicDeviceIO_*/
			Relay* pRelay1 = new Relay(inputName, L"WGJ", TRUE, FALSE, pSection);
			pRelay1->boardID_[0] = boardID(pInput->relayAddr_);
			pRelay1->portID_[0] = portID(pInput->relayAddr_);
			pRelay1->stat_ = pRelay1->lastStat_ = Relay::STAT_UP;

			if ((isNameRightA(pRelay1->deviceName_) == TRUE) && (pRelay1->deviceName_.Right(/*2).Left(*/1) != L'F'))
			{
				pSection->vRelay_.push_back(pRelay1);
				vRelay_.push_back(pRelay1);
			}
			Relay* pRelay2 = new Relay(inputName, L"FWJ", TRUE, FALSE, pSection);
			pRelay2->boardID_[0] = boardID(pInput->relayAddr_2);
			pRelay2->portID_[0] = portID(pInput->relayAddr_2);
			pRelay2->stat_ = pRelay2->lastStat_ = Relay::STAT_DOWN;

			if ((isNameRightA(pRelay2->deviceName_) == TRUE) && (pRelay2->deviceName_.Right(/*2).Left(*/1) != L'F'))
			{
				pSection->vRelay_.push_back(pRelay2);
				vRelay_.push_back(pRelay2);
			}
			bLoadSecess = TRUE;
		}

	}
	else if (loadSwitchSection(pInput))
	{
		bLoadSecess = TRUE;
	}
	else
	{
		int a = 1;
	}

	if (!bLoadSecess)
	{
		CString strErr(L"û���ҵ�������е����Σ�");
		strErr.Append(CString(pInput->name_));
		AfxGetMainWnd()->SendMessage(WM_OUT_MESSAGE, 1, (LPARAM)&strErr);
	}

}
//���ҵ�iNo���õ�ַ���豸
CADDevice* CILKTestDoc::findDevcieByAddr(int addr, CRuntimeClass* pRuntimeClass,int iNo)
{
	//static int tmp = 0;
	//auto it = find_if(vDevice_.begin(), vDevice_.end(), [&addr, &pRuntimeClass](CADDevice* pDevice) {
	//	if (pDevice->IsKindOf(pRuntimeClass))
	//	{
	//		pDevice->io_.addr_ == addr;
	//		tmp++;
	//	}
	//	if (tmp == iNo+1)
	//	{
	//		return pDevice->io_.addr_;
	//	}
	//	return 0xFFFF;
	//});
	int tmp = 0;
	CADDevice* pDevice = NULL;
	for (int i = 0; i < vDevice_.size(); i++)
	{
		pDevice = vDevice_.at(i);
		if ((pDevice->IsKindOf(pRuntimeClass)) && (pDevice->io_.addr_ == addr))
		{		
			tmp++;
		}
		if (tmp == iNo + 1)
		{
			break;
		}
		pDevice = NULL;
	}

	return pDevice;
}

//���ҵ�iNo���õ�ַ���豸
CADDevice* CILKTestDoc::findDevcieByIORelayAddr(int addr,int board,int port, CRuntimeClass* pRuntimeClass)
{
	//static int tmp = 0;
	//auto it = find_if(vDevice_.begin(), vDevice_.end(), [&addr, &pRuntimeClass](CADDevice* pDevice) {
	//	if (pDevice->IsKindOf(pRuntimeClass))
	//	{
	//		pDevice->io_.addr_ == addr;
	//		tmp++;
	//	}
	//	if (tmp == iNo+1)
	//	{
	//		return pDevice->io_.addr_;
	//	}
	//	return 0xFFFF;
	//});
	int tmp = 0;
	CADDevice* pDevice = NULL;
	for (int i = 0; i < vDevice_.size(); i++)
	{
		pDevice = vDevice_.at(i);
		if ((pDevice->IsKindOf(pRuntimeClass)) && (pDevice->vRelay_[2]->boardID_[0] == board)
			&&(pDevice->vRelay_[2]->portID_[0] == port)&&(pDevice->io_.addr_ != addr))
		{
			break;
		}
		pDevice = NULL;
	}

	return pDevice;
}

CADDevice* CILKTestDoc::findDevcieByAddr(int addr, CRuntimeClass* pRuntimeClass)
{
	auto it = find_if(vDevice_.begin(), vDevice_.end(), [&addr, &pRuntimeClass](CADDevice* pDevice) {
		if (pDevice->IsKindOf(pRuntimeClass))
		{
			return pDevice->io_.addr_ == addr;
		}
		return false;
	});
	return it == vDevice_.end() ? findLingsanDevByAddr(addr, pRuntimeClass) : (*it);
}

CADDevice* CILKTestDoc::findDevcieByEleIndex(CString index)
{
	auto it = find_if(vDevice_.begin(), vDevice_.end(), [&index](CADDevice* pDevice) {
		if (pDevice->eleIndex == index)
		{
			return true;
		}
		return false;
	});
	return it == vDevice_.end() ? findLingsanDevByEleIndex(index) : (*it);
}

CADDevice* CILKTestDoc::findLingsanDevByEleIndex(CString index)
{
	auto it = find_if(vLingSanDev.begin(), vLingSanDev.end(), [&index](CADDevice* pDevice) {
		if (pDevice->eleIndex == index)
		{
			return true;
		}
		return false;
	});
	return it == vLingSanDev.end() ? NULL : (*it);
}

CADDevice* CILKTestDoc::findLingsanDevByAddr(int addr, CRuntimeClass* pRuntimeClass)
{
	auto it = find_if(vLingSanDev.begin(), vLingSanDev.end(), [&addr, &pRuntimeClass](CADDevice* pDevice) {
		if (pDevice->IsKindOf(pRuntimeClass))
		{
			return pDevice->io_.addr_ == addr;
		}
		return false;
	});
	return it == vLingSanDev.end() ? NULL : (*it);
}

CADDevice* CILKTestDoc::findLingsanDevByAddr(int addr)
{
	auto it = find_if(vLingSanDev.begin(), vLingSanDev.end(), [&addr](CADDevice* pDevice) {
		if (pDevice->io_.addr_ == addr)
		{
			return true ;
		}
		return false;
	});
	return it == vLingSanDev.end() ? NULL : (*it);
}


BOOL CILKTestDoc::loadSwitchSection(SectionInput* pInput)
{
	BOOL bLoadSuccess = FALSE;

	for each (CADDevice* pDevice in vDevice_)
	{
		if (pDevice->IsKindOf(RUNTIME_CLASS(RailSwitch)))
		{
			RailSwitch* pSwitch = dynamic_cast<RailSwitch*>(pDevice);
			CString switsectname = CString(pSwitch->data_.section_.name_);

			if (m_isLogicSection == 1 && (switsectname.Right(1) == L"A" || switsectname.Right(1)==L"B"))
			{
				switsectname = switsectname.Left(switsectname.GetLength() - 1);
			}
			if (switsectname== CString(pInput->name_))
			{
				// �ҵ��в����ζ�Ӧ�ĵ���
				Relay* pRelay1 = findRelayByAddr(pInput->relayAddr_);
				Relay* pRelay2 = findRelayByAddr(pInput->relayAddr_2);
				if (pRelay1==NULL)
				{
					// �״ζ�ȡ���в����Σ������в����εļ̵����������档
					pRelay1 = new Relay(CString(pInput->name_), L"GJ", TRUE, FALSE, pSwitch);
					pRelay1->boardID_[0] = boardID(pInput->relayAddr_);
					pRelay1->portID_[0] = portID(pInput->relayAddr_);
					pRelay1->stat_ = pRelay1->lastStat_ = Relay::STAT_UP;
					vRelay_.push_back(pRelay1);

					if (pRelay2==NULL)
					{
						pRelay2 = new Relay(CString(pInput->name_), L"FWJ", TRUE, FALSE, pSwitch);
						pRelay2->boardID_[0] = boardID(pInput->relayAddr_2);
						pRelay2->portID_[0] = portID(pInput->relayAddr_2);
						pRelay2->stat_ = pRelay2->lastStat_ = Relay::STAT_DOWN;
						vRelay_.push_back(pRelay2);
					}					

					// �����в�����
					SwitchSection* pSwitchSection = new SwitchSection(pInput->name_, pInput->addr_);
					pSwitchSection->vSwitch_.push_back(pSwitch);
					vSwitchSection_.push_back(pSwitchSection);

				}
				else
				{
					// �Ѿ������в����Σ��ҵ����в����β����浱ǰ����
					auto it = find_if(vSwitchSection_.begin(), vSwitchSection_.end(), [&pInput](SwitchSection* pSwitchSection) {
						return pSwitchSection->addr_ == pInput->addr_;
					});

					if (it != vSwitchSection_.end())
					{
						(*it)->vSwitch_.push_back(pSwitch);

						pRelay1->pFDevice_ = (CADDevice*)pSwitch;

						if (pRelay2!=NULL)
						{
							pRelay2->pFDevice_ = (CADDevice*)pSwitch;
						}
					}
					else
					{
						CString strErr(L"û���ҵ��в����Σ�");
						strErr.Append(CString(pInput->name_));
						AfxGetMainWnd()->SendMessage(WM_OUT_MESSAGE, 1, (LPARAM)&strErr);
					}
				}
				// ������в������Ѿ����ڣ����ڵ����豸�б�������̵�����ַ
				pSwitch->vRelay_.push_back(pRelay1);
				pSwitch->vRelay_.push_back(pRelay2);
				bLoadSuccess = TRUE;
			}
		}
	}

	return bLoadSuccess;
}


Relay* CILKTestDoc::findRelayByAddr(int addr)
{
	auto it = find_if(vRelay_.begin(), vRelay_.end(), [&addr](Relay* pRelay) {
		return ((pRelay->boardID_[0] << 8) | (pRelay->portID_[0])) == addr;
	});

	return it == vRelay_.end() ? NULL : (*it);
}




void CILKTestDoc::setSwitchRelayIn(SwitchInput* pInput)
{
	if (!isSwitchRelayTrue(pInput))
	{
		return;
	}

	CString inputName = CString(pInput->name_);
	//if (RailSwitch* pSwitch = (RailSwitch*)/*findDeviceByName(inputName)*/findDevcieByAddr(pInput->addr_, RUNTIME_CLASS(RailSwitch)))
	RailSwitch* pSwitch = (RailSwitch*)findDeviceByName(inputName);
	if (pSwitch==NULL)
	{
		CString strErr(L"û���ҵ�������еĵ���");
		strErr.Append(CString(pInput->name_));
		AfxGetMainWnd()->SendMessage(WM_OUT_MESSAGE, 1, (LPARAM)&strErr);
		return;
	}
	if (pSwitch->IsKindOf(RUNTIME_CLASS(RailSwitch)))
	{
		if (Relay* pRelay = findRelay(boardID(pInput->relayAddr_[0]), portID(pInput->relayAddr_[0]), TRUE))
		{
			// ˫��������������������¼���� 
			for (int i = 2; i < 5; i++)
			{
				pSwitch->vRelay_.push_back(pRelay->pDevice_->vRelay_[i]);
				pRelay->pDevice_->vRelay_[i]->pFDevice_ = (CADDevice *)pSwitch;
			}

			return;
		}

		Relay* pRelay[3] = {
			new Relay(pSwitch->name_.text_, L"DBJ", TRUE, FALSE, pSwitch),
			new Relay(pSwitch->name_.text_, L"FBJ", TRUE, FALSE, pSwitch),
			new Relay(pSwitch->name_.text_, L"SJ", TRUE, FALSE/*TRUE*/, pSwitch),
		};

		// ����Ĭ�ϴ��ڶ�λ
		pRelay[0]->stat_ = pRelay[0]->lastStat_ = Relay::STAT_UP;

		for (size_t i = 0; i < 3; i++)
		{
			pRelay[i]->boardID_[0] = boardID(pInput->relayAddr_[i]);
			pRelay[i]->portID_[0] = portID(pInput->relayAddr_[i]);

			pSwitch->vRelay_.push_back(pRelay[i]);
			vRelay_.push_back(pRelay[i]);
		}
	}
	else
	{
		CString strErr(L"û���ҵ�������еĵ���");
		strErr.Append(CString(pInput->name_));
		AfxGetMainWnd()->SendMessage(WM_OUT_MESSAGE, 1, (LPARAM)&strErr);
	}
}


void CILKTestDoc::setSignalRelayIn(SignalInput* pInput)
{
	if (!isSignalRelayTrue(pInput))
	{
		return;
	}


	if (CADSignal* pSignal = (CADSignal*)findDevcieByAddr(pInput->addr_, RUNTIME_CLASS(CADSignal)))
	{
		pSignal->ioType_ = pInput->type_;

		WCHAR* relayType[] = {
			L"LXJ", L"ZXJ", L"DXJ", L"TXJ", L"LUXJ", L"YXJ", L"DJ", L"2DJ", L"DDJ", L"SGJ"
		};
		if (pSignal->name_.text_==L"S32")
		{
			int nihao;
			nihao = 0;
		}
		for (size_t i = 0; i < 10; i++)
		{
			if (pInput->relayAddr_[i] != UNUSERELAYINADDR)
			{
				Relay* pRelay = new Relay(pSignal->name_.text_, relayType[i], TRUE, ((i < 6 )||(i > 7))? TRUE : FALSE, pSignal);
				pRelay->boardID_[0] = boardID(pInput->relayAddr_[i]);
				pRelay->portID_[0] = portID(pInput->relayAddr_[i]);

				// Ĭ�ϵ�˿�Ǻõ�
				if ((i == 6)||(i == 7))
				{
					pRelay->stat_ = pRelay->lastStat_ = Relay::STAT_UP;
				}
	
				pSignal->vRelay_.push_back(pRelay);
				vRelay_.push_back(pRelay);
			}
		}
	}
}


static DeviceOutput* CreateOtherOutput(void) { return new OtherOutput; }
static DeviceOutput* CreateSwitchOutput(void) { return new SwitchOutput; }
static DeviceOutput* CreateSignalOutput(void) { return new SignalOutput; }
static DeviceOutput* CreateSectionOutput(void) { return new SectionOutput; }


void CILKTestDoc::loadOutputRelay(CString path)
{
	CString inputFileName[4] = {
		path + L"x86\\outputOther.le",
		path + L"x86\\outputSwitch.le",
		path + L"x86\\outputSignal.le",
		path + L"x86\\outputSection.le",
	};

	typedef DeviceOutput* (*OutputCreator)(void);
	OutputCreator createOutput[4] = { // ������Ϣ�� Creator
		CreateOtherOutput, CreateSwitchOutput, CreateSignalOutput, CreateSectionOutput, };
	int ioDataSize[4] = { // ÿ�����ʹ�С
		sizeof(OtherOutput), sizeof(SwitchOutput), sizeof(SignalOutput), sizeof(SectionOutput) };

	for (size_t i = 0; i < 4; i++)
	{
		CFile file;
		if (file.Open(inputFileName[i], CFile::modeRead | CFile::typeBinary))
		{
			int nTotal = 0; // ����
			file.Read(&nTotal, sizeof(nTotal));

			for (int j = 0; j < nTotal; j++)
			{
				DeviceOutput* pOutput = createOutput[i]();
				file.Read(pOutput, ioDataSize[i]);

				// �ɻ�
				switch (i)
				{
				case 0:
					setOtherRelayOut((OtherOutput*)pOutput);
					break;
				case 1:
					setSwitchRelayOut((SwitchOutput*)pOutput);
					break;
				case 2:
					setSignalRelayOut((SignalOutput*)pOutput);
					break;
				case 3:
					setSectionRelayOut((SectionOutput*)pOutput);
					break;
				default:
					break;
				}
				delete pOutput;
			}

			file.Close();
		}
		else
		{
			CString strErr(L"���ļ�ʧ�ܣ�");
			strErr.Append(inputFileName[i]);
			AfxGetMainWnd()->SendMessage(WM_OUT_MESSAGE, 1, (LPARAM)&strErr);
		}
	}
}


void CILKTestDoc::setOtherRelayOut(OtherOutput* pOutput)
{
	if (pOutput->addr == UNUSERELAYINADDR || pOutput->addr == UNUSERELAYOUTADDR)
	{
		return;
	}
	if (pOutput->devJ1 == UNUSERELAYOUTADDR)
	{
		return;
	}

	if (!addToDevByRelayName(pOutput))
	{
		// TODO����ȡ��ɢ�������ͼ̵���
		Relay* pRelay = new Relay(L"", CString(pOutput->name), FALSE, TRUE, NULL);
		pRelay->boardID_[1] = boardID(pOutput->devJ1);
		pRelay->portID_[1] = portID(pOutput->devJ1);
		pRelay->stat_ = pRelay->lastStat_ = Relay::STAT_DOWN;
		vRelay_.push_back(pRelay);
	}
	
}


void CILKTestDoc::setSwitchRelayOut(SwitchOutput* pOutput)
{
	//if (RailSwitch* pSwitch = (RailSwitch*)/*findDevByBtnID(pOutput->btn)*/findDevcieByAddr(pOutput->addr, RUNTIME_CLASS(RailSwitch)))

	if (pOutput->DCJ == UNUSERELAYOUTADDR||pOutput->FCJ == UNUSERELAYOUTADDR)
	{
		return;
	}

	
	RailSwitch* pSwitch = (RailSwitch*)findDevByBtnID(pOutput->btn);

	if (pSwitch == NULL)
	{
		CString strErr(L"û���ҵ���������еĵ���");
		strErr.Append(CString(pOutput->name));
		AfxGetMainWnd()->SendMessage(WM_OUT_MESSAGE, 1, (LPARAM)&strErr);
		return;
	}
	if (pSwitch->IsKindOf(RUNTIME_CLASS(RailSwitch)))
	{
		// ��������  �����  �����  ���ռ� 
		if (pSwitch->vRelay_.size()!=5)
		{
			AfxMessageBox(L"�������μ̵���ƥ��ʧ�ܣ���˲�"+pSwitch->io_.getName());
			exit(0);
		}
		if (Relay* pRelay = findRelay(boardID(pOutput->DCJ), portID(pOutput->DCJ), FALSE))
		{
			// ˫��������������������¼���� 
			for (int i = 5; i < 7;i++)
			{
				pSwitch->vRelay_.push_back(pRelay->pDevice_->vRelay_[i]);
				pRelay->pDevice_->vRelay_[i]->pFDevice_ = (CADDevice *)pSwitch;
			}
			
			return;
		}
		pSwitch->vRelay_[4]->bOutput_ = TRUE;
		pSwitch->vRelay_[4]->boardID_[1] = boardID(pOutput->SJ);
		pSwitch->vRelay_[4]->portID_[1] = portID(pOutput->SJ);

		Relay* pRelay[2] = {
			new Relay(pSwitch->name_.text_, L"DCJ", FALSE, TRUE, pSwitch),
			new Relay(pSwitch->name_.text_, L"FCJ", FALSE, TRUE, pSwitch),
		};

		pRelay[0]->boardID_[1] = boardID(pOutput->DCJ);
		pRelay[0]->portID_[1] = portID(pOutput->DCJ);

		pRelay[1]->boardID_[1] = boardID(pOutput->FCJ);
		pRelay[1]->portID_[1] = portID(pOutput->FCJ);

		for (size_t i = 0; i < 2; i++)
		{
			pSwitch->vRelay_.push_back(pRelay[i]);
			vRelay_.push_back(pRelay[i]);
		}
	}
}


void CILKTestDoc::setSignalRelayOut(SignalOutput* pOutput)
{
	if (pOutput->addr == UNUSERELAYOUTADDR)
	{
		return;
	}
	if (CADSignal* pSignal = (CADSignal*)findDevcieByAddr(pOutput->addr, RUNTIME_CLASS(CADSignal)))
	{
		// L"LXJ", L"ZXJ", L"DXJ", L"TXJ", L"LUXJ", L"YXJ",	L"DDJ",	L"DJ", L"2DJ"
		//   LXJ;    ZXJ;    DXJ;    TXJ;    LUXJ;    YXJ	DDJ;	
		UINT16* pPort = &(pOutput->LXJ);
		int flag = 0;
		for (size_t i = 0; i < 8; i++)
		{
			if (*pPort != UNUSERELAYOUTADDR) // �������û�õĶ˿ںţ������ 0XFF07
			{
				if ((i!=7)&&(i != 6))
				{
					pSignal->vRelay_[flag]->boardID_[1] = boardID(*pPort);
					pSignal->vRelay_[flag]->portID_[1] = portID(*pPort);
					flag++;
				}
				else if (i == 6)
				{
					while (pSignal->vRelay_[flag]->name_ != L"DDJ")
					{
						flag++;
					}
					pSignal->vRelay_[flag]->boardID_[1] = boardID(*pPort);
					pSignal->vRelay_[flag]->portID_[1] = portID(*pPort);
				}
				else if (i == 7)
				{
					while (pSignal->vRelay_[flag]->name_ != L"SGJ")
					{
						flag++;
					}
					pSignal->vRelay_[flag]->boardID_[1] = boardID(*pPort);
					pSignal->vRelay_[flag]->portID_[1] = portID(*pPort);
				}

			}
			pPort++;
		}
	}
}

void CILKTestDoc::OnSetSwitchSection(int sectionAddr, RailSwitch* pFrom)
{
	auto it = find_if(vSwitchSection_.begin(), vSwitchSection_.end(), [&sectionAddr](SwitchSection* pSection) {
		return pSection->addr_ == sectionAddr;
	});

	if (it != vSwitchSection_.end())
	{
		for each (RailSwitch* pSwitch in (*it)->vSwitch_)
		{
			pSwitch->Invalidate();
		}
	}
	else
	{
		CString strErr(L"�����в����δ����Ҳ����в����Σ�");
		strErr.Append(CString(pFrom->data_.section_.name_));
		AfxGetMainWnd()->SendMessage(WM_OUT_MESSAGE, 1, (LPARAM)&strErr);
	}
}


void CILKTestDoc::createIOBoard()
{
	int nInputBoard = getBoardCount(TRUE);
	int nOutputBoard = getBoardCount(FALSE);

	// �����������������ڴ�
	for (int i = 0; i < nInputBoard; i++)
	{
		array<IOItem, 16> ioArray;
		vInputBoard_.push_back(ioArray);
	}

	for (int i = 0; i < nOutputBoard; i++)
	{
		array<IOItem, 16> ioArray;
		vOutputBoard_.push_back(ioArray);
	}

	// ��ȡ CIOView
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CIOView* pIOView = pMainFrame->pSplitterFrame_->pIOView_;

	// ��ȡ CIOView �е� �������� Group
	CWnd* pInputGroup = pIOView->GetDlgItem(IDC_STATIC_INPUT_GROUP);
	CWnd* pOutputGroup = pIOView->GetDlgItem(IDC_STATIC_OUTPUT_GROUP);

	SetGroupRect(pInputGroup);

	CRect wndGroupRect;//��Ļλ��
	pInputGroup->GetWindowRect(wndGroupRect);
	pIOView->ScreenToClient(&wndGroupRect);//viewλ��

	wndGroupRect.top = wndGroupRect.bottom + 30;
	wndGroupRect.bottom = wndGroupRect.top + 15 * (BOARDDISTENCE + 5);
	pOutputGroup->MoveWindow(wndGroupRect);

	createIOItem(vInputBoard_, pInputGroup, TRUE);
	createIOItem(vOutputBoard_, pOutputGroup, FALSE);

	// ���� CIOView �Ŀ��
	CRect groupRect;
	pInputGroup->GetClientRect(groupRect);
	pIOView->SetScrollSizes(MM_TEXT, CSize(groupRect.Width() + 20, groupRect.Height() * 2 + 50));

	
}


void CILKTestDoc::createZCSimBoard()
{
	int nInBoardCount = 0, nOutBoardCount = 0;
	nInBoardCount = getBoardZcCommCount(TRUE);
	nOutBoardCount = getBoardZcCommCount(FALSE);

	for (int i = 0; i < nInBoardCount; i++)
	{
		array<IOItem, 8> zcSimInArray;
		vZCSimBoard_.push_back(zcSimInArray);
	}
	for (int i = 0; i < nOutBoardCount; i++)
	{
		array<IOItem, 8> zcSimOutArray;
		vZCSimOutputBoard_.push_back(zcSimOutArray);
	}

	// ��ȡ CIOView
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CIOView* pIOView = pMainFrame->pSplitterFrame_->pIOView_;

	CWnd* pZCSimGroup = pIOView->GetDlgItem(IDC_STATIC_ZCSEND_GROUP);
	CWnd* pZCSimOutGroup = pIOView->GetDlgItem(IDC_STATIC_ZCRECV_GROUP);

	createZCIOItem(vZCSimBoard_, pZCSimGroup, TRUE);
	createZCIOItem(vZCSimOutputBoard_, pZCSimOutGroup, FALSE);

	// ���� CIOView �Ŀ��
	CRect wndRect;
	pIOView->GetClientRect(wndRect);
	CRect groupRect;
	pZCSimGroup->GetClientRect(groupRect);

	pIOView->SetScrollSizes(MM_TEXT, CSize(groupRect.Width() + 20, groupRect.Height() * 6 + 50));
}


Relay* CILKTestDoc::findRelay(int boardID, int portID, BOOL bInput)
{
	auto it = find_if(vRelay_.begin(), vRelay_.end(), [boardID, portID, bInput](Relay* pRelay) {
		return (pRelay->boardID_[bInput ? 0 : 1] == boardID) && (pRelay->portID_[bInput ? 0 : 1] == portID);
	});

	return it == vRelay_.end() ? NULL : (*it);
}

CZcCommRelay* CILKTestDoc::findZcCommRelay(int boardID, int portID, BOOL bInput)
{
	auto it = find_if(vZcCommRelay_.begin(), vZcCommRelay_.end(), [boardID, portID, bInput](CZcCommRelay* pRelay) {
		return (pRelay->boardID_[bInput ? 0 : 1] == boardID) && (pRelay->portID_[bInput ? 0 : 1] == portID);
	});

	return it == vZcCommRelay_.end() ? NULL : (*it);
}
void CILKTestDoc::createIOItem(vector<array<IOItem, 16>>& vIOItem_, CWnd* pGroupWnd, BOOL bInput)
{
	const int margin = 5; // IOItem �ļ��
	const int width = 15; // CheckBox ���

	CRect groupRect;
	pGroupWnd->GetClientRect(groupRect);
	pGroupWnd->MapWindowPoints(pGroupWnd->GetParent(), groupRect);

	CRect btnRect(
		groupRect.left + 32,		 groupRect.top + 42,
		groupRect.left + 32 + width, groupRect.top + 42 + width);
	//�������а忨λ�����
	CRect captionRect1(groupRect.left+10, groupRect.top + 21,
		groupRect.left + 30, groupRect.top + 21 + width);

	for (int i = 0; i < 16;i++)
	{
		captionRect1.OffsetRect(0, BOARDDISTENCE);
		CIOCaptionWnd* pCaption_ = NULL;
		CString str ;
		str.Format(L"%d", i);
		pCaption_ = new CIOCaptionWnd(str, NULL);
		pCaption_->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, captionRect1, pGroupWnd->GetParent(), 0);
	}
	
	int boardID = 0;
	for (auto it = vIOItem_.begin(); it != vIOItem_.end(); it++)
	{
		// ÿ��IO��
		// Ϊÿ��IO�����ɰ��
		CIOView* pIOView = (CIOView*)pGroupWnd->GetParent();
		CString strBoardID;
		strBoardID.Format(L"%d", boardID);
		CRect boardIDRect(btnRect);
		boardIDRect.OffsetRect(0, -BOARDDISTENCE);
		pIOView->boardIDLabel_[bInput ? 0 : 1][boardID].Create(strBoardID, WS_CHILD | WS_VISIBLE, boardIDRect, pIOView);
		pIOView->boardIDLabel_[bInput ? 0 : 1][boardID].SetFont(&pIOView->font_);
		
		int portID = 0;
		int namelen = 0;
		for (auto jt = (*it).begin(); jt != (*it).end(); jt++)
		{
			// ÿ���˿�
			(*jt).pBtn_ = new CButton();
			(*jt).pBtn_->Create(L"", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, btnRect, pGroupWnd->GetParent(), 0);

			if (Relay* pRelay = findRelay(boardID, portID, bInput))
			{
				pRelay->pIoItem_[bInput ? 0 : 1] = &(*jt);
				if (pRelay->deviceName_ != L"")
				{
					CString name = L"";
					if (pRelay->deviceName_ != pRelay->name_)
					{
						name = pRelay->deviceName_ + L"_" + pRelay->name_;
					}
					else
					{
						name = pRelay->deviceName_;
					}
					
					(*jt).pCaption_ = new CIOCaptionWnd(name, pRelay);
					if (namelen < GetMultiByteLen(name))
					{
						namelen = GetMultiByteLen(name);
					}
				}
				else
				{
					(*jt).pCaption_ = new CIOCaptionWnd(pRelay->name_ , pRelay);

					if (namelen < GetMultiByteLen(pRelay->name_))
					{
						namelen = GetMultiByteLen(pRelay->name_);
					}
				}

				
			}
			else
			{
				CString str;
				str.Format(L"%d", portID);
				(*jt).pCaption_ = new CIOCaptionWnd(str, NULL);
			}
			CRect captionRect(btnRect);
			captionRect.OffsetRect(width + margin, 0);
			captionRect.right = captionRect.left + namelen * 6;

			(*jt).pCaption_->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, captionRect, pGroupWnd->GetParent(), 0);

			// �����ƶ� btnRect;
			portID++;
			btnRect.OffsetRect(0, 21);
		}

		// �����ƶ� btnRect λ��
		btnRect.OffsetRect(width + margin + namelen*6 + margin, -21 * 16);
		boardID++;
	}
	groupRect.right = btnRect.right;
	pGroupWnd->MoveWindow(groupRect);
}


void CILKTestDoc::createZCIOItem(vector<array<IOItem, 8>>& vIOItem_, CWnd* pGroupWnd, BOOL bInput)
{
	const int margin = 5; // IOItem �ļ��
	const int width = 15; // CheckBox ���

	CRect groupRect;
	pGroupWnd->GetClientRect(groupRect);
	pGroupWnd->MapWindowPoints(pGroupWnd->GetParent(), groupRect);

	CRect btnRect(
		groupRect.left + 32, groupRect.top + 42,
		groupRect.left + 32 + width, groupRect.top + 42 + width);

	int boardID = 0;
	for (auto it = vIOItem_.begin(); it != vIOItem_.end(); it++)
	{
		// ÿ��IO��
		// Ϊÿ��IO�����ɰ��
		CIOView* pIOView = (CIOView*)pGroupWnd->GetParent();
		CString strBoardID;
		strBoardID.Format(L"%d", boardID);
		CRect boardIDRect(btnRect);
		boardIDRect.OffsetRect(0, -21);
		pIOView->boardIDLabelZC_[bInput ? 0 : 1][boardID].Create(strBoardID, WS_CHILD | WS_VISIBLE, boardIDRect, pIOView);
		pIOView->boardIDLabelZC_[bInput ? 0 : 1][boardID].SetFont(&pIOView->font_);

		int portID = 0;
		for (auto jt = (*it).begin(); jt != (*it).end(); jt++)
		{
			// ÿ���˿�
			(*jt).pBtn_ = new CButton();
			(*jt).pBtn_->Create(L"", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, btnRect, pGroupWnd->GetParent(), 0);

			CRect captionRect(btnRect);
			captionRect.OffsetRect(width + margin, 0);
			captionRect.right = captionRect.left + 60;

			if (CZcCommRelay* pZcRelay = findZcCommRelay(boardID, portID, bInput))
			{
				pZcRelay->pIoItem_[bInput ? 0 : 1] = &(*jt);
				if (pZcRelay->name_ != L"")
				{
					(*jt).pCaption_ = new CIOCaptionWnd(pZcRelay->deviceName_ + L"_" + pZcRelay->name_, pZcRelay, true);
				}
				else
				{
					(*jt).pCaption_ = new CIOCaptionWnd(pZcRelay->deviceName_, pZcRelay, true);
				}
			}
			else
			{
				CString str;
				str.Format(L"%d", portID);
				(*jt).pCaption_ = new CIOCaptionWnd(str, NULL, true);
			}

			(*jt).pCaption_->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, captionRect, pGroupWnd->GetParent(), 0);

			// �����ƶ� btnRect;
			portID++;
			btnRect.OffsetRect(0, 21);
		}

		// �����ƶ� btnRect λ��
		btnRect.OffsetRect(width + margin + 60 + margin, -21 * 8);
		boardID++;
	}
	groupRect.right = btnRect.right;
	pGroupWnd->MoveWindow(groupRect);
}

int CILKTestDoc::boardZcCommID(UINT16 codeSeqIndex)
{
	int boarId = 0;
	int qouet = 0u, remaind = 0u;
	if (codeSeqIndex != 0xFFFF)
	{
		qouet = codeSeqIndex / 8;
		remaind = codeSeqIndex % 8;
		if (remaind == 0)
		{
			boarId = (qouet - 1);
		}
		else
		{
			boarId = qouet;
		}
	}
	return boarId;
}
int CILKTestDoc::portZcCommID(UINT16 codeSeqIndex)
{
	int portId = 0;
	int qouet = 0u, remaind = 0u;
	if (codeSeqIndex != 0xFFFFu)
	{
		qouet = codeSeqIndex / 8;
		remaind = codeSeqIndex % 8;
		if (remaind == 0)
		{
			portId = 0;
		}
		else
		{
			portId = (8 - remaind);
		}
	}
	return portId;
}

int CILKTestDoc::getBoardZcCommCount(BOOL bInput)
{
	int maxCount = 0;
	for each (CZcCommRelay* pZcRelay in vZcCommRelay_)
	{
		if (bInput)
		{
			if ((pZcRelay->bInput_) && (pZcRelay->boardID_[0] > maxCount) && (pZcRelay->boardID_[0] != 0xFFFF))
			{
				maxCount = pZcRelay->boardID_[0];
			}
		}
		else
		{
			if ((pZcRelay->bOutput_) && (pZcRelay->boardID_[1] > maxCount) && (pZcRelay->boardID_[1] != 0xFFFF))
			{
				maxCount = pZcRelay->boardID_[1];
			}
		}
	}
	return maxCount;
}

void CILKTestDoc::checkIOValid()
{
	// ���� Relay �� �ɼ��ӿں������ӿ���û�е�ַ��ͻ
	for (auto it = vRelay_.begin(); it != vRelay_.end(); it++)
	{
		for (auto jt = it + 1; jt != vRelay_.end(); jt++)
		{
			if ((*it)->deviceName_.Compare(L"9")==0 && (*jt)->deviceName_.Compare(L"D11")==0)
			{
				int a = 0;
			}

			if ((*it)->bInput_ && (*jt)->bInput_)
			{
				if (((*it)->boardID_[0] == (*jt)->boardID_[0]) && ((*it)->portID_[0] == (*jt)->portID_[0]) && (*it)->boardID_[0] != 0xff)
				{
					CString strErr;
					strErr.Format(L"�����λ����ȣ�%s %s (%s %s)", (*it)->deviceName_, (*it)->name_, (*jt)->deviceName_, (*jt)->name_);
					AfxGetMainWnd()->SendMessage(WM_OUT_MESSAGE, 1, (LPARAM)&strErr);
				}
			}

			if ((*it)->bOutput_ && (*jt)->bOutput_)
			{
				if (((*it)->boardID_[1] == (*jt)->boardID_[1]) && ((*it)->portID_[1] == (*jt)->portID_[1]) && (*it)->boardID_[1] != 0xff)
				{
					CString strErr;
					strErr.Format(L"�����λ����ȣ�%s %s (%s %s)", (*it)->deviceName_, (*it)->name_, (*jt)->deviceName_, (*jt)->name_);
					AfxGetMainWnd()->SendMessage(WM_OUT_MESSAGE, 1, (LPARAM)&strErr);
				}
			}
		}
	}
}


void CILKTestDoc::InitSockets()
{
	CString ioStrIP = L"";
	unsigned char* pIP = (unsigned char*)&netAddr_.localIP_;
	ioStrIP.Format(L"%u.%u.%u.%u", *(pIP + 3), *(pIP + 2), *(pIP + 1), *pIP);
	ioSock_.Close();
	ioSock_.InitSock(ioStrIP, netAddr_.localIOPort_);
	/*ioSock_.Bind(,);
	ioSock_.Create(netAddr_.localIOPort_, SOCK_DGRAM, FD_READ);*/

	CString atpStrIP = L"";
	unsigned char* pAtpIP = (unsigned char*)&netAddr_.LocalATPServerIP_;
	atpStrIP.Format(L"%u.%u.%u.%u", *(pAtpIP + 3), *(pAtpIP + 2), *(pAtpIP + 1), *pAtpIP);
	m_tcpSevSock_.Close();
	m_tcpSevSock_.InitSock(atpStrIP, netAddr_.localATPPort_);
}

void CILKTestDoc::updateIO(const char* pOutputData,int channel,int len)
{

	IOData recvData_;
	memset(&recvData_, 0, sizeof(IOData));
	switch (channel)
	{
	case 0:
	case 1:
		memcpy(&recvData_, pOutputData, sizeof(IOData));
		updateIOData(&recvData_, channel);
		break;
	case 2:
		if (m_isCnctToSAM)
		{
			m_3MProcess->SetRcvData(pOutputData, len);
		}
		break;
	case 3:
		if ((m_bIsCommToZC) && (m_iIsCnctToSamZC == 1))
		{
			if (recvDataHeadCheck(&pOutputData[20]))
			{
				updateCI2ZCData(&pOutputData[24], len);
			}
		}
	default:
		break;
	}
}

bool CILKTestDoc::recvDataHeadCheck(const char* pCiData)
{
	CString strErr;
	bool bVal = false;
	if ((pCiData[1] == ciIdWithZCSIM_) && (pCiData[0] == ciTypeWithZCSIM_) && (pCiData[2] == zcTypeWithZCSIM_) && (pCiData[3] == zcIdWithZCSIM_))
	{
		bVal = true;
		m_iDisConnCntWithZC = 0;
	}
	else
	{
		bVal = false;
		if (m_iDisConnCntWithZC >= MAX_CONNTIME_WITH_ZC)
		{
			strErr.Format(L"��ZCͨ��(����ͷ��ƥ�䣩: ���� = [%d, %d], ���� = [%d, %d]\n", pCiData[1], pCiData[3], zcIdWithZCSIM_, ciIdWithZCSIM_);
			AfxGetMainWnd()->SendMessage(WM_OUT_MESSAGE, 1, (LPARAM)&strErr);
		}
		else
		{
			m_iDisConnCntWithZC++;
		}
	}
	return bVal;
}
void CILKTestDoc::updateCI2ZCData(const char* pCiData, int len)
{
	BOOL bTemp = FALSE;
	int srcStat = 0;
	StationDev stationdev;
	for each(CZcCommRelay* pZcRelay in vZcCommRelay_)
	{
		if (pZcRelay->deviceName_.Right(1) == L'F')
		{
			continue;
		}
		if ((pZcRelay->bOutput_) && (pZcRelay->codeSeqIndex[1] != 0xFFFF))
		{
			srcStat = (pCiData[pZcRelay->boardID_[1]] >> pZcRelay->portID_[1] & 1);
			if ((pZcRelay->name_.Find(L"����ռ��") >= 0) || (pZcRelay->name_.Find(L"ESB����") >= 0))
			{
				srcStat = ((srcStat == 1) ? 0 : 1);
			}

			pZcRelay->setStat(srcStat, FALSE);
		}

		//����ѹ����·ͣ����֤��Ϣ��Ҫ���ڸ���
		updateInputByOutput(pZcRelay);
	}
}


void CILKTestDoc::updateInputByOutput(CZcCommRelay* pCurZcRelay)
{
	int iAuto, srcStat = 0;
	if (pCurZcRelay->bInput_)
	{
		if (pCurZcRelay->name_.Find(L"�г���ѹ") >= 0)
		{
			pCurZcRelay->setAutoLowStat(pCurZcRelay->stat_, m_iAutoUpdateZCStat_, 0, TRUE);
		}
		else if ((pCurZcRelay->name_.Find(L"_J_ͣ����֤") > 0) || (pCurZcRelay->name_.Find(L"_Z_ͣ����֤") > 0))
		{
			iAuto = ((m_iAutoUpdateZCStat_ & (!m_iManUpdateAswStat_)) ? 1 : 0);
			srcStat = getRouteSignAskStateInfo(pCurZcRelay);
			pCurZcRelay->setAutoLowStat(pCurZcRelay->stat_, iAuto, srcStat, TRUE);
		}
		else if (pCurZcRelay->name_.Find(L"�г�ͣ��") >= 0)
		{
			srcStat = getRouteSectRTLOCKStateInfo(pCurZcRelay);
			pCurZcRelay->setAutoLowStat(pCurZcRelay->stat_, m_iAutoUpdateZCStat_, srcStat, TRUE);
		}
	}
}


void CILKTestDoc::updateIOData(const IOData* pOutputData, int channel)
{
	
	if (!isProcessThisCnl(channel))
	{
		return;
	}
	// ���¼̵���״̬
	for each (Relay* pRelay in vRelay_)
	{
		if (pRelay->deviceName_.Right(1) == L'F')
		{
			continue;
		}
		if (pRelay->bOutput_)
		{
			// ���¼̵���״̬��CIOView �еļ̵���״̬
			
			pRelay->setStat((pOutputData->data_[pRelay->boardID_[1]] >> pRelay->portID_[1]) & 1, FALSE, TRUE); //�˴�����Relay����д��setStat����
		}
	}
}
void CILKTestDoc::packageIO(IOData* pInputData)
{
	for each (Relay* pRelay in vRelay_)
	{
		if (pRelay->bInput_)
		{
			if (pRelay->isUp())
			{
				pInputData->data_[pRelay->boardID_[0]] |= (1 << pRelay->portID_[0]);
			}
			else
			{
				pInputData->data_[pRelay->boardID_[0]] &= (~(1 << pRelay->portID_[0]));
			}
		}
	}
}

bool CILKTestDoc::isLogicHighState(CZcCommRelay* pCurZcRelay)
{
	bool bRet = false;
	if ((pCurZcRelay->name_.Find(L"ATռ��") >= 0) || (pCurZcRelay->name_.Find(L"UTռ��") >= 0))
	{
		if (pCurZcRelay->isLow())
		{
			bRet = true;
		}
	}
	else
	{
		if (pCurZcRelay->isHigh())
		{
			bRet = true;
		}
	}
	return bRet;
}

int CILKTestDoc::packZcSecndData(UINT8* pData/*, int& dataLen*/)
{
	int len = 5, num = 0;
	pData[0] = zcTypeWithZCSIM_;
	pData[1] = zcIdWithZCSIM_;
	pData[2] = ciTypeWithZCSIM_;
	pData[3] = ciIdWithZCSIM_;
	for each (CZcCommRelay* pZcCommRelay in vZcCommRelay_)
	{
		if ((pZcCommRelay->bInput_) && (pZcCommRelay->boardID_[0] != 0xFFFF) && (pZcCommRelay->portID_[0] != 0xFFFF) && (pZcCommRelay->codeSeqIndex[0] != 0xFFFF))
		{
			if (isLogicHighState(pZcCommRelay))
			{
				pData[4+pZcCommRelay->boardID_[0]] |= (1 << pZcCommRelay->portID_[0]);
			}
			else
			{
				pData[4+pZcCommRelay->boardID_[0]] &= (~(1 << pZcCommRelay->portID_[0]));
			}
			if ((pZcCommRelay->codeSeqIndex[0] > 0) && (pZcCommRelay->codeSeqIndex[0] != 0xFFFF) && (num < pZcCommRelay->codeSeqIndex[0]))
			{
				num = pZcCommRelay->codeSeqIndex[0];
			}
		}
	}

	len += (num / 8); //
	//dataLen = len;

	return len;
}


CADDevice* CILKTestDoc::findDeviceByName(CString& name)
{
	int islogic = m_isLogicSection;
	//auto it = find_if(vDevice_.begin(), vDevice_.end(), [&name, &islogic](CADDevice* pDevice) 
	for (int m = 0; m < vDevice_.size();m++)
	{
		//return pDevice->name_.text_.Compare(name) == 0;
		CADDevice* pDevice = vDevice_.at(m);
		if (islogic == 1)
		{
			if (CADSection* pSection = dynamic_cast<CADSection*>(pDevice))
			{
				if (pSection->logicDeviceIO_.name_ == name)
				{
					return pDevice;
				}
				else if (pDevice->/*name_.text_*/io_.getName().Compare(name) == 0)
				{
					return pDevice;//�����ң�ֻ���ҵ�����
				}
				else
				{
				
				}
			}
			else
			{
				if (pDevice->/*name_.text_*/io_.getName().Compare(name) == 0)
				{
					return pDevice;
				}
			}
		}
		else
		{
			if (pDevice->/*name_.text_*/io_.getName().Compare(name) == 0)
			{
				return pDevice;
			}
		}
	};

	return NULL ;
}

CADDevice* CILKTestDoc::findLogicDeviceByName(int addr, CString logicName, CRuntimeClass* pRuntimeClass)
{
	for (int m = 0; (m < vDevice_.size()) && (m_isLogicSection == 1); m++)
	{
		CADDevice* pDevice = vDevice_.at(m);
		if (CADSection* pSection = dynamic_cast<CADSection*>(pDevice))
		{
			CString strLogicName = pSection->io_.getName() + logicName;
			if ((addr == pSection->logicDeviceIO_.addr_) && ((pSection->logicDeviceIO_.name_ == logicName) || (pSection->logicDeviceIO_.name_ == strLogicName)))
			{
				return pDevice;
			}
			else
			{
			}
		}
	};

	return NULL;
}

DeviceIO CILKTestDoc::findSwitLogicIndexBySwitIndex2LogicName(int addr, int iNo, CString logicName, CRuntimeClass* pRuntimeClass)
{
	DeviceIO one;
	memset((void*)&one, 0, sizeof(one));
	one.addr_ = 0xFFFF;

	for (int m = 0; (m < vDevice_.size()) && (m_isLogicSection == 1); m++)
	{
		CADDevice* pDevice = vDevice_.at(m);
		if (RailSwitch* pSwitch = dynamic_cast<RailSwitch*>(pDevice))
		{
			CString strLogicName = pSwitch->logicDeviceIO_[iNo].getName() + logicName;
			if ((addr == pSwitch->io_.addr_) && CString(pSwitch->logicDeviceIO_[iNo].name_ ) != L"NO")// && ((pSwitch->logicDeviceIO_[iNo].name_ == logicName) || (pSwitch->logicDeviceIO_[iNo].name_ == strLogicName)))
			{
				return pSwitch->logicDeviceIO_[iNo];
			}
			else
			{
			}
		}
	};

	return one;
}


void CILKTestDoc::packageATP(UINT8* pData,INT32* len)
{

}


void CILKTestDoc::updateATP(UINT8* pData,INT32 len)
{
}


void CILKTestDoc::OnTimer()
{
	for each (Relay* pRelay in vRelay_)
	{
		if (pRelay->driveSecond_ >= 0)
		{
			pRelay->driveSecond_--;
			if (pRelay->driveSecond_ == 0)
			{
				// �����Ȳ������豸����Ϊһ���豸�п���ͬһ��������������̵��������¶��ˢ�¡�
				pRelay->setStat(pRelay->bDriveUp_, false);

				if (pRelay->bOutput_ && pRelay->pDevice_ != NULL)
				{
					pRelay->pDevice_->bStatChanged_ = TRUE;
				}
				
			}
		}
	}

	
	// ����豸�̵����仯ˢ���豸��ʾ
	for each (CADDevice* pDevice in vDevice_)
	{
		if (pDevice->bStatChanged_)
		{
			pDevice->setInputRelay();
			if (pDevice->IsKindOf(RUNTIME_CLASS(CADSection)))
			{
				for each (CADDevice* pDev in vDevice_)
				{
					if (pDevice->io_.getName().Left(pDevice->io_.getName().GetLength() - 1) == pDev->io_.getName().Left(pDev->io_.getName().GetLength() - 1))
					{
						pDev->Invalidate();
					}
				}
			}
			else
			{
				pDevice->Invalidate();
			}
			pDevice->bStatChanged_ = FALSE;
		}
	}

	for each (CADDevice* pDevice in vLingSanDev)
	{
		if (pDevice->bStatChanged_)
		{
			pDevice->setInputRelay();
			pDevice->Invalidate();
			pDevice->bStatChanged_ = FALSE;
		}
	}

}

void CILKTestDoc::setSectionRelayOut(SectionOutput* output)
{
	if (output->addr == UNUSERELAYOUTADDR || output->devJ1==UNUSERELAYOUTADDR)
	{
		return;
	}
	if (CADSection* pSection = (CADSection*)findDevcieByAddr(output->addr, RUNTIME_CLASS(CADSection)))
	{
		Relay* pRelay = new Relay(CString(output->name), CString(output->name), FALSE, TRUE, pSection);
		pRelay->boardID_[1] = boardID(output->devJ1);
		pRelay->portID_[1] = portID(output->devJ1);
		pRelay->stat_ = pRelay->lastStat_ = Relay::STAT_DOWN;
		pSection->vRelay_.push_back(pRelay);
		vRelay_.push_back(pRelay);
	}
	else
	{
		setSwitchSectionRelayOut(output);
	}
		
}

void CILKTestDoc::setSwitchSectionRelayOut(SectionOutput* output)
{
	for each (CADDevice* pDevice in vDevice_)
	{
		if (pDevice->IsKindOf(RUNTIME_CLASS(RailSwitch)))
		{
			RailSwitch* pSwitch = dynamic_cast<RailSwitch*>(pDevice);
			if (pSwitch->data_.section_.addr_ == output->addr)
			{
				Relay* pRelay = new Relay(CString(pSwitch->name_.text_), CString(output->name), FALSE, TRUE, pSwitch);
				pRelay->boardID_[1] = boardID(output->devJ1);
				pRelay->portID_[1] = portID(output->devJ1);
				pRelay->stat_ = pRelay->lastStat_ = Relay::STAT_DOWN;
				vRelay_.push_back(pRelay);
				pSwitch->vRelay_.push_back(pRelay);
				break;
			}
		}
	}

}


void CILKTestDoc::setLingSanRelayIn(LingSanInput* input)
{
	if (input->relayAddr_ != UNUSERELAYINADDR)
	{
		//վ̨�豸
		BOOL InitRelayUp=FALSE;
		CADStation *pStation = NULL;
		StationDev stationdev;
		stationdev = findStationDevByAddr(input->addr_, &InitRelayUp);//
		if (stationdev.pDev!=NULL)
		{
			SetStationRelay(input, stationdev, InitRelayUp);
		}
		else
		{
			stationdev = findStationPsdDevByName(CString(input->name_));
			if (stationdev.pDev != NULL)
			{
				SetStationRelay(input, stationdev, FALSE);
			}
		}

		//��ɢ
		CADDevice *pDev = NULL;
		if (stationdev.pDev == NULL)
		{
			pDev = findLingDevByRelay(input);
			if (pDev != NULL)
			{
				Relay* pRelay = new Relay(CString(input->name_), CString(input->name_), TRUE, FALSE, pDev);

				pRelay->boardID_[0] = boardID(input->relayAddr_);
				pRelay->portID_[0] = portID(input->relayAddr_);
				pRelay->stat_ = pRelay->lastStat_ = Relay::STAT_DOWN;

				pDev->vRelay_.push_back(pRelay);
				if (findLingsanDevByAddr(pDev->io_.addr_) == NULL)
				{
					vLingSanDev.push_back(pDev);      //��ɢ�豸
				}
				vRelay_.push_back(pRelay);// �̵��������豸������״̬����
			}
			else
			{
				Relay* pRelay = new Relay(CString(input->name_), CString(input->name_), TRUE, FALSE, pDev);

				pRelay->boardID_[0] = boardID(input->relayAddr_);
				pRelay->portID_[0] = portID(input->relayAddr_);
				pRelay->stat_ = pRelay->lastStat_ = Relay::STAT_DOWN;

				vRelay_.push_back(pRelay);// 
			}
			
		}		
	}
}


bool CILKTestDoc::isRelayExist(CString name)
{
	bool ret = false;
	int relayNum = vRelay_.size();
	for (int i = 0; i < relayNum; i++)
	{
		if (vRelay_.at(i)->name_ == name )
		{
			ret = true;
		}
	}
	return ret;
}


void CILKTestDoc::loadPourRelay()
{
	PourInput pourInputRelay[2] = {  { L"AGZJ", 0X0100 }, { L"BGZJ", 0x0101 } };
	PourInput pourOutputRelay[6] = { { L"AYUJ", 0X0000 }, { L"BYUJ", 0X0001 }, { L"AGZJ", 0X0002 }, { L"BGZJ", 0X0003 }, { L"AGZD", 0X0004 }, { L"BGZD", 0X0005 } };
	for (int i = 0; i < 2; i++)
	{
		Relay* pRelay = new Relay(L"", pourInputRelay[i].name, TRUE, TRUE, NULL);
		pRelay->boardID_[0] = boardID(pourInputRelay[i].relayAddr);
		pRelay->portID_[0] = portID(pourInputRelay[i].relayAddr);
		pRelay->boardID_[1] = boardID(pourOutputRelay[i + 2].relayAddr);
		pRelay->portID_[1] = portID(pourOutputRelay[i + 2].relayAddr);
		pRelay->stat_ = pRelay->lastStat_ = Relay::STAT_UP;
		pRelay->pDevice_ = NULL;  //��ɢ�̵���δ�����豸
		vRelay_.push_back(pRelay);
	}
	for (int i = 0; i < 6; i++)
	{
		if (i != 2 && i != 3)
		{
			Relay* pRelay = new Relay(L"", pourOutputRelay[i].name, FALSE, TRUE, NULL);
			pRelay->boardID_[1] = boardID(pourOutputRelay[i].relayAddr);
			pRelay->portID_[1] = portID(pourOutputRelay[i].relayAddr);
			pRelay->stat_ = pRelay->lastStat_ = Relay::STAT_UP;
			pRelay->pDevice_ = NULL;  //��ɢ�̵���δ�����豸
			vRelay_.push_back(pRelay);
		}
	}
}

void CILKTestDoc::OnMenuAtpShow()
{
	// TODO:  �ڴ���������������
	m_tcpSevSock_.showAllTestDlg();
}


CADDevice* CILKTestDoc::findDevcieByATPMsg(CString type, CString index)
{
	auto it = find_if(vDevice_.begin(), vDevice_.end(), [type, index](CADDevice* pDevice) {
		if (pDevice->eleType == type && pDevice->eleIndex == index)
		{
			return true;
		}
		return false;
	});

	return it == vDevice_.end() ? NULL : (*it);
}


StationDev CILKTestDoc::findStationDevByAddr(int addr,BOOL* bInitRelayUp)
{
	StationDev stationdev;
	stationdev.pDev = NULL;
	stationdev.pSta = NULL;
	if (addr ==UNUSERELAYINADDR)
	{
		return stationdev;
	}
	
	CADDevice *pdev = NULL;
	CADStation* pStation = NULL;
	vector<CADStation*>::size_type m;
	for (m = 0; m <vStation_.size(); m++)
	{
		pStation = vStation_.at(m);
		pdev = findStationESBByAddr(pStation, addr);
		if (pdev != NULL)
		{
			stationdev.pDev = pdev;
			stationdev.pSta = pStation;
			*bInitRelayUp = TRUE;
			break;
		}
		else
		{
			pdev = findStationPSDByAddr(pStation, addr, bInitRelayUp);
			if (pdev != NULL)
			{
				stationdev.pDev = pdev;
				stationdev.pSta = pStation;
				break;
			}
			else
			{
				pStation = NULL;
			}
		}
	}

	if (pdev==NULL) //DRB
	{
		pdev = findDevcieByAddr(addr, RUNTIME_CLASS(CADDeLight));
		if (pdev != NULL)
		{
			if (pdev->name_.text_.Left(3) == L"DRB")
			{
				pStation = findStationByNumb(pdev->io_.stationID_);
				stationdev.pDev = pdev;
				stationdev.pSta = pStation;
				*bInitRelayUp = FALSE;
			}
		}
		
	}

	return stationdev;
}
CADDevice* CILKTestDoc::findStationDevByAddr_F(int addr)
{
	if (addr == UNUSERELAYINADDR)
	{
		return NULL;
	}

	CADDevice *pdev = NULL;
	vector<CADStation*>::size_type m;
	for (m = 0; m <vStation_.size(); m++)
	{
		CADStation* pStation = vStation_.at(m);
		for (int i = 0; i < 4; i++)
		{
			if (pStation->btn_[i].io_.addr_ == addr)
			{
				return (CADDevice*)&(pStation->btn_[i+1]);
			}
		}
	}
	return pdev;
}


CADDevice* CILKTestDoc::findStationESBByAddr(const CADStation* pStation, const int addr)
{
	for (int i = 0; i < 4/*pStation->nBtn_*/; i++)
	{
		if (pStation->btn_[i].io_.addr_ == addr)
		{
			return (CADDevice*)&(pStation->btn_[i]);
		}
	}

	return NULL;
}

CADDevice* CILKTestDoc::findStationPSDByAddr(const CADStation* pStation, const int addr, BOOL* bInitRelayUp)
{
	int doorNo = 0;
	if (pStation->upDoor)
	{
		doorNo = 0;
		if (pStation->door_[doorNo].io_.addr_ == addr )
		{
			*bInitRelayUp = TRUE;
			return (CADDevice*)&(pStation->door_[doorNo]);
		}
		else if (pStation->door_[doorNo].io_.addr_ + 1 == addr)
		{
			*bInitRelayUp = FALSE;
			return (CADDevice*)&(pStation->door_[doorNo]);
		}
		else
		{

		}
		
	}
	if (pStation->downDoor)
	{
		doorNo = 1;
		if (pStation->door_[doorNo].io_.addr_ == addr)
		{
			*bInitRelayUp = TRUE;
			return (CADDevice*)&(pStation->door_[doorNo]);
		}
		else if (pStation->door_[doorNo].io_.addr_ + 1 == addr)
		{
			*bInitRelayUp = FALSE;
			return (CADDevice*)&(pStation->door_[doorNo]);
		}
		else
		{

		}
	}

	
	return NULL;
}

void CILKTestDoc::OutToFile(CString str)
{
	CTime tm = CTime::GetCurrentTime();
	CString strDateFileName = tm.Format("ATP_Log_%Y-%m-%d.txt");
	CStdioFile wfile;
	CFileFind fileFind;
	CString filePath = strPath + strDateFileName;

	if (!fileFind.FindFile(filePath)) // �����ļ��Ƿ����  
	{
		//������
		if (!wfile.Open(filePath, CFile::modeWrite | CFile::modeCreate | CFile::typeBinary)) //unicode �����Զ����ƴ�  
			return;
		WORD sign = 0xfeff;//unicode ��־
		wfile.Write(&sign, 2);

	}
	else
	{
		//����
		if (!wfile.Open(filePath, CFile::modeWrite | CFile::modeNoTruncate | CFile::typeBinary))
			return;
	}
	CString strTime = tm.Format("%Y-%m-%d %H:%M:%S :");

	wfile.SeekToEnd();
	if (isFirstOpenFile)
	{
		wfile.WriteString(strTime + L"/////////////////////////////////////////////////////////////\r\n\r\n");
		isFirstOpenFile = false;
	}
	strTime.Format(strTime + "ʱ�ӣ�%d ��", GetTickCount());
	wfile.WriteString(strTime + str);

	wfile.Close();
}


// ����վ�Ų���վ̨
CADStation* CILKTestDoc::findStationByNumb(int numb)
{
	auto it = find_if(vStation_.begin(), vStation_.end(), [numb](CADStation* pStation) {
		if (pStation->stationID_ == numb)
		{
			return true;
		}
		return false;
	});
	return it == vStation_.end() ? NULL : (*it);
}


void CILKTestDoc::SetStationRelay(LingSanInput* input, StationDev stationDev, BOOL bRelayUp)
{
	Relay* pRelay = new Relay(CString(input->name_), CString(input->name_), TRUE, FALSE, stationDev.pDev);

	pRelay->boardID_[0] = boardID(input->relayAddr_);
	pRelay->portID_[0] = portID(input->relayAddr_);
	if (bRelayUp)
	{
		pRelay->stat_ = pRelay->lastStat_ = Relay::STAT_UP;
	}
	else
	{
		pRelay->stat_ = pRelay->lastStat_ = Relay::STAT_DOWN;
	}
	

	if (stationDev.pDev != NULL)
	{
		stationDev.pDev->vRelay_.push_back(pRelay);  //�豸��ָ������״̬��ʾ
		if (findLingsanDevByAddr(stationDev.pDev->io_.addr_) == NULL)
		{
			vLingSanDev.push_back(stationDev.pDev);      //��ɢ�豸
		}
		

		stationDev.pSta->vRelay_.push_back(pRelay);  //վ̨��̵���������ʾѡ��
	}

	vRelay_.push_back(pRelay);// �̵��������豸������״̬����

	CADDevice *pDevF = findStationDevByAddr_F(input->addr_);  // һ���̵�����Ӧ����豸��ESB ��ʾ�豸������״̬���£�
	if (pDevF != NULL)
	{
		pDevF->vRelay_.push_back(pRelay);
		pRelay->SetFDevice(pDevF);
	}
}


CADDevice* CILKTestDoc::findAfgDoorByAddr(int addr)
{
	auto it = find_if(vDevice_.begin(), vDevice_.end(), [&addr](CADDevice* pDevice) {
		if (pDevice->IsKindOf(RUNTIME_CLASS(CADAfgDoor)))
		{
			return addr >= pDevice->io_.addr_ && addr <= pDevice->io_.addr_+2;
		}
		return false;
	});

	return it == vDevice_.end() ? NULL : (*it);
}
CADDevice* CILKTestDoc::findDriByAddr(int addr)
{
	//�����������ơ��ȵ�˳������
	auto it = find_if(vDevice_.begin(), vDevice_.end(), [&addr](CADDevice* pDevice) {
		if (pDevice->IsKindOf(RUNTIME_CLASS(CADDeLight)))
		{
			if (pDevice->name_.text_.Left(3) == L"DRI")
			{
				return addr == pDevice->io_.addr_ || addr == pDevice->io_.addr_ + 1;
			}
			
		}
		return false;
	});

	return it == vDevice_.end() ? NULL : (*it);
}

CADDevice* CILKTestDoc::findDriByNum(int iNo)
{
	int iTimes = 0, i = 0;
	CADDevice* pDevice = NULL;
	for (i = 0; i < vDevice_.size(); i++)
	{
		pDevice = vDevice_.at(i);
		if (pDevice->IsKindOf(RUNTIME_CLASS(CADDeLight)))
		{
			if ((pDevice->io_.getName().Find(L"WDJ") >= 0) || (pDevice->io_.getName().Left(3).Find(L"DRB") >= 0))
			{
				iTimes++;
				if (iTimes == iNo)
				{
					return pDevice;
				}
			}
		}
	}
	
	return NULL;
}


// ���ڴ洢KMJ��GMJ�����������ŵķ���false
bool CILKTestDoc::addToDevByRelayName(OtherOutput* pOutput)
{
	CString relayName(CString(pOutput->name));
	for each(CADDevice* pDevice in vLingSanDev)
	{
		for (int i = 0; i < pDevice->vRelay_.size(); i++)
		{
			if (pDevice->vRelay_.at(i)->name_ == relayName)
			{
				pDevice->vRelay_[i]->bOutput_ = true;
				pDevice->vRelay_[i]->boardID_[1] = boardID(pOutput->devJ1);
				pDevice->vRelay_[i]->portID_[1] = portID(pOutput->devJ1);
				return true;
			}
		}
	}

	for each(CADDevice* pDevice in vDevice_)
	{
		for (int i = 0; i < pDevice->vRelay_.size(); i++)
		{
			if (pDevice->vRelay_.at(i)->name_ == relayName)
			{
				pDevice->vRelay_[i]->bOutput_ = true;
				pDevice->vRelay_[i]->boardID_[1] = boardID(pOutput->devJ1);
				pDevice->vRelay_[i]->portID_[1] = portID(pOutput->devJ1);
				return true;
			}
		}
	}


	for each(Relay* pRelay in vRelay_)
	{
		if (pRelay->name_ == relayName)
		{
			pRelay->bOutput_ = true;
			pRelay->boardID_[1] = boardID(pOutput->devJ1);
			pRelay->portID_[1] = portID(pOutput->devJ1);
			return true;
		}
	}

	return false;
}

//PSD�� KMJ��GMJ
StationDev CILKTestDoc::findStationPsdDevByName(CString relayName)
{
	StationDev stationdev;
	stationdev.pDev = NULL;
	stationdev.pSta = NULL;
	if (relayName.Left(3) != L"PSD")
	{
		return stationdev;
	}

	CADDevice *pdev = NULL;
	CADStation* pStation = NULL;
	int psdDoorNumb;
	vector<CADStation*>::size_type m;
	for (m = 0; m <vStation_.size(); m++)
	{
		pStation = vStation_.at(m);
		CString psdName = findStationPsdName(pStation, &psdDoorNumb);
		if (psdName.GetLength()>0)
		{
			if (psdName == getRelayDevName(relayName))
			{
				stationdev.pDev = (CADDevice*)&(pStation->door_[psdDoorNumb]);
				stationdev.pSta = pStation;
				return  stationdev;
			}
		}
	}

	return stationdev;
}

//���ݼ̵������ƻ�ȡ�豸����
CString CILKTestDoc::getRelayDevName(CString psdRelayName)
{
	CString psdName(L"");
	int mm = psdRelayName.Find('-');
	if (mm>0)
	{
		psdName = psdRelayName.Left(mm);
	}
	return psdName;
}


CString CILKTestDoc::findStationPsdName(CADStation* pStation,int* psdDoorNumb)
{
	CString psdName(L"");
	for (int i = 0; i < 2; i++)
	{
		for (int n = 0; n < pStation->door_[i].vRelay_.size(); n++)
		{
			psdName = getRelayDevName(pStation->door_[i].vRelay_.at(n)->deviceName_);
			if (psdName.GetLength()>0)
			{
				*psdDoorNumb = i;
				return psdName;
			}
		}
	}
	return psdName;
}


CADDevice* CILKTestDoc::findSwitchBySectAddr(UINT16 addr)
{
	auto it = find_if(vDevice_.begin(), vDevice_.end(), [addr](CADDevice* pDevice) {
		if (pDevice->IsKindOf(RUNTIME_CLASS(RailSwitch)))
		{
			RailSwitch* pSwitch = dynamic_cast<RailSwitch*> (pDevice);
			if (pSwitch->data_.section_.addr_ == addr)
			{
				return true;
			}
		}
		return false;
	});

	return it == vDevice_.end() ? NULL : (*it);
}

CADDevice* CILKTestDoc::findSwitchBySectEleIndex(CString  eleIndex)
{
	auto it = find_if(vDevice_.begin(), vDevice_.end(), [eleIndex](CADDevice* pDevice) {
		if (pDevice->IsKindOf(RUNTIME_CLASS(RailSwitch)))
		{
			RailSwitch* pSwitch = dynamic_cast<RailSwitch*> (pDevice);
			if (pSwitch->switchSectionEleIndex == eleIndex)
			{
				return true;
			}
		}
		return false;
	});

	return it == vDevice_.end() ? NULL : (*it);
}

void CILKTestDoc::OnDeviceTimer()
{
	for each(CADDevice* pDevice in vDevice_)
	{
		if (pDevice->m_bFlash)
		{
			pDevice->m_nflashCount++;
			pDevice->Invalidate();
		}
		else
		{
			if (pDevice->m_nflashCount > 0)
			{
				pDevice->m_nflashCount = 0;
				pDevice->Invalidate();
			}
		}
		
	}
}


CADDevice* CILKTestDoc::findLingDevByRelay(LingSanInput* input)
{
	CADDevice *pDev = NULL;
	//������
	if (pDev = findAfgDoorByAddr(input->addr_))
	{
	}
	else if (pDev = findDriByAddr(input->addr_))
	{

	}
	else if (pDev = findDevcieByAddr(input->addr_, RUNTIME_CLASS(CADDeLight)))
	{
	}
	else if (pDev = findDevcieByAddr(input->addr_, RUNTIME_CLASS(CADButton)))
	{

	}
	else
	{
		
	}
	////�����豸������
	//CString devName = getRelayDevName(CString(input->name_));
	//pDev = findDeviceByName(devName);
	return pDev;
}


// ��ȡ���������ļ�
void CILKTestDoc::loadNetConfig(CString path)
{
	CString FilePath = path;
	FilePath += L"simconfig.txt";
	CFile File;
	if (!File.Open(FilePath, CFile::modeRead))
	{
		::MessageBox(AfxGetMainWnd()->GetSafeHwnd(), L"ϵͳȱ�������ļ� simconfig.txt", L"��ʾ��Ϣ", MB_OK);
		exit(0);
	}
	File.Close();

	CString str = L"";
	str = L"";
	GetPrivateProfileString(L"������ͨ����������", L"����IP-A", L"127.0.0.1", str.GetBuffer(20), 20, FilePath);
	str.ReleaseBuffer();
	netAddr_.localIP_Me[0] = str;
	netAddr_.localIOPort_Me[0] = GetPrivateProfileInt(L"������ͨ����������", L"����PORT-A", 6001, FilePath);

	str = L"";
	GetPrivateProfileString(L"������ͨ����������", L"����IP-B", L"127.0.0.1", str.GetBuffer(20), 20, FilePath);
	str.ReleaseBuffer();
	netAddr_.localIP_Me[1] = str;
	netAddr_.localIOPort_Me[1] = GetPrivateProfileInt(L"������ͨ����������", L"����PORT-B", 6002, FilePath);

	str = L"";
	GetPrivateProfileString(L"������ͨ����������", L"������IP-A", L"127.0.0.1", str.GetBuffer(20), 20, FilePath);
	str.ReleaseBuffer();
	netAddr_.remoteIP_CI[0] = str;
	netAddr_.remotePort_CI[0] = GetPrivateProfileInt(L"������ͨ����������", L"������PORT-A", 6000, FilePath);

	str = L"";
	GetPrivateProfileString(L"������ͨ����������", L"������IP-B", L"127.0.0.1", str.GetBuffer(20), 20, FilePath);
	str.ReleaseBuffer();
	netAddr_.remoteIP_CI[1] = str;
	netAddr_.remotePort_CI[1] = GetPrivateProfileInt(L"������ͨ����������", L"������PORT-B", 4000, FilePath);

	str = L"";
	GetPrivateProfileString(L"�п�����ͨ����������", L"����IP", L"127.0.0.1", str.GetBuffer(20), 20, FilePath);
	str.ReleaseBuffer();
	netAddr_.localIP_3M = str;
	netAddr_.localPort_3M = GetPrivateProfileInt(L"�п�����ͨ����������", L"����PORT", 14000, FilePath);
	
	str = L"";
	m_isCnctToSAM = GetPrivateProfileInt(L"�п�����ͨ����������", L"�Ƿ�����п������ӿ�", 0, FilePath);

	GetPrivateProfileString(L"�п�����ͨ����������", L"�Է�IP", L"127.0.0.1", str.GetBuffer(20), 20, FilePath);
	str.ReleaseBuffer();
	netAddr_.remoteIP_3M = str;
	netAddr_.remotePort_3M= GetPrivateProfileInt(L"�п�����ͨ����������", L"�Է�PORT", 24000, FilePath);
	
	ciIdWith3M= GetPrivateProfileInt(L"�п�����ͨ����������", L"�������", 0, FilePath);

	str = L"";
	m_isCnctToGalaxy= GetPrivateProfileInt(L"Galaxyͨ����������", L"�Ƿ����Galaxy", 0, FilePath);

	GetPrivateProfileString(L"Galaxyͨ����������", L"ATPͨ�ű���IP", L"127.0.0.1", str.GetBuffer(20), 20, FilePath);
	str.ReleaseBuffer();
	netAddr_.localIP_ATP_Me = str;
	netAddr_.localATPPort_Me = GetPrivateProfileInt(L"Galaxyͨ����������", L"ATPͨ�ű���PORT", 9000, FilePath);
	ixlIDForGalaxy = GetPrivateProfileInt(L"Galaxyͨ����������", L"����ID", 5, FilePath);

	m_isLogicSection = GetPrivateProfileInt(L"������������", L"�Ƿ��߼�����վ��", 10, FilePath);
	driveTimeCount = GetPrivateProfileInt(L"������������", L"��������ʱ��", 30, FilePath);
	//driveTimeCount = driveTimeCount * 10;
	m_nUpTime = GetPrivateProfileInt(L"������������", L"�̵�������ʱ��", 2, FilePath);
	m_nDownTime = GetPrivateProfileInt(L"������������", L"�̵�������ʱ��", 2, FilePath);

	/*WCU_ATP����IP����*/
	m_IsCnctToWcuATP = GetPrivateProfileInt(L"3����WCU_ATP����ͨ����������", L"�Ƿ���WCU_ATPͨ��", 0, FilePath);
	str = L"";
	GetPrivateProfileString(L"3����WCU_ATP����ͨ����������", L"����IP", L"127.0.0.1", str.GetBuffer(20), 20, FilePath);
	str.ReleaseBuffer();
	netAddr_.localIP_WcuATP = str;
	netAddr_.localPort_WcuATP = GetPrivateProfileInt(L"3����WCU_ATP����ͨ����������", L"����PORT", 82010, FilePath);
	str = L"";
	GetPrivateProfileString(L"3����WCU_ATP����ͨ����������", L"�Է�IP", L"127.0.0.1", str.GetBuffer(20), 20, FilePath);
	str.ReleaseBuffer();
	netAddr_.remoteIP_WcuATP = str;
	netAddr_.remotePort_WcuATP = GetPrivateProfileInt(L"3����WCU_ATP����ͨ����������", L"�Է�PORT", 82011, FilePath);

	/*ZC����IP����*/
	m_iIsCnctToSamZC = GetPrivateProfileInt(L"4����ZC����ͨ����������", L"�Ƿ���ZCͨ��", 0, FilePath);
	str = L"";
	GetPrivateProfileString(L"4����ZC����ͨ����������", L"����IP", L"127.0.0.1", str.GetBuffer(20), 20, FilePath);
	str.ReleaseBuffer();
	netAddr_.localIP_ZC = str;
	netAddr_.localPort_ZC = GetPrivateProfileInt(L"4����ZC����ͨ����������", L"����PORT", 82010, FilePath);

	str = L"";
	GetPrivateProfileString(L"4����ZC����ͨ����������", L"�Է�IP", L"127.0.0.1", str.GetBuffer(20), 20, FilePath);
	str.ReleaseBuffer();
	netAddr_.remoteIP_ZC = str;
	netAddr_.remotePort_ZC = GetPrivateProfileInt(L"4����ZC����ͨ����������", L"�Է�PORT", 82011, FilePath);

	zcTypeWithZCSIM_ = GetPrivateProfileInt(L"4����ZC����ͨ����������", L"ZC�豸����", 30, FilePath);
	ciTypeWithZCSIM_ = GetPrivateProfileInt(L"4����ZC����ͨ����������", L"�����豸����", 60, FilePath);
	zcIdWithZCSIM_ = GetPrivateProfileInt(L"4����ZC����ͨ����������", L"ZC��ID", 1, FilePath);
	ciIdWithZCSIM_ = GetPrivateProfileInt(L"4����ZC����ͨ����������", L"����ID", 1, FilePath);
	m_iAutoUpdateZCStat_ = GetPrivateProfileInt(L"4����ZC����ͨ����������", L"�Ƿ��Զ�����ZC״̬", 0, FilePath);
	m_iManUpdateAswStat_ = GetPrivateProfileInt(L"4����ZC����ͨ����������", L"�ֶ�����ͣ����֤״̬", 1, FilePath);
}


void CILKTestDoc::InitSocketByCfg()
{
	for (int i = 0; i < 2;i++)
	{
		m_pioSock[i]->Close();
		m_pioSock[i]->InitSock(netAddr_.localIP_Me[i], netAddr_.localIOPort_Me[i], netAddr_.remoteIP_CI[i], netAddr_.remotePort_CI[i]);
	}

	m_tcpSevSock_.Close();

	if (m_isCnctToGalaxy>0)
	{
		m_tcpSevSock_.InitSock(netAddr_.localIP_ATP_Me, netAddr_.localATPPort_Me);
	}

	
	m_p3MSock->Close();
	if (m_isCnctToSAM>0)
	{
		m_p3MSock->InitSock(netAddr_.localIP_3M, netAddr_.localPort_3M, netAddr_.remoteIP_3M, netAddr_.remotePort_3M);
	}

	m_pWcuATPSock->Close();
	if (m_IsCnctToWcuATP > 0)
	{
		m_pWcuATPSock->InitSock(netAddr_.localIP_WcuATP, netAddr_.localPort_WcuATP, netAddr_.remoteIP_WcuATP, netAddr_.remotePort_WcuATP);
	}
	
	m_pZcSock_->Close();
	if (m_iIsCnctToSamZC == 1)
	{
		m_pZcSock_->InitSock(netAddr_.localIP_ZC, netAddr_.localPort_ZC, netAddr_.remoteIP_ZC, netAddr_.remotePort_ZC);
	}
}



// �豸�Ƿ����߼�����A��������
BOOL CILKTestDoc::isNameRightA(CString name)
{
	BOOL tempRet = FALSE;
	if (m_isLogicSection ==1)
	{
		if (name.Right(1) == L"A")
		{
			tempRet = TRUE;
		}
		else
		{
			tempRet = FALSE;
		}
	}
	else
	{
		tempRet = TRUE;
	}
	
	return tempRet;
}


CADDevice* CILKTestDoc::findDevByBtnID(UINT16 btnId)
{
	auto it = find_if(vDevice_.begin(), vDevice_.end(), [&btnId](CADDevice* pDevice) {
		if (pDevice->io_.btnID_ == btnId)
		{
			return true;
		}
		return false;
	});

	return it == vDevice_.end() ? NULL : (*it);
}

BOOL CILKTestDoc::isProcessThisCnl(int channel)
{
	if (m_pioSock[0]->m_isConnected && m_pioSock[1]->m_isConnected)
	{
		if (channel==0)  //˫������ֻ����A��
		{
			return TRUE;
		}
	}
	else
	{
		for (int i = 0; i < 2;i++)
		{
			if (m_pioSock[i]->m_isConnected && channel==i)
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}

void CILKTestDoc::load3MData(CString strPath){
	CString inputFileName = strPath + L"x86\\deviceToAtpTable.le";//L"x86\\ATP\\deviceToAtpTable.le";
	CFile file;
	int dataSize = sizeof(ToSimCfgData);
	if (file.Open(inputFileName, CFile::modeRead | CFile::typeBinary))
	{ //���ļ�

		if (m_isCnctToSAM<=0)
		{
			CString strErr(L"��˲������ļ�<�Ƿ����3M�ӿ�>�������ã���ɾ����");
			strErr.Append(inputFileName);
			//AfxGetMainWnd()->SendMessage(WM_OUT_MESSAGE, 1, (LPARAM)&strErr);

			AfxMessageBox(strErr);
			file.Close();
			return;
		}

		int nDevTotal=0;
		file.Read(&nDevTotal, 4);
		TRACE1("\n nDevTotal = %d\n", nDevTotal);
		ToSimCfgData devData;
		while (file.Read(&devData, dataSize) > 0)
		{
			vToSimCfgData.push_back(devData);
		}
		StartMap3MDevData();

		file.Close();
	}
	else
	{
		if (m_isCnctToSAM>0)
		{
			CString strErr(L"���ļ�ʧ��,��˲�:\n");
			strErr.Append(L"1��" + inputFileName);
			//AfxGetMainWnd()->SendMessage(WM_OUT_MESSAGE, 1, (LPARAM)&strErr);

			AfxMessageBox(strErr + L"�ļ��Ƿ����\n,2�������ļ�<�Ƿ����3M�ӿ�>��������");
			exit(0);
		}
	}
}
void CILKTestDoc::loadOlpData(CString strPath)
{
	CString inputFileName = strPath + L"x86\\overlapTable.le";
	CFile file;
	int dataSize = sizeof(StProSectionData_EX2);
	if (file.Open(inputFileName, CFile::modeRead | CFile::typeBinary))
	{ //���ļ�
		int nDevTotal = 0;
		file.Read(&nDevTotal, 4);
		TRACE1("\n overlap nDevTotal = %d\n", nDevTotal);
		StProSectionData_EX2 devData;
		while (file.Read(&devData, dataSize) > 0)
		{
			vOverlapTable_.push_back(devData);
		}
		
		file.Close();
	}
	else
	{
		CString strErr(L"���ļ�ʧ�ܣ�");
		strErr.Append(inputFileName);
		AfxGetMainWnd()->SendMessage(WM_OUT_MESSAGE, 1, (LPARAM)&strErr);
	}
}

void CILKTestDoc::loadRouteTableData(CString strPath)
{
	CString inputFileName = strPath + L"x86\\table.le";
	CFile file;
	int dataSize = sizeof(RtOne);
	if (file.Open(inputFileName, CFile::modeRead | CFile::typeBinary))
	{ //���ļ�
		int nDevTotal = 0;
		file.Read(&nDevTotal, 4);
		TRACE1("\n table nDevTotal = %d\n", nDevTotal);
		RtOne devData;
		while (file.Read(&devData, dataSize) > 0)
		{
			vRtOne_.push_back(devData);
		}
		file.Close();
	}
	else
	{
		CString strErr(L"���ļ�ʧ�ܣ�");
		strErr.Append(inputFileName);
		AfxGetMainWnd()->SendMessage(WM_OUT_MESSAGE, 1, (LPARAM)&strErr);
	}
}

void CILKTestDoc::loadRouteAddrData(CString strPath)
{
	CString inputFileName = strPath + L"x86\\tableAddr.le";
	CFile file;
	int dataSize = sizeof(ADDR_UNIT_RT);
	if (file.Open(inputFileName, CFile::modeRead | CFile::typeBinary))
	{ //���ļ�
		int nDevTotal = 0;
		file.Read(&nDevTotal, 4);
		TRACE1("\n tableAddr nDevTotal = %d\n", nDevTotal);
		ADDR_UNIT_RT devData;
		while (file.Read(&devData, dataSize) > 0)
		{
			vRouteAddr_.push_back(devData);
		}
		file.Close();
	}
	else
	{
		CString strErr(L"���ļ�ʧ�ܣ�");
		strErr.Append(inputFileName);
		AfxGetMainWnd()->SendMessage(WM_OUT_MESSAGE, 1, (LPARAM)&strErr);
	}
}

void CILKTestDoc::loadPsdCfgData(CString strPath)
{
	CString inputFileName = strPath + L"x86\\psdInfo.le";
	CFile file;
	int dataSize = sizeof(PsdCfgData);
	if (file.Open(inputFileName, CFile::modeRead | CFile::typeBinary))
	{ //���ļ�
		int nDevTotal = 0;
		file.Read(&nDevTotal, 4);
		TRACE1("\n psdCfgData nDevTotal = %d\n", nDevTotal);
		PsdCfgData devData;
		while (file.Read(&devData, dataSize) > 0)
		{
			vPsdCfgData_.push_back(devData);
		}
		file.Close();
	}
	else
	{
		CString strErr(L"���ļ�ʧ�ܣ�");
		strErr.Append(inputFileName);
		AfxGetMainWnd()->SendMessage(WM_OUT_MESSAGE, 1, (LPARAM)&strErr);
	}
}

void CILKTestDoc::loadLogicTvsCfgData(CString strPath)
{
	CString inputFileName = strPath + L"x86\\PhyCfgTable.le";
	CFile file;
	int dataSize = sizeof(Phy2LogicCfg);
	if (file.Open(inputFileName, CFile::modeRead | CFile::typeBinary))
	{ //���ļ�
		int nDevTotal = 0;
		file.Read(&nDevTotal, 4);
		TRACE1("\n PhyCfgTable nDevTotal = %d\n", nDevTotal);
		Phy2LogicCfg devData;
		while (file.Read(&devData, dataSize) > 0)
		{
			vPhy2Logic_.push_back(devData);
		}
		file.Close();
	}
	else
	{
		CString strErr(L"���ļ�ʧ�ܣ�");
		strErr.Append(inputFileName);
		AfxGetMainWnd()->SendMessage(WM_OUT_MESSAGE, 1, (LPARAM)&strErr);
	}
}

void CILKTestDoc::load3MZCData(CString strPath)
{
	CString inputFileName = strPath + L"x86\\atpInterfaceTable.le";
	CFile file;
	int dataSize = sizeof(ZCInputCfgData);
	if (file.Open(inputFileName, CFile::modeRead | CFile::typeBinary))
	{ //���ļ�
		int nDevTotal = 0;
		file.Read(&m_ZcConnHead_, sizeof(m_ZcConnHead_));
		file.Read(&nDevTotal, 4);
		TRACE1("\n atpInterfaceTable nDevTotal = %d\n", nDevTotal);
		ZCInputCfgData devData;
		while (file.Read(&devData, dataSize) > 0)
		{
			vZcInputCfgData_.push_back(devData);
		}
		file.Close();
	}
	else
	{
		CString strErr(L"���ļ�ʧ�ܣ�");
		strErr.Append(inputFileName);
		AfxGetMainWnd()->SendMessage(WM_OUT_MESSAGE, 1, (LPARAM)&strErr);
	}
}

bool CILKTestDoc::checkZcConnHead()
{
	bool bVal = true;
	CString strErr;
		
	if (m_iIsCnctToSamZC == 1)
	{
		if ((bVal) && (m_ZcConnHead_.ciId != ciIdWithZCSIM_))
		{
			bVal = false;
		}
		if ((bVal) && (m_ZcConnHead_.zcId != zcIdWithZCSIM_))
		{
			bVal = false;
		}

		if ((bVal) && (m_ZcConnHead_.ciType != ciTypeWithZCSIM_))
		{
			bVal = false;
		}
		if ((bVal) && (m_ZcConnHead_.zcType != zcTypeWithZCSIM_))
		{
			bVal = false;
		}
		if (!bVal)
		{
			strErr.Format(L"ZCͨ������Э��ͷ(ciID, zcID)��simconfig[%d, %d], atpInterfaceTable[%d, %d]\n",
				ciIdWithZCSIM_, zcIdWithZCSIM_, m_ZcConnHead_.ciId, m_ZcConnHead_.zcId);
			AfxGetMainWnd()->SendMessage(WM_OUT_MESSAGE, 1, (LPARAM)&strErr);
		}
	}

	return bVal;
}

void CILKTestDoc::setZcCommRelay()
{
	ZCInputCfgData devData;

	for (int i = 0; i < vZcInputCfgData_.size(); i++)
	{
		devData = vZcInputCfgData_.at(i);
		switch (devData.devType)
		{
		case DEV_TYPE_SWIT_LOCK: //ci
			setSwitSectLogicZcCommRelay(devData);
			break;
		case DEV_TYPE_LTE_ATOCP: //zc
		case DEV_TYPE_LTE_UTOCP: //zc
			setLogicZcCommRelay(devData);
			break;
		default:
			break;
		}
	}

	for (int i = 0; i < vZcInputCfgData_.size(); i++)
	{
		devData = vZcInputCfgData_.at(i);
		switch (devData.devType)
		{
		case DEV_TYPE_PHY_ARB: //zc
			setPhySectZcCommRelay(devData);
			break;
		default:
			break;
		}
	}
	for (int i = 0; i < vZcInputCfgData_.size(); i++)
	{
		devData = vZcInputCfgData_.at(i);
		switch (devData.devType)
		{
		case DEV_TYPE_PHY_STOP: //zc
			setPhySectZcCommRelay(devData);
			break;
		default:
			break;
		}
	}

	for (int i = 0; i < vZcInputCfgData_.size(); i++)
	{
		devData = vZcInputCfgData_.at(i);
		switch (devData.devType)
		{
		case DEV_TYPE_SIGN_OVEROCCUPIED: //zc
			setSignalZcCommRelay(devData);
			break;
		default:
			break;
		}
	}
	for (int i = 0; i < vZcInputCfgData_.size(); i++)
	{
		devData = vZcInputCfgData_.at(i);
		switch (devData.devType)
		{
		case DEV_TYPE_SIGN_LIGHTSTATE: //zc
			setSignalZcCommRelay(devData);
			break;
		default:
			break;
		}
	}
	for (int i = 0; i < vZcInputCfgData_.size(); i++)
	{
		devData = vZcInputCfgData_.at(i);
		switch (devData.devType)
		{
		case DEV_TYPE_ROUTE_STOPASW:
		case DEV_TYPE_ATBRT_STOPASW:
			setRtStartSignZcCommRelay(devData);
			break;
		default:
			break;
		}
	}

	for (int i = 0; i < vZcInputCfgData_.size(); i++)
	{
		devData = vZcInputCfgData_.at(i);
		switch (devData.devType)
		{
		case DEV_TYPE_SIGN_OPENSTATE: //ci
			setSignalZcCommRelay(devData);
			break;
		case DEV_TYPE_PHY_SLOCK: //ci
		case DEV_TYPE_PHY_XLOCK: //ci
		case DEV_TYPE_PHY_OCCUPIED: //ci
		case DEV_TYPE_PHY_RTLOCK: //ci
			setPhySectZcCommRelay(devData);
			break;
		case DEV_TYPE_SWIT_NORMAL: //ci
		case DEV_TYPE_SWIT_REVERSE: //ci
		case DEV_TYPE_SWIT_LOCK: //ci
			setSwitchZcCommRelay(devData);
			break;
		case DEV_TYPE_ROUTE_STATE:
		case DEV_TYPE_ATBRT_STATE:
		case DEV_TYPE_ROUTE_STOPASK:
		case DEV_TYPE_ATBRT_STOPASK:
			setRtStartSignZcCommRelay(devData);
			break;
		case DEV_TYPE_OLP_STATE:
			setOlpSignalZcCommRelay(devData);
			break;
		case DEV_TYPE_PSD_STATE: //ci
		case DEV_TYPE_ESB_STATE: //ci
		case DEV_TYPE_ATB_STATE: //ci
			setPlatformZcCommRelay(devData);
			break;
		case DEV_TYPE_ATB_FLASH: //zc
		case DEV_TYPE_ATB_STABLE: //zc
			setOtherDRBZcCommRelay(devData);
			break;
		//case DEV_TYPE_ATB_FLASH: //zc
		//case DEV_TYPE_ATB_STABLE: //zc
		//	setOtherZcCommRelay(devData);
		//	break;
		case DEV_TYPE_AFG_STATE: //ci
			setAfgZcCommRelay(devData);
			break;
		default:
			break;
		}
	}
}

CZcCommRelay* CILKTestDoc::isZcCommRelayExsist(int devType, int devIndex)
{
	for each(CADDevice* pDevice in vDevice_)
	{
		for (int i = 0; i < pDevice->vZcCommRelay_.size(); i++)
		{
			switch (devType)
			{
			case DEV_TYPE_SWIT_NORMAL: //ci
			case DEV_TYPE_SWIT_REVERSE: //ci
			case DEV_TYPE_SWIT_LOCK: //ci
				if ((pDevice->vZcCommRelay_.at(i)->devType == T_ZC_SWIT) && (pDevice->io_.addr_))
				{
					return pDevice->vZcCommRelay_.at(i);
				}
				break;
			case DEV_TYPE_SIGN_OPENSTATE: //ci
			case DEV_TYPE_SIGN_OVEROCCUPIED: //zc
			case DEV_TYPE_SIGN_LIGHTSTATE: //zc

			case DEV_TYPE_ROUTE_STOPASW:
			case DEV_TYPE_ATBRT_STOPASW:
			case DEV_TYPE_ROUTE_STATE:
			case DEV_TYPE_ATBRT_STATE:
			case DEV_TYPE_ROUTE_STOPASK:
			case DEV_TYPE_ATBRT_STOPASK:
			case DEV_TYPE_OLP_STATE:
				if ((pDevice->vZcCommRelay_.at(i)->devType == T_ZC_SIGN) && (pDevice->io_.addr_))
				{
					return pDevice->vZcCommRelay_.at(i);
				}
				break;
			case DEV_TYPE_PHY_SLOCK: //ci
			case DEV_TYPE_PHY_XLOCK: //ci
			case DEV_TYPE_PHY_OCCUPIED: //ci
			case DEV_TYPE_PHY_RTLOCK: //ci
			case DEV_TYPE_PHY_ARB: //zc
			case DEV_TYPE_PHY_STOP: //zc
				if ((pDevice->vZcCommRelay_.at(i)->devType == T_ZC_PHY) && (pDevice->io_.addr_))
				{
					return pDevice->vZcCommRelay_.at(i);
				}
				break;
			case DEV_TYPE_LTE_ATOCP: //zc
			case DEV_TYPE_LTE_UTOCP: //zc
				return NULL;
				break;
			case DEV_TYPE_PSD_STATE: //ci
			case DEV_TYPE_ESB_STATE: //ci
				return NULL;
				break;
			case DEV_TYPE_ATB_STATE: //ci
				return NULL;
				break;
			case DEV_TYPE_ATB_FLASH: //zc
				return NULL;
				break;
			case DEV_TYPE_ATB_STABLE: //zc
				return NULL;
				break;
			case DEV_TYPE_AFG_STATE: //ci
				return NULL;
				break;
			default:
				break;
			}
		}
	}
		
}

void CILKTestDoc::setLogicZcCommRelay(ZCInputCfgData data)
{
	int iFlag = 0, tempI = 0, tvsIndex = 0, logicIndex = 0;
	WCHAR* zcRelayType[] = {
		L"ATռ��", L"UTռ��"
	};

	CString strLogicFlag[] = { L"A", L"B", L"C", L"D", L"E", L"F" };

	switch (data.devType)
	{
	case DEV_TYPE_LTE_ATOCP: //zc
		iFlag = 0;
		break;
	case DEV_TYPE_LTE_UTOCP: //zc
		iFlag = 1;
		break;
	default:
		break;
	}

	tvsIndex = getTvsIndexByLogicIndex(data.devIndex);
	if (CADSection* pSection = (CADSection*)findDevcieByAddr(tvsIndex, RUNTIME_CLASS(CADSection)))
	{
		for (tempI = 0; (tempI < 6) && (m_isLogicSection == 1); tempI++)
		{
			if (CADSection* pLSection = (CADSection*)findLogicDeviceByName(data.devIndex, strLogicFlag[tempI], RUNTIME_CLASS(CADSection)))
			{
				CZcCommRelay* pZcCommRelay = new CZcCommRelay(pLSection->io_.getName(), T_ZC_PHY, zcRelayType[iFlag], TRUE, FALSE, pLSection);
				pZcCommRelay->boardID_[0] = boardZcCommID(data.CodeSeqIndex);
				pZcCommRelay->portID_[0] = portZcCommID(data.CodeSeqIndex);
				pZcCommRelay->codeSeqIndex[0] = data.CodeSeqIndex;

				pLSection->vZcCommRelay_.push_back(pZcCommRelay);
				vZcCommRelay_.push_back(pZcCommRelay);
			}
		}
	}
	else //��������
	{
	}
}

void CILKTestDoc::setPlatformZcCommRelay(ZCInputCfgData data)
{
	BOOL bTemp = FALSE;
	int m, iFlag = 0, otherIndex = 0xFFFF;
	StationDev stationdev;
	WCHAR* zcRelayType[] = {
		L"PSD����", L"ESB����", L"DRB״̬"
	};

	switch (data.devType)
	{
	case DEV_TYPE_PSD_STATE:
		iFlag = 0;
		if (data.devIndex < vPsdCfgData_.size())
		{
			otherIndex = vPsdCfgData_.at(data.devIndex).index[2]; //��ɢ�����MGJ
		}
		break;
	case DEV_TYPE_ESB_STATE:
		iFlag = 1;
		otherIndex = data.devIndex;
		break;
	case DEV_TYPE_ATB_STATE:
		iFlag = 2;
		otherIndex = data.devIndex;
	default:
		break;
	}

	if (otherIndex != 0xFFFF)
	{
		stationdev = findStationDevByAddr(otherIndex, &bTemp);

		if (stationdev.pDev != NULL)
		{
			if (stationdev.pDev != NULL)
			{
				CString strName = /*vLingSanDev[otherIndex]->io_.getName() + L"-" + */zcRelayType[iFlag];
				CZcCommRelay* pZcCommRelay = new CZcCommRelay(stationdev.pDev->io_.getName(), T_ZC_PLAT, strName, FALSE, TRUE, stationdev.pDev);
				pZcCommRelay->boardID_[1] = boardZcCommID(data.CodeSeqIndex);
				pZcCommRelay->portID_[1] = portZcCommID(data.CodeSeqIndex);
				pZcCommRelay->codeSeqIndex[1] = data.CodeSeqIndex;

				stationdev.pDev->vZcCommRelay_.push_back(pZcCommRelay);
				if (findLingsanDevByAddr(stationdev.pDev->io_.addr_) == NULL)
				{
					vLingSanDev.push_back(stationdev.pDev);     //��ɢ�豸
				}

				stationdev.pSta->vZcCommRelay_.push_back(pZcCommRelay);
				vZcCommRelay_.push_back(pZcCommRelay);
			}
			else
			{
				
			}
		}
	}
}

void CILKTestDoc::setOtherDRBZcCommRelay(ZCInputCfgData data)
{
	int iFlag = 0, i = 0;
	CADDevice* pDevice = NULL;
	WCHAR* zcRelayType[] = {
		L"SDJ", L"WDJ"
	};

	switch (data.devType)
	{
	case DEV_TYPE_ATB_FLASH:
		iFlag = 0;
		break;
	case DEV_TYPE_ATB_STABLE:
		iFlag = 1;
		break;
	default:
		break;
	}

	pDevice = findDriByNum(1);
	if (pDevice != NULL)
	{
		CZcCommRelay* pZcCommRelay = new CZcCommRelay(pDevice->io_.getName(), T_ZC_DRB, zcRelayType[iFlag], TRUE, FALSE, pDevice);
		pZcCommRelay->boardID_[0] = boardZcCommID(data.CodeSeqIndex);
		pZcCommRelay->portID_[0] = portZcCommID(data.CodeSeqIndex);
		pZcCommRelay->codeSeqIndex[0] = data.CodeSeqIndex;

		pDevice->vZcCommRelay_.push_back(pZcCommRelay);
		vZcCommRelay_.push_back(pZcCommRelay);
	}
}

void CILKTestDoc::setPhySectZcCommRelay(ZCInputCfgData data)
{
	int iFlag = 0, tempI = 0, logicIndex = 0;
	BOOL bInput = FALSE, bOutput = FALSE;

	WCHAR* zcRelayType[] = {
		L"����ARB", L"�г�ͣ��",
		L"����ռ��", L"��������", L"��������", L"��������"
	};

	switch (data.devType)
	{
	case DEV_TYPE_PHY_ARB: //zc
		iFlag = 0;
		bInput = TRUE;
		break;
	case DEV_TYPE_PHY_STOP: //zc
		iFlag = 1;
		bInput = TRUE;
		break;
	case DEV_TYPE_PHY_OCCUPIED: //ci
		iFlag = 2;
		bOutput = TRUE;
		break;
	case DEV_TYPE_PHY_SLOCK: //ci
		iFlag = 3;
		bOutput = TRUE;
		break;
	case DEV_TYPE_PHY_XLOCK: //ci
		iFlag = 4;
		bOutput = TRUE;
		break;
	case DEV_TYPE_PHY_RTLOCK: //ci
		iFlag = 5;
		bOutput = TRUE;
		break;
	default:
		break;
	}

	if (CADSection* pSection = (CADSection*)findDevcieByAddr(data.devIndex, RUNTIME_CLASS(CADSection)))
	{
		CZcCommRelay* pZcCommRelay = new CZcCommRelay(pSection->io_.getName(), T_ZC_PHY, zcRelayType[iFlag], bInput, bOutput, pSection);
		pZcCommRelay->boardID_[bInput ? 0 : 1] = boardZcCommID(data.CodeSeqIndex);
		pZcCommRelay->portID_[bInput ? 0 : 1] = portZcCommID(data.CodeSeqIndex);
		pZcCommRelay->codeSeqIndex[bInput ? 0 : 1] = data.CodeSeqIndex;
		
		pSection->vZcCommRelay_.push_back(pZcCommRelay);
		vZcCommRelay_.push_back(pZcCommRelay);

		if (DEV_TYPE_PHY_STOP == data.devType)
		{
			CString strName = L"----------------";
			CZcCommRelay* pLineCommRelay = new CZcCommRelay(pSection->io_.getName(), T_ZC_PHY, strName, FALSE, FALSE, pSection);
			pLineCommRelay->boardID_[bInput ? 0 : 1] = boardZcCommID(0xFFFF);
			pLineCommRelay->portID_[bInput ? 0 : 1] = portZcCommID(0xFFFF);
			pLineCommRelay->codeSeqIndex[bInput ? 0 : 1] = 0xFFFF;

			pSection->vZcCommRelay_.push_back(pLineCommRelay);
			vZcCommRelay_.push_back(pLineCommRelay);
		}
	}
	else //��������
	{
		for (tempI = 0; tempI < 3; tempI++) //ͬһ�������������3������
		{
			if (RailSwitch* pSwitch = (RailSwitch*)findSwitDeviceBySwitSectAddr(data.devIndex, tempI))
			{
				CZcCommRelay* pZcCommRelay = new CZcCommRelay(pSwitch->data_.section_.getName(), T_ZC_SWIT, zcRelayType[iFlag], bInput, bOutput, pSwitch);
				pZcCommRelay->boardID_[bInput ? 0 : 1] = boardZcCommID(data.CodeSeqIndex);
				pZcCommRelay->portID_[bInput ? 0 : 1] = portZcCommID(data.CodeSeqIndex);
				pZcCommRelay->codeSeqIndex[bInput ? 0 : 1] = data.CodeSeqIndex;

				pSwitch->vZcCommRelay_.push_back(pZcCommRelay);
				vZcCommRelay_.push_back(pZcCommRelay);

				if (DEV_TYPE_PHY_STOP == data.devType)
				{
					CString strName = L"----------------";
					CZcCommRelay* pLineCommRelay = new CZcCommRelay(pSwitch->io_.getName(), T_ZC_PHY, strName, FALSE, FALSE, pSwitch);
					pLineCommRelay->boardID_[bInput ? 0 : 1] = boardZcCommID(0xFFFF);
					pLineCommRelay->portID_[bInput ? 0 : 1] = portZcCommID(0xFFFF);
					pLineCommRelay->codeSeqIndex[bInput ? 0 : 1] = 0xFFFF;

					pSwitch->vZcCommRelay_.push_back(pLineCommRelay);
					vZcCommRelay_.push_back(pLineCommRelay);
				}
			}
		}
	}
}

void CILKTestDoc::setSwitSectLogicZcCommRelay(ZCInputCfgData data)
{
	CString strName;
	DeviceIO devIO;
	ZCInputCfgData inputZcCfg[2];
	int iFlag = 0, tempI = 0, tempJ = 0, ATIndex = 0, UTIndex = 0;

	WCHAR* zcRelayType[] = {
		L"ATռ��", L"UTռ��"
	};

	CString strLogicFlag[] = { L"A", L"B", L"C", _T("D")};

	if ((data.devType != DEV_TYPE_SWIT_LOCK) || (m_isLogicSection != 1))
	{
		return;
	}
	
	for (tempI = 0; tempI < 4; tempI++)
	{
		if (RailSwitch* pSwitch = (RailSwitch*)findDevcieByAddr(data.devIndex, RUNTIME_CLASS(RailSwitch)))
		{
			devIO = findSwitLogicIndexBySwitIndex2LogicName(data.devIndex, tempI, strLogicFlag[tempJ], RUNTIME_CLASS(RailSwitch));
			if (devIO.addr_ != 0xFFFF)
			{
				inputZcCfg[0] = getZCInputCfgUnitByIndex2Type(devIO.addr_, DEV_TYPE_LTE_ATOCP);
				inputZcCfg[1] = getZCInputCfgUnitByIndex2Type(devIO.addr_, DEV_TYPE_LTE_UTOCP);

				for (tempJ = 0; tempJ < 2 && inputZcCfg[tempJ].devIndex != 0xFFFF; tempJ++)
				{
					strName = pSwitch->logicDeviceIO_[tempI].getName() + L"-" + zcRelayType[tempJ];
					CZcCommRelay* pZcCommRelay = new CZcCommRelay(pSwitch->data_.section_.getName(), T_ZC_SWIT, strName, TRUE, FALSE, pSwitch);
					pZcCommRelay->boardID_[0] = boardZcCommID(inputZcCfg[tempJ].CodeSeqIndex);
					pZcCommRelay->portID_[0] = portZcCommID(inputZcCfg[tempJ].CodeSeqIndex);
					pZcCommRelay->codeSeqIndex[0] = inputZcCfg[tempJ].CodeSeqIndex;

					pSwitch->vZcCommRelay_.push_back(pZcCommRelay);
					vZcCommRelay_.push_back(pZcCommRelay);
				}
			}
		}
	}
}


void CILKTestDoc::setSwitchZcCommRelay(ZCInputCfgData data)
{
	int iFlag = 0;
	if (RailSwitch* pSwitch = (RailSwitch*)findDevcieByAddr(data.devIndex, RUNTIME_CLASS(RailSwitch)))
	{
		WCHAR* zcRelayType[] = {
			L"����λ", L"����λ", L"��������"
		};

		switch (data.devType)
		{
		case DEV_TYPE_SWIT_NORMAL: //ci
			iFlag = 0;
			break;
		case DEV_TYPE_SWIT_REVERSE: //ci
			iFlag = 1;
			break;
		case DEV_TYPE_SWIT_LOCK: //ci
			iFlag = 2;
			break;
		default:
			break;
		}

		CZcCommRelay* pZcCommRelay = new CZcCommRelay(pSwitch->name_.text_, T_ZC_SWIT, zcRelayType[iFlag], FALSE, TRUE, pSwitch);
		pZcCommRelay->boardID_[1] = boardZcCommID(data.CodeSeqIndex);
		pZcCommRelay->portID_[1] = portZcCommID(data.CodeSeqIndex);
		pZcCommRelay->codeSeqIndex[1] = data.CodeSeqIndex;

		pSwitch->vZcCommRelay_.push_back(pZcCommRelay);
		vZcCommRelay_.push_back(pZcCommRelay);
	}
}

void CILKTestDoc::setAfgZcCommRelay(ZCInputCfgData data)
{
	if (CADAfgDoor* pAfg = (CADAfgDoor*)findAfgDoorByAddr(data.devIndex))
	{
		CZcCommRelay* pZcCommRelay = new CZcCommRelay(pAfg->name_.text_, T_ZC_AFG, _T("AFG����"), FALSE, TRUE, pAfg);
		pZcCommRelay->boardID_[1] = boardZcCommID(data.CodeSeqIndex);
		pZcCommRelay->portID_[1] = portZcCommID(data.CodeSeqIndex);
		pZcCommRelay->codeSeqIndex[1] = data.CodeSeqIndex;

		pAfg->vZcCommRelay_.push_back(pZcCommRelay);
		vZcCommRelay_.push_back(pZcCommRelay);
	}
}

void CILKTestDoc::setSignalZcCommRelay(ZCInputCfgData data)
{
	int iFlag = 0;
	BOOL bInput = FALSE, bOutput = FALSE;
	if (CADSignal* pSignal = (CADSignal*)findDevcieByAddr(data.devIndex, RUNTIME_CLASS(CADSignal)))
	{
		WCHAR* zcRelayType[] = {
			L"�ź����", L"�г���ѹ", L"�źſ���"
		};

		switch (data.devType)
		{
		case DEV_TYPE_SIGN_LIGHTSTATE: //zc
			iFlag = 0;
			bInput = TRUE;
			break;
		case DEV_TYPE_SIGN_OVEROCCUPIED: //zc
			iFlag = 1;
			bInput = TRUE;
			break;
		case DEV_TYPE_SIGN_OPENSTATE: //ci
			iFlag = 2;
			bOutput = TRUE;
			break;
		default:
			break;
		}

		if (DEV_TYPE_SIGN_OPENSTATE == data.devType)
		{
			CString strName = L"----------------";
			CZcCommRelay* pLineCommRelay = new CZcCommRelay(pSignal->io_.getName(), T_ZC_PHY, strName, FALSE, FALSE, pSignal);
			pLineCommRelay->boardID_[bInput ? 0 : 1] = boardZcCommID(0xFFFF);
			pLineCommRelay->portID_[bInput ? 0 : 1] = portZcCommID(0xFFFF);
			pLineCommRelay->codeSeqIndex[bInput ? 0 : 1] = 0xFFFF;

			pSignal->vZcCommRelay_.push_back(pLineCommRelay);
			vZcCommRelay_.push_back(pLineCommRelay);
		}
		
		CZcCommRelay* pZcCommRelay = new CZcCommRelay(pSignal->name_.text_, T_ZC_SIGN, zcRelayType[iFlag], bInput, bOutput, pSignal);
		pZcCommRelay->boardID_[bInput ? 0 : 1] = boardZcCommID(data.CodeSeqIndex);
		pZcCommRelay->portID_[bInput ? 0 : 1] = portZcCommID(data.CodeSeqIndex);
		pZcCommRelay->codeSeqIndex[bInput ? 0 : 1] = data.CodeSeqIndex;

		pSignal->vZcCommRelay_.push_back(pZcCommRelay);
		vZcCommRelay_.push_back(pZcCommRelay);
	}
}

UINT16 CILKTestDoc::getSignalIndexByOlpIndex(UINT16 olpIndex)
{
	UINT16 signIndex = 0xFFFF;
	if (olpIndex >= 0 && olpIndex<vOverlapTable_.size())
	{
		signIndex = vOverlapTable_.at(olpIndex).originSignal;
	}

	return signIndex;
}

UINT16 CILKTestDoc::getSignalIndexByRtIndex(UINT16 RtIndex)
{
	UINT16 signIndex = 0, offIndex = 0;
	ADDR_UNIT_RT one;
	
	if (RtIndex>=0 && RtIndex<vRouteAddr_.size())
	{
		one = vRouteAddr_.at(RtIndex);
		offIndex = one.selfAddr.tableIndex * 256 + one.selfAddr.offsetIndex + 5; //+5Ϊinputaddr
		signIndex = vRtOne_.at(offIndex).addr;
	}

	return signIndex;
}

CString CILKTestDoc::getRouteNameByRtIndex(UINT16 rtIndex, UINT16 rtType)
{
	CString strName = L"ERROR-ROUTE";
	UINT16 offIndex = 0;
	ADDR_UNIT_RT one;

	if (rtIndex >= 0 && rtIndex<vRouteAddr_.size())
	{
		one = vRouteAddr_.at(rtIndex);
		offIndex = one.selfAddr.tableIndex * 256 + one.selfAddr.offsetIndex; //ʼ��
		strName = CString(vRtOne_.at(offIndex).name) + L"-" + CString(vRtOne_.at(offIndex + 1).name);
	}

	return strName;
}

void CILKTestDoc::setOlpSignalZcCommRelay(ZCInputCfgData data)
{
	UINT16 uIndex = 0;
	CString strName = L"";
	uIndex = getSignalIndexByOlpIndex(data.devIndex);

	if (CADSignal* pSignal = (CADSignal*)findDevcieByAddr(uIndex, RUNTIME_CLASS(CADSignal)))
	{
		strName = CString(pSignal->name_.text_) + L"_OLP";
		CZcCommRelay* pZcCommRelay = new CZcCommRelay(L"", T_ZC_SIGN, strName, FALSE, TRUE, pSignal);
		pZcCommRelay->boardID_[1] = boardZcCommID(data.CodeSeqIndex);
		pZcCommRelay->portID_[1] = portZcCommID(data.CodeSeqIndex);
		pZcCommRelay->codeSeqIndex[1] = data.CodeSeqIndex;

		pSignal->vZcCommRelay_.push_back(pZcCommRelay);
		vZcCommRelay_.push_back(pZcCommRelay);
	}
}


void CILKTestDoc::setRtStartSignZcCommRelay(ZCInputCfgData data)
{
	int iFlag = 0;
	UINT16 uIndex = 0;
	CString strShowName;
	BOOL bInput = FALSE, bOutput = FALSE;

	uIndex = getSignalIndexByRtIndex(data.devIndex);
	strShowName = getRouteNameByRtIndex(data.devIndex, data.devType);
	if (CADSignal* pSignal = (CADSignal*)findDevcieByAddr(uIndex, RUNTIME_CLASS(CADSignal)))
	{
		WCHAR* zcRelayType[] = {
			L"_J_ͣ����֤", L"_Z_ͣ����֤", L"_J_��·״̬", L"_Z_��·״̬", L"_J_ͣ����֤����", L"_Z_ͣ����֤����"
		};
		
		switch (data.devType)
		{
		case DEV_TYPE_ROUTE_STOPASW:
			iFlag = 0;
			bInput = TRUE;
			break;
		case DEV_TYPE_ATBRT_STOPASW:
			iFlag = 1;
			bInput = TRUE;
			break;
		case DEV_TYPE_ROUTE_STATE:
			iFlag = 2;
			bOutput = TRUE;
			break;
		case DEV_TYPE_ATBRT_STATE:
			iFlag = 3;
			bOutput = TRUE;
			break;
		case DEV_TYPE_ROUTE_STOPASK:
			iFlag = 4;
			bOutput = TRUE;
			break;
		case DEV_TYPE_ATBRT_STOPASK:
			iFlag = 5;
			bOutput = TRUE;
			break;
		default:
			break;
		}
		strShowName = strShowName + zcRelayType[iFlag];

		CZcCommRelay* pZcCommRelay = new CZcCommRelay(L"", T_ZC_SIGN, strShowName, bInput, bOutput, pSignal);
		pZcCommRelay->boardID_[bInput ? 0 : 1] = boardZcCommID(data.CodeSeqIndex);
		pZcCommRelay->portID_[bInput ? 0 : 1] = portZcCommID(data.CodeSeqIndex);
		pZcCommRelay->codeSeqIndex[bInput ? 0 : 1] = data.CodeSeqIndex;

		pSignal->vZcCommRelay_.push_back(pZcCommRelay);
		vZcCommRelay_.push_back(pZcCommRelay);
	}
}



void CILKTestDoc::StartMap3MDevData()  //���ݵ�ַ���ң����ҵ����������ε�Σ��
{
	for (int index = 0; index < vToSimCfgData.size(); index++)
	{
		if (vToSimCfgData.at(index).devType == DEV_TYPE_SWITCH_STATE){//����ǵ���
			CADDevice* pDevice = findDevcieByAddr(vToSimCfgData.at(index).dev.devAddr, RUNTIME_CLASS(RailSwitch));
			if (pDevice != NULL){
				pDevice->devType_3M = vToSimCfgData.at(index).devType;
				pDevice->devId_3M = vToSimCfgData.at(index).zcIndex; 
			}
			else
			{
				AfxMessageBox(L"SAMͨ�ŵ���Ԫ�ر�ƥ��ʧ�ܣ�����˲�" + CString(vToSimCfgData.at(index).dev.name));
			}
		}
		else if (vToSimCfgData.at(index).devType == DEV_TYPE_SIGN_STATE){//������źŻ�
			CADDevice* pDevice = NULL;
			pDevice = findDevcieByAddr(vToSimCfgData.at(index).dev.devAddr, RUNTIME_CLASS(CADSignal));
			if (pDevice != NULL){
				pDevice->devType_3M = vToSimCfgData.at(index).devType;
				pDevice->devId_3M = vToSimCfgData.at(index).zcIndex;
			}
			else
			{
				AfxMessageBox(L"SAM�źŻ�Ԫ�ر�ƥ��ʧ�ܣ�����˲�" + CString(vToSimCfgData.at(index).dev.name));
			}
		}

		else if (vToSimCfgData.at(index).devType == DEV_TYPE_PHY_STATE)
		{//�������������
			CADDevice* pDevice = NULL;
			if (m_isLogicSection == 1)
			{
				CString name(vToSimCfgData.at(index).dev.name);
				pDevice = findDeviceByName(name + L"A");
			}
			else
			{
				pDevice = findDevcieByAddr(vToSimCfgData.at(index).dev.devAddr, RUNTIME_CLASS(CADSection));
			}

			if (pDevice == NULL)
			{
				pDevice = findSwitchBySectAddr(vToSimCfgData.at(index).dev.devAddr);
				RailSwitch* pswitch = dynamic_cast<RailSwitch*> (pDevice);
				if (pswitch != NULL)
				{
					pswitch->switchSectonType_3M = vToSimCfgData.at(index).devType;
					pswitch->switchSectionID_3M = vToSimCfgData.at(index).zcIndex;
				}
				else
				{
					AfxMessageBox(L"SAM����Ԫ�ر�ƥ��ʧ�ܣ�����˲�" + CString(vToSimCfgData.at(index).dev.name));
				}
			}
			else
			{
				pDevice->devType_3M = vToSimCfgData.at(index).devType;
				pDevice->devId_3M = vToSimCfgData.at(index).zcIndex;
			}
		}

		else if (vToSimCfgData.at(index).devType >= DEV_TYPE_PSD_STATE &&
			vToSimCfgData.at(index).devType < DEV_TYPE_ATB_STATE)
		{//��ɢ
			CADDevice* pDevice = NULL;
			pDevice = findLingsanDevByAddr(vToSimCfgData.at(index).dev.devAddr);
			if (pDevice != NULL)
			{
				pDevice->devType_3M = vToSimCfgData.at(index).devType;
				pDevice->devId_3M = vToSimCfgData.at(index).zcIndex;
			}
			else
			{
				AfxMessageBox(L"����SAM��ɢ�豸ʧ��" + CString(vToSimCfgData.at(index).dev.name));
			}
		}
		else if (vToSimCfgData.at(index).devType == DEV_TYPE_ATB_STATE)  //DRI��DRB
		{
			CADDevice* pDevice = NULL;
			pDevice = findLingsanDevByAddr(vToSimCfgData.at(index).dev.devAddr);
			if (pDevice != NULL)
			{
				pDevice->devType_3M = vToSimCfgData.at(index).devType;
				pDevice->devId_3M = vToSimCfgData.at(index).zcIndex;
			}
			else
			{
				AfxMessageBox(L"SAM������ɢDRB�豸ʧ��" + CString(vToSimCfgData.at(index).dev.name), MB_ICONEXCLAMATION);
			}

			pDevice = findLingsanDevByAddr(vToSimCfgData.at(index).dev.devAddr+1);
			if (pDevice != NULL)
			{
				if (pDevice->name_.text_==L"DRI")
				{
					pDevice->devType_3M = vToSimCfgData.at(index).devType;
					pDevice->devId_3M = vToSimCfgData.at(index).zcIndex;
				}
				
			}
			else
			{
				AfxMessageBox(L"DRB��DRI������������SAM�������ɢ����" + CString(vToSimCfgData.at(index).dev.name));
			}
		}
		else 
		{

		}
	}


}



CADDevice* CILKTestDoc::findSwitchBySectID_3M(UINT16  devId)
{
	auto it = find_if(vDevice_.begin(), vDevice_.end(), [devId](CADDevice* pDevice) {
		if (pDevice->IsKindOf(RUNTIME_CLASS(RailSwitch)))
		{
			RailSwitch* pSwitch = dynamic_cast<RailSwitch*> (pDevice);
			if (pSwitch->switchSectionID_3M == devId)
			{
				return true;
			}
		}
		return false;
	});

	return it == vDevice_.end() ? NULL : (*it);
}

CADDevice* CILKTestDoc::findDevcieByDevID_3M(UINT16 devId, CRuntimeClass* pRuntimeClass)
{
	auto it = find_if(vDevice_.begin(), vDevice_.end(), [&devId, &pRuntimeClass](CADDevice* pDevice) {
		if (	pDevice->IsKindOf(pRuntimeClass))
		{
			return pDevice->devId_3M == devId;
		}
		return false;
	});
	return it == vDevice_.end() ? findLingsanDevByID_3M(devId,pRuntimeClass) : (*it);
}

CADDevice* CILKTestDoc::findLingsanDevByID_3M(UINT16 devId, CRuntimeClass* pRuntimeClass)
{
	auto it = find_if(vLingSanDev.begin(), vLingSanDev.end(), [&devId, &pRuntimeClass](CADDevice* pDevice) {
		if (pDevice->IsKindOf(pRuntimeClass))
		{
			return pDevice->devId_3M == devId;
		}
		return false;
	});
	return it == vLingSanDev.end() ? NULL : (*it);
}

void CILKTestDoc::SendAllShowTo3M()
{
	char sendDataTo3M[2048];
	memset(sendDataTo3M,0,sizeof(sendDataTo3M));
	int sendDataTo3MLen = 0;

	if (/*m_p3MSock->m_isConnected  &&*/ m_3MProcess!=NULL  &&  m_isCnctToSAM > 0)
	{
		m_3MProcess->PackSendData(sendDataTo3M, &sendDataTo3MLen);
		//����
		m_p3MSock->sendDataToRemote(sendDataTo3M, sendDataTo3MLen);
	}
	
}


void CILKTestDoc::SendIOData()
{
	IOData ioData;
	char sendData[256] = { 0 };

	memset(sendData, 0, sizeof(IOData));
	memset(&ioData, 0, sizeof(IOData));
	ioData.head_ = 0x55aa;
	packageIO(&ioData);
	//����
	memcpy(sendData, &ioData, sizeof(IOData));
	for (int i = 0; i < 2; i++)
	{
		if (m_pioSock[i]->m_isConnected)
		{
			m_pioSock[i]->sendDataToRemote(sendData, sizeof(IOData));
		}
		else
		{
			//TRACE(L" %d Channal Connected  error  is %d \r\n",i, m_pioSock[i]->m_isConnected);
		}
	}

}

void CILKTestDoc::sendZcData()
{
	int Len = 0;
	unsigned short ciId = 0;
	UINT8 zcCommData[300];
	S_Msg Senddata = { 0u };
	unsigned char sendData[300] = { 0 };
	
	if ((m_bIsCommToZC) && (m_iIsCnctToSamZC == 1))
	{
		ciId = (unsigned short)((ciTypeWithZCSIM_ << 8) | ciIdWithZCSIM_);
		
		memset(zcCommData, 0, sizeof(zcCommData));
		memset(&sendData, 0, sizeof(sendData));

		Len = packZcSecndData(zcCommData/*, Len*/);
		memcpy(sendData, &zcCommData, Len);
	
		F_RSSP1_PackDataInOneFrame(ciId, RSSP_RSD, sendData, 100u, Senddata.buff, &Senddata.length);
		if ((Len>5) && (Len <= 300))
		{
			m_pZcSock_->sendDataToRemote((char*)&Senddata.buff, Senddata.length);
		}
		//TRACE(L"%d\n", Senddata.length);
	}
}


void CILKTestDoc::CheckCnctStatus()
{
	for (int i = 0; i < 2; i++)
	{
		m_pioSock[i]->updataCnctState();
	}

	m_p3MSock->updataCnctState();

	m_pZcSock_->updataCnctState();
}

BOOL CILKTestDoc::isSwitchRelayTrue(SwitchInput* pInput)
{
	BOOL tempR = FALSE;
	for (int i = 0; i < 3; i++)
	{
		if (pInput->relayAddr_[i] != UNUSERELAYINADDR)
		{
			tempR = TRUE;
		}
	}

	return  tempR;
}

BOOL CILKTestDoc::isSignalRelayTrue(SignalInput* pInput)
{
	BOOL tempR = FALSE;
	for (int i = 0; i < 10; i++)
	{
		if (pInput->relayAddr_[i] != UNUSERELAYINADDR)
		{
			tempR = TRUE;
		}
	}

	return  tempR;
}

UINT16 CILKTestDoc::GetUINT16ByBigData(const char* inBuff) 
{
	UINT8 tempVal[2] = { 0,0 };
	UINT16 tempI = 0;
	UINT32 val = 0;
	for (tempI = 0; tempI < 2; tempI++)
	{
		tempVal[tempI] = *inBuff;
		inBuff++;
	}
	//��λ��ǰ
	val = tempVal[1] + tempVal[0] * 256;

	return val;
}


void CILKTestDoc::GetBigDataByUINT16(UINT16 val, char* outBuff)
{
	*outBuff = (UINT8)(val >> 8) & 0xff;
	*(outBuff+1) = (UINT8)(val & 0xff);
}


void CILKTestDoc::OnAllNorPos()
{
	// TODO:  �ڴ���������������
	for (int i = 0; i < vDevice_.size();i++)
	{
		CADDevice* pDevice = vDevice_.at(i);
		RailSwitch* pswitch = dynamic_cast<RailSwitch*> (pDevice);
		if (pswitch != NULL)
		{
			pswitch->setDPos();
		}
	}
}


void CILKTestDoc::OnAllRevPos()
{
	// TODO:  �ڴ���������������

	for (int i = 0; i < vDevice_.size(); i++)
	{
		CADDevice* pDevice = vDevice_.at(i);
		RailSwitch* pswitch = dynamic_cast<RailSwitch*> (pDevice);
		if (pswitch != NULL)
		{
			pswitch->setFPos();
		}
	}
}


void CILKTestDoc::OnAll4KaiPos()
{
	// TODO:  �ڴ���������������

	for (int i = 0; i < vDevice_.size(); i++)
	{
		CADDevice* pDevice = vDevice_.at(i);
		RailSwitch* pswitch = dynamic_cast<RailSwitch*> (pDevice);
		if (pswitch != NULL)
		{
			pswitch->setUnPos();
		}
	}
}


void CILKTestDoc::OnAllOcpState()
{
	// TODO:  �ڴ���������������

	for (int i = 0; i < vDevice_.size(); i++)
	{
		CADDevice* pDevice = vDevice_.at(i);
		CADSection* pSection = dynamic_cast<CADSection*> (pDevice);
		if (pSection != NULL)
		{
			pSection->setSectionState(true);
		}
		else if (RailSwitch* pSwitch = dynamic_cast<RailSwitch*> (pDevice))
		{
			pSwitch->setSectionState(true);
		}
		else
		{

		}
	}
}


void CILKTestDoc::OnAllClearOcpState()
{
	// TODO:  �ڴ���������������
	for (int i = 0; i < vDevice_.size(); i++)
	{
		CADDevice* pDevice = vDevice_.at(i);
		CADSection* pSection = dynamic_cast<CADSection*> (pDevice);
		if (pSection != NULL)
		{
			pSection->setSectionState(false);
		}
		else if (RailSwitch* pSwitch = dynamic_cast<RailSwitch*> (pDevice))
		{
			pSwitch->setSectionState(false);
		}
		else
		{

		}
	}
}


void CILKTestDoc::OnAllPrePos()
{
	// TODO:  �ڴ���������������
	for (int i = 0; i < vDevice_.size(); i++)
	{
		CADDevice* pDevice = vDevice_.at(i);
		CADSection* pSection = dynamic_cast<CADSection*> (pDevice);
		if (pSection != NULL)
		{
			pSection->setPreResetState(true);
		}
		else if (RailSwitch* pSwitch = dynamic_cast<RailSwitch*> (pDevice))
		{
			pSwitch->setPreResetState(true);
		}
		else
		{

		}
	}
}


void CILKTestDoc::OnAllDS()
{
	// TODO:  �ڴ���������������
	for (int i = 0; i < vDevice_.size(); i++)
	{
		CADDevice* pDevice = vDevice_.at(i);
		CADSignal* pSignal = dynamic_cast<CADSignal*> (pDevice);
		if (pSignal != NULL)
		{
			pSignal->SetDsState(true);
		}
	}
}


void CILKTestDoc::OnAllClearDS()
{
	// TODO:  �ڴ���������������
	for (int i = 0; i < vDevice_.size(); i++)
	{
		CADDevice* pDevice = vDevice_.at(i);
		CADSignal* pSignal = dynamic_cast<CADSignal*> (pDevice);
		if (pSignal != NULL)
		{
			pSignal->SetDsState(false);
		}
	}
}


void CILKTestDoc::OnAllClearPreReset()
{
	// TODO:  �ڴ���������������
	for (int i = 0; i < vDevice_.size(); i++)
	{
		CADDevice* pDevice = vDevice_.at(i);
		CADSection* pSection = dynamic_cast<CADSection*> (pDevice);
		if (pSection != NULL)
		{
			pSection->setPreResetState(false);
		}
		else if (RailSwitch* pSwitch = dynamic_cast<RailSwitch*> (pDevice))
		{
			pSwitch->setPreResetState(false);
		}
		else
		{

		}
	}
}

////
void CILKTestDoc::OnAllLightOFF()
{
	// TODO:  �ڴ���������������
	for (int i = 0; i < vDevice_.size(); i++)
	{
		CADDevice* pDevice = vDevice_.at(i);
		CADSignal* pSignal = dynamic_cast<CADSignal*> (pDevice);
		if (pSignal != NULL)
		{
			pSignal->setZcCommRelayAllSignalLightMode(TRUE);
		}
		else
		{

		}
	}
}


void CILKTestDoc::OnAllLightON()
{
	// TODO:  �ڴ���������������
	for (int i = 0; i < vDevice_.size(); i++)
	{
		CADDevice* pDevice = vDevice_.at(i);
		CADSignal* pSignal = dynamic_cast<CADSignal*> (pDevice);
		if (pSignal != NULL)
		{
			pSignal->setZcCommRelayAllSignalLightMode(FALSE);
		}
		else
		{

		}
	}
}


void CILKTestDoc::OnAllLTEAT()
{
	// TODO:  �ڴ���������������
	for (int i = 0; (i < vDevice_.size()) && (m_isLogicSection); i++)
	{
		CADDevice* pDevice = vDevice_.at(i);
		CADSection* pSection = dynamic_cast<CADSection*> (pDevice);
		if (pSection != NULL)
		{
			pSection->setZcCommRelayAllLTEState(FALSE, FALSE); //�ȿ��У���ռ�ã�������֮ǰ��־
			pSection->setZcCommRelayAllLTEState(TRUE, FALSE);
		}
		else
		{
			RailSwitch* pSwitch = dynamic_cast<RailSwitch*>(pDevice);
			if (pSwitch != NULL)
			{
				pSwitch->setZcCommRelayAllLTEState(FALSE, FALSE);
				pSwitch->setZcCommRelayAllLTEState(TRUE, FALSE);
			}
		}
	}
}

void CILKTestDoc::OnAllLTEUT()
{
	// TODO:  �ڴ���������������
	for (int i = 0; (i < vDevice_.size()) && (m_isLogicSection); i++)
	{
		CADDevice* pDevice = vDevice_.at(i);
		CADSection* pSection = dynamic_cast<CADSection*> (pDevice);
		if (pSection != NULL)
		{
			pSection->setZcCommRelayAllLTEState(FALSE, FALSE); //�ȿ��У���ռ�ã�������֮ǰ��־
			pSection->setZcCommRelayAllLTEState(FALSE, TRUE);
		}
		else
		{
			RailSwitch* pSwitch = dynamic_cast<RailSwitch*>(pDevice);
			if (pSwitch != NULL)
			{
				pSwitch->setZcCommRelayAllLTEState(FALSE, FALSE);
				pSwitch->setZcCommRelayAllLTEState(FALSE, TRUE);
			}
		}
	}
}

void CILKTestDoc::OnAllLTEClear()
{
	// TODO:  �ڴ���������������
	for (int i = 0; (i < vDevice_.size()) && (m_isLogicSection); i++)
	{
		CADDevice* pDevice = vDevice_.at(i);
		CADSection* pSection = dynamic_cast<CADSection*> (pDevice);
		if (pSection != NULL)
		{
			pSection->setZcCommRelayAllLTEState(FALSE, FALSE);
		}
		else
		{
			RailSwitch* pSwitch = dynamic_cast<RailSwitch*>(pDevice);
			if (pSwitch != NULL)
			{
				pSwitch->setZcCommRelayAllLTEState(FALSE, FALSE);
			}
		}
	}
}

void CILKTestDoc::OnAllLTEEeror()
{
	// TODO:  �ڴ���������������
	for (int i = 0; (i < vDevice_.size()) && (m_isLogicSection); i++)
	{
		CADDevice* pDevice = vDevice_.at(i);
		CADSection* pSection = dynamic_cast<CADSection*> (pDevice);
		if (pSection != NULL)
		{
			pSection->setZcCommRelayAllLTEState(FALSE, FALSE); //�ȿ��У���ռ�ã�������֮ǰ��־
			pSection->setZcCommRelayAllLTEState(TRUE, TRUE);
		}
		else
		{
			RailSwitch* pSwitch = dynamic_cast<RailSwitch*>(pDevice);
			if (pSwitch != NULL)
			{
				pSwitch->setZcCommRelayAllLTEState(FALSE, FALSE);
				pSwitch->setZcCommRelayAllLTEState(TRUE, TRUE);
			}
		}
	}
}
////

////
int CILKTestDoc::getTvsIndexByLogicIndex(int logicIndex)
{
	int tvsIndex = 0xFFFF, i = 0;
	for (i = 0; i < vPhy2Logic_.size(); i++)
	{
		if (vPhy2Logic_.at(i).logicIndex == logicIndex)
		{
			tvsIndex = vPhy2Logic_.at(i).tvsIndex;
			break;
		}
	}
	return tvsIndex;
}
int CILKTestDoc::getLogicIndexByTvsIndex(int tvsIndex, int iNo) /*һ���������ζ�Ӧ����߼�����*/
{
	int tempNum = 0;
	int logicIndex = 0xFFFF, i = 0;
	for (i = 0; i < vPhy2Logic_.size(); i++)
	{
		if (vPhy2Logic_.at(i).tvsIndex == tvsIndex)
		{
			if (tempNum == iNo)
			{
				logicIndex = vPhy2Logic_.at(i).logicIndex;
				break;
			}
			else
			{
				tempNum++;
			}
		}
	}
	return logicIndex;
}

ZCInputCfgData CILKTestDoc::getZCInputCfgUnitByIndex2Type(int devIndex, int devType)
{
	ZCInputCfgData ret = { 0xFFFF, 0, 0xFFFF, 0xFFFF };
	ZCInputCfgData one = {0xFFFF, 0, 0xFFFF, 0xFFFF};
	for (int i = 0; i < vZcInputCfgData_.size(); i++)
	{
		one = vZcInputCfgData_.at(i);
		if (one.devIndex == devIndex && one.devType == devType)
		{
			ret = one;
			break;
		}
	}
	return ret;
}

ZCInputCfgData CILKTestDoc::getZCInputCfgUnitByCodeSeq2Type(int codeSeqIndex, int devType)
{
	ZCInputCfgData ret = { 0xFFFF, 0, 0xFFFF, 0xFFFF };
	ZCInputCfgData one = { 0xFFFF, 0, 0xFFFF, 0xFFFF };
	for (int i = 0; i < vZcInputCfgData_.size(); i++)
	{
		one = vZcInputCfgData_.at(i);
		if (one.CodeSeqIndex == codeSeqIndex && one.devType == devType)
		{
			ret = one;
			break;
		}
	}
	return ret;
}

CADDevice* CILKTestDoc::findSwitDeviceBySwitSectAddr(int sectAddr, int iNo)
{
	int tmpI = 0;
	CADDevice* pDevice = NULL;
	RailSwitch* pSwitch = NULL;
	for (int i = 0; i < vDevice_.size(); i++)
	{
		pDevice = vDevice_.at(i);
		if (pDevice->IsKindOf(RUNTIME_CLASS(RailSwitch)))
		{
			pSwitch = static_cast<RailSwitch*>(pDevice);
			if (pSwitch->data_.section_.addr_ == sectAddr)
			{
				tmpI++;
			}
		}
		if (tmpI == iNo + 1)
		{
			break;
		}
		pDevice = NULL;
	}

	return pDevice;
}

CADDevice* CILKTestDoc::findSwitDeviceByLogicAddr(int logicAddr, int iNo)
{
	int tmpI = 0, i, j;
	CADDevice* pDevice = NULL;
	RailSwitch* pSwitch = NULL;
	for (i = 0; i < vDevice_.size(); i++)
	{
		pDevice = vDevice_.at(i);
		if (pDevice->IsKindOf(RUNTIME_CLASS(RailSwitch)))
		{
			pSwitch = static_cast<RailSwitch*>(pDevice);
			for (j = 0; j < 4; j++)
			{
				if ((pSwitch->logicDeviceIO_[j].addr_ == logicAddr) && (logicAddr != 0xFFFF) && (pSwitch->logicDeviceIO_[j].getName().Compare(L"NO") != 0)) /*��ȷ�ϣ�CAD�ж���Ĭ��Ϊ�յ��߼���������ΪNO����ַΪ0xFFFF*/
				{
					tmpI++;
					break;
				}
			}
		}
		if (tmpI == iNo + 1)
		{
			break;
		}
		pDevice = NULL;
	}

	return pDevice;
}

CString CILKTestDoc::getSwitLogicNameBySwitDevice(RailSwitch* pSwitch, int iNo)
{
	if ((iNo < 4) && (iNo >=0))
	{
		return pSwitch->logicDeviceIO_[iNo].getName();
	}
	else
	{
		return L"NO";
	}
}

void CILKTestDoc::setSwitLogicState(CZcCommRelay* pCurZcRelay)
{
	int i = 0;
	CZcCommRelay* pZc = NULL;
	if ((pCurZcRelay->devType == T_ZC_SWIT) && ((pCurZcRelay->name_.Find(L"ATռ��") >= 0) || (pCurZcRelay->name_.Find(L"UTռ��") >= 0)))
	{
		pCurZcRelay->pDevice_->Invalidate();
		for (i = 0; i < vZcCommRelay_.size(); i++)
		{
			pZc = vZcCommRelay_.at(i);
			if ((pZc->devType == T_ZC_SWIT) && (pZc->deviceName_.Compare(pCurZcRelay->deviceName_) == 0)
				&& (pZc->name_.Compare(pCurZcRelay->name_) == 0)
				&& (pZc->pDevice_->io_.getName() != pCurZcRelay->pDevice_->io_.getName()))
			{
				pZc->pDevice_->Invalidate();
			}
		}
	}
}

void CILKTestDoc::setSwitSectOtherSwitARBState(CZcCommRelay* pCurZcRelay)
{
	int i = 0;
	CZcCommRelay* pZc = NULL;
	if ((pCurZcRelay->devType == T_ZC_SWIT) && (pCurZcRelay->name_.Compare(L"����ARB") >= 0))
	{
		for (i = 0; i < vZcCommRelay_.size(); i++)
		{
			pZc = vZcCommRelay_.at(i);
			if ((pZc->devType == T_ZC_SWIT) && (pZc->deviceName_.Compare(pCurZcRelay->deviceName_) == 0)
				&& (pZc->name_.Compare(pCurZcRelay->name_) == 0)
				&& (pZc->pDevice_->io_.getName() != pCurZcRelay->pDevice_->io_.getName()))
			{
				if (pCurZcRelay->isHigh())
				{
					pZc->setStat(FALSE, true);
				}
				else
				{
					pZc->setStat(TRUE, true);
				}
			}
		}
	}
}

void CILKTestDoc::setSwitSectOtherSwitStopState(CZcCommRelay* pCurZcRelay)
{
	int i = 0;
	CZcCommRelay* pZc = NULL;
	if ((pCurZcRelay->devType == T_ZC_SWIT) && (pCurZcRelay->name_.Compare(L"�г�ͣ��") >= 0))
	{
		for (i = 0; i < vZcCommRelay_.size(); i++)
		{
			pZc = vZcCommRelay_.at(i);
			if ((pZc->devType == T_ZC_SWIT) && (pZc->deviceName_.Compare(pCurZcRelay->deviceName_) == 0)
				&& (pZc->name_.Compare(pCurZcRelay->name_) == 0)
				&& (pZc->pDevice_->io_.getName() != pCurZcRelay->pDevice_->io_.getName()))
			{
				if (pCurZcRelay->isHigh())
				{
					pZc->setStat(FALSE, true);
				}
				else
				{
					pZc->setStat(TRUE, true);
				}
			}
		}
	}
}

void CILKTestDoc::setSwitSectAllSwitOcpState(CZcCommRelay* pCurZcRelay)
{
	int i = 0;
	CZcCommRelay* pZc = NULL;
	if ((pCurZcRelay->devType == T_ZC_SWIT) && ((pCurZcRelay->name_.Find(L"ATռ��") >= 0) || (pCurZcRelay->name_.Find(L"UTռ��") >= 0)))
	{
		for (i = 0; i < vZcCommRelay_.size(); i++)
		{
			pZc = vZcCommRelay_.at(i);
			if ((pZc->devType == T_ZC_SWIT) && (pZc->deviceName_.Compare(pCurZcRelay->deviceName_) == 0)
				&& (pZc->name_.Compare(pCurZcRelay->name_) == 0)
				&& (pZc->pDevice_->io_.getName() != pCurZcRelay->pDevice_->io_.getName()))
			{
				if (pCurZcRelay->isHigh())
				{
					pZc->setStat(FALSE, true);
				}
				else
				{
					pZc->setStat(TRUE, true);
				}
			}
		}
	}
}

int CILKTestDoc::getRouteSignAskStateInfo(CZcCommRelay* pCurZcRelay)
{
	CString Name;
	CZcCommRelay* pZc = NULL;
	int iPos = 0, isrcRet = 0;
	if (pCurZcRelay->name_.Find(L"_J_ͣ����֤") > 0)
	{
		iPos = pCurZcRelay->name_.Find(L"_J_ͣ����֤");
		Name = pCurZcRelay->name_.Left(iPos) + L"_J_ͣ����֤����";
		if ((pZc = getCommRelayDevice(T_ZC_SIGN, pCurZcRelay->pDevice_->io_.addr_, Name)) != NULL)
		{
			isrcRet = pZc->stat_;
		}
	}
	else if (pCurZcRelay->name_.Find(L"_Z_ͣ����֤") > 0)
	{
		iPos = pCurZcRelay->name_.Find(L"_Z_ͣ����֤");
		Name = pCurZcRelay->name_.Left(iPos) + L"_Z_ͣ����֤����";
		if ((pZc = getCommRelayDevice(T_ZC_SIGN, pCurZcRelay->pDevice_->io_.addr_, Name)) != NULL)
		{
			isrcRet = pZc->stat_;
		}
	}
	else
	{
		isrcRet = pCurZcRelay->stat_;
	}
	return isrcRet;
}

int CILKTestDoc::getRouteSectRTLOCKStateInfo(CZcCommRelay* pCurZcRelay)
{
	CString Name;
	CZcCommRelay* pZc = NULL;
	int iPos = 0, isrcRet = 0;
	if (pCurZcRelay->name_.Find(L"�г�ͣ��") >= 0)
	{
		Name = pCurZcRelay->deviceName_ + L"=" + L"��������";
		if ((pZc = getCommRelayDevice(T_ZC_PHY, pCurZcRelay->pDevice_->io_.addr_, Name)) != NULL)
		{
			isrcRet = pZc->stat_;
		}
	}
	else
	{
		isrcRet = pCurZcRelay->stat_;
	}
	return isrcRet;
}

CZcCommRelay* CILKTestDoc::getCommRelayDevice(int devType, CString devName, CString RelayName)
{
	int i = 0;
	CZcCommRelay* pRetDev = NULL;
	CZcCommRelay* pZcRelay = NULL;
	for (i = 0; i < vZcCommRelay_.size(); i++)
	{
		pZcRelay = vZcCommRelay_.at(i);
		if ((pZcRelay->devType == devType) && (pZcRelay->deviceName_.Compare(devName) == 0) && (pZcRelay->name_.Compare(RelayName) == 0))
		{
			pRetDev = pZcRelay;
			break;
		}
	}

	return pRetDev;
}

CZcCommRelay* CILKTestDoc::getCommRelayDevice(int devType, int devIndex, CString devName)
{
	int i = 0, iPos = 0;
	CZcCommRelay* pRetDev = NULL;
	CZcCommRelay* pZcRelay = NULL;
	CString strDevName, strRelayName;

	for (i = 0; i < vZcCommRelay_.size(); i++)
	{
		pZcRelay = vZcCommRelay_.at(i);
		if ((pZcRelay->devType == devType) && (pZcRelay->pDevice_->io_.addr_ == devIndex))
		{
			if ((pZcRelay->deviceName_.Compare(devName) == 0)/* || (pZcRelay->deviceName_.Find(devName) >= 0)*/)
			{
				pRetDev = pZcRelay;
				break;
			}
			else if ((pZcRelay->name_.Compare(devName) == 0)/* || (pZcRelay->name_.Find(devName) >= 0)*/)
			{
				pRetDev = pZcRelay;
				break;
			}
			else
			{
				iPos = devName.Find(L"=");
				if (iPos > 0)
				{
					strDevName = devName.Left(iPos);
					strRelayName = devName.Mid(iPos + 1, devName.GetLength() - iPos - 1);
					if ((pZcRelay->deviceName_.Compare(strDevName) == 0) && (pZcRelay->name_.Compare(strRelayName) == 0))
					{
						pRetDev = pZcRelay;
						break;
					}
				}
			}
		}
	}

	return pRetDev;
}

////
bool CILKTestDoc::F_RSSP1_ReadRssp1Cfg(CString strPath)
{
	bool bRet = false;
	MyUINT8 readBuffer[40800] = { 0 };
	MyUINT16 readNum = 0x3F8u;
	ifstream ifs;
	int zcId = (zcTypeWithZCSIM_ << 8) | zcIdWithZCSIM_;

	CString inputFileName = strPath + L"COMM\\RSSR.bin";
	ifs.open(inputFileName, ios::binary);
	if (ifs.is_open())
	{
		ifs.read((char*)&readBuffer, 40800);
		if (r_TRUE == Rsp1_Init(zcId, readNum, readBuffer))
		{
			bRet = true;
		}
		else
		{
			bRet = false;
		}
		ifs.close();
	}
	else
	{
		bRet = false;
	}
	return bRet;
}

void CILKTestDoc::SetGroupRect(CWnd* pwndGroup)
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CIOView* pIOView = pMainFrame->pSplitterFrame_->pIOView_;

	CRect wndGroupRect;//��Ļλ��
	pwndGroup->GetWindowRect(wndGroupRect);
	pIOView->ScreenToClient(&wndGroupRect);//viewλ��

	wndGroupRect.bottom = wndGroupRect.top + 15 * (BOARDDISTENCE + 5);
	pwndGroup->MoveWindow(wndGroupRect);
}


void CILKTestDoc::OnDisConnToZc()
{
	// TODO:  �ڴ���������������
	if (m_iIsCnctToSamZC == 1)
	{
		m_bIsCommToZC = !m_bIsCommToZC;
	}
	else
	{
		MessageBox(NULL, L"���������ļ�(simconfig.ini)�С��Ƿ���ZCͨ�š�������", L"��ʾ��Ϣ", MB_OK | MB_ICONASTERISK);
	}
}


void CILKTestDoc::OnUpdateDisConnToZc(CCmdUI *pCmdUI)
{
	// TODO:  �ڴ������������û����洦��������
	if (m_iIsCnctToSamZC == 1)
	{
		pCmdUI->Enable(TRUE);
		pCmdUI->SetCheck(!m_bIsCommToZC);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}


int CILKTestDoc::GetMultiByteLen(CString str)
{
	int len = WideCharToMultiByte(CP_ACP, 0, str, str.GetLength(), NULL, 0, NULL, NULL);
	str.ReleaseBuffer();
	return len;
}






void CILKTestDoc::setWcuCommRelay()
{
	ElementCode devData;

	for (int i = 0; i < vElements_.size(); i++)
	{
		devData = vElements_.at(i);
		switch (devData.wcu_instance_kind_id)
		{
		case WCU_TYPE_SG: 
			setWcuSg(devData);
			break;
		case WCU_TYPE_TS: 
			break;
		case WCU_TYPE_PT:
			break;
		case WCU_TYPE_CR: 
			setLogicZcCommRelay(devData);
			break;
		case WCU_TYPE_BL:
			break;
		default:
			break;
		}
	}
}


void CILKTestDoc::setWcuSg(ElementCode data)
{
	CString strName;
	DeviceIO devIO;
	ZCInputCfgData inputZcCfg[2];
	int iFlag = 0, tempI = 0, tempJ = 0, ATIndex = 0, UTIndex = 0;

	WCHAR* zcRelayType[] = {
		L"ATռ��", L"UTռ��"
	};

	CString strLogicFlag[] = { L"A", L"B", L"C", _T("D") };

	if ((data.devType != DEV_TYPE_SWIT_LOCK) || (m_isLogicSection != 1))
	{
		return;
	}

	for (tempI = 0; tempI < 4; tempI++)
	{
		if (RailSwitch* pSwitch = (RailSwitch*)findDevcieByAddr(data.devIndex, RUNTIME_CLASS(RailSwitch)))
		{
			devIO = findSwitLogicIndexBySwitIndex2LogicName(data.devIndex, tempI, strLogicFlag[tempJ], RUNTIME_CLASS(RailSwitch));
			if (devIO.addr_ != 0xFFFF)
			{
				inputZcCfg[0] = getZCInputCfgUnitByIndex2Type(devIO.addr_, DEV_TYPE_LTE_ATOCP);
				inputZcCfg[1] = getZCInputCfgUnitByIndex2Type(devIO.addr_, DEV_TYPE_LTE_UTOCP);

				for (tempJ = 0; tempJ < 2 && inputZcCfg[tempJ].devIndex != 0xFFFF; tempJ++)
				{
					strName = pSwitch->logicDeviceIO_[tempI].getName() + L"-" + zcRelayType[tempJ];
					CZcCommRelay* pZcCommRelay = new CZcCommRelay(pSwitch->data_.section_.getName(), T_ZC_SWIT, strName, TRUE, FALSE, pSwitch);
					pZcCommRelay->boardID_[0] = boardZcCommID(inputZcCfg[tempJ].CodeSeqIndex);
					pZcCommRelay->portID_[0] = portZcCommID(inputZcCfg[tempJ].CodeSeqIndex);
					pZcCommRelay->codeSeqIndex[0] = inputZcCfg[tempJ].CodeSeqIndex;

					pSwitch->vZcCommRelay_.push_back(pZcCommRelay);
					vZcCommRelay_.push_back(pZcCommRelay);
				}
			}
		}
	}
}
