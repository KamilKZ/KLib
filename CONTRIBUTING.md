<hidden><title>KLib Code Conventions</title></hidden>
# KLib #
## Code Conventions ##

1. ###Intro###
	* Don't use abbreviations in names <br/>
	For example: `worldPos` should be written as `worldPosition`
	
	* Prefer forward-declaration over including files <br/>
	Only use includes when neccessary <br/>
	```cpp
	class Foo;
	class Bar
	{
		Foo* _foo; //Doesn't do anything with _foo
	}
	```
	```cpp
	#include "Foo.hpp"
	class Bar
	{
		Foo* _foo;
		
		void DoSomething() { _foo->DoSomething() };
	}
	```
2. ###Cases###
	1. **Functions**: *CamelCaps* <br/>
	`Object::GetCircle()`
	
	2. **Variables**: *camelCase* <br/>
	`Float circleRadius`
	
	3. **Class Names**: *CamelCaps* <br/>
	`class Circle : IRenderer`
	
	4. **Member Variables**: *mCamelCaps* <br/>
	`Float mRadius` <br/>
	mLots mOf mVariables mIs mStill mEasier mTo mRead mThan
	m_lots m_of m_variables m_and m_easier m_to m_type
	
	5. **Global Variables**: *g_camelCase* <br/>
	`Object g_object`
	
	6. **Pointer Variables**: *pCamelCaps* <br/>
	`Object pObject()` <br/>
	
	7. **Macros**: *UPPER_CASE* (has exceptions) <br/>
	`#define SOME_MACRO_DEFINITION`
	
	8. **Namespaces**: *lower_case* <br/>
	`namespace titanium_eagle`
	
	9. **Enum Types**: *CamelCaps* <br/>
	`enum EnumType`

	10. **Enum Values**: *UPPER_CASE* <br/>
	`enum EnumType {TYPE_UPPER, TYPE_LOWER, TYPE_MIXED}`
	
	11. **Compound cases ( global pointer )** <br/>
	`Object* g_pGlobalPointerVariable`
	
	11. **Compound cases ( pointer member )** <br/>
	`Object* mpGlobalPointerVariable`
3. ###Classes###
	1. **Interfaces**
		1. Interface classes use `I` prefix <br/>
		`class IRenderer`
		2. Virtual methods should look like this <br/>
		`virtual void SomeMethod()=0 override;`
		3. Must use override keyword on implementations
	2. **Definition order**
		1. Public first, protected second, private last, unless otherwise is required
		2. Constructor and destructor at the top, follwed by non-virtual methods, enums, virtual methods and finally variables
	3. **Member variables**
		Member variables are prefixed by a lower-case 'm' and their [case is CamelCaps](#2.4)
4. ###Files and Includes###
	1. Header files use `.hpp` extension
	
	2. Java-like, single (public) class per file and name files after the class they contain. <br/>
	Ex: `EventManager` class in `EventManager.hpp` and `EventManager.cpp`
	
	3. Include order is
		1. Standard libraries and dependencies, ex: thread.h
		2. Common includes, ex: Common/Types/Number.hpp
		3. Declaration pair header, ex: MyClass.hpp
		
		```cpp
		#include <thread.h>
		#include <glfw/glfw.h>
		
		#include <Common/Types/Number.hpp>
		
		#include "MyClass.hpp"
		```
		
5. ###Braces###
	1. There are 2 bracing styles used:
		```cpp
		// Single line
		int f() { return 3; }
		```
		```cpp
		// New line
		int X::f()
		{
		   return 3;
		}
		```
	
		The first style is used for short, trivial, single-line functions, like getter and setter functions in the header files.
		
		The second is used all around because it produces the most readable code.

	2. The only time a nested scope may be used without braces is for short if statements.
	```cpp
	if (bTrue)
		return true;
	else
		return false;
	```

6. ###Operators and const-correctness###
	1. Place (De-)Reference operators on types
	```cpp
	Pointer* pPointer;
	void Function(Reference& reference);
	```
	2. Use `const` **ALWAYS** where neccessary
	```cpp
	void Function(const Reference& reference);
	```
	
7. ###Comments###
	1. **Use single-line for descriptive comments and multi-line comments for disabling code**
	
	Single-line comments are better for comments that you want to leave in the code, because they don't have any nesting problems, it is easy to see what is commented, etc.

	Multi-line is useful when you want to quickly disable a piece of code.
	
	2. **Use doxygen**
	
	Like this:
	
	```cpp
	/////////////////////////////////////
	/// \brief Some function
	///
	/// Long description
	///
	/// \param one 1st paramater
	/// \return Nothing
	///
	/////////////////////////////////////
	```

8. ###Miscellaneous###
	1. Use `#pragma once` instead of header include guards