////////////////////////////////////////////////////////////////
//  xcscodefragmentharness.cpp
//  xcscodefragement file for xcs based single step problems
//  developed by AB Siddique
//  School of Engineering and Computer Science
//  Victoria University of Wellington
/////////////////////////////////////////////////////////////////
#include "xcscodefragmentharness.hpp"
#include <string.h>
#include <cassert>
//#include "xcsclassifierharness.hpp"

namespace xcs
{

cfharness::cfharness()
{

    m_nPrevCFs = 0;
    m_nStPrevCFId = 0;
    m_pstPrevCFPop = NULL;
    m_pstOpDataSet = NULL;

    //const CodeFragment dontcareCF = {{0,0,NOT,OR,NOP,NOP,NOP,NOP},-1};
    //dontcareCF: it is just for completeness, not evaluated at all.
    //It's output value is always 1.
    m_stDontCareCF.m_pOpData = new OPTYPE[8];
    m_stDontCareCF.m_nID = -1;
    m_stDontCareCF.m_pOpData[0]= 0;
    m_stDontCareCF.m_pOpData[1]= 0;
    m_stDontCareCF.m_pOpData[2]= NOT;
    m_stDontCareCF.m_pOpData[3]= OR;
    m_stDontCareCF.m_pOpData[4]= NOP;
    m_stDontCareCF.m_pOpData[5]= NOP;
    m_stDontCareCF.m_pOpData[6]= NOP;
    m_stDontCareCF.m_pOpData[7]= NOP;
}

cfharness::~cfharness()
{
    delete []m_stDontCareCF.m_pOpData;
}

void cfharness::loadPrevCFPop()
{
   	//Load CF pop from previous level
	int nLineSize = 1000; // Enough to cover spaces in the CF
	char pczTempBuf[nLineSize];
	codeFragment prevCF;
	int nReadPrevCFs = 0;
	//temp chg AB 07/09/2022
	FILE* pfPrevCFPop = m_stGLogExec.getFilePtr(PREVCFPOP);

	if( fgets(pczTempBuf,nLineSize,pfPrevCFPop) !=NULL )
	{
        //first line is total number of CFs
        // maximum number of stored CFs
		m_nPrevCFs = atoi(pczTempBuf);
		//printf("\n%d\n",m_nPrevCFs);
		m_pstPrevCFPop = new codeFragment[m_nPrevCFs];
	}
	else
	{
		printf("\nError in reading previous CFs.\n");
		exit(0);
	}
	//second line is the starting ID number of previous CFs
	if( fgets(pczTempBuf,nLineSize,pfPrevCFPop) !=NULL )
	{
		m_nStPrevCFId = atoi(pczTempBuf);
		//printf("\n%d\n",m_nStPrevCFId);
	}
	else
	{
		printf("\nError in reading previous CFs.\n");
		exit(0);
	}

	//Read each line from the file of previous CFs population
	while(fgets(pczTempBuf, nLineSize, pfPrevCFPop) != NULL)
	{

		// Strip trailing '\n' if it exists
		int nLen = strlen(pczTempBuf)-1;
		if(pczTempBuf[nLen] == '\n')
            pczTempBuf[nLen] = 0;
		//printf("\n%s",pczTempBuf);

		char *pczTemp = NULL;
		OPTYPE cfOp;
		int i = 0;
		// start CFs ID numbers from condLength to avoid confusion with CFs and condition bits
		prevCF = createNewCF(nReadPrevCFs+m_stGEnvConfig.m_nCondLen);
		//Add to OpDataSet to delete memory at the end of prog
        addOpData(prevCF.m_pOpData);

		//tokenize the pczTempBuf
		pczTemp = strtok(pczTempBuf," ");
		while (pczTemp != NULL)
		{
			cfOp = getOpType(pczTemp);
			prevCF.m_pOpData[i++] = cfOp;
			if(cfOp == NOP)
			{
				if(!isExist(prevCF,m_pstPrevCFPop,nReadPrevCFs))
				{//insert the code fragment
					//printf("\n%s",pczTempBuf);
					//printCF(prevCF);
					memmove(&m_pstPrevCFPop[nReadPrevCFs],&prevCF,sizeof(codeFragment));
					nReadPrevCFs++;
				}
				break;
			}
			pczTemp = strtok (NULL, " ");
		} //end while pczTemp

	} //end while fgets
	m_nPrevCFs = nReadPrevCFs; //number of distint CFs
	//printf("\n%d\n",m_nPrevCFs);
	//fclose(pfPrevCFPop); //close
	//exit(0);

}

codeFragment cfharness::createNewCF(int nId)
{
    codeFragment newCF;
    newCF.m_pOpData = new OPTYPE[m_stGEnvConfig.m_nCfMaxLen];

	for(int i=0; i<m_stGEnvConfig.m_nCfMaxLen; i++)
		newCF.m_pOpData[i] = NOP;

	newCF.m_nID = nId;
	return newCF;
}

OPTYPE cfharness::getOpType(char pczBuf[])
{
	OPTYPE opRet;
	if(strcmp(pczBuf,"o")==0)
        return NOP;
	if(strcmp(pczBuf,"&")==0)
        return AND;
	if(strcmp(pczBuf,"|")==0)
        return OR;
	if(strcmp(pczBuf,"d")==0)
        return NAND;
	if(strcmp(pczBuf,"r")==0)
        return NOR;
	if(strcmp(pczBuf,"~")==0)
        return NOT;

	//D0, D1, D2, etc
	if(pczBuf[0] == 'D')
	opRet = atoi(pczBuf+1);
	//CF from a previous level
	else if(pczBuf[0] == 'C')
		opRet = atoi(pczBuf+3) + (m_stGEnvConfig.m_nCondLen - m_nStPrevCFId);
	else
		{
            printf("\nError in getOpType ... \n");
            exit(0);
        }

	return opRet;
}

bool cfharness::isExist(codeFragment newCF, codeFragment pstCFPop[], int nTotalCFs)
{
	for(int i=0; i<nTotalCFs; i++)
	{
		if(isEqualCFs(newCF,pstCFPop[i]))
			return true;
	}
	return false;
}

bool cfharness::isEqualCFs(codeFragment stCF1, codeFragment stCF2)
{
	for(int i=0; i<m_stGEnvConfig.m_nCfMaxLen; i++)
	{
		if(stCF1.m_pOpData[i] != stCF2.m_pOpData[i])
			return false;
	}
	return true;
}

//int cfharness::getNumPrevCFs()
//{
//    return m_nPrevCFs;
//}

bool cfharness::isDontCareCF(codeFragment stCF)
{
    return (stCF.m_nID == -1) ? true : false;
}

int cfharness::getNumSpecificCF(codeFragment stArrCond[])
{
    //returns the number of specific CFs in cond
	int nTotal=0;
	for(int i=0; i<m_stGEnvConfig.m_nCondLen; i++)
	{
		if(!isDontCareCF(stArrCond[i]))
			nTotal++;
	}
	return nTotal;
}

void cfharness::printCF(codeFragment stCF, FILE *pFile)
{
    if(pFile)
    {
        char pczBuf[1000];

        sprintf(pczBuf,"Printing CF ...... ");
        fwrite(pczBuf,strlen(pczBuf),1,pFile);

        char pczTempBuf[1000];

        for(int j = 0; j<m_stGEnvConfig.m_nCfMaxLen; j++)
        {
            getOpChar(stCF.m_pOpData[j], pczTempBuf);
            sprintf(pczBuf,"%s",pczTempBuf);
            fwrite(pczBuf,strlen(pczBuf),1,pFile);

            if(stCF.m_pOpData[j]==NOP)
                break; //reduce size of output
        }
        sprintf(pczBuf," ------> %d\n",stCF.m_nID);
        fwrite(pczBuf,strlen(pczBuf),1,pFile);
    }
    else
    {
        printf("Printing CF ...... ");
        char pczTempBuf[1000];
        for(int j = 0; j<m_stGEnvConfig.m_nCfMaxLen; j++)
        {
            getOpChar(stCF.m_pOpData[j], pczTempBuf);
            printf("%s",pczTempBuf);
            if(stCF.m_pOpData[j]==NOP)
                break; //reduce size of output
        }
        printf(" ------> %d",stCF.m_nID);
        printf("\n");

    }
}

void cfharness::getOpChar(const OPTYPE opCode, char pczTempBuf[])
{
    if(validLeaf(opCode)>=0)
	{
        getLeafName(opCode, pczTempBuf);
	}
	else
	{
        string strTemp;
        switch(opCode)
        {
            case AND:
                strTemp = "& ";
            break;
            case OR:
                strTemp = "| ";
            break;
            case NAND:
                strTemp = "d ";
            break;
            case NOR:
                strTemp = "r ";
            break;
            case NOT:
                strTemp = "~ ";
            break;
            case NOP:
                strTemp = "o ";
            break;
            default:
                strTemp = "[%d!!]";
                //sprintf(pczTempBuf,"[%d!!]",opCode);
        }//end switch code
        memcpy(pczTempBuf, strTemp.c_str(), strTemp.length());
	}


}

int cfharness::validLeaf(const OPTYPE opCode) {
	if( 0<=opCode && opCode<m_stGEnvConfig.m_nCondLen )
		return opCode;

	if(isPrevLevelsCode(opCode))
		return opCode;

	return -1;
}

bool cfharness::isPrevLevelsCode(const OPTYPE opCode)
{
    if(m_stGEnvConfig.m_nCurrProbLevel > 1)
    {
		return ( m_pstPrevCFPop[0].m_nID<=opCode && opCode<(m_pstPrevCFPop[0].m_nID + m_nPrevCFs) ) ? true : false;
	}
	return false;
}

void cfharness::getLeafName(const OPTYPE opCode, char pczTempBuf[])
{
    if(0<=opCode && opCode<m_stGEnvConfig.m_nCondLen)
	{
		sprintf(pczTempBuf,"D%d ",validLeaf(opCode));
    }
	else if(isPrevLevelsCode(opCode))
	{
		sprintf(pczTempBuf,"CF_%d ",validLeaf(opCode));
	}
	else{
		printf("\nERROR! invlalid leaf name\n");
		exit(0);
	}
}


void cfharness::ValidDepth(OPTYPE* opCF, OPTYPE* opEnd)
{
	//display 'cf' for debugging
	/*
	int i=0;
	char* temp = NULL;
	while(opCF[i]!=NOP){
		temp = opchar(opCF[i++]);
		printf("%s ",temp);
	}
	*/
	const OPTYPE* opStart = opCF;
	OPTYPE* opP = opEnd -1;
	int nMaxArg =1;
	int nDepth = -1;
	DepthMax(opStart,&opP,nMaxArg,nDepth);
	//printf("Depth: %d\n",nDepth);
	assert(nDepth<=m_stGEnvConfig.m_nCfMaxDepth);
}

void cfharness::DepthMax(const OPTYPE* const opEnd,OPTYPE** opProg, int& nArgsToGo, int& nDepth)
{
  if(*opProg<opEnd || nArgsToGo<=0)
  return;
  const int nArgs = getNumArgs(*opProg[0]);
  nArgsToGo += nArgs-1;
  *opProg = (*opProg-1);
  nDepth++;
  const int d0 = nDepth;
  for(int i=0;i<nArgs;i++)
  {
    int d1 = d0;
    DepthMax(opEnd,opProg,nArgsToGo,d1);
    nDepth = (nDepth>d1)? nDepth : d1; //max(nDepth,d1);
  }
}

int cfharness::getNumArgs(const OPTYPE opCode)
{
	switch(opCode)
	{
		case AND:
		case OR:
		case NAND:
		case NOR:
			return 2;
		case NOT:
			return 1;
		default:
			return 0;
	}//end switch code
}

void cfharness::delPrevCFpop()
{
    for(int i=0; i<m_nPrevCFs; i++)
        delete[] m_pstPrevCFPop[i].m_pOpData;

    delete[] m_pstPrevCFPop;
}

int cfharness::evaluateCF(OPTYPE arrOpCF[], int arrState[])
{
	//display 'cf' for debugging
	/*
	{ // intentionaly blocked to avoid redeclaration error of 'i'
		printf("\n");
		int i=0;
		char* temp = NULL;
		while(arrOpCF[i]!=OPNOP){
			temp = opchar(arrOpCF[i++]);
			printf("%s ",temp);
		}
	}
	*/
	int arrStack[m_stGEnvConfig.m_nCfMaxStack];
	arrStack[0] = 0;
	int nSP = 0;
	for(int i=0; /*i<cfMaxLength*/; i++)
	{
		const OPTYPE opCode = arrOpCF[i];
		//printf("%d ",opCode);
		if(opCode == NOP)
		{
			break;
		}
		if(0<=opCode && opCode<m_stGEnvConfig.m_nCondLen)
		{
            //condition bit
			arrStack[nSP++] = arrState[opCode];
		}
		else if(isPrevLevelsCode(opCode))
		{
            //CF from any previous level
			int nValOfCF = evaluateCF(m_pstPrevCFPop[opCode - m_stGEnvConfig.m_nCondLen].m_pOpData, arrState);
			arrStack[nSP++] = nValOfCF;
		}
		else if(opCode == NOT)
		{
			const int nSPTemp = arrStack[--nSP];
			arrStack[nSP++] = (!nSPTemp)?1:0;
		}
		else
		{
			const int nSP2 = arrStack[--nSP];
			const int nSP1 = arrStack[--nSP];
			switch(opCode){
				case AND:
					arrStack[nSP++] = (nSP1&&nSP2)?1:0;
					break;
				case OR:
					arrStack[nSP++] = (nSP1||nSP2)?1:0;
					break;
				case NAND:
					arrStack[nSP++] = (nSP1&&nSP2)?0:1;
					break;
				case NOR:
					arrStack[nSP++] = (nSP1||nSP2)?0:1;
					break;
			}//end switch
		}
	}
	int nVal = arrStack[--nSP];
	assert(nSP==0);

	//debugging: It is possible that the dontcareCF is created by the system, unintentionaly. So, subsumption deletion can change the results little bit with different dontcareCFs.
	/*
	{ // intentionaly blocked to avoid redeclaration error of 'i'
		bool isItDontcareCF = true;
		for(int i=0; i<m_stGEnvConfig.m_nCfMaxLen; i++)
		{
			if(arrOpCF[i]!=m_stDontCareCF.m_pOpData[i])
			{
				isItDontcareCF = false;
				break;
			}
		}
		if(isItDontcareCF)
		{
			printf("\nIt is dontcareCF ... \n");
			printf("\tvalue: %d\n", value);
			//exit(0);
		}
	}
	*/

	//printf("\tvalue: %d\n", value);
	return nVal;
}

OPTYPE  cfharness::randLeaf()
{
	OPTYPE opLeaf = NOP;
	if(m_nPrevCFs==0 || m_stGEnvConfig.m_nCurrProbLevel==1)
	{
		opLeaf = nRand(m_stGEnvConfig.m_nCondLen);
		//printf("leaf_1 %d\n",opLeaf);
		return opLeaf;
	}
	//Check the probability
	if(fRand() < 0.5)
	{
		opLeaf = nRand(m_stGEnvConfig.m_nCondLen);
		//printf("leaf_2 %d\n",opLeaf);
		return opLeaf;
	}
	int nPos = nRand(m_nPrevCFs);
	opLeaf = m_pstPrevCFPop[nPos].m_nID;
	//printf("n: %d leaf_3 %d\n",nPos,opLeaf);
	return opLeaf;
}

OPTYPE cfharness::randFunction()
{
  return arrGFuncCodes[nRand(nGTotalCFFuncs)];
}

//generate reverse polish
OPTYPE* cfharness::randProgram(OPTYPE* opProg,const int nIsFull,const int nMaxDepth, const int nMinDepth)
{
    //if reached max depth or probabilistically reached mindepth
	if( nMaxDepth<=0 || ( (!nIsFull) && nMinDepth<=0 && nRand(2) ) )
	{
	  *opProg = randLeaf();
	  return opProg+1;
	}
	else
	{
		OPTYPE* opPC = opProg;
		OPTYPE newFunc;
		newFunc = randFunction();
	    const int nArgs = getNumArgs(newFunc);

		for(int i=0; i<nArgs; i++)
			opPC = randProgram(opPC,nIsFull,nMaxDepth-1,nMinDepth-1);

		*opPC = newFunc;
		return opPC+1;
	}
}

void cfharness::addOpData(OPTYPE* pOpData)
{
    OpDataSet *pstSetP;
	pstSetP = new OpDataSet;

	pstSetP->pOpData=pOpData;
	pstSetP->pstNext=m_pstOpDataSet;
	m_pstOpDataSet=pstSetP;
}

void cfharness::freeOpDataSet()
{
    OpDataSet *tempODSet;
	while(m_pstOpDataSet!=NULL)
	{
        //Delete the OpData memory assigned at the time of creation of new CF
		delete[] m_pstOpDataSet->pOpData;
		tempODSet=m_pstOpDataSet->pstNext;
		delete m_pstOpDataSet;
		m_pstOpDataSet=tempODSet;
    }
}

}//end namespace xcs
