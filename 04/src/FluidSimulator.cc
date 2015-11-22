#include "FluidSimulator.hh"

// Constructor
FluidSimulator::FluidSimulator( const FileReader & conf ) : grid_(StaggeredGrid(conf)),
      solver_(SORSolver(conf))
{
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

   //Set and test boundaries
   //EnumParser<BCTYPE> parser;
   // Set boundary condition
   conditionNorth_ = boundaryCondition("boundary_condition_N", conf);
   conditionSouth_ = boundaryCondition("boundary_condition_S", conf);
   conditionEast_ = boundaryCondition("boundary_condition_E", conf);
   conditionWest_ = boundaryCondition("boundary_condition_W", conf);

   // Set boundary velocity
   setVelocityValues("boundary_velocity_N", conf);
   setVelocityValues("boundary_velocity_S", conf);
   setVelocityValues("boundary_velocity_E", conf);
   setVelocityValues("boundary_velocity_W", conf);

   // Init Arrays
   grid_.u().fill(conf.getRealParameter("U_INIT"));
   grid_.v().fill(conf.getRealParameter("V_INIT"));
   grid_.p().fill(conf.getRealParameter("P_INIT"));
}

void FluidSimulator::simulate( Real duration )
{
   // Init values (i do it in the constructor)
   Real t = 0.0;
   
   while(t<=duration)
   {
      determineNextDT();
      //TODO set boundary values for u and v
      computeFG();
      composeRHS();
      //TODO maxiter, normalization, residial frequency
      // SOR-solver
      solver_.solve(grid_);
      updateVelocities();
      t += dt_;
   }
}

void FluidSimulator::simulateTimeStepCount( unsigned int nrOfTimeSteps )
{
   for (unsigned int i = 0; i < nrOfTimeSteps; ++i)
   {
      determineNextDT();
      //TODO set boundary values for u and v
      computeFG();
      composeRHS();
      //TODO maxiter, normalization, residial frequency
      // SOR-solver
      solver_.solve(grid_);
      updateVelocities();
   }

}

void FluidSimulator::refreshBoundaries()
{

}

void FluidSimulator::setVelocityValues( const std::string & name, const FileReader & conf )
{
   if( name.empty())
      ABORT("Cannot check for empty string!");

   Array & u = grid_.u();
   Array & v = grid_.v();
   if( name.back() == 'N' )
   {
      // North:
      // u(i, jmax+1)
      // v(i, jmax)
      if( conf.isRealParameter("boundary_velocity_N"))
      {
         Real val = conf.getRealParameter("boundary_velocity_N");
         switch(conditionNorth_){
            case NOSLIP: //tangential 
               for(size_t i = 1; i<= imax_; ++i)
               {
                  u(i, jmax_+1) = val;
                  v(i, jmax_) = 0.0;
               }
               break;
            case INFLOW: // normal
               for(size_t i = 1; i<= imax_; ++i)
               {
                  u(i, jmax_+1) = 0.0;
                  v(i, jmax_) = val;
               }
               break;
            case OUTFLOW:
               ABORT("Cannot have a velocity for an OUTFLOW boundary!");
               break;
            case SLIP: //TODO
               break;
            case PERIODIC: //TODO
               break;
            default:
               ABORT("Wrong boundary parameter given");
               break;
         }
      }
      else
      {
         // Set to 0
         for(size_t i = 1; i<= imax_; ++i)
         {
            u(i, jmax_+1) = 0.0;
            v(i, jmax_) = 0.0;
         }
      }
   }
   else if( name.back() == 'S' )
   {
      //South:
      //u(i, 0)
      //v(i, 0)
      if( conf.isStringParameter("boundary_velocity_S"))
      {
         Real val = conf.getRealParameter("boundary_velocity_S");
         switch(conditionNorth_){
            case NOSLIP: // tangential 
               for(size_t i = 1; i<= imax_; ++i)
               {
                  u(i,0) = val;
                  v(i,0) = 0.0;
               }
               break;
            case INFLOW: // normal
               for(size_t i = 1; i<= imax_; ++i)
               {
                  u(i,0) = 0.0;
                  v(i,0) = val;
               }
               break;
            case OUTFLOW:
               ABORT("Cannot have a velocity for an OUTFLOW boundary!");
               break;
            case SLIP: //TODO
               break;
            case PERIODIC: //TODO
               break;
            default:
               ABORT("Wrong boundary parameter given");
               break;
         }
      }
      else
      {
         // Set to 0
         for(size_t i = 1; i<= imax_; ++i)
         {
            u(i,0) = 0.0;
            v(i,0) = 0.0;
         }
      }
   }
   else if ( name.back() == 'E' )
   {
      // East
      // u(imax,j)
      // v(imax+1 j)
      if( conf.isStringParameter("boundary_velocity_E"))
      {
         Real val = conf.getRealParameter("boundary_velocity_E");
         switch(conditionNorth_){
            case NOSLIP: // tangential 
               for(size_t j = 1; j<= imax_; ++j)
               {
                  u(imax_,j) = 0.0;
                  v(imax_+1,j) = val;
               }
               break;
            case INFLOW: // normal
               for(size_t j = 1; j<= imax_; ++j)
               {
                  u(imax_,j) = val;
                  v(imax_+1,j) = 0.0;
               }
               break;
            case OUTFLOW:
               ABORT("Cannot have a velocity for an OUTFLOW boundary!");
               break;
            case SLIP: //TODO
               break;
            case PERIODIC: //TODO
               break;
            default:
               ABORT("Wrong boundary parameter given");
               break;
         }
      }
      else
      {
         // Set to 0
         for(size_t j = 1; j<= imax_; ++j)
         {
            u(imax_,j) = 0.0;
            v(imax_+1,j) = 0.0;
         }
      }
   }
   else if ( name.back() == 'W' )
   {
      // West:
      // u(0, j)
      // v(0, j)
      if( conf.isStringParameter("boundary_velocity_W"))
      {
         Real val = conf.getRealParameter("boundary_velocity_E");
         switch(conditionNorth_){
            case NOSLIP: // tangential 
               for(size_t j = 1; j <= jmax_; ++j)
               {
                  u(0,j) = 0.0;
                  v(0,j) = val;
               }
               break;
            case INFLOW: // normal
               for(size_t j = 1; j <= jmax_; ++j)
               {
                  u(0,j) = val;
                  v(0,j) = 0.0;
               }
               break;
            case OUTFLOW:
               ABORT("Cannot have a velocity for an OUTFLOW boundary!");
               break;
            case SLIP: //TODO
               break;
            case PERIODIC: //TODO
               break;
            default:
               ABORT("Wrong boundary parameter given");
               break;
         }
      }
      else
      {
         // Set to 0
         for(size_t j = 1; j <= jmax_; ++j)
         {
            u(0,j) = 0.0;
            v(0,j) = 0.0;
         }
      }
   }
   else
   {
      ABORT("Wrong velocity direction given!");
   }
}

BCTYPE FluidSimulator::boundaryCondition( const std::string & name, const FileReader & conf)
{
   if( name.empty())
      ABORT("Cannot check for empty string!");

   if( conf.isStringParameter(name))  
      return parser_.Parse(conf.getStringParameter(name));
   else
      return NOSLIP;
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
