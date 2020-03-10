

#include "SS_socket.h"
#include "SS_socket_exception.h"


#include <iostream>


#pragma comment(lib, "ws2_32.lib")

using namespace std;

DWORD _tinet_addr(const TCHAR *cp)
{
	char IP[16];
	int Ret = 0;

	Ret = WideCharToMultiByte(CP_ACP, 0, cp, (int)_tcslen(cp), IP, 15, NULL, NULL);
	IP[Ret] = 0;
	return inet_addr(IP);
}

int ValidateAddress(SWstring sAddress)
{
	//SD_LOG_SCOPE_0;

	const int MinAddressLen = 7;
	const int MaxAddressLen = 15;

	int nSize = (int)sAddress.size();

	if (nSize >= MinAddressLen && nSize <= MaxAddressLen)
	{
		int nDigitLen = 0;
		int nDotCount = 0;
		SWstring sDigit = _T("");
		for (int pos = 0; pos < nSize; pos++)
		{
			TCHAR Ch = sAddress[pos];
			if (_T('0') <= Ch && Ch <= _T('9'))
			{
				sDigit = sDigit + SWstring(1, Ch);
				nDigitLen++;
				if (nDigitLen > 3)
				{
					//Logdebug("exceeded digit.");
					return false;
				}
				int nDigit = _tstoi((LPCTSTR)sDigit.c_str());
				if (nDigit < 0 || nDigit > 255)
				{
					//Logdebug("Digit is over flow");
					return false;
				}
			}
			else if (_T('.') == Ch)
			{
				int nDigit = _tstoi((LPCTSTR)sDigit.c_str());
				if (nDigit < 0 || nDigit > 255)
				{
					//Logdebug("Digit is over flow");
					return false;
				}

				sDigit = _T("");
				nDotCount++;
				if (nDotCount > 3)
				{
					//Logdebug("Too many dots.");
					return false;
				}
				nDigitLen = 0;
			}
			else
			{
				//Logdebug(_T("invalid address = %s"), sAddress.c_str());
				return false;
			}
		}
	}
	else
	{
		//Logdebug("invalid address : length.");
		return false;
	}

	return true;
}

int ValidateAddressA(string sAddress)
{
	//SD_LOG_SCOPE_0;

	const int MinAddressLen = 7;
	const int MaxAddressLen = 15;

	int nSize = (int)sAddress.size();

	if (nSize >= MinAddressLen && nSize <= MaxAddressLen)
	{
		int nDigitLen = 0;
		int nDotCount = 0;
		string sDigit = "";
		for (int pos = 0; pos < nSize; pos++)
		{
			char Ch = sAddress[pos];
			if ('0' <= Ch && Ch <= '9')
			{
				sDigit = sDigit + string(1, Ch);
				nDigitLen++;
				if (nDigitLen > 3)
				{
					//Logdebug("exceeded digit.");
					return false;
				}
				int nDigit = stoi(sDigit.c_str());
				if (nDigit < 0 || nDigit > 255)
				{
					//Logdebug("Digit is over flow");
					return false;
				}
			}
			else if ('.' == Ch)
			{
				int nDigit = stoi(sDigit.c_str());
				if (nDigit < 0 || nDigit > 255)
				{
					//Logdebug("Digit is over flow");
					return false;
				}

				sDigit = "";
				nDotCount++;
				if (nDotCount > 3)
				{
					//Logdebug("Too many dots.");
					return false;
				}
				nDigitLen = 0;
			}
			else
			{
				//Logdebug(_T("invalid address = %s"), sAddress.c_str());
				return false;
			}
		}
	}
	else
	{
		//Logdebug("invalid address : length.");
		return false;
	}

	return true;
}

CSocket::CSocket(SWstring sAddress, int nPort, bool bNetProtocol)
	: m_hSocket(NULL), m_nPort(nPort), m_bConnected(false), m_sAddress(sAddress)
{
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) 
	{
		throw ESocket(_T("error at socket()"), WSAGetLastError());
	};

	if (bNetProtocol == SS_TCP)
	{
		m_hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	}
	else if (bNetProtocol == SS_UDP) 
	{
		m_hSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	}

    if (m_hSocket == INVALID_SOCKET) 
	{
		throw ESocket(_T("error at socket()"), WSAGetLastError());
    }

	unsigned int rcvBuf = SET_SOCK_MAX_BUFF_SIZE;
	bool state;
	int len = sizeof(rcvBuf);

	if (bNetProtocol == SS_UDP)
	{
		state = setsockopt(m_hSocket, SOL_SOCKET, SO_RCVBUF, (char*)&rcvBuf, len);
		if (state == SOCKET_ERROR)
		{
			throw ESocket(_T("error at socket()"), WSAGetLastError());
		}
	}
    m_Address.sin_family = AF_INET;
    m_Address.sin_addr.s_addr = _tinet_addr(sAddress.c_str());
    m_Address.sin_port = htons(nPort);
}

CSocket::~CSocket()
{
	try
	{
		m_sAddress.clear();

		Close();
	}
	catch (...)
	{
		// do nothing...
	}
}


void CSocket::Close()
{

	const int nFINTimeout = 2;
	int nResult = 0;

	try
	{
		if (m_hSocket)
		{
			if (this->m_bConnected)
			{
				nResult = shutdown(m_hSocket, SD_BOTH);

				if (nResult == SOCKET_ERROR)
				{
					int nWSACode = WSAGetLastError();
					if (nWSACode)
					{
						//Logdebug(_T("shutdown WSACode = 0x%08X"), nWSACode);
					}
				}
				m_bConnected = false;
			}

			SOCKET hSocket = m_hSocket;
			m_hSocket = NULL;

			if (hSocket != 0)
			{
				closesocket(hSocket);
#ifdef _DEBUG
				//Logdebug("disconnected");
#endif
			}

			m_bConnected = false;
		}
	}
	catch (...)
	{
		//Logdebug("unknown exception");
		return;
	}

	m_bConnected = false;
}