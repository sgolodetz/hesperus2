/***
 * hesperus: IOUtil.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_IOUTIL
#define H_HESP_IOUTIL

#include <hesp/level/brushes/PolyhedralBrush.h>
#include <hesp/math/geom/Polygon.h>

namespace hesp {

struct IOUtil
{
	//#################### READING METHODS ####################
	template <typename Poly> static void read_counted_polygons(std::istream& is, std::vector<shared_ptr<Poly> >& polygons);
	template <typename Vert, typename AuxData> static shared_ptr<Polygon<Vert,AuxData> > read_polygon(const std::string& line, const std::string& n = "");
	template <typename Vert, typename AuxData> static void read_polygons(std::istream& is, std::vector<shared_ptr<Polygon<Vert,AuxData> > >& polygons, int maxToRead);
	template <typename Poly> static shared_ptr<PolyhedralBrush<Poly> > read_polyhedral_brush(std::istream& is);
	template <typename Poly> static void read_uncounted_polygons(std::istream& is, std::vector<shared_ptr<Poly> >& polygons);

	//#################### WRITING METHODS ####################
	template <typename Vert, typename AuxData> static void write_polygons(std::ostream& os, const std::vector<shared_ptr<Polygon<Vert,AuxData> > >& polygons, bool writeCount);
	template <typename Poly> static void write_polyhedral_brush(std::ostream& os, const PolyhedralBrush<Poly>& brush);
};

}

#include "IOUtil.tpp"

#endif
