/***
 * hesperus: Polygon.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_POLYGON
#define H_HESP_POLYGON

#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <hesp/math/vectors/Vector3.h>

namespace hesp {

/**
This class template allows users to create convex, planar polygons with a vertex type and
auxiliary data type of their choosing. For instance, they can create textured polygons by
specifying a vertex type with (u,v) texture coordinates and an auxiliary data type referencing
the texture itself.
*/
template <typename Vert, typename AuxData>
class Polygon
{
	//#################### FRIENDS ####################
	friend class Polygon;

	//#################### TYPEDEFS ####################
public:
	// Expose the template arguments so that they can easily be used by client code.
	typedef Vert Vert;
	typedef AuxData AuxData;

	typedef shared_ptr<Polygon> Polygon_Ptr;

	//#################### PRIVATE VARIABLES ####################
private:
	std::vector<Vert> m_vertices;
	Vector3d m_normal;
	AuxData m_auxData;

	//#################### CONSTRUCTORS ####################
public:
	Polygon(const std::vector<Vert>& vertices, const AuxData& auxData);
	template <typename OtherAuxData> Polygon(const Polygon<Vert,OtherAuxData>& otherPoly, const AuxData& auxData);

	//#################### PUBLIC METHODS ####################
public:
	AuxData& auxiliary_data();
	const AuxData& auxiliary_data() const;
	Polygon_Ptr flipped_winding() const;
	const Vector3d& normal() const;
	const Vert& vertex(int i) const;
	int vertex_count() const;

	//#################### PRIVATE METHODS ####################
private:
	void calculate_normal();
};

}

#include "Polygon.tpp"

#endif
