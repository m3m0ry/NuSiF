#pragma once

#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <cctype>

#include "Types.hh"
#include "Debug.hh"


//******************************************************************************************
/*! Class for reading configuration from a file
*
* Configuration File Syntax:
*   - everything between a '#' character and the beginning of the next line is ignored
*   - lines can be empty
*   - line contain parameter key followed by white-spaces followed by their value
*
*  All possible keys (and the datatype of the value) have to be registered first:
*   - for example usage have a look at the FileReaderTest
*
*
*
*  This Skeleton is just a suggestion. If you are familiar with template programming
*  a possible alternative would be a version that does not need registering of all keys
*  and has a getter function like the following:
*      template<typename ExpectedType>
*      ExpectedType getValue( const std::string & key );
*/
//******************************************************************************************
class FileReader
{
public:

	//register a new parameter with name key and initial int value
	void registerIntParameter( const std::string & key, int init = 0 );

	//register a new parameter with name key and initial double value
	void registerRealParameter( const std::string & key, Real init = 0 );

	//register a new parameter with name key and initial string value
	void registerStringParameter( const std::string & key, const std::string & init = "" );

	//set a value for the key string with value in
	void setParameter( const std::string & key, const std::string & in );

	//set a value for the key string with value in
	void setParameter( const std::string & key, Real in );

	//set a value for the key string with value in
	void setParameter( const std::string & key, int in );

	// get the int value of key 
	int getIntParameter( const std::string & key ) const;

	// get the double value of key 
	Real getRealParameter( const std::string & key ) const;

	// get the string value of key 
	std::string getStringParameter( const std::string & key ) const;

	// Check if parameter availible
	bool isIntParameter( const std::string & key ) const;
	bool isRealParameter( const std::string & key ) const;
	bool isStringParameter( const std::string & key ) const;

	//try to read all registered parameters from file name
	bool readFile( const std::string & name );

	//print out all parameters to std:out
	void printParameters() const;

private:
	std::map<std::string, int> intParameters;
	std::map<std::string, Real> RealParameters;
	std::map<std::string, std::string> stringParameters;

};



inline int FileReader::getIntParameter(const std::string &key) const
{
	CHECK_MSG( intParameters.end() != intParameters.find(key), "Parameter " + key + " not found!");
	return intParameters.at(key);
}

inline Real FileReader::getRealParameter(const std::string &key) const
{
	CHECK_MSG( RealParameters.end() != RealParameters.find(key), "Parameter " + key + " not found!");
	return RealParameters.at(key);
}

inline std::string FileReader::getStringParameter(const std::string &key) const
{
	CHECK_MSG( stringParameters.end() != stringParameters.find(key), "Parameter " + key + " not found!");
	return stringParameters.at(key);
}

inline bool FileReader::isIntParameter( const std::string &key ) const
{
	return intParameters.end() != intParameters.find(key);
}

inline bool FileReader::isRealParameter( const std::string &key ) const
{
	return RealParameters.end() != RealParameters.find(key);
}

inline bool FileReader::isStringParameter( const std::string &key ) const
{
	return stringParameters.end() != stringParameters.find(key);
}


