#pragma once

typedef void(_stdcall *tDLIB_CBProc)(tDLIB_CBRec*);

typedef struct _tPacket tPacket;
typedef struct _tAcqPar tAcqPar;

tDLIB_CBRec* AD_Connect(int AFlags, tDLIB_CBProc ACallBackProc,
	void* AUserCallBackProc, void* AUserParam, UINT16* AFrameBuffer);

int AD_Init(tAcqPar* argAcqPar);
bool ACQ_SendPacket(tPacket* Pkt);
void i_Feedback(int AType, int AEvent, int ASocket, const TCHAR *AMsg, void *APacketData);
void ACQ_Close();
BOOL ACQ_Abort();

class SS_Caputre
{
public:
	SS_Caputre();
	~SS_Caputre();
};

