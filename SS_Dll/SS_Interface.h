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
#define SS_DetectorHandel		void	*
#define SS_AcqHandle			void	*

#ifdef __cplusplus
extern "C" {
#endif

#if defined(SS_SDK_EXPORT)

	SS_API SS_DetectorHandel	_stdcall	ssCreateDetector	(CString csAppPath);
	SS_API SS_AcqHandle			_stdcall	ssCreateConnection	(SS_DetectorHandel hDetector);
	SS_API bool _stdcall	ssDestroyDetector(SS_DetectorHandel vdDetectorHandle);
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