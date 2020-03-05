#pragma once

/*
*  Copyright 2015 by Samsung Co.,Ltd., All rights reserved.
*
*  This software is the confidential and proprietary information
*  of Samsung, Inc. ("Confidential Information").  You
*  shall not disclose such Confidential Information and shall use
*  it only in accordance with the terms of the license agreement
*  you entered into with Samsung.
*/

/*!
* @file      sd_socket_exception.h
* @brief     SDSDK socket exceptions
* @author    js.view.kim@samsung.com
* @date      2015.07.16 (heading comment adding)
*/


#include "../SS_CommonUtils/SS_exception.h"

/*

ACQUISITION related exception.

*/

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




