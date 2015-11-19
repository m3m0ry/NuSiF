#include "StaggeredGrid.hh"

//==================
//
//  Constructors
//
//==================

//Direct Constructor
StaggeredGrid::StaggeredGrid( int xSize, int ySize, Real dx, Real dy ) : dx_(dx), dy_(dy), xSize_(xSize), ySize_(ySize)
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
	xSize_ = configuration.getIntParameter("imax");
	ySize_ = configuration.getIntParameter("jmax");

	p_ = Array(xSize_ + 2, ySize_ + 2);
	rhs_ = Array(xSize_ + 2, ySize_ + 2);
	u_ = Array(xSize_ + 1, ySize_ + 2);
	v_ = Array(xSize_ + 2, ySize_ + 1);
	f_ = Array(xSize_ + 1, ySize_ + 2);
	g_ = Array(xSize_ + 2, ySize_ + 1);

	dx_ = configuration.getRealParameter("xlength")/(xSize_);
	dy_ = configuration.getRealParameter("ylength")/(ySize_);
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
			rhs_(i,j) = sin(2.0 * PI * ((i-1)*dx_ + (dx_/2)));
		}
	}

}
