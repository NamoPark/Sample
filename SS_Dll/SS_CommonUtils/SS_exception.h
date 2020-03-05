#pragma once

#include "SS_StringDefines.h"
using namespace std;

struct Exception
{
	const SWstring m_sMsg;
	explicit Exception(const SWstring sMsg) : m_sMsg(sMsg)
	{}

	virtual const SWstring Message() const
	{
		return m_sMsg;
	}
};

class ENullDetector : public Exception
{
public:
	explicit ENullDetector(const SWstring sMsg) :
		Exception(sMsg)
	{}
};

class EInvalidParam : public Exception
{
public:
	explicit EInvalidParam(const SWstring sMsg) :
		Exception(sMsg)
	{}
};




