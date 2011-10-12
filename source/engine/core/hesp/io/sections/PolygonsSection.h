/***
 * hesperus: PolygonsSection.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_POLYGONSSECTION
#define H_HESP_POLYGONSSECTION

#include <iosfwd>
#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

struct PolygonsSection
{
	//#################### LOADING METHODS ####################
	template <typename Poly> static void load(std::istream& is, const std::string& sectionName, std::vector<shared_ptr<Poly> >& polygons);

	//#################### SAVING METHODS ####################
	template <typename Poly> static void save(std::ostream& os, const std::string& sectionName, const std::vector<shared_ptr<Poly> >& polygons);
};

}

#include "PolygonsSection.tpp"

#endif
