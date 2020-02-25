
#include "SS_sync.h"
#include "SS_Dll.h"
#include "Log/Logger.h"

CCriticalSection::CCriticalSection(SSstring name)
	: m_sName(name)
{
	SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("%s, CS is initialized."), m_sName.c_str());
	InitializeCriticalSection(&m_oCS);
}

CCriticalSection::~CCriticalSection()
{
	DeleteCriticalSection(&m_oCS);
	SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("%s, CS is deleted."), m_sName.c_str());
}

void CCriticalSection::Enter()
{
	EnterCriticalSection(&m_oCS);
	//SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("(enter CS) : %s"), m_sName.c_str());
}

void CCriticalSection::Leave()
{
	//SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("(leave CS) : %s"), m_sName.c_str());
	LeaveCriticalSection(&m_oCS);
}




CSmartCriticalSection::CSmartCriticalSection(CCriticalSection &oCS) : 
	m_oCS(oCS)
{
	m_oCS.Enter();
}

CSmartCriticalSection::~CSmartCriticalSection()
{
	m_oCS.Leave();
}