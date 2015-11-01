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

	for(int i = 0; i < 10; ++i)
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

		p.print();
		std::cout << "dx=" << dx << "dy=" << dy << "omg=" << omg_ << std::endl;


		//SOR iteration
		for(size_t i = 1; i < imax +1; ++i)
		{
			for(size_t j = 1; j < jmax + 1; ++j)
			{
				Real tmp1 = (1.0 - omg_) * p(i,j);
				Real tmp2 = omg_ / ( (2.0/(dx*dx)) + (2.0/(dy*dy)));
				Real tmp3 = ( p(i+1,j) + p(i-1,j) ) / (dx*dx);
				Real tmp4 = ( p(i,j+1) + p(i,j-1) ) / (dy*dy);
				Real tmp5 = (tmp3 + tmp4) - rhs(i,j);
				Real tmp6 = tmp2 * tmp5;
				//std::cout << "tmp1="<< tmp1 << " tmp2= " << tmp2 << " tmp5=" << tmp5 << " tmp6=" << tmp6 << std::endl;
				p(i,j) = tmp1 + tmp6;
				std::cout << p(i,j) << std::endl;
			}
		}
		
		p.print();

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

		//Calculate r
		Real r = 0.0;
		for(size_t i = 1; i < imax + 1; ++i )
		{
			for(size_t j = 1; j < jmax + 1; ++j )
			{

				Real rTmp1 = 2.0*(dx*dx + dy*dy)/(dx*dx*dy*dy) * p(i,j);
				Real rTmp2 = (p(i+1,j) + p(i-1,j))/(dx*dx);
				Real rTmp3 = (p(i,j+1) + p(i,j-1))/(dy*dy);
				Real rTmp4 = rTmp2 + rTmp3;
				Real rTmp = rTmp1 - rTmp4 + rhs(i,j);
				rTmp = rTmp * rTmp;
				r += rTmp;
			}
		}
		r = r  / (Real) (imax * jmax);
		r = sqrt( r );

		if( r < eps_)
		{
			//p.print();
			std::cout << "i = " << i << " r = " << r << std::endl;
			return true;
		}
			//std::cout << "i = " << i << " r = " << r << std::endl;
	}

	p.print();
	return false;

}
