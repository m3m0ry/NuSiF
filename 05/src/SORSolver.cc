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

void SORSolver::sor(StaggeredGrid & grid)
{
   Array<Real> & p = grid.p();
   Array<Real> & rhs = grid.rhs();
   size_t imax = p.getSize(0) -2;
   size_t jmax = p.getSize(1) -2;
   Real dx = grid.dx();
   Real dy = grid.dy();
   Real dx2 = 1.0 / (dx*dx);
   Real dy2 = 1.0 / (dy*dy);

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
}

void SORSolver::sorRedBlack(StaggeredGrid & grid)
{
   Array<Real> & p = grid.p();
   Array<Real> & rhs = grid.rhs();
   size_t imax = p.getSize(0) -2;
   size_t jmax = p.getSize(1) -2;
   Real dx = grid.dx();
   Real dy = grid.dy();
   Real dx2 = 1.0 / (dx*dx);
   Real dy2 = 1.0 / (dy*dy);

   //SOR iteration red
   #pragma omp parallel for
   for(size_t j = 1; j <= jmax; ++j)
   {
      for(size_t i = 1; i <= imax; i+=2)
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
   //Sor iteration black
   #pragma omp parallel for
   for(size_t j = 1; j <= jmax; ++j)
   {
      for(size_t i = 2; i <= imax; i+=2)
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
}

//Do not use! not finished
void SORSolver::sorParallel(StaggeredGrid & grid)
{
   Array<Real> & p = grid.p();
   Array<Real> & rhs = grid.rhs();
   size_t imax = p.getSize(0) -2;
   size_t jmax = p.getSize(1) -2;
   size_t numThreads = 1;
   size_t threadId = 0;
   Real dx = grid.dx();
   Real dy = grid.dy();
   Real dx2 = 1.0 / (dx*dx);
   Real dy2 = 1.0 / (dy*dy);

   //SOR iteration
   #pragma omp parallel private(threadId)
   {
#if defined(_OPENMP)
      threadId = omp_get_thread_num();
      #pragma omp single
      {
         numThreads = omp_get_num_threads();
      }
#endif
      size_t iStart = (imax+1)/numThreads * threadId;
      size_t iEnd = iStart + (imax+1)/numThreads;

      for(size_t j = 1; j <= jmax + numThreads -1; ++j)
      {
         size_t k = j - threadId;
         if(k > 0 && k <= jmax){
            for(size_t i = iStart; i < iEnd; ++i)
            {
               if(grid.isSolid(i,k))
                  continue;
               Real tmp1 = (1.0 - omg_)*p(i,k);
               Real tmp2 = omg_ / (2*dx2 + 2*dy2);
               Real tmp3 = (grid.p(i+1,k, EAST) + grid.p(i-1,k, WEST))*dx2;
               Real tmp4 = (grid.p(i,k+1, NORTH) + grid.p(i,k-1, SOUTH))*dy2;
               Real tmp5 = (tmp3 + tmp4) - rhs(i,k);
               Real tmp6 = tmp2 * tmp5;
               p(i,k) = tmp1 + tmp6;
            }
         }
         #pragma omp barrier
      }
   }
}

void SORSolver::updateBoundaries(StaggeredGrid & grid)
{
   Array<Real> & p = grid.p();
   size_t imax = p.getSize(0) -2;
   size_t jmax = p.getSize(1) -2;

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
}

Real SORSolver::computeResiduum(StaggeredGrid & grid)
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

   // Compute residuum
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
   return r;
}


// Solve the pressure equation on the staggered grid
bool SORSolver::solve( StaggeredGrid & grid )
{
   // Copy paste boundaries
   updateBoundaries(grid);
   for(unsigned int nIter = 0; nIter < itermax_; ++nIter)
   {
      // Solve pressure
      sorRedBlack(grid);

      // Copy paste boundaries
      updateBoundaries(grid);

      if(nIter % epsFrequency_ == 0)
      {
         // Calculate r
         Real r = computeResiduum(grid);

         if( r < eps_)
         {
            return true;
         }
      }
   }
   // Itermax reached
   WARN("Did not stop on eps_");
   return false;
}
