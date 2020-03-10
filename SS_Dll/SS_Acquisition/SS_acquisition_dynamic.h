#pragma once
#include "SS_socket.h"
#include "../SS_Interface.h"
#include "../SS_Acquisition/SS_acquisition_base.h"

class Dynamic_Connection : public CConnection
{
private:
public:
	Dynamic_Connection(SS_Detector* pDetector, SWstring sAddress);
	virtual ~Dynamic_Connection();

	int m_nFrameWidth;
	int m_nFrameHeight;
	int m_nBytesToRead;
	unsigned char *m_pFrameData;


	int Execute(int atAcqType);
};

