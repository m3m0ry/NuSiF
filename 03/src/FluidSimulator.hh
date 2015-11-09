#ifndef __FLUID_SIMULATOR_H__
#define __FLUID_SIMULATOR_H__


#include "FileReader.hh"

class FluidSimulator
{
  public:
      FluidSimulator( const FileReader & conf );

      /// Simulates a given time-length
      void simulate             ( real duration              );
      void simulateTimeStepCount( unsigned int nrOfTimeSteps );


      // Getter functions for the internally stored StaggeredGrid
            StaggeredGrid & grid();
      const StaggeredGrid & grid() const;

  private:
      void computeFG();

};



#endif
