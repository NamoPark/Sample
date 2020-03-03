#pragma 

typedef struct _tDLIB_CBR tDLIB_CBRec;
typedef void(_stdcall *tDLIB_CBProc)(tDLIB_CBRec*);

typedef struct _tVaU_ImgDim tVaU_ImgDim;
typedef struct _tCalPar tCalPar;
typedef struct _tAcqPar tAcqPar;

class SS_Calibration
{
public:
	SS_Calibration();
	~SS_Calibration();

	void SetCalibration();
protected:


public:
	CString m_DarkPath, m_BPMPath, m_GainPath;
	UINT m_PrevW, m_PrevH;
	UINT16* m_FrameBuf;
	INT32* m_PrBuf_Frame;
	double* m_pAvgDark;
	tAcqPar* m_AcqPar;
	tCalPar* m_CalPar;
	tVaU_ImgDim* m_ImgDim;
	int m_DetModel;
	int m_NP; // Number of pixels

	bool AD_GetDark(tDLIB_CBProc AUserCallBackProc, void *AUserParam, tAcqPar* argAcqPar, int bPreview = 0);
	int SaveCalFrame(int AcqType, UINT16* Buf, UINT nNP);

};

