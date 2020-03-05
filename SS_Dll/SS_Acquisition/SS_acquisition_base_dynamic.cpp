#include "SS_acquisition_base_dynamic.h"
#include "../SS_Logger/Logger.h"
#include "../SS_Dll.h"

Dynamic_CConnection::Dynamic_CConnection(SWstring sAddress):
	m_sAddress(sAddress),
	m_pControlSocket(NULL),
	m_pDataSocket(NULL),
	m_nConnectionState(STATE_INITIAL),
	m_bConnected(false),
	m_bAborted(false),
	m_bDataFailed(false)
{
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
		m_pDataSocket = new CSocket(sAddress, SOCK_DATA_PORT, SS_UDP);
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

Dynamic_CConnection::~Dynamic_CConnection()
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

//bool CConnection::Connect()
//{
//	return false;
//}
//
//bool CConnection::Disconnect()
//{
//	return false;
//}
