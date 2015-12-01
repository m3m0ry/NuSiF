#pragma once

#include "Boundary.hh"
#include "Debug.hh"
#include "Array.hh"

class PeriodicBoundary : public Boundary
{
public:
   PeriodicBoundary(DIRECTION direction, size_t imax, size_t jmax, Real velocity);
   void setVelocityValues(Array<Real> & u, Array<Real> & v) override final;
   void updateBoundaries(Array<Real> & u, Array<Real> & v) override final;
   //virtual ~NoslipBoundary ();

private:
};
