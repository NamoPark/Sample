#include "stdafx.h"
#include "SS_Dll.h"
#include "SS_Interface.h"
#include "SS_file_utils.h"
#include "SS_Calibration.h"
#include "SS_Param.h"

int _stdcall ssCaptureStart(tDLIB_CBProc AUserCallBackProc, void *AUserParam, 
	tAcqPar* argAcqPar, int bPreview)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	try {
		if (theApp.m_pD_Calibration == nullptr)
			theApp.CreateCalibration();
		else
			theApp.m_pD_Calibration->SetCalibration();
	}
	catch (bool exception) {
		return exception;
	}

	if (argAcqPar == nullptr) {
		tAcqPar A;
		memset(&A, 0, sizeof(tAcqPar));
		A.ExpDelay = 1;
		A.ExpTime = 500;
		A.RdyDelay = 1;
		A.PostDelay = 0;
		A.PreDelay = 0;
		argAcqPar = &A;
	}
	else {
		if (argAcqPar->ExpTime == 0)
			argAcqPar->ExpTime = 1;
		if (argAcqPar->ExpDelay == 0)
			argAcqPar->ExpDelay = 1;
		if (argAcqPar->RdyDelay == 0)
			argAcqPar->RdyDelay = 1;
	}
	return theApp.m_pD_Calibration->AD_GetDark(AUserCallBackProc, AUserParam, argAcqPar, bPreview);
}

int _stdcall ssCaptureStop()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	AfxBeginThread(theApp.CloseThread, &theApp);
	return 0;
}

int _stdcall ssInitialize(CString cstrFilePath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	theApp.ssAppParam = new SS_App_Param(cstrFilePath);
	return theApp.ssAppParam->bInitialized();
}

int _stdcall ssDeInitialize()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	delete theApp.ssAppParam;
	return 0;
}

int _stdcall ssDetInitialize(CString cStrPath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return 0;
}

tDlgParam* _stdcall ssGetAppParam()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return theApp.ssAppParam->GetAppParam();
}