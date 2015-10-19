
#include "Debug.hh"

#include <iostream>
#include <cstdlib> // std::abort

const char * BLACK        = "\033[0;30m";
const char * RED          = "\033[0;31m";
const char * GREEN        = "\033[0;32m";
const char * BROWN        = "\033[0;33m";
const char * BLUE         = "\033[0;34m";
const char * MAGENTA      = "\033[0;35m";
const char * CYAN         = "\033[0;36m";
const char * WHITE        = "\033[0;37m";
const char * LIGHTBLACK   = "\033[1;30m";
const char * LIGHTRED     = "\033[1;31m";
const char * LIGHTGREEN   = "\033[1;32m";
const char * YELLOW       = "\033[1;33m";
const char * LIGHTBLUE    = "\033[1;34m";
const char * LIGHTMAGENTA = "\033[1;35m";
const char * LIGHTCYAN    = "\033[1;36m";
const char * LIGHTWHITE   = "\033[1;37m";
const char * DEFAULTCOLOR = "\033[0m";


namespace internal
{
	//Check function
	void checkFct( bool b, const char * const expression, const std::string & message,
   	               const char * const filename, int line )
   	{
		if( !b )
   	   	{
			std::cerr << RED << "Check failed!" << DEFAULTCOLOR << "\n\tFile:       "
						<< filename << ":" << line << "\n"
						<< "\tExpression: " << expression << std::endl;
   	   	   	if ( message.size() > 0 )
				std::cerr << "\tMessage: \n\t" << message << std::endl;
   	   	}
   	   	std::abort();
   	}

	//Assert function -> Debug only
	void assertFct( bool b, const char * const expression, const std::string & message,
   	                const char * const filename, int line )
   	{
		if( !b )
   	   	{
			std::cerr << RED << "Assertion failed!" << DEFAULTCOLOR << "\n\tFile:       "
						<< filename << ":" << line << "\n"
						<< "\tExpression: " << expression << std::endl;

   	   	   	if ( message.size() > 0 )
				std::cerr << "\tMessage: \n\t" << message << std::endl;
   	   	}
   	   	std::abort();
   	}

	void warnFct( const std::string & message, const char * const filename, int line )
	{
		std::cerr << RED << "Warning!" << DEFAULTCOLOR << "\n\tFile:       "
					<< filename << ":" << line << "\n";
		std::cerr <<  "\t" << message << std::endl;
	}

	void progress(const std::string & message, int parts, int whole){
		CHECK_MSG( parts >= 0 && whole > 0, "Progress values are negative"); 
		if ( message.size() > 0 )
			std::cerr << message;

		std::cerr << "\t[" << GREEN;
		for(int i = 0; i < (parts *20) /whole -1 ; ++i){
			std::cerr << "=";
		}
		std::cerr << ">" << RED;
		for(int i = 0; i < 20 - (parts*20)/whole; ++i){
			std::cerr << "%";
		}
		std::cerr << DEFAULTCOLOR << "]" << std::endl;
	}
}



