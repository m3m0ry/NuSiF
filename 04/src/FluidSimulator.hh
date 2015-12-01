#ifndef __FLUID_SIMULATOR_H__
#define __FLUID_SIMULATOR_H__

#include <string>

#include "VTKWriter.hh"
#include "StaggeredGrid.hh"
#include "Array.hh"
#include "FileReader.hh"
#include "SORSolver.hh"
#include "Types.hh"
#include "Debug.hh"
#include "Boundary.hh"
#include "NoslipBoundary.hh"
#include "InflowBoundary.hh"
#include "OutflowBoundary.hh"
#include "FreeSlipBoundary.hh"

//Slip, Periodic


class FluidSimulator
{
  public:
     // c'tor
      FluidSimulator( const FileReader & conf );
      // d'tor
      ~FluidSimulator();
      //TODO fluidsimulator.hh rule of 3

      // Simulates a given time-length
      void simulate( Real duration );
      void simulateTimeStepCount( unsigned int nrOfTimeSteps );


      // Getter functions for the internally stored StaggeredGrid
      StaggeredGrid & grid(){ return grid_; }
      const StaggeredGrid & grid() const { return grid_; }

  private:
		// Computation of the next F and G
      void computeFG();

		// Computation of the rhs of the pressure equation
		void composeRHS();

		// Calculate u(n+1) after solution of pressure equation
		void updateVelocities();

		// Next dt which satisfies the stability conditions
		void determineNextDT();

		// Update boundaries
		void refreshBoundaries();

      // Solve the poisson eq.
      void solvePoisson();

      // Normalize the pressure grid
      void normalizePressure();

		// Return the boundary for given direction, condition and velocity
		Boundary* boundaryCondition( DIRECTION direction, const FileReader & conf);

		// Set the velocity values of a boundary
		void setVelocityValues( const std::string &, const FileReader &);	

		StaggeredGrid grid_;
		SORSolver solver_;
		//EnumParser<BCTYPE> parser_;

		Real gx_;
		Real gy_;
		Real re_;
		Real dt_;
		Real gamma_;
		Real tau_;

		Boundary * north_;
		Boundary * south_;
		Boundary * west_;
		Boundary * east_;

		size_t imax_;
		size_t jmax_;

      unsigned int normFreqency_;

      VTKWriter writer_;
      unsigned int outputInterval_;

};



#endif
