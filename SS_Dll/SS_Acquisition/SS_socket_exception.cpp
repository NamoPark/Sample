
#include "SS_socket_exception.h"
#include "SS_socket.h"


const SWstring WSACodeToString(const int nErrCode)
{
	SWstring sMsg = _T("");
	switch (nErrCode)
	{
	case NO_ERROR:
		sMsg = _T("NO_ERROR");
		break;
	case	WSA_INVALID_HANDLE:
		sMsg = _T("WSA_INVALID_HANDLE Specified event object handle is invalid.");
		break;
	case	WSA_NOT_ENOUGH_MEMORY:
		sMsg = _T("WSA_NOT_ENOUGH_MEMORY Insufficient memory available.");
		break;
	case	WSA_INVALID_PARAMETER:
		sMsg = _T("WSA_INVALID_PARAMETER One or more parameters are invalid.");
		break;
	case	WSA_OPERATION_ABORTED:
		sMsg = _T("WSA_OPERATION_ABORTED Overlapped operation aborted.");
		break;
	case	WSA_IO_INCOMPLETE:
		sMsg = _T("WSA_IO_INCOMPLETE Overlapped I/O event object not in signaled state.");
		break;
	case	WSA_IO_PENDING:
		sMsg = _T("WSA_IO_PENDING Overlapped operations will complete later.");
		break;
	case	WSAEINTR:
		sMsg = _T("WSAEINTR Interrupted function call.");
		break;
	case	WSAEACCES:
		sMsg = _T("WSAEACCES Permission denied.");
		break;
	case	WSAEFAULT:
		sMsg = _T("WSAEFAULT Bad address.");
		break;
	case	WSAEINVAL:
		sMsg = _T("WSAEINVAL Invalid argument.");
		break;
	case	WSAEMFILE:
		sMsg = _T("WSAEMFILE Too many open files.");
		break;
	case	WSAEWOULDBLOCK:
		sMsg = _T("WSAEWOULDBLOCK Resource temporarily unavailable.");
		break;
	case	WSAEINPROGRESS:
		sMsg = _T("WSAEINPROGRESS Operation now in progress.");
		break;
	case	WSAEALREADY:
		sMsg = _T("WSAEALREADY Operation already in progress.");
		break;
	case	WSAENOTSOCK:
		sMsg = _T("WSAENOTSOCK Socket operation on nonsocket.");
		break;
	case	WSAEDESTADDRREQ:
		sMsg = _T("WSAEDESTADDRREQ Destination address required.");
		break;
	case	WSAEMSGSIZE:
		sMsg = _T("WSAEMSGSIZE Message too long.");
		break;
	case	WSAEPROTOTYPE:
		sMsg = _T("WSAEPROTOTYPE Protocol wrong type for socket.");
		break;
	case	WSAENOPROTOOPT:
		sMsg = _T("WSAENOPROTOOPT Bad protocol option.");
		break;
	case	WSAEPROTONOSUPPORT:
		sMsg = _T("WSAEPROTONOSUPPORT Protocol not supported.");
		break;
	case	WSAESOCKTNOSUPPORT:
		sMsg = _T("WSAESOCKTNOSUPPORT Socket type not supported.");
		break;
	case	WSAEOPNOTSUPP:
		sMsg = _T("WSAEOPNOTSUPP Operation not supported.");
		break;
	case	WSAEPFNOSUPPORT:
		sMsg = _T("WSAEPFNOSUPPORT Protocol family not supported.");
		break;
	case	WSAEAFNOSUPPORT:
		sMsg = _T("WSAEAFNOSUPPORT Address family not supported by protocol family.");
		break;
	case	WSAEADDRINUSE:
		sMsg = _T("WSAEADDRINUSE Address already in use.");
		break;
	case	WSAEADDRNOTAVAIL:
		sMsg = _T("WSAEADDRNOTAVAIL Cannot assign requested address.");
		break;
	case	WSAENETDOWN:
		sMsg = _T("WSAENETDOWN Network is down.");
		break;
	case	WSAENETUNREACH:
		sMsg = _T("WSAENETUNREACH Network is unreachable.");
		break;
	case	WSAENETRESET:
		sMsg = _T("WSAENETRESET	Network dropped connection on reset.");
		break;
	case	WSAECONNABORTED:
		sMsg = _T("WSAECONNABORTED Software caused connection abort.");
		break;
	case	WSAECONNRESET:
		sMsg = _T("WSAECONNRESET Connection reset by peer.");
		break;
	case	WSAENOBUFS:
		sMsg = _T("WSAENOBUFS No buffer space available.");
		break;
	case	WSAEISCONN:
		sMsg = _T("WSAEISCONN Socket is already connected.");
		break;
	case	WSAENOTCONN:
		sMsg = _T("WSAENOTCONN Socket is not connected.");
		break;
	case	WSAESHUTDOWN:
		sMsg = _T("already shutdown");
		break;
	case	WSAETOOMANYREFS:
		sMsg = _T("WSAETOOMANYREFS Too many references.");
		break;
	case	WSAETIMEDOUT:
		sMsg = _T("WSAETIMEDOUT Connection timed out.");
		break;
	case	WSAECONNREFUSED:
		sMsg = _T("WSAECONNREFUSED Connection refused.");
		break;
	case	WSAELOOP:
		sMsg = _T("WSAELOOP	Cannot translate name.");
		break;
	case	WSAENAMETOOLONG:
		sMsg = _T("WSAENAMETOOLONG Name too long.");
		break;
	case	WSAEHOSTDOWN:
		sMsg = _T("WSAEHOSTDOWN	Host is down.");
		break;
	case	WSAEHOSTUNREACH:
		sMsg = _T("WSAEHOSTUNREACH No route to host.");
		break;
	case	WSAENOTEMPTY:
		sMsg = _T("WSAENOTEMPTY Directory not empty.");
		break;
	case	WSAEPROCLIM:
		sMsg = _T("WSAEPROCLIM Too many processes.");
		break;
	case	WSAEUSERS:
		sMsg = _T("WSAEUSERS User quota exceeded.");
		break;
	case	WSAEDQUOT:
		sMsg = _T("WSAEDQUOT Disk quota exceeded.");
		break;
	case	WSAESTALE:
		sMsg = _T("	WSAESTALE Stale file handle reference.");
		break;
	case	WSAEREMOTE:
		sMsg = _T("	WSAEREMOTE Item is remote.");
		break;
	case	WSASYSNOTREADY:
		sMsg = _T("	WSASYSNOTREADY Network subsystem is unavailable.");
		break;
	case	WSAVERNOTSUPPORTED:
		sMsg = _T("	WSAVERNOTSUPPORTED Winsock.dll version out of range.");
		break;
	case	WSANOTINITIALISED:
		sMsg = _T("	WSANOTINITIALISED Successful WSAStartup not yet performed.");
		break;
	case	WSAEDISCON:
		sMsg = _T("	WSAEDISCON Graceful shutdown in progress.");
		break;
	case	WSAENOMORE:
		sMsg = _T("	WSAENOMORE No more results.");
		break;
	case	WSAECANCELLED:
		sMsg = _T("	WSAECANCELLED Call has been canceled.");
		break;
	case	WSAEINVALIDPROCTABLE:
		sMsg = _T("	WSAEINVALIDPROCTABLE Procedure call table is invalid.");
		break;
	case	WSAEINVALIDPROVIDER:
		sMsg = _T("	WSAEINVALIDPROVIDER Service provider is invalid.");
		break;
	case	WSAEPROVIDERFAILEDINIT:
		sMsg = _T("	WSAEPROVIDERFAILEDINIT Service provider failed to initialize.");
		break;
	case	WSASYSCALLFAILURE:
		sMsg = _T("	WSASYSCALLFAILURE System call failure.");
		break;
	case	WSASERVICE_NOT_FOUND:
		sMsg = _T("	WSASERVICE_NOT_FOUND Service not found.");
		break;
	case	WSATYPE_NOT_FOUND:
		sMsg = _T("	WSATYPE_NOT_FOUND Class type not found.");
		break;
	case	WSA_E_NO_MORE:
		sMsg = _T("	WSA_E_NO_MORE No more results.");
		break;
	case	WSA_E_CANCELLED:
		sMsg = _T("	WSA_E_CANCELLED Call was canceled.");
		break;
	case	WSAEREFUSED:
		sMsg = _T("	WSAEREFUSED Database query was refused.");
		break;
	case	WSAHOST_NOT_FOUND:
		sMsg = _T("	WSAHOST_NOT_FOUND Host not found.");
		break;
	case	WSATRY_AGAIN:
		sMsg = _T("	WSATRY_AGAIN Nonauthoritative host not found.");
		break;
	case	WSANO_RECOVERY:
		sMsg = _T("	WSANO_RECOVERY This is a nonrecoverable error.");
		break;
	case	WSANO_DATA:
		sMsg = _T("	WSANO_DATA Valid name, no data record of requested type.");
		break;
	default:
	{
		TCHAR pMsg[128];
		SS_sprintf_s(pMsg, 128, _T("unknown message: 0x%08X(%d)"), nErrCode, nErrCode);
		sMsg = SWstring(pMsg);
	}
	}

	return sMsg;
}


const SWstring ESocket::Message() const
{
	const int nMessageSize = 1024;
	TCHAR pMsg[nMessageSize];
	SS_sprintf_s(pMsg, nMessageSize, _T("ESocket %s : 0x%X(%d), %s"), m_sMsg.c_str(), m_nError, m_nError, WSACodeToString(m_nError).c_str());

	return pMsg;
}

const SWstring ESocketClosed::Message() const
{
	const int nMessageSize = 1024;
	TCHAR pMsg[nMessageSize];

	SS_sprintf_s(pMsg, nMessageSize, _T("ESocketClosed %s code = %d, %s"),
		m_sMsg.c_str(),
		GetError(),
		WSACodeToString(GetError()).c_str());

	return pMsg;
}

const SWstring EConnectionCreate::Message() const
{
	const int nMessageSize = 1024;
	TCHAR pMsg[nMessageSize];

	SS_sprintf_s(pMsg, nMessageSize, _T("EConnectionCreate %s code = %d, %s"),
		m_sMsg.c_str(),
		GetError(),
		WSACodeToString(GetError()).c_str());

	return pMsg;
}

const SWstring EConnectionTimeout::Message() const
{
	const int nMessageSize = 1024;
	TCHAR pMsg[nMessageSize];

	SS_sprintf_s(pMsg, nMessageSize, _T("EConnectionTimeout %s timeout = %d, code = %d, %s"),
		m_sMsg.c_str(),
		m_nTimeout, GetError(),
		WSACodeToString(GetError()).c_str());

	return pMsg;
}
