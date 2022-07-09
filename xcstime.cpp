////////////////////////////////////////////////////////////////
//  xcstime.cpp
//  time file for xcs based single step problems
//  developed by AB Siddique
//  School of Engineering and Computer Science
//  Victoria University of Wellington
/////////////////////////////////////////////////////////////////
#include <string.h>
#include "xcstime.hpp"

using namespace std;

namespace xcs
{

XCSTime::XCSTime()
{

}

XCSTime::~XCSTime()
{

}

//start the timer
void XCSTime::startTimer()
{
    //get the closck time
    gettimeofday(&m_tClockStart,NULL);
    //get the cpu time
	gettimeofday(&m_tCpuStart,NULL);
	//reset the total time
	memset(&m_tCpuTotal,0,sizeof(timeval));
}

//convert the time into string and return it
string XCSTime::cnvrtToString(const timeval stTime)
{
    char buf[80];
    sprintf(buf,"%ld.%06ld ", stTime.tv_sec, stTime.tv_usec);
    return string(buf);
}

//extend the time
void XCSTime::extendTime(timeval *currTime, const timeval *addTime)
{
    currTime->tv_usec += addTime->tv_usec;
    int nsec = currTime->tv_usec / 1000000;
    currTime->tv_usec %= 1000000;
    currTime->tv_sec  += addTime->tv_sec+nsec;
}

//subtract the times and put the result in resTime. The function returns 1 if the resultant time is negative
int XCSTime::subtractTime(timeval *resTime, const timeval *xTime, const timeval *yTime)
{
    int y_tv_sec  = yTime->tv_sec;
    int y_tv_usec = yTime->tv_usec;

    //Perform the carry for the later subtraction by updating copy of yTime.
    if (xTime->tv_usec < y_tv_usec)
    {
        int nsec = (y_tv_usec - xTime->tv_usec) / 1000000 + 1;
        y_tv_usec -= 1000000 * nsec;
        y_tv_sec  += nsec;
    }
    if (xTime->tv_usec - y_tv_usec > 1000000)
    {
        int nsec = (y_tv_usec - xTime->tv_usec) / 1000000;
        y_tv_usec += 1000000 * nsec;
        y_tv_sec  -= nsec;
    }
    //Compute the time remaining to wait.
    // tv_usec is certainly positive.
    resTime->tv_sec  = xTime->tv_sec  - y_tv_sec;
    resTime->tv_usec = xTime->tv_usec - y_tv_usec;

    /* Return 1 if result is negative. */
    return xTime->tv_sec < y_tv_sec;
}

//Compute the elapsed time with respect to the given time
void XCSTime::getElapsedTime(const timeval* givenTime, timeval* elpsdTime)
{
    timeval currTime;
    gettimeofday(&currTime,NULL);
    subtractTime(elpsdTime, &currTime, givenTime);
}

//Compute the elapsed time with respect to clock start
void XCSTime::getClkElapsedTime(timeval* clkElpsdTime)
{
    timeval currTime;
    gettimeofday(&currTime,NULL);
    subtractTime(clkElpsdTime, &currTime, &m_tClockStart);
}

//Compute the elapsed time with respect to clock start and get it in a string
void XCSTime::getClkElapsedTime(string &strElpsdTime)
{
    timeval clkElpsdTime;
    getClkElapsedTime(&clkElpsdTime);
    strElpsdTime = cnvrtToString(clkElpsdTime);
}

//Compute the elapsed time with respect to CPU start time
void XCSTime::getCpuElapsedTime()
{
    timeval cpuElpsdTime;
  	getElapsedTime(&m_tCpuStart,&cpuElpsdTime);
  	extendTime(&cpuElpsdTime,&m_tCpuTotal);
}

}
