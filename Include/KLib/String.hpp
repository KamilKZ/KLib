#pragma once

//TODO: Create a universal String class like sf::String
//TODO: UTF-8 support

#include <algorithm> // std::transform
#include <sstream> // stringstream
#include <stdlib.h> // atoi, atol, atof
#include <string>

#include <KLib/Config.hpp>
#include <KLib/ArrayList.hpp>
#include <KLib/Number.hpp>

namespace klib
{

typedef std::string String;

//#include <SFML/System/String.hpp>
//typedef sf::String StringSuper;

namespace string
{

inline API_EXPORT ArrayList<String> Split(const String& str, char delim)
{
	ArrayList<String> elems;
	std::stringstream ss(str);
	String item;

	while (std::getline(ss, item, delim))
	{
		elems.push_back(item);
	}
	return elems;
}

inline API_EXPORT String Lower(const String& in)
{
	String str(in);
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}

inline API_EXPORT String Upper(const String& in)
{
	String str(in);
	std::transform(str.begin(), str.end(), str.begin(), ::toupper);
	return str;
}

} // string
// klib...

// Templates have to be in headers :'<

// ToString

template<typename T>
String API_EXPORT ToString(const T& generic)
{
	std::ostringstream convert;
	convert << generic;
	return convert.str();
}

template<>
String API_EXPORT ToString<bool>(const bool& boolean);

template<>
inline API_EXPORT String ToString<bool>(const bool& boolean)
{
	return boolean ? "true" : "false";
}

// FromString

template<typename T>
T API_EXPORT FromString(const String& in);

template<>
inline API_EXPORT Int FromString<Int>(const String& in)
{
	return atoi(in.c_str());
}

template<>
inline API_EXPORT Long FromString<Long>(const String& in)
{
	return atol(in.c_str());
}

template<>
inline API_EXPORT Float FromString<Float>(const String& in)
{
	return atof(in.c_str());
}

template<>
inline API_EXPORT Double FromString<Double>(const String& in)
{
	return atof(in.c_str());
}

template<>
inline API_EXPORT bool FromString<bool>(const String& in)
{
	return ( string::Lower(in) == "true" ) ? true : false;
}

// To/FromArray

template<typename T>
inline API_EXPORT String ToStringArray(const ArrayList<T>& array)
{
	std::ostringstream ts;

	for (int i = 0; i < array.size(); i++)
	{
		ts << ToString<T>(array[i]);
		if (i + 1 != array.size())
			ts << ",";
	}

	return ts.str();
}

template<typename T>
inline API_EXPORT ArrayList<T> FromStringArray(const String& in)
{
	ArrayList<T> elems;
	ArrayList<String> split = string::Split(in, ','); // split string at commas
	for (String elem : split)
		elems.push_back(FromString<T>(elem)); // FromString every separated 'string'

	return elems;
}

// ToHexString

template<typename T>
inline API_EXPORT String ToHexString(const T& generic)
{
	std::ostringstream convert;
	convert << "0x" << std::hex << generic;
	return convert.str();
}

} // klib