/***
 * hesperus: LightmapGrid.tpp
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#include <hesp/math/geom/GeomUtil.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
/**
Constructs the lightmap grid for a polygon and calculates the lightmap coordinates for the polygon's vertices.

@param poly						The polygon
@param vertexLightmapCoords		Used to return the vertex lightmap coordinates to the caller
*/
template <typename Vert, typename AuxData>
LightmapGrid::LightmapGrid(const Polygon<Vert,AuxData>& poly, std::vector<TexCoords>& vertexLightmapCoords)
:	m_plane(make_plane(poly))
{
	AxisPlane bestAxisPlane = find_best_axis_plane(poly.normal());
	std::vector<Vector2d> projectedVertices = project_vertices_onto(poly, bestAxisPlane);
	make_planar_grid(projectedVertices, bestAxisPlane, vertexLightmapCoords);
	project_grid_onto_polygon(poly, bestAxisPlane);
}

//#################### PRIVATE METHODS ####################
/**
Project the planar grid (in the specified axis plane) onto the polygon.

@param poly			The polygon
@param axisPlane	The axis plane in which the planar grid lies
*/
template <typename Vert, typename AuxData>
void LightmapGrid::project_grid_onto_polygon(const Polygon<Vert,AuxData>& poly, AxisPlane axisPlane)
{
	Plane plane = make_plane(poly);

	Vector3d dir;
	switch(axisPlane)
	{
		case YZ_PLANE:
			dir = Vector3d(1,0,0);
			break;
		case XZ_PLANE:
			dir = Vector3d(0,1,0);
			break;
		default:	// case XY_PLANE
			dir = Vector3d(0,0,1);
			break;
	}

	int rows = static_cast<int>(m_grid.size());
	int cols = static_cast<int>(m_grid[0].size());

	for(int i=0; i<rows; ++i)
	{
		for(int j=0; j<cols; ++j)
		{
			Vector3d& s = m_grid[i][j].position;
			s = determine_line_intersection_with_plane(s, dir, plane).first;
			m_grid[i][j].withinPolygon = point_in_polygon(s, poly);
		}
	}
}

/**
Project the polygon vertices onto a given axis.

@param poly			The polygon
@param axisPlane	The axis plane
@return				The projected vertices
*/
template <typename Vert, typename AuxData>
std::vector<Vector2d> LightmapGrid::project_vertices_onto(const Polygon<Vert,AuxData>& poly, AxisPlane axisPlane)
{
	int vertCount = poly.vertex_count();
	std::vector<Vector2d> projectedVertices;
	projectedVertices.reserve(vertCount);
	for(int i=0; i<vertCount; ++i)
	{
		projectedVertices.push_back(project_vertex_onto(Vector3d(poly.vertex(i)), axisPlane));
	}
	return projectedVertices;
}

}
