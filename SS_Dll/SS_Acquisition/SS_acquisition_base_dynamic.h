#pragma once
#include "SS_socket_exception.h"
#include "SS_socket.h"
#include "../SS_CommonUtils/SS_Sync.h"
// states
#define STATE_INITIAL			0x0000
#define STATE_CONNECTED			0x0001
#define STATE_WAIT_HSW			STATE_CONNECTED
#define STATE_MODE_SET			0x0002
#define STATE_HSW_PRESSED		0x0003
#define STATE_STARTED			0x0004
#define STATE_WINDOW_OPENED		0x0005
#define STATE_READOUT			0x0006
#define STATE_DATA_TRANSFERED	0x0007
#define STATE_DISCONNECTED		0x0008

#define SOCK_CTRL_PORT	20000
#define SOCK_DATA_PORT	20001

class Dynamic_CConnection
{
private:
	int m_nConnectionState;

protected:
	CSocket *m_pControlSocket;
	CSocket *m_pDataSocket;

	SWstring m_sAddress;
	CCriticalSection m_oSocketCS;
	bool m_bConnected;
	bool m_bAborted;
	bool m_bDataFailed;

public:
	Dynamic_CConnection(SWstring sAddress);
	virtual ~Dynamic_CConnection();

	//bool Connect();
	//bool Disconnect();

	//virtual bool Abort() = 0;
};

