
#include "stdafx.h"
#include "SS_Calibration.h"
#include "SS_Dll.h"
#include "SS_Interface.h"
#include "SS_Caputre.h"
#include "SS_Param.h"
#include "Common/Cmd_List.h"
#include "Misc.h"

static int g_nDarkCount = 0;
SS_Calibration* pSS_Calibration;

static tDLIB_CBProc ix_CBProc_Calib;
static tDLIB_CBRec ix_CBRec_Calib;

SS_Calibration::SS_Calibration()
{
	pSS_Calibration = this;
	m_FrameBuf = nullptr;
	m_pAvgDark = nullptr;
	m_PrBuf_Frame = nullptr;
	m_AcqPar = new tAcqPar;
	m_PrevH = m_PrevW = 0;
	SetCalibration();
}


SS_Calibration::~SS_Calibration()
{
	delete[] m_FrameBuf;
	delete[] m_pAvgDark;
	delete[] m_PrBuf_Frame;
	delete m_AcqPar;
}

static void _stdcall CBP_GetDark(tDLIB_CBRec *ACallBackRec)
{

	if (ACallBackRec->rUserCallBackProc) {
		((tDLIB_CBProc)ACallBackRec->rUserCallBackProc)(ACallBackRec);
	}
	switch (ACallBackRec->rType) {
	case ETErr: {
		switch (ACallBackRec->rEvent) {
		case EAbort:
		{
			break;
		}
		}
		break;
	}
	case ETTrace: {
		switch (ACallBackRec->rEvent) {
		case EAck_Start:
			break;
		case EClose: {
			break;
		}
		case ECaptureRecv:

		case ECapturePerc: // receive percentage
			if (ACallBackRec->rCapturePercent == 0x01) {
			}
			if (ACallBackRec->rCapturePercent == 0xFF) {
				pSS_Calibration->SaveCalFrame(DARK, ACallBackRec->rFrameBuffer, ACallBackRec->rFrameHeight*ACallBackRec->rFrameWidth);
			}
			break;
		}; // switch rEvent
		break;
	}
	}; // switch rType
} // OK


void SS_Calibration::SetCalibration()
{
	tDlgParam* tDlgParam = theApp.ssAppParam->GetAppParam();
	if (tDlgParam != NULL) {
		m_CalPar = &(tDlgParam->CalPar);
		m_ImgDim = &(tDlgParam->ImgDim);
		m_DetModel = tDlgParam->DetModel;
		m_GainPath = tDlgParam->PathCal;
		m_DarkPath = m_BPMPath = m_GainPath;
		if ((m_ImgDim->rFrameHeight != m_PrevH) || (m_ImgDim->rFrameWidth != m_PrevW)) {
			if (m_FrameBuf) {
				delete[] m_FrameBuf;
			}
			m_FrameBuf = new UINT16[m_ImgDim->rFrameHeight* m_ImgDim->rFrameWidth];
			m_PrevH = m_ImgDim->rFrameHeight;
			m_PrevW = m_ImgDim->rFrameWidth;
		}
	}
}

bool SS_Calibration::AD_GetDark(tDLIB_CBProc AUserCallBackProc, void * AUserParam, tAcqPar * argAcqPar, int bPreview)
{
	tPacket Pkt;
	g_nDarkCount = 0;

	if (argAcqPar)
		memcpy_s(m_AcqPar, sizeof(tAcqPar), argAcqPar, sizeof(tAcqPar));

	theApp.m_DLIB_CBR = AD_Connect(0, CBP_GetDark, AUserCallBackProc, AUserParam, m_FrameBuf);

	AD_Init(argAcqPar);

	m_NP = m_ImgDim->rFrameHeight * m_ImgDim->rFrameWidth;
	if (argAcqPar->bAverageDark) {
		if (m_pAvgDark)
			delete[] m_pAvgDark;

		m_pAvgDark = new double[m_ImgDim->rFrameHeight*m_ImgDim->rFrameWidth];
		memset(m_pAvgDark, 0, sizeof(double)*m_NP);
	}

	if (theApp.m_DLIB_CBR) {
		switch (m_DetModel)
		{
		case (long)DetModel::DM_SA132_Solution:
			Pkt.rStatus = CMD_132_START;
			break;
		case (long)DetModel::DM_SA130_Solution:
			Pkt.rStatus = CMD_130_START;
			break;
		default:
			break;
		}
		//Pkt.rStatus = CMD_START;
		Pkt.rData[0] = DARK;
		Pkt.rData[1] = argAcqPar->nAverageTotal;
		return ACQ_SendPacket(&Pkt);
	}
	else return false;
}
int Dark_Count = 0;
int SS_Calibration::SaveCalFrame(int AcqType, UINT16* Buf, UINT nNP)
{
	TCHAR l_tempstr[WCHAR_MAX_LENGTH];
	CString SavePath, SavePathDC;
	CFile fp;
	INT16 *BufDC = nullptr;
	int t;
	if (m_DarkPath.GetLength() == 0) {
		GetModuleFileName(AfxGetInstanceHandle(), l_tempstr, WCHAR_MAX_LENGTH);
		m_DarkPath.Format(_T("%s"), l_tempstr);
		t = m_DarkPath.ReverseFind(_T('\\'));
		m_DarkPath.Delete(t + 1, m_DarkPath.GetLength() - t);
	}
	else if (m_DarkPath.ReverseFind(_T('\\')) != (m_DarkPath.GetLength() - 1)) {
		m_DarkPath += _T('\\');
	}

	if (AcqType == DARK) {
		CString csDarkFileName;
		csDarkFileName.Format(_T("cDark_%d.raw"), Dark_Count++);
		SavePath = m_DarkPath + csDarkFileName; // default Dlib.ini path
	}
	// Save Unsigned Bright Image
	_tcscpy_s(l_tempstr, m_DarkPath);
	CreateDir(l_tempstr); // Create directory if it doesn't exist.
	if (!SavePath.IsEmpty())
	{
		SaveRawFrame(SavePath, FALSE, Buf, nNP);
	}
	return 0;
}