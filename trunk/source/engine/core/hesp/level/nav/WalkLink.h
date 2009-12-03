/***
 * hesperus: WalkLink.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_WALKLINK
#define H_HESP_WALKLINK

#include <hesp/math/geom/LineSegment.h>
#include "NavLink.h"

namespace hesp {

class WalkLink : public NavLink
{
	//#################### PRIVATE VARIABLES ####################
private:
	LineSegment3d m_edge;

	//#################### CONSTRUCTORS ####################
public:
	WalkLink(int sourcePoly, int destPoly, const Vector3d& p1, const Vector3d& p2);
	WalkLink(int sourcePoly, int destPoly, const LineSegment3d& edge);

	//#################### PUBLIC METHODS ####################
public:
	Vector3d dest_position() const;
	boost::optional<Vector3d> hit_test(const Vector3d& s, const Vector3d& d) const;
	static NavLink_Ptr load(const std::string& data);
	void output(std::ostream& os) const;
	void render() const;
	Vector3d source_position() const;
	double traversal_time(double traversalSpeed) const;
	Vector3d traverse(const Vector3d& source, double t) const;
};

}

#endif
