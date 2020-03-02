#include "SS_FrameSaver.h"
#include "SS_FileUtils.h"
#include "../SS_Logger/Logger.h"
#include "../SS_Dll.h"


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
	SaveRawFrame(csTemp,0, ucTemp, frameSize);
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

//
//void SS_FrameSaver::Errpop(int pType)
//{
//	CSmartCriticalSection smtVectorPop(vtErrFrameMutex);
//	if (pType == FRONT)
//	{
//		//SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("pixels.front() : %x "), pixels.front());
//		delete[] Err_pixels[0];
//		Err_pixels.erase(Err_pixels.begin());
//	}
//	else
//	{
//		Err_pixels.pop_back();
//	}
//}
//bool SS_FrameSaver::isErrPaused()
//{
//	return Err_paused;
//}
//bool SS_FrameSaver::isErrEmpty()
//{
//	return Err_pixels.empty();
//}
//void SS_FrameSaver::addErrFrame(const unsigned char * pixelsToCopy, unsigned int Size)
//{
//	if (paused)
//		return;
//	BYTE *vImageFrame = new BYTE[Size];
//	memcpy
//	(
//		vImageFrame,           // destination.
//		pixelsToCopy,                                                                                           // source
//		Size              // num of bytes to copy.
//	);
//	memset((void*)pixelsToCopy, 1, Size);
//	*((unsigned int*)(vImageFrame + 4)) = Size;
//	CSmartCriticalSection smtVectorPush(vtErrFrameMutex);
//	Err_pixels.push_back(vImageFrame);
//}
//void SS_FrameSaver::saveToErrDisk(int pType)
//{
//	CString csTemp;
//	BYTE* ucTemp;
//	if (pType == FRONT)
//		ucTemp = Err_pixels.front();
//	else
//		ucTemp = Err_pixels.back();
//
//	unsigned short usTemp = *((unsigned short*)(ucTemp));
//	unsigned short usPacketIndex = *((unsigned short*)(ucTemp + 2));
//	unsigned int uiFrameSize = *((unsigned int*)(ucTemp + 4));
//	csTemp.Format(_T("%serrDark_%hd_%hd.raw"), savePath, usPacketIndex, usTemp);
//	SS_LOG((*theApp.pSSLogger), LogLevel::Info, _T("FrameSize = %d , PacketIndex = %hd Image Cnt = %hd"), uiFrameSize, usPacketIndex, usTemp);
//}