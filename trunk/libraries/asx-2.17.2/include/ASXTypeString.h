/***
 * asx: ASXTypeString.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_ASX_ASXTYPESTRING
#define H_ASX_ASXTYPESTRING

#include <sstream>
#include <string>

//~~~~~~~~
// Helpers
//~~~~~~~~

struct ASXSimpleTypeString
{
	std::string name, prefix, suffix;

	explicit ASXSimpleTypeString(const std::string& name_) : name(name_) {}

	std::string operator()()
	{
		std::ostringstream os;
		if(prefix != "") os << prefix << ' ';
		os << type();
		if(suffix != "") os << ' ' << suffix;
		if(name != "") os << ' ' << name;
		return os.str();
	}

	virtual std::string type() = 0;

	ASXSimpleTypeString& as_param()
	{
		return *this;
	}
};

//~~~~~~~~~~~~~~~~~
// Default template
//~~~~~~~~~~~~~~~~~

template <typename T> struct ASXTypeString : ASXSimpleTypeString
{
	explicit ASXTypeString(const std::string& name_ = "") : ASXSimpleTypeString(name_) {}
	std::string type() { return T::type_string(); }
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Specializations for simple types
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template <> struct ASXTypeString<bool> : ASXSimpleTypeString
{
	explicit ASXTypeString(const std::string& name_ = "") : ASXSimpleTypeString(name_) {}
	std::string type() { return "bool"; }
};

template <> struct ASXTypeString<double> : ASXSimpleTypeString
{
	explicit ASXTypeString(const std::string& name_ = "") : ASXSimpleTypeString(name_) {}
	std::string type() { return "double"; }
};

template <> struct ASXTypeString<float> : ASXSimpleTypeString
{
	explicit ASXTypeString(const std::string& name_ = "") : ASXSimpleTypeString(name_) {}
	std::string type() { return "float"; }
};

template <> struct ASXTypeString<int> : ASXSimpleTypeString
{
	explicit ASXTypeString(const std::string& name_ = "") : ASXSimpleTypeString(name_) {}
	std::string type() { return "int"; }
};

template <> struct ASXTypeString<std::string> : ASXSimpleTypeString
{
	explicit ASXTypeString(const std::string& name_ = "") : ASXSimpleTypeString(name_) {}
	std::string type() { return "string"; }
};

template <> struct ASXTypeString<void> : ASXSimpleTypeString
{
	explicit ASXTypeString(const std::string& name_ = "") : ASXSimpleTypeString(name_) {}
	std::string type() { return "void"; }
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Specialization for const types
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template <typename T> struct ASXTypeString<const T> : ASXTypeString<T>
{
	explicit ASXTypeString(const std::string& name_ = "")
	:	ASXTypeString<T>(name_)
	{
		prefix = "const ";
	}

	ASXTypeString& as_param()
	{
		return *this;
	}
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Specializations for pointer and reference types
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

template <typename T> struct ASXTypeString<T*> : ASXTypeString<T>
{
	explicit ASXTypeString(const std::string& name_ = "")
	:	ASXTypeString<T>(name_)
	{
		suffix = "@";
	}
};

template <typename T> struct ASXTypeString<T&> : ASXTypeString<T>
{
	explicit ASXTypeString(const std::string& name_ = "")
	:	ASXTypeString<T>(name_)
	{
		suffix = "&";
	}

	ASXTypeString& as_param()
	{
		suffix = "& out";
		return *this;
	}
};

template <typename T> struct ASXTypeString<const T&> : ASXTypeString<T>
{
	explicit ASXTypeString(const std::string& name_ = "")
	:	ASXTypeString<T>(name_)
	{
		prefix = "const";
		suffix = "&";
	}

	ASXTypeString& as_param()
	{
		suffix = "& in";
		return *this;
	}
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Specializations for function types
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// No arguments
template <typename R> struct ASXTypeString<R ()>
{
	std::string name;

	explicit ASXTypeString(const std::string& name_) : name(name_) {}

	std::string operator()()
	{
		std::ostringstream os;
		os << ASXTypeString<R>()() << ' ' << name << "()";
		return os.str();
	}
};

// 1 argument
template <typename R, typename Arg0> struct ASXTypeString<R (Arg0)>
{
	std::string name;

	explicit ASXTypeString(const std::string& name_) : name(name_) {}

	std::string operator()()
	{
		std::ostringstream os;
		os << ASXTypeString<R>()() << ' ' << name << '(';
		os << ASXTypeString<Arg0>().as_param()();
		os << ')';
		return os.str();
	}
};

// 2 arguments
template <typename R, typename Arg0, typename Arg1> struct ASXTypeString<R (Arg0,Arg1)>
{
	std::string name;

	explicit ASXTypeString(const std::string& name_) : name(name_) {}

	std::string operator()()
	{
		std::ostringstream os;
		os << ASXTypeString<R>()() << ' ' << name << '(';
		os << ASXTypeString<Arg0>().as_param()() << ',';
		os << ASXTypeString<Arg1>().as_param()();
		os << ')';
		return os.str();
	}
};

// 3 arguments
template <typename R, typename Arg0, typename Arg1, typename Arg2> struct ASXTypeString<R (Arg0,Arg1,Arg2)>
{
	std::string name;

	explicit ASXTypeString(const std::string& name_) : name(name_) {}

	std::string operator()()
	{
		std::ostringstream os;
		os << ASXTypeString<R>()() << ' ' << name << '(';
		os << ASXTypeString<Arg0>().as_param()() << ',';
		os << ASXTypeString<Arg1>().as_param()() << ',';
		os << ASXTypeString<Arg2>().as_param()();
		os << ')';
		return os.str();
	}
};

// TODO: Functions with more arguments

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Specializations for function pointer types
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// No arguments
template <typename R> struct ASXTypeString<R (*)()>
:	ASXTypeString<R ()>
{
	explicit ASXTypeString(const std::string& name_) : ASXTypeString<R ()>(name_) {}
};

// 1 argument
template <typename R, typename Arg0> struct ASXTypeString<R (*)(Arg0)>
:	ASXTypeString<R (Arg0)>
{
	explicit ASXTypeString(const std::string& name_) : ASXTypeString<R (Arg0)>(name_) {}
};

// 2 arguments
template <typename R, typename Arg0, typename Arg1> struct ASXTypeString<R (*)(Arg0,Arg1)>
:	ASXTypeString<R (Arg0,Arg1)>
{
	explicit ASXTypeString(const std::string& name_) : ASXTypeString<R (Arg0,Arg1)>(name_) {}
};

// 3 arguments
template <typename R, typename Arg0, typename Arg1, typename Arg2> struct ASXTypeString<R (*)(Arg0,Arg1,Arg2)>
:	ASXTypeString<R (Arg0,Arg1,Arg2)>
{
	explicit ASXTypeString(const std::string& name_) : ASXTypeString<R (Arg0,Arg1,Arg2)>(name_) {}
};

// TODO: Pointers to functions with more arguments

#endif
