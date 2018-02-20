#ifndef VECRNG_MRG_H
#define VECRNG_MRG_H 1

/**
  MRG: NameSpace for the MRG32k3a class based on RngStream.h(cpp)

  RngStream is a class generating multiple streams of random numbers created
  by Prof. Pierre L'Ecuyer, University of Montreal (lecuyer@iro.umontreal.ca) 
  Original source codes of RngStream.h(cpp) is available at
  http://www.iro.umontreal.ca/~lecuyer/myftp/streams00/c++/

  Relevant articles in which MRG32k3a and the package with multiple streams 
  were proposed:

  P. L'Ecuyer, ``Good Parameter Sets for Combined Multiple Recursive Random
  Number Generators'', Operations Research, 47, 1 (1999), 159--164.

  P. L'Ecuyer, R. Simard, E. J. Chen, and W. D. Kelton, ``An Objected-Oriented
  Random-Number Package with Many Long Streams and Substreams'', Operations
  Research, 50, 6 (2002), 1073--1075
  */

#include "RngDefs.h"

namespace vecRng {
inline namespace VECRNG_IMPL_NAMESPACE {

namespace MRG {

  VECRNG_GLOBAL auto ndim    =   3;
  VECRNG_GLOBAL auto vsize   =   6;
    
  VECRNG_GLOBAL double m1    =   4294967087.0;
  VECRNG_GLOBAL double m2    =   4294944443.0;
  VECRNG_GLOBAL double norm  =   1.0 / (m1 + 1.0);
  VECRNG_GLOBAL double a12   =   1403580.0;
  VECRNG_GLOBAL double a13n  =   810728.0;
  VECRNG_GLOBAL double a21   =   527612.0;
  VECRNG_GLOBAL double a23n  =   1370589.0;
  VECRNG_GLOBAL double two17 =   131072.0;
  VECRNG_GLOBAL double two53 =   9007199254740992.0;
  VECRNG_GLOBAL double fact  =   5.9604644775390625e-8;     // 1 / 2^24  


  // The following are the transition matrices of the two MRG components
  // (in matrix form), raised to the powers -1, 1, 2^76, and 2^127, resp.

  VECRNG_GLOBAL double A1p76[ndim][ndim] = {
         {      82758667.0, 1871391091.0, 4127413238.0 },
         {    3672831523.0,   69195019.0, 1871391091.0 },
         {    3672091415.0, 3528743235.0,   69195019.0 }
         };
  
  VECRNG_GLOBAL double A2p76[ndim][ndim] = {
         {    1511326704.0, 3759209742.0, 1610795712.0 },
         {    4292754251.0, 1511326704.0, 3889917532.0 },
         {    3859662829.0, 4292754251.0, 3708466080.0 }
         };
  
  VECRNG_GLOBAL double A1p127[ndim][ndim] = {
         {    2427906178.0, 3580155704.0,  949770784.0 },
         {     226153695.0, 1230515664.0, 3580155704.0 },
         {    1988835001.0,  986791581.0, 1230515664.0 }
         };
  VECRNG_GLOBAL double A2p127[ndim][ndim] = {
         {    1464411153.0,  277697599.0, 1610723613.0 },
         {      32183930.0, 1464411153.0, 1022607788.0 },
         {    2824425944.0,   32183930.0, 2093834863.0 }
         };

  //parameters from curand_MRG32k3a
  VECRNG_GLOBAL double rh1 =  2.3283065498378290e-010;  /* (1.0 / m1)__hi */
  VECRNG_GLOBAL double rl1 = -1.7354913086174288e-026;  /* (1.0 / m1)__lo */
  VECRNG_GLOBAL double rh2 =  2.3283188252407387e-010;  /* (1.0 / m2)__hi */
  VECRNG_GLOBAL double rl2 =  2.4081018096503646e-026;  /* (1.0 / m2)__lo */

} // end of MRG namespace

} // end namespace impl
} // end namespace vecRng

#endif
