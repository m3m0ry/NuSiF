
#include "FileReader.hh"


void FileReader::registerIntParameter(const std::string &key, int init)
{
	CHECK_MSG( intParameters.end() == intParameters.find(key), "Parameter already registered!");
	intParameters[key] = init;
}

void FileReader::registerRealParameter(const std::string &key, real init)
{
	CHECK_MSG( realParameters.end() == realParameters.find(key), "Parameter already registered!");
	realParameters[key] = init;
}

void FileReader::registerStringParameter(const std::string &key, const std::string &init)
{
	CHECK_MSG( stringParameters.end() == stringParameters.find(key), "Parameter already registered!");
	stringParameters[key] = init;
}

void FileReader::setParameter(const std::string &key, const std::string &in)
{
	CHECK_MSG( stringParameters.end() != stringParameters.find(key), "Parameter not found!");
	stringParameters.at(key) = in;
}

void FileReader::setParameter(const std::string &key, real in)
{
	CHECK_MSG( realParameters.end() != realParameters.find(key), "Parameter not found!");
	realParameters.at(key) = in;
}

void FileReader::setParameter(const std::string &key, int in)
{
	CHECK_MSG( intParameters.end() != intParameters.find(key), "Parameter not found!");
	intParameters.at(key) = in;
}


bool FileReader::readFile(const std::string &name)
{

	registerIntParameter("test1", 1);
	registerIntParameter("test2", 2);
	registerIntParameter("test3", 2);
	printParameters();
	setParameter("test3", 3);
	printParameters();

   return false;
}


//Print all Parameters
void FileReader::printParameters() const
{
	for (auto it=intParameters.begin(); it!=intParameters.end(); ++it)
		std::cout << it->first << " => " << it->second << '\n';
	for (auto it=realParameters.begin(); it!=realParameters.end(); ++it)
		std::cout << it->first << " => " << it->second << '\n';
	for (auto it=stringParameters.begin(); it!=stringParameters.end(); ++it)
		std::cout << it->first << " => " << it->second << '\n';
}


