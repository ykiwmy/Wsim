//---------------------------------------------------------------------------

#ifndef SocketH
#define SocketH
//---------------------------------------------------------------------------
#include "IOSock.h"

#define IP_SELF		"127.0.0.1"//"192.168.1.60"//"192.168.137.1"
#define IP1_OPPO	"127.0.0.1"//"192.168.1.11"//"127.0.0.1"//"192.168.137.12"
#define IP2_OPPO    "192.168.1.52"
#define DevCI1             0
#define DevCI2             6
#define DevZc1             1
#define DevZc2             7
#define DevBtmA            2
#define DevBtmB            8
#define DevRedarA          3
#define DevRedarB          9
#define DevAtoA            4
#define DevAtoB            10
#define DevRemoteAtpA      5
#define DevRemoteAtpB      11
#define DevDmiA            12
#define DevDmiB            13
#define DevSdiA            14
#define DevSdiB            15
#define DevSdoA            16
#define DevSdoB            17
#define DevSfiA            18
#define DevSfiB            19
#define DevGdioA           20
#define DevGdioB           21

#define m_CIA1_PORT_SELF       6000//60000
#define m_CIA2_PORT_SELF       8000
#define m_ZcA1_PORT_SELF       7000
#define m_ZcA2_PORT_SELF       9000
#define m_BtmA_PORT_SELF       5001
#define m_BtmB_PORT_SELF       5002
#define m_RedarA_PORT_SELF     5101
#define m_RedarB_PORT_SELF     5102
#define m_AtoA_PORT_SELF       7101
#define m_AtoB_PORT_SELF       7102
#define m_RemoteAtpA_PORT_SELF 7201
#define m_RemoteAtpB_PORT_SELF 7202
#define m_DmiA_PORT_SELF       5201
#define m_DmiB_PORT_SELF       5202
#define m_SdiA_PORT_SELF       5301
#define m_SdiB_PORT_SELF       5302
#define m_SdoA_PORT_SELF       5401
#define m_SdoB_PORT_SELF       5402
#define m_SfiA_PORT_SELF       5501
#define m_SfiB_PORT_SELF       5502
#define m_GdioA_PORT_SELF      5601
#define m_GdioB_PORT_SELF      5602

#define m_CIA1_PORT_OPPO       6001//60000
#define m_CIA2_PORT_OPPO       8000
#define m_ZcA1_PORT_OPPO       7000
#define m_ZcA2_PORT_OPPO       9000
#define m_BtmA_PORT_OPPO       6001
#define m_BtmB_PORT_OPPO       6002
#define m_RedarA_PORT_OPPO     6101
#define m_RedarB_PORT_OPPO     6102
#define m_AtoA_PORT_OPPO       8101
#define m_AtoB_PORT_OPPO       8102
#define m_RemoteAtpA_PORT_OPPO 8201
#define m_RemoteAtpB_PORT_OPPO 8202
#define m_DmiA_PORT_OPPO       6201
#define m_DmiB_PORT_OPPO       6202
#define m_SdiA_PORT_OPPO       6301
#define m_SdiB_PORT_OPPO       6302
#define m_SdoA_PORT_OPPO       6401
#define m_SdoB_PORT_OPPO       6402
#define m_SfiA_PORT_OPPO       6501
#define m_SfiB_PORT_OPPO       6502
#define m_GdioA_PORT_OPPO      6601
#define m_GdioB_PORT_OPPO      6602


class SocketInterface
{
private:
	CIOSock *m_CIA1Sock;
	CIOSock *m_CIA2Sock;
	CIOSock *m_ZcA1Sock;
	CIOSock *m_ZcA2Sock;
	CIOSock *m_BtmASock;
	CIOSock *m_BtmBSock;
	CIOSock *m_RedarASock;
	CIOSock *m_RedarBSock;
	CIOSock *m_AtoASock;
	CIOSock *m_AtoBSock;
	CIOSock *m_RemoteAtpASock;
	CIOSock *m_RemoteAtpBSock;
	CIOSock *m_DmiASock;
	CIOSock *m_DmiBSock;

	CIOSock *m_SdiASock;
	CIOSock *m_SdiBSock;
	CIOSock *m_SdoASock;
	CIOSock *m_SdoBSock;
	CIOSock *m_SfiASock;
	CIOSock *m_SfiBSock;
	CIOSock *m_GdioASock;
	CIOSock *m_GdioBSock;

	SOCKADDR_IN m_addrSelf;
	SOCKADDR_IN m_addrOppo;
protected:

public:
	BOOL F_InitSock();
	BOOL F_InitSock(DWORD SelfIP,DWORD PeerIP,int SelfPort,int PeerPort);
	BOOL F_Send(UINT32 devId,BYTE *pbyData, MyUINT16 pusLen);
	BOOL F_Recv(UINT32 devId,BYTE *pbyData, MyUINT16 *pusLen);
	BOOL F_CloseConnect();
	SocketInterface();
	virtual ~SocketInterface();
};
#endif
