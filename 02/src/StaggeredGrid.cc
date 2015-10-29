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




void StaggeredGrid::initGridSetup1( )
{
	rhs_.fill(0.0);
}

void StaggeredGrid::initGridSetup2( )
{
	//TODO rhs_ = sin(2xPI)

}
