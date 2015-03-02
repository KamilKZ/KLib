#pragma once

#include <KLib/Exports.hpp>

namespace te
{

template <typename T>
class API_EXPORT Range
{
public:
	Range(const T* collection, const size_t size) :
		mCollection(collection), mSize(size)
	{
	}

	T* begin() const { return &mCollection[0]; }
	T* end() const { return &mCollection[mSize]; }

private:
	T* mCollection;
	size_t mSize;
};

} // te