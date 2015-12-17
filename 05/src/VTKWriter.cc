#include <fstream>
#include <sstream>
#include <iomanip>

#include "VTKWriter.hh"
#include "Debug.hh"
#include "StaggeredGrid.hh"

template<typename T> struct RealTypeToString         {};
template<>           struct RealTypeToString<float>  { static const char * str; };
template<>           struct RealTypeToString<double> { static const char * str; };

const char * RealTypeToString<float>::str  = "float";
const char * RealTypeToString<double>::str = "double";



VTKWriter::VTKWriter(  const StaggeredGrid & grid, const std::string & basename, bool writePressure, bool writeVelocity, bool writeFluid )
      : grid_(grid), baseName_( basename ),
        writeVelocity_(writeVelocity), writePressure_(writePressure), counter_ (0 ),
        writeFluid_(writeFluid)
{
   ASSERT_MSG( writePressure_ || writeVelocity_ , "VTK Writer has to write at least velocity or pressure" );

   std::stringstream sstream;
   sstream << "# vtk DataFile Version 4.0\n";
   sstream << "Nusif VTK output\n";
   sstream << "ASCII\n";
   sstream << "DATASET STRUCTURED_POINTS\n";

   sstream << "DIMENSIONS " << grid_.xSize() << " " << grid_.ySize() << " 1\n";
   sstream << "ORIGIN 0 0 0 \n";
   sstream << "SPACING " << grid_.dx() << " " << grid_.dy() << " 1\n";
   sstream << "POINT_DATA " << grid_.xSize() * grid_.ySize() << " \n" << std::endl;

   header_ = sstream.str();
}

void VTKWriter::write()
{
   std::stringstream fileName;
   fileName << baseName_ << "_" <<  std::setw(4) << std::setfill( '0') << counter_ << ".vtk";
   std::ofstream fileStream ( fileName.str().c_str() );

   fileStream << header_;

   if ( writeVelocity_ )
   {
      fileStream << "VECTORS velocity " << RealTypeToString<Real>::str << "\n";

      for ( size_t j = 0; j < grid_.ySize (); ++j )
         for ( size_t i = 0; i < grid_.xSize (); ++i )
         {
            const Real u = 0.5 * ( grid_.u() ( i, j + 1 ) + grid_.u() ( i + 1, j + 1 ) );
            const Real v = 0.5 * ( grid_.v() ( i + 1, j ) + grid_.v() ( i + 1, j + 1 ) ) ;

            fileStream << u << " " << v << " " << " 0\n";
         }

      fileStream << "\n";
   }

   if ( writePressure_ )
   {
      fileStream << "SCALARS pressure " << RealTypeToString<Real>::str << " 1\n";
      fileStream << "LOOKUP_TABLE default\n";

      for ( size_t j = 0; j < grid_.ySize (); ++j )
         for ( size_t i = 0; i < grid_.xSize (); ++i )
            fileStream << grid_.p()( i+1, j+1 ) << "\n";
   }


   if ( writeFluid_ )
   {
      fileStream << "SCALARS fluid " << RealTypeToString<Real>::str << " 1\n";
      fileStream << "LOOKUP_TABLE default\n";

      for ( size_t j = 0; j < grid_.ySize (); ++j )
         for ( size_t i = 0; i < grid_.xSize (); ++i )
            fileStream << grid_.isFluids()( i+1, j+1 ) << "\n";
   }

   ++counter_;
}
