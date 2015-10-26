#include "Array.hh"
#include "FileReader.hh"

#include <iostream>


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

	int height = filereader->getIntParameter("height");
	int width = filereader->getIntParameter("width");
	Real initial = filereader->getRealParameter("initial");


	int x = filereader->getIntParameter("x");
	int y = filereader->getIntParameter("y");

	std::cout << "2D array" << std::endl;
	Array * bla = new Array(height, width);
	bla->fill(initial);
	(*bla)(x,y) = (*bla)(x,y) * 2;
	bla->print();
	delete bla;

	delete filereader;

   return 0;
}
