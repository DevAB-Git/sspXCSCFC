////////////////////////////////////////////////////////////////
//  xcsmacros.hpp
//  macros file for xcs based single step problems
//  developed by AB Siddique
//  School of Engineering and Computer Science
//  Victoria University of Wellington
/////////////////////////////////////////////////////////////////
#ifndef XCSDEFS_HPP__
#define XCSDEFS_HPP__
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <sstream>

namespace xcs
{

//MACROS
//#define ALIGN(N)     __attribute__((aligned(N)))
#define TOSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

typedef int OPTYPE;

//ENUMS
//Experimental environment
enum Environment
{
    multiplexer, hiddenEvenParity, hiddenOddParity, countOnes, carry, evenParity, majorityOn, dv1
};

enum ClfrSetType
{
    POPSET, MSET, ACTSET, KILLSET
};

enum LogFileType
{
    PERFORMANCE, CNDNSDCLSPOP, SMPLCLSPOP, WHOLECLSPOP, PREVCFPOP, CFPOP
};

enum operType
{
  //NOP=-100,AND, OR, NAND, NOT, NOR
 NOT=-105, NOR, NAND, OR, AND, NOP
};

//FUNCTIONS

//set seed
void setSeed(long nSeed);
//get seed
long getSeed();
//float random function that returns a floating-point random number generated according to uniform distribution from [0,1]
double fRand();
//integer random function that returns a number from 0 to n-1 inclusive
int nRand(int nNum); //

//It rounds a value to a certain number of decimals.
double fRound( double fVal, int nDecimals );


//get absolute value
double absValue(double fValue);

//VARIABLES
const  long         arrSeeds[30] = {114665,134296,176806,247157,262025,311756,336922,337104,344465,362234,379332,425485,470006,490758,538402,583115,584068,614795,678991,710953,715062,715911,784943,787483,790558,810292,824057,846845,968381,972820};

//environmental vars
const int nSizeDV1 = 74;
const uint32_t MAXBUFFSIZE = 500; //200
const int32_t  UNKNOWNVALUE = -999;
const int nGTotalCFFuncs = 5;
const OPTYPE arrGFuncCodes[] = {AND,OR,NAND,NOR,NOT};

const int DV1[] = {1, 2, 3, 8, 9, 10, 11, 13, 14, 24, 25, 26, 27, 28, 30, 40, 41, 42, 43, 46, 47, 56, 57, 58, 59, 61, 65, 66, 67, 69, 70, 71, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 88, 89, 90, 91, 93, 94, 95, 97, 98, 99, 101, 102, 103, 104, 105, 106, 107, 109, 110, 113, 114, 115, 117, 118, 121, 122, 123, 125, 126, 127};

//code fragment

} //~end namespace xcs
#endif //XCSDEFS_HPP__
