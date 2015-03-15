#pragma once

#include <iostream>
#include <fstream>

#include <KLib/Config.hpp>
#include <KLib/ArrayList.hpp>
#include <KLib/Number.hpp>
#include <KLib/String.hpp>
#include <KLib/ISerializable.hpp>
#include <KLib/ByteBuffer.hpp>
#include <KLib/Logging.hpp>


///Todo
///* ~~Document all~~
///* Use different mode setting so you can:
///		*Read - ReadOnly (ios::in)
//		*Write - WriteOnly, overwrite (ios::out, ios::trunc)
//		*Append - WriteOnly, append (ios::out, ios::app)
//		*ReadWrite - ReadWrite (ios::in, ios::out)


namespace klib
{
class ISerializable;

namespace io
{

typedef Byte FileMode;

enum FileModes : FileMode
{
	Read = 1, // (Default) Allow input operations, pointer at start (0)
	Write = 2, // Allow output operations, preserve previous data, pointer at start (0)
	Overwrite = 4, // Allow output operations, erase previous data, pointer at start (0)
	Append = 8, // Allow output operations, preserve previous data, pointer at end of file
	Binary = 16 // Open in binary mode

	//Note: These can be mixed, i.e. (FileMode::Read | FileMode::Write), apart from Overwrite to Append, the rest are compatible
};

namespace priv
{
std::ios::openmode TranslateFileMode(FileMode mode)
{
	std::ios::openmode openmode;

	if (mode && FileModes::Read > 0) { openmode |= std::ios::in; }
	if (mode && FileModes::Overwrite > 0) { openmode |= ( std::ios::out | std::ios::trunc ); }
	if (mode && FileModes::Write > 0) { openmode |= std::ios::out; }
	if (mode && FileModes::Append > 0) { openmode |= ( std::ios::out | std::ios::app ); }
	if (mode && FileModes::Binary > 0) { openmode |= std::ios::binary; }

	return openmode;
}
}

///////////////////////////////////////////////////////////
/// \namespace te::io
/// \defgroup FileIO
/// \ingroup FileIO
///
/// A set of classes for manipulating files
///
///////////////////////////////////////////////////////////

class API_EXPORT FileBase
{
public:
	virtual ~FileBase()
	{
		if (IsOpen())
			Close();
	}

	///////////////////////////////////////////////////////////
	/// \brief Open a new file
	///
	/// Attempts to open a new file at the path specified
	/// and the mode given.
	///
	/// If the file couldn't be opened: returns false
	/// and logs a warning.
	/// Otherwise it returns true.
	///
	/// Overloadable, so that subclass can implement
	/// their own way to open files.
	/// For example, BinaryFile adds a std::ios::binary
	/// flag to the open mode.
	///
	/// \param path Path to file, from executable
	/// \param mode ios::openmode to open file with (default is in/out)
	///
	/// \return File opened successfully
	///
	///////////////////////////////////////////////////////////
	virtual inline bool Open(String path, FileMode mode = FileModes::Read)
	{
		std::ios::openmode mode_translated = priv::TranslateFileMode(mode);

		mStream.open(path, mode_translated);
		mOpen = mStream.is_open();
		mMode = mode;

		if (!mOpen)
			KL_WARNING("Failed to open file '" + path + "'");

		return mOpen;
	}

	///////////////////////////////////////////////////////////
	/// \brief Check to see if the file has been opened
	///
	/// \return File opened successfully
	///
	///////////////////////////////////////////////////////////
	inline bool IsOpen() const
	{
		return mOpen;
	}

	///////////////////////////////////////////////////////////
	/// \brief Close the file
	///
	/// Closes the file, and saves changes if the file
	/// open for output.
	///
	///////////////////////////////////////////////////////////
	inline void Close()
	{
		if (( mMode & std::ios::out ) > 0)
			Flush();

		mStream.close();
	}

	///////////////////////////////////////////////////////////
	/// \brief Push any changes to and save the file
	///
	///////////////////////////////////////////////////////////
	inline void Flush()
	{
		KL_ASSERT((mMode & io::FileModes::Write) > 0);
		mStream.flush();
	}

	///////////////////////////////////////////////////////////
	/// \brief Get the size of the file
	///
	/// This function may be expensive if called over and over again,
	/// so if you need this value multiple times, store it.
	///
	/// \return file size in bytes
	///
	///////////////////////////////////////////////////////////
	inline UInt GetSize()
	{
		KL_ASSERT((mMode & io::FileModes::Read) > 0);
		UInt pos = Tell(); // store current pos to restore
		Seek(0, std::ios::end); // go to end of file
		UInt length = Tell(); // retrieve pos at end
		Seek(pos); // restore last position
		return length;
	}

	///////////////////////////////////////////////////////////
	/// \brief Check to see if the file is good
	///
	/// Checks to see if any bad flags are set, 
	/// this function is what is returned by I/O operations
	/// on files, so you don't have to call it if you use those.
	///
	/// \code
	/// if (!file.Read(..))
	/// \endcode
	/// is the same as
	/// \code
	/// file.Read(..); if (!file.IsHealthy())
	/// \endcode
	/// except IsHealthy actually gets called twice
	///
	/// \return healthy
	///
	///////////////////////////////////////////////////////////
	inline bool IsHealthy() const
	{
		return mStream.rdstate() == std::ios::goodbit;
	}

	///////////////////////////////////////////////////////////
	/// \brief Move pointer to a new position
	///
	/// \param pos Position to move pointer to
	/// \param way Optional direction to move from
	///
	/// \return file healthy
	///
	///////////////////////////////////////////////////////////
	inline bool Seek(UInt pos, std::ios::seekdir way = std::ios::beg)
	{
		mStream.seekg(pos, way);
		return IsHealthy();
	}

	///////////////////////////////////////////////////////////
	/// \brief Skip pointer
	///
	/// \param amount Amount to skip by, can be negative
	///
	/// \return file healthy
	///
	///////////////////////////////////////////////////////////
	inline bool Skip(Int amount)
	{
		mStream.seekg(amount, std::ios_base::cur);
		return IsHealthy();
	}

	///////////////////////////////////////////////////////////
	/// \brief Returns the current position of the pointer
	///
	/// \return Position of pointer
	///
	///////////////////////////////////////////////////////////
	inline UInt Tell()
	{
		return mStream.tellg();
	}

	///////////////////////////////////////////////////////////
	/// \brief Check if the reading position reached the end of the file
	///
    /// This function is useful to know if there is some data
    /// left to be read, without actually reading it.
	///
	/// \return True if all data was read, false otherwise
	///
	///////////////////////////////////////////////////////////
	inline bool IsEndOfFile()
	{
		KL_ASSERT((mMode & io::FileModes::Read) > 0);
		return ( mStream.rdstate() == std::ios::eofbit ) ||
			( Tell() == (Int)(mStream.end) );
	}

	///////////////////////////////////////////////////////////
	/// \brief Get the openmode
	///
	/// Returns the openmode used to open the file
	///
	/// \return File openmode
	///
	///////////////////////////////////////////////////////////
	inline FileMode GetMode() const
	{
		return mMode;
	}

	///////////////////////////////////////////////////////////
	/// \brief Get the fstream object
	///
	/// Returns a writable copy of the fstream object
	///
	/// \return file stream
	///
	///////////////////////////////////////////////////////////
	inline std::fstream& GetStream()
	{
		return mStream;
	}

protected:
	bool mOpen;
	String mPath;
	std::fstream mStream;
	FileMode mMode;
	UInt mSize;
};
///////////////////////////////////////////////////////////
/// \class FileBase
/// \brief Baseclass for File IO; doesn't have read/write functionality
/// \ingroup FileIO
///
/// Used as a base class by TextFile and BinaryFile, 
/// cannot be used to read files directly
///
/// \see TextFile, BinaryFile
///
///////////////////////////////////////////////////////////

class API_EXPORT TextFile : public FileBase
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Default Constructor
	///
	/// Initializes the class, but doesn't load any files.
	/// See either the second constructor or Open()
	/// if you would like to open a file.
	///
	/// Example usage with this constructor:
	/// \code
	/// TextFile file;
	/// for(String fileName : files)
	/// {
	///     if(file.Open(fileName))
	///     {
	///         file.Write("hello world");
	///         file.Close();
	///     }
	/// }
	/// \endcode
	///
	///////////////////////////////////////////////////////////
	TextFile() {};

	///////////////////////////////////////////////////////////
	/// \brief 'Attempt-Open' Constructor
	///
	/// Initializes the class, and attempts to open a file.
	/// Use IsOpen() to check if the file was actually
	/// opened.
	///
	/// Example usage with this constructor:
	/// \code
	/// String contents;
	/// TextFile file(path, std::ios::in);
	/// if (file.IsOpen())
	/// {
	///     contents = file.ReadAll();
	///     file.Close();
	/// }
	/// \endcode
	///
	/// Interally, this uses Open() but there is no
	/// way to return the bool from a constructor
	///
	/// \param path File to open
	/// \param mode ios::openmode to open file with (default is in/out)
	///
	///////////////////////////////////////////////////////////
	TextFile(String path, FileMode mode = FileModes::Read)
	{
		Open(path, mode);
	}

	///////////////////////////////////////////////////////////
	/// \brief Read some characters
	///
	/// Reads 'size' amount of characters and returns them
	/// Reads forwards from the position of the pointer.
	///
	/// \param size Amount of characters to read
	///
	/// \return Characters as a String
	///
	///////////////////////////////////////////////////////////
	inline String Read(UInt size)
	{
		KL_ASSERT((mMode & io::FileModes::Read) > 0);
		char* data = new char[size];
		mStream.read(data, size);
		String strData(data);
		SAFE_DELETE_ARRAY(data);
		return strData;
	}

	///////////////////////////////////////////////////////////
	/// \brief Reads a line
	///
	/// Reads a line and returns it as a String
	///
	/// Note: not sure what happens when pointer is in middle
	///
	/// \return Line as a String
	///
	///////////////////////////////////////////////////////////
	inline String ReadLine()
	{
		KL_ASSERT((mMode & io::FileModes::Read) > 0);
		String str;
		std::getline(mStream, str);
		return str;
	}

	///////////////////////////////////////////////////////////
	/// \brief Read whole of the file
	///
	/// Reads the whole file and return it as a String
	///
	/// \return File contents as a String
	///
	///////////////////////////////////////////////////////////
	inline String ReadAll()
	{
		KL_ASSERT((mMode & io::FileModes::Read) > 0);
		String str;
		str.reserve(GetSize());
		str.assign(std::istreambuf_iterator<char>(mStream),
				   std::istreambuf_iterator<char>());

		return str;
	}

	///////////////////////////////////////////////////////////
	/// \brief Write to file
	///
	/// Writes a string at the position of the pointer
	///
	/// \return File healthy (write successful)
	///
	///////////////////////////////////////////////////////////
	inline bool Write(const String& data)
	{
		KL_ASSERT((mMode & io::FileModes::Write) > 0);
		mStream << data;
		return IsHealthy();
	}

	///////////////////////////////////////////////////////////
	/// \brief Write a line to file
	///
	/// Writes a line at the position of the pointer.
	///
	/// Same as Write() but includes a linebreak at the end of the string.
	///
	/// \return File healthy (write successful)
	///
	///////////////////////////////////////////////////////////
	inline bool WriteLine(const String& data)
	{
		KL_ASSERT((mMode & io::FileModes::Write) > 0);
		mStream << data << '\r\n';
		return IsHealthy();
	}
};
///////////////////////////////////////////////////////////
/// \class TextFile
/// \brief File which can be used to read and write text.
/// \ingroup FileIO
///
/// Includes purely string-based functions.
///
/// \code
/// file.Read(chars);
/// file.ReadLine();
/// file.ReadAll();
/// file.Write(output);
/// foreach(list as line)
///     file.WriteLine(line);
/// \endcode
///
/// \see FileBase
///
///////////////////////////////////////////////////////////

class API_EXPORT BinaryFile : public FileBase // templates must be in header file
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Default Constructor
	///
	/// Initializes the class, but doesn't load any files.
	/// See either the second constructor or Open()
	/// if you would like to open a file.
	///
	/// Example usage with this constructor:
	/// \code
	/// BinaryFile file;
	/// for(String fileName : files)
	/// {
	///     if(file.Open(fileName))
	///     {
	///         file << data;
	///         file.Close();
	///     }
	/// }
	/// \endcode
	///
	///////////////////////////////////////////////////////////
	BinaryFile() {};

	///////////////////////////////////////////////////////////
	/// \brief 'Attempt-Open' Constructor
	///
	/// Initializes the class, and attempts to open a file.
	/// Use IsOpen() to check if the file was actually
	/// opened.
	///
	/// Example usage with this constructor:
	/// \code
	/// ByteBuffer buffer;
	/// BinaryFile file(path, std::ios::in);
	/// if (file.IsOpen())
	/// {
	///     file.Read(buffer, file.GetSize());
	///     file.Close();
	/// }
	/// \endcode
	///
	/// Interally, this uses Open() but there is no
	/// way to return the bool from a constructor
	///
	/// \param path File to open
	/// \param mode ios::openmode to open file with (default is in/out)
	///
	///////////////////////////////////////////////////////////
	BinaryFile(String path, FileMode mode = FileModes::Read)
	{
		Open(path, mode);
	}

	///////////////////////////////////////////////////////////
	/// \brief Overloaded Open method
	///
	/// BinaryFile overloads this to add in the 
	/// std::ios::binary flag to openmode
	///
	/// \return Successful
	///
	/// \see FileBase::Open
	///
	///////////////////////////////////////////////////////////
	inline bool Open(String path, FileMode mode = FileModes::Read) override
	{
		return FileBase::Open(path, ( mode | FileModes::Binary ));
	}

	////////////////////////////////////////////////////////////
    /// \brief Test the validity of the file, for reading
    ///
    /// This operator allows to test the file as a boolean
    /// variable, to check if a reading operation was successful.
    ///
    /// A file will be in an invalid state if it has no more
    /// data to read, or the read failed.
    ///
    /// This behaviour is the same as standard C++ streams.
    ///
    /// Usage example:
    /// \code
    /// float x;
    /// file >> x;
    /// if (file)
    /// {
    ///    // ok, x was extracted successfully
    /// }
    ///
    /// // -- or --
    ///
    /// float x;
    /// if (file >> x)
    /// {
    ///    // ok, x was extracted successfully
    /// }
    /// \endcode
    ///
    /// \return True if last data extraction from file was successful
    ///
    /// \see IsEOF
    ///
    ////////////////////////////////////////////////////////////
	inline operator bool() const
	{
		return IsHealthy();
	}

	///////////////////////////////////////////////////////////
	/// \brief Read raw bytes
	///
	/// Provides Low-level access to file, possibly for custom
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
		KL_ASSERT((mMode & io::FileModes::Read) > 0);
		mStream.read(buffer, bytes);
		return IsHealthy();
	}

	///////////////////////////////////////////////////////////
	/// \brief Write raw bytes
	///
	/// Provides Low-level access to file, possibly for custom
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
		KL_ASSERT((mMode & io::FileModes::Write) > 0);
		mStream.write(data, bytes);
		return IsHealthy();
	}
	
};
///////////////////////////////////////////////////////////
/// \class BinaryFile
/// \brief File which can be used to read and write binary data.
/// \ingroup FileIO
///
/// Very versatile, includes multiple ways to transmit the data across.
///
/// \code
/// file.Read(buffer, bytes); // low-level access
/// BinaryStream stream(file);
/// stream << serializableObject; // high-level access
/// stream >> floatValue;
/// \endcode
///
/// \see FileBase, BinaryStream
///
///////////////////////////////////////////////////////////

} // io
} // klib

