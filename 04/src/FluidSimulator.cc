#include "FluidSimulator.hh"

// Constructor
FluidSimulator::FluidSimulator( const FileReader & conf ) : grid_(StaggeredGrid(conf)),
      solver_(SORSolver(conf))
{
   // Init all values
   gx_ = conf.getRealParameter("GX");
   gy_ = conf.getRealParameter("GY");
   re_ = conf.getRealParameter("Re");
   dt_ = conf.getRealParameter("dt");
   gamma_ = conf.getRealParameter("gamma");
   int test = conf.getIntParameter("imax");
   CHECK_MSG(test >= 0, "Imax is lesser then 0");
   imax_ = (size_t) test;
   test = conf.getIntParameter("jmax");
   CHECK_MSG(test >= 0, "Jmax is lesser then 0");
   jmax_ = (size_t) test;
   tau_ = conf.getRealParameter("safetyfactor");
   test = conf.getIntParameter("normalizationfrequency");
   CHECK_MSG(test >= 0, "Normalization frequency is lesser then 0");
   normFreqency_ = (unsigned) test;

   // Init Arrays
   grid_.u().fill(conf.getRealParameter("U_INIT"));
   grid_.v().fill(conf.getRealParameter("V_INIT"));
   grid_.p().fill(conf.getRealParameter("P_INIT"));

   //Set boundaries
   north_ = boundaryCondition(NORTH, conf);
   south_ = boundaryCondition(SOUTH, conf);
   west_ = boundaryCondition(WEST, conf);
   east_ = boundaryCondition(EAST, conf);
   // Set boundary velocity
   north_->setVelocityValues(grid_.u(), grid_.v());
   south_->setVelocityValues(grid_.u(), grid_.v());
   west_->setVelocityValues(grid_.u(), grid_.v());
   east_->setVelocityValues(grid_.u(), grid_.v());
}

FluidSimulator::~FluidSimulator()
{
   delete north_;
   delete south_;
   delete west_;
   delete east_;
}

void FluidSimulator::simulate( Real duration )
{
   // Init values (i do it in the constructor)
   Real t = 0.0;
   
   while(t<=duration)
   {
      determineNextDT();
      refreshBoundaries();
      computeFG();
      composeRHS();
      //TODO normalization
      solvePoisson();
      updateVelocities();
      t += dt_;
   }
}

void FluidSimulator::simulateTimeStepCount( unsigned int nrOfTimeSteps )
{
   for (unsigned int i = 0; i < nrOfTimeSteps; ++i)
   {
      determineNextDT();
      computeFG();
      composeRHS();
      //TODO normalization
      solvePoisson();
      updateVelocities();
   }
}

Boundary* FluidSimulator::boundaryCondition( DIRECTION direction, const FileReader & conf)
{
   // Determine strings of conf file
   std::string condition;
   std::string velocity;
   switch(direction){
      case NORTH:
         condition = "boundary_condition_N";
         velocity = "boundary_velocity_N";
         break;
      case SOUTH:
         condition = "boundary_condition_N";
         velocity = "boundary_velocity_S";
         break;
      case WEST:
         condition = "boundary_condition_N";
         velocity = "boundary_velocity_W";
         break;
      case EAST:
         condition = "boundary_condition_N";
         velocity = "boundary_velocity_E";
         break;
      default:
         ABORT("No direction given");
   }

   // Set the right velocity
   BCTYPE type = NOSLIP;
   if(conf.isStringParameter(condition))
      type = bcParser.Parse(conf.getStringParameter(condition));
   Real vel = 0.0;
   if(conf.isRealParameter(velocity))
      vel = conf.getRealParameter(velocity);
   Boundary * boundary;
   switch(type){
      case NOSLIP:
         boundary = new NoslipBoundary(direction, imax_, jmax_, vel);
         break;
      case INFLOW:
         boundary = new InflowBoundary(direction, imax_, jmax_, vel);
         break;
      case OUTFLOW:
         boundary = new OutflowBoundary(direction, imax_, jmax_, vel);
         break;
      case SLIP:
         boundary = 0;
         ABORT("Slip not yet implemented");
         break;
      case PERIODIC:
         boundary = 0;
         ABORT("Periodic not yet implemented");
         break;
      default:
         boundary = 0;
         ABORT("No condition given");
         break;
   }
   return boundary;
}

void FluidSimulator::refreshBoundaries()
{
   north_->updateBoundaries( grid_.u(), grid_.v());
   south_->updateBoundaries( grid_.u(), grid_.v());
   west_->updateBoundaries( grid_.u(), grid_.v());
   east_->updateBoundaries( grid_.u(), grid_.v());
}

void FluidSimulator::solvePoisson()
{
   solver_.solve(grid_);
}

void FluidSimulator::determineNextDT()
{
   const Real & dx = grid_.dx();
   const Real & dy = grid_.dy();
   Array & u = grid_.u();
   Array & v = grid_.v();

   if( tau_ <= 0.0 )
      return;
   Real umax = 0.0;
   for(size_t j = 0; j <= jmax_+1; ++j)
   {
      for(size_t i = 0; i <= imax_; ++i)
      {
         Real abs = fabs(u(i,j));
         if( umax < abs )
            umax = abs;
      }
   }

   Real vmax = 0.0;
   for(size_t j = 0; j <= jmax_; ++j)
   {
      for(size_t i = 0; i <= imax_+1; ++i)
      {
         Real abs = fabs(v(i,j));
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




void FluidSimulator::updateVelocities()
{
   Array & u = grid_.u();
   Array & v = grid_.v();
   Array & F = grid_.f();
   Array & G = grid_.g();

   for(size_t j = 1; j <= jmax_; ++j)
   {
      for(size_t i = 1; i <= imax_-1; ++i)
      {
         u(i,j) = F(i,j) - dt_ * grid_.dpdx(i,j);
      }
   }
   
   for(size_t j = 1; j <= jmax_-1; ++j)
   {
      for(size_t i = 1; i <= imax_; ++i)
      {
         v(i,j) = G(i,j) - dt_ * grid_.dpdy(i,j);
      }
   }
}


void FluidSimulator::composeRHS()
{

   Array & rhs = grid_.rhs();
   Array & F = grid_.f();
   Array & G = grid_.g();
   const Real & dx = grid_.dx();
   const Real & dy = grid_.dy();


   for(size_t j = 1; j <= jmax_; ++j)
   {
      for(size_t i = 1; i <= imax_; ++i)
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
