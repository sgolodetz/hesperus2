/***
 * hesperus: Bounds.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_BOUNDS
#define H_HESP_BOUNDS

#include <utility>

#include <boost/optional.hpp>

#include <hesp/math/vectors/Vector3.h>

namespace hesp {

class Bounds
{
	//#################### DESTRUCTOR ####################
public:
	virtual ~Bounds() {}

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual double brush_expansion_distance(const Vector3d& n) const = 0;
	virtual boost::optional<std::pair<Vector3d,Vector3d> > determine_halfray_intersection(const Vector3d& s, const Vector3d& v) const = 0;
	virtual Vector3d half_dimensions() const = 0;
	virtual double height() const = 0;
	virtual void render(const Vector3d& pos) const = 0;
	virtual Vector3d support_point(const Vector3d& n) const = 0;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<Bounds> Bounds_Ptr;
typedef shared_ptr<const Bounds> Bounds_CPtr;

}

#endif
