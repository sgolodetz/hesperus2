/***
 * hesperus: Lightmap.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_LIGHTMAP
#define H_HESP_LIGHTMAP

#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <hesp/colours/Colour3d.h>
#include <hesp/images/Image.h>

namespace hesp {

class Lightmap
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::vector<std::vector<Colour3d> > m_lumels;
	int m_rows, m_cols;

	//#################### CONSTRUCTORS ####################
public:
	Lightmap(int rows, int cols, const Colour3d& initial = Colour3d());

	//#################### PUBLIC OPERATORS ####################
public:
	Colour3d& operator()(int row, int col);
	const Colour3d& operator()(int row, int col) const;
	Lightmap& operator+=(const Lightmap& rhs);

	//#################### PUBLIC METHODS ####################
public:
	Image24_Ptr to_image() const;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<Lightmap> Lightmap_Ptr;
typedef shared_ptr<const Lightmap> Lightmap_CPtr;

}

#endif
