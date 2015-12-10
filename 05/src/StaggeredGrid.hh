#pragma once

#include <cmath>
#include <ctgmath>

#define PI           3.14159265358979323846

#include "Types.hh"
#include "Array.hh"
#include "FileReader.hh"

//**************************************
/*! Class for storing all arrays required for the simulation
*
* For now it only contains an array for the pressure and another array for the
* right hand side of the pressure equation.
* In following assignments this will be extended and also contain
* arrays for x/y velocity and some arrays for intermediate values.
*
* Feel free to add member functions or variables to this class, but please don't
* delete or rename any existing functions, since future skeletons rely on these functions.
*
*/
//**************************************
class StaggeredGrid
{
public:
   // Constructors to manually create staggered grid
   StaggeredGrid( size_t, size_t, Real, Real);

   // Constructor to create a staggered grid from a parsed configuration file
   StaggeredGrid( const FileReader & );


   // f(x,y) = 0
   void initGridSetup1( );
   
   // f(x,y) = sin(2xPI)
   void initGridSetup2( );

   void normalizePressure();

   void createRectangle(size_t x1, size_t y1, size_t x2, size_t y2);

   void createCircle(size_t x, size_t y, size_t r);

   // Getters / Setters for member variables
   Array<Real> & p()    { return p_;    }
   Array<Real> & rhs()  { return rhs_;  }
   Array<Real> & u()    { return u_;    }
   Array<Real> & v()    { return v_;    }
   Array<Real> & f()    { return f_;    }
   Array<Real> & g()    { return g_;    }
   Array<bool> & isFluids()  { return isFluids_;  }

   const Array<Real> & p()   const { return p_;   }
   const Array<Real> & rhs() const { return rhs_; }
   const Array<Real> & u()   const { return u_;   }
   const Array<Real> & v()   const { return v_;   }
   const Array<Real> & f()   const { return f_;   }
   const Array<Real> & g()   const { return g_;   }
   const Array<bool> & isFluids()  const { return isFluids_;  }

   Real & dx()    { return dx_; }
   const Real & dx() const { return dx_; }
   Real & dy()    { return dy_; }
   const Real & dy() const { return dy_; }

   size_t xSize() const    { return xSize_; }
   size_t ySize() const    { return ySize_; }
   
   inline void setCellToObstacle(size_t x, size_t y)
   {
      if(isFluids_(x,y))
         nFluid_--;
      isFluids_(x,y) = false;
   }

   inline void setCellToFluid(size_t x, size_t y)
   {
      if(!isFluids_(x,y))
         nFluid_++;
      isFluids_(x,y) = true;
   }

   inline Real u(size_t i, size_t j, DIRECTION dir) const
   {
      switch(dir){
         case NORTH:
            if(isFluid(i,j) && isFluid(i+1,j))
               return u_(i,j);
            else if(isSolid(i,j) && isSolid(i+1,j))
               return -u_(i,j-1);
            break;
         case SOUTH:
            if(isFluid(i,j) && isFluid(i+1,j))
               return u_(i,j);
            else if(isSolid(i,j) && isSolid(i+1,j))
               return -u_(i,j+1);
            break;
         case EAST:
         case WEST:
            if(isFluid(i,j) && isFluid(i+1,j))
               return u_(i,j);
            break;
         default:
            ABORT("No direction given");
      }

      // else
      return 0;
   }

   inline Real v(size_t i, size_t j, DIRECTION dir) const
   {
      switch(dir){
         case NORTH:
         case SOUTH:
            if(isFluid(i,j) && isFluid(i,j+1))
               return v_(i,j);
            break;
         case EAST:
            if(isFluid(i,j) && isFluid(i,j+1))
               return v_(i,j);
            else if(isSolid(i,j) && isSolid(i,j+1))
               return -v_(i-1,j);
            break;
         case WEST:
            if(isFluid(i,j) && isFluid(i,j+1))
               return v_(i,j);
            else if(isSolid(i,j) && isSolid(i,j+1))
               return -v_(i+1,j);
            break;
         default:
            ABORT("No direction given");
      }
      // else
      return 0;
   }

   inline Real p(size_t i, size_t j, DIRECTION dir) const
   {
      if(isFluid(i,j))
         return p_(i,j);
      switch(dir){
         case NORTH:
            return p_(i,j-1);
         case SOUTH:
            return p_(i,j+1);
         case EAST:
            return p_(i-1,j);
         case WEST:
            return p_(i+1,j);
         default:
            ABORT("No direction given");
      }
      ABORT("Should not be here!");
      return 0.0;
   }

   inline bool isFluid(size_t i, size_t j) const
   {
      return isFluids_(i,j);
   }

   inline bool isSolid(size_t i, size_t j) const
   {
      return not isFluids_(i,j);
   }

   inline size_t getNumFluid()
   {
      return nFluid_;
   }

   inline Real dpdx(size_t i, size_t j) const
   {
      return (p(i+1,j, EAST) - p_(i,j)) / dx_;
   }

   inline Real dpdy(size_t i, size_t j) const
   {
      return (p(i,j+1, NORTH) - p_(i,j)) / dy_;
   }

   inline Real d2udx2(size_t i, size_t j) const
   {
      return ( u(i+1,j, EAST) - 2.0*u_(i,j) + u(i-1,j, WEST) ) / (dx_*dx_);

   }

   inline Real d2udy2(size_t i, size_t j) const
   {
      return ( u(i,j+1, NORTH) - 2.0*u_(i,j) + u(i,j-1, SOUTH) ) / (dy_*dy_);

   }

   inline Real du2dx(size_t i, size_t j, Real gamma) const
   {
      Real tmp0 = ( (u_(i,j) + u(i+1,j, EAST) ) *0.5);
      Real tmp1 = ( (u(i-1,j, WEST) + u_(i,j) ) *0.5);
      Real tmp2 = (1.0/dx_) * ( tmp0*tmp0 - tmp1*tmp1 );
      
      Real tmp3 = (fabs( u_(i,j) + u(i+1,j, EAST) ) *0.5) * (( u_(i,j) - u(i+1,j, EAST) ) *0.5);
      Real tmp4 = (fabs( u(i-1,j, WEST) + u_(i,j) ) *0.5) * (( u(i-1,j, WEST) - u_(i,j) ) *0.5);
      Real tmp5 = ( gamma / dx_ ) * ( tmp3 - tmp4 );
      return tmp2 + tmp5;
   }

   inline Real duvdy(size_t i, size_t j, Real gamma) const
   {
      Real tmp0 = (( v_(i,j) + v(i+1,j, EAST) ) *0.5) * (( u_(i,j) + u(i,j+1, NORTH) ) *0.5);
      Real tmp1 = (( v(i,j-1, SOUTH) + v(i+1,j-1, SOUTH) ) *0.5) * (( u(i,j-1, SOUTH) + u_(i,j) ) *0.5);
      Real tmp2 = (1.0/dy_) * ( tmp0 - tmp1 );

      Real tmp3 = (fabs( v_(i,j) + v(i+1, j, EAST) )*0.5) * ( ( u_(i,j)  - u(i,j+1, NORTH)) *0.5);
      Real tmp4 = (fabs( v(i,j-1, SOUTH) + v(i+1, j-1, SOUTH) )*0.5) * ( ( u(i,j-1, SOUTH)- u_(i,j) ) *0.5);
      Real tmp5 = ( gamma / dy_ ) * (tmp3 - tmp4);
      return tmp2 + tmp5;
   }

   inline Real d2vdx2( size_t i, size_t j) const
   {
      return ( v(i+1,j, EAST) - 2.0*v_(i,j) + v(i-1,j, WEST) ) / (dx_*dx_);
   }

   inline Real d2vdy2( size_t i, size_t j) const
   {
      return ( v(i,j+1, NORTH) - 2.0*v_(i,j) + v(i,j-1, SOUTH) ) / (dy_*dy_);

   }

   inline Real duvdx( size_t i, size_t j, Real gamma) const
   {
      Real tmp0 = (( u_(i,j) + u(i,j+1, NORTH) ) *0.5) * (( v_(i,j) + v(i+1,j, EAST) ) *0.5);
      Real tmp1 = (( u(i-1,j, WEST) + u(i-1, j+1, WEST) ) *0.5) * (( v(i-1, j, WEST) + v_(i,j) ) *0.5);
      Real tmp2 = (1.0 /(dx_)) * ( tmp0 - tmp1 );

      Real tmp3 = (fabs( u_(i,j) + u(i,j+1, NORTH) ) *0.5) * (( v_(i,j) -v(i+1,j, EAST) ) *0.5);
      Real tmp4 = (fabs( u(i-1,j, WEST) + u(i-1,j+1, WEST) ) *0.5) * ( (v(i-1,j, WEST) - v_(i,j)) *0.5);
      Real tmp5 = ( gamma / dx_) * ( tmp3 - tmp4 );
      return tmp2 + tmp5;
   }

   inline Real dv2dy( size_t i, size_t j, Real gamma) const
   {
      Real tmp0 = (( v_(i,j) + v(i,j+1, NORTH) ) *0.5);
      Real tmp1 = (( v(i,j-1, SOUTH) + v_(i,j) ) *0.5);
      Real tmp2 = (1.0/ dy_) * ( tmp0*tmp0 - tmp1*tmp1 );

      Real tmp3 = (fabs(v_(i,j) + v(i,j+1, NORTH) ) *0.5) * ((v_(i,j) - v(i,j+1, NORTH) ) *0.5);
      Real tmp4 = (fabs(v(i,j-1, SOUTH) + v_(i,j)) *0.5) * (( v(i,j-1, SOUTH) - v_(i,j)) *0.5);
      Real tmp5 = (gamma / dy_) * ( tmp3 - tmp4);
      return tmp2 + tmp5;
   }

protected:
   Array<Real> p_;   //< pressure field
   Array<Real> rhs_; //< right hand side of the pressure equation
   Array<Real> u_;   //< horizontal velocity field
   Array<Real> v_;   //< vertical velocity field
   Array<Real> f_;   //< horizontal intermediate result
   Array<Real> g_;   //< vertical intermediate result
   Array<bool> isFluids_; //< is cell fluid

   Real dx_;   //< distance between two grid points in x direction
   Real dy_;   //< distance between two grid points in y direction
   size_t xSize_; //< aka imax
   size_t ySize_; //< aka jmax
   size_t nFluid_; //< number of fluid cells
};


