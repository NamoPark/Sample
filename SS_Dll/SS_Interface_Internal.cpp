#include "stdafx.h"
#include "SS_Acquisition/SS_acquisition_base_dynamic.h"
#include "SS_CommonUtils/SS_data.h"
#include "SS_CommonUtils/SS_StringDefines.h"

#include "SS_Dll.h"
#include "SS_Logger/Logger.h"

SS_AcqHandle _stdcall ssCreateConnection(SS_DetectorHandel hDetector)
{
	SS_data* tDetector = (SS_data*)hDetector;

	SWstring sAddress(tDetector->m_ptDlgParam->IP_Addr);
	if (::ValidateAddress(sAddress) == false)
	{
		SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("invalid address."));
		return NULL;
	}

	Dynamic_CConnection *pInst = NULL;

	try
	{
		pInst = new Dynamic_CConnection(sAddress);
	}
	catch (ENullDetector &e)
	{
		SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("ENullDetector : %s"), e.Message().c_str());
		return NULL;
	}
	catch (EInvalidParam &e)
	{
		SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("EInvalidParam : %s"), e.Message().c_str());
		return NULL;
	}
	catch (EConnectionCreate &e)
	{
		SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("EConnectionCreate : %s"), e.Message().c_str());
		return NULL;
	}

	return pInst;
}