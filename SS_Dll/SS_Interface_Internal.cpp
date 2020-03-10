#include "stdafx.h"
#include "SS_Acquisition/SS_acquisition_dynamic.h"
#include "SS_CommonUtils/SS_data.h"
#include "SS_CommonUtils/SS_StringDefines.h"

#include "SS_Dll.h"
#include "SS_Logger/Logger.h"

SS_AcqHandle _stdcall ssCreateConnection(SS_DetectorHandel vdDetectorHandle)
{
	SS_Detector* tDetector = (SS_Detector*)vdDetectorHandle;

	SWstring sAddress(tDetector->m_ptDlgParam->IP_Addr);
	if (::ValidateAddress(sAddress) == false)
	{
		SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("invalid address."));
		return NULL;
	}

	Dynamic_Connection *pInst = NULL;

	try
	{
		pInst = new Dynamic_Connection(tDetector,sAddress);
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

bool _stdcall ssDestroyConnection(SS_AcqHandle hHandle)
{
	if (hHandle)
	{
		Dynamic_Connection *pInst = (Dynamic_Connection *)hHandle;
		delete pInst;
		pInst = NULL;
		return true;
	}
	return false;
}

bool _stdcall ssSetDataHandler(SS_AcqHandle hHandle, SSAcq_DataHandler pDataHandler)
{
	if (hHandle)
	{
		Dynamic_Connection *pInst = (Dynamic_Connection *)hHandle;
		if (pInst == NULL)
		{
			SS_LOG((*theApp.pSSLogger), LogLevel::Info,_T("cannot resolve acquisition handle"));
			return false;
		}
		pInst->SetDataHandler(pDataHandler);
		return true;
	}
	return false;
}

bool _stdcall ssSetExceptionHandler(SS_AcqHandle hHandle, SSAcq_ExceptionHandler pExceptionHandler)
{
	if (hHandle)
	{
		Dynamic_Connection *pInst = (Dynamic_Connection *)hHandle;
		if (pInst == NULL)
		{
			SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("cannot resolve acquisition handle"));
			return false;
		}
		pInst->SetExceptionHandler(pExceptionHandler);
		return true;
	}
	return false;
}

bool _stdcall ssSetEventHandler(SS_AcqHandle hHandle, SSAcq_EventHandler pEventHandler)
{
	if (hHandle)
	{
		Dynamic_Connection *pInst = (Dynamic_Connection *)hHandle;
		if (pInst == NULL)
		{
			SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("cannot resolve acquisition handle"));
			return false;
		}
		pInst->SetEventHandler(pEventHandler);
		return true;
	}
	return false;
}

int _stdcall ssCaptureStart(SS_AcqHandle hHandle,int AcqType) 
{
	if (hHandle) {
		Dynamic_Connection *pInst = (Dynamic_Connection*)hHandle;
		if (pInst == NULL)
		{
			SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("cannot resolve acquisition handle"));
			return RESULT_INVALID_ARG;
		}

		try
		{
			int oRet = pInst->Execute(AcqType);
			return oRet;
		}
		catch (EConnectionTimeout &e)
		{
			SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("EConnectionTimeout %s"), e.Message().c_str());
			return RESULT_TIMEOUT;
		}
		catch (ESocketClosed &e)
		{
			SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("ESocketClosed %s"), e.Message().c_str());
			return RESULT_ERROR;
		}
		catch (ESocket &e)
		{
			SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("ESocket %s"), e.Message().c_str());
			return RESULT_ERROR;
		}
		catch (Exception &e)
		{
			SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("Exception %s"), e.Message().c_str());
			return RESULT_ERROR;
		}
		catch (...)
		{
			SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("Exception"));
			return RESULT_ERROR;
		}
		return RESULT_ERROR;
	}
	else {
		SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("execute : hHandle is NULL"));
		return RESULT_INVALID_ARG;
	}
};

int _stdcall ssCaptureStop(SS_AcqHandle hHandle)
{
	return 0;
}

int _stdcall ssCaptureClose(SS_AcqHandle hHandle)
{
	return 0;
};