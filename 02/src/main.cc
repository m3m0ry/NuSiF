#include "Array.hh"
#include "FileReader.hh"
#include "SORSolver.hh"
#include "StaggeredGrid.hh"

#include <iostream>
#include <ostream>
#include <random>


int main( int argc, char** argv )
{
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

	auto grid = new StaggeredGrid( *filereader );
	grid->initGridSetup2();
	Array & p = grid->p();
	std::default_random_engine generator;
	std::uniform_real_distribution<double> distribution(0.0, 1.0);
	for(size_t i = 0; i < p.getSize(0)*p.getSize(1); ++i)
	{
		p(i) = distribution(generator);
	}
	p.print();
	Array & rhs = grid->rhs();
	rhs.print();
	auto solver = new SORSolver( *filereader );
	internal::progress("Start solving", 0,200);

	if(solver->solve(*grid))
		std::cout << "yeah" << std::endl;

	internal::progress("Solved", 200,200);
	p.print();


	delete filereader;

	delete grid;

	delete solver;

   return 0;
}
