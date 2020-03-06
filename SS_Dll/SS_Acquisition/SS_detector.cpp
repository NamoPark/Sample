#include "../SS_Dll.h"
#include "../SS_Logger/Logger.h"
#include "../SS_Interface.h"


SS_API int _stdcall ssCreateDetector(CString cstrFilePath)
{

	SS_LOG((*theApp.pSSLogger), LogLevel::Error, cstrFilePath);

	return 0;
}