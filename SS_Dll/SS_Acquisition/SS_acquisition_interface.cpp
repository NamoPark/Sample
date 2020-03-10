#include "SS_acquisition_interface.h"
#include "SS_acquisition_base_static.h"
#include "SS_acquisition_base_dynamic.h"
#include "../SS_CommonUtils/SS_data.h"
#include "../SS_CommonUtils/SS_StringDefines.h"

#include "../SS_Interface.h"

SS_AcqHandle _stdcall ssCreateConnection(SS_DetectorHandel hDetector) 
{
	SS_data* tDetector = (SS_data*)hDetector;

	SWstring sAddress(tDetector->m_ptDlgParam->IP_Addr);
	if (::ValidateAddress(sAddress) == false)
	{
		//LogDebug("invalid address.");
		return NULL;
	}

	Dynamic_CConnection *pInst = NULL;


	try
	{
		pInst = new Dynamic_CConnection(sAddress);
	}
	catch (ENullDetector &e)
	{
		//LogInfo(LC_INFO_ACQUISITION).WriteLine(_T("ENullDetector : %s"), e.Message().c_str());
		return NULL;
	}
	catch (EInvalidParam &e)
	{
		//LogInfo(LC_INFO_ACQUISITION).WriteLine(_T("EInvalidParam : %s"), e.Message().c_str());
		return NULL;
	}
	catch (EConnectionCreate &e)
	{
		//LogInfo(LC_INFO_ACQUISITION).WriteLine(_T("EConnectionCreate : %s"), e.Message().c_str());
		return NULL;
	}

	return pInst;
}