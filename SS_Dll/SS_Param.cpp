#include "stdafx.h"
#include "SS_Param.h"
#include "SS_CommonUtils/SS_FileUtils.h"
#include "SS_Dll.h"

SS_App_Param::SS_App_Param()
{

}

SS_App_Param::SS_App_Param(CString csBasePath)
{

	wsCalDirPath = L"";
	wsImgDirPath = L"";
	wsDetIniDirPath = L"";
	wsAppIniFilePath = L"";
	m_DetCount = 0;
	m_ptDlgParam = nullptr;
	b_Initialize = 1 ;

	SSstring wsAppIniFilePath((LPCTSTR)csBasePath);
	//Check, Make App ini File
	CheckIniFile(wsAppIniFilePath);
	//Set default path (ini, img, cal)
	SetDefaultPath(wsAppIniFilePath);
	//Count Detector
	m_DetCount = CountDetector(csBasePath);
	//if count 0, create default Detector info
	if (m_DetCount == 0) 
	{
		CreateDefaultInfo(csBasePath);
		m_DetCount = CountDetector(csBasePath);
	}
	//Count 만큼 App ini param 메모리 할당 및 초기화
	if (m_DetCount > 0) {
		if (m_ptDlgParam == nullptr) {
			m_ptDlgParam = new tDlgParam[m_DetCount];
			for (int i = 0; i < m_DetCount; i++) {
				*(m_ptDlgParam + i) = { 0, };
			}
		}
	}
	// Load App ini param
	if (LoadAppIniParam(csBasePath, m_ptDlgParam))
	{
		SS_LOG((*theApp.pSSLogger), LogLevel::Error, _T("Initialize Success"));
	}
	else
	{
		SS_LOG((*theApp.pSSLogger), LogLevel::Error, _T("Initialize Fail"));
		b_Initialize = false;
		return;
	}

	if(m_ptDlgParam==nullptr || m_DetCount == 0)
		b_Initialize = false;

	SetAppiniParam(m_ptDlgParam+ m_lastSelected);
}

SS_App_Param::~SS_App_Param()
{
	delete[] m_ptDlgParam;
}

void SS_App_Param::SetCalDirPath(SSstring CalPath)
{
	wsCalDirPath = CalPath;
}

void SS_App_Param::SetImgDirPath(SSstring ImgPath)
{
	wsImgDirPath = ImgPath;
}

void SS_App_Param::SetAppIniFilePath(SSstring IniPath)
{
	wsAppIniFilePath = IniPath;
}

void SS_App_Param::SetDetIniDirPath(SSstring IniPath)
{
	wsDetIniDirPath = IniPath;
}

void SS_App_Param::SetDetCount(int DetCount)
{
	m_DetCount = DetCount;
}

void SS_App_Param::SetDetLastSelected(int DetIndex)
{
	m_lastSelected = DetIndex;
}

void SS_App_Param::SetDetModel(int DetModel)
{
	m_DetType = DetModel;
}

void SS_App_Param::SetAppiniParam(tDlgParam * DlgParam)
{
	m_ptDlgParam = DlgParam;
}

SSstring SS_App_Param::GetCalPath()
{
	return wsCalDirPath;
}

SSstring SS_App_Param::GetImgPath()
{
	return wsImgDirPath;
}

SSstring SS_App_Param::GetAppIniPath()
{
	return wsAppIniFilePath;
}
SSstring SS_App_Param::GetDetIniPath()
{
	return wsDetIniDirPath;
}

int SS_App_Param::GetDetCount()
{
	return m_DetCount;
}

int SS_App_Param::GetDetLastSelected()
{
	return m_lastSelected;
}

int SS_App_Param::GetDetModel()
{
	return m_DetType;
}

tDlgParam * SS_App_Param::GetAppParam()
{
	return m_ptDlgParam+ m_lastSelected;
}

bool SS_App_Param::bInitialized()
{
	return b_Initialize;
}

int SS_App_Param::SaveLoad_AppIni(CString Sect, tDlgParam * DlgParam, int Method, int DetCount)
{
	gAppIni.SetSectName(_T("General"));
	gAppIni.TouchIniInt(_T("LastSelected"), &m_lastSelected, Method, 0);
	gAppIni.TouchIniInt(_T("AuxInputType"), &m_InputType, Method, 1);
	gAppIni.TouchIniStr(_T("AuxInputFolderPath"), m_iMultiFilePath, Method, _T(".\\"));
	gAppIni.TouchIniStr(_T("AuxInputFilePath"), m_iSingleFilePath, Method, _T(".\\"));
	gAppIni.TouchIniStr(_T("AuxInputWildCard"), m_iWildCard, Method, _T("*.raw"));
	gAppIni.TouchIniStr(_T("AuxOutputFolderPath"), m_oFolderPath, Method, _T(".\\"));

	if (DlgParam) {
		gAppIni.SetSectName(Sect);

		gAppIni.TouchIniStr(_T("Alias"), DlgParam->Alias, Method, _T("Alias"));

		CString cstrDlibCount;
		cstrDlibCount.Format(_T(".\\Dlib_%d.ini"), DetCount);
		gAppIni.TouchIniStr(_T("DLibINI"), DlgParam->DLibINI, Method, cstrDlibCount);

		CString cstrDefIni_SA130, cstrDefIni_SA132;
		cstrDefIni_SA130.Format(_T(".\\ini\\DM_SA130_Solution.ini"));
		gAppIni.TouchIniStr(_T("DefIni_SA130"), DlgParam->DefINI_SA130, Method, cstrDefIni_SA130);
		cstrDefIni_SA132.Format(_T(".\\ini\\DM_SA132_Solution.ini"));
		gAppIni.TouchIniStr(_T("DefIni_SA132"), DlgParam->DefINI_SA132, Method, cstrDefIni_SA132);
		
		CString DefCalPath, DefImgPath;
		DefCalPath = GetCalPath().c_str();
		DefImgPath = GetImgPath().c_str();
		gAppIni.TouchIniStr(_T("PathCal"), DlgParam->PathCal, Method, DefCalPath);
		gAppIni.TouchIniStr(_T("PathImg"), DlgParam->PathImg, Method, DefImgPath);
		SetCalDirPath((LPCTSTR)DefCalPath);
		SetImgDirPath((LPCTSTR)DefImgPath);

		
		gAppIni.TouchIniInt(_T("TrgMode"), &DlgParam->TrgMode, Method, 0);
		gAppIni.TouchIniInt(_T("CalAlgorithm"), &DlgParam->CalAlgorithm, Method, 0);
		gAppIni.TouchIniInt(_T("WndTime"), &DlgParam->WndTime, Method, 500);
		gAppIni.TouchIniInt(_T("ExpDelay"), &DlgParam->ExpDelay, Method, 0);
		gAppIni.TouchIniInt(_T("PreDelay"), &DlgParam->PreDelay, Method, 0);
		gAppIni.TouchIniInt(_T("PostDelay"), &DlgParam->PostDelay, Method, 0);
		gAppIni.TouchIniInt(_T("ReadyDelay"), &DlgParam->RdyDelay, Method, 0);
		
		gAppIni.TouchIniInt(_T("Tolerence"), &DlgParam->CalPar.Tolerence, Method, 350);
		gAppIni.TouchIniInt(_T("DarkMaxLSB"), &DlgParam->CalPar.DarkMaxLSB, Method, 1000);
		gAppIni.TouchIniInt(_T("ApplyOfsSaturationTH"), &DlgParam->CalPar.ApplyOffsetSaturationThreshold, Method, 1);
		gAppIni.TouchIniInt(_T("OfsSatuationTH"), &DlgParam->CalPar.OfsSatuationTH, Method, 14000);
		gAppIni.TouchIniInt(_T("bNegPxCompensation"), &DlgParam->CalPar.bNegPxCompensation, Method, 1);
		gAppIni.TouchIniInt(_T("bSaveDecompensation"), &DlgParam->CalPar.bSaveDecompensation, Method, 0);
		
		gAppIni.TouchIniInt(_T("BPM_UseDark"), &DlgParam->CalPar.rUseDark, Method, 1);
		gAppIni.TouchIniInt(_T("BPM_UseBright"), &DlgParam->CalPar.rUseBright, Method, 1);
		gAppIni.TouchIniFloat(_T("BrightHiPerc"), &DlgParam->CalPar.rBrightHiPerc, Method, 0.15);
		gAppIni.TouchIniFloat(_T("BrightLoPerc"), &DlgParam->CalPar.rBrightLoPerc, Method, 0.15);
		gAppIni.TouchIniFloat(_T("DarkHiPerc"), &DlgParam->CalPar.rDarkHiPerc, Method, 0.3);
		gAppIni.TouchIniFloat(_T("DarkLoPerc"), &DlgParam->CalPar.rDarkLoPerc, Method, 0.3);
		gAppIni.TouchIniInt(_T("NumADC"), &DlgParam->CalPar.rNumADC, Method, 12);
		gAppIni.TouchIniInt(_T("Radius"), &DlgParam->CalPar.rRadius, Method, 30);
		gAppIni.TouchIniInt(_T("aedLineCorrection"), &DlgParam->CalPar.aedLineCorrection, Method, 0);
		gAppIni.TouchIniInt(_T("aedCorrectionEnable"), &DlgParam->CalPar.aedCorrectionEnable, Method, 1);
		
		gAppIni.TouchIniInt(_T("FrameSource"), &DlgParam->FrameSource, Method, 0);
		gAppIni.TouchIniInt(_T("OfsCorrection"), &DlgParam->OfsCorection, Method, 1);
		gAppIni.TouchIniInt(_T("GainCorrection"), &DlgParam->GainCorrection, Method, 1);
		gAppIni.TouchIniInt(_T("BPCorrection"), &DlgParam->BPCorrection, Method, 1);
		gAppIni.TouchIniInt(_T("Saturation Correction"), &DlgParam->SaturationMap, Method, 1);
		gAppIni.TouchIniInt(_T("Crop"), &DlgParam->Crop, Method, 1);
		
		gAppIni.TouchIniStr(_T("ExPath_FrameSrc"), DlgParam->Acq_ExPath_FrameSrc, Method, DefImgPath);
		gAppIni.TouchIniStr(_T("ExPath_Filename"), DlgParam->Acq_ExPath_FileName, Method, _T("Specify Name"));
		
		gAppIni.TouchIniInt(_T("AverageDark"), &DlgParam->AverageDark, Method, 1);
		gAppIni.TouchIniInt(_T("TotalDark"), &DlgParam->TotalDark, Method, 10);
		gAppIni.TouchIniInt(_T("SkipDark"), &DlgParam->SkipDark, Method, 3);
		gAppIni.TouchIniInt(_T("GainStatingPoint_X"), &DlgParam->CalPar.GainStatingPoint_X, Method, 0);
		gAppIni.TouchIniInt(_T("GainStatingPoint_Y"), &DlgParam->CalPar.GainStatingPoint_Y, Method, 0);
		
		gAppIni.TouchIniInt(_T("DetectorModel"), (long*)& (DlgParam->DetModel), Method, (long)DetModel::DM_SA130_Solution);
		gAppIni.TouchIniStr(_T("IP"), DlgParam->IP_Addr, Method, _T("2.2.22.99"));
		gAppIni.TouchIniInt(_T("FrameWidth"), (long*)&(DlgParam->ImgDim.rFrameWidth), Method, 3072);
		gAppIni.TouchIniInt(_T("FrameHeight"), (long*)&(DlgParam->ImgDim.rFrameHeight), Method, 3072);
		gAppIni.TouchIniInt(_T("CropLeft"), (long*)&(DlgParam->ImgDim.rImgCutLeft), Method, 0);
		gAppIni.TouchIniInt(_T("CropRight"), (long*)&(DlgParam->ImgDim.rImgCutRight), Method, 0);
		gAppIni.TouchIniInt(_T("CropTop"), (long*)&(DlgParam->ImgDim.rImgCutTop), Method, 0);
		gAppIni.TouchIniInt(_T("CropBottom"), (long*)&(DlgParam->ImgDim.rImgCutBottom), Method, 0);
	
	}
	return 0;
}

void SS_App_Param::CheckIniFile(SSstring wsBasePath)
{
	if (!(FileExists(wsBasePath)))	//if not exist
	{
		MakeFile(wsBasePath);
	}
	gAppIni.SetFileName(wsBasePath.c_str());
}

void SS_App_Param::SetDefaultPath(SSstring wsBasePath)
{
	SSstring wsDefaultPath = ExtractFilePath(wsBasePath);
	SetAppIniFilePath(wsDefaultPath);
	SetCalDirPath(wsDefaultPath + L"Cal\\");
	SetImgDirPath(wsDefaultPath + L"Img\\");
	SetDetIniDirPath(wsDefaultPath + L"Ini\\");
}

int SS_App_Param::CountDetector(CString wsBasePath)
{
	//Section을 \0으로 구분하여 strSect에 저장
	TCHAR strSect[WCHAR_MAX_LENGTH];
	int nLen = GetPrivateProfileSectionNames(strSect, WCHAR_MAX_LENGTH, wsBasePath);
	int tDetCount = 0;
	CString cstrSect;
	cstrSect.Empty();
	for (int i = 0; i < nLen; i++) {
		cstrSect += strSect[i];
		if (strSect[i] == _T('\0')) {
			if (cstrSect.Find(_T("Detector_")) == 0)
			{
				tDetCount++;
			}
			cstrSect.Empty();
		}
	}
	return tDetCount;
}

void SS_App_Param::CreateDefaultInfo(CString wsBasePath)
{
	// -- Generate default templete
	TCHAR tcAppIniFilePath[WCHAR_MAX_LENGTH];
	_tcscpy(tcAppIniFilePath, wsBasePath);
	int tLastSelected = 0;
	gAppIni.SetFileName(tcAppIniFilePath);
	gAppIni.SetSectName(_T("General"));
	tLastSelected = gAppIni.SGetIniInt(_T("LastSelected"), (UINT16)0);
	SetDetLastSelected(tLastSelected);
	gAppIni.SetSectName(_T("Detector_0"));
	gAppIni.SGetIniStr(_T("Alias"), tcAppIniFilePath, _T("Default_0"));
	gAppIni.SGetIniStr(_T("DLibINI"), tcAppIniFilePath, _T(".\\Dlib_0.ini")/*theApp.m_Def_DIniPath*/);

}

int SS_App_Param::LoadAppIniParam(CString wsBasePath, tDlgParam* pDlgParam)
{
	CString localStr;
	CString cstrSect;
	TCHAR strSect[WCHAR_MAX_LENGTH];
	//Section을 \0으로 구분하여 strSect에 저장
	int nLen = GetPrivateProfileSectionNames(strSect, WCHAR_MAX_LENGTH, wsBasePath);
	cstrSect.Empty();
	/*_detList = new CString[theApp.m_nCntDet];*/
	int l_nCntDet = 0;
	for (int i = 0; i < nLen; i++) {
		cstrSect += strSect[i];
		if (strSect[i] == _T('\0'))
		{
			if (cstrSect.Find(_T("Detector_")) == 0)
			{
				BOOL bIniFile = 0;
				//SENA INI 내용을 읽어온다
				SaveLoad_AppIni(cstrSect, (pDlgParam + l_nCntDet), RD, l_nCntDet);

				if (AfxExtractSubString(localStr, cstrSect, 1, _T('_'))) {
					localStr += _T(' ');
					localStr += (pDlgParam + l_nCntDet)->Alias;
				}
				l_nCntDet++;
			}
			cstrSect.Empty();
		}
	}
	if (l_nCntDet == 0 || pDlgParam == NULL)
		return 0;

	return 1;
}

