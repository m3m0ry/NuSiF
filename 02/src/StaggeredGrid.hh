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

   const Array & p()   const { return p_;   }
   const Array & rhs() const { return rhs_; }

   Real dx() const { return dx_; }
   Real dy() const { return dy_; }

protected:
   Array p_;   //< pressure field
   Array rhs_; //< right hand side of the pressure equation

   Real dx_;   //< distance between two grid points in x direction
   Real dy_;   //< distance between two grid points in y direction
};


