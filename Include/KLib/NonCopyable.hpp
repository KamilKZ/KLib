#pragma once

#include <KLib/Exports.hpp>

namespace klib
{
//namespace utilities
//{

class API_EXPORT NonCopyable
{
private:
	NonCopyable(const NonCopyable& x);
	NonCopyable& operator=(const NonCopyable& x);
public:
	NonCopyable() {}; // Default constructor  
};

//} // utilities
} // klib