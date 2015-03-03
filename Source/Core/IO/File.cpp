#include <iostream>

#include <KLib/ISerializable.hpp>
#include <KLib/ByteBuffer.hpp>
#include <KLib/File.hpp>
#include <KLib/Logging.hpp>

namespace klib
{
namespace io
{

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

////////////////////////////////////////////////////////////
/// FileBase
////////////////////////////////////////////////////////////

FileBase::~FileBase()
{
	if (IsOpen())
		Close();
}

bool FileBase::Open(String path, FileMode mode)
{
	std::ios::openmode mode_translated = priv::TranslateFileMode(mode);

	mStream.open(path, mode_translated);
	mOpen = mStream.is_open();
	mMode = mode;

	if (!mOpen)
		KL_WARNING("Failed to open file '" + path + "'");

	return mOpen;
}

bool FileBase::IsOpen() const
{
	return mOpen;
}

void FileBase::Close()
{
	if ((mMode & std::ios::out) > 0)
		Flush();

	mStream.close();
}

void FileBase::Flush()
{
	KL_ASSERT((mMode & std::ios::out) > 0);
	mStream.flush();
}

UInt FileBase::GetSize()
{
	KL_ASSERT((mMode & std::ios::in) > 0);
	Int pos = Tell(); // store current pos to restore
	Seek(0, std::ios::end); // go to end of file
	Int length = Tell(); // retrieve pos at end
	Seek(pos); // restore last position
	return length;
}

bool FileBase::IsHealthy() const
{
	return mStream.rdstate() == std::ios::goodbit;
}

bool FileBase::Seek(UInt pos, std::ios::seekdir way)
{
	mStream.seekg(pos, way);
	return IsHealthy();
}

bool FileBase::Skip(Int amount)
{
	mStream.seekg(amount, std::ios_base::cur);
	return IsHealthy();
}

UInt FileBase::Tell()
{
	return mStream.tellg();
}

bool FileBase::IsEndOfFile()
{
	KL_ASSERT((mMode & std::ios::in) > 0);
	return (mStream.rdstate() == std::ios::eofbit) ||
			(Tell() == (Int)(mStream.end));
}

////////////////////////////////////////////////////////////
/// TextFile
////////////////////////////////////////////////////////////

TextFile::TextFile(String path, FileMode mode)
{
	Open(path, mode);
}

String TextFile::Read(UInt size)
{
	KL_ASSERT((mMode & std::ios::in) > 0);
	char* data = new char[size];
	mStream.read(data, size);
	String strData(data);
	SAFE_DELETE_ARRAY(data);
	return strData;
}

String TextFile::ReadLine()
{
	KL_ASSERT((mMode & std::ios::in) > 0);
	String str;
	std::getline(mStream, str);
	return str;
}

String TextFile::ReadAll() // Thanks http://stackoverflow.com/a/2602060
{
	KL_ASSERT((mMode & std::ios::in) > 0);
	String str;
	str.reserve(GetSize());
	str.assign((std::istreambuf_iterator<char>(mStream)),
				std::istreambuf_iterator<char>());

	return str;
}

bool TextFile::Write(const String& data)
{
	KL_ASSERT((mMode & std::ios::out) > 0);
	mStream << data;
	return IsHealthy();
}

////////////////////////////////////////////////////////////
/// BinaryFile
////////////////////////////////////////////////////////////

bool BinaryFile::Open(String path, FileMode mode)
{
	return FileBase::Open(path, ( mode | FileModes::Binary ));
}

BinaryFile::operator bool() const
{
	return IsHealthy();
}

// R/W Bytes
bool BinaryFile::Read(char* buffer, UInt bytes) // Read Raw Bytes
{
	KL_ASSERT((mMode & std::ios::in) > 0);
	mStream.read(buffer, bytes);
	return IsHealthy();
}

bool BinaryFile::Write(const char* data, UInt bytes) // Write Raw Bytes
{
	KL_ASSERT((mMode & std::ios::out) > 0);
	mStream.write(data, bytes);
	return IsHealthy();
}

} // io
} // klib

