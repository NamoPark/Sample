#include "HighResolutionTimer.h"
#include <stdlib.h>


HighResolutionTimer::HighResolutionTimer()
{
    QueryPerformanceFrequency(&frequency);
    startCount.QuadPart = 0;
    endCount.QuadPart = 0;

    stopped = 0;
    startTimeInMicroSec = 0;
    endTimeInMicroSec = 0;
}

HighResolutionTimer::~HighResolutionTimer()
{
}

void HighResolutionTimer::start()
{
    stopped = 0; // reset stop flag
    QueryPerformanceCounter(&startCount);

}

void HighResolutionTimer::stop()
{
    stopped = 1; // set timer stopped flag

    QueryPerformanceCounter(&endCount);
}

double HighResolutionTimer::getElapsedTimeInMicroSec()
{
    if(!stopped)
        QueryPerformanceCounter(&endCount);

    startTimeInMicroSec = startCount.QuadPart * (1000000.0 / frequency.QuadPart);
    endTimeInMicroSec = endCount.QuadPart * (1000000.0 / frequency.QuadPart);

    return endTimeInMicroSec - startTimeInMicroSec;
}

double HighResolutionTimer::getElapsedTimeInMilliSec()
{
    return this->getElapsedTimeInMicroSec() * 0.001;
}

double HighResolutionTimer::getElapsedTimeInSec()
{
    return this->getElapsedTimeInMicroSec() * 0.000001;
}

double HighResolutionTimer::getElapsedTime()
{
    return this->getElapsedTimeInSec();
}
