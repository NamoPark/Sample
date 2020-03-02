
#include "SystemStatus.h"
#include <string>
#include <tchar.h>
 
CSystemStatus::CSystemStatus(void)
:m_hQuery(NULL),m_hCounterCPUTotal(NULL), m_CPUCoreCount(0), m_phCounterCPUCore(nullptr),m_NetAdaptorCount(0)
{
	
}

CSystemStatus::~CSystemStatus(void)
{
}

void
CSystemStatus::Init()
{
	PdhOpenQuery(NULL, 0, &m_hQuery);

	//CPU Total
	PdhAddCounter( m_hQuery, TEXT("\\Processor(_Total)\\% Processor Time"), 1, &m_hCounterCPUTotal );

	// CPU core 정보
	SYSTEM_INFO SystemInfo = { 0 };
	GetSystemInfo(&SystemInfo);
	if (SystemInfo.dwNumberOfProcessors > 0){
		m_CPUCoreCount = SystemInfo.dwNumberOfProcessors;
		m_phCounterCPUCore = new HCOUNTER[m_CPUCoreCount];
		for (int cnt = 0; cnt < m_CPUCoreCount; cnt++){
			TCHAR szFullCounterPath[1024] = { 0 };
			wsprintf(szFullCounterPath, TEXT("\\Processor(%d)\\%% Processor Time"), cnt);
			PdhAddCounter(m_hQuery, szFullCounterPath, 1, &m_phCounterCPUCore[cnt]);
		}
	}
}

void CSystemStatus::getCPUStatus(LONG &total, LONG* arrCore, size_t arrCoreSize) {
	PDH_FMT_COUNTERVALUE PFC_Value = { 0 };
	PdhGetFormattedCounterValue(m_hCounterCPUTotal, PDH_FMT_LONG, NULL, &PFC_Value);
	total = PFC_Value.longValue;

	if (arrCore != nullptr && arrCoreSize > 0) {
		for (size_t c1 = 0; c1 < arrCoreSize; c1++){
			PDH_FMT_COUNTERVALUE PFC_Value = { 0 };
			PdhGetFormattedCounterValue(m_phCounterCPUCore[c1], PDH_FMT_LONG, NULL, &PFC_Value);
			arrCore[c1] = PFC_Value.longValue;
		}
	}
}

void CSystemStatus::getRAMStatus(int &availableMem, int &physicalMem) {
	//메모리값
	MEMORYSTATUSEX MemoryStatus = {0};
	MemoryStatus.dwLength = sizeof (MemoryStatus);
	::GlobalMemoryStatusEx(&MemoryStatus);
	
	availableMem = (int)((MemoryStatus.ullTotalPhys - MemoryStatus.ullAvailPhys)/(1024*1024));
	physicalMem =  (int)((MemoryStatus.ullTotalPhys)/(1024*1024));
}

void CSystemStatus::getNETStatus(LONG *adapTotalByte, size_t adaptorCount) {
	////네트워크
	PDH_FMT_COUNTERVALUE PFC_Value = { 0 };

	for (int cnt = 0; cnt < adaptorCount; cnt++) {
		PdhGetFormattedCounterValue(m_phCounterNetAdaptor[cnt], PDH_FMT_LONG, NULL, &PFC_Value);
		adapTotalByte[cnt] = PFC_Value.longValue;
	}
}

void CSystemStatus::Update(){
	// 카운트 갱신
	PdhCollectQueryData( m_hQuery );
}

void CSystemStatus::Terminate(){

	PdhRemoveCounter(m_hCounterCPUTotal);
	
	for (int cnt = 0; cnt < m_CPUCoreCount; cnt++)
		PdhRemoveCounter(m_phCounterCPUCore[cnt]);
	delete(m_phCounterCPUCore);
}