#pragma once

#include "Boundary.hh"
#include "Debug.hh"
#include "Array.hh"

class FreeSlipBoundary : public Boundary
{
public:
   FreeSlipBoundary(DIRECTION direction, size_t imax, size_t jmax, Real velocity, Real delta);
   void setVelocityValues(Array<Real> & u, Array<Real> & v) override final;
   void updateBoundaries(Array<Real> & u, Array<Real> & v) override final;
   //virtual ~SlipBoundary () {};

private:
   Real delta_;
};
