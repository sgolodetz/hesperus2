/***
 * hesperus: Interval.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_INTERVAL
#define H_HESP_INTERVAL

#include <iosfwd>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

class Interval
{
	//#################### PRIVATE VARIABLES ####################
private:
	bool m_empty;
	double m_low, m_high;

	//#################### CONSTRUCTORS ####################
public:
	Interval(double low, double high);
private:
	Interval();

	//#################### PUBLIC METHODS ####################
public:
	bool empty() const;
	double high() const;
	Interval intersect(const Interval& rhs) const;
	double low() const;
};

//#################### GLOBAL OPERATORS ####################
std::ostream& operator<<(std::ostream& os, const Interval& rhs);

//#################### TYPEDEFS ####################
typedef shared_ptr<Interval> Interval_Ptr;

}

#endif
