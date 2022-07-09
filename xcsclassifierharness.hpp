////////////////////////////////////////////////////////////////
//  xcsclassifierharness.hpp
//  xcsclassifier file for xcs based single step problems
//  developed by AB Siddique
//  School of Engineering and Computer Science
//  Victoria University of Wellington
/////////////////////////////////////////////////////////////////
#ifndef XCSCLASSIFIER_HARNESS_HPP__
#define XCSCLASSIFIER_HARNESS_HPP__
#include "xcsenvironment.hpp"
#include "xcscodefragmentharness.hpp"

namespace xcs
{

struct Classifier
{
	int                 m_nAction;
	int                 m_nNumerosity;
	int                 m_nExprnc;
	int                 m_nTimeStamp;
	int                 m_nSpecificness;        //number of specific CFs
	double              m_fPredict;
	double              m_fPredictErr;
	double              m_fAccuracy;
	double              m_fFitness;
	double              m_fActSetSize;
	codeFragment*       m_pstCFCondition;

}; //temp chg AB ALIGN(64);

struct ClassifierSet
{
	Classifier  *pstClfr;
	ClassifierSet *pstNext;
};//ALIGN(64);

class clfrharness
{
public:
    clfrharness();
    ~clfrharness();

    void initClassifierSets();
    //initialize classifier configuration and other settings
    void initClassifier(Classifier *pstClfr, double fActSetSize, int nTimeStamp);
    int getNumerositySum(ClassifierSet *pstClfrSet);
    int getSetSize(ClassifierSet *pstClfrSet);
    int getSetSize(ClfrSetType eClfrSetType);

    void getMatchSet(OPTYPE pOpState[], int nTimeStamp);
    int getActions(ClassifierSet *pstClfrSet, bool arrCvrdActions[]);
    bool isCondMatched(codeFragment pstClfrCond[], OPTYPE pOpState[]);
    void createMatchCond(codeFragment pstClfrCond[], OPTYPE pOpState[]);
    //void createMatchCond(Classifier *pstClfr, OPTYPE pOpState[]);
    Classifier* createClfr(OPTYPE pOpState[], int nAction, int nSetSize, int nTimeStamp);

    //determines the prediction array out of the match set ms
    void getPredictArr();
    //Returns the highest value in the prediction array.
    double getBestValue();
    int randActionWinner();
    int bestActionWinner();
    int rouletteActionWinner();

    void getActionSet(int nAction);
    void updateActionSet(double fMaxPredict, double fReward);
    void updateFitness(ClassifierSet *pstClfrSet);

    void discoveryComponent(ClassifierSet **pstSet, int nItTime, OPTYPE pOpSituation[]);
    void getDiscoversSums(ClassifierSet *pstSet, double *fFitSum, int *nSetSum, int *nGaitSum);
    void setTimeStamps(ClassifierSet *pstSet, int nItTime);

    void selectTwoClassifiers(Classifier **pstClfr, Classifier **pstParents, ClassifierSet *pstSet, double fFitSum, int nSetSum);
    Classifier* tournamentSelection(ClassifierSet *pstSet, int nSetSum, Classifier *pstNotMe);
    Classifier* rouletteWheelSelection(ClassifierSet *pstSet, double fFitSum);

    void crossover(Classifier **pstClfr, int nType);
    void uniformCrossover(Classifier **pstClfr);
    void onePointCrossover(Classifier **pstClfr);
    void twoPointCrossover(Classifier **pstClfr);
    bool mutation(Classifier *pstClfr, OPTYPE pOpState[]);
    bool applyNicheMutation(Classifier *pstClfr, OPTYPE pOpState[]);
    bool applyGeneralMutation(Classifier *pstClfr, OPTYPE pOpState[]);
    bool mutateAction(Classifier *pstClfr);

    void insertDiscoveredClassifier(Classifier **pstClfr, Classifier **pstParents, ClassifierSet **pstSet, int nLen);

    void actSetSubsumption();
    void subsumeClassifier(Classifier *pstClfr, Classifier **pstParents, ClassifierSet *pstLocSet);
    bool subsumeClassifierToSet(Classifier *pstClfr, ClassifierSet *pstSet);
    bool isSubSumes(Classifier *pstClfr1, Classifier * pstClfr2);
    bool isSubSumer(Classifier *pstClfr);
    bool isMoreGeneral(Classifier *pstClfr1, Classifier * pstClfr2);
    bool compareDontCares(Classifier *pstClfr1, Classifier *pstClfr2);
    bool checkNonDontCares(codeFragment pstClfrCond1[], codeFragment pstClfrCond2[]);


    // it is similar to addNewClassifierToSet
    bool addClassifier(Classifier *pstClfr, ClassifierSet **pstClfrSet);
    //it is similar to addClassifierToPointerSet or addClassifierToSet for  false or true value of bIncNum respectivelly
    bool addClassifier(Classifier *pstClfr, ClassifierSet **pstClfrSet, bool bIncNum);
    bool isEquals(Classifier *pstClfr1, Classifier *pstClfr2);

    //Deletes a classifier from the population.
    Classifier* delStochClassifier();
    //Returns the vote for deletion of the classifier.
    double getDelProp(Classifier *pstClfr, double fMeanFitness);
    //Deletes the classifier setp from the population pop, setpl points to the classifier that is before setp in the list
    Classifier* delClassifier(ClassifierSet *pstSetP, ClassifierSet *pstSetPl);
    bool updateSet(ClassifierSet **pstUset);
    bool delClfrPointerFromSet(ClassifierSet **pstUset, Classifier *pstClfr);

    void freeClassifier(Classifier *pstClfr);
    void freeSet(ClfrSetType eClfrSetType);
    void freeSet(ClassifierSet **pstClfrSet);
    void freeClassifierSet(ClfrSetType eClfrSetType);
    void freeClassifierSet(ClassifierSet **pstClfrSet);

    void printClassifierSet(ClassifierSet *pstHead, FILE *pFile=NULL);
    void printClassifierSet(ClfrSetType eClfrSetType, FILE *pFile=NULL);
    void printClassifier(Classifier *pstClfr, FILE *pFile=NULL);

    ClassifierSet* sortClassifierSet(ClassifierSet **pstSet, int nType);
    void sortClassifierSetAndUpdatePtr(ClfrSetType eClfrSetType, int nType);
    //condense the population of classifiers by deleting classifier rules with "numerosity < limit"
    //void condensePopulation();
    void simplifyPopulation();
    //bool qualifyForSimplification(Classifier *pstClfr1, Classifier *pstClfr2);

    ClassifierSet**  getPopSet();
    ClassifierSet**  getActSet();
    ClassifierSet**  getKilSet();

    //CF
    void storeCFs();
    void freeCFData();
    double getAvgFitness();
    int getNumFitterCFs(double fAvgFitness);
    void outProg(const OPTYPE* opProg, int nSize);
    void outProg(const OPTYPE* const opProg, int nSize, FILE *pfWrite);

private:
    env                  m_stEnv;
    int                  m_nNewCfs;
    cfharness            m_stCfHarness;
    ClassifierSet       *m_pstPopulation;
    ClassifierSet       *m_pstMatchSet;
    ClassifierSet       *m_pstActionSet;
    ClassifierSet       *m_pstKillSet;
    double              *m_pfPredictArr;               //prediction array
    double              *m_pfSumClfrFtnsPredictArr;    //The sum of the fitnesses of classifiers that represent each entry in the prediction array.

};

}
#endif //XCSCLASSIFIER_HARNESS_HPP__
