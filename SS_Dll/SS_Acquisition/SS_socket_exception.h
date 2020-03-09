#pragma once
#include "../SS_CommonUtils/SS_exception.h"

const SWstring WSACodeToString(const int nErrCode);


class ESocket : public Exception
{
protected:
	const int m_nError; // by int WSAGetLastError()
public:
	ESocket(const SWstring sMsg, const int nError) : 
		Exception(sMsg), 
		m_nError(nError) {}

	virtual const int GetError(void) const
	{
		return m_nError;
	}

	virtual const SWstring Message() const;
};

class ESocketClosed : public ESocket
{
public:
	ESocketClosed(const SWstring sMsg, const int nError) :
		ESocket(sMsg, nError)
	{}

	virtual const SWstring Message() const;
};

class EConnectionCreate : public ESocket
{
public:
	EConnectionCreate(const SWstring sMsg, const int nError) : 
		ESocket(sMsg, nError)
	{}

	virtual const SWstring Message() const;
};

class EConnectionTimeout : public ESocket
{
	const int m_nTimeout;
public:
	EConnectionTimeout(const SWstring sMsg, const int nError, int nTimeout) : 
		ESocket(sMsg, nError), 
		m_nTimeout(nTimeout)
	{}

	const int GetTimeout() const { return m_nTimeout; }

	virtual const SWstring Message() const;
};




