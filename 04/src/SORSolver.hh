#pragma once

#include <cmath>
//#include <ctgmath>
#include <iostream>
#include <ostream>

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
   SORSolver( int itermax, Real eps, Real omg );

   // Constructor to create a SORSolver from a parsed configuration file
   SORSolver( const FileReader & configuration );


   // solve the pressure equation on the staggered grid
   bool solve( StaggeredGrid & grid );

private:
   int itermax_;
   Real eps_; // Epsilon
   Real omg_; // 
	
	int epsFrequency_;
	int normFrequency_;
};


