/***
 * asx: ASXVariable.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_ASX_ASXVARIABLE
#define H_ASX_ASXVARIABLE

template <typename T>
class ASXVariable
{
	//#################### PRIVATE VARIABLES ####################
private:
	T *m_p;

	//#################### CONSTRUCTORS ####################
public:
	explicit ASXVariable(T *p) : m_p(p) {}

	//#################### PUBLIC OPERATORS ####################
public:
	ASXVariable& operator=(const T& t)
	{
		*m_p = t;
		return *this;
	}

	operator T&()
	{
		return *m_p;
	}

	operator const T&() const
	{
		return *m_p;
	}
};

#endif
