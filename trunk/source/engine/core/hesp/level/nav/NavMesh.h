/***
 * hesperus: NavMesh.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_NAVMESH
#define H_HESP_NAVMESH

#include <map>
#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class NavLink> NavLink_Ptr;
typedef shared_ptr<class NavPolygon> NavPolygon_Ptr;

class NavMesh
{
	//#################### TYPEDEFS ####################
private:
	typedef std::vector<NavLink_Ptr> NavLinkVector;
	typedef std::vector<NavPolygon_Ptr> NavPolyVector;

	//#################### PRIVATE VARIABLES ####################
private:
	NavLinkVector m_links;
	NavPolyVector m_polygons;
	std::map<int,int> m_colToNavLookup;

	//#################### CONSTRUCTORS ####################
public:
	NavMesh(const NavPolyVector& polygons, const NavLinkVector& links);

	//#################### PUBLIC METHODS ####################
public:
	const NavLinkVector& links() const;
	int lookup_nav_poly_index(int collisionPolyIndex) const;
	const NavPolyVector& polygons() const;

	//#################### PRIVATE METHODS ####################
private:
	void build_collision_to_nav_lookup();
};

//#################### TYPEDEFS ####################
typedef shared_ptr<NavMesh> NavMesh_Ptr;
typedef shared_ptr<const NavMesh> NavMesh_CPtr;

}

#endif
