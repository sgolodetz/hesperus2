/***
 * hesperus: Interval.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "Interval.h"

#include <algorithm>
#include <ostream>

#include <hesp/exceptions/Exception.h>
#include "Constants.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
/**
Constructs an Interval representing [low,high].

Preconditions:

- high > low + EPSILON

@param low		The low end of the interval
@param high		The high end of the interval
*/
Interval::Interval(double low, double high)
:	m_low(low), m_high(high)
{
	m_empty = m_high - m_low < EPSILON;
}

/**
Constructs an empty Interval.
*/
Interval::Interval()
:	m_empty(true)
{}

//#################### PUBLIC METHODS ####################
bool Interval::empty() const	{ return m_empty; }

double Interval::high() const
{
	if(!m_empty) return m_high;
	else throw Exception("The interval is empty and thus has no high end");
}

/**
Calculates the intersection of this interval with another.

@param rhs	The other interval
@return		The intersection of the two intervals
*/
Interval Interval::intersect(const Interval& rhs) const
{
	if(empty() || rhs.empty()) return Interval();

	double largerLow = std::max(m_low, rhs.m_low);
	double smallerHigh = std::min(m_high, rhs.m_high);
	return Interval(largerLow, smallerHigh);
}

double Interval::low() const
{
	if(!m_empty) return m_low;
	else throw Exception("The interval is empty and thus has no low end");
}

//#################### GLOBAL OPERATORS ####################
std::ostream& operator<<(std::ostream& os, const Interval& rhs)
{
	if(rhs.empty()) os << "[]";
	else os << '[' << rhs.low() << ',' << rhs.high() << ']';
	return os;
}

}
