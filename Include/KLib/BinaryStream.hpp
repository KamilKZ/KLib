#pragma once

#include <iostream>

#include <KLib/Config.hpp>
#include <KLib/ArrayList.hpp>
#include <KLib/Number.hpp>
#include <KLib/String.hpp>
#include <KLib/Logging.hpp>
#include <KLib/ISerializable.hpp>
#include <KLib/ByteBuffer.hpp> // ByteBuffer, buffering for String
#include <KLib/File.hpp>

namespace klib
{
namespace io
{

//class ByteBuffer; // Forward declaration
//class BinaryFile;
//class ISerializable;

class API_EXPORT BinaryStream
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Default Constructor
	///
	/// This constructor is actually a fail in terms of BinaryStream,
	/// and raises an error.
	///
	///////////////////////////////////////////////////////////
	BinaryStream()
	{
		KL_ERROR("BinaryStream initialized wrong");
	}

	///////////////////////////////////////////////////////////
	/// \brief iostream Constructor
	///
	/// Creates a BinaryStream layer on top of a std::iostream
	///
	///////////////////////////////////////////////////////////
	BinaryStream(std::iostream* baseStream)
	{
		mStream = baseStream;
		mMode = ( io::FileModes::Read | io::FileModes::Write );
	}

	///////////////////////////////////////////////////////////
	/// \brief BinaryFile Constructor
	///
	/// Creates a BinaryStream layer on top of a BinaryFile
	///
	/// It actually uses std::iostream, but it also get's the
	/// file's openmode
	///
	///////////////////////////////////////////////////////////
	BinaryStream(BinaryFile& file)
	{
		mStream = &file.GetStream();
		mMode = file.GetMode();
	}

	///////////////////////////////////////////////////////////
	/// \brief Default Destructor
	///
	/// <nothing>
	///
	///////////////////////////////////////////////////////////
	~BinaryStream() {};
	
	///////////////////////////////////////////////////////////
	/// \brief Get the size of the stream
	///
	/// This function may be expensive if called over and over again,
	/// so if you need this value multiple times, store it.
	///
	/// \return stream size in bytes
	///
	///////////////////////////////////////////////////////////
	inline UInt GetSize()
	{
		KL_ASSERT(( mMode & io::FileModes::Read ) > 0);
		UInt pos = Tell(); // store current pos to restore
		Seek(0, std::ios::end); // go to end of file
		UInt length = Tell(); // retrieve pos at end
		Seek(pos); // restore last position
		return length;
	}

	///////////////////////////////////////////////////////////
	/// \brief Check to see if the stream is good
	///
	/// Checks to see if any bad flags are set, this
	/// function is what is returned by I/O operations
	/// on the stream, so you don't have to call it if
	/// you use those.
	///
	/// \code
	/// if (stream >> var)
	/// \endcode
	/// is the same as
	/// \code
	/// stream >> var; if (stream.IsHealthy())
	/// \endcode
	/// except IsHealthy actually gets called twice
	///
	/// \return healthy
	///
	///////////////////////////////////////////////////////////
	inline bool IsHealthy() const
	{
		return mStream->rdstate() == std::ios::goodbit;
	}

	///////////////////////////////////////////////////////////
	/// \brief Move pointer to a new position
	///
	/// \param pos Position to move pointer to
	/// \param way Optional direction to move from
	///
	/// \return stream healthy
	///
	///////////////////////////////////////////////////////////
	inline bool Seek(UInt pos, std::ios::seekdir way = std::ios::beg)
	{
		mStream->seekg(pos, way);
		return IsHealthy();
	}

	///////////////////////////////////////////////////////////
	/// \brief Skip pointer
	///
	/// \param amount Amount to skip by, can be negative
	///
	/// \return stream healthy
	///
	///////////////////////////////////////////////////////////
	inline bool Skip(Int amount)
	{
		mStream->seekg(amount, std::ios_base::cur);
		return IsHealthy();
	}

	///////////////////////////////////////////////////////////
	/// \brief Returns the current position of the pointer
	///
	/// \return Position of pointer
	///
	///////////////////////////////////////////////////////////
	inline UInt Tell() const
	{
		return mStream->tellg();
	}

	///////////////////////////////////////////////////////////
	/// \brief Check if the reading position reached the end of the stream
	///
    /// This function is useful to know if there is some data
    /// left to be read, without actually reading it.
	///
	/// \return True if all data was read, false otherwise
	///
	///////////////////////////////////////////////////////////
	inline bool IsEnd() const
	{
		KL_ASSERT(( mMode & io::FileModes::Read ) > 0);
		return ( mStream->rdstate() == std::ios::eofbit ) ||
			( Tell() == (UInt)(mStream->end) );
	}
	
	////////////////////////////////////////////////////////////
    /// \brief Test the validity of the stream, for reading
    ///
    /// This operator allows to test the stream as a boolean
    /// variable, to check if a reading operation was successful.
    ///
    /// A stream will be in an invalid state if it has no more
    /// data to read, or the read failed.
    ///
    /// This behaviour is the same as standard C++ streams.
    ///
    /// Usage example:
    /// \code
    /// float x;
    /// stream >> x;
    /// if (stream)
    /// {
    ///    // ok, x was extracted successfully
    /// }
    ///
    /// // -- or --
    ///
    /// float x;
    /// if (stream >> x)
    /// {
    ///    // ok, x was extracted successfully
    /// }
    /// \endcode
    ///
    /// \return True if last data extraction from stream was successful
    ///
    /// \see IsHealthy
    ///
    ////////////////////////////////////////////////////////////
	inline operator bool() const
	{
		return IsHealthy();
	}
	
	///////////////////////////////////////////////////////////
	/// \brief Read raw bytes
	///
	/// Provides Low-level access to stream, possibly for custom
	/// data access
	///
	/// \param buffer char* bytes buffer to store loaded data to
	/// \param bytes Number of bytes to read
	///
	/// \return success
	///
	///////////////////////////////////////////////////////////
	inline bool Read(char* buffer, UInt bytes)
	{
		KL_ASSERT(( mMode & io::FileModes::Read ) > 0);
		mStream->read(buffer, bytes);
		return IsHealthy();
	}

	///////////////////////////////////////////////////////////
	/// \brief Write raw bytes
	///
	/// Provides Low-level access to stream, possibly for custom
	/// data access
	///
	/// \param buffer char* bytes buffer to write
	/// \param bytes Number of bytes to store
	///
	/// \return success
	///
	///////////////////////////////////////////////////////////
	inline bool Write(const char* data, UInt bytes)
	{
		KL_ASSERT(( mMode & io::FileModes::Write ) > 0);
		mStream->write(data, bytes);
		return IsHealthy();
	}

	///////////////////////////////////////////////////////////
	/// \brief Read from stream
	///
	/// A template function which converts a set of
	/// bytes into basic types. 
	///
	/// \param data Reference to store read data into
	///
	/// \return success
	///
	/// \code
	/// UInt four_bytes;
	/// stream >> four_bytes;
	/// Double myVar;
	/// stream >> myVar;
	/// \endcode
	///
	/// Note: Template specialization is optional
	///
	///////////////////////////////////////////////////////////
	template<typename T = String>
	inline BinaryStream& operator>>(T& data) // Read Type
	{
		// Make sure we can read
		KL_ASSERT((mMode & io::FileModes::Read) > 0);
		
		// Then write basic type
		Read((char*)&data, sizeof(T));
		
		// Return self, because stream
		return *this;
	}
	
	template<typename T, typename Array = ArrayList<T>>
	inline BinaryStream& operator>>(Array& data)
	{
		// Make sure we can red
		KL_ASSERT((mMode & io::FileModes::Read) > 0);

		// First read array size
		UInt length;
		*this >> length;

		// Then read contents
		data.clear();
		for (UInt i = 0; i < length; i++)
		{
			T value;
			*this >> value;
			data.push_back(value);
		}

		return *this;
	}

	template<typename T = ByteBuffer>
	inline BinaryStream& operator>>(ByteBuffer& data)
	{
		// Make sure we can write
		KL_ASSERT( mMode & io::FileModes::Read) > 0);

		// First extract data length
		UInt length = 0;
		*this >> length;

		data = ByteBuffer(length);

		// Then insert data
		if (length > 0)
			Read(data, length);

		return *this;
	}

	template<typename T = String>
	inline BinaryStream& operator>>(String& data)
	{
		// Make sure we can read
		KL_ASSERT((mMode & io::FileModes::Read) > 0);

		// First extract string length
		UInt length = 0;
		*this >> length;

		data.clear();

		if (length > 0)
		{
			// Then extract characters
			ByteBuffer buffer(length);
			Read(buffer, length);
			data.assign(buffer, length);
		}

		return *this;
	}
	
	inline BinaryStream& operator>>(char* data)
	{
		// Make sure we can read
		KL_ASSERT((mMode & io::FileModes::Read) > 0);

		// Rirst extract length of data
		UInt length = 0;
		*this >> length;

		// Then extract characters
		if (length > 0)
		{
			Read(data, length);
			data[length] = '\0';
		}

		// Return self, because stream
		return *this;
	}
	
	///////////////////////////////////////////////////////////
	/// \brief Write to stream
	///
	/// A template function which converts basic types
	/// into a set of bytes, which are written to the stream
	/// through Write(buf, bytes)
	///
	/// \param data Data to write
	///
	/// \return success
	///
	/// \code
	/// UInt four_bytes = 123456;
	/// stream << four_bytes;
	/// Double myVar = 0.864;
	/// stream << myVar;
	/// \endcode
	///
	/// Note: Template specialization is optional
	///
	///////////////////////////////////////////////////////////
	template<typename T>
	inline BinaryStream& operator<<(const T& data) // Write Type
	{
		// Make sure we can write
		KL_ASSERT((mMode & io::FileModes::Write) > 0);
		
		// Write raw bytes from 'T data'
		Write((char*)&data, sizeof(T));
		
		// Return self, because stream
		return *this;
	}
	
	template<typename T, typename Array = ArrayList<T>>
	inline BinaryStream& operator<<(const Array& data)
	{
		// Make sure we can write
		KL_ASSERT((mMode & io::FileModes::Write) > 0);

		// First write array size
		UInt length = static_cast<UInt>(data.size());
		*this << length;

		// Then write contents
		for (T value : data)
		{
			*this << value;
		}

		return *this;
	}

	template<typename T = ByteBuffer>
	inline BinaryStream& operator<<(const ByteBuffer& data)
	{
		// Make sure we can write
		KL_ASSERT((mMode & io::FileModes::Write) > 0);

		// First insert data length
		UInt length = static_cast<UInt>( data.GetSize() );
		*this << length;

		// Then insert data
		if (length > 0)
			Write(data, length);

		return *this;
	}

	template<typename T = String>
	inline BinaryStream& operator<<(const String& data)
	{
		// Make sure we can write
		KL_ASSERT((mMode & io::FileModes::Write) > 0);

		// First insert string length
		UInt length = static_cast<UInt>( data.size() );
		*this << length;

		// Then insert characters
		if (length > 0)
			Write(data.c_str(), length * sizeof(String::value_type));

		return *this;
	}
	
	inline BinaryStream& operator<<(const char* data)
	{
		// Make sure we can write
		KL_ASSERT((mMode & io::FileModes::Write) > 0);

		// First insert string length
		UInt length = std::strlen(data);
		*this << length;

		// Then insert characters
		if (length > 0)
			Write(data, length * sizeof(char));

		return *this;
	}

protected:
	std::iostream* mStream;
	std::ios::openmode mMode;
};

///////////////////////////////////////////////////////////
/// \class BinaryStream
/// \brief A helper class/stream used to aid with binary I/O on objects.
///
/// It offers String and Array read/write support,
/// as well as all the basic types, and possibly
/// even any arbitrary struct type.
///
/// \code
/// BinaryStream bs;
/// // Write
/// bs << UInt(123) << String("hello");
///
/// // Extract 
/// UInt value;
/// String string;
/// bs >> value >> string;
/// \endcode
///
/// Usage with BinaryFile:
///
/// \code
/// BinaryFile file("Binary.test", std::ios::out);
/// BinaryStream stream(file);
/// 
/// stream << values;
/// stream << String("hello binary");
/// \endcode
///
/// \see BinaryFile, ISerializable
///
///////////////////////////////////////////////////////////

} // io
} // klib
