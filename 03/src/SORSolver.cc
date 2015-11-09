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
	epsFrequency_ = configuration.getIntParameter("checkfrequency");
	normFrequency_ = configuration.getIntParameter("normalizationfrequency");
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
	for(int nIter = 0; nIter < itermax_; ++nIter)
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
		//for(size_t j = 1; j < jmax +1; ++j)
		//{
		//	for(size_t i = 1; i < imax +1; ++i)
		//	{
		//		Real tmp1 = (1.0 - omg_)*p(i,j);
		//		Real tmp2 = omg_ / (2/(dx*dx) + 2/(dy*dy));
		//		Real tmp3 = (p(i+1,j) + p(i-1,j))/(dx*dx);
		//		Real tmp4 = (p(i,j+1) + p(i,j-1))/(dy*dy);
		//		Real tmp5 = (tmp3 + tmp4) - rhs(i,j);
		//		Real tmp6 = tmp2 * tmp5;
		//		p(i,j) = tmp1 + tmp6;
		//	}
		//}
		//Calculate r
		Real r = 0.0;
		for(size_t j = 1; j < jmax +1; ++j)
		{
			for(size_t i = 1; i < imax + 1; ++i)
			{
				Real rTmp1 = (p(i+1,j) - 2.0 * p(i,j) + p(i-1,j))/(dx*dx);
				Real rTmp2 = (p(i,j+1) - 2.0 * p(i,j) + p(i,j-1))/(dy*dy);
				Real rTmp = rhs(i,j) - (rTmp1 + rTmp2);
				r += rTmp * rTmp;
				p(i,j) = p(i,j) - rFactor * rTmp;
			}
		}

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


		//for(size_t j = 1; j < jmax +1; ++j)
		//{
		//	for(size_t i = 1; i < imax +1; ++i)
		//	{
		//		Real rTmp1 = (2.0 * (dx*dx + dy*dy) ) / (dx*dx*dy*dy) * p(i,j);
		//		Real rTmp2 = (p(i+1,j) + p(i-1,j))/(dx*dx);
		//		Real rTmp3 = (p(i,j+1) + p(i,j-1))/(dy*dy);
		//		Real rTmp4 = rTmp1 + rhs(i,j) - (rTmp2 + rTmp3);
		//		r += rTmp4 * rTmp4;
		//		//std::cout <<  rTmp4 * rTmp4 << std::endl;
		//	}
		//}
		
		r = r  / ((Real)(imax * jmax));
		r = sqrt( r );

		if(nIter % epsFrequency_ == 0){
			if( r < eps_)
			{
#ifndef NDEBUG
				std::ofstream myfile;
				myfile.open("text.txt", std::ios::out);
				myfile << p;
				myfile << std::endl;
				myfile.close();
				std::cout << "i = " << nIter << " r = " << r << std::endl;
#endif //NDEBUG
				return true;
			}
		}
#ifndef NDEBUG
			std::cout << "i = " << nIter << " r = " << r << std::endl;
		if( nIter % 1000 == 0)
		{
			std::ofstream myfile;
			myfile.open("text.txt", std::ios::out);
			myfile << p;
			myfile << std::endl;
			myfile.close();
			DEBUG_PROGRESS("SOR-Progress", nIter, itermax_);
		}
#endif //NDEBUG
	}
	WARN("Did not stop on eps_");
	return false;
}
