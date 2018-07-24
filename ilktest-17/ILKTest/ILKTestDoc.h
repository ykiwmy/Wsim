// ILKTestDoc.h : CILKTestDoc 类的接口
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

#define SignalColor_GREEN   85636    //信号机颜色
#define SignalColor_RED     85637
#define SignalColor_DISTUEBED   85638
#define SignalColor_YELLOW   85639
#define SignalColor_HH   85640
#define SignalColor_AUTO   85651
#define SignalColor_MD   85641

#define Switch_ForceRight 85536   //道岔状态
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
	DWORD localIP_;		// 本地IP
	DWORD LocalATPServerIP_;	// 下位机IP

	int localIOPort_;		// 本地端口
	int localATPPort_;		// 本地ATP端口

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
	CADDevice* pDev;//站台设备
	CADStation* pSta;//站台
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
	UINT16 originSignal;	/*始端信号机索引号*/
	UINT16 rlsSectIndex;
	UINT16 delayTime[2];		/*延时解锁时间*/
	UINT16 dir; /*保护区段方向*/
	UINT16 nSection;		/*保护区段数量*/
	UINT16 sectIndex[MAX_OLPTABLE_SESW_NUM];	/*保护区段索引号*/
	UINT16 nSwitch;
	UINT16 switIndex[MAX_OLPTABLE_SESW_NUM];
	UINT16 switPos[MAX_OLPTABLE_SESW_NUM];
	UINT16 nFSwitch; /*防护道岔*/
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
#define DEV_TYPE_SIGN_STATE	0x0004u //信号机
#define DEV_TYPE_PHY_STATE			0x0007u //物理区段
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
protected: // 仅从序列化创建
	CILKTestDoc();
	DECLARE_DYNCREATE(CILKTestDoc)
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 实现
public:
	virtual ~CILKTestDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	vector<CADGraphic*>		vGraphic_;
	vector<CADDevice*>		vDevice_;			// 设备
	vector<Relay*>			vRelay_;			// 存储所有继电器
	vector<SwitchSection*>	vSwitchSection_;	// 有岔区段
	vector<CADStation*>	vStation_;	// 站台
	vector<CADDevice*> vLingSanDev;

	vector<array<IOItem, 16>> vInputBoard_;		// 输入板
	vector<array<IOItem, 16>> vOutputBoard_;	// 输出板

	
	vector<array<IOItem, 8>> vZCSimBoard_; //zc仿真板
	vector<array<IOItem, 8>> vZCSimOutputBoard_;
	//vector<CBaseRoute*> vBaseRoute_;

	/*配置数据*/
	vector<RtOne> vRtOne_;
	vector<ADDR_UNIT_RT> vRouteAddr_;
	vector<Phy2LogicCfg> vPhy2Logic_;
	vector<PsdCfgData> vPsdCfgData_;
	ZCCommHead m_ZcConnHead_;
	vector<ZCInputCfgData> vZcInputCfgData_;
	vector<StProSectionData_EX2> vOverlapTable_;

	vector<CZcCommRelay*> vZcCommRelay_;
	

	vector<ElementCode>			vElements_;			// ATP接口元素信息表
	vector<ToSimCfgData>			vToSimCfgData;			// ATP接口元素信息表-3M

	NetAddress netAddr_;	// 通信地址和端口号
	CIOSocket  ioSock_;		// 驱采通信
	CIOSocket*  m_pioSock[2];
	CMySever m_tcpSevSock_;
	CIOSocket*  m_p3MSock;
	CIOSocket*  m_pZcSock_;
protected:
	DECLARE_MESSAGE_MAP()

public:
	int getBoardCount(BOOL bInput); // 获取输入板个数

	void loadGraphic(CArchive& ar); // 读取图形元素

	void loadInputRelay(CString strPath); // 读取输入板信息

	void setSectionRelay(SectionInput* pInput); // 读取区段信息

	CADDevice* findDevcieByAddr(int addr, CRuntimeClass* pRuntimeClass); // 按照地址号查找设备
	CADDevice* findDevcieByAddr(int addr, CRuntimeClass* pRuntimeClass, int iNo);
	CADDevice* findLingsanDevByAddr(int addr, CRuntimeClass* pRuntimeClass);
	CADDevice* findLingsanDevByAddr(int addr);
	CADDevice* findDevcieByEleIndex(CString index);
	CADDevice* findLingsanDevByEleIndex(CString index);
	
	CADDevice* findSwitDeviceBySwitSectAddr(int sectAddr, int iNo);
	CADDevice* findSwitDeviceByLogicAddr(int logicAddr, int iNo);
	CString getSwitLogicNameBySwitDevice(RailSwitch* pSwitch, int iNo);
	CZcCommRelay* findSwitZcCommRelayByDevice(RailSwitch* pSwitch);

	BOOL loadSwitchSection(SectionInput* pInput); // 读取有岔区段

	Relay* findRelayByAddr(int addr); // 按照地址号查找继电器

	int boardID(UINT16 relayAddr) { return relayAddr >> 8; } // 获取继电器板号

	int portID(UINT16 relayAddr) { return relayAddr & 0XFF; } // 获取继电器位号

	void setSwitchRelayIn(SwitchInput* pInput); // 读取道岔采集型继电器

	void setSignalRelayIn(SignalInput* pInput); // 读取信号机采集型继电器

	void loadOutputRelay(CString path); // 读取输出表

	void setOtherRelayOut(OtherOutput* pOutput); // 读取其他设备表

	void setSwitchRelayOut(SwitchOutput* pOutput); // 读取道岔输出表

	void setSignalRelayOut(SignalOutput* pOutput); // 读取信号机输出表

	void OnSetSwitchSection(int sectionAddr, RailSwitch* pFrom); // 设置了有岔区段的占用状态，此时要刷新有岔区段中其他道岔的占用状态

	void createIOBoard(); // 创建输入输出板
	void createZCSimBoard(); //创建ZC仿真板

	Relay* findRelay(int boardID, int portID, BOOL bInput); // 根据输入输出板的端口号查找对应的继电器
	CZcCommRelay* findZcCommRelay(int boardID, int portID, BOOL bInput);

	void createIOItem(vector<array<IOItem, 16>>& vIOItem_, CWnd* pGroupWnd, BOOL bInput); // 创建 IOItem 的窗口
	void createZCIOItem(vector<array<IOItem, 8>>& vIOItem_, CWnd* pGroupWnd, BOOL bInput); // 创建 IOItem 的窗口
	int boardZcCommID(UINT16 codeSeqIndex);
	int portZcCommID(UINT16 codeSeqIndex);
	int getBoardZcCommCount(BOOL bInput);

	void checkIOValid(); // 检查每个IO端口是否有冲突

	void InitSockets(); // 初始化网络

	void updateIO(const char* pOutputData, int channel, int len);
	void packageIO(IOData* pInputData); // 打包采集类型继电器的状态

	CADDevice* findDeviceByName(CString& name);
	CADDevice* findLogicDeviceByName(int addr, CString logicName, CRuntimeClass* pRuntimeClass);
	DeviceIO findSwitLogicIndexBySwitIndex2LogicName(int addr, int iNo, CString logicName, CRuntimeClass* pRuntimeClass);


	void packageATP(UINT8* pData,INT32* len);

	void updateATP(UINT8* pData,INT32 len);

	void OnTimer();

	//ElementCode Elements[160];

	void loadElementData(CString strPath); // 读取输出表
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
	// 根据站号查找站台
	CADStation* findStationByNumb(int numb);

	
	void SetStationRelay(LingSanInput* input, StationDev stationDev, BOOL bRelayUp);
	CADDevice* findAfgDoorByAddr(int addr);
	CADDevice* findDriByAddr(int addr);
	// 用于存储KMJ、GMJ，不是屏蔽门的返回false
	bool addToDevByRelayName(OtherOutput* pOutput);
	StationDev findStationPsdDevByName(CString ourRelayName);
	CString getRelayDevName(CString relayName);
	CString findStationPsdName(CADStation* pStation, int* psdDoorNumb);
	CADDevice* findSwitchBySectAddr(UINT16 addr);
	CADDevice* findSwitchBySectEleIndex(CString  eleIndex);
	void OnDeviceTimer();
	CADDevice* findLingDevByRelay(LingSanInput* input);
	// 读取网络配置文件
	void loadNetConfig(CString path);
	void InitSocketByCfg();
	// 设备是否是逻辑区段A
	BOOL isNameRightA(CString name);
	CADDevice* findDevcieByIORelayAddr(int addr, int board, int port, CRuntimeClass* pRuntimeClass);
	CADDevice* findDevByBtnID(UINT16 btnId);
	// 是否是逻辑区段站场
	int m_isLogicSection; // 1为逻辑区段站场 0为非逻辑区段站场
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
	int m_iAutoUpdateZCStat_; //自动跨压、停稳
	int m_iManUpdateAswStat_; // 手动停车保证
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
	// 励磁时间，0->1
	int m_nUpTime;
	// 继电器消磁时间
	int m_nDownTime;
	int GetMultiByteLen(CString str);
};
