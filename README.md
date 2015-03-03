KLib
====

A fairly small utility library which aims to provide useful classes and functions for a range of programs, though it is more geared towards game development.

Makes extensive use of C++11 and templates to provide clear and readable code.

KLib Core
---------

The core library is the base of the other libraries and has standard types, logging, and basic time and file io functions.

Example with reading files:
```cpp
#include <KLib/File.hpp>

using namespace klib;

// Reads all the lines in the file and adds the number values together
Int getResult(String filename)
{
	io::TextFile file(filename, std::ios::in); //open 'filename' in read mode
	
	KL_ASSERT(file.IsOpen()); // error if not open
	
	Int count = 0;
	while(!file.IsEndOfFile())
	{
		String line = file.ReadLine();
		
		Int value = FromString<Int>(line);
		
		count += value;
	}
	
	file.Close(); //Automatically closes when the object loses scope, but it's always better to make sure
	
	return count;
}
```