#include "Array.hh"
#include "FileReader.hh"

#include <iostream>


int main( int argc, char** argv )
{
	auto filereader = new FileReader();
	filereader->readFile("arraytest.txt");


	filereader->printParameters();

	internal::progress("test", 0, 10);
	internal::progress("", 50, 100);
	internal::progress("test2", 200,200);


	std::cout << "2D array" << std::endl;
	Array * bla = new Array(20, 20);
	bla->fill(42);
	bla->print();
	delete bla;


	std::cout << "1D array" << std::endl;
	bla = new Array(20);
	bla->fill(42);
	bla->print();
	delete bla;

	delete filereader;

   return 0;
}
