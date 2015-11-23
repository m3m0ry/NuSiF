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

int main( int argc, char** argv )
{
   // Done checked by Christoph :)
   if(argc < 2)
   {
      std::cerr << "No configuration file given" << std::endl;
      std::cerr << "Usage: " << argv[0] << "<config_file>" << std::endl;
      return -1;
   }

   internal::progress("Starting to process file:", 0, 10);
   auto filereader = new FileReader();
   filereader->readFile(argv[1]);
   filereader->printParameters();
   internal::progress("File read", 200,200);


   Real uInit = filereader->getRealParameter("U_INIT");
   Real vInit = filereader->getRealParameter("V_INIT");
   Real pInit = filereader->getRealParameter("P_INIT");

   auto sim = new FluidSimulator( *filereader );
   auto grid = & sim->grid();

   Array & u = grid->u();
   for(size_t j = 0; j < u.getSize(1); ++j)
   {
      for(size_t i = 0; i < u.getSize(0); ++i)
         u(i,j) = uInit;
   }

   Array & v = grid->v();
   for(size_t j = 0; j < v.getSize(1); ++j)
   {
      for(size_t i = 0; i < v.getSize(0); ++i)
         v(i,j) = vInit;
   }

   Array & p = grid->p();
   for(size_t j = 0; j < p.getSize(1); ++j)
   {
      for(size_t i = 0; i < p.getSize(0); ++i)
         p(i,j) = pInit;
   }

   Array & f = grid->f();
   Array & g = grid->g();

   sim->simulate(1.0);
   f.print();
   g.print();


   delete sim;
   delete filereader;


   //delete solver;

   return 0;
}
