#ifndef __FLUID_SIMULATOR_H__
#define __FLUID_SIMULATOR_H__

#include <string>


#include "StaggeredGrid.hh"
#include "FileReader.hh"
#include "SORSolver.hh"
#include "Types.hh"
#include "Debug.hh"


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


		// Return the boundary condition
		BCTYPE boundaryCondition( const std::string &);

		// Set the velocity values of a boundary
		void setVelocityValues( const std::string &);	


		//TODO delete
		const FileReader * reader_;
		StaggeredGrid grid_;
		SORSolver solver_;
		EnumParser parser_;

		Real gx_;
		Real gy_;
		Real re_;
		Real dt_;
		Real gamma_;
		Real tau_;

		BCTYPE conditionNorth_;
		BCTYPE conditionSouth_;
		BCTYPE conditionWest_;
		BCTYPE conditionEast_;

		BCTYPE velocityNorth_;
		BCTYPE velocitySouth_;
		BCTYPE velocityWest_;
		BCTYPE velocityEast_;

		size_t imax_;
		size_t jmax_;
};



#endif
