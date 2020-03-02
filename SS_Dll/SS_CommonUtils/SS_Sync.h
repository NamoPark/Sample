#pragma once

#include "../stdafx.h"
#include "SS_StringDefines.h"

class CCriticalSection
{
	CRITICAL_SECTION m_oCS;
	SSstring m_sName;
	CCriticalSection(const CCriticalSection &o) = delete;
	CCriticalSection& operator = (const CCriticalSection &o) = delete;
public:
	CCriticalSection(SSstring name = _T("critical section"));
	virtual ~CCriticalSection();
	
	void Enter();
	void Leave();
};


class CSmartCriticalSection
{
	CCriticalSection &m_oCS;
	bool m_bLog;
	CSmartCriticalSection(const CSmartCriticalSection &o) = delete;
	CSmartCriticalSection& operator = (const CSmartCriticalSection &o) = delete;
public:
	CSmartCriticalSection(CCriticalSection &oCS);
	virtual ~CSmartCriticalSection();
};





