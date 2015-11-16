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
   StaggeredGrid( int xSize, int ySize, Real dx, Real dy );

   // Constructor to create a staggered grid from a parsed configuration file
   StaggeredGrid( const FileReader & configuration );


	// f(x,y) = 0
	void initGridSetup1( );
	
	// f(x,y) = sin(2xPI)
	void initGridSetup2( );

   // Getters / Setters for member variables
   Array & p()    { return p_;    }
   Array & rhs()  { return rhs_;  }
   Array & u()    { return u_;    }
   Array & v()    { return v_;    }
   Array & f()    { return f_;    }
   Array & g()    { return g_;    }

   const Array & p()   const { return p_;   }
   const Array & rhs() const { return rhs_; }
   const Array & u()   const { return u_;   }
   const Array & v()   const { return v_;   }
   const Array & f()   const { return f_;   }
   const Array & g()   const { return g_;   }

   Real dx() const { return dx_; }
   Real dy() const { return dy_; }


	inline Real dpdx(size_t i, size_t j) const
	{
		return (p_(i+1,j) - p(i,j)) / dx_;
	}

	inline Real dpdy(size_t i, size_t j) const
	{
		return (p_(i,j+1) - p(i,j)) / dy_;
	}

	inline Real d2udx2(size_t i, size_t j) const
	{
		return ( u_(i+1,j) - 2.0*u_(i,j) + u_(i-1,j) ) / (dx_*dx_);

	}

	inline Real d2udy2(size_t i, size_t j) const
	{
		return ( u_(i,j+1) - 2.0*u_(i,j) + u_(i,j-1) ) / (dy_*dy_);

	}

	inline Real du2dx(size_t i, size_t j, Real gamma) const
	{
		Real tmp0 = ( (u_(i,j) + u_(i+1,j) ) /2);
		Real tmp1 = ( (u_(i-1,j) + u_(i,j) ) /2);
		Real tmp2 = (1.0/dx_) * ( tmp0*tmp0 - tmp1*tmp1 );
		
		Real tmp3 = (fabs( u_(i,j) + u_(i+1,j) ) /2) * (( u_(i,j) - u_(i+1,j) ) /2);
		Real tmp4 = (fabs( u_(i-1,j) + u_(i,j) ) /2) * (( u_(i-1,j) - u_(i,j) ) /2);
		Real tmp5 = ( gamma / dx_ ) * ( tmp3 - tmp4 );
		return tmp2 + tmp5;
	}

	inline Real duvdy(size_t i, size_t j, Real gamma) const
	{
		Real tmp0 = (( v_(i,j) + v_(i+1,j) ) /2) * (( u_(i,j) + u_(i,j+1) ) /2);
		Real tmp1 = (( v_(i,j-1) + v_(i+1,j-1) ) /2) * (( u_(i,j-1) + u_(i,j) ) /2);
		Real tmp2 = (1.0/dy_) * ( tmp0 - tmp1 );

		Real tmp3 = (fabs( v_(i,j) + v_(i+1, j) )/2) * ( ( u_(i,j)  - u_(i,j+1)) /2);
		Real tmp4 = (fabs( v_(i,j-1) + v_(i+1, j-1) )/2) * ( ( u_(i,j-1)- u_(i,j) ) /2);
		Real tmp5 = ( gamma / dy_ ) * (tmp3 - tmp4);
		return tmp2 + tmp5;
	}

	inline Real d2vdx2( size_t i, size_t j) const
	{
		return ( v_(i+1,j) - 2.0*v_(i,j) + v_(i-1,j) ) / (dx_*dx_);
	}

	inline Real d2vdy2( size_t i, size_t j) const
	{
		return ( v_(i,j+1) - 2.0*v_(i,j) + v_(i,j-1) ) / (dy_*dy_);

	}

	inline Real duvdx( size_t i, size_t j, Real gamma) const
	{
		Real tmp0 = (( u_(i,j) + u_(i,j+1) ) /2) * (( v_(i,j) + v_(i+1,j) ) /2);
		Real tmp1 = (( u_(i-1,j) + u_(i-1, j+1) ) /2) * (( v_(i-1, j) + v_(i,j) ) /2);
		Real tmp2 = (1.0 /(dx_)) * ( tmp0 - tmp1 );

		Real tmp3 = (fabs( u_(i,j) + u_(i,j+1) ) /2) * (( v_(i,j) -v_(i+1,j) ) /2);
		Real tmp4 = (fabs( u_(i-1,j) + u_(i-1,j+1) ) /2) * ( (v_(i-1,j) - v_(i,j)) /2);
		Real tmp5 = ( gamma / dx_) * ( tmp3 - tmp4 );
		return tmp2 + tmp5;
	}

	inline Real dv2dy( size_t i, size_t j, Real gamma) const
	{
		Real tmp0 = (( v_(i,j) + v_(i,j+1) ) /2);
		Real tmp1 = (( v_(i,j-1) + v_(i,j) ) /2);
		Real tmp2 = (1.0/ dy_) * ( tmp0*tmp0 - tmp1*tmp1 );

		Real tmp3 = (fabs(v_(i,j) + v_(i,j+1) ) /2) * ((v_(i,j) - v_(i,j+1) ) /2);
		Real tmp4 = (fabs(v_(i,j-1) + v_(i,j)) /2) * (( v_(i,j-1) - v_(i,j)) /2);
		Real tmp5 = (gamma / dy_) * ( tmp3 - tmp4);
		return tmp2 + tmp5;
	}

protected:
   Array p_;   //< pressure field
   Array rhs_; //< right hand side of the pressure equation
	Array u_;	//< horizontal velocity field
	Array v_;	//< vertical velocity field
	Array f_;	//< horizontal intermediate result
	Array g_;	//< vertical intermediate result

   Real dx_;   //< distance between two grid points in x direction
   Real dy_;   //< distance between two grid points in y direction
};


