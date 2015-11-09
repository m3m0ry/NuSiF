#ifndef __FLUID_SIMULATOR_H__
#define __FLUID_SIMULATOR_H__


#include "StaggeredGrid.hh"
#include "FileReader.hh"
#include "SORSolver.hh"


class FluidSimulator
{
  public:
      FluidSimulator( const FileReader & conf );

      /// Simulates a given time-length
      void simulate( Real );
      void simulateTimeStepCount( unsigned int nrOfTimeSteps );


      // Getter functions for the internally stored StaggeredGrid
      StaggeredGrid & grid(){ return grid_; }
      const StaggeredGrid & grid() const { return grid_; }

  private:
      void computeFG();

		StaggeredGrid grid_;
		SORSolver solver_;

		Real gx_;
		Real gy_;
		Real re_;
		Real dt_;
		Real gamma_;

		int imax_;
		int jmax_;
};



#endif
