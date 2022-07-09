////////////////////////////////////////////////////////////////
//  xcssystem.hpp
//  xcssystem file for xcs based single step problems
//  developed by AB Siddique
//  School of Engineering and Computer Science
//  Victoria University of Wellington
/////////////////////////////////////////////////////////////////
#ifndef XCSSYSTEM_HPP__
#define XCSSYSTEM_HPP__
#include "xcstime.hpp"
#include "xcsclassifierharness.hpp"
#include "xcsenvironment.hpp"


namespace xcs
{

class xcsSys
{
//functions
public:
    xcsSys();
    ~xcsSys();

    void startXCS();
    void singleStepExp();
    void singleStepExplore(OPTYPE pOpState[], int nInsts);
    void singleStepExploit(OPTYPE pOpState[], int nInsts, int arrCorrect[], double arrSysErr[]);
    void writePerformance(int arrCorrect[], double arrSysErr[], int nInsts, ClfrSetType eClfrSetType, LogFileType eFileType);
    void expExit(LogFileType eFileType);
    void logHeader();
//variables
private:
    XCSTime             m_tXcsTime;
    env                 m_stEnv;
    clfrharness         m_stClfrHrns;
};

} //end namespace xcs

#endif //XCSSYSTEM_HPP__
