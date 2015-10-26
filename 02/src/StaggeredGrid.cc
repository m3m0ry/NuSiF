#include "StaggeredGrid.hh"


StaggeredGrid ( int xSize, int ySize, Real dx, Real dy ) : dx_(dx), dy_(dy)
{
	p_ = Array(xSize + 2, ySize + 2);
	rhs_ = Array(xSize + 2, ySize + 2);
}

StaggeredGrid ( const FileReader & configuration )
{
	int xSize = configuration->getIntParameter("imax");
	int ySize = configuration->getIntParameter("jmax");
	p_ = Array(xSize + 2, ySize + 2);
	rhs_ = Array(xSize + 2, ySize + 2);
	dx_ = configuration->getRealParameter("xlength");
	dy_ = configuration->getRealParameter("ylength");
}
