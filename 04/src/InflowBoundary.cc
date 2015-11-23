#include "InflowBoundary.hh"

InflowBoundary::InflowBoundary(DIRECTION direction, size_t imax, size_t jmax, Real velocity) : Boundary(direction, imax, jmax, velocity)
{
   
}

void InflowBoundary::setVelocityValues(Array & u, Array & v)
{

}

void InflowBoundary::updateBoundaries(Array & u, Array & v) 
{
   switch(direction_){
      case NORTH:
         for (size_t i = 0; i < imax_; ++i)
         {
            u(i,jmax_ +1) = -u(i,jmax_);
            v(i, jmax_) = 0;
         }
         break;
      case SOUTH:
         for (size_t i = 0; i < imax_; ++i)
         {
            u(i,0) = -u(i,1);
         }
         break;
      case WEST:
         for (size_t j = 0; j < jmax_; ++j)
         {
            v(0,j) = -v(1,j);
         }
         break;
      case EAST:
         for (size_t j = 0; j < jmax_; ++j)
         {
            v(imax_+1,j) = -v(imax_,j);
         }
         break;
      default:
         ABORT("No direction of boundary given");
         break;
   }
}

