#include "PeriodicBoundary.hh"
PeriodicBoundary::PeriodicBoundary(DIRECTION direction, size_t imax, size_t jmax, Real velocity) : Boundary(direction, imax, jmax, velocity)
{
   
}
//TODO periodicboundary.cc evrything

void PeriodicBoundary::setVelocityValues(Array<Real> & u, Array<Real> & v)
{
   switch(direction_){
      case NORTH:
         for (size_t i = 1; i <= imax_; ++i)
         {
            v(i,jmax_) = 0.0;
            u(i,jmax_ +1) = 2.0 * velocity_ -u(i,jmax_);
         }
         break;
      case SOUTH:
         for (size_t i = 1; i <= imax_; ++i)
         {
            v(i,0) = 0.0;
            u(i,0) = 2.0 * velocity_ -u(i,1);
         }
         break;
      case WEST:
         for (size_t j = 1; j <= jmax_; ++j)
         {
            u(0,j) = 0.0;
            v(0,j) = 2.0 * velocity_ - v(1,j); 
         }
         break;
      case EAST:
         for (size_t j = 1; j <= jmax_; ++j)
         {
            u(imax_,j) = 0.0;
            v(imax_+1,j) = 2.0 * velocity_ - v(imax_,j);
         }
         break;
      default:
         ABORT("No direction of boundary given");
         break;
   }
}


void PeriodicBoundary::updateBoundaries(Array<Real> & u, Array<Real> & v) 
{
   switch(direction_){
      case NORTH:
         for (size_t i = 1; i <= imax_; ++i)
         {
            u(i,jmax_ +1) = 2.0 * velocity_ -u(i,jmax_);
         }
         break;
      case SOUTH:
         for (size_t i = 1; i <= imax_; ++i)
         {
            u(i,0) = 2.0 * velocity_ -u(i,1);
         }
         break;
      case WEST:
         for (size_t j = 1; j <= jmax_; ++j)
         {
            v(0,j) = 2.0 * velocity_ -v(1,j);
         }
         break;
      case EAST:
         for (size_t j = 1; j <= jmax_; ++j)
         {
            v(imax_+1,j) = 2.0 * velocity_ -v(imax_,j);
         }
         break;
      default:
         ABORT("No direction of boundary given");
         break;
   }
}

