/***
 * hesperus: NavPolygon.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "NavPolygon.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
NavPolygon::NavPolygon(int colPolyIndex)
:	m_colPolyIndex(colPolyIndex)
{}

//#################### PUBLIC METHODS ####################
void NavPolygon::add_in_link(int linkIndex)					{ m_inLinks.push_back(linkIndex); }
void NavPolygon::add_out_link(int linkIndex)				{ m_outLinks.push_back(linkIndex); }
int NavPolygon::collision_poly_index() const				{ return m_colPolyIndex; }
const std::vector<int>& NavPolygon::in_links() const		{ return m_inLinks; }
const std::vector<int>& NavPolygon::out_links() const		{ return m_outLinks; }

}
