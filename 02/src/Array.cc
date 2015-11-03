#include "Array.hh"

//==================
//
//  Constructors
//
//==================

//Default
Array::Array( )
{
	array_ = 0;
	size_ = 0;
	xSize_ = 0;
	ySize_ = 0;
	zSize_ = 0;
	dimension_ = 0;
}

//1D
Array::Array( size_t xSize )
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
Array::Array( size_t xSize, size_t ySize )
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
Array::Array( size_t xSize, size_t ySize, size_t zSize )
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
Array::Array(const Array & other) : size_(other.size_), xSize_(other.xSize_), ySize_(other.ySize_),
										zSize_(other.zSize_), array_(new Real[xSize_ * ySize_ * zSize_])
{
	memcpy(array_, other.array_, xSize_ * ySize_ *zSize_ * sizeof(Real)); 
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
void Array::print() const
{
	std::cout << *this;
}


size_t Array::getSize( char dimension ) const
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
size_t Array::getSize() const
{
   return size_;
}

//=================
//
//  Operators
//
//=================
std::ostream& operator<<(std::ostream& stream, const Array &a)
{
   // For 2D Arrays the positive x-coordinate goes to the right
   //                   positive y-coordinate goes upwards
   //      -> the line with highest y-value should be printed first
	for(size_t z = 0; z < a.getSize(2); ++z)
	{
		stream << "#----------------------" << std::endl;
	   for(size_t y = a.getSize(1) -1; y <a.getSize(1); --y)
	   {
		   for( size_t x = 0; x < a.getSize(0); ++x)
		   {
			   stream << " " << a(x,y,z);
		   }
		   stream << std::endl;
	   }
	}
	return stream;
}

Array& Array::operator= (const Array& other)
{
	if( this == &other )
		return *this;
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
	for(size_t k = 0; k < zSize_; ++k)
	{
		for(size_t j = 0; j < ySize_; ++j)
		{
			for(size_t i = 0; i < xSize_; ++i)
			result(i,j,k) = (*this)(i,j,k) + other(i,j,k);
		}
	}
	return result;
}

Array& Array::operator+= (const Array& other)
{
	ASSERT_MSG(xSize_ == other.xSize_ && ySize_ == other.ySize_ && zSize_ == other.zSize_,
			"Matrix size not equal");
	for(size_t k = 0; k < zSize_; ++k)
	{
		for(size_t j = 0; j < ySize_; ++j)
		{
			for(size_t i = 0; i < xSize_; ++i)
			(*this)(i,j,k) += other(i,j,k);
		}
	}
	return *this;
}

Array Array::operator+ (const Real value) const
{
	Array result( xSize_, ySize_, zSize_ );
	for(size_t k = 0; k < zSize_; ++k)
	{
		for(size_t j = 0; j < ySize_; ++j)
		{
			for(size_t i = 0; i < xSize_; ++i)
			result(i,j,k) = (*this)(i,j,k) + value;
		}
	}
	return result;
}
