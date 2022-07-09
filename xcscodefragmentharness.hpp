////////////////////////////////////////////////////////////////
//  xcscodefragmentharness.hpp
//  xcscodefragement file for xcs based single step problems
//  developed by AB Siddique
//  School of Engineering and Computer Science
//  Victoria University of Wellington
/////////////////////////////////////////////////////////////////
#ifndef XCSCODEFRAGMENT_HARNESS_HPP__
#define XCSCODEFRAGMENT_HARNESS_HPP__
#include "xcsconfig.hpp"

namespace xcs
{

struct codeFragment
{
	int         m_nID;
	OPTYPE*     m_pOpData;
};

struct OpDataSet
{
	OPTYPE*      pOpData;
	OpDataSet   *pstNext;
};

//forward declaration
//struct ClassifierSet;

class cfharness
{
//friend class
    friend class clfrharness;
public:
    cfharness();
    ~cfharness();

    void loadPrevCFPop();
    OPTYPE getOpType(char pczBuff[]);
    bool isExist(codeFragment newCF, codeFragment pstPrevCFPop[], int nTotalCFs);
    bool isEqualCFs(codeFragment stCF1, codeFragment stCF2);
    bool isDontCareCF(codeFragment stCF);
    //get the number of specific CFs in cond
    int getNumSpecificCF(codeFragment stArrCond[]);
    void printCF(codeFragment stCF, FILE *pFile=NULL);
    void ValidDepth(OPTYPE* opCF, OPTYPE* opEnd);
    codeFragment createNewCF(int nId);

    int evaluateCF(OPTYPE arrOpCF[], int arrState[]);
    bool isPrevLevelsCode(const OPTYPE opCode);

    int getNumArgs(const OPTYPE opCode);
    //it just return the same value
    //OPTYPE leafOpCode(const int r);
    OPTYPE randLeaf();
    int validLeaf(const OPTYPE opCode);
    OPTYPE randFunction();
    OPTYPE* randProgram(OPTYPE* opProg,const int nIsFull,const int nMaxDepth, const int nMinDepth);
    //char* getLeafName(const OPTYPE opCode);
    //char* getOpChar(const OPTYPE opCode);
    void getLeafName(const OPTYPE opCode, char pczTempBuff[]);
    void getOpChar(const OPTYPE opCode, char pczTempBuff[]);

    void DepthMax(const OPTYPE* const opEnd,OPTYPE** opProg, int& nArgsToGo, int& nDepth);
    void delPrevCFpop();

    //Add the CF data pointers to delete at end of prog
    void addOpData(OPTYPE* pOpData);
    void freeOpDataSet();


private:
    codeFragment        m_stDontCareCF;
    int                 m_nPrevCFs;
    int                 m_nStPrevCFId;
    codeFragment       *m_pstPrevCFPop;
    OpDataSet          *m_pstOpDataSet;

};


} //end namespace xcs

#endif //XCSCODEFRAGMENT_HARNESS_HPP__
