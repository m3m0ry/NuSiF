#include "SORSolver.hh"

// Constructor to manually create SORSolver
SORSolver::SORSolver( int itermax, Real eps, Real omg ) : itermax_(itermax), eps_(eps), omg_(omg)
{

}

// Constructor to create a SORSolver from a parsed configuration file
SORSolver::SORSolver ( const FileReader & configuration )
{
	itermax_ = configuration.getIntParameter("itermax");
	eps_ = configuration.getRealParameter("eps");
	omg_ = configuration.getRealParameter("omg");
}



// Solve the pressure equation on the staggered grid
bool SORSolver::solve( StaggeredGrid & grid )
{
	Array & p = grid.p();
	Array & rhs = grid.rhs();
	size_t imax = p.getSize(0) -2;
	size_t jmax = p.getSize(1) -2;
	Real dx = grid.dx();
	Real dy = grid.dy();
	Real rFactor = omg_ * (dx*dx*dy*dy/(2.0*(dx*dx+dy*dy)));

	for(int i = 0; i < itermax_; ++i)
	{
		//Copy paste boundaries
		for(size_t j = 1; j < jmax + 1; ++j)
		{
			p(0, j) = p(1, j);
			p(imax + 1, j) = p(imax, j);
		}
		for(size_t i = 1; i < imax + 1; ++i)
		{
			p(i, 0) = p(i, 1);
			p(i, jmax + 1) = p(i, jmax);
		}

		//SOR iteration
		//Calculate r
		Real r = 0.0;
		for(size_t j = 1; j < jmax +1; ++j)
		{
			for(size_t i = 1; i < imax + 1; ++i)
			{
				Real rTmp1 = (p(i+1,j) - 2.0 * p(i,j) + p(i-1,j))/(dx*dx);
				Real rTmp2 = (p(i,j+1) - 2.0 * p(i,j) + p(i,j-1))/(dy*dy);
				Real rTmp = rhs(i,j) - (rTmp1 + rTmp2);
				r += rTmp;
				p(i,j) = p(i,j) - rFactor * rTmp;
			}
		}
		
		r = r  / (Real) (imax * jmax);
		r = sqrt( r );

		if( r < eps_)
		{
			std::cout << "i = " << i << " r = " << r << std::endl;
			return true;
		}
		std::cout << "i = " << i << " r = " << r << std::endl;
	}

	p.print();
	return false;

}
