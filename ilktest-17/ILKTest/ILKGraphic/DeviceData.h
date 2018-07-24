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
	UINT16	addr_;		// ��ַ
	UINT16	btnID_;		// ��ť��
	UINT16	byteFlag_;	// byte ���
	UINT16	bitFlag_;	// bit ���
	UINT16	stationID_;	// ��վ ID

	CString getName(void){ return CString(name_); };
};



struct DeviceInput
{
	
public:
	char	name_[16];	// ����
	UINT16	addr_;		// ��ַ
	UINT16	btnID_;		// ��ť��
	UINT16	byteFlag_;	// byte ���
	UINT16	bitFlag_;	// bit ���
	UINT16	stationID_;	// ��վ ID
public:
	CString getName(void){ return CString(name_); };
};





struct SectionInput :
	public DeviceInput
{
	UINT16 relayAddr_; // ����ռ��
	UINT16 relayAddr_2;  //Ԥ��λ
};

struct PourInput
{
	CString	name;
	UINT16 relayAddr;
};


struct LingSanInput :
	public DeviceInput
{
	UINT16 relayAddr_; // ��ɢ�̵���
};

struct SignalInput :
	public DeviceInput
{
	UINT16 	type_;			// �źŻ�����
	UINT16	btnID2_;		// �г���ť��
	UINT16	relayAddr_[10];	// �ɼ��̵�����ַ
};


struct SwitchInput :
	public DeviceInput
{
	UINT16 relayAddr_[3];	// �ɼ��̵�����ַ
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
	UINT16 style;  //��ʾ����
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
	bool	bValid_;		// ��ť�Ƿ���Ч
	LONG	lfEscapement;	// ��ת�Ƕ�
	UINT32	backup_[N_BTYES_BACKUP];
	DeviceData(void);
	~DeviceData(void){};
};

