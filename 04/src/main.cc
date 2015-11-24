#include <iostream>
#include <ostream>
#include <random>

#include "Types.hh"
#include "Array.hh"
#include "FileReader.hh"
#include "SORSolver.hh"
#include "StaggeredGrid.hh"
#include "Debug.hh"
#include "FluidSimulator.hh"
#include "VTKWriter.hh"

int main( int argc, char** argv )
{
   // Done checked by Christoph :)
   if(argc < 2)
   {
      std::cerr << "No configuration file given" << std::endl;
      std::cerr << "Usage: " << argv[0] << "<config_file>" << std::endl;
      return -1;
   }

   //internal::progress("Starting to process file:", 0, 10);
   auto filereader = FileReader();
   filereader.readFile(argv[1]);
   filereader.printParameters();
   internal::progress("File read", 200,200);


   auto sim = FluidSimulator( filereader );

   sim.simulate(10.0);

   VTKWriter writer(sim.grid(), filereader.getStringParameter("name"), true, true);
   writer.write();

   return 0;
}
