#pragma once

struct DeviceIO
{
	DeviceIO(void);
	~DeviceIO(void){}
	enum 
	{
		MAX_NAME_LEN = 16,
	};
	enum DEV_BTN_ID_BASE
	{
		SECTION_BTN_BASE	= 0X30,
		SWITCH_BTN_BASE		= 0X23,
		SIGNAL_BTN_BASE		= 0X10,
		BTN_ID_BASE			= 0X20,
	};

	enum N_BITS_PER_DEV
	{
		N_BITS_PER_SIGNAL	= 4,
		N_BITS_PER_SECTION	= 2,
		N_BITS_PER_SWITCH	= 2,
	};
public:
	char	name_[MAX_NAME_LEN];
	UINT16	addr_;		// 地址
	UINT16	btnID_;		// 按钮号
	UINT16	byteFlag_;	// byte 序号
	UINT16	bitFlag_;	// bit 序号
	UINT16	stationID_;	// 车站 ID

	CString getName(void){ return CString(name_); };
};



struct DeviceInput
{
	
public:
	char	name_[16];	// 名称
	UINT16	addr_;		// 地址
	UINT16	btnID_;		// 按钮号
	UINT16	byteFlag_;	// byte 序号
	UINT16	bitFlag_;	// bit 序号
	UINT16	stationID_;	// 车站 ID
public:
	CString getName(void){ return CString(name_); };
};





struct SectionInput :
	public DeviceInput
{
	UINT16 relayAddr_; // 区段占用
	UINT16 relayAddr_2;  //预复位
};

struct PourInput
{
	CString	name;
	UINT16 relayAddr;
};


struct LingSanInput :
	public DeviceInput
{
	UINT16 relayAddr_; // 零散继电器
};

struct SignalInput :
	public DeviceInput
{
	UINT16 	type_;			// 信号机类型
	UINT16	btnID2_;		// 列车按钮号
	UINT16	relayAddr_[10];	// 采集继电器地址
};


struct SwitchInput :
	public DeviceInput
{
	UINT16 relayAddr_[3];	// 采集继电器地址
};


struct DeviceOutput
{

	char name[20];
	UINT16 addr;
};

struct SectionOutput :
	public DeviceOutput
{
	UINT16 style;
	UINT16 btn;
	UINT16 devJ1;
};

struct SignalOutput : DeviceOutput
{
	UINT16 style;  //显示类型
	UINT16 btn;
	UINT16 btn2;
	UINT16 LXJ;
	UINT16 ZXJ;
	UINT16 DXJ;
	UINT16 TXJ;
	UINT16 LUXJ;
	UINT16 YXJ;
	UINT16 DDJ;
	UINT16 SGJ;
	UINT16 AFJ;
	UINT16 BFJ;
	UINT16 CFJ;
};


struct SwitchOutput : DeviceOutput
{
	UINT16 btn;
	UINT16 DCJ;
	UINT16 FCJ;
	UINT16 SJ;
};


struct OtherOutput : DeviceOutput
{
	UINT16 style;
	UINT16 btn;
	UINT16 devJ1;
};

struct PourOutput : DeviceOutput
{
	UINT16 style;
	UINT16 btn;
	UINT16 devJ1;
};


struct DeviceData
{
	enum
	{
		N_BTYES_BACKUP = 16,
	};
	bool	bValid_;		// 按钮是否有效
	LONG	lfEscapement;	// 旋转角度
	UINT32	backup_[N_BTYES_BACKUP];
	DeviceData(void);
	~DeviceData(void){};
};

