#ifndef __FLUID_SIMULATOR_H__
#define __FLUID_SIMULATOR_H__


#include "StaggeredGrid.hh"
#include "FileReader.hh"
#include "SORSolver.hh"


class FluidSimulator
{
  public:
      FluidSimulator( const FileReader & conf );

      // Simulates a given time-length
      void simulate( Real );
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

		StaggeredGrid grid_;
		SORSolver solver_;

		Real gx_;
		Real gy_;
		Real re_;
		Real dt_;
		Real gamma_;
		Real tau_;

		size_t imax_;
		size_t jmax_;
};



#endif
