#pragma once
#include "stdafx.h"
#include "SS_sync.h"
#include "SystemStatus.h"
#include <vector>
using namespace std;
#define FRONT 0
#define BACK 1

class SS_FrameSaver
{
public	:
	SS_FrameSaver				();
	~SS_FrameSaver				();

	void setup					(int height,int width,int pixelSize,CString csPath);
	void addFrame				();
	void saveToDisk				(int pType);
	void setPause				(bool value);
	void pop					(int pType);
	bool isPaused				();
	bool isEmpty				();
	BYTE * getFrame				();
	BYTE * getReturnFrame();
	//void Errpop(int pType);
	//bool isErrPaused();
	//bool isErrEmpty();
	//void saveToErrDisk(int pType);	
	//void addErrFrame(const unsigned char* pixelsToCopy, unsigned int Size);


private	:
	bool						paused;
	bool						Err_paused;
	int							imageH;
	int							imageW;
	int							frameSize;
	BYTE *						vImageFrame;
	CString						savePath;
	vector<unsigned char*>		pixels;
	//vector<unsigned char*>		Err_pixels;
	CCriticalSection			vtFrameMutex;
	CCriticalSection			vtErrFrameMutex;
	CSystemStatus sStatus;


	CString m_strOutput;
	size_t	cpuCount = 0;
	LONG	*pArrayCore = nullptr;

	//size_t	netAdaptorCount = 0;
	//LONG	*pArrayNetValue = nullptr;

};

