/***
 * hesperus: GeomUtil.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_GEOMUTIL
#define H_HESP_GEOMUTIL

#include <list>
#include <utility>

#include <boost/optional.hpp>

#include "AABB.h"
#include "LineSegment.h"
#include "Plane.h"
#include "Polygon.h"

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
class Sphere;

//#################### CLASSES ####################
template <typename Vert, typename AuxData>
struct SplitResults
{
	typedef Polygon<Vert,AuxData> Poly;
	typedef shared_ptr<Poly> Poly_Ptr;

	Poly_Ptr back;
	Poly_Ptr front;

	SplitResults(const Poly_Ptr& back_, const Poly_Ptr& front_) : back(back_), front(front_) {}
};

//#################### GLOBAL FUNCTIONS ####################
PlaneClassifier classify_linesegment_against_plane(const Vector3d& p1, const Vector3d& p2, const Plane& plane, PlaneClassifier& cp1, PlaneClassifier& cp2);

PlaneClassifier classify_point_against_plane(const Vector3d& p, const Plane& plane);

template <typename Vert, typename AuxData>
PlaneClassifier classify_polygon_against_plane(const Polygon<Vert,AuxData>& poly, const Plane& plane);

template <typename Vert, typename AuxData>
AABB3d construct_bounding_box(const std::vector<shared_ptr<Polygon<Vert,AuxData> > >& polys);

template <typename T>
boost::optional<std::pair<Vector3d,Vector3d> > determine_halfray_intersection_with_shape(const Vector3d& s, const Vector3d& v, const T& shape);

boost::optional<Vector3d> determine_linesegment_intersection_with_nonvertical_linesegment(const LineSegment3d& segment, const LineSegment3d& nvSegment);

template <typename Vec>
std::pair<Vec,double> determine_line_intersection_with_plane(const Vec& s, const Vec& v, const Plane& plane);

template <typename Vec>
std::pair<Vec,bool> determine_linesegment_intersection_with_plane(const Vec& p1, const Vec& p2, const Plane& plane, bool strict = true);

boost::optional<std::pair<double,double> > determine_ray_intersection_parameters(const Vector3d& s, const Vector3d& v, const AABB3d& aabb);

boost::optional<std::pair<double,double> > determine_ray_intersection_parameters(const Vector3d& s, const Vector3d& v, const Sphere& sphere);

double displacement_from_plane(const Vector3d& p, const Plane& plane);

double distance_to_plane(const Vector3d& p, const Plane& plane);

std::list<Plane_CPtr> find_unique_planes(const std::list<Plane_CPtr>& planes);

Vector3d generate_arbitrary_coplanar_unit_vector(const Plane& plane);

Vector3d generate_specific_coplanar_unit_vector(const Plane& plane);

Plane_Ptr make_axial_plane(const Vector3d& p1, const Vector3d& p2, const Vector3d& axis);

template <typename Vert, typename AuxData>
Plane make_plane(const Polygon<Vert,AuxData>& poly);

template <typename AuxData>
shared_ptr<Polygon<Vector3d,AuxData> > make_universe_polygon(const Plane& plane, const AuxData& auxData = AuxData());

Vector3d nearest_point_in_plane(const Vector3d& p, const Plane& plane);

template <typename Vert, typename AuxData>
bool point_in_polygon(const Vector3d& p, const Polygon<Vert,AuxData>& poly);

Vector3d project_vector_onto_plane(const Vector3d& v, const Plane& plane);

template <typename Vert, typename AuxData>
SplitResults<Vert,AuxData> split_polygon(const Polygon<Vert,AuxData>& poly, const Plane& plane);

//################## HELPER METHODS FOR THE split_polygon FUNCTION ##################
int next_vert(int index, int vertCount);

int prev_vert(int index, int vertCount);

}

#include "GeomUtil.tpp"

#endif
