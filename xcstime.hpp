////////////////////////////////////////////////////////////////
//  xcstime.hpp
//  time file for xcs based single step problems
//  developed by AB Siddique
//  School of Engineering and Computer Science
//  Victoria University of Wellington
/////////////////////////////////////////////////////////////////
#ifndef XCSTIME_HPP__
#define XCSTIME_HPP__
#include <sys/time.h>
#include <stddef.h>
#include <string>
#include<stdio.h>

using namespace std;

namespace xcs
{

class XCSTime
{
public:
    XCSTime();
    ~XCSTime();

    //start the clock and cpu time, and reset the total time
    void startTimer();
    //convert the time into string and return it
    string cnvrtToString(const timeval stTime);
    //extend the time
    void extendTime(timeval *currTime, const timeval *addTime);
    //subtract the times and put the result in resTime. The function returns 1 if the resultant time is negative
    int subtractTime(timeval *resTime, const timeval *xTime, const timeval *yTime);
    //Compute the elapsed time with respect to the given time
    void getElapsedTime(const timeval* givenTime, timeval* elpsdTime);
    //Compute the elapsed time with respect to clock start
    void getClkElapsedTime(timeval* clkElpsdTime);
    //Compute the elapsed time with respect to clock start and get it in a string
    void getClkElapsedTime(string &strElpsdTime);
    //Compute the elapsed time with respect to CPU start time
    void getCpuElapsedTime();


private:
    timeval     m_tClockStart;
    timeval     m_tCpuStart;
    timeval     m_tCpuTotal;

};

} //~end namespace xcs

#endif //XCSTIME_HPP__
