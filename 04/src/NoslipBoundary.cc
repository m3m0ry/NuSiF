#include "NoslipBoundary.hh"
NoslipBoundary::NoslipBoundary(DIRECTION direction, size_t imax, size_t jmax, Real velocity) : Boundary(direction, imax, jmax, velocity)
{
   
}

void NoslipBoundary::setVelocityValues(Array & u, Array & v)
{
   
}


void NoslipBoundary::updateBoundaries(Array & u, Array & v) 
{
   switch(direction_){
      case NORTH:
         for (size_t i = 0; i < imax_; ++i)
         {
            u(i,jmax_ +1) = 2.0 * velocity_ -u(i,jmax_);
         }
         break;
      case SOUTH:
         for (size_t i = 0; i < imax_; ++i)
         {
            u(i,0) = 2.0 * velocity_ -u(i,1);
         }
         break;
      case WEST:
         for (size_t j = 0; j < jmax_; ++j)
         {
            v(0,j) = 2.0 * velocity_ -v(1,j);
         }
         break;
      case EAST:
         for (size_t j = 0; j < jmax_; ++j)
         {
            v(imax_+1,j) = 2.0 * velocity_ -v(imax_,j);
         }
         break;
      default:
         ABORT("No direction of boundary given");
         break;
   }
}

