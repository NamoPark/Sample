#include "SS_Timer.h"
#include <stdlib.h>


SS_Timer::SS_Timer()
{
    QueryPerformanceFrequency(&frequency);
    startCount.QuadPart = 0;
    endCount.QuadPart = 0;

    stopped = 0;
    startTimeInMicroSec = 0;
    endTimeInMicroSec = 0;
}

SS_Timer::~SS_Timer()
{
}

void SS_Timer::start()
{
    stopped = 0; // reset stop flag
    QueryPerformanceCounter(&startCount);

}

void SS_Timer::stop()
{
    stopped = 1; // set timer stopped flag

    QueryPerformanceCounter(&endCount);
}

double SS_Timer::getElapsedTimeInMicroSec()
{
    if(!stopped)
        QueryPerformanceCounter(&endCount);

    startTimeInMicroSec = startCount.QuadPart * (1000000.0 / frequency.QuadPart);
    endTimeInMicroSec = endCount.QuadPart * (1000000.0 / frequency.QuadPart);

    return endTimeInMicroSec - startTimeInMicroSec;
}

double SS_Timer::getElapsedTimeInMilliSec()
{
    return this->getElapsedTimeInMicroSec() * 0.001;
}

double SS_Timer::getElapsedTimeInSec()
{
    return this->getElapsedTimeInMicroSec() * 0.000001;
}

double SS_Timer::getElapsedTime()
{
    return this->getElapsedTimeInSec();
}
