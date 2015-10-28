#ifndef SOR_SOLVER_HH
#define SOR_SOLVER_HH

#include <cmath>
//#include <ctgmath>

#include "StaggeredGrid.hh"
#include "Types.hh"
#include "FileReader.hh"


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
};






#endif //SOR_SOLVER_HH




