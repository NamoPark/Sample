#pragma once

template <typename Tp>
Tp GIniFile::GetIniInt(const TCHAR *AKeyName, Tp ADeflt)
{
	TCHAR qS[cMaxStrLength], qSD[cMaxStrLength];
	_ltot_s(ADeflt, qSD, cMaxStrLength, 0xA);
	GetIniStr(AKeyName, qS, qSD);
	if (!rLastResult) return ADeflt;
	else return (Tp)_tcstol(qS, NULL, 0xA);
}

template<typename Tp>
Tp GIniFile::SGetIniInt(const TCHAR *AKeyName, Tp ADeflt)
{
	Tp qRes = GetIniInt(AKeyName, ADeflt);
	if (!rLastResult)
		SetIniInt(AKeyName, ADeflt);
	return qRes;
}

template<typename Tp>
int GIniFile::SetIniInt(const TCHAR *AKeyName, Tp AVal)
{
	TCHAR qS[cMaxStrLength];
	_stprintf_s(qS, _T("%d"), AVal);
	return SetIniStr(AKeyName, qS);
}

// original
//template<typename Tp1, typename Tp3, typename Tp2>
//BOOL GIniFile::TouchIniInt(const TCHAR *AKeyName, Tp1* AVal, Tp3 Method, Tp2 ADeflt)
//{
//	switch (Method)
//	{
//	case WR:
//	{
//		SetIniInt(AKeyName, *AVal);
//		return true;
//	}
//	case RD:
//	{
//		*AVal = SGetIniInt(AKeyName, ADeflt);
//		return true;
//	}
//	default:
//	{
//		return false;
//	}
//	}
//}

template<typename Tp1, typename Tp3, typename Tp2>
BOOL GIniFile::TouchIniInt(const TCHAR *AKeyName, Tp1* AVal, Tp3 Method, Tp2 ADeflt)
{
	switch (Method)
	{
		case WR:
		{
			SetIniInt(AKeyName, *AVal);
			return true;
		}
		case RD:
		{
			*AVal = SGetIniInt(AKeyName, ADeflt);
			return true;
		}
		default:
		{
			return false;
		}
	}
}