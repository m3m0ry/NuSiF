#pragma once

#include <cstddef>
// This typedef makes it possible to switch between float and double accuracy
// please do not use "float" or "double" directly in your code, but use real instead
typedef double Real;
using std::size_t;



// Enumeration of boundary conditions
typedef enum { NOSLIP, SLIP, INFLOW, OUTFLOW, PERIODIC } BCTYPE;

// String search for enum
template <typename T>
class EnumParser
{
	map <string, T> enumMap;
	public:
		EnumParser(){};

		T Parse(const string &value)
		{ 
			map <string, T>::const_iterator iValue = enumMap.find(value);
			if (iValue  == enumMap.end())
				throw runtime_error("");
			return iValue->second;
		}
};

EnumParser<BCTYPE>::EnumParser()
{
	enumMap["noslip"] = NOSLIP;
	enumMap["slip"] = SLIP;
	enumMap["inflow"] = INFLOW;
	enumMap["outflow"] = OUTFLOW;
	enumMap["periodic"] = PERIODIC;
}

