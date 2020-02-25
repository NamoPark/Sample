#pragma once

#include "SS_Interface.h"
#include "SS_defines.h"

#include <string>
using namespace std;

DWORD _tinet_addr(const TCHAR *cp);

#define CONNECT_INFINITE	0

// for client socket
class CSocket 
{
	SOCKET m_hSocket;
	SSstring m_sAddress;
	int  m_nPort;
	struct sockaddr_in m_Address; 
	int m_bConnected;
	CSocket(const CSocket &o) = delete;
	CSocket& operator = (const CSocket &o) = delete;
public:
	CSocket(SSstring sAddress, int nPort, bool bTCPNoDelay = true);
	virtual ~CSocket();
	SSstring &Address() { return m_sAddress; }
	void Close();
};


void EnsureWinsock();

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

int ValidateAddress(SSstring sAddress);
int ValidateAddressA(string sAddress);



