//---------------------------------------------------------------------------

#ifndef IOSockH
#define IOSockH
//---------------------------------------------------------------------------
//#include "Classes.hpp"
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <MSWSock.h>


typedef unsigned char  MyUINT8;
typedef unsigned short MyUINT16;
typedef unsigned int   MyUINT32;
typedef signed int	   MyINT32;
typedef signed char    MyINT8;
class CIOSock
{
public:
	CIOSock(SOCKADDR_IN addrSelf, SOCKADDR_IN addrOppo);
	BOOL F_InitSock();
	BOOL F_GetData(BYTE *pbyData, MyUINT16 *pusLen);
	BOOL F_SendData(BYTE *pbyData, MyUINT16 usLen);
	void CloseConnect();


private:
	SOCKET m_Socket;
	SOCKADDR_IN m_addrSelf;
	SOCKADDR_IN m_addrOppo;		//对方地址信息

};
#endif
