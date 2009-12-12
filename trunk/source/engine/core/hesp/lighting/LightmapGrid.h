/***
 * hesperus: LightmapGrid.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_LIGHTMAPGRID
#define H_HESP_LIGHTMAPGRID

#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <hesp/math/geom/Polygon.h>
#include <hesp/math/geom/Plane.h>
#include <hesp/math/vectors/TexCoords.h>
#include <hesp/math/vectors/Vector2d.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class BSPTree> BSPTree_Ptr;
struct Light;
typedef shared_ptr<class Lightmap> Lightmap_Ptr;

class LightmapGrid
{
	//#################### ENUMERATIONS ####################
private:
	enum AxisPlane
	{
		YZ_PLANE, XZ_PLANE, XY_PLANE
	};

	//#################### NESTED CLASSES ####################
private:
	struct GridPoint
	{
		Vector3d position;		// where does the grid point lie in world space?
		bool withinPolygon;		// is it within the polygon being lightmapped? (if not, we can skip some work when constructing a lightmap)

		GridPoint(const Vector3d& position_) : position(position_), withinPolygon(true) {}
	};

	//#################### PRIVATE VARIABLES ####################
private:
	std::vector<std::vector<GridPoint> > m_grid;
	Plane m_plane;

	//#################### CONSTRUCTORS ####################
public:
	template <typename Vert, typename AuxData>
	LightmapGrid(const Polygon<Vert,AuxData>& poly, std::vector<TexCoords>& vertexLightmapCoords);

	//#################### PUBLIC METHODS ####################
public:
	Lightmap_Ptr lightmap_from_light(const Light& light, const BSPTree_Ptr& tree) const;
	int lightmap_height() const;
	int lightmap_width() const;

	//#################### PRIVATE METHODS ####################
private:
	static AxisPlane find_best_axis_plane(const Vector3d& n);

	void make_planar_grid(const std::vector<Vector2d>& projectedVertices, AxisPlane axisPlane, std::vector<TexCoords>& vertexLightmapCoords);

	static Vector3d planar_to_real(const Vector2d& v, AxisPlane axisPlane);

	template <typename Vert, typename AuxData>
	void project_grid_onto_polygon(const Polygon<Vert,AuxData>& poly, AxisPlane axisPlane);

	static Vector2d project_vertex_onto(const Vector3d& v, AxisPlane axisPlane);

	template <typename Vert, typename AuxData>
	static std::vector<Vector2d> project_vertices_onto(const Polygon<Vert,AuxData>& poly, AxisPlane axisPlane);
};

//#################### TYPEDEFS ####################
typedef shared_ptr<LightmapGrid> LightmapGrid_Ptr;
typedef shared_ptr<const LightmapGrid> LightmapGrid_CPtr;

}

#include "LightmapGrid.tpp"

#endif
