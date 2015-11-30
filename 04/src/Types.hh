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
// Enumaration of the oriantation
typedef enum { NORTH, SOUTH, EAST, WEST } DIRECTION;

// String search for enum
template <typename T>
class EnumParser
{
public:
    EnumParser(){};

    T Parse(const std::string &value) const
    { 
      //std::map<std::string, T>::const_iterator iValue = enumMap.find(value);
      typename std::map <std::string, T>::const_iterator iValue = enumMap.find(value);

      CHECK_MSG(iValue != enumMap.end(), "Wrong string to parse!");
      return iValue->second;
   }
private:
   std::map <std::string, T> enumMap;
};

template <>
EnumParser<BCTYPE>::EnumParser();

extern const EnumParser<BCTYPE> bcParser;
