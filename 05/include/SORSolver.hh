#pragma once

#include <cmath>
//#include <ctgmath>
#include <iostream>
#include <ostream>
//because clang is stupid and cannot include omp.h
#if defined(_OPENMP)
   #include <omp.h>
#endif

#include "StaggeredGrid.hh"
#include "Debug.hh"
#include "Types.hh"
#include "FileReader.hh"

#ifndef NDEBUG
#include <iostream>
#include <fstream>

#endif

class SORSolver
{
public:
   // Constructor to manually create SORSolver
   SORSolver(unsigned int itermax, Real eps, Real omg );

   // Constructor to create a SORSolver from a parsed configuration file
   SORSolver( const FileReader & configuration );


   // solve the pressure equation on the staggered grid
   bool solve( StaggeredGrid & grid );

private:
   void sor(StaggeredGrid & grid);
   void sorParallel(StaggeredGrid & grid);
   void sorRedBlack(StaggeredGrid & grid);
   void updateBoundaries(StaggeredGrid & grid);
   Real computeResiduum(StaggeredGrid & grid);

   unsigned int itermax_;
   Real eps_; // Epsilon
   Real omg_; // Omega
   unsigned int epsFrequency_;
   unsigned int normFreqency_;
};


