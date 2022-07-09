////////////////////////////////////////////////////////////////
//  xcsmacros.cpp
//  source macros file for xcs based single step problems
//  developed by AB Siddique
//  School of Engineering and Computer Science
//  Victoria University of Wellington
/////////////////////////////////////////////////////////////////
#include "xcsdefs.hpp"
#include "math.h"

namespace xcs
{

//VARS
    long                m_nSeed = 143907;       // a number between 1 and _M-1
    const  long         _M = 2147483647;        //ant for the random number generator (modulus of PMMLCG = 2^31 -1).
    const  long         _A = 16807;             //ant for the random number generator (default = 16807).
    const  long         _Q = _M/_A;             // constant for the random number generator (=_M/_A).
    const  long         _R = _M%_A;             // constant for the random number generator (=_M mod _A).

//Functions

// sets a random seed in order to randomize the pseudo random generator
void setSeed(long nSeed)
{
	m_nSeed=nSeed;
}

long getSeed()
{
	return m_nSeed;
}

// returns a floating-point random number generated according to uniform distribution from [0,1]
double fRand()
{
	long hi   = m_nSeed / _Q;
	long lo   = m_nSeed % _Q;
	long test = _A*lo - _R*hi;
	if (test>0)
	    m_nSeed = test;
	else
	    m_nSeed = test+_M;

	return (double)(m_nSeed)/_M;
}

// returns a random number generated according to uniform distribution from [0,n-1]
int nRand(int nNum)
{
	int num = (int)(fRand()*(float)nNum);

	while(num == nNum)
		num = (int)(fRand()*(float)nNum);

	return num;
}

double absValue(double fValue)
{
	if(fValue < 0.0)
		return -1.0*fValue;

	else
		return fValue;

}

double fRound( double fVal, int nDecimals )
{
    double fMult = (double) pow( 10.0, (double) nDecimals );
    double fAux = fVal * fMult;
    return ((double) round( fAux )) / fMult;
}

} //~end namespace xcs
