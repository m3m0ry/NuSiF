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
	Array( );
   Array( MySize xSize );
   Array( MySize xSize, MySize ySize );
   Array( MySize xSize, MySize ySize, MySize zSize );
   Array(const Array& other);
   ~Array();

   //Array operator- (const Array& s);
   //Array operator- (const int value);

   // Access Operators for 1D, 2D and 3D
   inline Real & operator () ( MySize i );
   inline Real & operator () ( MySize i ,MySize j );
   inline Real & operator () ( MySize i, MySize j, MySize k );

   // for const Arrays the following access operators are required
    inline const Real & operator () ( MySize i) const;
    inline const Real & operator () ( MySize i, MySize j) const;
    inline const Real & operator () ( MySize i, MySize j, MySize k ) const;


	//Operators
	Array& operator= (const Array& other);
	Array& operator+= (const Array& other);
	Array operator+ (const Array& other) const;
	Array operator+ (const Real value) const;

   // initialize the whole array with a constant value
   void fill( Real value );


   // return total size of the array
   MySize getSize() const;

   // return xSize for dimension==0, ySize for dimension==1 and zSize for dimension==2
   // other dimension values are not allowed
   MySize getSize(char dimension ) const;


   // Print the whole array ( for debugging purposes )
   void print();

private:
	MySize size_;
	MySize xSize_;
	MySize ySize_;
	MySize zSize_;
	Real * array_;
	char dimension_;
};


//=======================================
//
//  Inline Access Operators and Sizes
//
//=======================================


// Operator() 1D
inline Real& Array::operator ()(MySize i)
{
   ASSERT_MSG(i < xSize_, "Access value too large");
   return array_[i];
}

// Operator() 2D
inline Real& Array::operator ()(MySize j,MySize i)
{
   ASSERT_MSG(i < xSize_ && j < ySize_, "Access value too large");
   return array_[i + j*xSize_];
}

// Operator() 3D
inline Real& Array::operator ()(MySize k, MySize j, MySize i)
{
	ASSERT_MSG(i < xSize_ && j < ySize_ && k < zSize_, "Access value too large");
	return array_[i + j * xSize_ + k * xSize_ * ySize_];
}

inline const Real & Array::operator () ( MySize i ) const
{
    ASSERT_MSG(i < xSize_, "Access value too large");
	return array_[i];
}

inline const Real & Array::operator () ( MySize j ,MySize i ) const
{
    ASSERT_MSG(i < xSize_ && j < ySize_, "Access value too large");
	return array_[i + j * xSize_];
}

inline const Real & Array::operator () ( MySize k, MySize j, MySize i ) const
{
    ASSERT_MSG(i < xSize_ && j < ySize_ && k < zSize_, "Access value too large");
	return array_[i + j * xSize_ + k * xSize_ * ySize_];
}


#endif //ARRAY_HH

