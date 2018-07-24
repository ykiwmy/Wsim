/******************************************************************************
 * File Name		:	parameter.h
 * Copyright		:	Zhuzhou CSR Times Electric Co.,Ltd. All Rights Reserved.
 * Create Data	:	2014年1月21日星期二
  *Description   : 定义软件中所需要的通用参数
 * 
 * 
 * Rev1.0.0	yzb	2014年1月21日星期二	File Create
 *
******************************************************************************/
  
/******************************************************************************
*    Multi-Include-Prevent Start Section
******************************************************************************/
#ifndef _PARAMETER_H_
#define _PARAMETER_H_
/******************************************************************************
*    Debug switch Section
******************************************************************************/
#define PCSIM				1
#define MIDRETRLS			1
/******************************************************************************
*    Include File Section
******************************************************************************/
#include <stdio.h>


/******************************************************************************
 *     Global Macro Define Section*
 ******************************************************************************/

#define MAX_SWIT_SPLITCOUNT 81u/*道岔挤岔周期数*/


#define MAX_FIRFLKPELE_NUM 5u /*最大一级侧防元件数量*/
#define MAX_FIRFLKCELE_NUM 5u /*最大一级侧防区域元件数量*/
#define MAX_SECFLKPELE_NUM 3u /*最大二级侧防元件数量*/
#define MAX_SECFLKCELE_NUM 3u /*最大二级侧防区域元件数量*/


#define MAX_DWELL_NUM		24u	/*停车轨信息*/

#define MAX_INBOOL_NUM	40u
#define MAX_OUTBOOL_NUM	30u


#define ROUTE_INTEVAL	0xFFFFu	/*进路间隔符*/

#define YX_ROUTE			0x0002u	/*进路可延续*/
#define FYX_ROUTE			0x0000u	/*进路不可延续*/
#define CLINE				0x0004u	/*侧线*/
#define ZLINE				0x0000u	/*正线*/
#define DZ					0x0008u	/*调车*/
#define LZ					0x0000u	/*列车*/
#define X_DIR				0x0010u	/*下行方向*/
#define S_DIR				0x0000u	/*上行方向*/
#define ARRIVE_ROUTE		0x0020u	/*接车进路*/
#define DEPART_ROUTE		0x0040u	/*发车进路*/
#define OTHER_ROUTE			0x0000u	/*普通进路(不区分接发车)*/
#define ONE_RLS				0x0100u/*一次解锁*/
#define THREE_RLS			0x0000u/*分段解锁*/
#define ABLE_CALLON			0x0200u/*进路具备引导属性*/
#define DISABLE_CALLON		0x0000u/*进路不具备引导属性*/
#define ZDTG_ROUTE			0x0080u/*自动通过进路*/
#define ZRJL_ROUTE      	0x4000u/*折入进路*/
#define LZGD_ROUTE			0x1000u/*调车以列车方式关灯*/
#define TSDC_ROUTE			0x2000u/*特殊调车进路*/

#define YX_TO_BLOCK         0x1000u/*延续至区间*/
#define YX_TO_SAFETY        0x2000u/*延续至安全线*/
#define YX_NO				0xFF07u

#define CBTC_SECT			0x2000u/*CBTC 区段标志，即监控区段*/
#define DELAY_SECT			0x4000u/*区段延时标志*/
#define NOOCCUPIED_SECT		0x8000u/*区段占用不检查标志*/
#define ILK_APPRSECT		0x1000u	/*联锁表中接近区段标志*/
#define ILK_LEAVESECT		0x0800u	/*联锁表中离去区段标志*/
#define ILK_SECT_SWITCH     0x0400u	/*道岔区段*/

#define ILK_OTH_AUTOREOPEN		0x0400u 	/*其它条件不满足，可自动重开标志*/
#define ILK_OTH_NOCHACK			0x0800u 	/*其它条件不满足，可排标志*/
#define ILK_OTH_CBTCNOCHECK       0x0200  //cbtc下不检查该其他条件


#define UNLOCK				0xAAu/*解锁*/
#define MPLOCK				0x55u/*单锁与保护锁*/
#define MLOCK				0x5Au/*单锁*/
#define PLOCK				0xA5u/*保护锁*/
#define CALOCK              0x53u/*引导总锁*/
#define CALOCK2             0x77u/*引导总锁*/

#define RLOCK				0x55u
#define DZLOCK				0x5Fu/*调车锁闭*/
#define DZ2LOCK				0x66u/*调车2次锁闭*/
#define LZSLOCK				0xF5u/*列车上行锁闭*/
#define LZXLOCK				0x75u/*列车下行锁闭*/

#define YSLOCK				0xB5u/*引导上行锁*/
#define YXLOCK				0x5Bu/*引导下行锁*/

#define REQUEST				0x55u/*征用*/
#define LZSREQ				0xF5u/*上行征用*/
#define LZXREQ				0x75u/*下行征用*/
#define UNREQUEST			0xAAu/*未征用*/

#define BLOCK				0x55u	/*封锁*/
#define UNBLOCK				0xAAu	/*未封锁*/

#define PROHIBIT			0x55u	/*禁止进入*/
#define UNPROHIBIT			0xAAu	/*未禁止进入*/

#define OCCUPIED			0x55u	/*占用 继电器落下*/
#define CLEAR				0xAAu	/*空闲 继电器吸起*/

#define RELAY_DOWN			0x55u
#define RELAY_UP			0xAAu

#define HOLD				0x55u	/*扣车*/
#define UNHOLD				0xAAu	/*未扣车*/

#define CLOSE				0xAAu	/*关闭*/
#define OPEN				0x55u	/*打开*/

#define RSET				0x5555u	/*进路设置*/
#define	RCLEAR				0xAAAAu	/*进路未设置*/

#define MUTLOCK				0xAAu	/*互锁*/
#define UNMUTLOCK			0x55u	/*互锁解除*/	

#define NORMAL				0x55u	/*道岔定位*/
#define REVERSE				0xAAu	/*道岔反位*/
#define SIKAI				0x5Au	/*道岔四开*/
#define SWITFAULT           0xA5u   /*道岔故障*/

#define ILK_REVERSE			0x1000u		/*联锁表中道岔反位(右位)*/
#define	ILK_NORMAL			0x0000u		/*联锁表中道岔定位(中位)*/
#define ILK_RIGHT			0x2000u		/*左位*/

#define CBTCSIGN			0x55u	/*CBTC 信号*/
#define IATPSIGN			0xAAu	/*IATP 信号*/

#define YES					0x55u	/**/
#define NO					0xAAu	/**/

#define TSR_I				0x55u //最高级
#define TSR_II				0x75u
#define TSR_III				0x95u
#define TSR_NO				0xAAu

#define BTN_UP				0x55u	/*按钮弹起*/
#define BTN_DOWN			0xAAu	/*按钮落下*/

#define DRVING				0x55u	/*驱动中*/
#define NODRV				0xAAu	/*未驱动*/

#define SIGNON				0x55u	/*信号开放*/
#define SIGNOFF				0xAAu	/*信号关闭*/

#define FLANKPROT			0x55u	/*侧防满足*/
#define UNFLANKPROT			0xAAu	/*侧防不满足*/

#define YDY_FLAG			0x01u	/*引导延时标志*/
#define ZRJ_FLAG			0x02u	/*总人解标志*/
#define ZQX_FLAG			0x03u	/*总取消标志*/
#define QGJ_FLAG			0x04u	/*区故解标志*/

#define UDELAY				0x00u	/*无延时*/
#define THREE_M				1125u	/*3 分钟延时*/
#define THIRTY_S			188u	/*30秒延时*/
#define FOURTEEN_S			88u		/*14秒*/
#define THREE_S				18u		/*3 秒延时*/
#define EIGHT_M				3000u	/*8 分钟计时*/
#define FIVEH_MS			6u		/*500MS计时*/

#define ROUTE_CALL			0x01	/*进路时引导*/
#define ALOCK_CALL			0x02	/*总锁闭引导*/

#define FLT_OCCUPIED		0x55u	/*后占用.引导进路锁闭后区段占用*/
#define FLT_BOCCUPIED		0xA5u	/*预占用.引导进路锁闭前区段占用*/
#define FLT_CLEAR			0xAAu	/*空闲*/

#define SWIT_TIME_INIT       	81 /*道岔动作初始值*/
#define OLP_TRIGGER_TIME		375u/*1分钟：保护区段触发时间*/

#define RED                  		0x55u
#define GREEN                		0xAAu
#define YELLOW               		0xA5u
#define BLUE                 		0x5Au
#define WHITE                		0x3Cu
#define DOUBLEGREEN					0xC3u
#define DOUBLEYELLOW				0x69u
#define GREENYELLOW					0x96u
#define REDYELLOW            		0x66u
#define REDWHITE					0x99u
#define DZDS1F						0x88u	/*通行灯灯丝断丝*/
#define DS1F		           		0x33u
#define DS2F			    		0xCCu
#define YINDAO						0x77u
#define NOINDICATION				0xFFu
#define CTRCLR                      0XBBu//自动闭塞灯色可变标志
#define BSD_RED						0X88u//转盘或地航表示灯点亮灯色
#define BSD_GRAY					0XEEu//转盘或地航表示灯灭灯颜色

#define FOLLOW_SWITCH				0x01u	
#define PROT_SWITCH					0x02u
#define COM_SWITCH					0x03u
#define OVERLAP_SWITCH				0x04u

#define APPRO_SWITCH				0x06u

#define COMMON_ROUTE				0x00u
#define FAST_ROUTE					0x01u
#define AUTO_ROUTE					0x02u
#define ATB_ROUTE                   0x03u

#define CROSS						0x55u
#define UNCROSS						0xAAu

#define MASTER_COMPUTER				0x55u
#define SLAVE_COMPUTER				0xAAu

#define SG_INIT				1u/*初态*/
#define SG_FAULT			2u/*故障关闭*/
#define SG_NORM				3u/*正常关闭*/

/*信号机提示信息定义*/
#define I_SG_XHFS		1u/*信号封锁*/
#define I_SG_XHJF		2u/*信号解封*/
#define I_SG_XHCK		3u/*信号重开*/
#define I_SG_SZZDCF		4u/*设置自动触发*/
#define I_SG_QXZDCF		5u/*取消自动触发*/
#define I_SG_SZZDTG		6u/*设置自动通过*/
#define I_SG_QXZDTG		7u/*取消自动通过*/
#define I_SG_TJBMZ		8u/*条件不满足*/
#define I_SG_CZCW		9u/*操作错误*/
#define I_SG_LCSB		10u/*非本场设备*/
#define I_SG_IOCW		11u/*信号机IO配置错误*/
#define I_SG_JFQQ      12u/*信号机解封请求*/
#define I_SG_YDXHTJBMZ		13u/*引导信号开放条件不满足*/
#define I_SG_YDXHBNBC		14u/*引导信号不能保持*/
#define I_SG_SZBHQD1    15u/*设置保护区段1*/
#define I_SG_SZBHQD2    16u/*设置保护区段2*/
#define I_SG_SZBHQD3    17u/*设置保护区段3*/
#define I_SG_QXBHQD     18u/*取消保护区段*/
#define I_SG_PDJ        19u/*坡道解*/

/*道岔提示信息定义*/
#define I_SW_DCDS		1u/*道岔单锁*/
#define I_SW_DCDJ		2u/*道岔单解*/
#define I_SW_DCDF		3u/*道岔封锁*/
#define I_SW_DCJF		4u/*道岔解封*/
#define I_SW_DCDC		5u/*道岔定操*/
#define I_SW_DCFC		6u/*道岔反操*/
#define I_SW_DCBSB		7u/*道岔被锁闭*/
#define I_SW_DCQDBZY	8u/*道岔区段被占用*/
#define I_SW_TJBMZ		9u/*条件不满足*/
#define I_SW_CZCW		10u/*操作错误*/
#define I_SW_LCSB		11u/*非本场设备*/
#define I_SW_SZLSXSCG   12u/*设置临时限速成功*/
#define I_SW_QXLSXSCG   13u/*取消临时限速成功*/
#define I_SW_TYDC       14u/*同意动岔*/
#define I_SW_QXTYDC     15u/*取消同意动岔*/

/*区段提示信息定义*/
#define I_SE_QDFS		1u/*区段封锁*/
#define I_SE_QDJF		2u/*区段解封*/
#define I_SE_QDYFW	3u/*区段预复位*/
#define I_SE_LSXS		4u/*临时限速*/
#define I_SE_QDKX		5u/*区段空闲不能预复位*/
#define I_SE_QGJ		6u/*区故解*/
#define I_SE_TJBMZ		7u/*条件不满足*/
#define I_SE_CZCW		8u/*操作错误*/
#define I_SE_LCSB		9u/*非本场设备*/

/*逻辑区段提示信息*/
#define I_TS_SZLSXSCG   1u/*设置临时限速成功*/
#define I_TS_QXLSXSCG   2u/*取消临时限速成功*/
#define I_TS_TJBMZ		3u/*条件不满足*/

/*Cross提示信息*/
#define I_CR_SZLSXSCG   1u/*设置临时限速成功*/
#define I_CR_QXLSXSCG   2u/*取消临时限速成功*/
#define I_CR_TJBMZ       3u/*条件不满足*/

/*进路提示信息定义*/
#define I_RT_JLXP			1u/*进路选排*/
#define I_RT_JLYD			2u/*进路引导*/
#define I_RT_ZSBYD			3u/*总锁闭引导*/
#define I_RT_ZQX			4u/*进路总取消*/
#define I_RT_ZRJ			5u/*进路总人解*/
#define I_RT_WCTJL			6u/*无此条进路*/
#define I_RT_QXTJBMZ		7u/*侵限条件不满足*/
#define I_RT_ESBTJBMZ		8u/*紧停条件不满足*/
#define I_RT_PSDTJBMZ		9u/*屏蔽门条件不满足*/
#define I_RT_AFGTJBMZ		10u/*防淹门条件不满足*/
#define I_RT_ZCTJBMZ		11u/*照查条件不满足*/
#define I_RT_JLBWZ			12u/*进路不完整*/
#define I_RT_QDBZY			13u/*区段被占用*/
#define I_RT_JLDDSB			14u/*进路敌对锁闭*/
#define I_RT_QDBFS			15u/*区段被封锁*/
#define I_RT_XHBFS			16u/*信号被封锁*/
#define I_RT_DCBDS			17u/*道岔被单锁*/
#define I_RT_DCBFS			18u/*道岔被封锁*/
#define I_RT_CFTJBMZ		19u/*侧防条件不满足*/
#define I_RT_BHQDTJBMZ		20u/*保护区段条件不满足*/
#define I_RT_XHBNKF			21u/*信号不能开放*/
#define I_RT_XHBNBC			22u/*信号不能保持*/
#define I_RT_ATPJZJS		23u/*ATP禁止解锁*/
#define I_RT_TJBMZ			24u/*条件不满足*/
#define I_RT_XHKF			25u/*信号开放*/
#define I_RT_XHGZGB			26u/*信号故障关闭*/
#define I_RT_XHGB			27u/*信号关闭*/

/*站台提示信息定义*/
#define I_ST_SZKC			1u/*设置扣车*/
#define I_ST_QXKC			2u/*取消扣车*/
#define I_ST_LCSB		3u/*非本场设备*/

/*功能按钮提示信息定义*/
#define I_FB_SDJS			1u/*上电解锁*/
#define I_FB_SDJSCS			2u/*上电解锁超时*/
#define I_FB_QCJF			3u/*全场解封*/
#define I_FB_QZLSXSJC		4u/*全站临时限速解除*/
#define I_FB_SZWXDD			5u/*设置维修点灯*/
#define I_FB_QXWXDD			6u/*取消维修点灯*/
#define I_FB_SZATB			7u/*设置自动折返*/
#define I_FB_QXATB			8u/*取消自动折返*/
#define I_FB_MLBNZX			9u/*命令不能执行*/

#define I_FB_QQSC			10u/*请求试车*/
#define I_FB_KSSC			11u/*开始试车*/
#define I_FB_JSSC			12u/*结束试车*/
#define I_FB_TYSC			13u/*同意试车*/
#define I_FB_ZDSC			14u/*中断试车*/
#define I_FB_SZFJLDC       15u/*设置非进路调车*/
#define I_FB_FJLGZJS       16u/*非进路故障解锁*/

/*其他提示信息*/
#define I_OT_CZCW			1u/*操作错误*/

/*功能命令ID标识*/
#define A_SWIT_DCJC		1u/*道岔挤岔*/
#define A_SWIT_JCHF		2u/*挤岔回恢复*/
#define A_SIGN_DSDS		1u/*灯丝断丝*/
#define A_SIGN_DSHF		2u/*断丝恢复*/

#define I_FUNC_ID			0xFFu/*功能按钮命令提示，ID标识默认为0xFFu*/

#define QCTYPE2					0x77u	/**/
#define ZC_FLAG					0x01u	/*正常*/
#define HT_FLAG					0x02u	/*后退*/
#define GZ_FLAG					0x03u	/*故障*/

#ifndef NULL
#define NULL ((void*)0)
#endif

#define c_SignalTotalNum 100u
#define c_SwitchTotalNum 100u
#define c_SectionTotalNum 100u

#define FIRST_SYS_ID	0x01ul	/*I 系*/
#define SECOND_SYS_ID	0x02ul	/*II 系*/

#define TAG_MYMODE_AUTO  0x00ul //自动
#define TAG_MYMODE_MAN_I  0x01ul //人工I系
#define TAG_MYMODE_MAN_II  0x02ul //人工II系

#define MAXSYSHALTNUMB 3u   /*宕机容忍周期*/

#define FUN_NO_USRDATAWATCH 1u
#define FUN_NO_USRDATARECV 2u
#define FUN_NO_USRDATARECVEX 3u
#define FUN_NO_USRDATARECVMUTUAL 4u
#define FUN_NO_USRDATASEND 5u
#define FUN_NO_USRDATASENDEX 6u
#define FUN_NO_USRDATASENDMUTUAL 7u
#define FUN_NO_INQUIRYDEVSTATUS 8u
#define FUN_NO_INQUIRYSWVERSION 9u
#define FUN_NO_GETSYSTEMID 10u
#define FUN_NO_GETSWID 11u
#define FUN_NO_GETMASTERID 12u
/******************************************************************************
 *    Global Struct    Define Section*
 ******************************************************************************/

typedef unsigned char  MyUINT8;
typedef unsigned short MyUINT16;
typedef unsigned int   MyUINT32;
typedef signed int	   MyINT32;
typedef signed char    MyINT8;

typedef enum YLinkPropty
{
	YL_IsCurNot   = 0, /*本场非场联设备*/
	YL_IsCurLink  = 1, /*本场场联设备*/
	YL_IsNotCur   = 2,  /*非本场设备*/
	YL_IsNearNot  = 3,/*临场非场联设备*/
	YL_IsNearLink = 4, /*临场场联设备*/
	YL_IsNotNear  = 5  /*非临场设备*/

}YLinkPropty;

typedef enum MyBOOL
{
	e_UsrFalse = 0,
	e_UsrTrue = 1
}MyBOOL;

typedef enum E_DevType
{
	E_SECTION, E_SWITCH, 	E_SIGNAL, E_TRACKSECTION,	E_CROSSING,	E_FUNCBTN
}E_DevType;

typedef enum E_DrvState
{
	e_Fail = -1,
	e_Driving = 0,
	e_Success = 1,
	E_XHFS = 2,	/*信号封锁*/
	E_DSDS = 3,	/*灯丝断丝*/
	E_QDZY = 4,	/*区段占用*/
	E_QDFS = 5,	/*区段封锁*/
	E_CWSB = 6,	/*错误锁闭*/
	E_DCSB = 7,	/*道岔失表*/
	E_QXTJ = 8,	/*侵限条件*/
	E_CLTJ = 9, /*场联条件*/
	E_TYTJ = 10, /*同意条件*/
	E_ESBTJ = 11, /*ESB条件*/
	E_PSDTJ = 12, /*PSD条件*/
	E_HOLDTJ = 13, /*扣车条件*/
	E_AFDTJ = 14,/*防淹门条件*/
	E_DDXH = 15,/*敌对信号*/
	E_CFTJ = 16,/*侧防条件*/
	E_CZCW = 17,/*操作错误*/
	E_KYTJ = 18,/*跨压条件*/
	E_ATJJ = 19,/*AT接近*/
	E_BHQDTJ = 20, /*保护区段条件*/
	E_DYQDZY = 21,		/*第一区段占用*/
	e_JLDDSB = 22,		/*进路敌对锁闭*/
	e_JLDDZY = 23,		/*进路敌对征用*/
	e_JLBHDDZY = 24,	/*进路保护敌对征用*/
	e_JLBHDDSB = 25,	/*进路保护敌对锁闭*/
	e_JLJKQDZY = 26,	/*进路监控区段占用*/
	e_JLJKQDSB = 27,	/*进路监控区段锁闭*/
	e_JLFJKDDSB = 28,/*进路非监控区段敌对锁闭*/
	e_QDTSR = 29,/*区段临时限速*/
	e_ZDXH = 30/*阻挡信号*/
}E_DrvState;

typedef struct LockAddr
{
	MyUINT8 table;
	MyUINT8 offset;
}LockAddr;

typedef struct MutiSection
{
	MyUINT16 nSection_;
	char sectionName_[3][16];
	MyUINT16 sectionAddr_[3];
	MyUINT16 relayAddr_[3];
}MultiSect;

typedef struct RtAddrData
{
   	MyUINT16 StartBtn;/*进路始端按钮*/
	MyUINT16 alterBtn[2];/*变更按钮号*/
   	MyUINT16 EndBtn;/*进路终端按钮*/
   	MyUINT16 BlkSign;/*阻挡信号机编码*/
   	MyUINT16 RtProp; /*进路性质*/
   	MyUINT16 BlkAddr;/*阻挡信号机进路在进路地址表中的地址*/
   	LockAddr RtAddr;/*表序号+表偏移量*/
}RtAddrData;

typedef struct ApprOneDataEX
{
	MyUINT16 sectIndex;
	MyUINT16 devType; //检查设备类型
	MyUINT16 devIndex; //检查设备
	MyUINT16 devPos; //位置
}ApprOneDataEX;

typedef struct PSDCfg
{
	MyUINT16 psdRelayIndex[6];

}PSD_Cfg;

typedef struct AUTO_ZONE_Dev_CFG_DATA
{
	MyUINT16 devIndex;
	MyUINT16 isAutoZone;

}AutoZoneCfg;

typedef struct AUTO_ZONE_Dev_ST_DATA
{
	MyUINT16 devIndex;
	MyUINT8  isAutoZone;

}AutoZoneDevice;

typedef struct ApprSectionDataEX
{
	MyUINT16 signIndex;
	MyUINT16 time;
	MyUINT16 numApprSection;
	ApprOneDataEX apprDev[10];

}ApprSectionDataEX; //升级版接近区段表

typedef struct TurnTableAddr
{
	MyUINT16 WZJAddr;	    /*in，区段零散表示区的index*/
	MyUINT16 XHBSJOutAddr;/*Out，驱动板上的地址*/
	MyUINT16 ZBQC1JAddr;	/*Out，驱动板上的地址*/
}TurnTableAddr;

#define COM_RSSPI_PEER_CI 1u
#define COM_RSSPI_PEER_ZC 2u
#define COM_RSSPI_PEER_DMS 3u
#define COM_RSSPI_PEER_VOBC 4u
typedef struct RSSP_COM_CfgData
{
	MyUINT16    peerType; //通信方类型
	MyUINT16	VcpDevId;	//配置软件生成的配置ID
	MyUINT16	Peer_RsspID;//临站对应的协议ID
}RsspComCfgData;

typedef struct RSSP_COM_CFG
{
	MyUINT16	VcpDevId;	//配置软件生成的配置ID
	MyUINT16	Peer_RsspID;//临站对应的协议ID
}RSSP_COM_CFG;

typedef struct COMM_RSP_CFG
{
	MyUINT16    peerType; //通信方类型
	MyUINT16	Peer_RsspID; //通信方ID
	MyUINT16	VcpId_A;//A网Vcp
	MyUINT16	VcpId_B;//B网Vcp
}CommRspCfg;
/******************************************************************************
 *    Global Variable Declare Section
 ******************************************************************************/

/******************************************************************************
*    Global Prototype Declare Section
 ******************************************************************************/

/******************************************************************************
 *    Multi-Include-Prevent End Section
 ******************************************************************************/
#endif

