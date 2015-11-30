#include "Types.hh"

template <>
EnumParser<BCTYPE>::EnumParser()
{
   enumMap["noslip"] = NOSLIP;
   enumMap["slip"] = SLIP;
   enumMap["inflow"] = INFLOW;
   enumMap["outflow"] = OUTFLOW;
   enumMap["periodic"] = PERIODIC;
}

const EnumParser<BCTYPE> bcParser;
