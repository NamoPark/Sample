#include "SS_acquisition_base.h"
#include "../SS_CommonUtils/Misc.h"
#include "../SS_Logger/Logger.h"
#include "../SS_Dll.h"
#include "../SS_CommonFW/Cmd_List.h"
#include "../SS_CommonUtils/SS_FrameSaver.h"

static const int cMutexID = 0; // use mutex #0 here

static UINT WINAPI CtlReadThread(void *AParam)
{
	CSocket* ixCtl_Soc = (CSocket*)AParam;
	SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("CtlReadThread Generation Success"));
	CString strFirmVer;
	int iFirmVer = 0;
#ifdef EMULATION
	Sleep(200);
	ix_RecvInitDone = TRUE;
	Sleep(200);
	ix_RecvSetDone = TRUE;
	return 0;
#endif

	TCHAR  qStr[256];
	int    qLastErrCnt = 0;
	while (!VaU3_MutexLocked(cMutexID)) {
		tPacket qPacket;
		memset(qPacket.rData, 0, sizeof(qPacket.rData));
		int  qL = recv(ixCtl_Soc->GetSocket(), (char*)&qPacket, sizeof(qPacket), 0);
		if (qL <= 0)
		{
			break;
		}
		else if (WSAGetLastError() == 10053)
		{
		}
		else if (WSAGetLastError() == 10038)
		{
			break;
		}
		else if (qL == SOCKET_ERROR) {
			if (VaU3_MutexLocked(cMutexID)) break;
			if (!qLastErrCnt) {
			}
			if (qLastErrCnt++ > 100) {
				break;
			};
			Sleep(250);
		}
		else {
			qLastErrCnt = 0;
			unsigned int nPrefix = qPacket.rStatus & 0xffff0000;
			switch (nPrefix & 0xff000000) {
			case P_ACK:
			{
				switch (nPrefix & 0x00ff0000)
				{
				case P_CMD:
				{
					switch (qPacket.rStatus)
					{
					case ACK_INIT_DONE:
						TRACE(_T("\nACK_INIT_DONE\n"));
						break;
					case ACK_SET_FPGA_PARAM:
						//i_Feedback(ETTrace, EAck_EParam_FPGA, 1, _T("ACK_SET_FPGA_PARAM"), &qPacket);
						break;
					case ACK_SET_ROIC_PARAM:
						//i_Feedback(ETTrace, EAck_EParam_ROIC, 1, _T("ACK_SET_ROIC_PARAM"), &qPacket);
						break;
					case ACK_START:
						//i_Feedback(ETTrace, EAck_Start, 1, _T("ACK_START"), &qPacket);
						break;
					}
				}
				case P_AUX:
				{
					switch (qPacket.rStatus)
					{
					case ACK_AUX_FPGA_PARAM:
					{
						//i_Feedback(ETTrace, EAck_EParam_FPGA, 1, _T("ACK_AUX_FPGA_PARAM"), &qPacket);

						break;
					}
					case ACK_AUX_ROIC_PARAM:
					{
						//i_Feedback(ETTrace, EAck_EParam_ROIC, 1, _T("ACK_AUX_ROIC_PARAM"), &qPacket);

						break;
					}
					case ACK_AUX_VER_FIRM:
						TRACE(_T("\nACK_AUX_VER_FIRM\n"));
						//i_Feedback(ETAck, EAck_Ver_Firmware, 1, _T("s2 AUX_VER_FIRM"), &qPacket);
						break;
					case ACK_AUX_VER_FPGA:
						TRACE(_T("\nACK_AUX_VER_FPGA\n"));
						//i_Feedback(ETAck, EAck_Ver_FPGA, 2, _T("s2 AUX_VER_FPGA"), &qPacket);
						break;
					case ACK_ABORT:
					{
						//ssCaptureStop();
						TRACE(_T("\nAbort OK\n"));
						break;
					}

					default:
						_stprintf_s(qStr, _T("s2 unk msg %xh"), qPacket.rStatus);
					}; // switch qPacket.rStatus
				} // case P_AUX:
				default: {}
				} // switch (nPrefix & 0x00ff0000)
			} // case P_ACK:
			case P_MSG:
			{
				switch (qPacket.rStatus)
				{
				case MSG_WLAN_INFO:
				{
					break;
				}
				case MSG_INITIALIZING:
				{
					TRACE(_T("\nAED_INITIALINZE\n"), MSG_INITIALIZING);
					break;
				}
				case MSG_AT_READY:
				{
					TRACE(_T("\nAED_READY\n"), MSG_AT_READY);
					break;
				}
				case MSG_AXIS:
				{
					CString cstrTemp;
					TCHAR qMsg[256];
					memset(qMsg, 0, 256);
					cstrTemp.Format(_T(" --- X :%u Y: %u Z: %u"), qPacket.rData[0], qPacket.rData[1], qPacket.rData[2]);
					_tcscpy_s(qMsg, 256, cstrTemp);
					TRACE(cstrTemp);
					break;
				}
				default: {}
				} // switch (qPacket.rStatus)
			} // case P_MSG:
			default: {}
			}
			Sleep(1);
		}; // if recvfrom()<>SOCKET_ERROR
	}; // while not VaU3_MutexLocked
	return 0;
}

static UINT WINAPI DataListenThread(void *AParam)
{
	SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("DataListenThread Generation Success"));
	return 0;
}

static UINT WINAPI FileSaveThread(void *AParam)
{
	SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("FileSaveThread Generation Success"));
	while (!VaU3_MutexLocked(cMutexID))
	{

	}
	return 0;
}

CConnection::CConnection(SS_Detector* pDetector, SWstring sAddress,int NetProtocol):
	m_sAddress(sAddress),
	m_pDetector(pDetector),
	m_nNetProtocol(NetProtocol),
	m_pControlSocket(NULL),
	m_pDataSocket(NULL),
	m_nConnectionState(STATE_INITIAL),
	OnData(NULL),
	OnException(NULL),
	OnEvent(NULL),
	m_bConnected(false),
	m_bAborted(false),
	m_bDataFailed(false),
	ix_ThreadCtlRead(NULL),
	ix_ThreadDataListen(NULL),
	ix_ThreadFileSave(NULL)
{

	if (pDetector == NULL)
	{
		throw ENullDetector(_T("detector is null"));
	}
	// address validation
	if (ValidateAddress(sAddress) == false)
	{
		SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("invalid address string"));
		throw EInvalidParam(_T("invalid address string"));
	}
	m_sAddress = sAddress;

	SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("ip address is %s"), sAddress.c_str());

	try
	{
		m_pControlSocket = new CSocket(sAddress, SOCK_CTRL_PORT,SS_TCP);
	}
	catch (ESocket &e)
	{
		if (m_pControlSocket)
		{
			delete m_pControlSocket;
			m_pControlSocket = NULL;
		}
		SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("unable to create control socket : %s."), e.Message().c_str());
		throw EConnectionCreate(_T("unable to create control socket."), e.GetError());
	}

	try
	{
		if(NetProtocol == SS_UDP)
			m_pDataSocket = new CSocket(sAddress, SOCK_DATA_PORT, SS_UDP);
		else
			m_pDataSocket = new CSocket(sAddress, SOCK_DATA_PORT, SS_TCP);
	}
	catch (ESocket &e)
	{
		if (m_pDataSocket)
		{
			delete m_pDataSocket;
			m_pDataSocket = NULL;
		}
		SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("unable to create data socket : %s."), e.Message().c_str());
		throw EConnectionCreate(_T("unable to create data socket."), e.GetError());
	}
}

CConnection::~CConnection()
{
	try
	{
		CSmartCriticalSection oSocketCS(m_oSocketCS);

		if (m_pDataSocket)
		{
			delete m_pDataSocket;
			m_pDataSocket = NULL;
		}

		if (m_pControlSocket)
		{
			delete m_pControlSocket;
			m_pControlSocket = NULL;
		}

	}
	catch (...)
	{
		// do nothing
	}
}

void CConnection::SetDataHandler(SSAcq_DataHandler pfnHandler)
{
	OnData = pfnHandler;
}

void CConnection::SetExceptionHandler(SSAcq_ExceptionHandler pfnHandler)
{
	OnException = pfnHandler;
}

void CConnection::SetEventHandler(SSAcq_EventHandler pfnHandler)
{
	OnEvent = pfnHandler;
}

bool CConnection::Connect()
{
	m_bConnected = false;

	if (m_pControlSocket == NULL)
	{
		SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("control socket is null"));
		return false;
	}
	if (m_pDataSocket == NULL)
	{
		SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("data socket is null"));
		return false;
	}


	try
	{
		SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("control socket, try connect with %d sec"), SOCK_TIMEOUT);

		{
			CSmartCriticalSection oSocketCS(m_oSocketCS);
			m_pControlSocket->Connect(SOCK_TIMEOUT);
		}
		// LogDebug("control socket connected");
	}
	catch (EConnectionTimeout &e)
	{
		m_bDetectorTimeouted = true;
		SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("control connection timeouted (timeout = %d sec)"), e.GetTimeout());
		//ProcessException(SD_EXP_CONNECTION_TIMEOUT);
		return false;
	}
	catch (ESocket &e)
	{
		SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("control connection failed : %s."), e.Message().c_str());
		//ProcessException(SD_EXP_CONNECTION_FAILED);
		return false;
	}

	try
	{
		SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("data socket, try connect with %d sec"), SOCK_TIMEOUT);

		{
			CSmartCriticalSection oSocketCS(m_oSocketCS);
			m_pDataSocket->Connect(SOCK_TIMEOUT);
		}
		// LogDebug("data socket connected");
	}
	catch (EConnectionTimeout &e)
	{
		m_bDetectorTimeouted = true;
		SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("data connection timeouted (timeout = %d sec)."), e.GetTimeout());
		//ProcessException(SD_EXP_CONNECTION_TIMEOUT);
		return false;
	}
	catch (ESocket &e)
	{
		SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("data connection failed : %s."), e.Message().c_str());
		//ProcessException(SD_EXP_CONNECTION_FAILED);
		return false;
	}

	m_bConnected = true;
	return true;
}

bool CConnection::Disconnect()
{
	return false;
}

bool CConnection::CreateThread()
{
	UINT qThreadID;
	ix_ThreadCtlRead = (HANDLE)_beginthreadex(NULL, 0, CtlReadThread, (void *)m_pControlSocket, 0, &qThreadID);
	if (ix_ThreadCtlRead == false) 
	{
		SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("CtlReadThread Generation Failed"));
		return false;
	}

	ix_ThreadDataListen = (HANDLE)_beginthreadex(NULL, 0, DataListenThread, (void *)m_pDataSocket, 0, &qThreadID);
	if (ix_ThreadDataListen == false)
	{
		SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("DataListenThread Generation Failed"));
		return false;
	}

	ix_ThreadFileSave = (HANDLE)_beginthreadex(NULL, 0, FileSaveThread, NULL, 0, &qThreadID);
	if (ix_ThreadFileSave == false)
	{
		SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("FileSaveThread Generation Failed"));
		return false;
	}
	VaU3_MutexCreate(cMutexID);
	return true;
}

bool CConnection::CloseThread()
{
	VaU3_MutexLock(cMutexID); // locked mutex indicates that we are in 'closing' state
	VaU3_CloseThreadHandle(&ix_ThreadCtlRead, 3000); // close control thread
	VaU3_CloseThreadHandle(&ix_ThreadDataListen, 3000); // close TCP-thread
	VaU3_CloseThreadHandle(&ix_ThreadFileSave, 3000); // close TCP-thread

	return false;
}
