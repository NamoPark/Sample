#pragma once
#include "../stdafx.h"
#include "Ginifile.h"
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
	int   rRadius;       // BP Ž�� ����
	double   rDarkLoPerc, // Dark / Bright���� ���� �ȼ��� �ش��ϴ� ���� (%)
		rDarkHiPerc;
	double rBrightLoPerc,
		rBrightHiPerc;
	int   rNumADC;      // ADC �� ROIC ������ ���� ���� ���� �� ����

	// Offset & Gain Calibration
	int bNegPxCompensation;
	int bSaveDecompensation;
	int Tolerence;
	int DarkMaxLSB; // �Ϲ����� ���ǿ��� Dark������ �ְ� LSB�� (�����ȼ�)
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
	//IP,DetModel,ImgDim�� ���� Dlib Ini�� �����
	//RD�� �Ϸ��� Dlib Ini�� �о����
	TCHAR IP_Addr[WCHAR_MAX_LENGTH];
	UINT DetModel;
	tVaU_ImgDim ImgDim;
	//SENA �� ��������� Detector Change, Parameter ���� ���� �� Set �ʿ���
	TCHAR DLibINI[WCHAR_MAX_LENGTH];
	//TCHAR DefINI_ADI[WCHAR_MAX_LENGTH];
	TCHAR DefINI_SA130[WCHAR_MAX_LENGTH];
	TCHAR DefINI_SA132[WCHAR_MAX_LENGTH];
	TCHAR PathCal[WCHAR_MAX_LENGTH];
	TCHAR PathImg[WCHAR_MAX_LENGTH];
	TCHAR PathIni[WCHAR_MAX_LENGTH];
	int TrgMode;

	//���ϴ� SENA INI �����̴�
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

typedef struct _SS_data
{
	GIniFile		gAppIni;
	tDlgParam		DlgParam;

}SS_data;