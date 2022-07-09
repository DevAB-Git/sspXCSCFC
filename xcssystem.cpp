////////////////////////////////////////////////////////////////
//  xcssystem.cpp
//  xcssystem file for xcs based single step problems
//  developed by AB Siddique
//  School of Engineering and Computer Science
//  Victoria University of Wellington
/////////////////////////////////////////////////////////////////
#include "xcssystem.hpp"
#include <string.h>

namespace xcs
{

xcsSys::xcsSys()
{

}

xcsSys::~xcsSys()
{

}

void xcsSys::startXCS()
{
    m_tXcsTime.startTimer();
    m_stClfrHrns.initClassifierSets();
    //Status
    printf("XCS is busy! Please wait ....\n");
    singleStepExp();
    //temp chg AB
    m_stClfrHrns.simplifyPopulation();
    //Sort and print classifier set
    m_stClfrHrns.sortClassifierSetAndUpdatePtr(POPSET, 2);
    m_stClfrHrns.printClassifierSet(POPSET, m_stGLogExec.getFilePtr(WHOLECLSPOP));
    //Free pop set
    m_stClfrHrns.freeClassifierSet(POPSET);
    //Free the CF Data
    //m_stClfrHrns.freeCFData();
}

//Executes one single-step experiment and monitor the performance.
void xcsSys::singleStepExp()
{
    int nExplore=0;
    OPTYPE pOpState[m_stGEnvConfig.m_nCondLen];
    int arrCorrect[m_stGLogExec.m_nTestFrequency];
    double arrSysErr[m_stGLogExec.m_nTestFrequency];
    //run the program for total number of instances
    for(int nExplrInst=0; nExplrInst <= m_stGEnvConfig.m_nMaxProblems; nExplrInst+=nExplore)
    {
		nExplore = (nExplore+1)%2;
		m_stEnv.resetState(pOpState);

		if(nExplore==1)
            singleStepExplore(pOpState,nExplrInst);
		else
            singleStepExploit(pOpState,nExplrInst, arrCorrect, arrSysErr);

		if(nExplrInst%m_stGLogExec.m_nTestFrequency==0 && nExplore==0 && nExplrInst>0)
			writePerformance(arrCorrect, arrSysErr, nExplrInst, POPSET, PERFORMANCE);
	}

}

// Executes one main learning loop for a single step problem.
void xcsSys::singleStepExplore(OPTYPE pOpState[], int nInsts)
{
    bool bWasCorrect = false;
	m_stClfrHrns.getMatchSet(pOpState,nInsts);
	m_stClfrHrns.freeSet(KILLSET);

	m_stClfrHrns.getPredictArr();
	int nActWinner = m_stClfrHrns.randActionWinner();
	m_stClfrHrns.getActionSet(nActWinner);
	double fReward = m_stEnv.prfmAction(nActWinner,pOpState,bWasCorrect);

	m_stClfrHrns.updateActionSet(0.0,fReward);
	m_stClfrHrns.freeSet(KILLSET);

	m_stClfrHrns.discoveryComponent(m_stClfrHrns.getActSet(),nInsts,pOpState);
	m_stClfrHrns.freeSet(KILLSET);

	m_stClfrHrns.freeSet(MSET);
	m_stClfrHrns.freeSet(ACTSET);

}

void xcsSys::singleStepExploit(OPTYPE pOpState[], int nInsts, int arrCorrect[], double arrSysErr[])
{
    bool bWasCorrect = false;

    m_stClfrHrns.getMatchSet(pOpState,nInsts);
	m_stClfrHrns.freeSet(KILLSET);

	m_stClfrHrns.getPredictArr();
	int nActWinner = m_stClfrHrns.bestActionWinner();
	double fReward = m_stEnv.prfmAction(nActWinner,pOpState,bWasCorrect);

	if(bWasCorrect)
		arrCorrect[nInsts%m_stGLogExec.m_nTestFrequency]=1;
	else
		arrCorrect[nInsts%m_stGLogExec.m_nTestFrequency]=0;

    arrSysErr[nInsts%m_stGLogExec.m_nTestFrequency] = absValue(fReward - m_stClfrHrns.getBestValue());

	m_stClfrHrns.freeSet(MSET);
}

void xcsSys::writePerformance(int arrCorrect[], double arrSysErr[], int nInsts, ClfrSetType eClfrSetType, LogFileType eFileType)
{
    char pczBuf[100];
	double fPerf=0.0, fSysErr=0.0;
	FILE* fWriteFile = m_stGLogExec.getFilePtr(eFileType);

    if(!fWriteFile)
    {
        printf("\nError in opening the write file ");
		exit(1);
    }

	for(uint32_t i=0; i<m_stGLogExec.m_nTestFrequency; i++)
	{
		fPerf+=arrCorrect[i];
        fSysErr+=arrSysErr[i];
	}

	fPerf/=m_stGLogExec.m_nTestFrequency;
    fSysErr/=m_stGLogExec.m_nTestFrequency;

	int nSetSize = m_stClfrHrns.getSetSize(eClfrSetType);

    sprintf(pczBuf,"%d ",nInsts); fwrite(pczBuf,strlen(pczBuf),1,fWriteFile);
	sprintf(pczBuf,"%f ",fPerf); fwrite(pczBuf,strlen(pczBuf),1,fWriteFile);
	sprintf(pczBuf,"%f ",fSysErr); fwrite(pczBuf,strlen(pczBuf),1,fWriteFile);
	sprintf(pczBuf,"%d ",nSetSize); fwrite(pczBuf,strlen(pczBuf),1,fWriteFile);
	fwrite("\n",strlen("\n"),1,fWriteFile);

	//int numerositySum = getNumerositySum(population); printf("%d %f %f %d %d\n",exploreProbC,perf,serr,setSize,numerositySum);
	//printf("%d %f %f %d\n",exploreProbC,perf,serr,setSize);
}

void xcsSys::expExit(LogFileType eFileType)
{
    printf("Shutting down...\n");
	m_tXcsTime.getCpuElapsedTime();
   	logHeader();

	//compute elapsed time
	string strTime;
	m_tXcsTime.getClkElapsedTime(strTime);
	FILE* fWriteFile = m_stGLogExec.getFilePtr(eFileType);
	fwrite(strTime.c_str(),strlen(strTime.c_str()),1,fWriteFile);
	fwrite("\n",strlen("\n"),1,fWriteFile);

	printf("Elapsed Time: %s", strTime.c_str());
	printf(" Secs\n\n");
}

void xcsSys::logHeader()
{
    time_t t;
    //char hostName[1024]; //[MAXHOSTNAMELEN];
    time(&t);
    //comiplation error only at KDE & code block
    //gethostname(hostName,sizeof(hostName));
    //printf("%s\nSeed: %ld\n%s",hostName,getSeed(),ctime(&t));
    printf("%s\nSeed: %ld\n%s","ABSystem",getSeed(),ctime(&t));
}

}//~end namespace xcs
