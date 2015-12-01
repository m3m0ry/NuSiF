#include "StaggeredGrid.hh"

//==================
//
//  Constructors
//
//==================

//Direct Constructor
StaggeredGrid::StaggeredGrid( size_t xSize, size_t ySize, Real dx, Real dy ) : dx_(dx), dy_(dy), xSize_(xSize), ySize_(ySize)
{
   p_ = Array<Real>(xSize_ + 2, ySize_ + 2);
   rhs_ = Array<Real>(xSize_ + 2, ySize_ + 2);
   u_ = Array<Real>(xSize_ + 1, ySize_ + 2);
   v_ = Array<Real>(xSize_ + 2, ySize_ + 1);
   f_ = Array<Real>(xSize_ + 1, ySize_ + 2);
   g_ = Array<Real>(xSize_ + 2, ySize_ + 1);
}

//Constructor from configuration
StaggeredGrid::StaggeredGrid( const FileReader & configuration )
{
   int test = configuration.getIntParameter("imax");
   CHECK_MSG(test >= 0, "imax is less then 0");
   xSize_ = (size_t) test;
   test = configuration.getIntParameter("jmax");
   CHECK_MSG(test >= 0, "jmax is less then 0");
   ySize_ = (size_t) test;

   p_ = Array<Real>(xSize_ + 2, ySize_ + 2);
   rhs_ = Array<Real>(xSize_ + 2, ySize_ + 2);
   u_ = Array<Real>(xSize_ + 1, ySize_ + 2);
   v_ = Array<Real>(xSize_ + 2, ySize_ + 1);
   f_ = Array<Real>(xSize_ + 1, ySize_ + 2);
   g_ = Array<Real>(xSize_ + 2, ySize_ + 1);

   dx_ = configuration.getRealParameter("xlength")/(Real) xSize_;
   dy_ = configuration.getRealParameter("ylength")/(Real) ySize_;
}



// f(x,y) = 0
void StaggeredGrid::initGridSetup1( )
{
   rhs_.fill(0.0);
}

// f(x,y) = sin(2xPI)
void StaggeredGrid::initGridSetup2( )
{
   for(size_t j = 1; j < rhs_.getSize(1)-1; ++j)
   {
      for(size_t i = 1; i < rhs_.getSize(0)-1; ++i)
      {
         rhs_(i,j) = sin(2.0 * PI * ((Real)(i-1)*dx_ + (dx_/2)));
      }
   }
}

void StaggeredGrid::normalizePressure()
{
   Real norm = 0.0;
   for(size_t j = 0; j < p_.getSize(1); ++j)
   {
      for(size_t i = 0; i < p_.getSize(0); ++i)
      {
         norm += p_(i,j);
      }
   }
   norm = norm / (p_.getSize(0) * p_.getSize(1));
   for(size_t j = 0; j < p_.getSize(1); ++j)
   {
      for(size_t i = 0; i < p_.getSize(0); ++i)
      {
         p_(i,j) = p_(i,j) - norm;
      }
   }

}
