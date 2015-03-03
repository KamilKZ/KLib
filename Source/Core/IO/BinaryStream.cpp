#include <KLib/ISerializable.hpp>
#include <KLib/ByteBuffer.hpp> // ByteBuffer, buffering for String
#include <KLib/File.hpp>
#include <KLib/BinaryStream.hpp>

namespace klib
{
namespace io
{

BinaryStream::BinaryStream()
{
	KL_ERROR("BinaryStream initialized wrong");
}

BinaryStream::BinaryStream(std::iostream* baseStream)
{
	mStream = baseStream;
	mMode = (std::ios::in | std::ios::out);
}

BinaryStream::BinaryStream(BinaryFile& file)
{
	mStream = &file.GetStream();
	mMode = file.GetMode();
}
	
BinaryStream::~BinaryStream() {}

bool BinaryStream::IsHealthy() const
{
	return mStream->rdstate() == std::ios::goodbit;
}

bool BinaryStream::Seek(UInt pos, std::ios::seekdir way)
{
	mStream->seekg(pos, way);
	return IsHealthy();
}

bool BinaryStream::Skip(Int amount)
{
	mStream->seekg(amount, std::ios_base::cur);
	return IsHealthy();
}

UInt BinaryStream::Tell() const
{
	return mStream->tellg();
}

bool BinaryStream::IsEnd() const
{
	KL_ASSERT((mMode & std::ios::in) > 0);
	return (mStream->rdstate() == std::ios::eofbit) ||
			(Tell() == (Int)(mStream->end));
}

BinaryStream::operator bool() const
{
	return IsHealthy();
}

// R/W Bytes
bool BinaryStream::Read(char* buffer, UInt bytes) // Read Raw Bytes
{
	KL_ASSERT((mMode & std::ios::in) > 0);
	mStream->read(buffer, bytes);
	return IsHealthy();
}

bool BinaryStream::Write(const char* data, UInt bytes) // Write Raw Bytes
{
	KL_ASSERT((mMode & std::ios::out) > 0);
	mStream->write(data, bytes);
	return IsHealthy();
}

BinaryStream& BinaryStream::operator>>(char* data)
{
	// Make sure we can read
	KL_ASSERT((mMode & std::ios::in) > 0);
	
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

template<>
BinaryStream& BinaryStream::operator>>(String& data)
{
	// Make sure we can read
	KL_ASSERT((mMode & std::ios::in) > 0);
	
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

template<>
BinaryStream& BinaryStream::operator>>(ByteBuffer& data)
{
	// Make sure we can write
	KL_ASSERT((mMode & std::ios::out) > 0);

	// First extract data length
	UInt length;
	*this >> length;

	data = ByteBuffer(length);

	// Then insert data
	if (length > 0)
		Read(data, length);

	return *this;
}

BinaryStream& BinaryStream::operator<<(const char* data)
{
	// Make sure we can write
	KL_ASSERT((mMode & std::ios::out) > 0);
	
	// First insert string length
	UInt length = std::strlen(data);
	*this << length;
	
	// Then insert characters
	if (length > 0)
		Write(data, length * sizeof(char));
	
	return *this;
}

template<>
BinaryStream& BinaryStream::operator<<(const String& data)
{
	// Make sure we can write
	KL_ASSERT((mMode & std::ios::out) > 0);

	// First insert string length
	UInt length = static_cast<UInt>(data.size());
	*this << length;

	// Then insert characters
	if (length > 0)
		Write(data.c_str(), length * sizeof(String::value_type));

	return *this;
}

template<>
BinaryStream& BinaryStream::operator<<(const ByteBuffer& data)
{
	// Make sure we can write
	KL_ASSERT((mMode & std::ios::out) > 0);

	// First insert data length
	UInt length = static_cast<UInt>(data.GetSize());
	*this << length;

	// Then insert data
	if (length > 0)
		Write(data, length);

	return *this;
}

} // io
} // klib