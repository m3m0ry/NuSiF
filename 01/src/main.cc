#include "Array.hh"
#include "FileReader.hh"

#include <iostream>


int main( int argc, char** argv )
{
	auto filereader = new FileReader();
	filereader->readFile("something");

	internal::progress("test", 0, 10);
	internal::progress("", 50, 100);
	internal::progress("test2", 200,200);
	internal::progress("tersterror", -1, 0);

   return 0;
}
