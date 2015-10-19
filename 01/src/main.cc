#include "Array.hh"
#include "FileReader.hh"

#include <iostream>


int main( int argc, char** argv )
{
	auto filereader = new FileReader();
	filereader->readFile("something");

   return 0;
}
