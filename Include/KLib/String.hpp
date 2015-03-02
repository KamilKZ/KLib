#pragma once

//TODO: Create a universal String class like sf::String
//TODO: UTF-8 support

#include <KLib/Exports.hpp>
#include <KLib/ArrayList.hpp>
#include <KLib/Number.hpp>

#include <string>

namespace klib
{

typedef std::string String;

//#include <SFML/System/String.hpp>
//typedef sf::String StringSuper;

namespace string
{

ArrayList<String> API_EXPORT Split(const String& str, char delim);

String API_EXPORT Lower(const String& in);
String API_EXPORT Upper(const String& in);

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

// FromString

template<typename T>
T API_EXPORT FromString(const String& in);

template<> Int		API_EXPORT FromString<Int>(const String& in);
template<> Long		API_EXPORT FromString<Long>(const String& in);
template<> Float	API_EXPORT FromString<Float>(const String& in);
template<> Double	API_EXPORT FromString<Double>(const String& in);
template<> bool		API_EXPORT FromString<bool>(const String& in);

template<typename T>
String API_EXPORT ToStringArray(const ArrayList<T>& array)
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
ArrayList<T> API_EXPORT FromStringArray(const String& in)
{
	ArrayList<T> elems;
	ArrayList<String> split = string::Split(in, ','); // split string at commas
	for (String elem : split)
		elems.push_back(FromString<T>(elem)); // FromString every separated 'string'

	return elems;
}

template<typename T>
String API_EXPORT ToHexString(const T& generic)
{
	std::ostringstream convert;
	convert << "0x" << std::hex << generic;
	return convert.str();
}

} // klib