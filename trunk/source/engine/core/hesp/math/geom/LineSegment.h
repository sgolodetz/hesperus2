/***
 * hesperus: LineSegment.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_LINESEGMENT
#define H_HESP_LINESEGMENT

#include <ostream>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <hesp/math/vectors/Vector2d.h>
#include <hesp/math/vectors/Vector3.h>

namespace hesp {

template <typename Vec>
struct LineSegment
{
	//#################### PUBLIC VARIABLES ####################
	Vec e1, e2;		// endpoints

	//#################### CONSTRUCTORS ####################
	LineSegment() {}

	LineSegment(const Vec& e1_, const Vec& e2_)
	:	e1(e1_), e2(e2_)
	{}
};

//#################### GLOBAL OPERATORS ####################
template <typename Vec>
std::ostream& operator<<(std::ostream& os, const LineSegment<Vec>& rhs)
{
	os << "[ " << rhs.e1 << ' ' << rhs.e2 << " ]";
	return os;
}

template <typename Vec>
std::istream& operator>>(std::istream& is, LineSegment<Vec>& rhs)
{
	std::ios_base::fmtflags oldFlags = is.flags();
	is.setf(std::ios_base::skipws);

	std::string dummy;
	is >> dummy >> rhs.e1 >> rhs.e2 >> dummy;

	is.flags(oldFlags);

	return is;
}

//#################### TYPEDEFS ####################
typedef LineSegment<Vector2d> LineSegment2d;
typedef LineSegment<Vector3d> LineSegment3d;

typedef shared_ptr<LineSegment2d> LineSegment2d_Ptr;
typedef shared_ptr<LineSegment3d> LineSegment3d_Ptr;

}

#endif
