// ILKTestDoc.h : CILKTestDoc ��Ľӿ�
//
#pragma once
#include "IOItem.h"
#include "IOSocket.h"
#include "TCPSocket.h"
#include <vector>
#include <array>
#include "MySever.h"
#include "BaseRoute.h"
using namespace std;

class  CADGraphic;
class  CADDevice;
class  CADStation;
class  SwitchSection;
class  Relay;
class  CZcCommRelay;
class  RailSwitch;
class  CIOView;
class  TestCase;
class  CADAfgDoor;
class  CSAMProcess;
class CBaseRoute;

struct DeviceIO;
struct DeviceOutput;
struct DeviceInput;
struct SectionInput;
struct SwitchInput;
struct SignalInput;
struct OtherOutput;
struct SwitchOutput;
struct SignalOutput;
struct SectionOutput;
struct LingSanInput;

#define MessageIDToTrainSim 10710    //MESSAGE ID
#define MessageIDTVSSet     10700
#define MessageIDTrail      10724

#define SignalColor_GREEN   85636    //�źŻ���ɫ
#define SignalColor_RED     85637
#define SignalColor_DISTUEBED   85638
#define SignalColor_YELLOW   85639
#define SignalColor_HH   85640
#define SignalColor_AUTO   85651
#define SignalColor_MD   85641

#define Switch_ForceRight 85536   //����״̬
#define Switch_ForceLeft  85538
#define Switch_Left       85549
#define Switch_Right      85550
#define Switch_Trailed    85551

#define STX 0x02
#define ETX 0x03
#define DLE 0x10
#define XON 0x12
#define XOFF 0x14

#define Free 86837
#define  Occupied 86838


struct NetAddress
{
	DWORD localIP_;		// ����IP
	DWORD LocalATPServerIP_;	// ��λ��IP

	int localIOPort_;		// ���ض˿�
	int localATPPort_;		// ����ATP�˿�

	CString localIP_Me[2];
	CString remoteIP_CI[2];

	int localIOPort_Me[2];
	int remotePort_CI[2];

	CString localIP_3M;
	CString remoteIP_3M;
	int localPort_3M;
	int remotePort_3M;

	CString localIP_ATP_Me;
	int localATPPort_Me;

	CString localIP_ZC;
	CString remoteIP_ZC;
	int localPort_ZC;
	int remotePort_ZC;
};

struct ElementCode
{
	UINT16 DevID;
	char ixl_elementname[64];
	char ixl_elementkind[16];
	UINT16 ixl_elementkind_id;
	UINT16 ixl_element_index;
	UINT16 instance_kind_id_to_wcu;
	UINT16 instance_kind_id_from_wcu;
	UINT16 ixl_instance_index;
	UINT16 wcu_instance_kind_id;
	UINT16 wcu_instance_index;
	UINT16 ixl_Devaddr;
	UINT16  io_type;//(0001 == input, 0010 == output, 0011 == input and output)
	UINT16 inputaddr;
	UINT16 outputaddr;
};
struct  StationDev
{
	CADDevice* pDev;//վ̨�豸
	CADStation* pSta;//վ̨
};
typedef struct {
	char name[20];
	unsigned short devAddr;  //

}DevOne;

typedef struct SAM_ZC_SIM_CFG
{
	UINT16 devType;
	DevOne dev;
	UINT32 zcIndex;

}ToSimCfgData;

typedef struct PSD_CFG_DATA
{
	UINT16 index[6]; //0-1out 2-5in
}PsdCfgData;

typedef struct SAM_COMM_HEAD
{
	UINT16 zcType;
	UINT16 zcId;
	UINT16 ciType;
	UINT16 ciId;

}ZCCommHead;

typedef struct SAM_INPUT_CODE_CFG
{
	UINT16 devIndex;
	UINT16 devType;
	UINT16 zcIndex;
	UINT16 CodeSeqIndex;

}ZCInputCfgData;

#define NAMENUM				 20
#define RT_DEVNUM         30
#define MAX_OLPTABLE_SESW_NUM 10
typedef struct
{
	char   name[NAMENUM];
	UINT16 addr;

}RtOne;

typedef struct PHY2LOGICCFG
{
	UINT16 logicIndex;
	UINT16 tvsIndex;

}Phy2LogicCfg;

typedef struct /*TAG_StOverlapT*/
{
	UINT16 originSignal;	/*ʼ���źŻ�������*/
	UINT16 rlsSectIndex;
	UINT16 delayTime[2];		/*��ʱ����ʱ��*/
	UINT16 dir; /*�������η���*/
	UINT16 nSection;		/*������������*/
	UINT16 sectIndex[MAX_OLPTABLE_SESW_NUM];	/*��������������*/
	UINT16 nSwitch;
	UINT16 switIndex[MAX_OLPTABLE_SESW_NUM];
	UINT16 switPos[MAX_OLPTABLE_SESW_NUM];
	UINT16 nFSwitch; /*��������*/
	UINT16 fSwitIndex[MAX_OLPTABLE_SESW_NUM];
	UINT16 fSwitPos[MAX_OLPTABLE_SESW_NUM];

	RtOne devOther[RT_DEVNUM];

}StProSectionData_EX2;

struct OFFSET_ADDR
{
	unsigned char tableIndex;
	unsigned char offsetIndex;
};

struct ADDR_UNIT_RT
{
	UINT16 startBtn;
	UINT16 alterBtn[2];
	UINT16 endBtn;
	UINT16 offsetBtn;
	UINT16 rtType;
	UINT16 nextIndex;
	OFFSET_ADDR selfAddr;
};


#define DEV_TYPE_SWITCH_STATE  0x0001
#define DEV_TYPE_SIGN_STATE	0x0004u //�źŻ�
#define DEV_TYPE_PHY_STATE			0x0007u //��������
#define DEV_TYPE_PSD_STATE			0x000Fu //PSD
#define DEV_TYPE_ESB_STATE			0x0010u //ESB
#define DEV_TYPE_ATB_STATE			0x0013u //ATB

#define UNUSERELAYOUTADDR	0xFF07
#define UNUSERELAYINADDR	0xFFFF


/*zc-ci mask*/
#define DEV_TYPE_SWIT_NORMAL		0x0001u
#define DEV_TYPE_SWIT_REVERSE		0x0002u
#define DEV_TYPE_SWIT_LOCK			0x0003u


#define DEV_TYPE_SIGN_OVEROCCUPIED	0x0004u
#define DEV_TYPE_SIGN_LIGHTSTATE	0x0005u
#define DEV_TYPE_SIGN_OPENSTATE		0x0006u

#define DEV_TYPE_PHY_ARB			0x0007u
#define DEV_TYPE_PHY_STOP			0x0008u
#define DEV_TYPE_PHY_SLOCK			0x0009u
#define DEV_TYPE_PHY_XLOCK			0x000Au
#define DEV_TYPE_PHY_OCCUPIED		0x000Bu
#define DEV_TYPE_PHY_RTLOCK			0x000Cu

#define DEV_TYPE_LTE_ATOCP			0x000Du
#define DEV_TYPE_LTE_UTOCP			0x000Eu

#define DEV_TYPE_PSD_STATE			0x000Fu
#define DEV_TYPE_ESB_STATE			0x0010u
#define DEV_TYPE_ATB_FLASH			0x0011u
#define DEV_TYPE_ATB_STABLE			0x0012u
#define DEV_TYPE_ATB_STATE			0x0013u

#define DEV_TYPE_AFG_STATE			0x0014u
#define DEV_TYPE_ROUTE_STOPASW		0x0015u
#define DEV_TYPE_ATBRT_STOPASW		0x0016u
#define DEV_TYPE_ROUTE_STATE		0x0017u
#define DEV_TYPE_ATBRT_STATE		0x0018u
#define DEV_TYPE_ROUTE_STOPASK		0x0019u
#define DEV_TYPE_ATBRT_STOPASK		0x001Au
#define DEV_TYPE_OLP_STATE			0x001Bu
#define MAX_CONNTIME_WITH_ZC		14
/*zc-ci mask end*/

enum ZC_RELAY_TYPE
{
	T_ZC_SIGN, T_ZC_PHY, T_ZC_SWIT, T_ZC_LOGIC, T_ZC_AFG, T_ZC_PLAT, T_ZC_DRB
};

class CILKTestDoc : public CDocument
{
protected: // �������л�����
	CILKTestDoc();
	DECLARE_DYNCREATE(CILKTestDoc)
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CILKTestDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	vector<CADGraphic*>		vGraphic_;
	vector<CADDevice*>		vDevice_;			// �豸
	vector<Relay*>			vRelay_;			// �洢���м̵���
	vector<SwitchSection*>	vSwitchSection_;	// �в�����
	vector<CADStation*>	vStation_;	// վ̨
	vector<CADDevice*> vLingSanDev;

	vector<array<IOItem, 16>> vInputBoard_;		// �����
	vector<array<IOItem, 16>> vOutputBoard_;	// �����

	
	vector<array<IOItem, 8>> vZCSimBoard_; //zc�����
	vector<array<IOItem, 8>> vZCSimOutputBoard_;
	//vector<CBaseRoute*> vBaseRoute_;

	/*��������*/
	vector<RtOne> vRtOne_;
	vector<ADDR_UNIT_RT> vRouteAddr_;
	vector<Phy2LogicCfg> vPhy2Logic_;
	vector<PsdCfgData> vPsdCfgData_;
	ZCCommHead m_ZcConnHead_;
	vector<ZCInputCfgData> vZcInputCfgData_;
	vector<StProSectionData_EX2> vOverlapTable_;

	vector<CZcCommRelay*> vZcCommRelay_;
	

	vector<ElementCode>			vElements_;			// ATP�ӿ�Ԫ����Ϣ��
	vector<ToSimCfgData>			vToSimCfgData;			// ATP�ӿ�Ԫ����Ϣ��-3M

	NetAddress netAddr_;	// ͨ�ŵ�ַ�Ͷ˿ں�
	CIOSocket  ioSock_;		// ����ͨ��
	CIOSocket*  m_pioSock[2];
	CMySever m_tcpSevSock_;
	CIOSocket*  m_p3MSock;
	CIOSocket*  m_pZcSock_;
protected:
	DECLARE_MESSAGE_MAP()

public:
	int getBoardCount(BOOL bInput); // ��ȡ��������

	void loadGraphic(CArchive& ar); // ��ȡͼ��Ԫ��

	void loadInputRelay(CString strPath); // ��ȡ�������Ϣ

	void setSectionRelay(SectionInput* pInput); // ��ȡ������Ϣ

	CADDevice* findDevcieByAddr(int addr, CRuntimeClass* pRuntimeClass); // ���յ�ַ�Ų����豸
	CADDevice* findDevcieByAddr(int addr, CRuntimeClass* pRuntimeClass, int iNo);
	CADDevice* findLingsanDevByAddr(int addr, CRuntimeClass* pRuntimeClass);
	CADDevice* findLingsanDevByAddr(int addr);
	CADDevice* findDevcieByEleIndex(CString index);
	CADDevice* findLingsanDevByEleIndex(CString index);
	
	CADDevice* findSwitDeviceBySwitSectAddr(int sectAddr, int iNo);
	CADDevice* findSwitDeviceByLogicAddr(int logicAddr, int iNo);
	CString getSwitLogicNameBySwitDevice(RailSwitch* pSwitch, int iNo);
	CZcCommRelay* findSwitZcCommRelayByDevice(RailSwitch* pSwitch);

	BOOL loadSwitchSection(SectionInput* pInput); // ��ȡ�в�����

	Relay* findRelayByAddr(int addr); // ���յ�ַ�Ų��Ҽ̵���

	int boardID(UINT16 relayAddr) { return relayAddr >> 8; } // ��ȡ�̵������

	int portID(UINT16 relayAddr) { return relayAddr & 0XFF; } // ��ȡ�̵���λ��

	void setSwitchRelayIn(SwitchInput* pInput); // ��ȡ����ɼ��ͼ̵���

	void setSignalRelayIn(SignalInput* pInput); // ��ȡ�źŻ��ɼ��ͼ̵���

	void loadOutputRelay(CString path); // ��ȡ�����

	void setOtherRelayOut(OtherOutput* pOutput); // ��ȡ�����豸��

	void setSwitchRelayOut(SwitchOutput* pOutput); // ��ȡ���������

	void setSignalRelayOut(SignalOutput* pOutput); // ��ȡ�źŻ������

	void OnSetSwitchSection(int sectionAddr, RailSwitch* pFrom); // �������в����ε�ռ��״̬����ʱҪˢ���в����������������ռ��״̬

	void createIOBoard(); // �������������
	void createZCSimBoard(); //����ZC�����

	Relay* findRelay(int boardID, int portID, BOOL bInput); // �������������Ķ˿ںŲ��Ҷ�Ӧ�ļ̵���
	CZcCommRelay* findZcCommRelay(int boardID, int portID, BOOL bInput);

	void createIOItem(vector<array<IOItem, 16>>& vIOItem_, CWnd* pGroupWnd, BOOL bInput); // ���� IOItem �Ĵ���
	void createZCIOItem(vector<array<IOItem, 8>>& vIOItem_, CWnd* pGroupWnd, BOOL bInput); // ���� IOItem �Ĵ���
	int boardZcCommID(UINT16 codeSeqIndex);
	int portZcCommID(UINT16 codeSeqIndex);
	int getBoardZcCommCount(BOOL bInput);

	void checkIOValid(); // ���ÿ��IO�˿��Ƿ��г�ͻ

	void InitSockets(); // ��ʼ������

	void updateIO(const char* pOutputData, int channel, int len);
	void packageIO(IOData* pInputData); // ����ɼ����ͼ̵�����״̬

	CADDevice* findDeviceByName(CString& name);
	CADDevice* findLogicDeviceByName(int addr, CString logicName, CRuntimeClass* pRuntimeClass);
	DeviceIO findSwitLogicIndexBySwitIndex2LogicName(int addr, int iNo, CString logicName, CRuntimeClass* pRuntimeClass);


	void packageATP(UINT8* pData,INT32* len);

	void updateATP(UINT8* pData,INT32 len);

	void OnTimer();

	//ElementCode Elements[160];

	void loadElementData(CString strPath); // ��ȡ�����
	void StartMapTrainsimData(int Count);

	int findSinglePakageEnd(UINT8* buffer, int length, int startpos);

	int nElementTotal;
	int driveTimeCount;
	void setSectionRelayOut(SectionOutput* output);
	void setLingSanRelayIn(LingSanInput* input);
	bool isRelayExist(CString name);
	void setSwitchSectionRelayOut(SectionOutput* output);
	void loadPourRelay();
	afx_msg void OnMenuAtpShow();
	CADDevice* findDevcieByATPMsg(CString type, CString index);

	StationDev findStationDevByAddr(int addr, BOOL* bInitRelayUp);
	CADDevice* findStationDevByAddr_F(int addr);
	CADDevice* findStationESBByAddr(const CADStation* pStation, const int addr);
	CADDevice* findStationPSDByAddr(const CADStation* pStation, const int addr, BOOL* bInitRelayUp);
	void OutToFile(CString str);
	CString strPath;
	BOOL isFirstOpenFile;
	// ����վ�Ų���վ̨
	CADStation* findStationByNumb(int numb);

	
	void SetStationRelay(LingSanInput* input, StationDev stationDev, BOOL bRelayUp);
	CADDevice* findAfgDoorByAddr(int addr);
	CADDevice* findDriByAddr(int addr);
	// ���ڴ洢KMJ��GMJ�����������ŵķ���false
	bool addToDevByRelayName(OtherOutput* pOutput);
	StationDev findStationPsdDevByName(CString ourRelayName);
	CString getRelayDevName(CString relayName);
	CString findStationPsdName(CADStation* pStation, int* psdDoorNumb);
	CADDevice* findSwitchBySectAddr(UINT16 addr);
	CADDevice* findSwitchBySectEleIndex(CString  eleIndex);
	void OnDeviceTimer();
	CADDevice* findLingDevByRelay(LingSanInput* input);
	// ��ȡ���������ļ�
	void loadNetConfig(CString path);
	void InitSocketByCfg();
	// �豸�Ƿ����߼�����A
	BOOL isNameRightA(CString name);
	CADDevice* findDevcieByIORelayAddr(int addr, int board, int port, CRuntimeClass* pRuntimeClass);
	CADDevice* findDevByBtnID(UINT16 btnId);
	// �Ƿ����߼�����վ��
	int m_isLogicSection; // 1Ϊ�߼�����վ�� 0Ϊ���߼�����վ��
	int ixlIDForGalaxy;
	int ciIdWith3M;
	BOOL isProcessThisCnl(int channel);
	void updateIOData(const IOData* pOutputData, int channel);
	void load3MData(CString strPath);
	void StartMap3MDevData();
	CADDevice* findSwitchBySectID_3M(UINT16 devId);
	CADDevice* findLingsanDevByID_3M(UINT16 devId, CRuntimeClass* pRuntimeClass);
	CADDevice* findDevcieByDevID_3M(UINT16 devId, CRuntimeClass* pRuntimeClass);
	CSAMProcess* m_3MProcess;
	void SendAllShowTo3M();
	void SendIOData();
	void CheckCnctStatus();
	BOOL isSwitchRelayTrue(SwitchInput* pInput);
	BOOL isSignalRelayTrue(SignalInput* pInput);
	UINT16 GetUINT16ByBigData(const char* inBuff);
	void GetBigDataByUINT16(UINT16 val, char* outBuff);

	CADDevice* findDriByNum(int iNo);
	
	void loadRouteAddrData(CString strPath);
	void loadRouteTableData(CString strPath);
	void loadLogicTvsCfgData(CString strPath);
	void loadPsdCfgData(CString strPath);
	void loadOlpData(CString strPath);
	void load3MZCData(CString strPath);
	bool checkZcConnHead();
	void setZcCommRelay();
	CZcCommRelay* isZcCommRelayExsist(int devType, int devIndex);
	void setSignalZcCommRelay(ZCInputCfgData data);
	void setOlpSignalZcCommRelay(ZCInputCfgData data);
	void setRtStartSignZcCommRelay(ZCInputCfgData data);
	void setPhySectZcCommRelay(ZCInputCfgData data);
	void setLogicZcCommRelay(ZCInputCfgData data);

	void setOtherDRBZcCommRelay(ZCInputCfgData data);
	void setSepartLine(ZCInputCfgData data);
	void setSwitSectLogicZcCommRelay(ZCInputCfgData data);
	void setSwitchZcCommRelay(ZCInputCfgData data);
	void setPlatformZcCommRelay(ZCInputCfgData data);
	void setAfgZcCommRelay(ZCInputCfgData data);
	int getTvsIndexByLogicIndex(int logicIndex);
	int getLogicIndexByTvsIndex(int tvsIndex, int iNo);
	ZCInputCfgData getZCInputCfgUnitByIndex2Type(int devIndex, int devType);
	ZCInputCfgData getZCInputCfgUnitByCodeSeq2Type(int codeSeqIndex, int devType);
	ZCInputCfgData getZCInputCfgUnitByCodeSeq(int codeSeqIndex, BOOL bInput);

	BOOL m_bIsCommToZC;
	int m_iIsCnctToSamZC;
	int m_iDisConnCntWithZC;
	int zcIdWithZCSIM_;
	int ciIdWithZCSIM_;
	int zcTypeWithZCSIM_;
	int ciTypeWithZCSIM_;
	int m_iAutoUpdateZCStat_; //�Զ���ѹ��ͣ��
	int m_iManUpdateAswStat_; // �ֶ�ͣ����֤
	int packZcSecndData(UINT8* pData/*, int& dataLen*/);
	bool isLogicHighState(CZcCommRelay* pCurZcRelay);
	void sendZcData();
	void updateCI2ZCData(const char* pCiData, int len);
	void updateInputByOutput(CZcCommRelay* pCurZcRelay);
	bool recvDataHeadCheck(const char* pCiData);
	int getRouteSignAskStateInfo(CZcCommRelay* pCurZcRelay);
	int getRouteSectRTLOCKStateInfo(CZcCommRelay* pCurZcRelay);
	void setSwitSectOtherSwitARBState(CZcCommRelay* pCurZcRelay);
	void setSwitSectOtherSwitStopState(CZcCommRelay* pCurZcRelay);
	void setSwitSectAllSwitOcpState(CZcCommRelay* pCurZcRelay);
	void setSwitLogicState(CZcCommRelay* pCurZcRelay);

	CZcCommRelay* getCommRelayDevice(int devType, CString devName, CString RelayName);
	CZcCommRelay* getCommRelayDevice(int devType, int devIndex, CString devName);
	bool F_RSSP1_ReadRssp1Cfg(CString strPath);

	UINT16 getSignalIndexByOlpIndex(UINT16 olpIndex);
	UINT16 getSignalIndexByRtIndex(UINT16 rtIndex);
	CString getRouteNameByRtIndex(UINT16 rtIndex, UINT16 rtType);

	afx_msg void OnAllNorPos();
	afx_msg void OnAllRevPos();
	afx_msg void OnAll4KaiPos();
	afx_msg void OnAllOcpState();
	afx_msg void OnAllClearOcpState();
	afx_msg void OnAllPrePos();
	afx_msg void OnAllDS();
	afx_msg void OnAllClearDS();
	afx_msg void OnAllClearPreReset();
	afx_msg void OnAllLightOFF();
	afx_msg void OnAllLightON();
	afx_msg void OnAllLTEAT();
	afx_msg void OnAllLTEClear();
	afx_msg void OnAllLTEEeror();
	afx_msg void OnAllLTEUT();
	
	int m_isCnctToSAM;
	int m_isCnctToGalaxy;
	void SetGroupRect(CWnd* pwndGroup);
	afx_msg void OnDisConnToZc();
	afx_msg void OnUpdateDisConnToZc(CCmdUI *pCmdUI);
	// ����ʱ�䣬0->1
	int m_nUpTime;
	// �̵�������ʱ��
	int m_nDownTime;
	int GetMultiByteLen(CString str);
};
