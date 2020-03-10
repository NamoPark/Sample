#include "SS_acquisition_dynamic.h"
#include "../SS_Logger/Logger.h"
#include "../SS_Dll.h"

Dynamic_Connection::Dynamic_Connection(SS_Detector* pDetector, SWstring sAddress) :
	CConnection(pDetector, sAddress, SS_UDP)
{
	if (pDetector == NULL)
	{
		throw ENullDetector(_T("detector is null"));
	}

	if (ValidateAddress(sAddress) == false)
	{
		throw EInvalidParam(_T("invalid address string"));
	}

	m_nFrameWidth = m_pDetector->m_ptDlgParam->ImgDim.rFrameWidth;
	m_nFrameHeight = m_pDetector->m_ptDlgParam->ImgDim.rFrameHeight;
	if (m_nFrameWidth <= 0 || m_nFrameHeight <= 0)
	{
		throw EInvalidParam(_T("invalid frame size."));
	}
	//nh SaveFrame이랑 합쳐야함.
	m_nBytesToRead= m_nFrameWidth * m_nFrameHeight * sizeof(short);
	m_pFrameData = new unsigned char[m_nBytesToRead];
	
}

Dynamic_Connection::~Dynamic_Connection()
{
	try
	{
		if (m_pFrameData)
		{
			delete[] m_pFrameData;
			m_pFrameData = NULL;
		}
	}
	catch (...)
	{
		// do nothing
	}
}

int Dynamic_Connection::Execute(int atAcqType)
{
	bool bOK = false;

	bOK = Connect();

	if (bOK == false)
	{
		Disconnect();
		return RESULT_ERROR;
	}

	bOK = CreateThread();
	if (bOK == false)
	{
		CloseThread();
		return RESULT_ERROR;
	}

	return bOK;
}
