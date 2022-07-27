////////////////////////////////////////////////////////////////
//  xcsmain.cpp
//  main file for xcs based single step problems
//  developed by AB Siddique
//  School of Engineering and Computer Science
//  Victoria University of Wellington
/////////////////////////////////////////////////////////////////
#ifndef XCSHARNESS_HPP__
#define XCSHARNESS_HPP__
#include "xcssystem.hpp"

using namespace xcs;
using namespace std;

//main function
int main(int argc, char** argv)
{
    //xcsSys  stXcsSys;
    //Update parameters
    m_stGLoadConfig.parseargs(argc, argv);
    if(!m_stGLoadConfig.m_strConfigFile.empty())
        m_stGLoadConfig.loadConfigFromFile(m_stGLoadConfig.m_strConfigFile);

    xcsSys  stXcsSys;
    //Runs experiment for multiple time
    for(uint32_t nRuns=0; nRuns < m_stGLogExec.m_nTotalRuns; nRuns++)
    {
        //set different seed for each run
        setSeed(arrSeeds[nRuns]);
        printf("Experiment: %d\n",nRuns+1);
        //Init log file paths for the current session
        m_stGLogExec.setLogFileNames(nRuns);
        //open the log files
        m_stGLogExec.openLogFiles();
        stXcsSys.startXCS();
        printf("Done\n");
		stXcsSys.expExit(PERFORMANCE);
		//close all the log files
        m_stGLogExec.closeLogFiles();

     }

    return 0;
}


#endif // XCSHARNESS_HPP__

