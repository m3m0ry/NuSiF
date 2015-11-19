#pragma once

#include <cstddef>
#include <map>
#include <string>

#include "Debug.hh"
// This typedef makes it possible to switch between float and double accuracy
// please do not use "float" or "double" directly in your code, but use real instead
typedef double Real;
using std::size_t;



// Enumeration of boundary conditions
typedef enum { NOSLIP, SLIP, INFLOW, OUTFLOW, PERIODIC } BCTYPE;

// String search for enum
//template <typename T>
//class EnumParser
//{
//	public:
//		EnumParser(){};
//
//		T Parse(const std::string &value)
//		{ 
//			std::map<std::string, T>::const_iterator iValue = enumMap.find(value);
//			
//			CHECK_MSG(iValue != enumMap.end(), "Wrong string to parse!");
//			return iValue->second;
//		}
//	private:
//		std::map <std::string, T> enumMap;
//};
//
//EnumParser<BCTYPE>::EnumParser()
//{
//	enumMap["noslip"] = NOSLIP;
//	enumMap["slip"] = SLIP;
//	enumMap["inflow"] = INFLOW;
//	enumMap["outflow"] = OUTFLOW;
//	enumMap["periodic"] = PERIODIC;
//}

// String search for enum
class EnumParser
{
	public:
		EnumParser(){
			enumMap["noslip"] = NOSLIP;
			enumMap["slip"] = SLIP;
			enumMap["inflow"] = INFLOW;
			enumMap["outflow"] = OUTFLOW;
			enumMap["periodic"] = PERIODIC;
		}

		BCTYPE Parse(const std::string &value)
		{ 
			std::map<std::string, BCTYPE>::const_iterator iValue = enumMap.find(value);
			
			CHECK_MSG(iValue != enumMap.end(), "Wrong string to parse!");
			return iValue->second;
		}
	//private:
		std::map <std::string, BCTYPE> enumMap;
};

