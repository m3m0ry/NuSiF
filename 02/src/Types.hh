#ifndef TYPES_HH
#define TYPES_HH


#include <cstddef>
// This typedef makes it possible to switch between float and double accuracy
// please do not use "float" or "double" directly in your code, but use real instead
typedef double Real;
typedef std::size_t MySize;



// Enumeration of boundary conditions
typedef enum { NOSLIP, SLIP, OUTFLOW, PERIODIC } BCTYPE;


#endif //TYPES_HH
