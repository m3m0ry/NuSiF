
#include "FileReader.hh"


void FileReader::registerIntParameter(const std::string &key, int init)
{
	CHECK_MSG( intParameters.end() == intParameters.find(key), "Parameter already registered!");
	intParameters[key] = init;
}

void FileReader::registerRealParameter(const std::string &key, Real init)
{
	CHECK_MSG( RealParameters.end() == RealParameters.find(key), "Parameter already registered!");
	RealParameters[key] = init;
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

void FileReader::setParameter(const std::string &key, Real in)
{
	CHECK_MSG( RealParameters.end() != RealParameters.find(key), "Parameter not found!");
	RealParameters.at(key) = in;
}

void FileReader::setParameter(const std::string &key, int in)
{
	CHECK_MSG( intParameters.end() != intParameters.find(key), "Parameter not found!");
	intParameters.at(key) = in;
}


bool FileReader::readFile(const std::string &name)
{
	std::string line;
	std::ifstream parameterFile(name);
	if(parameterFile.is_open())
	{
		for(size_t j = 1; getline(parameterFile, line); j++)
		{
			//DELETE Comments
			line = line.substr(0, line.find('#'));

			char c;
			std::string key;
			std::string value;
			int state = 0;
			for( int i = 0; '\0' != (c = line[i]); ++i)
			{
				//Ignore whitespace
				if(isspace(c))
				{
					if(1 == state)
						state = 2;
					else if(2 == state)
						state = 3;
					continue;
				}
				//Read parameter name
				if(0 == state || 1 == state)
				{
					key = key + c;
					state = 1;
				}
				//Read paremeter value
				else if(2 == state)
				{
					value = value + c;
				}
				//Warn if multiple values given
				else if(3 == state){
					internal::warnFct( "Multiple values for one parameter given", name.c_str(), j);
					break;
				}
			}
			//Check if line was valid
			if( !key.empty() && value.empty())
			{
				internal::warnFct( "No value for one parameter given", name.c_str(), j);
				continue;
			}
			//Register valid parameter
			else if( !key.empty() && !value.empty())
			{
				//INT
				if( value.find_first_not_of("0123456789") == std::string::npos)
					registerIntParameter(key, stoi(value));
				//Real
				else if( value.find_first_not_of(".0123456789") == std::string::npos)
					registerRealParameter(key, stod(value));
				//STRING
				else
					registerStringParameter(key, value);
			}

		}
		parameterFile.close();
	}
	else
	{
		WARN("Cannot open file");	
	}

   return false;
}


//Print all Parameters
void FileReader::printParameters() const
{
	std::cout << "INT Parameters:" << std::endl;
	for (auto it=intParameters.begin(); it!=intParameters.end(); ++it)
		std::cout << it->first << " => " << it->second << '\n';
	std::cout << "Real Parameters:" << std::endl;
	for (auto it=RealParameters.begin(); it!=RealParameters.end(); ++it)
		std::cout << it->first << " => " << it->second << '\n';
	std::cout << "STRING Parameters:" << std::endl;
	for (auto it=stringParameters.begin(); it!=stringParameters.end(); ++it)
		std::cout << it->first << " => " << it->second << '\n';
}


