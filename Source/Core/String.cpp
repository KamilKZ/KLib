#include <algorithm> // std::transform
#include <stdlib.h> // atoi, atol, atof
#include <sstream>

#include <KLib/String.hpp>

namespace klib
{
namespace string
{

ArrayList<String> Split(const String& str, char delim)
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

String Lower(const String& in)
{
	String str(in);
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}

String Upper(const String& in)
{
	String str(in);
	std::transform(str.begin(), str.end(), str.begin(), ::toupper);
	return str;
}

} // string

// klib

template<>
String ToString<bool>(const bool& boolean)
{
	return boolean ? "true" : "false";
}

template<>
Int FromString<Int>(const String& in)
{
	return atoi(in.c_str());
}

template<>
Long FromString<Long>(const String& in)
{
	return atol(in.c_str());
}

template<>
Float FromString<Float>(const String& in)
{
	return atof(in.c_str());
}

template<>
Double FromString<Double>(const String& in)
{
	return atof(in.c_str());
}

template<>
bool FromString<bool>(const String& in)
{
	return ( string::Lower(in) == "true" ) ? true : false;
}

} // klib
