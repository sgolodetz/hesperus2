/***
 * hesperus: PolyhedralBrush.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <hesp/math/geom/GeomUtil.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
template <typename Poly>
PolyhedralBrush<Poly>::PolyhedralBrush(const AABB3d& bounds, const PolyVector& faces, BrushFunction function)
:	m_bounds(bounds), m_faces(faces), m_function(function)
{}

//#################### PUBLIC METHODS ####################
template <typename Poly>
const AABB3d& PolyhedralBrush<Poly>::bounds() const
{
	return m_bounds;
}

template <typename Poly>
const typename PolyhedralBrush<Poly>::PolyVector&
PolyhedralBrush<Poly>::faces() const
{
	return m_faces;
}

template <typename Poly>
BrushFunction PolyhedralBrush<Poly>::function() const
{
	return m_function;
}

}
