/***
 * hesperus: BrushesFile.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_BRUSHESFILE
#define H_HESP_BRUSHESFILE

#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <hesp/level/brushes/PolyhedralBrush.h>

namespace hesp {

struct BrushesFile
{
	//#################### LOADING METHODS ####################
	template <typename Poly> static std::vector<shared_ptr<PolyhedralBrush<Poly> > > load(const std::string& filename);

	//#################### SAVING METHODS ####################
	template <typename Poly> static void save(const std::string& filename, const std::vector<shared_ptr<PolyhedralBrush<Poly> > >& brushes);
};

}

#include "BrushesFile.tpp"

#endif
