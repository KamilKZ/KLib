#pragma once

#if defined(_MSC_VER)
	
	#define API_EXPORT __declspec(dllexport)
	#define API_IMPORT __declspec(dllimport)
	
#elif defined(__GNUC__)

	#if __GNUC__ > 4
	
		#define API_EXPORT __attribute__((visibility("default")))
		#define API_IMPORT __attribute__((visibility("default")))
		
	#else
	
		#define API_EXPORT
		#define API_IMPORT
		
	#endif
	
#else

	#error("Don't know how to export shared object libraries")
	
#endif