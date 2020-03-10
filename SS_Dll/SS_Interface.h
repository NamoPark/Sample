#pragma once

#include "stdafx.h"
#include "SS_CommonUtils/SS_StringDefines.h"

#define SS_SDK_EXPORT 

#ifdef SS_SDK_EXPORT
#define SS_API __declspec(dllexport)
#else
#define OD_API __declspec(dllimport)
#endif 

//Network Define
#define SS_TCP			0
#define SS_UDP			1

//Handler Define
#define SS_DetectorHandel		void	*
#define SS_AcqHandle			void	*

//Acq Type Define
#define SS_GetDark		0
#define SS_GetBright	1
#define SS_GetImage		2	


/// the return value of SDAcq_Execute, which stands for 'acquisition success'.
/// @see SDAcq_Execute
#define RESULT_SUCCESS	0
/// the return value of SDAcq_Execute, which stands for 'acquisition aborted'.
/// @see SDAcq_Execute
#define RESULT_ABORT	1
/// the return value of SDAcq_Execute, which stands for 'the detector was time-out'.
/// @see SDAcq_Execute
#define RESULT_TIMEOUT	2
/// the return value of SDAcq_Execute, which stands for 'the detector was got error'.
/// @see SDAcq_Execute
#define RESULT_ERROR	3
/// the return value of SDAcq_Execute, which stands for 'the hand-switch is canceled'.
/// @see SDAcq_Execute
#define RESULT_CANCEL	4
/// validation of argument is failed.
/// @see SDAcq_Execute
#define RESULT_INVALID_ARG	5
/// the call is rejected, it may be caused by 'acquisition state' requirement. or be caused by 'not acceptable operation mode'.
/// i.e if you called SDAcq_GetDetectorStatus(...) when the acquisition state is 'disconnected', it fails with RESULT_REJECTED.
/// @see SDAcq_Execute
#define RESULT_REJECTED		6
/// the call is not possible to process, it may be caused by 'connection type', wired, wireless. 
/// for example, when the detector uses wired connection, SD_SetOperationMode(..., DR_OPR_SLEEP_MODE) returns RESULT_IMPOSIBLE.
/// @see SDAcq_Execute
#define RESULT_IMPOSSIBLE	7
/// the detector is on refreshing. it shows that the detector is not ready to acquire a frame.
/// try again about 5 seconds from the last successful acquisition.
#define RESULT_BUSY			8
/// the SDSDK suffer from the lack of resource.
/// it may cause by allocation error.
#define RESULT_LACK			9


//Callback handler
/// the data handler is a function to get the data when the SDK receives the image data of the detector.
typedef void(_stdcall *SSAcq_DataHandler)(void *pContextData, void *pData, int nSize);

/// the exception handler is a function to get the exception notification from the SDK.
typedef void(_stdcall *SSAcq_ExceptionHandler)(void *pContextData, int nExpId);

/// the event handler is to get the event notification from the SDK.
typedef void(_stdcall *SSAcq_EventHandler)(void *pContextData, int nEventId);

#ifdef __cplusplus
extern "C" {
#endif

#if defined(SS_SDK_EXPORT)
	//Detector 관련 Handler
	SS_API SS_DetectorHandel	_stdcall	ssCreateDetector		(CString csAppPath);
	SS_API bool					_stdcall	ssDestroyDetector		(SS_DetectorHandel vdDetectorHandle);
	//Acquisition 관련 Handler
	SS_API SS_AcqHandle			_stdcall	ssCreateConnection		(SS_DetectorHandel vdDetectorHandle);
	SS_API bool					_stdcall	ssDestroyConnection		(SS_AcqHandle hAcqHandle);
	//Acquisition 동작 관련
	SS_API int					_stdcall	ssCaptureStart			(SS_AcqHandle hAcqHandle, int AcqType);
	SS_API int					_stdcall	ssCaptureStop			(SS_AcqHandle hAcqHandle);
	SS_API int					_stdcall	ssCaptureClose			(SS_AcqHandle hAcqHandle);
	//Callback 관련 Handler
	SS_API bool					_stdcall	ssSetDataHandler		(SS_AcqHandle hAcqHandle, SSAcq_DataHandler pDataHandler);
	SS_API bool					_stdcall	ssSetExceptionHandler	(SS_AcqHandle hAcqHandle, SSAcq_ExceptionHandler pExceptionHandler);
	SS_API bool					_stdcall	ssSetEventHandler		(SS_AcqHandle hAcqHandle, SSAcq_EventHandler pEventHandler);

#else
	typedef struct {
		HINSTANCE rHDLL; // the library's handle
		SS_DetectorHandel	(_stdcall *ssCreateDetector)		(CString csAppPath);
		SS_AcqHandle		(_stdcall *ssCreateConnection)		(SS_DetectorHandel hDetector);
		bool				(_stdcall *ssDestroyDetector)		(SS_DetectorHandel csAppPath);

	}tDlib_InterfaceRec;
	BOOL DLib_MapDLL(const TCHAR *ADllFName, tDlib_InterfaceRec *AIntfRec);
	void DLib_ReleaseDLL(tDlib_InterfaceRec *AIntfRec);
#endif
}