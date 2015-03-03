#pragma once

#include <KLib/Number.hpp>

namespace klib
{
namespace io
{

class ByteBuffer
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Default Constructor
	///
	/// Create a new buffer with size of 'bytes'
	///
	/// \param bytes Buffer size to create
	///
	///////////////////////////////////////////////////////////
	ByteBuffer(ULong bytes)
	{
		mData = new char[bytes];
		mSize = bytes;
	}

	///////////////////////////////////////////////////////////
	/// \brief Default Destructor
	///
	/// Very useful for managing a buffer because using the
	/// destructor, memory can be cleaned up when out of scope
	///
	///////////////////////////////////////////////////////////
	~ByteBuffer()
	{
		if (mData)
			delete[] mData;
	}

	///////////////////////////////////////////////////////////
	/// \brief Allows using the object like an actual byte buffer
	///
	/// Example usage:
	/// \code
	/// void SomeFunc(char* buf, UInt bytes);
	///
	/// ByteBuffer buffer(1024);
	/// SomeFunc(buffer, 1024);
	/// \endcode
	///
	/// \return char* Pointer to buffer
	///
	///////////////////////////////////////////////////////////
	operator char*( ) const
	{
		return mData;
	}

	///////////////////////////////////////////////////////////
	/// \brief Get the size of the buffer in bytes
	///
	/// \return Size of buffer in bytes
	///
	///////////////////////////////////////////////////////////
	ULong GetSize() const
	{
		return mSize;
	}

private:
	char* mData;
	ULong mSize;
};
///////////////////////////////////////////////////////////
/// \class ByteBuffer
/// \brief Managed wrapper for a 'char* buffer'
///
/// Wraps up the char* buffer so that it's easier to use and
/// doesn't leak memory.
///
/// The destructor deletes the memory, so you don't have to
/// worry about cleaning up.
///
/// Example usage:
/// \code
/// ByteBuffer buffer(data.GetSize());
/// return data.Read(buffer, data.GetSize());
/// // Memory not leaked!
/// \endcode
///
///////////////////////////////////////////////////////////

} // io
} // klib