#ifndef ARRAY_HH
#define ARRAY_HH

#include <cstdlib>
#include <iostream>

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
   Array( int xSize );
   Array( int xSize, int ySize );
   Array( int xSize, int ySize, int zSize );
   Array(const Array& other);
   ~Array();

   Array& operator= (const Array& other);
   Array operator+ (const Array& other);
   Array operator+ (const int value);
   Array operator- (const Array& s);
   Array operator- (const int value);

   // Access Operators for 1D, 2D and 3D
   inline real & operator () ( int i );
   inline real & operator () ( int i ,int j );
   inline real & operator () ( int i, int j, int k );

   // for const Arrays the following access operators are required
    inline const real & operator () ( int i ) const;
    inline const real & operator () ( int i ,int j ) const;
    inline const real & operator () ( int i, int j, int k ) const;



   // initialize the whole array with a constant value
   void fill( real value );


   // return total size of the array
   int getSize() const;

   // return xSize for dimension==0, ySize for dimension==1 and zSize for dimension==2
   // other dimension values are not allowed
   int getSize(int dimension ) const;


   // Print the whole array ( for debugging purposes )
   void print();

private:
	int size_ = 0;
	int xSize_ = 0;
	int ySize_ = 0;
	int zSize_ = 0;
	real * array_;
};


//=======================================
//
//  Inline Access Operators and Sizes
//
//=======================================


// Operator() 1D
inline real& Array::operator ()(int i)
{
	//TODO checks
   return array_[i];
}

// Operator() 2D
inline real& Array::operator ()(int i,int j)
{
	//TODO checks
   return array_[i + j*xSize_];
}

// Operator() 3D
inline real& Array::operator ()(int i, int j, int k)
{
   //TODO checks
   return array_[i + j * xSize_ + k * xSize_ * ySize_];
}

inline const real & Array::operator () ( int i ) const
{
	return array_[i];
}

inline const real & Array::operator () ( int i ,int j ) const
{
	return array_[i + j * xSize_];
}

inline const real & Array::operator () ( int i, int j, int k ) const
{
	return array_[i + j * xSize_ + k * xSize_ * ySize_];
}

Array& Array::operator= (const Array& other)
{
	Array tmp(other);
	std::swap( size_, tmp.size_);
	std::swap( xSize_, tmp.xSize_);
	std::swap( ySize_, tmp.ySize_);
	std::swap( zSize_, tmp.zSize_);
	std::swap( array_, tmp.array_);
	return *this;
}

Array Array::operator+ (const Array& other)
{
//	assert( size_ == other.size_);
	Array result( size_ );
	for(int i = 0; i < size_; ++i)
	{
		result.array_[i] = array_[i] + other.array_[i];
	}
	return result;
}


#endif //ARRAY_HH

