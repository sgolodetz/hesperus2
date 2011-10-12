/***
 * hesperus: TexCoords.h
 * Copyright Stuart Golodetz, 2008. All rights reserved.
 ***/

#ifndef H_HESP_TEXCOORDS
#define H_HESP_TEXCOORDS

namespace hesp {

struct TexCoords
{
	//#################### PUBLIC VARIABLES ####################
	double u, v;

	//#################### CONSTRUCTORS ####################
	TexCoords(double u_, double v_) : u(u_), v(v_) {}
};

}

#endif
