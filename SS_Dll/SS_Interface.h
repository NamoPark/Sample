#pragma once

#include "stdafx.h"
#include <WinSock2.h>
#include <Windows.h>
#include <tchar.h>
#include "SS_CommonUtils/SS_StringDefines.h"

#define SS_SDK_EXPORT 
typedef struct _tPacket {
	INT32 rStatus;
	UINT16 rData[1024];
} tPacket;

// ----- Callback Events -----
// Event Types
#define ETTrace		1
#define ETErr		2
#define ETWSAErr	3
#define ETAck		4

// Events
#define EAbort					0x00010001
#define EClose					0x00010002
#define ECapture				0x00010003
#define ECapturePerc			0x00010004
#define ECaptureRecv			0x00010005

#define EAck_Start				0x00010006
#define EAck_EParam_FPGA		0x00020001
#define EAck_EParam_ROIC		0x00020002
#define EAck_Ver_Firmware		0x00020003
#define EAck_Ver_FPGA			0x00020004

#ifdef SS_SDK_EXPORT
#define SS_API __declspec(dllexport)
#else
#define OD_API __declspec(dllimport)
#endif 
// acquisition feedback record   
typedef struct _tDLIB_CBR {
	int    rFlags,            // combination of cVDACQ_Fxxxx
		rType,             // cVDACQ_ETxxx
		rEvent,            // cVDACQ_Exxx
		rSocket;           // 0:no socket relation; otherwise socket's ID >0 
	TCHAR  rMsg[512];         // message (trace, wrn, err)
	int    rFrameWidth,       // full frame width
		rFrameHeight;      // full frame height
	UINT16 *rFrameBuffer;      // user supplied frame buffer "AFrameBuffer"
	int    rCaptureRows,      // # of received rows
		rCapturePercent;   // received data in percents
	int RouteFunc;
	void  *rUserCallBackProc, // user supplied "ACallBackProc"
		*rUserParam;        // user supplied "AUserParam"
	int    rAborted;          // 1: VDACQ_Abort -1:internally
	void  *rPacketData;       // pointer to received packet; usually it is nil  
	int rThread_Flag;
	HANDLE rThread;
	INT16 *rDecompensationFrameBuffer;	// user supplied negative frame buffer
	bool bNPC;					// Negative Pixel Compensation
	bool bSaveDecompensation;		// Save Original Image

} tDLIB_CBRec;
typedef struct _tVaU_ImgDim {
	int  // 32bit signed integer
		rFrameWidth,    // full frame width
		rFrameHeight,   // full frame height
		rImgCutLeft,    // cut left after rotation & flip 
		rImgCutTop,     // cut top after rotation & flip
		rImgCutRight,   // cut right after rotation & flip
		rImgCutBottom,  // cut bottom after rotation & flip
		rRotation,      // 0:none 1:CCW 2:CW
		rFlip;          // bit #0: horz, bit #1:vert 

	// calculated fields (filled by CalcStoredDim)
	int rStoredWidth,   // image width
		rStoredHeight; // image height
} tVaU_ImgDim;


typedef struct _tCalPar {
	int rUseDark, rUseBright;
	int   rRadius;       // BP 탐색 범위
	double   rDarkLoPerc, // Dark / Bright에서 정상 픽셀에 해당하는 범위 (%)
		rDarkHiPerc;
	double rBrightLoPerc,
		rBrightHiPerc;
	int   rNumADC;      // ADC 및 ROIC 개수에 따른 가로 방향 블럭 갯수

	// Offset & Gain Calibration
	int bNegPxCompensation;
	int bSaveDecompensation;
	int Tolerence;
	int DarkMaxLSB; // 일반적인 조건에서 Dark영상의 최고 LSB값 (정상픽셀)
	int OfsSatuationTH;
	int GainStatingPoint_X;
	int GainStatingPoint_Y;
	int ApplyOffsetSaturationThreshold;
	int aedCmdEnable;
	int aedLineCorrection;
	int aedCorrectionEnable;
} tCalPar;

typedef struct _tAcqPar {
	UINT32 ExpTime;
	int PreDelay, PostDelay;
	int RdyDelay, ExpDelay;
	int bAverageDark, nAverageSkip, nAverageTotal;
	int CalibFlag;
	int bNoSave;
	// shlee
	int MultiFrame;
	int RoicNum;
} tAcqPar;

typedef struct _tDlgParam {
	//IP,DetModel,ImgDim은 실제 Dlib Ini에 저장됨
	//RD를 하려면 Dlib Ini를 읽어야함
	TCHAR IP_Addr[WCHAR_MAX_LENGTH];
	UINT DetModel;
	tVaU_ImgDim ImgDim;

	//SENA 에 저장되지만 Detector Change, Parameter 설정 변경 시 Set 필요함
	TCHAR DLibINI[WCHAR_MAX_LENGTH];
	//TCHAR DefINI_ADI[WCHAR_MAX_LENGTH];
	TCHAR DefINI_SA130[WCHAR_MAX_LENGTH];
	TCHAR DefINI_SA132[WCHAR_MAX_LENGTH];
	TCHAR PathCal[WCHAR_MAX_LENGTH];
	TCHAR PathImg[WCHAR_MAX_LENGTH];

	int TrgMode;

	//이하는 SENA INI 내용이다
	TCHAR Alias[WCHAR_MAX_LENGTH];
	int CalAlgorithm;
	UINT32 WndTime;
	UINT ExpDelay;
	UINT RdyDelay;
	UINT PreDelay;
	UINT PostDelay;
	tCalPar CalPar;
	// Flags
	int OfsCorection;
	int GainCorrection;
	int BPCorrection;
	int Crop;
	int SaturationMap;
	// Last known
	TCHAR Acq_ExPath_FrameSrc[WCHAR_MAX_LENGTH];
	TCHAR Acq_ExPath_FileName[WCHAR_MAX_LENGTH];
	int AverageDark;
	int TotalDark;
	int SkipDark;
	// Temporary (Only available in runtime)
	BOOL IsNewAdd;
	BOOL Modified;
	BOOL ChangeDetectorType;
	int FrameSource;
	BOOL NegPxCompensation;
	BOOL SaveDecompensation;
} tDlgParam;

// callback procedure  
typedef void(_stdcall *tDLIB_CBProc)(tDLIB_CBRec*);

#ifdef __cplusplus
extern "C" {
#endif

#if defined(SS_SDK_EXPORT)
	SS_API int _stdcall	ssCaptureStart(tDLIB_CBProc AUserCallBackProc, void *AUserParam,
		tAcqPar* argAcqPar = nullptr, int bPreview = 0);
	SS_API int _stdcall	ssCaptureStop();
	SS_API int _stdcall ssInitialize(CString cstrFilePath);
	SS_API int _stdcall ssDeInitialize();
	SS_API tDlgParam* _stdcall ssGetAppParam();
#else

	int(_stdcall *ssCaptureStart)		(tDLIB_CBProc AUserCallBackProc, void *AUserParam,
		tAcqPar* argAcqPar = nullptr, int bPreview = 0);
	int(_stdcall *ssCaptureStop)		();
	int(_stdcall *ssInitialize)			(CString cstrFilePath);
	int(_stdcall *ssDeInitialize)		();
	tDlgParam*(_stdcall *ssGetAppParam)	();
#endif
}