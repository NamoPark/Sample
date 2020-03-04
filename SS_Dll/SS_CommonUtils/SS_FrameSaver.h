#pragma once
#include "../stdafx.h"
#include "SS_sync.h"
#include "SS_SystemStatus.h"
#include "opencv2/opencv.hpp"
#include <vector>

using namespace std;
using namespace cv; 

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
	void oc_addFrame();
	void oc_saveToDisk(int pType);
	void oc_pop(int pType);
	bool oc_isEmpty();
	BYTE * oc_getFrame();
	BYTE * oc_getReturnFrame();
private	:
	bool						paused;
	bool						Err_paused;
	int							imageH;
	int							imageW;
	int							frameSize;
	BYTE *						vImageFrame;
	CString						savePath;
	vector<unsigned char*>		pixels;
	CCriticalSection			vtFrameMutex;
	
	//vector<unsigned char*>	Err_pixels;
	//CCriticalSection			vtErrFrameMutex;

	Mat							*ptrMat;
	vector<Mat*>				vqMat;
};

