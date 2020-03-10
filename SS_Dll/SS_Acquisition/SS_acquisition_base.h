#pragma once
#include "SS_socket_exception.h"
#include "SS_socket.h"
#include "../SS_Interface.h"
#include "../SS_CommonUtils/SS_Sync.h"
#include "../SS_CommonUtils/SS_data.h"

typedef struct _tPacket {
	INT32 rStatus;
	UINT16 rData[1024];
} tPacket;

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

#define SOCK_TIMEOUT	1

#define UDP_MAX_PSIZE 65000

class CConnection
{
private:
	int m_nConnectionState;

protected:
	SS_Detector* m_pDetector;
	CSocket *m_pControlSocket;
	CSocket *m_pDataSocket;

	SWstring m_sAddress;
	CCriticalSection m_oSocketCS;
	int m_nNetProtocol;
	bool m_bConnected;
	bool m_bAborted;
	bool m_bDetectorTimeouted;
	bool m_bDataFailed;

	HANDLE ix_ThreadCtlRead;
	HANDLE ix_ThreadDataListen;
	HANDLE ix_ThreadFileSave;
public:
	CConnection(SS_Detector* pDetector, SWstring sAddress, int NetProtocol);
	virtual ~CConnection();

	SSAcq_DataHandler OnData;
	SSAcq_ExceptionHandler OnException;
	SSAcq_EventHandler OnEvent;
	void SetDataHandler(SSAcq_DataHandler pfnHandler);	
	void SetExceptionHandler(SSAcq_ExceptionHandler pfnHandler);
	void SetEventHandler(SSAcq_EventHandler pfnHandler);


	bool Connect();
	bool Disconnect();
	bool CreateThread();
	bool CloseThread();


	//virtual bool Abort() = 0;
};

