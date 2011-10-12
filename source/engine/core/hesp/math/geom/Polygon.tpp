/***
 * hesperus: Polygon.tpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include <hesp/exceptions/InvalidParameterException.h>

#define Polygon_HEADER	template <typename Vert, typename AuxData>
#define Polygon_THIS	Polygon<Vert,AuxData>

namespace hesp {

//#################### CONSTRUCTORS ####################
/**
Constructs a polygon from the specified vertices and auxiliary data.

- |vertices| >= 3

@param vertices						The vertices
@param auxData						The auxiliary data
@throws InvalidParameterException	If the precondition is violated
*/
Polygon_HEADER
Polygon_THIS::Polygon(const std::vector<Vert>& vertices, const AuxData& auxData)
:	m_vertices(vertices), m_auxData(auxData)
{
	if(vertices.size() < 3) throw InvalidParameterException("A polygon must have at least 3 vertices");
	calculate_normal();
}

/**
Constructs a polygon from another polygon with the same vertex type.

@param otherPoly	The other polygon
@param auxData		Auxiliary data for the new polygon
*/
Polygon_HEADER
template <typename OtherAuxData>
Polygon_THIS::Polygon(const Polygon<Vert,OtherAuxData>& otherPoly, const AuxData& auxData)
:	m_vertices(otherPoly.m_vertices), m_normal(otherPoly.m_normal), m_auxData(auxData)
{}

//#################### PUBLIC METHODS ####################
/**
Returns the auxiliary data stored with the polygon.

@return	As stated
*/
Polygon_HEADER
AuxData& Polygon_THIS::auxiliary_data()
{
	return m_auxData;
}

/**
Returns the auxiliary data stored with the polygon.

@return	As stated
*/
Polygon_HEADER
const AuxData& Polygon_THIS::auxiliary_data() const
{
	return m_auxData;
}

/**
Returns a copy of this polygon with flipped winding order.

@return	As stated
*/
Polygon_HEADER
typename Polygon_THIS::Polygon_Ptr Polygon_THIS::flipped_winding() const
{
	std::vector<Vert> flippedVertices;
	flippedVertices.reserve(m_vertices.size());
	std::copy(m_vertices.rbegin(), m_vertices.rend(), std::back_inserter(flippedVertices));
	return Polygon_Ptr(new Polygon(flippedVertices, m_auxData));
}

/**
Returns the polygon normal.

@return	As stated
*/
Polygon_HEADER
const Vector3d& Polygon_THIS::normal() const
{
	return m_normal;
}

/**
Returns the i'th vertex of the polygon.

@return	As stated
*/
Polygon_HEADER
const Vert& Polygon_THIS::vertex(int i) const
{
	return m_vertices[i];
}

/**
Returns the number of vertices in the polygon.

@return	As stated
*/
Polygon_HEADER
int Polygon_THIS::vertex_count() const
{
	return static_cast<int>(m_vertices.size());
}

//#################### PRIVATE METHODS ####################
Polygon_HEADER
void Polygon_THIS::calculate_normal()
{
	Vector3d v1 = m_vertices[1], v2 = m_vertices[2];
	v1 -= m_vertices[0];
	v2 -= m_vertices[0];
	m_normal = v1.cross(v2).normalize();
}

}

#undef Polygon_THIS
#undef Polygon_HEADER
