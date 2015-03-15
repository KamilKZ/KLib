#pragma once

#if defined(_MSC_VER)

	#if _MSC_VER < 1700

		#error("This version of Visual Studio is not supported, please use Visual Studio 2012 or above")

	#endif

	#define API_EXPORT __declspec(dllexport)
	#define API_IMPORT __declspec(dllimport)

#elif defined(__GNUG__)

	#if __GNUG__ > 4

		#if GNUC_MINOR__ > 5

			#define API_EXPORT __attribute__((visibility("default")))
			#define API_IMPORT __attribute__((visibility("default")))

		#else

			#error("This version of g++ is unsupported, please use g++ 4.6 or above")

		#endif

	#endif

#else

	#error("Unsupported compiler")

#endif