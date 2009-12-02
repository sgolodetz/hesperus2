/***
 * hesperus: PointBounds.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_POINTBOUNDS
#define H_HESP_POINTBOUNDS

#include "Bounds.h"

namespace hesp {

class PointBounds : public Bounds
{
	//#################### PUBLIC METHODS ####################
public:
	double brush_expansion_distance(const Vector3d&) const;
	boost::optional<std::pair<Vector3d,Vector3d> > determine_halfray_intersection(const Vector3d& s, const Vector3d& v) const;
	Vector3d half_dimensions() const;
	double height() const;
	void render(const Vector3d& pos) const;
	Vector3d support_point(const Vector3d& n) const;
};

}

#endif
