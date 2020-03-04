#include "SS_FrameSaver.h"
#include "SS_FileUtils.h"
#include "../SS_Logger/Logger.h"
#include "../SS_Dll.h"
#include "SS_StringDefines.h"


SS_FrameSaver::SS_FrameSaver() //: mtVector(_T("FrameSaveVector"))
{
	paused = 0;
	Err_paused = 0;
	savePath = _T("");
	imageH = 0;
	imageW = 0;
	frameSize = 0;
	vImageFrame =NULL;
}


SS_FrameSaver::~SS_FrameSaver()
{
	for (int i = 0; i < pixels.size(); i++)
	{
		delete[] pixels[i];
	}
	pixels.clear();
	delete[] vImageFrame;

}

void SS_FrameSaver::setup(int height, int width, int pixelSize, CString csPath)
{
	paused			=	0;
	Err_paused		=	0;
	savePath		=	csPath;
	imageH			=	height;
	imageW			=	width;
	frameSize		=	imageH * imageW * pixelSize;
	MakeAllDirectory(CTOW(savePath));
}

void SS_FrameSaver::addFrame()
{
	if (paused)
		return;
	CSmartCriticalSection smtVectorPush(vtFrameMutex);
	pixels.push_back(vImageFrame);
}



void SS_FrameSaver::saveToDisk(int pType)
{
	CString csTemp;
	BYTE* ucTemp;
	if(pType == FRONT)
		ucTemp = pixels.front();
	else
		ucTemp = pixels.back();

	//System Log
	unsigned short usTemp = *((unsigned short*)(ucTemp));
	unsigned short usPacketIndex = *((unsigned short*)(ucTemp+2));
	csTemp.Format(_T("%scDark_%hd_%hd.raw"), savePath, usPacketIndex, usTemp);
	SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("Size = %d , PacketIndex = %hd Image Cnt = %hd"), pixels.size(), usPacketIndex, usTemp);

	//SaveRawFrame(csTemp,0, ucTemp, frameSize);
	SaveToFile((unsigned char*)ucTemp, frameSize, CTOW(csTemp));
}



void SS_FrameSaver::setPause(bool value)
{
	paused = value;
}

void SS_FrameSaver::pop(int pType)
{
	CSmartCriticalSection smtVectorPop(vtFrameMutex);
	if (pType == FRONT)
	{
		delete[] pixels[0];
		pixels.erase(pixels.begin());
	}
	else
	{
		pixels.pop_back();
	}
}

bool SS_FrameSaver::isPaused()
{
	return paused;
}


bool SS_FrameSaver::isEmpty()
{
	return pixels.empty();
}

BYTE * SS_FrameSaver::getFrame()
{
	if (paused)
		return vImageFrame;

	if (frameSize != 0) 
	{
		vImageFrame = new BYTE[frameSize];
		memset(vImageFrame, 0, frameSize);
		return vImageFrame;
	}
	else
	{
		SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("frameSize = 0"));
		return NULL;
	}

}

BYTE * SS_FrameSaver::getReturnFrame()
{
	if (vImageFrame != NULL) 
	{
		return vImageFrame;
	}
	else
	{
		SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("vImageFrame = NullPtr"));
		return NULL;
	}

}


///////////////////////////////////////////
//OpenCV
void SS_FrameSaver::oc_addFrame()
{
	if (paused)
		return;
	CSmartCriticalSection smtVectorPush(vtFrameMutex);
	vqMat.push_back(ptrMat);
}

void SS_FrameSaver::oc_saveToDisk(int pType)
{
	CString csTemp;
	Mat* ucTemp;
	if (pType == FRONT)
		ucTemp = vqMat.front();
	else
		ucTemp = vqMat.back();

	//System Log

	unsigned short usImageCount = *((unsigned short*)(ucTemp->data));
	unsigned short usPacketIndex = *((unsigned short*)(ucTemp->data + 2));
	csTemp.Format(_T("%scDark_%hd_%hd.raw"), savePath, usPacketIndex, usImageCount);
	SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("Size = %d , PacketIndex = %hd Image Cnt = %hd"), vqMat.size(), usPacketIndex, usImageCount);
	String stemp = CTOS(csTemp);
	imwrite(stemp, *(vqMat.front()));

	SaveToFile((unsigned char*)ucTemp->data, frameSize, CTOW(csTemp));
}

void SS_FrameSaver::oc_pop(int pType)
{
	CSmartCriticalSection smtVectorPop(vtFrameMutex);
	if (pType == FRONT)
	{
		delete vqMat[0];
		vqMat.erase(vqMat.begin());
	}
	else
	{
		vqMat.pop_back();
	}
}

bool SS_FrameSaver::oc_isEmpty()
{
	return vqMat.empty();
}

BYTE * SS_FrameSaver::oc_getFrame()
{
	if (paused)
		return ptrMat->data;

	if (frameSize != 0)
	{
		ptrMat = new Mat(imageH, imageW, CV_8UC2);
		return  ptrMat->data;
	}
	else
	{
		SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("frameSize = 0"));
		return NULL;
	}
}

BYTE * SS_FrameSaver::oc_getReturnFrame()
{
	if (ptrMat->data != NULL)
	{
		return ptrMat->data;
	}
	else
	{
		SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("vImageFrame = NullPtr"));
		return NULL;
	}
}

