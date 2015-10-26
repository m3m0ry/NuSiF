#include "Array.hh"

//==================
//
//  Constructors
//
//==================

//1D
Array::Array( MySize xSize )
{
	ASSERT_MSG(xSize > 0, "Array size too small");
	array_ = new Real[xSize];
	size_ = xSize;
	xSize_ = xSize;
	ySize_ = 1;
	zSize_ = 1;
	dimension_ = 1;
}

//2D
Array::Array( MySize xSize, MySize ySize )
{
	ASSERT_MSG(xSize > 0 && ySize > 0, "Array size too small");
	array_ = new Real[xSize * ySize];
	size_ = xSize*ySize;
	xSize_ = xSize;
	ySize_ = ySize;
	zSize_ = 1;
	dimension_ = 2;
}

//3D
Array::Array( MySize xSize, MySize ySize, MySize zSize )
{
	ASSERT_MSG(xSize > 0 && ySize > 0 && zSize > 0, "Array size too small");
	array_ = new Real[xSize * ySize * zSize];
	size_ = xSize*ySize*zSize;
	xSize_ = xSize;
	ySize_ = ySize;
	zSize_ = zSize;
	dimension_ = 3;
}

//Copy constructor
Array::Array(const Array & other) : size_(other.size_), xSize_(other.xSize_), ySize_(other.ySize_), zSize_(other.zSize_)
{
	//TODO memcpy allocate
}

//Destructor
Array::~Array(){
	delete[] array_;
}


//===========================
//
//  Convenience Functions
//
//===========================


//Initialize the whole array with a constant value
void Array::fill( Real value )
{
	std::fill(array_, array_ + size_, value);
}


// Print the whole array (for debugging purposes)
void Array::print()
{
   // For 2D Arrays the positive x-coordinate goes to the right
   //                   positive y-coordinate goes upwards
   //      -> the line with highest y-value should be printed first
	for(MySize z = 0; z < zSize_; ++z)
	{
		std::cout << "----------------------" << std::endl;
	   for(MySize y = ySize_ -1; y <ySize_; --y)
	   {
		   for( MySize x = 0; x < xSize_; ++x)
		   {
			   std::cout << " " << (*this)(z,y,x);
		   }
		   std::cout << std::endl;
	   }
	}
}

MySize Array::getSize( char dimension ) const
{
	ASSERT_MSG(dimension < 3, "Dimension toolarge");
	if(0 == dimension)
		return xSize_;
	if(1 == dimension)
		return ySize_;
	if(2 == dimension)
		return zSize_;
   return size_;
}

//return total size of the array
MySize Array::getSize() const
{
   return size_;
}

//=================
//
//  Operators
//
//=================

Array& Array::operator= (const Array& other)
{
	Array tmp(other);
	std::swap( size_, tmp.size_);
	std::swap( xSize_, tmp.xSize_);
	std::swap( ySize_, tmp.ySize_);
	std::swap( zSize_, tmp.zSize_);
	std::swap( dimension_, tmp.dimension_);
	std::swap( array_, tmp.array_);
	return *this;
}


Array Array::operator+ (const Array& other) const
{
	ASSERT_MSG(xSize_ == other.xSize_ && ySize_ == other.ySize_ && zSize_ == other.zSize_,
			"Matrix size not equal");
	Array result( xSize_, ySize_, zSize_ );
	for(MySize i = 0; i < zSize_; ++i)
	{
		for(MySize j = 0; j < ySize_; ++j)
		{
			for(MySize k = 0; k < xSize_; ++k)
			result(i,j,k) = (*this)(i,j,k) + other(i,j,k);
		}
	}
	return result;
}

Array& Array::operator+= (const Array& other)
{
	ASSERT_MSG(xSize_ == other.xSize_ && ySize_ == other.ySize_ && zSize_ == other.zSize_,
			"Matrix size not equal");
	for(MySize i = 0; i < zSize_; ++i)
	{
		for(MySize j = 0; j < ySize_; ++j)
		{
			for(MySize k = 0; k < xSize_; ++k)
			(*this)(i,j,k) += other(i,j,k);
		}
	}
	return *this;
}

Array Array::operator+ (const Real value) const
{
	Array result( xSize_, ySize_, zSize_ );
	for(MySize i = 0; i < zSize_; ++i)
	{
		for(MySize j = 0; j < ySize_; ++j)
		{
			for(MySize k = 0; k < xSize_; ++k)
			result(i,j,k) = (*this)(i,j,k) + value;
		}
	}
	return result;
}
