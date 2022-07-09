////////////////////////////////////////////////////////////////
//  xcsenvironment.cpp
//  environment file for xcs based single step problems
//  developed by AB Siddique
//  School of Engineering and Computer Science
//  Victoria University of Wellington
/////////////////////////////////////////////////////////////////
#include "xcsenvironment.hpp"
#include <math.h>
#include "xcsconfig.hpp"

namespace xcs
{

env::env()
{
    m_pnPosRlvntBits = new int[m_stGEnvConfig.m_nRlvntBits];
    for(int i=0; i<m_stGEnvConfig.m_nRlvntBits; i++)
        m_pnPosRlvntBits[i]=i;
}

env::~env()
{
    delete []m_pnPosRlvntBits;
}

bool env::isDV1Term(int nNum)
{
    for(int i=0; i<nSizeDV1; i++)
    {
		if(nNum == DV1[i]){
			return true;
		}
	}
	return false;
}

void env::resetState(OPTYPE pOpState[])
{
    //Generates a new random problem instance.
	for(int i=0; i<m_stGEnvConfig.m_nCondLen; i++)
	{
		if(fRand()<0.5){
			pOpState[i]=0;
		}
		else{
			pOpState[i]=1;
		}
	}
}

// Executes the action and determines the reward.
double env::prfmAction(int nAction, OPTYPE pOpState[], bool &bReset)
{
    switch(m_stGEnvConfig.m_eEnv)
    {
		case evenParity:
			return prfmAction(nAction,pOpState,bReset);
		case carry:
			return prfmCarryAction(nAction,pOpState,bReset);
        case multiplexer:
            return prfmMuxAction(nAction,pOpState,bReset);
        case hiddenEvenParity:
            return prfmHiddenEvenParityAction(nAction,pOpState,bReset);
        case hiddenOddParity:
            return prfmHiddenOddParityAction(nAction,pOpState,bReset);
        case countOnes:
            return prfmCountOnesAction(nAction,pOpState,bReset);
		case majorityOn:
            return prfmMajorityOnAction(nAction,pOpState,bReset);
		case dv1:
            return prfmDV1Action(nAction,pOpState,bReset);
        default:
            printf("\nEnvironment not supported!\n");
            exit(0);
    }
}

double env::evlAction(int nAction, int nGenunAct, bool &bReset)
{
    int nRes=0;
    if(nAction == nGenunAct)
	{
	    bReset=true;
	    nRes = m_stGEnvConfig.m_nMaxPayoff;
	}
	else
	{
	    bReset=false;
	    nRes = 0;
	}

	return (double)nRes;
}

double env::prfmEvnParityAction(int nAction, OPTYPE pOpState[], bool &bReset)
{
    int nGenunAct = 0;
	int nNumOnes = 0;

	for(int i=0; i<m_stGEnvConfig.m_nCondLen; i++)
	{
		if(pOpState[i] == 1)
			nNumOnes++;
	}

	if (nNumOnes%2 == 0)
		nGenunAct = 1;

    return evlAction(nAction, nGenunAct, bReset);
}

double env::prfmCarryAction(int nAction, OPTYPE pOpState[], bool &bReset)
{
    int nCarry = 0;
	int nHalfCondLen = m_stGEnvConfig.m_nCondLen/2;

	for(int i=nHalfCondLen-1; i>=0; i--)
	{
        //need to check it carefully
		nCarry = ((pOpState[i]-0) + (pOpState[i+nHalfCondLen]-0) + nCarry)/2;
	}

    return evlAction(nAction, nCarry, bReset);
}

double env::prfmMuxAction(int nAction, OPTYPE pOpState[], bool &bReset)
{
    int nPlace=m_stGEnvConfig.m_nPosBits;

	for(int i=0; i<m_stGEnvConfig.m_nPosBits; i++)
	{
	    if(pOpState[i]==1)
			nPlace += (int)pow(2.0, (double)(m_stGEnvConfig.m_nPosBits-1-i));

	}

    int nGenunAct = pOpState[nPlace];
    return evlAction(nAction, nGenunAct, bReset);
}

double env::prfmHiddenEvenParityAction(int nAction, OPTYPE pOpState[], bool &bReset)
{
    int nGenunAct = 0;
	int nNumOnes = 0;

	for(int i=0; i<m_stGEnvConfig.m_nRlvntBits; i++)
	{
		if(pOpState[m_pnPosRlvntBits[i]] == '1')
			nNumOnes++;

	}
	if (nNumOnes%2 == 0)
		nGenunAct = 1;

	return evlAction(nAction, nGenunAct, bReset);
}

double env::prfmHiddenOddParityAction(int nAction, OPTYPE pOpState[], bool &bReset)
{
    int nGenunAct = 0;
	int nNumOnes = 0;

	for(int i=0; i<m_stGEnvConfig.m_nRlvntBits; i++)
	{
		if(pOpState[m_pnPosRlvntBits[i]] == '1')
			nNumOnes++;

	}
	if (nNumOnes%2 == 0)
		nGenunAct = 1;

	return evlAction(nAction, nGenunAct, bReset);
}

double env::prfmCountOnesAction(int nAction, OPTYPE pOpState[], bool &bReset)
{
    int nGenunAct = 0;
	int nNumOnes = 0;

	for(int i=0; i<m_stGEnvConfig.m_nRlvntBits; i++)
	{
		if(pOpState[m_pnPosRlvntBits[i]] == 1)
			nNumOnes++;

	}
	if (nNumOnes > m_stGEnvConfig.m_nRlvntBits/2)
		nGenunAct = 1;

	return evlAction(nAction, nGenunAct, bReset);
}
double env::prfmMajorityOnAction(int nAction, OPTYPE pOpState[], bool &bReset)
{
    int nGenunAct = 0;
	int nNumOnes = 0;

	for(int i=0; i<m_stGEnvConfig.m_nCondLen; i++)
	{
		if(pOpState[i] == 1)
			nNumOnes++;

	}

	if (nNumOnes > m_stGEnvConfig.m_nCondLen/2)
		nGenunAct = 1;

    return evlAction(nAction, nGenunAct, bReset);
}
double env::prfmDV1Action(int nAction, OPTYPE pOpState[], bool &bReset)
{
    int nGenunAct = 0;
	int nNumOnes = 0;
	int p = 0;

	for(int i=m_stGEnvConfig.m_nCondLen-1; i>=0; i--)
	{
        //need to check it carefully
		nNumOnes += (pOpState[i]-0)*pow(2,p);
		p++;
	}
	if (isDV1Term(nNumOnes))
		nGenunAct = 1;

	return evlAction(nAction, nGenunAct, bReset);
}


} //end namespace xcs
