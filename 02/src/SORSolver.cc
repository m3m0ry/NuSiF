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



// solve the pressure equation on the staggered grid
bool SORSolver::solve( StaggeredGrid & grid )
{
	for(int i = 0; i < itermax_; ++i)
	{
		//TODO Copy paste boundaries


		//TODO SOR iteration
		

		//TODO Copy paste boundaries


		//TODO calculate r
		Real r = 0.0;
		Real rTmp = 0.0;
		Array & p = grid.p();
		Array & rhs = grid.rhs();
		size_t imax = p.getSize(0);
		size_t jmax = rhs.getSize(1);
		for(size_t i = 1; i < imax + 1; ++i )
		{
			for(size_t j = 1; j < jmax + 1; ++j )
			{
				rTmp = pow( rhs(i,j) - p(i,j)  , 2 );
				r += rTmp;
			}
		}
		r = r  / (Real) (imax * jmax);
		r = sqrt( r );


		if( r < eps_)
			return true;
	}

	return false;

}
