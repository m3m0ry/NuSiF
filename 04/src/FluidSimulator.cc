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
	tau_ = conf.getRealParameter("safetyfactor");
}



void FluidSimulator::simulate( Real duration )
{
	computeFG();
	composeRHS();
	solver_.solve(grid_);
	updateVelocities();
	determineNextDT();

}



void FluidSimulator::simulateTimeStepCount( unsigned int nrOfTimeSteps )
{
	computeFG();
}



void refreshBoundaries()
{


}



void determineNextDT()
{
	Real & dx = grid_.dx();
	Real & dy = grid_.dy();

	if( tau_ <= 0.0 )
		return;
	Real umax = 0.0;
	for(size_t j = 0; i <= jmax_+1; ++j)
	{
		for(size_t i = 0; j <= imax_; ++i)
		{
			Real abs = fabs(u(i,j));
			if( umax < abs )
				umax = abs;
		}
	}

	Real vmax = 0.0;
	for(size_t j = 0; i <= jmax_; ++j)
	{
		for(size_t i = 0; j <= imax_+1; ++i)
		{
			Real abs = fabs(u(i,j));
			if( vmax < abs )
				vmax = abs;
		}
	}

	Real min1 = (re_ / 2) / ( 1 / (dx*dx) + 1/(dy*dy));
	Real min2 = dx / umax;
	Real min3 = dy / vmax;
	Real min = 0.0;
	if(min1 < min2)
		if(min1 < min3)
			min = min1;
		else
			min = min3;
	else
		if(min2 < min3)
			min = min2;
		else
			min = min3;

	dt_ = tau_ * min;
}




void updateVelocities()
{
	Array & u = grid_.u();
	Array & v = grid_.v();
	Array & F = grid_.f();
	Array & G = grid_.g();

	for(size_t j = 1; i <= jmax_; ++j)
	{
		for(size_t i = 1; j <= imax_-1; ++i)
		{
			u(i,j) = F(i,j) - dt_ * dpdx;
		}
	}
	
	for(size_t j = 1; i <= jmax_-1; ++j)
	{
		for(size_t i = 1; j <= imax_; ++i)
		{
			v(i,j) = G(i,j) - dt_ * dpdy;
		}
	}
}


void composeRHS()
{

	Array & rhs = grid_.rhs();
	Array & F = grid_.f();
	Array & G = grid_.g();
	int & dx = grid_.dx();
	int & dy = grid_.dy();


	for(size_t j = 1; i <= jmax_; ++j)
	{
		for(size_t i = 1; j <= imax_; ++i)
		{
			rhs(i,j) = (1.0 / dt_) * ( ( F(i,j) - F(i-1,j) )/dx + ( G(i,j) - G(i,j-1) )/dy);

		}
	}
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

	for(size_t j = 1; i <= jmax_; ++j)
	{
		for(size_t i = 1; j < imax_; ++i)
		{
			F(i,j) = u(i,j) + dt_* ( (1.0 / re_) * ( grid_.d2udx2(i,j) + grid_.d2udy2(i,j)) - grid_.du2dx(i,j,gamma_) - grid_.duvdy(i,j, gamma_) + gx_);
		}
	}
	for(size_t j = 1; i < jmax_; ++j)
	{
		for(size_t i = 1; i <= imax_; ++i)
		{
			G(i,j) = v(i,j) + dt_* ( (1.0 / re_) * ( grid_.d2vdx2(i,j) + grid_.d2vdy2(i,j)) - grid_.duvdx(i,j, gamma_) - grid_.dv2dy(i,j, gamma_) + gy_);
		}
	}
}
