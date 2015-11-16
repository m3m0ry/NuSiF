#include "FluidSimulator.hh"

// Constructor
FluidSimulator::FluidSimulator( const FileReader & conf ) : grid_(StaggeredGrid(conf)),
		solver_(SORSolver(conf))
{
	gx_ = conf.getRealParameter("gx");
	gy_ = conf.getRealParameter("gy");
	re_ = conf.getRealParameter("Re");
	dt_ = conf.getRealParameter("dt");
	gamma_ = conf.getRealParameter("gamma");
	imax_ = conf.getIntParameter("imax");
	jmax_ = conf.getIntParameter("jmax");
}



void FluidSimulator::simulate( Real duration )
{

	computeFG();

}



void FluidSimulator::simulateTimeStepCount( unsigned int nrOfTimeSteps )
{

	computeFG();
	
}



void FluidSimulator::computeFG()
{
	Array & F = grid_.f();
	Array & G = grid_.g();
	Array & u = grid_.u();
	Array & v = grid_.v();

	for(size_t j = 1; j <= jmax_; ++j)
	{
		F(0,j) = u(0,j);
		F(imax_,j) = u(imax_, j);
	}

	for(size_t i = 1; i <= imax_; ++i)
	{
		G(i,0) = v(i,0);
		G(i, jmax_) = v(i, jmax_);
	}

	for(size_t j = 1; j <= jmax_; ++j)
	{
		for(size_t i = 1; i < imax_; ++i)
		{
			F(i,j) = u(i,j) + dt_* ( (1.0 / re_) * ( grid_.d2udx2(i,j) + grid_.d2udy2(i,j)) - grid_.du2dx(i,j,gamma_) - grid_.duvdy(i,j, gamma_) + gx_);
		}
	}
	for(size_t j = 1; j < jmax_; ++j)
	{
		for(size_t i = 1; i <= imax_; ++i)
		{
			G(i,j) = v(i,j) + dt_* ( (1.0 / re_) * ( grid_.d2vdx2(i,j) + grid_.d2vdy2(i,j)) - grid_.duvdx(i,j, gamma_) - grid_.dv2dy(i,j, gamma_) + gy_);
		}
	}
}
