/***
 * hesperus: StepLink.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_STEPLINK
#define H_HESP_STEPLINK

#include <hesp/math/geom/LineSegment.h>
#include "NavLink.h"

namespace hesp {

class StepLink : public NavLink
{
	//#################### PROTECTED VARIABLES ####################
protected:
	LineSegment3d m_sourceEdge, m_destEdge;

	//#################### CONSTRUCTORS ####################
public:
	StepLink(int sourcePoly, int destPoly, const Vector3d& s1, const Vector3d& s2, const Vector3d& d1, const Vector3d& d2);
	StepLink(int sourcePoly, int destPoly, const LineSegment3d& sourceEdge, const LineSegment3d& destEdge);

	//#################### PRIVATE ABSTRACT METHODS ####################
private:
	virtual std::string link_name() const = 0;

	//#################### PUBLIC METHODS ####################
public:
	Vector3d dest_position() const;
	boost::optional<Vector3d> hit_test(const Vector3d& s, const Vector3d& d) const;
	void output(std::ostream& os) const;
	Vector3d source_position() const;
	double traversal_time(double traversalSpeed) const;
	Vector3d traverse(const Vector3d& source, double t) const;
};

}

#endif
