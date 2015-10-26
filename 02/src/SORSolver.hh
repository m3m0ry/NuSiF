#ifndef SOR_SOLVER_HH
#define SOR_SOLVER_HH

#include "StaggeredGrid.hh"


class SORSolver
{
public:
   // Constructor to manually create SORSolver
   SORSolver (  );

   // Constructor to create a SORSolver from a parsed configuration file
   SORSolver ( const FileReader & configuration );


   // solve the pressure equation on the staggered grid
   bool solve( StaggeredGrid & grid );

private:
   // TODO add solver parameters here as member
   int itermax_;
   Real eps_; // Epsilon
   Real omg_; // 
};






#endif //SOR_SOLVER_HH




