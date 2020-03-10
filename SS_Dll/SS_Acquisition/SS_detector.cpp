
#include "../SS_Interface.h"
#include "../SS_CommonUtils/Ginifile.h"
#include "../SS_CommonUtils/SS_FileUtils.h"
#include "../SS_CommonUtils/SS_data.h"


int CountDetector(CString cstr_AppPath)
{
	//Section을 \0으로 구분하여 strSect에 저장
	TCHAR strSect[WCHAR_MAX_LENGTH];
	int nLen = GetPrivateProfileSectionNames(strSect, WCHAR_MAX_LENGTH, cstr_AppPath);
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
int SaveLoad_AppIni(CString Sect, SS_Detector* Detector_Data, int Method, int DetCount)
{
	SS_Detector lDetector_Data = *(Detector_Data + DetCount);
	lDetector_Data.gAppIni.SetSectName(_T("General"));
	lDetector_Data.gAppIni.TouchIniInt(_T("LastSelected"), &lDetector_Data.m_lastSelected, Method, 0);
	lDetector_Data.gAppIni.TouchIniInt(_T("AuxInputType"), &lDetector_Data.m_InputType, Method, 1);
	lDetector_Data.gAppIni.TouchIniStr(_T("AuxInputFolderPath"), lDetector_Data.m_iMultiFilePath, Method, _T(".\\"));
	lDetector_Data.gAppIni.TouchIniStr(_T("AuxInputFilePath"), lDetector_Data.m_iSingleFilePath, Method, _T(".\\"));
	lDetector_Data.gAppIni.TouchIniStr(_T("AuxInputWildCard"), lDetector_Data.m_iWildCard, Method, _T("*.raw"));
	lDetector_Data.gAppIni.TouchIniStr(_T("AuxOutputFolderPath"), lDetector_Data.m_oFolderPath, Method, _T(".\\"));

	if (lDetector_Data.m_ptDlgParam!= nullptr) {
		lDetector_Data.gAppIni.SetSectName(Sect);

		lDetector_Data.gAppIni.TouchIniStr(_T("Alias"), lDetector_Data.m_ptDlgParam->Alias, Method, _T("Alias"));

		CString cstrDlibCount;
		cstrDlibCount.Format(_T(".\\Dlib_%d.ini"), DetCount);
		lDetector_Data.gAppIni.TouchIniStr(_T("DLibINI"), lDetector_Data.m_ptDlgParam->DLibINI, Method, cstrDlibCount);

		CString cstrDefIni_SA130, cstrDefIni_SA132;
		cstrDefIni_SA130.Format(_T(".\\ini\\DM_SA130_Solution.ini"));
		lDetector_Data.gAppIni.TouchIniStr(_T("DefIni_SA130"), lDetector_Data.m_ptDlgParam->DefINI_SA130, Method, cstrDefIni_SA130);
		cstrDefIni_SA132.Format(_T(".\\ini\\DM_SA132_Solution.ini"));
		lDetector_Data.gAppIni.TouchIniStr(_T("DefIni_SA132"), lDetector_Data.m_ptDlgParam->DefINI_SA132, Method, cstrDefIni_SA132);

		lDetector_Data.gAppIni.TouchIniStr(_T("PathCal"), lDetector_Data.m_ptDlgParam->PathCal, Method, lDetector_Data.m_default_cal_path);
		lDetector_Data.gAppIni.TouchIniStr(_T("PathImg"), lDetector_Data.m_ptDlgParam->PathImg, Method, lDetector_Data.m_default_img_path);



		lDetector_Data.gAppIni.TouchIniInt(_T("TrgMode"), &lDetector_Data.m_ptDlgParam->TrgMode, Method, 0);
		lDetector_Data.gAppIni.TouchIniInt(_T("CalAlgorithm"), &lDetector_Data.m_ptDlgParam->CalAlgorithm, Method, 0);
		lDetector_Data.gAppIni.TouchIniInt(_T("WndTime"), &lDetector_Data.m_ptDlgParam->WndTime, Method, 500);
		lDetector_Data.gAppIni.TouchIniInt(_T("ExpDelay"), &lDetector_Data.m_ptDlgParam->ExpDelay, Method, 0);
		lDetector_Data.gAppIni.TouchIniInt(_T("PreDelay"), &lDetector_Data.m_ptDlgParam->PreDelay, Method, 0);
		lDetector_Data.gAppIni.TouchIniInt(_T("PostDelay"), &lDetector_Data.m_ptDlgParam->PostDelay, Method, 0);
		lDetector_Data.gAppIni.TouchIniInt(_T("ReadyDelay"), &lDetector_Data.m_ptDlgParam->RdyDelay, Method, 0);
		lDetector_Data.gAppIni.TouchIniInt(_T("Tolerence"), &lDetector_Data.m_ptDlgParam->CalPar.Tolerence, Method, 350);
		lDetector_Data.gAppIni.TouchIniInt(_T("DarkMaxLSB"), &lDetector_Data.m_ptDlgParam->CalPar.DarkMaxLSB, Method, 1000);
		lDetector_Data.gAppIni.TouchIniInt(_T("ApplyOfsSaturationTH"), &lDetector_Data.m_ptDlgParam->CalPar.ApplyOffsetSaturationThreshold, Method, 1);
		lDetector_Data.gAppIni.TouchIniInt(_T("OfsSatuationTH"), &lDetector_Data.m_ptDlgParam->CalPar.OfsSatuationTH, Method, 14000);
		lDetector_Data.gAppIni.TouchIniInt(_T("bNegPxCompensation"), &lDetector_Data.m_ptDlgParam->CalPar.bNegPxCompensation, Method, 1);
		lDetector_Data.gAppIni.TouchIniInt(_T("bSaveDecompensation"), &lDetector_Data.m_ptDlgParam->CalPar.bSaveDecompensation, Method, 0);
		lDetector_Data.gAppIni.TouchIniInt(_T("BPM_UseDark"), &lDetector_Data.m_ptDlgParam->CalPar.rUseDark, Method, 1);
		lDetector_Data.gAppIni.TouchIniInt(_T("BPM_UseBright"), &lDetector_Data.m_ptDlgParam->CalPar.rUseBright, Method, 1);
		lDetector_Data.gAppIni.TouchIniFloat(_T("BrightHiPerc"), &lDetector_Data.m_ptDlgParam->CalPar.rBrightHiPerc, Method, 0.15);
		lDetector_Data.gAppIni.TouchIniFloat(_T("BrightLoPerc"), &lDetector_Data.m_ptDlgParam->CalPar.rBrightLoPerc, Method, 0.15);
		lDetector_Data.gAppIni.TouchIniFloat(_T("DarkHiPerc"), &lDetector_Data.m_ptDlgParam->CalPar.rDarkHiPerc, Method, 0.3);
		lDetector_Data.gAppIni.TouchIniFloat(_T("DarkLoPerc"), &lDetector_Data.m_ptDlgParam->CalPar.rDarkLoPerc, Method, 0.3);
		lDetector_Data.gAppIni.TouchIniInt(_T("NumADC"), &lDetector_Data.m_ptDlgParam->CalPar.rNumADC, Method, 12);
		lDetector_Data.gAppIni.TouchIniInt(_T("Radius"), &lDetector_Data.m_ptDlgParam->CalPar.rRadius, Method, 30);
		lDetector_Data.gAppIni.TouchIniInt(_T("aedLineCorrection"), &lDetector_Data.m_ptDlgParam->CalPar.aedLineCorrection, Method, 0);
		lDetector_Data.gAppIni.TouchIniInt(_T("aedCorrectionEnable"), &lDetector_Data.m_ptDlgParam->CalPar.aedCorrectionEnable, Method, 1);
		lDetector_Data.gAppIni.TouchIniInt(_T("FrameSource"), &lDetector_Data.m_ptDlgParam->FrameSource, Method, 0);
		lDetector_Data.gAppIni.TouchIniInt(_T("OfsCorrection"), &lDetector_Data.m_ptDlgParam->OfsCorection, Method, 1);
		lDetector_Data.gAppIni.TouchIniInt(_T("GainCorrection"), &lDetector_Data.m_ptDlgParam->GainCorrection, Method, 1);
		lDetector_Data.gAppIni.TouchIniInt(_T("BPCorrection"), &lDetector_Data.m_ptDlgParam->BPCorrection, Method, 1);
		lDetector_Data.gAppIni.TouchIniInt(_T("Saturation Correction"), &lDetector_Data.m_ptDlgParam->SaturationMap, Method, 1);
		lDetector_Data.gAppIni.TouchIniInt(_T("Crop"), &lDetector_Data.m_ptDlgParam->Crop, Method, 1);
		lDetector_Data.gAppIni.TouchIniStr(_T("ExPath_FrameSrc"), lDetector_Data.m_ptDlgParam->Acq_ExPath_FrameSrc, Method, lDetector_Data.m_default_img_path);
		lDetector_Data.gAppIni.TouchIniStr(_T("ExPath_Filename"), lDetector_Data.m_ptDlgParam->Acq_ExPath_FileName, Method, _T("Specify Name"));
		lDetector_Data.gAppIni.TouchIniInt(_T("AverageDark"), &lDetector_Data.m_ptDlgParam->AverageDark, Method, 1);
		lDetector_Data.gAppIni.TouchIniInt(_T("TotalDark"), &lDetector_Data.m_ptDlgParam->TotalDark, Method, 10);
		lDetector_Data.gAppIni.TouchIniInt(_T("SkipDark"), &lDetector_Data.m_ptDlgParam->SkipDark, Method, 3);
		lDetector_Data.gAppIni.TouchIniInt(_T("GainStatingPoint_X"), &lDetector_Data.m_ptDlgParam->CalPar.GainStatingPoint_X, Method, 0);
		lDetector_Data.gAppIni.TouchIniInt(_T("GainStatingPoint_Y"), &lDetector_Data.m_ptDlgParam->CalPar.GainStatingPoint_Y, Method, 0);
		lDetector_Data.gAppIni.TouchIniInt(_T("DetectorModel"), (long*)& (lDetector_Data.m_ptDlgParam->DetModel), Method, (long)DetModel::DM_SA130_Solution);
		
		lDetector_Data.gAppIni.TouchIniStr(_T("IP"), lDetector_Data.m_ptDlgParam->IP_Addr, Method, _T("2.2.22.99"));
		lDetector_Data.gAppIni.TouchIniInt(_T("FrameWidth"), (long*)&(lDetector_Data.m_ptDlgParam->ImgDim.rFrameWidth), Method, 3072);
		lDetector_Data.gAppIni.TouchIniInt(_T("FrameHeight"), (long*)&(lDetector_Data.m_ptDlgParam->ImgDim.rFrameHeight), Method, 3072);
		lDetector_Data.gAppIni.TouchIniInt(_T("CropLeft"), (long*)&(lDetector_Data.m_ptDlgParam->ImgDim.rImgCutLeft), Method, 0);
		lDetector_Data.gAppIni.TouchIniInt(_T("CropRight"), (long*)&(lDetector_Data.m_ptDlgParam->ImgDim.rImgCutRight), Method, 0);
		lDetector_Data.gAppIni.TouchIniInt(_T("CropTop"), (long*)&(lDetector_Data.m_ptDlgParam->ImgDim.rImgCutTop), Method, 0);
		lDetector_Data.gAppIni.TouchIniInt(_T("CropBottom"), (long*)&(lDetector_Data.m_ptDlgParam->ImgDim.rImgCutBottom), Method, 0);

	}
	return 0;
}

SS_DetectorHandel _stdcall ssCreateDetector(CString cstr_AppPath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// 1. Check, Make App ini File
	SS_Detector *tDetectorHandle = new SS_Detector;
	if (!(FileExists(CTOW(cstr_AppPath))))	//if not exist
	{
		 bool bOk = MakeFile(CTOW(cstr_AppPath));
		 if (!bOk) 
		 {
			 delete tDetectorHandle;
			 return nullptr;
		 }
	}
	tDetectorHandle->gAppIni.SetFileName(cstr_AppPath);

	// 2. Set default path (ini, img, cal)
	SWstring wsDefaultPath = ExtractFilePath(CTOW(cstr_AppPath));
	CString csDefaultPath = WTOC(wsDefaultPath);
	tDetectorHandle->m_default_cal_path = csDefaultPath + _T("Cal\\");
	tDetectorHandle->m_default_img_path = csDefaultPath + _T("Img\\");
	tDetectorHandle->m_default_ini_path = csDefaultPath + _T("Ini\\");

	// 3 . Number of registered detectors
	tDetectorHandle->m_DetCount = CountDetector(cstr_AppPath);

	// 4. if Count 0, create default Detector info
	if (tDetectorHandle->m_DetCount == 0)
	{
		TCHAR tcAppIniFilePath[WCHAR_MAX_LENGTH];
		_tcscpy(tcAppIniFilePath, cstr_AppPath);
		int tLastSelected = 0;
		tDetectorHandle->gAppIni.SetSectName(_T("General"));
		tDetectorHandle->m_lastSelected = tDetectorHandle->gAppIni.SGetIniInt(_T("LastSelected"), (UINT16)0);
		tDetectorHandle->gAppIni.SetSectName(_T("Detector_0"));
		tDetectorHandle->gAppIni.SGetIniStr(_T("Alias"), tcAppIniFilePath, _T("Default_0"));
		tDetectorHandle->gAppIni.SGetIniStr(_T("DLibINI"), tcAppIniFilePath, _T(".\\Dlib_0.ini")/*theApp.m_Def_DIniPath*/);
		tDetectorHandle->m_DetCount = CountDetector(cstr_AppPath);
	}

	// 5. Count 만큼 App ini param 메모리 할당 및 초기화
	if (tDetectorHandle->m_DetCount > 0) {
		if (tDetectorHandle->m_ptDlgParam == nullptr) {
			tDetectorHandle->m_ptDlgParam = new tDlgParam[tDetectorHandle->m_DetCount];
			for (int i = 0; i < tDetectorHandle->m_DetCount; i++) {
				*(tDetectorHandle->m_ptDlgParam + i) = { 0, };
			}
		}
	}
	else
	{
		delete tDetectorHandle;
		return nullptr;
	}

	CString localStr;
	CString cstrSect;
	TCHAR strSect[WCHAR_MAX_LENGTH];
	//Section을 \0으로 구분하여 strSect에 저장
	int nLen = GetPrivateProfileSectionNames(strSect, WCHAR_MAX_LENGTH, cstr_AppPath);
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
				SaveLoad_AppIni(cstrSect, tDetectorHandle, RD, l_nCntDet);

				if (AfxExtractSubString(localStr, cstrSect, 1, _T('_'))) {
					localStr += _T(' ');
					localStr += (tDetectorHandle->m_ptDlgParam + l_nCntDet)->Alias;
				}
				l_nCntDet++;
			}
			cstrSect.Empty();
		}
	}
	return tDetectorHandle;
}


bool _stdcall ssDestroyDetector(SS_DetectorHandel vdDetectorHandle)
{
	SS_Detector *tDetectorHandle = (SS_Detector*) vdDetectorHandle;

	if (tDetectorHandle != nullptr)
	{
		if (tDetectorHandle->m_ptDlgParam != nullptr)
		{
			delete[] tDetectorHandle->m_ptDlgParam;
			tDetectorHandle->m_ptDlgParam = nullptr;
		}
		delete tDetectorHandle;
		tDetectorHandle = nullptr;
		return true;
	}
	else
	{
		return false;
	}
}