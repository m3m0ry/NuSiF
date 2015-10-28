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



		if( r < eps_)
			return true;
	}

	return false;

}
