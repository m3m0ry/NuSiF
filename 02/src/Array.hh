#pragma once

#include <cstdlib>
#include <iostream>
#include <ostream>
#include <cstring>

#include "Types.hh"
#include "Debug.hh"


//*********************************************************************************************
/*!  Array class for 1,2 and 3 dimensions
*
*    - all elements should be stored in a contiguous chunk of memory ( no vector<vector> ! )
*/
//*********************************************************************************************
class Array
{
public:
   // Constructors for 1D,2D and 3D
	Array( );
   Array( size_t xSize );
   Array( size_t xSize, size_t ySize );
   Array( size_t xSize, size_t ySize, size_t zSize );
   Array(const Array& other);
   ~Array();

   //Array operator- (const Array& s);
   //Array operator- (const int value);

   // Access Operators for 1D, 2D and 3D
   inline Real & operator () ( size_t );
   inline Real & operator () ( size_t ,size_t );
   inline Real & operator () ( size_t, size_t, size_t );

   // for const Arrays the following access operators are required
    inline const Real & operator () ( size_t) const;
    inline const Real & operator () ( size_t, size_t) const;
    inline const Real & operator () ( size_t, size_t, size_t ) const;


	//Operators
	Array& operator= (const Array& other);
	Array& operator+= (const Array& other);
	Array operator+ (const Array& other) const;
	Array operator+ (const Real value) const;

   // initialize the whole array with a constant value
   void fill( Real value );


   // return total size of the array
   size_t getSize() const;

   // return xSize for dimension==0, ySize for dimension==1 and zSize for dimension==2
   // other dimension values are not allowed
   size_t getSize(char dimension ) const;


   // Print the whole array ( for debugging purposes )
   void print() const;

private:
	size_t size_;
	size_t xSize_;
	size_t ySize_;
	size_t zSize_;
	Real * array_;
	char dimension_;
};

friend std::ostream& operator<< (std::ostream&, const Array&);

//=======================================
//
//  Inline Access Operators and Sizes
//
//=======================================


// Operator() 1D
inline Real& Array::operator ()(size_t i)
{
   ASSERT_MSG(i < xSize_, "Access value too large");
   return array_[i];
}

// Operator() 2D
inline Real& Array::operator ()(size_t i,size_t j)
{
   ASSERT_MSG(i < xSize_ && j < ySize_, "Access value too large");
   return array_[i + j*xSize_];
}

// Operator() 3D
inline Real& Array::operator ()(size_t i, size_t j, size_t k)
{
	ASSERT_MSG(i < xSize_ && j < ySize_ && k < zSize_, "Access value too large");
	return array_[i + j * xSize_ + k * xSize_ * ySize_];
}

inline const Real & Array::operator () ( size_t i ) const
{
    ASSERT_MSG(i < xSize_, "Access value too large");
	return array_[i];
}

inline const Real & Array::operator () ( size_t i ,size_t j ) const
{
    ASSERT_MSG(i < xSize_ && j < ySize_, "Access value too large");
	return array_[i + j * xSize_];
}

inline const Real & Array::operator () ( size_t i, size_t j, size_t k ) const
{
    ASSERT_MSG(i < xSize_ && j < ySize_ && k < zSize_, "Access value too large");
	return array_[i + j * xSize_ + k * xSize_ * ySize_];
}

