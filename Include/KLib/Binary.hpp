#pragma once

#include <KLib/Exports.hpp>
#include <KLib/Number.hpp>

namespace klib
{

template<typename T = Int>
const char* API_EXPORT ToBytes(const T& in)
{
	const char* bytes = reinterpret_cast<const char*>( &in );
	return bytes;
}

template<typename T = Int>
T API_EXPORT FromBytes(const char* bytes)
{
	T out;
	
	memcpy(&out, bytes, sizeof(T));

	return out;
}

} // klib