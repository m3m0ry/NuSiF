#include "StaggeredGrid.hh"

//==================
//
//  Constructors
//
//==================

//Direct Constructor
StaggeredGrid::StaggeredGrid( int xSize, int ySize, Real dx, Real dy ) : dx_(dx), dy_(dy)
{
	p_ = Array(xSize + 2, ySize + 2);
	rhs_ = Array(xSize + 2, ySize + 2);
}

//Constructor from configuration
StaggeredGrid::StaggeredGrid( const FileReader & configuration )
{
	int xSize = configuration.getIntParameter("imax");
	int ySize = configuration.getIntParameter("jmax");

	p_ = Array(xSize + 2, ySize + 2);
	rhs_ = Array(xSize + 2, ySize + 2);

	dx_ = configuration.getRealParameter("xlength")/(xSize);
	dy_ = configuration.getRealParameter("ylength")/(ySize);
}



// f(x,y) = 0
void StaggeredGrid::initGridSetup1( )
{
	rhs_.fill(0.0);
}

// f(x,y) = sin(2xPI)
void StaggeredGrid::initGridSetup2( )
{
	for(size_t j = 0; j < rhs_.getSize(0); ++j)
	{
		for(size_t i = 0; i < rhs_.getSize(1); ++i)
		{
			rhs_(i,j) = sin(2.0 * i * PI);
		}
	}

}
