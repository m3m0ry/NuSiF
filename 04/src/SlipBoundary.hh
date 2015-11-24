#pragma once

#include "Boundary.hh"
#include "Debug.hh"
#include "Array.hh"

class SlipBoundary : public Boundary
{
public:
   SlipBoundary(DIRECTION direction, size_t imax, size_t jmax, Real velocity);
   void setVelocityValues(Array & u, Array & v) override final;
   void updateBoundaries(Array & u, Array & v) override final;
   //virtual ~SlipBoundary () {};

private:
};
