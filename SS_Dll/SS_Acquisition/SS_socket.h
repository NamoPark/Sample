#pragma once

#include "../SS_Interface.h"
#include "../SS_CommonUtils/SS_StringDefines.h"

#include <string>
using namespace std;

DWORD _tinet_addr(const TCHAR *cp);

#define CONNECT_INFINITE	0
#define SET_SOCK_MAX_BUFF_SIZE		0xFFFFFFFF
// for client socket
class CSocket 
{
	SOCKET m_hSocket;
	WSAData wsaData;
	SWstring m_sAddress;
	int  m_nPort;
	struct sockaddr_in m_Address; 
	int m_bConnected;
	//CSocket(const CSocket &o) = delete;
	//CSocket& operator = (const CSocket &o) = delete;
public:
	CSocket(SWstring sAddress, int nPort, bool bNetProtocol);
	virtual ~CSocket();
	SWstring &Address() { return m_sAddress; }
	void Close();
};

inline void SafeDeleteSocket(CSocket **ppSocket)
{
	if (ppSocket)
	{
		CSocket *pSocket = (CSocket *)*ppSocket;
		if (pSocket)
		{
			try
			{
				pSocket->Close();
			}
			catch (...)
			{}

			delete pSocket;
			*ppSocket = NULL;
		}
	}
}

int ValidateAddress(SWstring sAddress);
int ValidateAddressA(string sAddress);



