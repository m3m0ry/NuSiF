#include "Array.hh"



//==================
//
//  Constructors
//
//==================


//1D
Array::Array( int xSize )
{
	CHECK_MSG(xSize > 1, "Array size too small");
	array_ = new real[xSize];
	size_ = xSize;
	xSize_ = xSize;
	ySize_ = 1;
	zSize_ = 1;
}

//2D
Array::Array( int xSize, int ySize )
{
	CHECK_MSG(xSize > 1 && ySize > 1, "Array size too small");
	array_ = new real[xSize * ySize];
	size_ = xSize*ySize;
	xSize_ = xSize;
	ySize_ = ySize;
	zSize_ = 1;
}

//3D
Array::Array( int xSize, int ySize, int zSize )
{
	CHECK_MSG(xSize > 1 && ySize > 1 && zSize > 1, "Array size too small");
	array_ = new real[xSize * ySize * zSize];
	size_ = xSize*ySize*zSize;
	xSize_ = xSize;
	ySize_ = ySize;
	zSize_ = zSize;
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


//initialize the whole array with a constant value
void Array::fill( real value )
{
	std::fill(array_, array_ + size_, value);
}


// Print the whole array (for debugging purposes)
void Array::print()
{
   // For 2D Arrays the positive x-coordinate goes to the right
   //                   positive y-coordinate goes upwards
   //      -> the line with highest y-value should be printed first
	for(int z = 0; z < zSize_; ++z)
	{
		std::cout << "----------------------" << std::endl;
	   for(int y = ySize_ -1; y >= 0; --y)
	   {
		   for( int x = 0; x < xSize_; ++x)
		   {
			   std::cout << array_[x + y*xSize_ + z*xSize_*ySize_] << " ";
		   }
		   std::cout << std::endl;
	   }
	}
}

int Array::getSize( int dimension ) const
{
	if(1 == dimension)
		return xSize_;
	if(2 == dimension)
		return ySize_;
	if(3 == dimension)
		return zSize_;
	WARN("Invalid dimension given, returning size");
   return size_;
}

//return total size of the array
int Array::getSize() const
{
   return size_;
}
