/***
 * asx: ASXStructorTypeString.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_ASX_ASXSTRUCTORTYPESTRING
#define H_ASX_ASXSTRUCTORTYPESTRING

#include "ASXTypeString.h"

//~~~~~~~~~~~~~~~~~
// Default template
//~~~~~~~~~~~~~~~~~

template <typename F> struct ASXStructorTypeString;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Specializations for functions with different parameter counts
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// No real structor arguments (only the pointer to the memory for the object)
template <typename T> struct ASXStructorTypeString<void (T*)>
{
	std::string function_type()
	{
		return ASXTypeString<void ()>("f")();
	}

	std::string object_type()
	{
		return ASXTypeString<T>()();
	}
};

// 1 real structor argument
template <typename T, typename Arg0> struct ASXStructorTypeString<void (T*,Arg0)>
{
	std::string function_type()
	{
		return ASXTypeString<void (Arg0)>("f")();
	}

	std::string object_type()
	{
		return ASXTypeString<T>()();
	}
};

// 2 real structor arguments
template <typename T, typename Arg0, typename Arg1> struct ASXStructorTypeString<void (T*,Arg0,Arg1)>
{
	std::string function_type()
	{
		return ASXTypeString<void (Arg0,Arg1)>("f")();
	}

	std::string object_type()
	{
		return ASXTypeString<T>()();
	}
};

// TODO: Structors with more arguments

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Specializations for pointers to functions with different parameter counts
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// No real structor arguments (only the pointer to the memory for the object)
template <typename T> struct ASXStructorTypeString<void (*)(T*)>
:	ASXStructorTypeString<void (T*)> {};

// 1 real structor argument
template <typename T, typename Arg0> struct ASXStructorTypeString<void (*)(T*,Arg0)>
:	ASXStructorTypeString<void (T*,Arg0)> {};

// 2 real structor arguments
template <typename T, typename Arg0, typename Arg1> struct ASXStructorTypeString<void (*)(T*,Arg0,Arg1)>
:	ASXStructorTypeString<void (T*,Arg0,Arg1)> {};

// TODO: Pointers to structors with more arguments

#endif
