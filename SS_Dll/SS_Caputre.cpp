#include "stdafx.h"
#include "SS_Calibration.h"
#include "SS_Caputre.h"
#include "SS_Interface.h"
#include "SS_Dll.h"
#include "Misc.h"
#include "Common/Cmd_List.h"
#include "Log/Logger.h"


#define UDP_MAX_PSIZE 65000
//// Statics --
static BYTE *ix_FrameBufferCurr = NULL;
static BYTE *qFrameBufferMax = NULL;
static SOCKET ixCtl_Soc = INVALID_SOCKET,
ixTCP_Soc = INVALID_SOCKET;

static HANDLE ix_ThreadCtlRead = nullptr;
static HANDLE ix_ThreadDataListen = nullptr;
static HANDLE ix_ThreadFileSave = nullptr;
static tDLIB_CBProc  ix_CallBackProc = NULL;
static tDLIB_CBRec   ix_CallBackRec;
static const int cMutexID = 0; // use mutex #0 here

static SS_Caputre* pCD_Acq;

static UINT WINAPI CtlReadThread(void *AParam);
static UINT WINAPI DataListenThread(void *AParam);
static UINT WINAPI FileSaveThread(void *AParam);
static UINT WINAPI i_ConnectSockets(void *AParam);
static void i_CloseSockets(void);
static tDlgParam* tpDlgParam;

int bAbort_AckFlag = 0;
int abort_mutex = 0;

tDLIB_CBRec* AD_Connect(int AFlags, tDLIB_CBProc ACallBackProc,
	void* AUserCallBackProc, void* AUserParam, UINT16* AFrameBuffer)
{
	tpDlgParam = theApp.ssAppParam->GetAppParam();
	memset(&ix_CallBackRec, 0, sizeof(tDLIB_CBRec));
	ix_CallBackRec.rFlags = AFlags;
	ix_CallBackProc = ACallBackProc;
	ix_CallBackRec.rUserCallBackProc = AUserCallBackProc;
	ix_CallBackRec.rUserParam = AUserParam;
	ix_CallBackRec.rFrameWidth = tpDlgParam->ImgDim.rFrameWidth;
	ix_CallBackRec.rFrameHeight = tpDlgParam->ImgDim.rFrameHeight;
	int qNP = ix_CallBackRec.rFrameWidth*ix_CallBackRec.rFrameHeight;
	ix_CallBackRec.rFrameBuffer = AFrameBuffer;
	ix_FrameBufferCurr = (BYTE*)ix_CallBackRec.rFrameBuffer;
	//ix_StartSuccess = ix_RecvInitDone = ix_RecvSetDone = ix_RecvInfoInit = ix_RecvInfoEL = ix_RecvInfoEL2 = ix_RecvInfoEL3 = FALSE;
	VaU3_MutexCreate(cMutexID);


	// control read/write TCP socket (#1)
	TCHAR   qMsg[128];
	_stprintf_s(qMsg, _T("Connecting sockets at %s"), tpDlgParam->IP_Addr);
	i_Feedback(ETTrace, ECapture, 0, qMsg, NULL);
	ixCtl_Soc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ixCtl_Soc == INVALID_SOCKET) {
		SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("INVALID_SOCKET1"));
		i_Feedback(ETWSAErr, ECapture, 1, _T("s1 create"), NULL);
		VaU3_MutexDelete(cMutexID);
		return NULL;
	};

	// data TCP socket (#3)
	// if (AFrameBuffer) { // !! do not open data socket if don't need it
	if (TRUE) { // !! open the couple of sockets (even we don't use that because of firmware bug)
		ixTCP_Soc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (ixTCP_Soc == INVALID_SOCKET) {
			SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("INVALID_SOCKET2"));
			i_Feedback(ETWSAErr, ECapture, 3, _T("s3 create"), NULL);
			i_CloseSockets();
			VaU3_MutexDelete(cMutexID);
			return NULL;
		};
	};

	UINT qThreadID;
#ifndef EMULATION
	DWORD qSocketConnectErr = (DWORD)-1;
	HANDLE qThreadConnect = (HANDLE)_beginthreadex(NULL, 0, i_ConnectSockets, &qSocketConnectErr, 0, &qThreadID);
	if (WaitForSingleObject(qThreadConnect, 1000/*5000*/) == WAIT_TIMEOUT) {
		SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("Connect Socket Error"));
		i_CloseSockets();
		Sleep(50);
		TerminateThread(qThreadConnect, 3);
		qSocketConnectErr = (DWORD)-1;
	};
	GetExitCodeThread(qThreadConnect, (LPDWORD)&qThreadID);
	CloseHandle(qThreadConnect);

	if (qSocketConnectErr) {
		i_CloseSockets();
		if (qSocketConnectErr == (DWORD)-1) WSASetLastError(WSAETIMEDOUT);
		else WSASetLastError(qSocketConnectErr);
		switch (qThreadID) {
		case 1:
			i_Feedback(ETWSAErr, ECapture, 1, _T("s1,s2 connect"), NULL);
			break;
		case 2:
			i_Feedback(ETWSAErr, ECapture, 1, _T("s3 connect"), NULL);
			break;
		default:
			i_Feedback(ETErr, ECapture, 1, _T("Can't connect detectors' sockets"), NULL);
			break;
		};
		VaU3_MutexDelete(cMutexID);
		return NULL;
	};
#endif
	ix_ThreadCtlRead = (HANDLE)_beginthreadex(NULL, 0, CtlReadThread, NULL, 0, &qThreadID); 
	if (ixTCP_Soc != INVALID_SOCKET) {
		char message[30] = "SendStart";
		send(ixTCP_Soc, message, 30, 0);
		ix_ThreadDataListen = (HANDLE)_beginthreadex(NULL, 0, DataListenThread,NULL, 0, &qThreadID);
		SetThreadPriority(ix_ThreadDataListen, THREAD_PRIORITY_TIME_CRITICAL);
		ix_ThreadFileSave = (HANDLE)_beginthreadex(NULL, 0, FileSaveThread, NULL, 0, &qThreadID);

	}
	else 
	{
			ix_ThreadDataListen = NULL;
	}
	i_Feedback(ETTrace, ECapture, 0, _T("sockets creation OK"), NULL);
	return &ix_CallBackRec;
}

static UINT WINAPI CtlReadThread(void *AParam)
{
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
		int  qL = recv(ixCtl_Soc, (char*)&qPacket, sizeof(qPacket), 0);
		if (qL <= 0)
		{
			i_CloseSockets();
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
				i_Feedback(ETWSAErr, ECapture, 2, _T("s2 recv"), NULL);
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
						i_Feedback(ETTrace, EAck_EParam_FPGA, 1, _T("ACK_SET_FPGA_PARAM"), &qPacket);
						break;
					case ACK_SET_ROIC_PARAM:
						i_Feedback(ETTrace, EAck_EParam_ROIC, 1, _T("ACK_SET_ROIC_PARAM"), &qPacket);
						break;
					case ACK_START:
						i_Feedback(ETTrace, EAck_Start, 1, _T("ACK_START"), &qPacket);
						break;
					}
				}
				case P_AUX:
				{
					switch (qPacket.rStatus)
					{
					case ACK_AUX_FPGA_PARAM:
					{
						i_Feedback(ETTrace, EAck_EParam_FPGA, 1, _T("ACK_AUX_FPGA_PARAM"), &qPacket);

						break;
					}
					case ACK_AUX_ROIC_PARAM:
					{
						i_Feedback(ETTrace, EAck_EParam_ROIC, 1, _T("ACK_AUX_ROIC_PARAM"), &qPacket);

						break;
					}
					case ACK_AUX_VER_FIRM:
						TRACE(_T("\nACK_AUX_VER_FIRM\n"));
						i_Feedback(ETAck, EAck_Ver_Firmware, 1, _T("s2 AUX_VER_FIRM"), &qPacket);
						break;
					case ACK_AUX_VER_FPGA:
						TRACE(_T("\nACK_AUX_VER_FPGA\n"));
						i_Feedback(ETAck, EAck_Ver_FPGA, 2, _T("s2 AUX_VER_FPGA"), &qPacket);
						break;
					case ACK_ABORT:
					{
						bAbort_AckFlag = 1;
						ssCaptureStop();
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
#define MAX_BUFF_SIZE 3072*3072*2*227
#define MIN_BUFF_SIZE 0
static UINT WINAPI DataListenThread(void *AParam)
{
	// shlee
	//if (theApp.IsInitialized() == 0)
	//	return NULL;

	unsigned int rcvBuf = MAX_BUFF_SIZE;
	bool state;
	int len =sizeof(rcvBuf);

	state = setsockopt(ixTCP_Soc, SOL_SOCKET, SO_RCVBUF, (char*)&rcvBuf, len);
	
	if (state == SOCKET_ERROR)
		TRACE(_T("\nSocket Error \n"));

	state = getsockopt(ixTCP_Soc, SOL_SOCKET, SO_RCVBUF, (char*)&rcvBuf, &len);

	if (state == SOCKET_ERROR)
		TRACE(_T("\nSocket Error \n"));

	SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("getsockopt rcvBuf : %u len :%d  "), rcvBuf, len);

	int NP = tpDlgParam->ImgDim.rFrameHeight * tpDlgParam->ImgDim.rFrameWidth;
	(theApp.ssFrameSave)->setup(tpDlgParam->ImgDim.rFrameHeight, tpDlgParam->ImgDim.rFrameWidth, sizeof(UINT16), tpDlgParam->PathCal);
	BYTE qBuf[UDP_MAX_PSIZE];
	BYTE *FrameBufferCurr = (theApp.ssFrameSave)->getFrame();
	BYTE *FrameBufferMax = FrameBufferCurr + NP * sizeof(unsigned short);
	unsigned short RecvImageCount = 0;
	unsigned short RecvPacketCount= 0;

	while (!VaU3_MutexLocked(cMutexID))
	{
			int qL = 0;
			qL = recv(ixTCP_Soc, (char*)qBuf, UDP_MAX_PSIZE, 0);
			if (!(qL == SOCKET_ERROR))
			{

				unsigned short SendPacketCount = *((unsigned short*)(qBuf + 2));
				if (RecvPacketCount != SendPacketCount)
				{
					unsigned short SendImageCount = *((unsigned short*)(qBuf));
					if (RecvImageCount != SendImageCount) 
					{
						SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("Packet Cross Error\nRecvPacketCount : %d , SendPacketCount : %d , RecvImageCount : %d ,SendImageCount : %d "),RecvPacketCount, SendPacketCount, RecvImageCount, SendImageCount);
						FrameBufferCurr = (theApp.ssFrameSave)->getReturnFrame();
						RecvPacketCount = 0;
						RecvImageCount = SendImageCount;
					}
					FrameBufferCurr+=(UDP_MAX_PSIZE*(SendPacketCount - RecvPacketCount));
					SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("Packet Cross Error 22 \nRecvPacketCount : %d , SendPacketCount : %d , FrameBufferCurr : %x ,FrameBufferMax : %x "), RecvPacketCount, SendPacketCount, FrameBufferCurr, FrameBufferMax);
					RecvPacketCount = SendPacketCount;
				}
				memcpy(FrameBufferCurr, qBuf, qL);
				FrameBufferCurr += qL;
				RecvPacketCount++;
			}
			if (FrameBufferCurr == FrameBufferMax)
			{
				(theApp.ssFrameSave)->addFrame();
				FrameBufferCurr = (theApp.ssFrameSave)->getFrame();
				FrameBufferMax = FrameBufferCurr + NP * sizeof(unsigned short);
				RecvPacketCount = 0;
				RecvImageCount++;
			}
	}
	return 0;
}

static UINT WINAPI FileSaveThread(void *AParam)
{
	while (!VaU3_MutexLocked(cMutexID))
	{
		
		while(!((theApp.ssFrameSave)->isEmpty()))
		{
			(theApp.ssFrameSave)->saveToDisk(FRONT);
			(theApp.ssFrameSave)->pop(FRONT);
		}
	}
	return 0;
}

BOOL AD_Init(tAcqPar* argAcqPar)
{
	tPacket tPkt;
	memset(&tPkt, 0, sizeof(tPacket));
	long DetType = tpDlgParam->DetModel;
	switch (DetType)
	{
	case (long)DetModel::DM_SA132_Solution:
		tPkt.rStatus = CMD_132_INIT;
		break;
	case (long)DetModel::DM_SA130_Solution:
		tPkt.rStatus = CMD_130_INIT;
		break;
	default:
		break;
	}
	memcpy_s(&tPkt.rData, sizeof(tAcqPar), argAcqPar, sizeof(tAcqPar));
	return ACQ_SendPacket(&tPkt);
}

bool ACQ_SendPacket(tPacket* Pkt)
{
	//	return NULL;
	SS_LOG((*theApp.pSSLogger), LogLevel::Debug, _T("\n CMD %d %d \n"), Pkt->rStatus, Pkt->rData[0]);
	TCHAR       qMsg[256];
#ifndef EMULATION
	if (send(ixCtl_Soc, (char*)Pkt, sizeof(tPacket), 0) == SOCKET_ERROR) {
		_stprintf_s(qMsg, _T("s1 can't send packet %xh"), Pkt->rStatus);
		i_Feedback(ETWSAErr, ECapture, 1, qMsg, NULL);
		return FALSE;
	};
#endif
	_stprintf_s(qMsg, _T("s1 sent packet with command %xh"), Pkt->rStatus);
	i_Feedback(ETTrace, ECapture, 1, qMsg, NULL);
	return TRUE;
}

// feedback dispatcher ///
void i_Feedback(int AType, int AEvent, int ASocket, const TCHAR *AMsg, void *APacketData)
{
	if (abort_mutex == 1)
	{
		TRACE(_T("\nReturn\n"));
		return;
	}
	//TRACE(_T("\nAType %d AEvent %x %s \n"), AType, AEvent, AMsg);
	//if (theApp.IsInitialized() == 0)
	//	return;
	// retun ASAP when in closing state

	//TRACE(_T("\nIn Feedback\n"));
	int qGotMutex;
	if (abs(AEvent) == EClose) {
		qGotMutex = FALSE;
	}
	else {
		if (VaU3_MutexLocked(cMutexID)) {
			return;
		}
		qGotMutex = VaU3_MutexGet(cMutexID);
		if (qGotMutex == 2)
		{
			return;
		}
	};
	ix_CallBackRec.rMsg[0] = 0;

	// trace message; never invoke modal MessageBox before callback
	switch (AType)
	{
	case ETTrace:
	{
		////TRACE(_T("\nFeedBack Section 3 \n"));
		if (AEvent != 9 && AEvent != 13)
		{
			if (AMsg) _tcscpy_s(ix_CallBackRec.rMsg, AMsg);
			//VaU_TraceMsg(ix_CallBackRec.rMsg);
		}
		break;
	}
	}; // switch
	   // callback
	if (ix_CallBackProc)
	{
		ix_CallBackRec.rType = AType;
		ix_CallBackRec.rEvent = AEvent;
		ix_CallBackRec.rSocket = ASocket;
		ix_CallBackRec.rPacketData = APacketData;
		ix_CallBackProc(&ix_CallBackRec);

	}; // if ix_CallBackProc
	   // invoke modal MessageBox after callback if required
	switch (AType) {
	case ETErr:
		ix_CallBackRec.rAborted = -1;
		break;
	};
	if (qGotMutex) {
		VaU3_MutexRelease(cMutexID);
	}
	//TRACE(_T("\nEnd Feed Back\n"));
}

void ACQ_Close()
{
	VaU3_MutexLock(cMutexID); // locked mutex indicates that we are in 'closing' state
	i_CloseSockets();
	VaU3_CloseThreadHandle(&ix_ThreadCtlRead, 1000); // close control thread
	VaU3_CloseThreadHandle(&ix_ThreadDataListen, 100000); // close TCP-thread
	TRACE(_T("\nThreadDataListen Close \n"));
	abort_mutex = 0;
	VaU3_MutexDelete(cMutexID);
}

BOOL ACQ_Abort()
{
	abort_mutex = 1;
	ix_CallBackRec.rAborted = 1;
	tPacket qPacket;
	long DetType = tpDlgParam->DetModel;
	switch (DetType)
	{
	case (long)DetModel::DM_SA132_Solution:
		qPacket.rStatus = CMD_132_ABORT;
		break;
	case (long)DetModel::DM_SA130_Solution:
		qPacket.rStatus = CMD_130_ABORT;
		break;
	default:
		break;
	}
	//qPacket.rStatus = CMD_ABORT;
	if (send(ixCtl_Soc, (char*)&qPacket, sizeof(qPacket), 0) == SOCKET_ERROR) {
		i_Feedback(ETErr, ECapture, 1, _T("s1 send cmd_abort"), NULL);
		ssCaptureStop();
		return FALSE;
	};

	int Cnt_ms = 0;
	while (bAbort_AckFlag == 0) {
		SleepMsg(100);
		Cnt_ms += 100;
		if (Cnt_ms >= 5000)
		{
			ssCaptureStop();
			break;
		}
	}
	return TRUE;
}

static UINT WINAPI i_ConnectSockets(void *AParam)
{
	//if (theApp.IsInitialized() == 0)
	//	return NULL;

	DWORD       *qErrCodeParam = (DWORD*)AParam,
		qErrCode;
	sockaddr_in  qSockAdr;

	memset(qSockAdr.sin_zero, 0, 8);
	qSockAdr.sin_addr.s_addr = VAU3_InetAddrTCHAR(tpDlgParam->IP_Addr);
	qSockAdr.sin_port = htons(20000);
	qSockAdr.sin_family = AF_INET;

	if (connect(ixCtl_Soc, (SOCKADDR*)&qSockAdr, sizeof(qSockAdr))) {
		qErrCode = WSAGetLastError();
		if (qErrCode) *qErrCodeParam = qErrCode;
		//SS_LOG((*theApp.ptr_DLib_logger), LogLevel::Error, _T(" --- Network Ctrl Connection Fail"));
		return 1;
	};

	if (ixTCP_Soc != INVALID_SOCKET) {
		memset(qSockAdr.sin_zero, 0, 8);
		qSockAdr.sin_addr.s_addr = VAU3_InetAddrTCHAR(tpDlgParam->IP_Addr);
		qSockAdr.sin_port = htons(20001);
		qSockAdr.sin_family = AF_INET;
		if (connect(ixTCP_Soc, (SOCKADDR*)&qSockAdr, sizeof(qSockAdr))) {
			qErrCode = WSAGetLastError();
			if (qErrCode) *qErrCodeParam = qErrCode;
			TRACE(_T("\nUDP Connect Error\n"));
			//SS_LOG((*theApp.ptr_DLib_logger), LogLevel::Error, _T(" --- Network TCP Connection Fail"));
			return 2;
		}
		else
		{
			TRACE(_T("\nUDP Connect Success\n"));
		}
	};
	*qErrCodeParam = 0;
	return 0;
}

static void i_CloseSockets(void)
{
	//if (theApp.IsInitialized() == 0)
	//	return;
	if (ixCtl_Soc != INVALID_SOCKET) {
		closesocket(ixCtl_Soc);
		ixCtl_Soc = INVALID_SOCKET;
	};
	if (ixTCP_Soc != INVALID_SOCKET) {
		closesocket(ixTCP_Soc);
		ixTCP_Soc = INVALID_SOCKET;
	};
}

SS_Caputre::SS_Caputre()
{
	pCD_Acq = this;
}

SS_Caputre::~SS_Caputre()
{
}

