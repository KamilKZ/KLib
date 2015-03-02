#pragma once

#ifdef _WIN32
#	define WIN32_LEAN_AND_MEAN
#	define NOMINMAX
#endif

// Debugable overloaded new operator

#if defined(_DEBUG)
#	define KL_NEW new(_NORMAL_BLOCK,__FILE__, __LINE__)
#else
#	define KL_NEW new
#endif

// Useful #defines

#ifdef UNICODE
#	define _tcssprintf wsprintf
#	define tcsplitpath _wsplitpath
#else
#	define _tcssprintf sprintf
#	define tcsplitpath _splitpath
#endif