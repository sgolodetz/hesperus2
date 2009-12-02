/***
 * hesperus: SphereBounds.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_SPHEREBOUNDS
#define H_HESP_SPHEREBOUNDS

#include "Bounds.h"

namespace hesp {

class SphereBounds : public Bounds
{
	//#################### PRIVATE VARIABLES ####################
private:
	double m_radius;

	//#################### CONSTRUCTORS ####################
public:
	explicit SphereBounds(double radius);

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
