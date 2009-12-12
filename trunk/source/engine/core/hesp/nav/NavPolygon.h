/***
 * hesperus: NavPolygon.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_NAVPOLYGON
#define H_HESP_NAVPOLYGON

#include <vector>

#include <hesp/util/PolygonTypes.h>

namespace hesp {

class NavPolygon
{
	//#################### PRIVATE VARIABLES ####################
private:
	int m_colPolyIndex;					// the index of the collision polygon on which this is based
	std::vector<int> m_inLinks;			// indices of navigation links from other polygons
	std::vector<int> m_outLinks;		// indices of navigation links to other polygons

	//#################### CONSTRUCTORS ####################
public:
	explicit NavPolygon(int colPolyIndex);

	//#################### PUBLIC METHODS ####################
public:
	void add_in_link(int linkIndex);
	void add_out_link(int linkIndex);
	int collision_poly_index() const;
	const std::vector<int>& in_links() const;
	const std::vector<int>& out_links() const;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<NavPolygon> NavPolygon_Ptr;
typedef shared_ptr<const NavPolygon> NavPolygon_CPtr;

}

#endif
