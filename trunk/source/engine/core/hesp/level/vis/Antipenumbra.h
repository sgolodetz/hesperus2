/***
 * hesperus: Antipenumbra.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_ANTIPENUMBRA
#define H_HESP_ANTIPENUMBRA

#include <vector>

#include <hesp/level/portals/Portal.h>
#include <hesp/math/geom/Plane.h>
#include <hesp/math/vectors/Vector3.h>

namespace hesp {

class Antipenumbra
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::vector<Plane> m_planes;

	//#################### CONSTRUCTORS ####################
public:
	Antipenumbra(const Portal_Ptr& source, const Portal_Ptr& target);

	//#################### PUBLIC METHODS ####################
public:
	template <typename Vert, typename AuxData>
	shared_ptr<Polygon<Vert,AuxData> > clip(const shared_ptr<Polygon<Vert,AuxData> >& poly);

	//#################### PRIVATE METHODS ####################
private:
	void add_clip_planes(const Portal_CPtr& from, const Portal_CPtr& to, PlaneClassifier desiredFromClassifier);
	static Plane_Ptr construct_clip_plane(const Vector3d& a, const Vector3d& b, const Vector3d& c);
};

}

#include "Antipenumbra.tpp"

#endif
