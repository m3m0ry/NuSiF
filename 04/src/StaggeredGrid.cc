#include "StaggeredGrid.hh"

//==================
//
//  Constructors
//
//==================

//Direct Constructor
StaggeredGrid::StaggeredGrid( size_t xSize, size_t ySize, Real dx, Real dy ) : dx_(dx), dy_(dy), xSize_(xSize), ySize_(ySize)
{
   p_ = Array(xSize_ + 2, ySize_ + 2);
   rhs_ = Array(xSize_ + 2, ySize_ + 2);
   u_ = Array(xSize_ + 1, ySize_ + 2);
   v_ = Array(xSize_ + 2, ySize_ + 1);
   f_ = Array(xSize_ + 1, ySize_ + 2);
   g_ = Array(xSize_ + 2, ySize_ + 1);
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

   p_ = Array(xSize_ + 2, ySize_ + 2);
   rhs_ = Array(xSize_ + 2, ySize_ + 2);
   u_ = Array(xSize_ + 1, ySize_ + 2);
   v_ = Array(xSize_ + 2, ySize_ + 1);
   f_ = Array(xSize_ + 1, ySize_ + 2);
   g_ = Array(xSize_ + 2, ySize_ + 1);

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
