#include "SORSolver.hh"

// Constructor to manually create SORSolver
SORSolver::SORSolver(unsigned int itermax, Real eps, Real omg ) : itermax_(itermax), eps_(eps), omg_(omg)
{

}

// Constructor to create a SORSolver from a parsed configuration file
SORSolver::SORSolver ( const FileReader & configuration )
{
   int test = 0;
   test = configuration.getIntParameter("itermax");
   CHECK_MSG(test >= 0, "Itermax is less then 0");
   itermax_ = (unsigned int) test;
   eps_ = configuration.getRealParameter("eps");
   omg_ = configuration.getRealParameter("omg");
   test = configuration.getIntParameter("checkfrequency");
   CHECK_MSG(test >= 0, "Checkfrequency is less then 0");
   epsFrequency_ = (unsigned int) test;
}



// Solve the pressure equation on the staggered grid
bool SORSolver::solve( StaggeredGrid & grid )
{
   Array<Real> & p = grid.p();
   Array<Real> & rhs = grid.rhs();
   size_t imax = p.getSize(0) -2;
   size_t jmax = p.getSize(1) -2;
   Real dx = grid.dx();
   Real dy = grid.dy();
   Real dx2 = 1.0 / (dx*dx);
   Real dy2 = 1.0 / (dy*dy);
   Real dx2dy2 = 1.0 / (dx*dx*dy*dy);
   //Copy paste boundaries
   for(size_t j = 1; j <= jmax; ++j)
   {
      p(0, j) = p(1, j);
      p(imax + 1, j) = p(imax, j);
   }
   for(size_t i = 1; i <= imax; ++i)
   {
      p(i, 0) = p(i, 1);
      p(i, jmax + 1) = p(i, jmax);
   }
   for(unsigned int nIter = 0; nIter < itermax_; ++nIter)
   {
      //SOR iteration
      for(size_t j = 1; j <= jmax; ++j)
      {
         for(size_t i = 1; i <= imax; ++i)
         {
            if(grid.isSolid(i,j))
               continue;
            Real tmp1 = (1.0 - omg_)*p(i,j);
            Real tmp2 = omg_ / (2*dx2 + 2*dy2);
            Real tmp3 = (grid.p(i+1,j, EAST) + grid.p(i-1,j, WEST))*dx2;
            Real tmp4 = (grid.p(i,j+1, NORTH) + grid.p(i,j-1, SOUTH))*dy2;
            Real tmp5 = (tmp3 + tmp4) - rhs(i,j);
            Real tmp6 = tmp2 * tmp5;
            p(i,j) = tmp1 + tmp6;
         }
      }

      //Copy paste boundaries
      #pragma omp parallel for
      for(size_t j = 1; j <= jmax; ++j)
      {
         p(0, j) = p(1, j);
         p(imax + 1, j) = p(imax, j);
      }
      #pragma omp parallel for
      for(size_t i = 1; i <= imax; ++i)
      {
         p(i, 0) = p(i, 1);
         p(i, jmax + 1) = p(i, jmax);
      }

      if(nIter % epsFrequency_ == 0)
      {
         //Calculate r
         Real r = 0.0;
         #pragma omp parallel for reduction(+:r)
         for(size_t j = 1; j <= jmax; ++j)
         {
            for(size_t i = 1; i <= imax; ++i)
            {
               if(grid.isSolid(i,j))
                  continue;
               Real rTmp1 = (2.0 * (dx*dx + dy*dy) ) * dx2dy2 * p(i,j);
               Real rTmp2 = (grid.p(i+1,j, EAST) + grid.p(i-1,j, WEST))*dx2;
               Real rTmp3 = (grid.p(i,j+1, NORTH) + grid.p(i,j-1, SOUTH))*dy2;
               Real rTmp4 = rTmp1 + rhs(i,j) - (rTmp2 + rTmp3);
               r += rTmp4 * rTmp4;
            }
         }
         
         r = r  / ((Real)(grid.getNumFluid()));
         r = sqrt( r );

         if( r < eps_)
         {
            return true;
         }
      }
   }

   //No more omp here
   WARN("Did not stop on eps_");
#ifndef NDEBUG
      std::ofstream myfile;
      myfile.open("text.txt", std::ios::out);
      myfile << p;
      myfile << std::endl;
      myfile.close();
#endif //NDEBUG
   return false;
}
