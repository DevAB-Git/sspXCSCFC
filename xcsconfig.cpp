////////////////////////////////////////////////////////////////
//  xcsconfig.cpp
//  xcssystem file for xcs based single step problems
//  developed by AB Siddique
//  School of Engineering and Computer Science
//  Victoria University of Wellington
/////////////////////////////////////////////////////////////////
#include "xcsconfig.hpp"
//just to remove getopt compilation error from CDK
#include <getopt.h>
using namespace xcs;
using namespace std;

/////////////////////////////////////////////////////////////////
//Configuration data
/////////////////////////////////////////////////////////////////

ClsConfig::ClsConfig() :
    m_cDntCare('#'),
    m_bGASubSump(true),
    m_bActSetSubSump(true),
    m_bGAErrBasedSel(false),
    m_fAlpha(0.1),
    m_fBeta(0.2),
    m_fGama(0.95),
    m_fDelta(0.1),
    m_fNu(5.0),
    m_fThetaGA(25.0),
    m_fEpsilon0(10.0),
    m_fpX(0.8),
    m_fpM(0.04),
    m_fProbDntCare(0.33),
    m_pCF(0.5),
    m_fPredictErrReduction(0.25),
    m_fFitnessReduction(0.1),
    m_fInitPredict(10.0),
    m_fInitPredictErr(0.0),
    m_fInitFitness(0.01),
    m_fThetaSel(0.4),
    m_fForceDiffInTornmnt(0.0),
    m_fSelTolrnc(0.0),
    m_nThetaDel( 20),
    m_nCrssoverType(2),
    m_nMutationType(0),
    m_nThetaSub(20)
{

}

envConfig::envConfig()
{
    m_nPosBits = 2;                     //2, 3, 4, 5, 6, and 7 for 6-, 11-, 20-, 37-, 70-, and 135-bits MUX respectively
    m_nCurrProbLevel = m_nPosBits-1;
    m_nCondLen = 6;                    //posBits + pow(2,posBits);
    m_nclfrCondLen = m_nCondLen;        //it would be condLength/2 and condLength/4 for 70mux and 135mux respectively.
    m_nRlvntBits = 9;
    m_nMaxPopSize = 0.5*1000;             //Specifies the maximal number of micro-classifiers in the population.
                                        //[ (0.5, 1, 2, 5, 10/20, 50)*1000 for 6-, 11-, 20-, 37-, 70-, 135-bits MUX respectively]
    //temp chg AB
    m_nMaxProblems = 5*100*1000;      //training set = [ (5, 5, 5, 5/10, 20, 50)*100*1000 for 6-, 11-, 20-, 37-, 70-, 135-bits MUX respectively]
    m_nMaxPayoff = 1000;
    m_nActions = 2;
    m_eEnv = multiplexer;
    //code fragment vars
    m_nCfMaxDepth = 2;
    m_nCfMinDepth = 0;
    m_nCfMaxLen = 8;// pow(2,adfMaxDepth+1); //allow for endstop OPNOP
    m_nCfMaxArity = 2;
    m_nCfMaxStack = (m_nCfMaxArity-1)*(m_nCfMaxDepth-1)+2;
    m_nTotalFuncs = 5;
}

LogExec::LogExec()
{
    m_strLogPath ="output/sspXCSCFC/";
    m_pfPerformance = NULL;
    m_pfCndnsdClsPop = NULL;
    m_pfSmplClsPop = NULL;
    m_pfWholeClsPop = NULL;
    m_pfPrevCFPop = NULL;
    m_pfCFPop = NULL;
    m_nTotalRuns = 30;
    m_nTestFrequency = 100;
}

//It checks if the given line is a comment or not
//A comment line starts with '%', '#' or '@'.
bool loadConfig::isComment(string& strLine)
{
    bool isAComment = false;
    uint32_t i = 0;
    while( i < strLine.size() ) {
      if( strLine.at(i) == '%' || strLine.at(i) == '@' ) //strLine.at(i) == '#'  bcz it is don't care symbol
      {
          isAComment = true;
          break;
      }
      if( strLine.at( i ) != ' ' || strLine.at( i ) != '\t' )
      {
          break;
          ++i;
      }
    }
    return isAComment;
}

string loadConfig::lineTrim(string& strLine)
{
    uint32_t i = 0, j = strLine.size() - 1, nSize = strLine.size();

    //The initial spaces, tabs, and other non-useful stuff.
    while( i < strLine.size() && (strLine[i] == ' ' ||
      strLine[i] == '\n' || strLine[i] == '\t' || strLine[i] == '\r' ||
      strLine[i] == ( char ) 13) )
    {
      ++i;
      --nSize;
    }
    //The final spaces, tabs, and others.
    while( j >= 1 && (strLine[j] == ' ' ||
      strLine[j] == '\n' || strLine[j] == '\t' || strLine[j] == '\r' ||
      strLine[j] == ( char ) 13) )
    {
      --j;
      --nSize;
    }
    return strLine.substr( i, nSize );
}

//It removes the final spaces from a line.
string loadConfig::removeFinalSpaces(string& strLine)
{
    string strOut;
    uint32_t i = strLine.size() - 1;
    while( i >= 1 && (strLine[i] == ' ' || strLine[i] == '\t') )
    {
      --i;
    }
    return strLine.substr( 0, i );
}

bool loadConfig::readLine(ifstream& ifsFile, string& strLine)
{
    //Read until an interesting thing appears.
    getline( ifsFile, strLine, '\n' );
    while(!ifsFile.eof() && (strLine.size() == 0 || isComment( strLine )))
    {
      getline( ifsFile, strLine, '\n' );
    }
    return ifsFile.eof();
}

//It gets the token that follows '='.
//The strLine that contains token "X = Y"
string loadConfig::getNextToken(string& strLine )
{
    char cpVal[MAXBUFFSIZE] = {0};
    uint32_t nPos = 0, j = 0;
    while( nPos < strLine.size() && strLine[ nPos ] != '=' )
    {
        ++nPos;
    }
    ++nPos;

    while( nPos < strLine.size() && (strLine[ nPos ] == ' ' || strLine[ nPos ] == '\t') )
    {
        //Jump blank spaces.
        ++nPos;
    }

    while( nPos < strLine.size() && strLine[ nPos ] != ' ' && strLine[ nPos ] != '\t' && j < MAXBUFFSIZE )
    {
        //Get the value.
        cpVal[ j++ ] = strLine[ nPos++ ];
    }
    cpVal[j] = '\0';

    string strVal(cpVal);
    return strVal;
}

void loadConfig::loadConfigFromFile(string strFileName)
{
    ifstream configFile(strFileName.c_str(), ios::in);
    string strLine;
    if(configFile.is_open())
    {
      while(!readLine(configFile, strLine))
      {
          strLine = lineTrim( strLine );
          //Environment configurations
          if( strLine.find( "PopSize ", 0 ) != string::npos )
              m_stGEnvConfig.m_nMaxPopSize = atoi( getNextToken( strLine ).c_str() );
          else if( strLine.find( "Problems " ) != string::npos )
              m_stGEnvConfig.m_nMaxProblems = atoi( getNextToken( strLine ).c_str() );
          else if( strLine.find( "PosBits " ) != string::npos )
          {
              m_stGEnvConfig.m_nPosBits = atoi( getNextToken( strLine ).c_str() );
              m_stGEnvConfig.m_nCurrProbLevel = m_stGEnvConfig.m_nPosBits-1;
          }
          else if( strLine.find( "ConditionLength " ) != string::npos )
          {
              m_stGEnvConfig.m_nCondLen = atoi( getNextToken( strLine ).c_str() );
              //CF specific implementation
              //it would be condLength/2 and condLength/4 for 70mux and 135mux respectively.
              m_stGEnvConfig.m_nclfrCondLen = m_stGEnvConfig.m_nCondLen;

          }

          else if( strLine.find( "RelevantBits" ) != string::npos )
              m_stGEnvConfig.m_nRlvntBits = atoi( getNextToken( strLine ).c_str() );
          else if( strLine.find( "MaximumPayoff " ) != string::npos )
              m_stGEnvConfig.m_nMaxPayoff = atoi( getNextToken( strLine ).c_str() );
          else if( strLine.find( "TotalActions " ) != string::npos )
              m_stGEnvConfig.m_nActions = atoi( getNextToken( strLine ).c_str() );
          else if( strLine.find( "Environment " ) != string::npos )
          {
              string strEnv=getNextToken( strLine );
              if(strEnv == "Multiplexer" || strEnv == "multiplexer")
                m_stGEnvConfig.m_eEnv = multiplexer;
              else if(strEnv == "HiddenEvenParity" || strEnv == "hiddenevenparity")
                m_stGEnvConfig.m_eEnv = hiddenEvenParity;
              else if(strEnv == "HiddenOddParity" || strEnv == "hiddenoddparity")
                m_stGEnvConfig.m_eEnv = hiddenOddParity;
              else if(strEnv == "Carry" || strEnv == "carry")
                m_stGEnvConfig.m_eEnv = carry;
              else if(strEnv == "EvenParity" || strEnv == "evenparity")
                m_stGEnvConfig.m_eEnv = evenParity;
              else if(strEnv == "MajorityOn" || strEnv == "majorityon")
                m_stGEnvConfig.m_eEnv = majorityOn;
              else if(strEnv == "DV1" || strEnv == "dv1")
                m_stGEnvConfig.m_eEnv = dv1;
              else
                printf("\nUnsupported environment ");
          }
          //Classifier configurations
          else if( strLine.find( "Alpha " ) != string::npos )
              m_stGClsCFG.m_fAlpha = atof( getNextToken( strLine ).c_str() );
          else if( strLine.find( "Beta " ) != string::npos )
              m_stGClsCFG.m_fBeta = atof( getNextToken( strLine ).c_str() );
          else if( strLine.find( "Gama " ) != string::npos )
              m_stGClsCFG.m_fGama = atof( getNextToken( strLine ).c_str() );
          else if( strLine.find( "Delta " ) != string::npos )
              m_stGClsCFG.m_fDelta = atof( getNextToken( strLine ).c_str() );
          else if( strLine.find( "m_fNu " ) != string::npos )
              m_stGClsCFG.m_fNu = atof( getNextToken( strLine ).c_str() );
          else if( strLine.find( "ThetaGA " ) != string::npos )
              m_stGClsCFG.m_fThetaGA = atof( getNextToken( strLine ).c_str() );
          else if( strLine.find( "Epsilon0 " ) != string::npos )
              m_stGClsCFG.m_fEpsilon0 = atof( getNextToken( strLine ).c_str() );
          else if( strLine.find( "ThetaDel " ) != string::npos )
              m_stGClsCFG.m_nThetaDel = atoi( getNextToken( strLine ).c_str() );
          else if( strLine.find( "m_fpX " ) != string::npos )
              m_stGClsCFG.m_fpX = atof( getNextToken( strLine ).c_str() );
          else if( strLine.find( "CrssoverType " ) != string::npos )
              m_stGClsCFG.m_nCrssoverType = atoi( getNextToken( strLine ).c_str() );
          else if( strLine.find( "m_fpM " ) != string::npos )
              m_stGClsCFG.m_fpM = atof( getNextToken( strLine ).c_str() );
          else if( strLine.find( "MutationType " ) != string::npos )
              m_stGClsCFG.m_nMutationType = atoi( getNextToken( strLine ).c_str() );
          else if( strLine.find( "ProbDntCare " ) != string::npos )
              m_stGClsCFG.m_fProbDntCare = atof( getNextToken( strLine ).c_str() );
          else if( strLine.find( "PredictErrReduction " ) != string::npos )
              m_stGClsCFG.m_fPredictErrReduction = atof( getNextToken( strLine ).c_str() );
          else if( strLine.find( "FitnessReduction " ) != string::npos )
              m_stGClsCFG.m_fFitnessReduction = atof( getNextToken( strLine ).c_str() );
          else if( strLine.find( "ThetaSub " ) != string::npos )
              m_stGClsCFG.m_nThetaSub = atoi( getNextToken( strLine ).c_str() );
          else if( strLine.find( "InitPredictErr " ) != string::npos )
              m_stGClsCFG.m_fInitPredictErr = atof( getNextToken( strLine ).c_str() );
          else if( strLine.find( "InitPredict " ) != string::npos )
              m_stGClsCFG.m_fInitPredict = atof( getNextToken( strLine ).c_str() );
          else if( strLine.find( "InitFitness " ) != string::npos )
              m_stGClsCFG.m_fInitFitness = atof( getNextToken( strLine ).c_str() );
          else if( strLine.find( "GASubSump " ) != string::npos )
          {
              string strTemp = getNextToken(strLine);
              if(strTemp == "false")
                m_stGClsCFG.m_bGASubSump = false;
              else
                m_stGClsCFG.m_bGASubSump = true;
          }

          else if( strLine.find( "ActSetSubSump " ) != string::npos )
          {
              string strTemp = getNextToken(strLine);
              if(strTemp == "false")
                m_stGClsCFG.m_bActSetSubSump = false;
              else
                m_stGClsCFG.m_bActSetSubSump = true;
          }
          else if( strLine.find( "ThetaSel " ) != string::npos )
              m_stGClsCFG.m_fThetaSel = atof( getNextToken( strLine ).c_str() );
          else if( strLine.find( "ForceDiffInTornmnt " ) != string::npos )
              m_stGClsCFG.m_fForceDiffInTornmnt = atof( getNextToken( strLine ).c_str() );
          else if( strLine.find( "GAErrBasedSel " ) != string::npos )
          {
              string strTemp = getNextToken(strLine);
              if(strTemp == "false")
                m_stGClsCFG.m_bGAErrBasedSel = false;
              else
                m_stGClsCFG.m_bGAErrBasedSel = true;
          }
          else if( strLine.find( "SelTolrnc " ) != string::npos )
              m_stGClsCFG.m_fSelTolrnc = atof( getNextToken( strLine ).c_str() );
          else if( strLine.find( "DntCare " ) != string::npos )
              m_stGClsCFG.m_cDntCare = getNextToken( strLine ).at(0);
          //LogExe configurations
          else if( strLine.find( "LogPath " ) != string::npos )
              m_stGLogExec.m_strLogPath = getNextToken( strLine );
          else if( strLine.find( "TotalRuns " ) != string::npos )
              m_stGLogExec.m_nTotalRuns = atoi( getNextToken( strLine ).c_str() );
          else if( strLine.find( "TestFrequency " ) != string::npos )
              m_stGLogExec.m_nTestFrequency = atoi( getNextToken( strLine ).c_str() );
      }
      configFile.close();
    }
    else
      cout << "> Warning: I couldn't load the configuration file " << strFileName << "; loading defaults..." << endl;


}

void loadConfig::printConfig()
{
    cout<<"m_stGClsCFG \n";
    cout<<" "<<m_stGClsCFG.m_bActSetSubSump;
    cout<<" "<<m_stGClsCFG.m_bGAErrBasedSel;
    cout<<" "<<m_stGClsCFG.m_bGASubSump;
    cout<<" "<<m_stGClsCFG.m_cDntCare;
    cout<<" "<<m_stGClsCFG.m_fAlpha;
    cout<<" "<<m_stGClsCFG.m_fBeta;
    cout<<" "<<m_stGClsCFG.m_fDelta;
    cout<<" "<<m_stGClsCFG.m_fEpsilon0;
    cout<<" "<<m_stGClsCFG.m_fFitnessReduction;
    cout<<" "<<m_stGClsCFG.m_fForceDiffInTornmnt;
    cout<<" "<<m_stGClsCFG.m_fGama;
    cout<<" "<<m_stGClsCFG.m_fInitFitness;
    cout<<" "<<m_stGClsCFG.m_fInitPredict;
    cout<<" "<<m_stGClsCFG.m_fInitPredictErr;
    cout<<" "<<m_stGClsCFG.m_fNu;
    cout<<" "<<m_stGClsCFG.m_fpM;
    cout<<" "<<m_stGClsCFG.m_fPredictErrReduction;
    cout<<" "<<m_stGClsCFG.m_fProbDntCare;
    cout<<" "<<m_stGClsCFG.m_fpX;
    cout<<" "<<m_stGClsCFG.m_fSelTolrnc;
    cout<<" "<<m_stGClsCFG.m_fThetaGA;
    cout<<" "<<m_stGClsCFG.m_fThetaSel;
    cout<<" "<<m_stGClsCFG.m_nCrssoverType;
    cout<<" "<<m_stGClsCFG.m_nMutationType;
    cout<<" "<<m_stGClsCFG.m_nThetaDel;
    cout<<" "<<m_stGClsCFG.m_nThetaSub;
    cout<<endl;
}

//Parse command line arguments
void loadConfig::parseargs(int argc, char** argv)
{
    // parse the command-line options
    int opt;
    while ((opt = getopt(argc, argv, "f:F:p")) != -1) {
        switch(opt) {
          case 'f':
          case 'F':
                m_strConfigFile  = string(optarg);
          break;
          //case 'p': CFG.threads       = strtol(optarg, NULL, 10); break;
          case 'p':
          case 'P':
            printConfig();
        }
    }
}
//////////////////////////////////////////////////////////////////////
//Log execution data
//////////////////////////////////////////////////////////////////////

void LogExec::setLogFileNames(int nSession)
{
    string strTemp = TOSTR(nSession) + "/" + TOSTR(m_stGEnvConfig.m_nCondLen);
    m_strPerformance = m_strLogPath + strTemp + "MuxPerformance.txt";
    m_strCndnsdClsPop = m_strLogPath + strTemp + "rulescondensed.txt";
    m_strSmplClsPop = m_strLogPath + strTemp + "rulesimplified.txt";
    m_strWholeClsPop = m_strLogPath + strTemp + "MUXRules.txt";
    m_strCFPop = m_strLogPath + strTemp + "MuxCFPop.txt";

    if(m_stGEnvConfig.m_nCurrProbLevel>1)
    {
        int nPrevCondLen;
        switch(m_stGEnvConfig.m_nCondLen)
        {
            case 11:
                nPrevCondLen = 6;
            break;
            case 20:
                nPrevCondLen = 11;
            break;
            case 37:
                nPrevCondLen = 20;
            break;
            case 70:
                nPrevCondLen = 37;
            break;
            case 135:
                nPrevCondLen = 70;
            break;
            default:
                cout<<"Unsupported condition length for Mux problem\n";
                nPrevCondLen = -1;

        }
        m_strPrevCFPop = m_strLogPath + TOSTR(nSession) + "/" + TOSTR(nPrevCondLen) + "MuxCFPop.txt";
    }
}

void LogExec::openLogFiles()
{
    m_pfPerformance=fopen(m_strPerformance.c_str(), "w");
    if(!m_pfPerformance)
    {
        printf("Error in opening a file.. %s", m_strPerformance.c_str());
		exit(1);
    }

    m_pfCndnsdClsPop=fopen(m_strCndnsdClsPop.c_str(), "w");
    if(!m_pfCndnsdClsPop)
    {
        printf("Error in opening a file.. %s", m_strCndnsdClsPop.c_str());
		exit(1);
    }

    m_pfSmplClsPop=fopen(m_strSmplClsPop.c_str(), "w");
    if(!m_pfSmplClsPop)
    {
        printf("Error in opening a file.. %s", m_strSmplClsPop.c_str());
		exit(1);
    }

    m_pfWholeClsPop=fopen(m_strWholeClsPop.c_str(), "w");
    if(!m_pfWholeClsPop)
    {
        printf("Error in opening a file.. %s", m_strWholeClsPop.c_str());
		exit(1);
    }

    //open previous CF pop in read only mode
    if(m_stGEnvConfig.m_nCurrProbLevel>1)
    {
        m_pfPrevCFPop=fopen(m_strPrevCFPop.c_str(), "r");
        if(!m_pfPrevCFPop)
        {
            printf("Error in opening a file.. %s", m_strPrevCFPop.c_str());
            exit(1);
        }
    }


    m_pfCFPop=fopen(m_strCFPop.c_str(), "w");
    if(!m_pfCFPop)
    {
        printf("Error in opening a file.. %s", m_strWholeClsPop.c_str());
		exit(1);
    }

}

void LogExec::closeLogFiles()
{
    if(m_pfPerformance)
        fclose(m_pfPerformance);
    if(m_pfCndnsdClsPop)
        fclose(m_pfCndnsdClsPop);
    if(m_pfSmplClsPop)
        fclose(m_pfSmplClsPop);
    if(m_pfWholeClsPop)
        fclose(m_pfWholeClsPop);
    if(m_pfPrevCFPop)
        fclose(m_pfPrevCFPop);
    if(m_pfCFPop)
        fclose(m_pfCFPop);

}

FILE* LogExec::getFilePtr(LogFileType eFileType)
{
    switch(eFileType)
    {
        case PERFORMANCE:
            return m_pfPerformance;
        break;
        case CNDNSDCLSPOP:
            return m_pfCndnsdClsPop;
        break;
        case SMPLCLSPOP:
            return m_pfSmplClsPop;
        break;
        case WHOLECLSPOP:
            return m_pfWholeClsPop;
        break;
        case PREVCFPOP:
            return m_pfPrevCFPop;
        break;
        case CFPOP:
            return m_pfCFPop;
        break;
        default:
            printf("\nInvalid File type ");
            return NULL;
    }
}


//Classifier Config Object
ClsConfig xcs::m_stGClsCFG;// ALIGN(64);
//LogExe object
LogExec xcs::m_stGLogExec;// ALIGN(64);
//Environment setting object
envConfig xcs::m_stGEnvConfig;
//LoadConfig object
loadConfig xcs::m_stGLoadConfig;



