#pragma once
#include "SS_CommonUtils/SS_StringDefines.h"
#include "GIniFile.h"
#include "SS_Interface.h"

typedef struct		_tVaU_ImgDim tVaU_ImgDim;
typedef struct		_tCalPar tCalPar;


class SS_App_Param
{
private:
	SWstring		wsCalDirPath;
	SWstring		wsImgDirPath;
	SWstring		wsDetIniDirPath;
	SWstring		wsAppIniFilePath;
	TCHAR			IPAddr[WCHAR_MAX_LENGTH] = { 0, };
	TCHAR			m_iSingleFilePath[WCHAR_MAX_LENGTH] = { 0, };
	TCHAR			m_iMultiFilePath[WCHAR_MAX_LENGTH] = { 0, };
	TCHAR			m_iWildCard[WCHAR_MAX_LENGTH] = { 0, };
	TCHAR			m_oFolderPath[WCHAR_MAX_LENGTH] = { 0, };
	long			m_DetType;
	int				m_DetCount;
	int				m_lastSelected;
	int				m_InputType;
	bool			b_Initialize;
	GIniFile		gAppIni;
	tDlgParam		*m_ptDlgParam;
public:
	void			SetCalDirPath		(SWstring CalPath);
	void			SetImgDirPath		(SWstring ImgPath);
	void			SetDetIniDirPath	(SWstring IniPath);
	void			SetAppIniFilePath	(SWstring IniPath);
	void			SetDetCount			(int DetCount);
	void			SetDetLastSelected	(int DetIndex);
	void			SetDetModel			(int DetModel);
	void			SetAppiniParam		(tDlgParam* DlgParam);
	SWstring		GetCalPath			();
	SWstring		GetImgPath			();
	SWstring		GetAppIniPath		();
	SWstring		GetDetIniPath		();
	int				GetDetCount			();
	int				GetDetLastSelected	();
	int				GetDetModel			();
	tDlgParam *		GetAppParam			();

	SS_App_Param();
	SS_App_Param(CString csBasePath);
	~SS_App_Param();

	//Initialize sequence ฐüทร
	void			CheckIniFile		(SWstring wsBasePath);
	void			SetDefaultPath		(SWstring wsBasePath);
	int				CountDetector		(CString wsBasePath);
	void			CreateDefaultInfo	(CString wsBasePath);
	int				LoadAppIniParam		(CString wsBasePath, tDlgParam* pDlgParam);
	bool			bInitialized		();
	int				SaveLoad_AppIni		(CString Sect, tDlgParam* DlgParam, int Method, int DetCount);
};

