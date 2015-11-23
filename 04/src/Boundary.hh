#pragma once

#include "Types.hh"
#include "Array.hh"

class Boundary
{
public:
   Boundary (DIRECTION dir, size_t imax, size_t jmax, Real velocity) : direction_(dir), imax_(imax), jmax_(jmax), velocity_(velocity)  {};
   virtual void updateBoundaries(Array & u, Array & v) = 0;
   virtual void setVelocityValues(Array & u, Array & v) = 0;
   virtual ~Boundary () {};

protected:
   DIRECTION direction_;
   size_t imax_;
   size_t jmax_;
   Real velocity_;
private:
};
