#pragma once

//TODO Memory watchers

#include <memory>

template<typename T>
using StrongPtr = std::shared_ptr<T>;

template<typename T>
using WeakPtr = std::weak_ptr<T>;

/*
template<typename T>
using StaticCast = std::static_pointer_cast<T*>;

template<typename T>
using DynamicCast = std::dynamic_pointer_cast<T*>;*/

#if !defined(SAFE_DELETE)
#	define SAFE_DELETE(x) if(x) delete x; x=nullptr;
#endif

#if !defined(SAFE_DELETE_ARRAY)
#	define SAFE_DELETE_ARRAY(x) if (x) delete [] x; x=nullptr; 
#endif

#if !defined(SAFE_RELEASE)
#	define SAFE_RELEASE(x) if(x) x->Release(); x=nullptr;
#endif