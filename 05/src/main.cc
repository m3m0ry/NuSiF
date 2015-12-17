#include <iostream>
#include <ostream>
#include <random>
#include <fstream>

#include "Types.hh"
#include "Array.hh"
#include "FileReader.hh"
#include "SORSolver.hh"
#include "StaggeredGrid.hh"
#include "Debug.hh"
#include "FluidSimulator.hh"
#include "VTKWriter.hh"

#include "GrayScaleImage.hh"

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
   StaggeredGrid & grid = sim.grid();
   if(filereader.isStringParameter("Solidpgn")){
      std::ifstream png(filereader.getStringParameter("Solidpgn"));
      if(!png){
         if(filereader.isIntParameter("Rectangle"))
         {
            Real x1 = filereader.getRealParameter("RectangleX1") / grid.dx();
            Real y1 = filereader.getRealParameter("RectangleY1") / grid.dy();
            Real x2 = filereader.getRealParameter("RectangleX2") / grid.dx();
            Real y2 = filereader.getRealParameter("RectangleY2") / grid.dy();
            grid.createRectangle((size_t)x1, (size_t)y1, (size_t)x2, (size_t)y2-1);
         }
      }
      else{
         GrayScaleImage gr = GrayScaleImage(filereader.getStringParameter("Solidpgn"));
         for(size_t j = 0; j < (size_t)gr.height(); ++j)
         {
            for(size_t i = 0; i < (size_t)gr.width(); ++i)
            {
               if(gr(i,j) <= 0.5)
                  grid.setCellToObstacle(i,j);
            }
         }
      }
   }
   int imax = filereader.getIntParameter("imax");
   int jmax = filereader.getIntParameter("jmax");
   Array<Real> & u = grid.u();
   GrayScaleImage gr = GrayScaleImage((size_t)imax, (size_t)jmax);
   for(size_t j = 0; j < (size_t)jmax; ++j){
      for(size_t i = 0; i < (size_t)imax; ++i){
         unsigned char & element = gr.getElement(i,jmax -1 -j);
         if(grid.isFluid(i,j)){
            element = 255;
         }
         else{
            element = 0;
         }
         if(j < 8)
            u(i,j) = 0.0;
      }
   }
   gr.save("output.png");



   if(filereader.isRealParameter("time")){
      sim.simulate(filereader.getRealParameter("time"));
   }
   else{
      sim.simulateTimeStepCount((unsigned int)filereader.getIntParameter("timesteps"));
   }

   return 0;
}
