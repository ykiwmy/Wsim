#pragma once
#include "afxwin.h"
#include "ILKTestDoc.h"

#define MAXPHYNUMB 200
#define  MAXSIGNALNUMB 100
#define MAXSWITCHNUMB  100
#define  MAXPSDNUMB  20
#define MAXEMPNUMB 20
#define MAXDRLNUMB 10
#define MAXDRBNUMB 10

struct Dev_Data_3M
{
	UINT16 Dev_ID;
	UINT8 Dev_Status;
};

struct Dev_Data2_3M
{
	UINT16 Dev_ID;
	UINT8 Dev_Status1;//闪灯
	UINT8 Dev_Status2;//稳灯
};


enum Signal_Status_3M
{
	RED_3M = 0x01, YELLOW_3M, REDYELLOW_3M, GREEN_3M,
	YELLOW_C_3M/*黄灭*/, RED_C_3M, GREEN_C_3M,
	WHITE_3M=0x08,/*BLUE_3M,*/
	RED_D_3M/*红断*/ = 0x09, GREEN_D_3M = 0x10, YELLOW_D_3M = 0x20, WHITE_D_3M = 0x30,
};
enum Switch_Status_3M
{
	NORMAL_3M = 0x01, REVERSE_3M = 0x02, NOSTATE_3M = 0x04/*四开*/
};

#define OCCUPY_3M 0xaa
#define CLEAR_3M 0x55

#define OCCUPY_3M 0xaa
#define CLEAR_3M 0x55

#define  PSDCLOSED_3M 0x55
#define  PSDOPEN_3M 0xaa
#define  RPSDNLOCK_3M  0x55   //未互锁解除
#define  RPSDLOCK_3M  0xaa 

#define  EMPUP_3M  0x55  // 吸起，未按下，紧停无效
#define  EMPDOWN_3M  0xaa  //// 紧停有效

#define  DRBDOWN_3M   0xaa  // 按下
#define  DRBUP_3M  0x55

#define  DRLS_3M   0xaa  // 闪
#define  DRLNS_3M  0x55
#define  DRLW_3M   0xaa  // 稳
#define  DRLNW_3M  0x55

struct TS2CIData
{
	UINT8 nTs_Type;
	UINT8 nTs_ID;
	UINT8 nCI_Type;
	UINT8 nCI_ID;

	UINT16 Phy_Numb;
	Dev_Data_3M  phy_data[MAXPHYNUMB];

	UINT16 Signal_Numb;
	Dev_Data_3M signal_data[MAXSIGNALNUMB];

	UINT16 Switch_Numb;
	Dev_Data_3M switch_data[MAXSWITCHNUMB];

	UINT16 Psd_Numb;
	Dev_Data2_3M psd_data[MAXPSDNUMB];

	UINT16 Emp_Numb;
	Dev_Data_3M emp_data[MAXEMPNUMB];

	UINT16 Drb_Numb;
	Dev_Data_3M drb_data[MAXDRBNUMB];
};

struct CI2TSData
{
	UINT8 nCI_Type;
	UINT8 nCI_ID;
	UINT8 nTs_Type;
	UINT8 nTs_ID;

	UINT16 Phy_Numb;
	Dev_Data_3M  phy_data[MAXPHYNUMB];

	UINT16 Signal_Numb;
	Dev_Data_3M signal_data[MAXSIGNALNUMB];

	UINT16 Switch_Numb;
	Dev_Data_3M switch_data[MAXSWITCHNUMB];

	UINT16 Psd_Numb;
	Dev_Data_3M psd_data[MAXPSDNUMB];

	UINT16 Drl_Numb;
	Dev_Data2_3M drl_data[MAXDRLNUMB];
};
class CSAMProcess :
	public CWnd
{
public:
	CSAMProcess(CILKTestDoc* pDoc, int ciId);
	virtual ~CSAMProcess();
	int m_myId;
	CILKTestDoc* pDoc_;
	void SetRcvData(const char* rcvBuff, int buffLen);
	TS2CIData m_rcvData;
	CI2TSData m_sndData;
	void ProcessRcvData();
	void SetSectionState(Dev_Data_3M data);
	void SetPsdState(Dev_Data2_3M data);
	void SetEmpState(Dev_Data_3M data);
	void SetDrbState(Dev_Data_3M data);
	void ProcessSendData();
	void SetSwitchStatus(RailSwitch* pSwitch);
	void SetSwitchSectionStatus(RailSwitch* pSwitch);
	void SetSectionStatus(CADSection* pSect);
	void SetSignalStatus(CADSignal* pSignal);
	void SetPsdStatus(CADDoor* pDoor);
	void PackSendData(char * sendBuff, int* sendBuffLen);
	void SetDRIStatus(CADDeLight* pLight);
	void SetSwitchState(Dev_Data_3M data);
	Dev_Data_3M GetDevData(const char* inBuff);
	void SetDevData(Dev_Data_3M data, char* inBuff);
};

